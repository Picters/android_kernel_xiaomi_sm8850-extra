// SPDX-License-Identifier: GPL-2.0
/*
 * Picters performance caps — hard, in-kernel CPU frequency ceilings.
 *
 * The vendor perf HAL owns scaling_max_freq and rewrites it on load and on
 * thermal events, so a userspace cap only holds while something keeps
 * re-writing it. A FREQ_QOS_MAX request is the same lever thermal cooling
 * devices use and cpufreq_set_policy() aggregates every request with a min(),
 * so a cap placed here cannot be raised by anything userspace does — including
 * perfd. No polling loop, no fight, and the ceiling holds through suspend.
 *
 * Purely additive: no exported symbol and no struct in the frozen GKI KMI is
 * touched, so stock vendor modules keep loading unchanged.
 *
 * Interface (/sys/kernel/picters_perf):
 *   available     r  "<first-cpu>:<cpuinfo_max_freq_khz>" per cpufreq policy
 *   cpu_max_freq  rw "<first-cpu>:<khz>" pairs; khz 0 drops that policy's cap
 *   profile       rw free-form label the app round-trips (kernel never reads it)
 */

#include <linux/cpufreq.h>
#include <linux/cpumask.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/kobject.h>
#include <linux/minmax.h>
#include <linux/mutex.h>
#include <linux/notifier.h>
#include <linux/pm_qos.h>
#include <linux/slab.h>
#include <linux/string.h>
#include <linux/sysfs.h>

#define PICTERS_PROFILE_LEN 24

struct picters_cap {
	struct freq_qos_request req;
	unsigned int khz;	/* requested ceiling, 0 = uncapped */
	bool active;		/* req is currently registered on a live policy */
};

static DEFINE_MUTEX(picters_lock);
static struct picters_cap *picters_caps;	/* indexed by first related CPU (stable across hotplug) */
static char picters_profile[PICTERS_PROFILE_LEN] = "full";
static struct kobject *picters_kobj;

/* Drop a cap whose policy is still alive. Caller holds picters_lock. */
static void picters_drop_locked(struct picters_cap *c)
{
	if (c->active) {
		freq_qos_remove_request(&c->req);
		c->active = false;
	}
}

/*
 * Register/update/remove the FREQ_QOS_MAX request for the policy [cpu] leads.
 * [khz] is clamped into the policy's own range, so a caller can never ask for
 * a frequency the hardware doesn't have — and never for one above stock.
 */
static int picters_set_cap(unsigned int cpu, unsigned int khz)
{
	struct cpufreq_policy *policy;
	struct picters_cap *c;
	int ret;

	if (cpu >= nr_cpu_ids)
		return -EINVAL;
	policy = cpufreq_cpu_get(cpu);
	if (!policy)
		return -ENODEV;
	c = &picters_caps[cpumask_first(policy->related_cpus)];

	mutex_lock(&picters_lock);

	if (!khz) {
		picters_drop_locked(c);
		c->khz = 0;
		ret = 0;
		goto out;
	}

	khz = clamp(khz, policy->cpuinfo.min_freq, policy->cpuinfo.max_freq);

	if (c->active) {
		ret = freq_qos_update_request(&c->req, khz);
	} else {
		ret = freq_qos_add_request(&policy->constraints, &c->req,
					   FREQ_QOS_MAX, khz);
		if (ret >= 0)
			c->active = true;
	}
	if (ret < 0)
		goto out;

	c->khz = khz;
	ret = 0;
out:
	mutex_unlock(&picters_lock);
	cpufreq_cpu_put(policy);
	return ret;
}

/*
 * A policy can be torn down and rebuilt under us (cluster hotplug, driver
 * reload). Our request lives in that policy's freq_constraints, so it must go
 * with it — and come back when the policy does, or the cap would silently
 * vanish. The QoS max notifier only schedules work, so nothing here can
 * deadlock against the policy rwsem cpufreq holds across these callbacks.
 */
static int picters_policy_notifier(struct notifier_block *nb,
				   unsigned long event, void *data)
{
	struct cpufreq_policy *policy = data;
	struct picters_cap *c;
	int ret;

	if (!policy || policy->cpu >= nr_cpu_ids)
		return NOTIFY_DONE;
	c = &picters_caps[cpumask_first(policy->related_cpus)];

	mutex_lock(&picters_lock);
	switch (event) {
	case CPUFREQ_CREATE_POLICY:
		if (c->khz && !c->active) {
			ret = freq_qos_add_request(&policy->constraints,
						   &c->req, FREQ_QOS_MAX,
						   c->khz);
			if (ret >= 0)
				c->active = true;
		}
		break;
	case CPUFREQ_REMOVE_POLICY:
		picters_drop_locked(c);
		break;
	}
	mutex_unlock(&picters_lock);

	return NOTIFY_OK;
}

static struct notifier_block picters_policy_nb = {
	.notifier_call = picters_policy_notifier,
};

static ssize_t available_show(struct kobject *kobj,
			      struct kobj_attribute *attr, char *buf)
{
	struct cpufreq_policy *policy;
	unsigned int cpu;
	int len = 0;

	for_each_possible_cpu(cpu) {
		policy = cpufreq_cpu_get(cpu);
		if (!policy)
			continue;
		/* One line per policy, keyed by the CPU that leads it. */
		if (cpumask_first(policy->related_cpus) == cpu)
			len += sysfs_emit_at(buf, len, "%u:%u\n", cpu,
					     policy->cpuinfo.max_freq);
		cpufreq_cpu_put(policy);
	}
	return len;
}

static ssize_t cpu_max_freq_show(struct kobject *kobj,
				 struct kobj_attribute *attr, char *buf)
{
	unsigned int cpu;
	int len = 0;

	mutex_lock(&picters_lock);
	for_each_possible_cpu(cpu) {
		if (picters_caps[cpu].khz)
			len += sysfs_emit_at(buf, len, "%u:%u\n", cpu,
					     picters_caps[cpu].khz);
	}
	mutex_unlock(&picters_lock);
	return len;
}

/*
 * Accepts any number of "<cpu>:<khz>" pairs on one write, separated by
 * whitespace or commas — the app applies a whole profile in a single echo.
 * A pair naming a CPU with no cpufreq policy is skipped rather than failing
 * the write, so one absent cluster can't drop the caps for the others.
 */
static ssize_t cpu_max_freq_store(struct kobject *kobj,
				  struct kobj_attribute *attr,
				  const char *buf, size_t count)
{
	const char *p = buf;
	unsigned int applied = 0;

	while (*p) {
		unsigned int cpu, khz;
		int ret, consumed = 0;

		while (*p == ' ' || *p == '\t' || *p == '\n' || *p == ',')
			p++;
		if (!*p)
			break;
		if (sscanf(p, "%u:%u%n", &cpu, &khz, &consumed) != 2)
			return -EINVAL;
		p += consumed;
		if (*p && *p != ' ' && *p != '\t' &&
		    *p != '\n' && *p != ',')
			return -EINVAL;

		ret = picters_set_cap(cpu, khz);
		if (ret && ret != -ENODEV)
			return ret;
		if (!ret)
			applied++;
	}

	return applied ? count : -ENODEV;
}

static ssize_t profile_show(struct kobject *kobj, struct kobj_attribute *attr,
			    char *buf)
{
	ssize_t len;

	mutex_lock(&picters_lock);
	len = sysfs_emit(buf, "%s\n", picters_profile);
	mutex_unlock(&picters_lock);
	return len;
}

static ssize_t profile_store(struct kobject *kobj, struct kobj_attribute *attr,
			     const char *buf, size_t count)
{
	char tmp[PICTERS_PROFILE_LEN];

	strscpy(tmp, buf, sizeof(tmp));
	mutex_lock(&picters_lock);
	strscpy(picters_profile, strim(tmp), sizeof(picters_profile));
	mutex_unlock(&picters_lock);
	return count;
}

static struct kobj_attribute available_attr = __ATTR_RO(available);
static struct kobj_attribute cpu_max_freq_attr = __ATTR_RW(cpu_max_freq);
static struct kobj_attribute profile_attr = __ATTR_RW(profile);

static struct attribute *picters_attrs[] = {
	&available_attr.attr,
	&cpu_max_freq_attr.attr,
	&profile_attr.attr,
	NULL,
};
ATTRIBUTE_GROUPS(picters);

static int __init picters_perf_init(void)
{
	int ret;

	picters_caps = kcalloc(nr_cpu_ids, sizeof(*picters_caps), GFP_KERNEL);
	if (!picters_caps)
		return -ENOMEM;

	picters_kobj = kobject_create_and_add("picters_perf", kernel_kobj);
	if (!picters_kobj) {
		ret = -ENOMEM;
		goto err_free;
	}

	ret = sysfs_create_groups(picters_kobj, picters_groups);
	if (ret)
		goto err_kobj;

	ret = cpufreq_register_notifier(&picters_policy_nb,
					CPUFREQ_POLICY_NOTIFIER);
	if (ret)
		goto err_sysfs;

	return 0;

err_sysfs:
	sysfs_remove_groups(picters_kobj, picters_groups);
err_kobj:
	kobject_put(picters_kobj);
err_free:
	kfree(picters_caps);
	picters_caps = NULL;
	return ret;
}

/* late_initcall so every cpufreq policy already exists when we come up. */
late_initcall(picters_perf_init);

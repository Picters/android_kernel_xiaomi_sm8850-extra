<div align="center">

<img src="assets/logo.jpg" alt="Picters kernel" width="640">

# Picters Kernel — Xiaomi SM8850

A custom Android 16 kernel for the Xiaomi Mi 17 (sm8850, "Pudding"), focused on first-class support for external USB Wi-Fi. 📡

`6.12.23-android16` / `6.12.69-android17` &nbsp;•&nbsp; ReSukiSU (KernelSU) &nbsp;•&nbsp; SUSFS &nbsp;•&nbsp; out-of-tree USB Wi-Fi

</div>

---

## Overview

Picters Kernel extends the stock Android 16 kernel with proper support for external USB Wi-Fi adapters and a set of common out-of-tree drivers, all managed from a dedicated companion app.

- **External Wi-Fi adapters.** Realtek RTL8812AU / 8812BU / 8814AU / 8188EUS adapters work out of the box — for packet injection and monitor mode, and as a standard managed station inside stock Xiaomi Settings.
- **Per-adapter control.** Tap a connected adapter in the app to switch it between **monitor** and **managed** mode, bring it **up / down**, and set its **tx power** — with a safe, recommended value per chipset (capped at the chip's physical limit). No `iw` commands needed.
- **Bundled drivers.** Realtek Wi-Fi (aircrack-ng and morrownr), CAN bus, DVB-T / RTL-SDR and USB-serial are built in — no manual compilation required.
- **Root.** ReSukiSU (KernelSU) with SUSFS.
- **Companion app.** *Picters Modules Manager* ships with the kernel: switch Wi-Fi between Stock and Inject live (no reboot), hand an adapter back to Android, and open the latest GitHub release when an update is available.

<div align="center">
<img src="assets/manager.jpg" width="43%" alt="Picters Modules Manager with two adapters loaded">
&nbsp;&nbsp;
<img src="assets/iwdev.jpg" width="43%" alt="Two adapters in monitor mode via iw dev">
<br>
<sub>Left: the manager app with two adapters loaded. Right: both adapters in monitor mode on this kernel.</sub>
</div>

---

## Installation

| Channel | Kernel | Matching modules |
| --- | --- | --- |
| A16 · KMI 5 | `Mi17_Kernel-6.12.23-android16-…zip` | `Mi17_OOTMODULES-6.12.23-android16-…zip` |
| A17 · KMI 6 | `Mi17_Kernel-6.12.69-android17-…zip` | `Mi17_OOTMODULES-6.12.69-android17-…zip` |

Install only the pair for your Android version; the A17 kernel and app have not been tested on Android 17 firmware.

1. Download the latest build from the [Releases](../../releases) page. Each build ships two archives:
   - `Mi17_Kernel-…zip` — the kernel (AnyKernel3).
   - `Mi17_OOTMODULES-…zip` — the drivers and the companion app.
2. Flash the kernel with an AnyKernel3-compatible installer and boot it, then install the matching OOTMODULES pack in KernelSU or Magisk and reboot.
3. The manager ships as a **system app** — enable **Show system apps** in your KernelSU/Magisk manager to find *Picters Modules Manager* and grant it root.
4. Open it, set Wi-Fi to **Inject**, and connect an adapter.

> 💡 The blue Update chip opens the latest GitHub release. Download and install updates manually.

---

## Details

| | |
|---|---|
| **Base** | A16: Linux 6.12.23 / KMI 5 · A17: Linux 6.12.69 / KMI 6 · Xiaomi sm8850 |
| **Root** | ReSukiSU (KernelSU) + SUSFS |
| **Wi-Fi injection** | `88XXau` (RTL8812AU), `88x2bu` (RTL8812BU), `8814au`, `8188eus` — patched for Linux 6.12 (no UBSAN panics, correct cfg80211 hand-off) |
| **Additional drivers** | CAN, DVB-T / RTL-SDR, USB-serial (CP210x / CH341 / FTDI / PL2303) |
| **Build** | Continuous integration; each release includes the kernel and matching modules |

---

## Credits

ReSukiSU / KernelSU · SUSFS · [aircrack-ng](https://github.com/aircrack-ng/rtl8812au) · [morrownr](https://github.com/morrownr) · AnyKernel3 (osm0sis) · [YuzakiKokuban](https://github.com/YuzakiKokuban) for the build tooling.

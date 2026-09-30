## Релизы и совместимость

| Канал | База | Ядро | Модули и менеджер |
| --- | --- | --- | --- |
| **A16 · `android16`** | Linux 6.12.23 / KMI 5 | `A16-Kernel.zip` | `A16-OOTMODULES.zip` |
| **A17 · `android17`** | Linux 6.12.69 / KMI 6, экспериментальный | `A17-Kernel.zip` | `A17-OOTMODULES.zip` |

Оба канала используют только ReSukiSU. Поддержка Android 17 новой базой не подтверждена: на Android 16 / OS3.0.315.0.WPCCNXM она не загрузилась. Совместимость определяется также vendor-модулями, а не только версией Android.

**Скачивайте оба архива из одного релиза своего канала.** Установите ядро и загрузите телефон, затем установите соответствующий OOT-пакет через KernelSU/Magisk и перезагрузитесь. Пакет проверяет точную строку ядра; его boot-service не загружает драйверы при смене ядра.

В каждом OOT-пакете находится подписанный Picters Modules Manager **1.3.2** с фиксами частот. Менеджер только показывает новые релизы и открывает GitHub: скачивание, установка APK/ZIP и прошивка ядра из приложения удалены. Старый менеджер 1.3.1 новые имена архивов не распознаёт.

Workflow собирает каждый канал отдельно и сохраняет архивы, SHA-256, отчёт KMI и описание установки в артефактах. Релизы публикуются только после отдельного разрешения и проверки совместимости.

<div align="center">

<img src="assets/logo.jpg" alt="Picters kernel" width="640">

# Picters Kernel — Xiaomi SM8850

A custom Android 16 kernel for the Xiaomi Mi 17 (sm8850, "Pudding"), focused on first-class support for external USB Wi-Fi. 📡

`6.12.69-android16` &nbsp;•&nbsp; ReSukiSU (KernelSU) &nbsp;•&nbsp; SUSFS &nbsp;•&nbsp; out-of-tree USB Wi-Fi

</div>

---

## Overview

Picters Kernel extends the stock Android 16 kernel with proper support for external USB Wi-Fi adapters and a set of common out-of-tree drivers, all managed from a dedicated companion app.

- **External Wi-Fi adapters.** Realtek RTL8812AU / 8812BU / 8814AU / 8188EUS adapters work out of the box — for packet injection and monitor mode, and as a standard managed station inside stock Xiaomi Settings.
- **Per-adapter control.** Tap a connected adapter in the app to switch it between **monitor** and **managed** mode, bring it **up / down**, and set its **tx power** — with a safe, recommended value per chipset (capped at the chip's physical limit). No `iw` commands needed.
- **Bundled drivers.** Realtek Wi-Fi (aircrack-ng and morrownr), CAN bus, DVB-T / RTL-SDR and USB-serial are built in — no manual compilation required.
- **Root.** ReSukiSU (KernelSU) with SUSFS.
- **Companion app.** *Picters Modules Manager* ships with the kernel: switch Wi-Fi between Stock and Inject live (no reboot), hand an adapter back to Android, and keep the kernel, modules and app up to date.

<div align="center">
<img src="assets/manager.jpg" width="43%" alt="Picters Modules Manager with two adapters loaded">
&nbsp;&nbsp;
<img src="assets/iwdev.jpg" width="43%" alt="Two adapters in monitor mode via iw dev">
<br>
<sub>Left: the manager app with two adapters loaded. Right: both adapters in monitor mode on this kernel.</sub>
</div>

---

## Installation

1. Download the latest build from the [Releases](../../releases) page. Each build ships two archives:
   - `Mi17_Kernel-…zip` — the kernel (AnyKernel3).
   - `…OOT-Modules…zip` — the drivers and the companion app.
2. Flash both in KernelSU or Magisk (or through the companion app), then reboot.
3. The manager ships as a **system app** — enable **Show system apps** in your KernelSU/Magisk manager to find *Picters Modules Manager* and grant it root.
4. Open it, set Wi-Fi to **Inject**, and connect an adapter.

> 💡 The companion app can update the kernel, the modules and itself in a single step, with an A/B slot selector.

---

## Details

| | |
|---|---|
| **Base** | Android 16 GKI · Linux 6.12.69 · Xiaomi sm8850 |
| **Root** | ReSukiSU (KernelSU) + SUSFS |
| **Wi-Fi injection** | `88XXau` (RTL8812AU), `88x2bu` (RTL8812BU), `8814au`, `8188eus` — patched for Linux 6.12 (no UBSAN panics, correct cfg80211 hand-off) |
| **Additional drivers** | CAN, DVB-T / RTL-SDR, USB-serial (CP210x / CH341 / FTDI / PL2303) |
| **Build** | Continuous integration; each release is version-stamped for in-app updates |

---

## Credits

ReSukiSU / KernelSU · SUSFS · [aircrack-ng](https://github.com/aircrack-ng/rtl8812au) · [morrownr](https://github.com/morrownr) · AnyKernel3 (osm0sis) · [YuzakiKokuban](https://github.com/YuzakiKokuban) for the build tooling.

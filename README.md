<div align="center">

<img src="assets/logo.jpg" alt="Picters Kernel" width="640">

# Picters Kernel — Xiaomi 17

Custom kernel for Xiaomi 17 (`pudding`, SM8850) with external USB Wi-Fi support.

`Android 16 / 17` • `ReSukiSU` • `SUSFS` • `OOT USB Wi-Fi`

</div>

---

## Releases

| Channel | Base | Kernel |
| --- | --- | --- |
| **A16 · `android16`** | Linux 6.12.23 / KMI 5 | `Mi17_Kernel-6.12.23-android16-…zip` |
| **A17 · `android17`** | Linux 6.12.69 / KMI 6 · experimental | `Mi17_Kernel-6.12.69-android17-…zip` |

**Install strictly for your Android version: A16 for Android 16, A17 for Android 17. Never mix channels.**

| Your Android version | Download the matching pair |
| --- | --- |
| **Android 16 only** | [A16 — Kernel + OOTMODULES](https://github.com/Picters/android_kernel_xiaomi_sm8850-extra/releases/tag/A16-20261003-0027) |
| **Android 17 only — Kernel not tested** | [A17 — Kernel + OOTMODULES](https://github.com/Picters/android_kernel_xiaomi_sm8850-extra/releases/tag/A17-20261003-0027) |

**A17 Kernel not tested. The Android 17 kernel and bundled Picters Modules Manager 1.3.2 have not been tested on Android 17 firmware. Android 17 vendor-module CRC compatibility has not been verified. Booting and app/module functionality on A17 are not confirmed.**

The A16 kernel has been boot-tested on Xiaomi 17 with HyperOS OS3.0.315.0.WPCCNXM (Android 16). The A17 base did not boot on that Android 16 firmware; do not install A17 on A16.

Each release contains only its own kernel and matching OOT modules, including the signed manager APK.

---

## Features

- ReSukiSU / KernelSU + SUSFS
- RTL8812AU / RTL8812BU / RTL8814AU / RTL8188EUS
- Monitor mode and packet injection
- Managed Wi-Fi mode in Android
- CAN, DVB-T / RTL-SDR and USB-serial drivers
- Picters Modules Manager
- Per-adapter mode, power and interface controls

---

## Installation

1. Check your Android version in Settings. Open the **A16 release for Android 16** or **A17 release for Android 17**.
2. Download `Mi17_Kernel-…zip` and the matching `Mi17_OOTMODULES-…zip` from that same release.
3. Flash the kernel and reboot.
4. Install the OOT package through KernelSU/Magisk.
5. Reboot again.

The installers reject the wrong Android version before proceeding. The OOT package also checks the exact running kernel version and will not load drivers with an incompatible kernel. A matching Android version does not prove A17 vendor compatibility.

---

## Modules Manager

Each OOT package includes **Picters Modules Manager 1.3.2**.

It can:

- switch Wi-Fi between Stock and Inject modes;
- control connected USB Wi-Fi adapters;
- show available releases and open GitHub.

Kernel flashing and automatic APK/ZIP installation were removed from the app.

---

## Credits

ReSukiSU / KernelSU · SUSFS · aircrack-ng · morrownr · AnyKernel3 · YuzakiKokuban

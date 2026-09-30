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

Download the **kernel and OOT modules from the same release/channel**.

Android 17 compatibility is experimental and also depends on Xiaomi vendor modules.

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

1. Download `Mi17_Kernel-…zip` and the matching `Mi17_OOTMODULES-…zip`.
2. Flash the kernel and reboot.
3. Install the OOT package through KernelSU/Magisk.
4. Reboot again.

The OOT package checks the exact kernel version and will not load drivers with an incompatible kernel.

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

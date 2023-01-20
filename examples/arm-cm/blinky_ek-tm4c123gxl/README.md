> **NOTE**
This file is best viewed in a **markdown viewer**, such as the one built into GitHub. Markdown viewers are also available as plug-ins to popular Internet browsers.

# Blinky on EK-TM4C123GXL
This example implements the simple [Blinky application](https://www.state-machine.com/qpc/tut_blinky.html) on the EK-TM4C123GLX board (ARM Cortex-M4F). The example blinks the LED
on the EK-TM4C123GXL board at 1Hz (0.5 seconds on, 0.5 seconds off).

<p align="center">
<img src="./ek-tm4c123gxl.jpg"/><br>
<b>EK-TM4C123GXL (TivaC LaunchPad)</b>
</p>

## Features Demonstrated
- cooperative QV kernel
  + with ARM-KEIL toolchain
  + with GNU-ARM toolchain
  + with IAR-ARM toolchain
- preemptive run-to-completion QK kernel
  + with ARM-KEIL toolchain
  + with GNU-ARM toolchain
  + with IAR-ARM toolchain

## Build Configurations
- Debug
- Release
- Spy - NOT supported for the simple Blinky example

# Code Organization
```
examples\arm-cm\blinky_ek-tm4c123gxl
|
+---qk             // preemptive QK kernel
|   +---gnu        // GNU-ARM toolchain
|   |   \---targetConfigs
|   |       Makefile          // Makefile for GNU-ARM
|   +---armclang   // ARM/KEIL toolchain with Compiler 6 (ARM/CLANG)
|   |       blinky-qk.uvprojx // uVision project
|   \---iar        // IAR EWARM
|           blinky-qk.eww     // IAR EW-ARM workspace
|
\---qv             // cooperative QK kernel
|   +---gnu        // GNU-ARM toolchain
|   |   \---targetConfigs
|   |       Makefile          // Makefile for GNU-ARM
|   +---armclang   // ARM/KEIL toolchain with Compiler 6 (ARM/CLANG)
|   |       blinky-qv.uvprojx // uVision project
|   \---iar        // IAR EWARM
|           blinky-qv.eww     // IAR EW-ARM workspace
```

# Builing the example

### GNU/ARM
- open terminal window
- change to the desired directory (either `examples\arm-cm\blinky_ek-tm4c123gxl\qk\gnu`
or `examples\arm-cm\blinky_ek-tm4c123gxl\qv\gnu`)
- to build the default Debug configuration, type:

```
make
```

> **NOTE**
The `make` utility for Windows is provided in the QTools collection for Windows.

- to build the Release configuration, type:

```
make CONF=rel
```


### ARM/KEIL MDK
- Open the provided KEIL uVision project (either `blinky-qk.uvprojx` or `blinky-qv.uvprojx`)
in Keil uVision IDE. Build/Debug/Download to the board from the IDE.
- Change the build configuration in the "Project Target" drop-down menu.


### IAR EWARM
- Open the provided IAR EWARM workspace (either `blinky-qk.eww` or `blinky-qv.eww`)
in IAR EWARM IDE. Build/Debug/Download to the board from the IDE.
- Change the build configuration in the "Project Configuration" drop-down menu.


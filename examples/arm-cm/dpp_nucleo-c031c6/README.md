> **NOTE**
This file is best viewed in a **markdown viewer**, such as the one built into GitHub. Markdown viewers are also available as plug-ins to popular Internet browsers.

# DPP on NUCLEO-C031C6
This example demonstrates the [Dining Philosophers Problem (DPP) application](https://www.state-machine.com/qpc/tut_dpp.html) on the STM32 NUCLEO-C031C6 board (ARM Cortex-M0+).

<p align="center">
<img src="./stm32-nucleo-c031c6.jpg"/><br>
<b>STM32 NUCLEO-C031C6</b>
</p>

## Features Demonstrated
- multiple cooperating active objects
- immutable (const) events
- mutable (dynamic) events
- time events
- direct event posting
- publish-subscribe event delivery
- cooperative QV kernel
  + with ARM-KEIL toolchain
  + with GNU-ARM toolchain
  + with IAR-ARM toolchain
- preemptive run-to-completion QK kernel
  + with ARM-KEIL toolchain
  + with GNU-ARM toolchain
  + with IAR-ARM toolchain
- preemptive dual-mode QXK kernel
  + with ARM-KEIL toolchain
  + with GNU-ARM toolchain
  + with IAR-ARM toolchain

## Build Configurations
- Debug
- Release
- Spy - software tracing with the built-in virtual COM port

# Code Organization
```
examples\arm-cm\dpp_nucleo-c031c6
|
+---qk             // preemptive QK kernel
|   +---gnu        // GNU-ARM toolchain
|   |   \---targetConfigs
|   |       Makefile          // Makefile for GNU-ARM
|   +---armclang   // ARM/KEIL toolchain with Compiler 6 (ARM/CLANG)
|   |       dpp-qk.uvprojx // uVision project
|   \---iar        // IAR EWARM
|           dpp-qk.eww     // IAR EW-ARM workspace
|
\---qv             // cooperative QK kernel
|   +---gnu        // GNU-ARM toolchain
|   |   \---targetConfigs
|   |       Makefile          // Makefile for GNU-ARM
|   +---armclang   // ARM/KEIL toolchain with Compiler 6 (ARM/CLANG)
|   |       dpp-qv.uvprojx // uVision project
|   \---iar        // IAR EWARM
|           dpp-qv.eww     // IAR EW-ARM workspace
|
+---qxk            // preemptive, dual-mode QXK kernel
|   +---gnu        // GNU-ARM toolchain
|   |   \---targetConfigs
|   |       Makefile          // Makefile for GNU-ARM
|   +---armclang   // ARM/KEIL toolchain with Compiler 6 (ARM/CLANG)
|   |       dpp-qk.uvprojx // uVision project
|   \---iar        // IAR EWARM
|           dpp-qk.eww     // IAR EW-ARM workspace
|
```

# Builing the example

### GNU/ARM
- open terminal window
- change to the desired directory (either `examples\arm-cm\dpp_nucleo-c031c6\qk\gnu`, `examples\arm-cm\dpp_nucleo-c031c6\qv\gnu`, or `examples\arm-cm\dpp_nucleo-c031c6\qxk\gnu`)
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

- to build the Spy configuration, type:

```
make CONF=spy
```


### ARM/KEIL MDK
- Open the provided KEIL uVision project (either `dpp-qk.uvprojx`, `dpp-qv.uvprojx`, or `dpp-qxk.uvprojx`)
in Keil uVision IDE. Build/Debug/Download to the board from the IDE.
- Change the build configuration in the "Project Target" drop-down menu.


### IAR EWARM
- Open the provided IAR EWARM workspace (either `dpp-qk.eww`, `dpp-qv.eww`, or `dpp-qxk.eww`)
in IAR EWARM IDE. Build/Debug/Download to the board from the IDE.
- Change the build configuration in the "Project Configuration" drop-down menu.


# Tracing with QP/Spy
When the board is flashed with the Spy build configuration, it produces the QP/Spy software tracing output to the built-in virtual COM port of the TivaC LauchPad board. The trace is binary rather than ASCII, and therefore requires a special host-based application called QSPY.

> **NOTE** QSPY host application is available in the QTools collection.

To launch the QSPY host application:
- open terminal window
- type:

```
qspy -c COM5
```

where "COM5" is an example virtual COM port enumerated by the board. You need to check the specific COM port number on your host computer using the Device Manager application, Ports (COM and LPT) section.


The following screen shot shows a typical output from QSPY:

<p align="center">
<img src="./qspy-output.png"/><br>
<b>Typical QSPY output produced by the Spy build configuration</b>
</p>


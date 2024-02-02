![STM32 EFM32-SLSTK3401A](../../../doxygen/images/bd_EFM32-SLSTK3401A.jpg)

Documentation for this example is available in the QP/C Manual at:

- https://www.state-machine.com/qpc/arm-cm_game_efm32-slstk3401a

# About this Example
This directory contains the "Fly 'n' Shoot" game example application for
the EFM32-SLSTK3401A board (Pearl Gecko evaluation kit) board. This
directory contains portable code that should compile with any C compiler
for ARM Cortex-M.

The sub-directories contain code and project files, which are specific to
the particular ARM toolchains, such as ARM (MDK-ARM), GCC, and IAR.

Please refer to the README files in the sub-directories for specific
instructions how to use and customize the example to your needs.


# Support Code for EFM32-SLSTK3401A Board
The directory qpc\3rd_party\efm32pg1b\ contains the CMSIS-compliant
device code for the LM3S811 MCU. Please see the README file in this
folder for more details.


# QS Software Tracing Instrumentation
The example provides the "Spy" build configuration, which outputs the
QS (Quantum Spy) software tracing data through UART0, which is connected
to the virtual COM port of the board.

The output is generated at 115200 baud rate.

Here is an example invocation of the QSPY host application to receive
the QS data from EFM32-SLSTK3401A:

```
qspy -c COM1             (Windows)

qspy -c /dev/ttyS1       (POSIX)
```

The actual serial-port number might be different on your workstation.
Please check the Device Manager to find the COM port number.


# Win32-GUI Emulation
The directory `qpc\examples\workstation\game-gui` contains the emulation
of the "Fly 'n' Shoot" game on Windows GUI, either single-threaded (win32-qv)
or multithreaded (win32). This sub-directory contains the  Visual Studio
solution file (game-gui.sln) for Visual C++.

The Win32-GUI emulation is based on the QWin™ GUI Prototyping Toolkit, see:

- https://www.state-machine.com/products/qtools#QWIN

The emulation uses exactly the same code as the embedded board and differs
only in the Board Support Package (bsp.c). This example demonstrates the
"dual targeting" development approach, where most of the embedded code is
developed on the workstation (Windows), but is intended for a deeply
embedded target (EFM32-SLSTK3401A here).


# QS Software Tracing Instrumentation
The "win32-gui" emulation also supports the "Spy" build configuration,
in which case it attempts to connect to the QSPY host application via
a TCP/IP socket (default). Here is how you launch the QSPY host utility:


```
qspy
```


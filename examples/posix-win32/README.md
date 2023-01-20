# ABOUT THE "posix-win32" EXAMPLES
The examples in the "posix-win32" directory are for the General Purpose
Operating Systems (GPOSes), such as:

- POSIX (e.g., Linux, macOS, and RTOSes with the
POSIX subsystems, like INTEGRITY, vxWorks, etc.)
- Windows (Win32 API)

The examples run in the standard terminal (as console applications)
and can be built with the standard C compilers available on the
respective operating systems, such as GCC, LLVM, Visual C++. The provided Makefiles support the GNU GCC toolchain.

> **NOTE**
For Windows, the GNU GCC toolchain (MinGW) and the 'make' utility are
provided in the QTools collection, which is available from:

- https://github.com/QuantumLeaps/qtools

> **NOTE**
The "QP-bundle" for Windows also contains the GCC toolchain for Windows
(MinGW), the 'make' utility, and other POSIX-style extensions.

- https://www.state-machine.com/#Downloads


# Single-Threaded and Multi-Threaded QP/C Ports
Each of the examples can be built with the single-threaded QP/C ports
(posix-qv or win32-qv) or multi-threaded ports (posix or win32). The choice
is made in the Makefiles, by editing the line, which defines the
`QP_PORT_DIR` symbol in the Makefile. For example, the following lines
in the Makefile select the single-threaded posix-qv port and leave the
multi-threaded posix port commented-out:

```
QP_PORT_DIR := $(QPC)/ports/posix-qv
#QP_PORT_DIR := $(QPC)/ports/posix
```

To reverse the selection, you need to move the comment '#' character.

> **NOTE**
The single-threaded QP/C ports (posix-qv and win32-qv) are recommended for
emulating deeply-embedded software on the desktop.

The multi-threaded QP/C ports (posix and win32) are intended for the
actual applications that run on (embedded)Linux or (embedded)Windows.


# Debug, Release, and Spy Build Configurations
The Makefiles and Visual Studio projects for the examples generally
support the following three build configurations:

### Debug Configuration
This is the default build configuration, with full debugging information and
minimal optimization. To build this configuration, type:

```
make
```

To clean this build, type

```
make clean
```

The object files and the executable is located in the 'build' sub-directory.


### Release Configuration
This configuration is built with no debugging information and high
optimization. Single-stepping and debugging might be difficult due
to the lack of debugging information and optimized code. To build
this configuration, type:

```
make CONF=rel
```

To clean this build, type

```
make CONF=rel clean
```

The object files and the executable is located in the 'build_rel' directory.

### Spy Configuration
This configuration is built with the QP's Q-SPY trace functionality.
The QP/Spy output is performed by a TCP/IP socket and requires launching
the QSPY host application with the -t option. To build this configuration,
type:

```
make CONF=spy
```

To clean this build, type

```
make CONF=spy clean
```

The object files and the executable are located in the 'build_spy' directory.

> **NOTE**
Only specific examples support the Spy build configuration. The examples
that don't support it, will report an error or will fail the linking stage.


# Support and Contact Information
- Free discussion forum: https://sourceforge.net/p/qpc/discussion/668726/
- Quantum Leaps website: https://www.state-machine.com
- Quantum Leaps email: info@state-machine.com

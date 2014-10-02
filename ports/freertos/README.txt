This directory contains a port of the QP/C framework to FreeRTOS.org.

The port has been designed to use the FreeRTOS hardware abstraction
layer, but this port does NOT eliminate all dependencies on the CPU
architecture.

Specifically, this port has been designed for the ARM Cortex-M
(ARM Cortex-M0/M3/M4/M4F) and might need some changes to work with
other CPU architectures.

However, this port should compile with all compilers for ARM Cortex-M.

Quantun Leaps
state-machine.com
info@state-machine.com
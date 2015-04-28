About The QP-ARM-CM Port
========================
This directory contains the QP/C ports to the ARM Cortex-M processors. These
ports cover the whole Cortex-M family: Cortex-M0/M0+/M3/M4F. The following
ports are provided:

arm-cm
|
+-qk      - port to the preemptive QK kernel
| +-arm   - port with the ARM-KEIL toolset (MDK-ARM)
| +-gnu   - port with the GNU-ARM toolset
| +-iar   - port with the IAR-ARM toolset (IAR EWARM)
|
+-qv      - port to the cooperative QV kernel
| +-arm   - port with the ARM-KEIL toolset (MDK-ARM)
| +-gnu   - port with the GNU-ARM toolset
| +-iar   - port with the IAR-ARM toolset (IAR EWARM)


Please see the README.txt files in the sub-directories for more information
about building the QP libraries.
About The QP-ARM-CM Port
========================
This directory contains the QP/C ports to the ARM Cortex-M processors. These
ports cover the whole Cortex-M family: Cortex-M0/M0+/M3/M4/M7. The following
ports are provided:

arm-cm
|
+-qv      - port to the cooperative QV kernel
| +-arm   - port with the ARM-KEIL toolset (MDK-ARM)
| +-gnu   - port with the GNU-ARM toolset
| +-iar   - port with the IAR-ARM toolset (IAR EWARM)
| +-ti    - port with the TI CCS ARM toolset
|
+-qk      - port to the preemptive, non-blocking QK kernel
| +-arm   - port with the ARM-KEIL toolset (MDK-ARM)
| +-gnu   - port with the GNU-ARM toolset
| +-iar   - port with the IAR-ARM toolset (IAR EWARM)
| +-ti    - port with the TI CCS ARM toolset
|
+-qxk     - port to the preemptive, blocking QXK kernel
| +-arm   - port with the ARM-KEIL toolset (MDK-ARM)
| +-gnu   - port with the GNU-ARM toolset
| +-iar   - port with the IAR-ARM toolset (IAR EWARM)
| +-ti    - port with the TI CCS ARM toolset


# About this Example

This example for the MSP540 LaunchPad (MSP-EXP430G2)
uses low-end MSP430G2553 device with 16KB or flash ROM
and only 512 bytes of RAM. The example demonstrates how
to reduce the size of the QP/C framework to fit in such
small RAM/ROM.

# QP/C Configuration

The QP/C framework is configured as follows:

1. cooperative QV kernel (to minimize the stack size)
2. dynamic events are excluded
3. only QHsm state machine implementation strategy

## Minimal QP/C port

The minimal QP/C configuration is NOT taken from the
official QP/C port to MSP430 because that port uses more
features. Instead, the minimal port is co-located with
this example in the qv_mini directory. Here are the files
and most important QP configuration settings:

```
qv/qv_mini/
   +-qep_port.h
   |    #define Q_SIGNAL_SIZE        1U
   |
   +-qf_port.h
   |    #define QF_MAX_ACTIVE        4U
   |    #define QF_EVENT_SIZ_SIZE    1U
   |    #define QF_EQUEUE_CTR_SIZE   1U
   |    #define QF_MPOOL_SIZ_SIZE    1U
   |    #define QF_MPOOL_CTR_SIZE    1U
   |    #define QF_TIMEEVT_CTR_SIZE  2U
   |
   |    /* ...do NOT use dynamic events */
   |    #define QF_MAX_EPOOL         0U
   |
```

# RAM/ROM Footprint

The following excerpt from the blinky-qv_msp-exp430g2.map
map file illustrates the resutling RAM/ROM footprint:

```
MODULE SUMMARY

       Module                 code   ro data   rw data
       ------                 ----   -------   -------
    .\
       blinky.obj             152    2         30
       main.obj               86     57        4
       bsp.obj                128    2         0
    +--+----------------------+------+---------+---------+
       Total:                 366    61        34

    .\QP\
       qep_hsm.obj            860    24        0
       qf_actq.obj            450    8         0
       qf_time.obj            396    8         12
       qf_qact.obj            258    34        11
       qf_qeq.obj             38     0         0
    +--+----------------------+------+---------+---------+
       Total:                 2002   74        23

    .\QV\
       qv.obj                 238    3         0
    +--+----------------------+------+---------+---------+
       Total:                 238    3         0

    C:\TI\ccs1100\ccs\tools\compiler\ti-cgt-msp430_21.6.0.LTS\lib\rts430_eabi.lib
       autoinit.c.obj         66     0         0
       asr16.asm.obj          44     0         0
       lsl16.asm.obj          44     0         0
       boot.c.obj             28     2         0
       copy_zero_init.c.obj   20     0         0
       mult16.asm.obj         20     0         0
       epilog.asm.obj         16     0         0
       isr_trap.asm.obj       8      0         0
       exit.c.obj             6      0         0
       pre_init.c.obj         4      0         0
       int00.asm.obj          0      2         0
       int02.asm.obj          0      2         0
       int03.asm.obj          0      2         0
       int05.asm.obj          0      2         0
       int06.asm.obj          0      2         0
       int07.asm.obj          0      2         0
       int08.asm.obj          0      2         0
       int10.asm.obj          0      2         0
       int11.asm.obj          0      2         0
       int12.asm.obj          0      2         0
       int13.asm.obj          0      2         0
       int14.asm.obj          0      2         0
       startup.c.obj          2      0         0
    +--+----------------------+------+---------+---------+
       Total:                 258    26        0

       Stack:                 0      0         128
       Linker Generated:      0      10        0
    +--+----------------------+------+---------+---------+
       Grand Total:           2864   174       185
```

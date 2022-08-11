Testing Interrupt Preemption
============================
The BSP contains code for testing interrupt preemption. This code
is based on the System Software Interrupt (SSI) capability of the
TMS570 CPU. Specifically, the SSI interrupt has been used for this
purpose. The BSP contains the ssi IRQ handler defined as follows:

QK_IRQ_BEGIN(ssiTest)  /* System Software Interrupt for testing */
    systemREG1->SSIF = 0x01; /* clear the SSI0 source */
    QACTIVE_POST(AO_Table, Q_NEW(QEvt, MAX_PUB_SIG), /* for testing... */
                 &l_ssiTest);
QK_IRQ_END()


This handler posts an event to the AO_Table active object, which is
the highest-priority in the system. This allows you to test various
scenarios of thread preemption.


Triggering SSI for Testing
==========================
The basic technique of testing preemption is to stop the code in the
debugger at the interesting point and triggering the SSI interrupt
at this place.

To trigger SSI, you need to write a special key value of 0x75 into
the System SSKEY1 register in your debugger.

IAR Debugger
------------
In IAR debugger, open the Regiser view and select the SYS1 (Primary
System Control) tab. Scroll down to the SSIR1 register, which has two
fields: SSDATA1 and SSKEY1. You type 0x75 to SSKEY1 and press Enter.
The SSI interrupt is triggered immediately.

CCS Debugger
------------
In CCS debugger, open the Registers view and select the Sys regiser
group. Scroll down to the SSIr1, which has the SSDATA1 and SSKEY1
fields combined. You type 0x7500 to the Value of the SSIr1 and press
Enter. The SSI interrupt is triggered immediately.


References
==========
TI E2E Community, Hercules Safety Microcontroller Forum
How to force System Software Interrupts(SSI):

https://e2e.ti.com/support/microcontrollers/hercules/f/312/t/413343   
/*##########################################################################*/
/*! @page exa_native Native Examples (Bare-Metal)

@htmlonly
<script src="preview.js" type="text/javascript"></script>
@endhtmlonly

@tableofcontents

<p>The QP/C framework contains real-time kernels (@ref comp_qv and @ref comp_qk), so it can run natively ("bare-metal") on single-chip microcontrollers, completely replacing a traditional RTOS. The following ready-to-use, native (bare-metal) examples are provided for QP/C:
</p>

@note
You can hover the mouse cursor over the <span class="board"></span>&nbsp;&nbsp; icon in the list below to see the picture of the board.

- @subpage exa_arm-cm (Cortex-M0/M0+/M3/M4F)
    - @ref arm-cm_blinky_ek-tm4c123gxl <a class="preview board" href="bd_EK-TM4C123GXL.jpg" title="EK-TM4C123GXL"></a>
    - @ref arm-cm_dpp_ek-tm4c123gxl <a class="preview board" href="bd_EK-TM4C123GXL.jpg" title="EK-TM4C123GXL"></a>
    - @ref arm-cm_dpp_mbed-lpc1768  <a class="preview board" href="bd_mbed-LPC1768.jpg" title="mbed-LPC1768"></a>
    - @ref arm-cm_dpp_nucleo-l053r8  <a class="preview board" href="bd_nucleo-l053r8.jpg" title="NUCLEO-L053R8"></a>
    - @ref arm-cm_dpp_nucleo-l152re  <a class="preview board" href="bd_nucleo-l152re.jpg" title="NUCLEO-L152RE"></a>
    - @ref arm-cm_game_ek-lm3s811  <a class="preview board" href="bd_EK-LM3S811.jpg" title="EK-LM3S811"></a>

- @subpage exa_arm7-9 ("classic" ARM)
    - @ref arm7-9_dpp_at91sam7s-ek <a class="preview board" href="bd_AT91SAM7S-EK.jpg" title="AT91SAM7S-EK"></a>

- @subpage exa_avr (8-bit megaAVR)
    - @ref avr_blinky_arduino-uno <a class="preview board" href="bd_Arduino-UNO.jpg" title="Arduino-UNO"></a>
    - @ref avr_dpp_arduino-uno <a class="preview board" href="bd_Arduino-UNO.jpg" title="Arduino-UNO"></a>

- @subpage exa_msp430 ("classic" MSP430 and "extended" MSP430x)
    - @ref msp430_blinky_msp-exp430g2 <a class="preview board" href="bd_MSP-EXP430G2.jpg" title="MSP-EXP430G2"></a>
    - @ref msp430_blinky_msp-exp430f5529lp <a class="preview board" href="bd_MSP-EXP430F5529LP.jpg" title="MSP-EXP430F5529LP"></a>
    - @ref msp430_dpp_msp-exp430g2 <a class="preview board" href="bd_MSP-EXP430G2.jpg" title="MSP-EXP430G2"></a>
*/
/*##########################################################################*/
/*! @page exa_arm-cm ARM Cortex-M

@htmlonly
<script src="preview.js" type="text/javascript"></script>
@endhtmlonly

The native (bare-metal) QP/C examples for ARM Cortex-M are as follows:

- @subpage arm-cm_blinky_ek-tm4c123gxl <a class="preview board" href="bd_EK-TM4C123GXL.jpg" title="EK-TM4C123GXL"></a>
- @subpage arm-cm_dpp_ek-tm4c123gxl <a class="preview board" href="bd_EK-TM4C123GXL.jpg" title="EK-TM4C123GXL"></a>
- @subpage arm-cm_dpp_mbed-lpc1768  <a class="preview board" href="bd_mbed-LPC1768.jpg" title="mbed-LPC1768"></a>
- @subpage arm-cm_dpp_nucleo-l053r8  <a class="preview board" href="bd_nucleo-l053r8.jpg" title="NUCLEO-L053R8"></a>
- @subpage arm-cm_dpp_nucleo-l152re  <a class="preview board" href="bd_nucleo-l152re.jpg" title="NUCLEO-L152RE"></a>
- @subpage arm-cm_game_ek-lm3s811  <a class="preview board" href="bd_EK-LM3S811.jpg" title="EK-LM3S811"></a>
*/
/*##########################################################################*/
/*! @page arm-cm_blinky_ek-tm4c123gxl Blinky on EK-TM4C123GXL

@image html bd_EK-TM4C123GXL.jpg EK-TM4C123GXL board
@n
@n
@image html blinky_ek-tm4c123gxl.gif Blinky on EK-TM4C123GLX (TivaC LaunchPad)
@n
Simple "Blinky" example for Texas Instruments TivaC123GXL MCU (Cortex-M4F), ARM (MDK-ARM), GNU-ARM, IAR EWARM toolsets.
@n
@image html blinky_win32.png Blinky emulation running in a Windows console 
@n
@n
@image html under_construction.jpg
*/
/*##########################################################################*/
/*! @page arm-cm_dpp_ek-tm4c123gxl DPP on EK-TM4C123GXL

@image html bd_EK-TM4C123GXL.jpg EK-TM4C123GXL board

Dining Philosophers Problem (DPP) example for Texas Instruments TivaC123GXL MCU (Cortex-M4F), ARM (MDK-ARM), GNU-ARM, IAR EWARM toolsets.

@image html dpp_win32.png DPP emulation running in Windows GUI 
@n
@n
@image html under_construction.jpg
*/
/*##########################################################################*/
/*! @page arm-cm_dpp_mbed-lpc1768 DPP on mbed-LPC1768

@image html bd_mbed-LPC1768.jpg mbed-LPC1768 board

Dining Philosophers Problem (DPP) example for  NXP LPC1768 MCU (Cortex-M3) with GNU-ARM toolset.

@image html mbed-LPC1768_button.jpg Adding External Button to mbed-LPC1768
@n
@n
@image html under_construction.jpg
*/
/*##########################################################################*/
/*! @page arm-cm_dpp_nucleo-l053r8 DPP on NUCLEO-L053R8

@image html bd_nucleo-l053r8.jpg NUCLEO-L053R8 board

Dining Philosophers Problem (DPP) example for STM32-L053R8T6 MCU (Cortex-M0+) with ARM-Keil, GNU-ARM, and IAR-ARM toolsets.

@image html under_construction.jpg
*/
/*##########################################################################*/
/*! @page arm-cm_dpp_nucleo-l152re DPP on NUCLEO-L152RE

@image html bd_nucleo-l152re.jpg NUCLEO-L152RE board

Dining Philosophers Problem (DPP) example for STM32-L152RET6 MCU (Cortex-M3) with ARM-Keil, GNU-ARM, and IAR-ARM toolsets.

@image html under_construction.jpg
*/
/*##########################################################################*/
/*! @page arm-cm_game_ek-lm3s811 "Fly 'n' Shoot" Game on EK-LM3S811

@image html bd_EK-LM3S811.jpg EK-LM3S811 board

"Fly 'n' Shoot" game example for Texas Instruments LM3S811 MCU (Cortex-M3), ARM (MDK-ARM), GNU-ARM, IAR EWARM toolsets.

@image html bd_EK-LM3S811_game.jpg Game playing on EK-LM3S811 board
@n
@n
@image html game_win32.png Game emulation running in Windows GUI 
@n
@n
@image html under_construction.jpg
*/

/*##########################################################################*/
/*! @page exa_arm7-9 ARM7 / ARM9

@htmlonly
<script src="preview.js" type="text/javascript"></script>
@endhtmlonly

The native (bare-metal) QP/C examples for the "classic" ARM7/ARM9 are as follows:

- @subpage arm7-9_dpp_at91sam7s-ek <a class="preview board" href="bd_AT91SAM7S-EK.jpg" title="AT91SAM7S-EK"></a>
*/
/*##########################################################################*/
/*! @page arm7-9_dpp_at91sam7s-ek DPP on AT91SAM7S-EK

@image html bd_AT91SAM7S-EK.jpg AT91SAM7S-EK board

Dining Philosophers Problem (DPP) example for Atmel AT91SAM7S MCU (ARM7) with GNU-ARM toolset.

@image html under_construction.jpg
*/

/*##########################################################################*/
/*! @page exa_avr AVR

@htmlonly
<script src="preview.js" type="text/javascript"></script>
@endhtmlonly

The native (bare-metal) QP/C examples for the 8-bit AVRmega are as follows:

- @subpage avr_blinky_arduino-uno <a class="preview board" href="bd_Arduino-UNO.jpg" title="Arduino-UNO"></a>
- @subpage avr_dpp_arduino-uno <a class="preview board" href="bd_Arduino-UNO.jpg" title="Arduino-UNO"></a>
*/
/*##########################################################################*/
/*! @page avr_blinky_arduino-uno Blinky on Arduino-UNO

@image html bd_Arduino-UNO.jpg Arduino-UNO board

Simple Blinky example for Arduino-UNO with GNU-AVR and IAR-AVR toolsets.

@image html under_construction.jpg
*/
/*##########################################################################*/
/*! @page avr_dpp_arduino-uno DPP on Arduino-UNO

@image html bd_Arduino-UNO.jpg Arduino-UNO board

Dining Philosophers Problem (DPP) example for Arduino-UNO with GNU-AVR and IAR-AVR toolsets.

@image html Arduino-UNO_button.jpg Adding External Button to Arduino-UNO
@n
@n
@image html Arduino-UNO_button_schem.jpg External Button Schematics
@n
@n
@image html under_construction.jpg
*/

/*##########################################################################*/
/*! @page exa_msp430 MSP430

@htmlonly
<script src="preview.js" type="text/javascript"></script>
@endhtmlonly

The native (bare-metal) QP/C examples for MSP430 are as follows:

- @subpage msp430_blinky_msp-exp430g2 <a class="preview board" href="bd_MSP-EXP430G2.jpg" title="MSP-EXP430G2"></a>
- @subpage msp430_blinky_msp-exp430f5529lp <a class="preview board" href="bd_MSP-EXP430F5529LP.jpg" title="MSP-EXP430F5529LP"></a>
- @subpage msp430_dpp_msp-exp430g2 <a class="preview board" href="bd_MSP-EXP430G2.jpg" title="MSP-EXP430G2"></a>
*/
/*##########################################################################*/
/*! @page msp430_blinky_msp-exp430g2 Blinky on MSP-EXP430G2

@image html bd_MSP-EXP430G2.jpg MSP-EXP430G2 board

Simple Blinky example for MSP-EXP430G2 with CCS-430 and IAR-430 toolsets.

@image html under_construction.jpg
*/
/*##########################################################################*/
/*! @page msp430_blinky_msp-exp430f5529lp Blinky on MSP-EXP430F5529LP

@image html bd_MSP-EXP430F5529LP.jpg MSP-EXP430F5529LP board

Simple Blinky example for MSP-EXP430F5529LP with CCS-430 and IAR-430 toolsets.

@image html under_construction.jpg
*/
/*##########################################################################*/
/*! @page msp430_dpp_msp-exp430g2 DPP on MSP-EXP430G2

@image html bd_MSP-EXP430G2.jpg MSP-EXP430G2 board

DPP example for MSP-EXP430G2 with CCS-430 and IAR-430 toolsets.

@section msp430_dpp_msp-exp430g2_qs QS Output
This example demonstrates the QS software tracing output in the Spy build configuration. QS uses the hardware UART of the MSP-EXP430G2 board connected to the Virtual COM Port on the debugger. This means that you don't need any additional wiring to receive the QS output on your development workstation.

The QS trace date requires the following setting of the QSPY host utility

@verbatim
qspy -cCOM_PORT -b9600 -O2 -F2 -E1 -P1 -B1
@endverbatim 

where `COM_PORT` denotes the Virtual COM port, which you can find out in the Device Manager (see the screen shot below):

@image html com_msp-exp430g2.png Virtual COM port of the MSP-EXP430G2 board

@note
To receive the QS data over the Virtual COM Port, you need to make sure that the jumper J3 on the MSP-EXP430G2 board is configured for the "Hardware UART" (the first two jumpers should be in the horizontal setting, as shown in the picture of the MSP-EXP430G2 board on top of this page. 

@image html under_construction.jpg
*/

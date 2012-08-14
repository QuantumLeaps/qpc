***********************************************************************************
DESCRIPTION		Renesas Starter Kit Project Readme File
***********************************************************************************

***********************************************************************************
Revision History
DD.MM.YYYY OSO-UID Description
22.05.2006 RTE-VNA First Release.
***********************************************************************************

This project can be debugged with HEW and an E8.
The Quick Start Guide supplied with the RSK can be referred to for help in
connecting to the target. This code has been designed so that the E8 Firmware
Program can be located at address h'8000.

Details of the sample project can be found at the top of the C file which contains
the projects main() function.

Support
Online technical support and information is available at: www.renesas.com/rsk

Technical Contact Details:

America: techsupport.rta@renesas.com 
Europe:  tools.support.eu@renesas.com 
Japan:   csc@renesas.com 

RSKR8C25 MCU Port - Board Connector Cross-Reference

*********************************************************************************** 
PORT	 	PIN NAME/				RING 		COMMON BUS			
NUMBER		FUNCTION				BUS PIN		PIN	Hardware		
***********************************************************************************			

			Vcc - 5V							JA1-1
			VCC - 3V							JA1-3
			AVcc								
			Gnd									JA1-2, JA2-4
			AVss								
			Res						J1-8		JA2-1
			MODE					J1-5							MODE_E8B
					
P0_0		AN7						J4-12							LCD_D4
P0_1		AN6						J4-11							LCD_D5
P0_2		AN5						J4-10							LCD_D6
P0_3		AN4						J4-9							LCD_D7
P0_4		AN3						J4-4		JA1-12				
P0_5		AN2						J4-3		JA1-11				
P0_6		AN1						J4-2		JA1-10				
P0_7		AN0						J3-12		JA1-9				

P1_0		KI0/AN8					J3-7							AD_POT
P1_1		KI1/AN9					J3-5							LCD_RS
P1_2		KI2/AN10				J3-4							LCD_E
P1_3		KI3/AN11				J2-12		JA1-23				SW3
P1_4		TXD0					J2-11		JA2-6				
P1_5		RXD0/TRAIO/INT1			J2-10		JA2-8				
P1_6		CLK0					J2-9		JA2-10				
P1_7		TRAIO/INT1				J2-8		JA2-22				SW2

P2_0		TRDIOA0/TCLK			J2-7		JA2-25	
P2_1		TRDIOB0					J2-6		JA2-13				
P2_2		TRDIOC0					J2-5		JA2-19 				
P2_3		TRDIOD0					J2-4		JA2-14				
P2_4		TRDIOA1					J2-3		JA2-15				LED0
P2_5		TRDIOB1					J2-2		JA2-17				LED1
P2_6		TRDIOC1					J2-1		JA2-16				LED2
P2_7		TRDIOD1					J1-13		JA2-18				LED3

P3_0		TRAO					J3-8		JA2-20				
P3_1		TRBO					J3-7		JA2-26		
P3_3		SSI						J1-3		JA2-24					
P3_4		SDA/SCS					J1-4		JA1-25				
P3_5		SCL/SSCK				J1-2		JA1-26				
P3_7		SSO						J4-13		JA2-11				

P4_2		VREF					J4-5		JA1_7				VREF
P4_3		XCIN					J1-6							XCIN
P4_4		XCOUT					J1-8							XCOUT
P4_5		INT0					J3-1		JA2-7,JA2-21		SW1
P4_6		XIN						J1-11		JA2-2				XIN
P4_7		XOUT					J1-9							XOUT

P6_0		TREO					J4-6		JA1-15				
P6_1								J4-8		JA1-16				
P6_2								J4-7		JA1-17				
P6_3								J3-11		JA1-18				
P6_4								J3-10		JA1-19				
P6_5		CLK1					J3-9		JA1-20				
P6_6		INT2/TXD1				J3-2		JA1-21,JA2-9		PTTX
P6_7		INT3/RXD1				J3-3		JA1-22,JA2-23		PTRX
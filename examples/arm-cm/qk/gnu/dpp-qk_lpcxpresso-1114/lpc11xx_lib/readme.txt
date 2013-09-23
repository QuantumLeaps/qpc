LPC11_Lib
=========
This library project contains code to handle various peripherals contained
in the LPC11xx.

Several files have been modified slightly from the original NXP sources to
comment out application specific code (mainly the interrupt handlers).

This means that the library project only contains the common code. Interrupt
handlers are moved into projects making use of the peripherals.





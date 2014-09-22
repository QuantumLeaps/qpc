ReadMe.txt for the STM32F429-Discovery project
This project was built for the IAR ARM Compiler 7.10.

Supported hardware:
===================
The sample project was built for the ST STM32F429 discovery eval board.
Using other hardware may require modification on the BSP.

Configurations
==============
- Debug:
  This configuration is prepared for download into
  internal Flash of the target CPU using ST-Link and CSpy.
  An embOS debug and profiling library is used.

- Release:
  This configuration is prepared to build a "release"
  output which can be downloaded into the internal FLASH
  of the target CPU using CSpy and ST-Link.


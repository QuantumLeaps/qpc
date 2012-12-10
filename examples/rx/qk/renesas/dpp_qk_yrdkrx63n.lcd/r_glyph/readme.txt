PLEASE REFER TO THE APPLICATION NOTE FOR THIS MIDDLEWARE FOR MORE INFORMATION

Glyph Library Code
==================

Document Number: 
----------------
N/A

Version:
--------
v1.0

Overview
--------
The Glyph library gives users an easy way to write to Okaya display on the RDK. By default the 8x8 font is used. This
is also the font that is used in most of the RDK demos.

Features
--------
* Functions to make it easier to control the Okaya display
* Interfaces to LCD controller on Okaya display across SPI
 
Limitations:
------------
* This code has only been tested on the Okaya LCD on the RDK.

Peripherals Used Directly:
--------------------------
* None

Required Packages:
------------------
* r_rspi_rx600

How to add to your project:
---------------------------
*  Add src\r_glyph.c to your project.
*  Add src\r_glyph_register.c to your project.
*  Add src\glyph\glyph.c to your project.
*  Add src\glyph\drivers\st7579_lcd.c to your project.
*  Add the font(s) you wish to use to your project from the src\glyph\fonts\ directory. Since the 8x8 font is used
   by default make sure to include font_8x8.c.
*  Add an include path to the 'r_glyph' directory. 
*  Add an include path to the 'r_glyph\src' directory.
*  Add an include path to the 'r_glyph\src\glyph' directory.
*  Configure Glyph library code through src\glyph\config.h.
*  Configure r_glyph middleware through r_glyph_config.h.

File Structure:
---------------
r_rspi_rx600
|
|--->src (contains package source)  
|  |
|  |--->glyph (contains Glyph code)
|     |
|     |--->drivers (contains drivers to interface to LCD controllers)
|     |
|     |--->fonts (contains available fonts)
|     |
|     |--->config.h (used for configuring Glyph code, e.g. choosing available and default font)
|
|--->r_glyph_config.h (used for configuring middleware)
|
|--->readme.txt
                

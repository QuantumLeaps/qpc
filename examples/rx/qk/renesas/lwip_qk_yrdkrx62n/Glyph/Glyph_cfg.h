/*-------------------------------------------------------------------------*
 * File:  Config.h
 *-------------------------------------------------------------------------*
 * Description:
 *     Configuration file for The Generic API for Graphics LCD
 *     Glyph
 *-------------------------------------------------------------------------*/
#ifndef __GLYPH__CONFIG_H
#define __GLYPH__CONFIG_H

#define VERSION_MAJOR           1
#define VERSION_MINOR           0
#define VERSION_STRING          "1.00"

/*-------------------------------------------------------------------------*
 *     Glyph API Library FONT USE DEFINES
 * In the first list uncomment any font you would like to choose from
 * In your code that uses the Glyph API.
 *
 * In the second list uncomment the desired DEFAULT FONT.  Comment out
 * the rest.
 *
 * The reason for this is so you can have a smaller build if you would like.
 *-------------------------------------------------------------------------*/
//#define USE_GLYPH_FONT_BITMAP
#define USE_GLYPH_FONT_HELVR10
//#define USE_GLYPH_FONT_8_BY_16
//#define USE_GLYPH_FONT_8_BY_8
//#define USE_GLYPH_FONT_WINFREE
//#define USE_GLYPH_FONT_5_BY_7
//#define USE_GLYPH_FONT_6_BY_13

//#define USE_DEFAULT_FONT Bitmaps_table
#define USE_DEFAULT_FONT FontHelvr10_table
//#define USE_DEFAULT_FONT Font8x16_table
//#define USE_DEFAULT_FONT Font8x8_table
//#define USE_DEFAULT_FONT FontWinFreeSystem14x16_table
//#define USE_DEFAULT_FONT Fontx5x7_table
//#define USE_DEFAULT_FONT Fontx6x13_table
//#define USE_DEFAULT_FONT FontHelvr10_table

#endif // __GLYPH__CONFIG_H
/*-------------------------------------------------------------------------*
 * End of File:  Config.H
 *-------------------------------------------------------------------------*/






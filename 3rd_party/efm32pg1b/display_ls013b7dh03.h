/*****************************************************************************
* Device driver for the Sharp Memory LCD LS013B7DH03
* Adapted and simplified from the file: displayls013b7dh03.h
* by Quantum Leaps, LLC
* 2016-05-12
*/
/*******************************************************************************
 * @section License
 * <b>Copyright 2015 Silicon Labs, Inc. http://www.silabs.com</b>
 *******************************************************************************
 *
 * This file is licensed under the Silabs License Agreement. See the file
 * "Silabs_License_Agreement.txt" for details. Before using this software for
 * any purpose, you must agree to the terms of that agreement.
 *
 ******************************************************************************/

#ifndef _DISPLAY_LS013B7DH03_H_
#define _DISPLAY_LS013B7DH03_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Display geometry */
#define LS013B7DH03_WIDTH   128
#define LS013B7DH03_HEIGHT  128

/* initialize the display */
bool Display_init(void);
void Display_clear(void);
void Display_enable(bool enable);
void Display_refresh(void);

/* send horizontal pixel array to the display.
* The pixel array has the full width of the display
* and height [in pixels] specified by 'h'
*
* @par pa  - pointer to the pixel array
* @par y   - start row
* @par h   - height of the pixel array
*/
void Display_sendPA(uint32_t const *pa,
                    uint8_t y, uint8_t h);
#ifdef __cplusplus
}
#endif

#endif /* DISPLAY_LS013B7DH03_H_ */

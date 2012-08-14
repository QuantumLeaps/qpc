// TI File $Revision: /main/2 $
// Checkin $Date: October 29, 2008   14:53:46 $
//###########################################################################
//
// FILE:    DSP2802x_BootVars.h
//
// TITLE:   DSP2802x Boot Variable Definitions.
//
// NOTES:
//
//###########################################################################
// $TI Release: 2802x Header Files V1.00 $
// $Release Date: November 10, 2008 $
//###########################################################################

#ifndef DSP2802x_BOOT_VARS_H
#define DSP2802x_BOOT_VARS_H

#ifdef __cplusplus
extern "C" {
#endif



//---------------------------------------------------------------------------
// External Boot ROM variable definitions:
//
extern Uint16 EmuKey;
extern Uint16 EmuBMode;
extern Uint32 Flash_CPUScaleFactor;
extern void (*Flash_CallbackPtr) (void);

#ifdef __cplusplus
}
#endif /* extern "C" */

#endif  // end of DSP2802x_BOOT_VARS_H definition

//===========================================================================
// End of file.
//===========================================================================


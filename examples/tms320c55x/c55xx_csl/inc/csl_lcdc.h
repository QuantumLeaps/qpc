/*  ===========================================================================
 *   Copyright (c) Texas Instruments Inc 2002, 2003, 2004, 2005, 2008
 *
 *   Use of this software is controlled by the terms and conditions found in
 *   the license agreement under which this software has been supplied.
 *  ===========================================================================
 */

/** @defgroup CSL_LCDC_API LCDC
 *
 * @section Introduction
 *
 * @subsection xxx Purpose and Scope
 * The purpose of this document is to identify a set of common CSL APIs for
 * the LCDC module across various devices. The Driver developer is expected to
 * refer to this document while designing APIs for these modules. Some of the
 * listed APIs may not be applicable to a given LCDC module. While other cases
 * this list of APIs may not be sufficient to cover all the features of a
 * particular LCDC Module. The Driver developer should use his discretion
 * designing new APIs or extending the existing ones to cover these.
 *
 * @subsection aaa Terms and Abbreviations
 *   -# CSL:  Chip Support Library
 *   -# API:  Application Programmer Interface
 *
 * @subsection References
 *    -# hibari_lcd_spec11.doc
 *
 */

/** @file csl_lcdc.h
 *
 *  @brief Functional layer API header file for LCD CSL
 *
 *  Path: \(CSLPATH)/inc
 */

/* ============================================================================
 * Revision History
 * ================
 * 05-Sept-2008 Added LCDC of CSL.
 * 06-May-2009 Removed the Raster mode support for C5505 PG1.4
 * 26-May-2009 Modified based on the review comments
 * ============================================================================
 */

#ifndef _CSL_LCD_H_
#define _CSL_LCD_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <soc.h>
#include <tistdtypes.h>
#include <cslr_sysctrl.h>
#include <csl_types.h>
#include <csl_error.h>
#include <csl_general.h>

/**
\defgroup CSL_LCDC_API LCDC
   \internal Based on <b>BCG LCD Controller Module Specification
              Version 0.4 </b>
*/
/**
@defgroup CSL_LCDC_SYMBOL LCDC Symbols Defined
@ingroup CSL_LCDC_API
*/

/**
@defgroup CSL_LCDC_DATASTRUCT LCDC Data Structures
@ingroup CSL_LCDC_API
*/

/**
@defgroup CSL_LCDC_ENUM  LCDC Enumerated Data Types
@ingroup CSL_LCDC_API
*/

/**
@defgroup CSL_LCDC_FUNCTION  LCDC Functions
@ingroup CSL_LCDC_API
*/

/*****************************************************************************\
          LCDC global macro declarations
\*****************************************************************************/

/** \addtogroup CSL_LCDC_SYMBOL
* @{ */
/** hash define value for instance 0 of LCDC                                 */
#define CSL_LCDC_INSTANCE_0             (0x0)
/** hash define value for invalid instance of LCDC                           */
#define CSL_LCDC_INSTANCE_INV           (0xFF)
/** hash define value for enabling the particular bit                        */
#define CSL_LCD_ENABLE                  (Uint16)(0x1)
/** hash define value for disabling the particular bit                       */
#define CSL_LCD_DISABLE                 (Uint16)(0x0)
/** hash define bit value for active condition for LCDC                      */
#define CSL_LCD_ACTIVE                  (Uint16)(0x0)
/** LCD reset clock cycle count                                              */
#define CSL_LCD_RESET_CLOCK_CYCLE       (0x20)
/** SARAM starting address                                                   */
#define CSL_LCD_SARAM_START_ADDR        (0x08000)
/** SARAM ending address                                                     */
#define CSL_LCD_SARAM_END_ADDR          (0x27FFF)
/** SARAM starting address seen by DMA                                       */
#define CSL_LCD_SARAM_DMA_ADDR_OFFSET   (0x080000)
/** mask value for 16 bit variable                                           */
#define CSL_LCD_UINT16_MASK             (0xFFFF)
/** shift value to change cpu addr for DMA                                   */
#define CSL_LCD_DMA_SHIFT               (0x01)
/** no of bits in a Uint16 datatype  variable                                */
#define CSL_LCD_UINT16_NUMBER_BITS      (0x0010)
/** mask value for LIDD Controller Polarity Field                            */
#define CSL_LCD_LCDLIDDCR_POLARITY_CONTROL_MASK  (0x00F8)
/**
@} */

/*****************************************************************************\
          LCDC global typedef declarations
\*****************************************************************************/

/**\addtogroup CSL_LCDC_ENUM
@{*/

/** \brief LIDD Mode Chip Select
*
*/
typedef enum {
    CSL_LCDC_LIDD_CS0, /**< <b>: Chipselect 0</b> */
    CSL_LCDC_LIDD_CS1  /**< <b>: Chipselect 1</b> */
} CSL_LcdcChipSelect;

/** \brief LIDD Mode CS1 Use Status
*
*/
typedef enum {
    CSL_LCDC_LIDD_NOT_USE_CS1, /**< <b>: CS1 not used</b> */
    CSL_LCDC_LIDD_USE_CS1  /**< <b>: Chipselect 1</b> */
} CSL_LcdcCs1Status;

/** \brief LIDD Mode Select
*
*  Selects type of LCD display interface for the LIDD to drive
*/
typedef enum {
    /**< <b>: Sync MPU68</b> */
    CSL_LCDC_LIDD_SYNC_MPU68 = CSL_LCDC_LCDLIDDCR_LIDD_MODE_SEL_SYNC_MPU68,
    /**< <b>: Async MPU68</b> */
    CSL_LCDC_LIDD_ASYNC_MPU68 = CSL_LCDC_LCDLIDDCR_LIDD_MODE_SEL_ASYNC_MPU68,
    /**< <b>: Sync MPU80</b> */
    CSL_LCDC_LIDD_SYNC_MPU80 = CSL_LCDC_LCDLIDDCR_LIDD_MODE_SEL_SYNC_MPU80,
    /**< <b>: Async MPU80</b> */
    CSL_LCDC_LIDD_ASYNC_MPU80 = CSL_LCDC_LCDLIDDCR_LIDD_MODE_SEL_ASYNC_MPU80,
    /**< <b>: Async Hitachi</b> */
    CSL_LCDC_LIDD_ASYNC_HITACHI = CSL_LCDC_LCDLIDDCR_LIDD_MODE_SEL_HITACHI
} CSL_LcdcLiddMode;

/** \brief LIDD frame done interrupt control
*
*/
typedef enum {
    /**< <b>: Disable LIDD Frame Done interrupt</b>*/
    CSL_LCDC_LIDD_FDONE_DISABLE,
    /**< <b>: Enable LIDD Frame Done interrupt</b>*/
    CSL_LCDC_LIDD_FDONE_ENABLE
} CSL_LcdcFdoneCtl;

/** \brief LIDD mode DMA control
*
*/
typedef enum {
    /**< <b>: Deactivate DMA control of LIDD interface
              DMA control is released upon completion of transfer of the
              current frame of data (LIDD Frame Done) after this bit is
              cleared. The MPU has direct read/write access to the panel
              in this mode</b> */
    CSL_LCDC_LIDD_DMA_DISABLE,
    /**< <b>: Activate DMA to drive LIDD interface to
              support streaming data to "smart" panels. The MPU cannot
              access the panel directly in this mode </b> */
    CSL_LCDC_LIDD_DMA_ENABLE
} CSL_LcdcDmaCtl;

/** \brief Enums for DMA burst size
*
*/
typedef enum {
    /**< <b>:  burst size of 1</b>*/
    CSL_LCDC_DMA_BURST1 = CSL_LCDC_LCDDMACR_BURST_SIZE_BSIZE1,
    /**< <b>:  burst size of 2</b>*/
    CSL_LCDC_DMA_BURST2 = CSL_LCDC_LCDDMACR_BURST_SIZE_BSIZE2,
    /**< <b>:  burst size of 4</b>*/
    CSL_LCDC_DMA_BURST4 = CSL_LCDC_LCDDMACR_BURST_SIZE_BSIZE4,
    /**< <b>:  burst size of 8</b>*/
    CSL_LCDC_DMA_BURST8 = CSL_LCDC_LCDDMACR_BURST_SIZE_BSIZE8,
    /**< <b>:  burst size of 16</b>*/
    CSL_LCDC_DMA_BURST16 = CSL_LCDC_LCDDMACR_BURST_SIZE_BSIZE16
} CSL_LcdcDmaBurst;

/** \brief Enums for Sub panel enable / disable
*
*/
typedef enum {
    /**< <b>: Sub-panel function mode disabled </b>*/
    CSL_LCDC_SUBPANEL_DISABLE = 0,
    /**< <b>: Sub-panel function mode enabled </b>*/
    CSL_LCDC_SUBPANEL_ENABLE  = 1
} CSL_LcdcSubpanelCtl;

/** \brief Enums for Sub panel position control
*
*/
typedef enum {
    /**< <b>: The image from video memory is displayed below the
             threshold value (LPPT) </b>*/
    CSL_LCDC_SUBPANPOS_LOW = 0,
    /**< <b>: The image from video memory is displayed above the
             threshold value (LPPT) </b>*/
    CSL_LCDC_SUBPANPOS_HIGH  = 1
} CSL_LcdcSubpanPosCtl;

/** \brief Enums for HSYNC/VSYNC pixel clock control
*
*/
typedef enum {
    /**< <b>: Lcd_lp and lcd_fp are driven on opposite  edges
             of  pixel clock than the lcd_pixel_o </b>*/
    CSL_LCDC_SYNCCTL_PIXCLK = 0,
    /**< <b>: Lcd_lp and lcd_fp are driven according to bit24,
             should be on only when in TFT mode </b>*/
    CSL_LCDC_SYNCCTL_BIT24  = 1
} CSL_LcdcSyncClkCtl;

/** \brief Enums for HSYNC/VSYNC pixel clock edge control
*
*/
typedef enum {
     /**< <b>: lcd_lp and lcd_fp are driven on rising edge of
              pixel clock </b>*/
    CSL_LCDC_SYNCEDGECTL_FALL = 0,
     /**< <b>: lcd_lp and lcd_fp are driven on falling edge of
              pixel clock </b>*/
    CSL_LCDC_SYNCEDGECTL_RISE = 1
} CSL_LcdcSyncEdgeCtl;

/** \brief Enums for EOF Interrupt enable/disable
*
*/
typedef enum {
    /**< <b>: End of Frame 0/1Interrupt disabled </b>*/
    CSL_LCDC_EOFINT_DISABLE = 0,
    /**< <b>: End of Frame 0/1Interrupt enabled </b>*/
    CSL_LCDC_EOFINT_ENABLE = 1
} CSL_LcdcEofIntCtl;

/** \brief Enums for EOF Big endian enable/disable
*
*/
typedef enum {
    /**< <b>: Big Endian data reordering disabled </b>*/
    CSL_LCDC_ENDIANESS_LITTLE = 0,
    /**< <b>: Big Endian data reordering enabled </b>*/
    CSL_LCDC_ENDIANESS_BIG    = 1
} CSL_LcdcEndianess;

/** \brief Enums for Frame buffer mode
*
*/
typedef enum {
    /**< <b>: One frame buffer (FB0 only) used </b>*/
    CSL_LCDC_FBMODE_SINGLE = 0,
    /**< <b>: Two frame buffers used; DMA ping-pongs between
             FB0 and FB1 in this mode </b>*/
    CSL_LCDC_FBMODE_DOUBLE = 1
} CSL_LcdcFrameMode;

/**
@} */

/**
\addtogroup CSL_LCDC_DATASTRUCT
@{*/

/**
   \brief Configuration parameters for LCDC in LIDD mode
*
*   Used to configure LCDC for LIDD mode
*/
typedef struct {
    /** Clock divisor (2-255)*/
    Uint16  clkDiv;
    /** LIDD frame done interrupt */
    CSL_LcdcFdoneCtl  fdoneIntEn;
    /** CS0/CS1 Select for LIDD DMA writes
         0: DMA writes to LIDD CS0
         1: DMA writes to LIDD CS1*/
    CSL_LcdcChipSelect  dmaCs0Cs1;
    /** LIDD DMA enable */
    CSL_LcdcDmaCtl  dmaEnable;
    /** LIDD polarity control mask*/
    CSL_BitMask16 polControl;
    /** LIDD Mode Select */
    CSL_LcdcLiddMode  modeSel;
} CSL_LcdcConfigLidd;

/**
   \brief Timing parameters for LCDC in LIDD mode
*
*   Used to configure timing parameters in LIDD mode
*/
typedef struct {
    /** Write Strobe Set-Up cycles (0-31) */
    Uint16    wSu;
    /** Write Strobe Duration cycles (0-63) */
    Uint16    wStrobe;
    /** Write Strobe Hold cycles (0-15) */
    Uint16    wHold;
    /** Read Strobe Set-Up cycles (0-31) */
    Uint16    rSu;
    /** Read Strobe Duration cycles (0-63) */
    Uint16    rStrobe;
    /** Read Strobe Hold cycles (0-15)*/
    Uint16    rHold;
    /** Turn Around cycles (0-3) */
    Uint16    ta;
} CSL_LcdcLiddTiming;

/**
   \brief Configuration parameters for DMA engine
*
*   Used to configure DMA
*/
typedef struct {
    /** Burst Size setting for DMA transfers */
    CSL_LcdcDmaBurst   burstSize;
    /** End of Frame Interrupt Enable */
    CSL_LcdcEofIntCtl  eofIntEn;
    /** Big Endian enable */
    CSL_LcdcEndianess  bigEndian;
    /** Frame Mode */
    CSL_LcdcFrameMode  frameMode;
    /** The start address for frame buffer 0,
         specify most significant 30-bits */
    Uint32  fb0Base;
    /** The end address for frame buffer 0,
         specify most significant 30-bits */
    Uint32  fb0Ceil;
    /** The start address for frame buffer 1,
         specify most significant 30-bits*/
    Uint32  fb1Base;
    /** The end address for frame buffer 1,
         specify most significant 30-bits */
    Uint32  fb1Ceil;
} CSL_LcdcConfigDma;

/**
 *  \brief Input parameters for setting up LCD Controller
 *   Parameters used to put LCDC into useful state
 */
typedef struct {
    /** place holder for future implementation */
    void                  *extendSetup;
    /** LCDC config sructure instance          */
    CSL_LcdcConfigLidd    config;
    /** LCDC timing sructure instance for cs0  */
    CSL_LcdcLiddTiming    timingCs0;
    /** Enum for chosing the chip select       */
    CSL_LcdcCs1Status     useCs1;
    /** LCDC timing sructure instance for cs1  */
    CSL_LcdcLiddTiming    timingCs1;
} CSL_LcdcHwSetup;

/** \brief This object contains the reference to the instance of LCDC
* opened using the @a CSL_lcdcOpen()
*
* The pointer to this is passed to all LCDC CSL APIs
* This structure has the fields required to configure LCDC for any test
* case/application. It should be initialized as per requirements of a
* test case/application and passed on to the setup function
*/

typedef struct CSL_LcdcObj {
    /** This is a pointer to the registers of the
         instance of LCDC referred to by this object */
    CSL_LcdcRegsOvly    regs;
    /** This is the instance of LCDC being referred to
         by this object */
    CSL_InstNum         perNum;
} CSL_LcdcObj;

/** \brief this is a pointer to @a CSL_LcdcObj and is passed as the first
* parameter to all LCDC CSL APIs */
typedef CSL_LcdcObj*    CSL_LcdcHandle;

/** @brief This will have the base-address information for the peripheral
 *  instance
 */
typedef struct {
    /** Base-address of the Configuration registers of LCDC.
     */
    CSL_LcdcRegsOvly regs;
} CSL_LcdcBaseAddress;

/** @brief LCDC specific parameters. Present implementation doesn't have
 *  any specific parameters.
 */
typedef struct {
    /** Bit mask to be used for module specific parameters.
     *  The below declaration is just a place-holder for future
     *  implementation.
     */
    CSL_BitMask16   flags;
} CSL_LcdcParam;

/** @brief LCDC specific context information. Present implementation doesn't
 *  have any Context information.
 */

typedef struct {
    /** Context information of LCDC.
     *  The below declaration is just a place-holder for future
     *  implementation.
     */
    Uint16  contextInfo;
} CSL_LcdcContext;

/**
@} */

/******************************************************************************
 * CSL LCDC function declarations
 *****************************************************************************/
/** @addtogroup CSL_LCDC_FUNCTION
 @{ */
/** ===========================================================================
 *   @n@b LCD_init
 *
 *   @b Description
 *   @n This is the initialization function for the LCD CSL. The function
 *      must be called before calling any other API from this CSL. This
 *      function is for enabling the clock to the LCD Controller.
 *      Currently, the function just return status CSL_SOK. as of now no
 *      checking is done inside the definition of function , but in near
 *      future it can be implemented.
 *
 *   @b Arguments
 *   @verbatim
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                        CSL_SOK - Always returns
 *
 *   <b> Pre Condition </b>
 *   @n  This must be first API to use LCDC module.
 *
 *   <b> Post Condition </b>
 *   @n  None
 *
 *   @b Modifies
 *   @n None
 *
 *   @b Example
 *   @verbatim
             CSL_status       status;

             status = LCD_init();

     @endverbatim
 *  ===========================================================================
 */
CSL_Status LCD_init(void);

/** ===========================================================================
 *   @n@b LCD_open
 *
 *   @b Description
 *   @n This function returns the handle to the LCDC.
 *      and handle is passed to all other CSL APIs.
 *
 *   @b Arguments
 *   @verbatim
            instanceNum   instance number for LCDC
            pLcdcObj      pointer to LCDC object.
            status        Status of the function call
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_LcdcHandle
 *   @n                     Valid LCDC handle will be returned if
 *                          status value is equal to CSL_SOK.
 *
 *   <b> Pre Condition </b>
 *   @n  LCD_init() API should be called before LCD_open().
 *
 *   <b> Post Condition </b>
 *   @n   1.    The status is returned in the status variable. If status
 *              returned is
 *   @li            CSL_SOK            - Valid LCDC handle is returned
 *   @li            CSL_ESYS_INVPARAMS  - lcdc object pointer is invalid.
 *   @n   2.    CSL_LcdcObj object structure is populated
 *
 *   @b Modifies
 *   @n    1. The status variable
 *   @n    2. CSL_LcdcObj object structure
 *
 *   @b Example
 *   @verbatim

            Uint16               instanceNum;
            CSL_status           status;
            CSL_LcdcObj          pLcdcObj
            CSL_LcdcHandle       hLcdc;
            ...

            instanceNum = CSL_LCDC_INSTANCE_0
            hLcdc = LCD_open(instanceNum, &pLcdcObj, &status);
            ...
    @endverbatim
 *  ===========================================================================
 */
CSL_LcdcHandle LCD_open (
                   Uint16       instanceNum,
                   CSL_LcdcObj  *pLcdcObj,
                   CSL_Status   *status     );

/** ===========================================================================
 *   @n@b LCD_setup
 *
 *   @b Description
 *   @n It configures the LCDC register specific to LIDD controller to draw
 *      the images or text on LCD panel.
 *
 *   @b Arguments
 *   @verbatim
            hLcdc            Handle to the LCDC.
            setup            Pointer to LCDC setup structure.

     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK             - Hardware setup successful
 *   @li                    CSL_ESYS_BADHANDLE  - Invalid handle
 *   @li                    CSL_ESYS_INVPARAMS  - setup Parameter is invalid.
 *
 *   <b> Pre Condition </b>
 *   @n  LCD_open() API should be called before this.
 *
 *   <b> Post Condition </b>
 *   @n  None
 *
 *   @b Modifies
 *   @n  LCDC Registers will be populated.
 *
 *   @b Example
 *   @verbatim
            Uint16               instanceNum;
            CSL_status           status;
            CSL_LcdcObj          pLcdcObj
            CSL_LcdcHandle       hLcdc;
            CSL_LcdcHwSetup      setup;
            ...
     CSL_LcdcConfigLidd       configLIDD;
     CSL_LcdcLiddTiming       timingCs0LIDD;
     CSL_LcdcLiddTiming       timingCs1LIDD;

.........\\\\\\\\\\\\\  SETUP for LIDD Controller

    configLIDD.clkDiv      = 0x3;
    configLIDD.fdoneIntEn  = CSL_LCDC_LIDD_FDONE_DISABLE ;
    configLIDD.dmaCs0Cs1   = CSL_LCDC_LIDD_CS0;
    configLIDD.dmaEnable   = CSL_LCDC_LIDD_DMA_DISABLE;
    configLIDD.polControl  = 0x0000;
    configLIDD.modeSel     = CSL_LCDC_LIDD_ASYNC_MPU80;

    timingCs0LIDD.wSu      = 0x1F;
    timingCs0LIDD.wStrobe  = 0x3F ;
    timingCs0LIDD.wHold    = 0x0F;
    timingCs0LIDD.rSu      = 0x1F;
    timingCs0LIDD.rStrobe  = 0x3F;
    timingCs0LIDD.rHold    = 0x0F;
    timingCs0LIDD.ta       = 0x01;

    timingCs1LIDD.wSu      = 0x1F;
    timingCs1LIDD.wStrobe  = 0x3F ;
    timingCs1LIDD.wHold    = 0x0F;
    timingCs1LIDD.rSu      = 0x1F;
    timingCs1LIDD.rStrobe  = 0x3F;
    timingCs1LIDD.rHold    = 0x0F;
    timingCs1LIDD.ta       = 0x01;

    setup.config        = configLIDD;
    setup.useCs1        = CSL_LCDC_LIDD_NOT_USE_CS1;
    setup.timingCs0        = timingCs0LIDD;
    setup.extendSetup   = NULL;
||||||||||||*******************************************************||||||||||||

            instanceNum = CSL_LCDC_INSTANCE_0
            hLcdc = LCD_open(instanceNum, &pLcdcObj, &status);

            status = LCD_setup(hLcdc, &setup);
            ...

     @endverbatim
 *  ===========================================================================
 */
CSL_Status LCD_setup (
               CSL_LcdcHandle         hLcdc,
               CSL_LcdcHwSetup*       setup  );

/** ===========================================================================
 *   @n@b LCD_configDMA
 *
 *   @b Description
 *   @n It configures the LCDC register specific to DMA configuration and used
 *      to put the bulky data on LCD panel.
 *
 *   @b Arguments
 *   @verbatim
            hLcdc            Handle to the LCDC.
            pconfigDma       Pointer to DMA Config structure.

     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK             - Hardware setup successful
 *   @li                    CSL_ESYS_BADHANDLE  - Invalid handle
 *   @li                    CSL_ESYS_INVPARAMS  - Congig Parameter is invalid.
 *   @li                    CSL_ESYS_NOTSUPPORTED
 *                                    - Buffer memory is not in SARAM Portion.
 *
 *   <b> Pre Condition </b>
 *   @n  LCD_open() and LCD_setup() API should be called before this and
 *       memory buffer must be in the SARAM section.
 *
 *   <b> Post Condition </b>
 *   @n  For star of data Trnsfer enable the dma bit in LIDD register
 *
 *   @b Modifies
 *   @n  LCDC DMA Related Registers will be populated.
 *
 *   @b Example
 *   @verbatim

#define LCD_MAX_BUFFER_SIZE      (0x0FFF)

            Uint16               instanceNum;
            CSL_status           status;
            CSL_LcdcObj          pLcdcObj
            CSL_LcdcHandle       hLcdc;
            CSL_LcdcHwSetup      setup;
            CSL_LcdcConfigDma    pConfigDma;

    Uint16 Buf[LCD_MAX_BUFFER_SIZE];
                ...
     CSL_LcdcConfigLidd       configLIDD;
     CSL_LcdcLiddTiming       timingCs0LIDD;
     CSL_LcdcLiddTiming       timingCs1LIDD;
 *
.........\\\\\\\\\\\\\  SETUP for LIDD Controller

    configLIDD.clkDiv      = 0x3;
    configLIDD.fdoneIntEn  = CSL_LCDC_LIDD_FDONE_DISABLE ;
    configLIDD.dmaCs0Cs1   = CSL_LCDC_LIDD_CS0;
    configLIDD.dmaEnable   = CSL_LCDC_LIDD_DMA_DISABLE;
    configLIDD.polControl  = 0x0000;
    configLIDD.modeSel     = CSL_LCDC_LIDD_ASYNC_MPU80;

    timingCs0LIDD.wSu      = 0x1F;
    timingCs0LIDD.wStrobe  = 0x3F ;
    timingCs0LIDD.wHold    = 0x0F;
    timingCs0LIDD.rSu      = 0x1F;
    timingCs0LIDD.rStrobe  = 0x3F;
    timingCs0LIDD.rHold    = 0x0F;
    timingCs0LIDD.ta       = 0x01;

    timingCs1LIDD.wSu      = 0x1F;
    timingCs1LIDD.wStrobe  = 0x3F ;
    timingCs1LIDD.wHold    = 0x0F;
    timingCs1LIDD.rSu      = 0x1F;
    timingCs1LIDD.rStrobe  = 0x3F;
    timingCs1LIDD.rHold    = 0x0F;
    timingCs1LIDD.ta       = 0x01;

    setup.config        = configLIDD;
    setup.useCs1        = CSL_LCDC_LIDD_NOT_USE_CS1;
    setup.timingCs0        = timingCs0LIDD;
    setup.extendSetup   = NULL;

||||||||||||*******************************************************||||||||||||

            instanceNum = CSL_LCDC_INSTANCE_0
            hLcdc = LCD_open(instanceNum, &pLcdcObj, &status);

            status = LCD_setup(hLcdc, &setup);

        pConfigDma.burstSize  = CSL_LCDC_DMA_BURST4;
        pConfigDma.eofIntEn   = CSL_LCDC_EOFINT_DISABLE;
        pConfigDma.bigEndian  = CSL_LCDC_ENDIANESS_LITTLE;
        pConfigDma.frameMode  = CSL_LCDC_FBMODE_SINGLE;
        pConfigDma.fb0Base    = (Uint32)Buf;
        pConfigDma.fb0Ceil    = (Uint32)&Buf[LCD_MAX_BUFFER_SIZE];
        pConfigDma.fb1Base    = (Uint32)NULL;
        pConfigDma.fb1Ceil    = (Uint32)NULL;

            status = LCD_configDMA(hLcdc, &pconfigDma);
            ...

     @endverbatim
 *  ===========================================================================
 */
CSL_Status LCD_configDMA (
               CSL_LcdcHandle         hLcdc,
               CSL_LcdcConfigDma*     pconfigDma );

/** ===========================================================================
 *   @n@b LCD_close
 *
 *   @b Description
 *   @n This function closes the specified handle to LCDC.
 *
 *   @b Arguments
 *   @verbatim
            hLcdc            Handle to the LCDC
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK             - Close successful
 *   @li                    CSL_ESYS_BADHANDLE  - Invalid handle
 *
 *   <b> Pre Condition </b>
 *   @n  LCD_open() API should be called before this.
 *
 *   <b> Post Condition </b>
 *   @n  No other API of LCDC should be call after this.
 *
 *   @b Modifies
 *   @n LCDC Register Overlay structure will be assigned to NULL.
 *
 *   @b Example
 *   @verbatim
            Uint16               instanceNum;
            CSL_status           status;
            CSL_LcdcObj          pLcdcObj
            CSL_LcdcHandle       hLcdc;
            ...

            instanceNum = CSL_LCDC_INSTANCE_0
            hLcdc = LCD_open(instanceNum, &pLcdcObj, &status);

            status = LCD_close(hLcdc);
     @endverbatim
 *  ===========================================================================
 */
CSL_Status LCD_close(
               CSL_LcdcHandle     hLcdc);
/**
@} */

#ifdef __cplusplus
}
#endif
#endif /* _CSL_LCDC_H_ */


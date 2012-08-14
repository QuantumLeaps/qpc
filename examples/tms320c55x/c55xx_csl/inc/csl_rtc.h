/*  ============================================================================
 *   Copyright (c) Texas Instruments Inc 2002, 2003, 2004, 2005, 2008
 *
 *   Use of this software is controlled by the terms and conditions found in the
 *   license agreement under which this software has been supplied.
 *  ============================================================================
 */

/** @file csl_rtc.h
 *
 *  @brief RTC functional layer API header file
 *
 *  Path: \(CSLPATH)/inc
 */

/* ============================================================================
 * Revision History
 * ================
 * 25-Sep-2008 Created
 * 04-May-2009 Added RTC_setCompensation API for C5505 PG1.4
 * 25-May-2009 Modified based on the review comments
 * ============================================================================
 */

/** @defgroup CSL_RTC_API RTC
 *
 * @section Introduction
 *
 * @subsection xxx Overview
 * C5505 includes a Real Time Clock (RTC) with its own separated power supply
 * and isolation circuits. The separate supply and isolation circuits allow RTC
 * to run while the rest of the C5505 device is powered off. All RTC registers
 * are preserved and the counter continues to operate when the C5505 device is
 * powered off. The RTC also has the capability wakeup the Power Management and
 * apply power to the rest of the C5505 through an alarm, periodic interrupt,
 * or external WAKEUP signal.
 *  The RTC includes the following features:
 * Time Information (seconds, minutes, and hours) in binary codec decimal(BCD),
 * 24 hour mode,millisecond time correction Calendar Information (day, month,
 * and year) in BCD,100-year calendar, up to the year 2099,leap year compensation.
 *
 * Interrupt generation,Periodic interrupt: every millisecond, second, minute,hour
 * Alarm interrupt: precise time of day.
 * 32KHz Oscillator with frequency calibration.
 *
 */

#ifndef _CSL_RTC_H_
#define _CSL_RTC_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "cslr.h"
#include "csl_error.h"
#include "csl_types.h"
#include "soc.h"
#include "csl_general.h"

/**
@defgroup CSL_RTC_SYMBOL  RTC Symbols Defined
@ingroup CSL_RTC_API
*/

/**
@defgroup CSL_RTC_ENUM RTC Enumerated Data Types
@ingroup CSL_RTC_API
*/

/**
@defgroup CSL_RTC_DATASTRUCT  RTC Data Structures
@ingroup CSL_RTC_API
*/

/**
@defgroup CSL_RTC_FUNCTION  RTC Functions
@ingroup CSL_RTC_API
*/

/**
@defgroup CSL_RTC_INLINE_FUNCTION  RTC Inline Function
@ingroup CSL_RTC_API
*/

/*****************************************************************************\
          RTC global macro declarations
\*****************************************************************************/

/** @addtogroup CSL_RTC_SYMBOL
 @{ */
/** hash define for enabling the particular bit of RTC registers  */
#define CSL_RTC_BIT_SET                 (1u)
/** hash define for disabling the particular bit of RTC registers */
#define CSL_RTC_BIT_RESET               (0)
/** no of no operation cycle for waiting of 3-32kz */
#define CSL_RTC_WAIT_CYCLE              (50u)
/** Scratch Mask value for wakeup of corazon                      */
#define CSL_RTC_SCRATCH1_MASK           (0x8000u)
/** Status Mask value for reset the status register.              */
#define CSL_RTC_STATUS_MASK             (0x803Fu)
/** Size of dispatch table                                          */
#define CSL_RTC_DISPATCH_TABLE_SIZE        (0x07u)
/** RTC maximum compensation value                                  */
#define CSL_RTC_COMPENSATION_MAX        (1024u)
/** RTC Time out value                                             */
#define CSL_RTC_TIME_OUT                (0x1FFu)

/**
@} */

/**************************************************************************\
* RTC global typedef declarations                                         *
\**************************************************************************/

/**
@addtogroup CSL_RTC_ENUM
@{*/

/**
 *\brief
 *   enum variable for the selection of rtc Periodic interrupt
 *   for Day , Hour , Minute , Second and MilliSecond Period.
 */
typedef enum
{
    CSL_RTC_DAY_PERIODIC_INTERRUPT,   /* day periodic Interrupt enable */
    CSL_RTC_HR_PERIODIC_INTERRUPT,    /* hour periodic Interrupt enable */
    CSL_RTC_MINS_PERIODIC_INTERRUPT,  /* minute periodic Interrupt enable */
    CSL_RTC_SEC_PERIODIC_INTERRUPT,   /* second periodic Interrupt enable */
    CSL_RTC_MS_PERIODIC_INTERRUPT  /* milli second periodic Interrupt enable*/
} CSL_RTCPeriodicInterruptType;

/**
 *\brief
 *   enum variable for the selection of rtc interrupt
 *   Alarm Int,External Event, Day Event, Hour Event,
 *   Minute Event, Second Event, MilliSecond Event.
 */
typedef enum
{
    CSL_RTC_MSEVENT_INTERRUPT,     /* milli second int enable      */
    CSL_RTC_SECEVENT_INTERRUPT,    /* second event int enable      */
    CSL_RTC_MINSEVENT_INTERRUPT,   /* minute event int enable      */
    CSL_RTC_HREVENT_INTERRUPT,     /* hour event int enable        */
    CSL_RTC_DAYEVENT_INTERRUPT,    /* day event int enable         */
    CSL_RTC_EXTEVENT_INTERRUPT,    /* external event int enable    */
    CSL_RTC_ALARM_INTERRUPT,       /* alarm int enable             */
    CSL_RTC_INTERRUPT_NONE           /*  No interrupt enable         */
} CSL_RTCEventType;

/**
 *\brief
 *   This enum holds type of the compensation required for the RTC
 */
typedef enum
{
    CSL_RTC_COMPENSATION_NEGATIVE = 0,   /* Negative drift in the msec value */
    CSL_RTC_COMPENSATION_POSITIVE,       /* Positive drift in the msec value */
    CSL_RTC_COMPENSATION_INVALID         /* Invalid value for error checking */
} CSL_RTCCompType;

/**
@}*/

/**
\addtogroup CSL_RTC_DATASTRUCT
@{*/

/**
 * \brief RTC Config Structure.
 *
 */
typedef struct {
    Uint16 rtcmSec;  /**< MSecond field value between 0 - 1023 */
    Uint16 rtcsec;   /**< Second field value between  0 - 59 */
    Uint16 rtcmin;   /**< Minute field value between  0 - 59 */
    Uint16 rtchour;  /**< Hour field value between 0 - 23 */
    Uint16 rtcday;   /**< Month field value between 1 - 31 */
    Uint16 rtcmonth; /**< Month field value between 1 - 12 */
    Uint16 rtcyear;  /**< Year field value between 0 - 99 */
    Uint16 rtcmSeca; /**< MSecond Alarm field value between 0 - 1023 */
    Uint16 rtcseca;  /**< Hour Alarm field value between 0 - 23 */
    Uint16 rtcmina;  /**< Minute Alarm field value between  0 - 59 */
    Uint16 rtchoura; /**< Second Alarm field value between  0 - 59 */
    Uint16 rtcdaya;  /**< Day Alarm field value between 1 - 31 */
    Uint16 rtcmontha;/**< Month Alarm field value between 1 - 12 */
    Uint16 rtcyeara; /**< Year Alarm field value between 0 - 99 */
    Uint16 rtcintcr; /**< RTC Interrupt field */
} CSL_RtcConfig;

/**
 * \brief RTC Date Structure.
 *  Contains the field of year, month and day for Date.
 */
typedef struct
{
    Uint16 year;  /**< Year field value between 0 - 99 */
    Uint16 month; /**< Month field value between 1 - 12 */
    Uint16 day;   /**< Month field value between 1 - 31 */
} CSL_RtcDate;

/**
 * \brief RTC Time Structure.
 *  Contains the field of hour, minute, second and milli second for time.
 */
typedef struct {
    Uint16 hours; /**< Hour field value between 0 - 23 */
    Uint16 mins;  /**< Minute field value between  0 - 59 */
    Uint16 secs;  /**< Second field value between  0 - 59 */
    Uint16 mSecs; /**< MSecond field value between 0 - 1023 */
} CSL_RtcTime;

/**
 * \brief RTC Alarm Time  Structure.
 *  Contains the field of hour, minute, second and milli second for Alarm time.
 */
typedef struct {
    Uint16 year;   /**< Year Alarm field value between 0 - 99 */
    Uint16 month;  /**< Month Alarm field value between 1 - 12 */
    Uint16 day;    /**< Day Alarm field value between 1 - 31 */
    Uint16 hours;  /**< Hour Alarm field value between 0 - 23 */
    Uint16 mins;   /**< Minute Alarm field value between  0 - 59 */
    Uint16 secs;   /**< Second Alarm field value between  0 - 59 */
    Uint16 mSecs;  /**< MSecond Alarm field value between 0 - 1023 */
} CSL_RtcAlarm;

/**
 *\brief Declarations for interrupt routines
 */
typedef struct
{
    void (*MilEvtAddr)(void);     /**< Millisecond interrupt */
       void (*SecEvtAddr)(void);     /**< Second interrupt      */
    void (*MinEvtAddr)(void);     /**< Minute interrupt      */
    void (*HourEvtAddr)(void);    /**< Hour interrupt        */
    void (*DayEvtAddr)(void);     /**< Day interrupt         */
    void (*ExtEvtAddr)(void);     /**< External interrupt    */
    void (*AlarmEvtAddr)(void);   /**< Alarm interrupt       */
} CSL_RtcIsrAddr;

/**
* \brief  call back function for RTC.
*
*/
typedef void (* CSL_RTCCallBackPtr)(void);

/**
* \brief structure array of isr function for RTC.
*
*/
typedef struct {
/**< Isr function for RTC  */
   CSL_RTCCallBackPtr isr[CSL_RTC_DISPATCH_TABLE_SIZE];
} CSL_RtcIsrDispatchTable;

/**
@}*/

/******************************************************************************
 * CSL RTC Inline function
 *****************************************************************************/
/** @addtogroup CSL_RTC_INLINE_FUNCTION
 @{ */

/** ===========================================================================
 *   @n@b rtcDivFun
 *
 *   @b Description
 *   @n division of two integer number.
 *
 *   @b Arguments
 *   @verbatim
            dividend         value.
            divisor          value
     @endverbatim
 *
 *   <b> Return Value </b>  Uint16
 *
 *   <b> Pre Condition </b>
 *   @n  NOne.
 *
 *   <b> Post Condition </b>
 *   @n  None
 *
 *   @b Modifies
 *   @n  None
 *
 *  ===========================================================================
 */

static inline
Uint16 rtcDivFun(
          Uint16 dividend,
          Uint16 divisor  )
{
    Uint16 quoitient;

    quoitient = 0;
    while (dividend >= divisor)
    {
        quoitient++;
        dividend = dividend - divisor;
    }
    return quoitient;
}

/** ===========================================================================
 *   @n@b rtcModuloDivFun
 *
 *   @b Description
 *   @n division of two integer number.
 *
 *   @b Arguments
 *   @verbatim
            dividend         value.
            divisor          value
     @endverbatim
 *
 *   <b> Return Value </b>  Uint16
 *
 *   <b> Pre Condition </b>
 *   @n  NOne.
 *
 *   <b> Post Condition </b>
 *   @n  None
 *
 *   @b Modifies
 *   @n  None
 *
 *  ===========================================================================
 */

static inline
Uint16 rtcModuloDivFun(
          Uint16 dividend,
          Uint16 divisor  )
{
    while (dividend >= divisor)
    {
        dividend = dividend - divisor;
    }
    return dividend;
}
/** ===========================================================================
 *   @n@b rtcModuloDivFun
 *
 *   @b Description
 *   @n function for power operator.
 *
 *   @b Arguments
 *   @verbatim
            var
            pow
     @endverbatim
 *
 *   <b> Return Value </b>  Uint16
 *
 *   <b> Pre Condition </b>
 *   @n  NOne.
 *
 *   <b> Post Condition </b>
 *   @n  None
 *
 *   @b Modifies
 *   @n  None
 *
 *  ===========================================================================
 */

/**
 * \brief
 *        function for power operator.
 * \param var [IN].
 * \param pow [IN].
 * \return Uint16.
 */
static inline
Uint16 rtc_pow(
           Uint16  var,
           Uint16  pow )
{
    Uint16 result;
    Uint16 looper;

    result = 1;
    for (looper = 0 ; looper < pow; looper++)
    {
        result  = result * var;
    }
    return result;
}

/**
@} */

/******************************************************************************
 * RTC function declarations
 *****************************************************************************/
/** @addtogroup CSL_RTC_FUNCTION
 @{ */

/** ===========================================================================
 *   @n@b RTC_reset
 *
 *   @b Description
 *   @n Function reset all the register of RTC.
 *
 *   @b Arguments
 *   @verbatim
     @endverbatim
 *
 *   <b> Return Value </b>
 *    @n None
 *
 *   <b> Pre Condition </b>
 *    @n None
 *
 *   <b> Post Condition </b>
 *    @n None
 *
 *   @b Modifies
 *   @n  RTC Register will be populated with the reset values.
 *
 *   @b Example
 *   @verbatim

             RTC_reset();
     @endverbatim
 *  ===========================================================================
 */
void RTC_reset(void);

/** ===========================================================================
 *   @n@b RTC_start
 *
 *   @b Description
 *   @n Function enable the RTC interrupts.
 *
 *   @b Arguments
 *   @verbatim
     @endverbatim
 *
 *   <b> Return Value </b>
 *    @n None
 *
 *   <b> Pre Condition </b>
 *    @n None
 *
 *   <b> Post Condition </b>
 *    @n None
 *
 *   @b Modifies
 *   @n NOne
 *
 *   @b Example
 *   @verbatim

             RTC_start();
     @endverbatim
 *  ===========================================================================
 */
void RTC_start(void);

/** ===========================================================================
 *   @n@b RTC_stop
 *
 *   @b Description
 *   @n Function disable the RTC interrupts.
 *
 *   @b Arguments
 *   @verbatim
     @endverbatim
 *
 *   <b> Return Value </b>
 *    @n None
 *
 *   <b> Pre Condition </b>
 *    @n None
 *
 *   <b> Post Condition </b>
 *    @n None
 *
 *   @b Modifies
 *   @n None
 *
 *   @b Example
 *   @verbatim

             RTC_stop();
     @endverbatim
 *  ===========================================================================
 */
void RTC_stop(void);

/** ===========================================================================
 *   @n@b RTC_config
 *
 *   @b Description
 *   @n It configures the RTC registers as per the RTC config structure.
 *
 *   @b Arguments
 *   @verbatim
            pConfig          Pointer to RTC Config structure.

     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK
 *   @li                    CSL_ESYS_INVPARAMS  - Congig Parameter is invalid.
 *
 *   <b> Pre Condition </b>
 *   @n  NOne.
 *
 *   <b> Post Condition </b>
 *   @n  None
 *
 *   @b Modifies
 *   @n 1. RTC registers will be updated as per config parameter
 *
 *   @b Example
 *   @verbatim

            CSL_RtcConfig       pConfig;
            CSL_status           status;

    pConfig.rtcyear  = 8;
    pConfig.rtcmonth = 8;
    pConfig.rtcday   = 8;
    pConfig.rtchour  = 8;
    pConfig.rtcmin   = 8;
    pConfig.rtcsec   = 8;
    pConfig.rtcmSec  = 8;

    pConfig.rtcyeara  = 8;
    pConfig.rtcmontha = 8;
    pConfig.rtcdaya   = 8;
    pConfig.rtchoura  = 8;
    pConfig.rtcmina   = 8;
    pConfig.rtcseca   = 8;
    pConfig.rtcmSeca  = 10;

    pConfig.rtcintcr  = 0x803F;

            status = RTC_config(&pConfig);
            ...
     @endverbatim
 *  ===========================================================================
 */
CSL_Status RTC_config (
               CSL_RtcConfig    *pConfig);

/** ===========================================================================
 *   @n@b RTC_getConfig
 *
 *   @b Description
 *   @n It get the RTC registers value.
 *
 *   @b Arguments
 *   @verbatim
            pGetConfig      Pointer to RTC Config structure.

     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK
 *   @li                    CSL_ESYS_INVPARAMS  - Congig Parameter is invalid.
 *
 *   <b> Pre Condition </b>
 *   @n  None.
 *
 *   <b> Post Condition </b>
 *   @n  None
 *
 *   @b Modifies
 *   @n 1. RTC Config structure will be updated with RTC Registers field.
 *
 *   @b Example
 *   @verbatim

            CSL_RtcConfig       pGetConfig;
            CSL_status           status;

            status = RTC_getConfig(&pGetConfig);
            ...
     @endverbatim
 *  ===========================================================================
 */
CSL_Status RTC_getConfig (
               CSL_RtcConfig    *pGetConfig);

/** ===========================================================================
 *   @n@b RTC_setTime
 *
 *   @b Description
 *   @n It configures the RTC registers related to time field as per passed
 *      Time structure.
 *
 *   @b Arguments
 *   @verbatim
            pRtcTime          Pointer to RTC Time structure.

     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK
 *   @li                    CSL_ESYS_INVPARAMS  - Parameter is invalid.
 *
 *   <b> Pre Condition </b>
 *   @n  NOne.
 *
 *   <b> Post Condition </b>
 *   @n  None
 *
 *   @b Modifies
 *   @n 1. RTC Time Related registers will be updated as per passed parameter.
 *
 *   @b Example
 *   @verbatim

            CSL_RtcTime       pRtcTime;
            CSL_status         status;

    pRtcTime.hours = 12;
    pRtcTime.mins = 0;
    pRtcTime.secs = 0;
    pRtcTime.mSecs =0;

            status = RTC_setTime(&pRtcTime);
            ...
     @endverbatim
 *  ===========================================================================
 */
CSL_Status RTC_setTime (
               CSL_RtcTime    *pRtcTime);

/** ===========================================================================
 *   @n@b RTC_getTime
 *
 *   @b Description
 *   @n It get the RTC registers value related to Time Field.
 *
 *   @b Arguments
 *   @verbatim
            pGetRtcTime      Pointer to RTC Time structure.

     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK
 *   @li                    CSL_ESYS_INVPARAMS  - Congig Parameter is invalid.
 *
 *   <b> Pre Condition </b>
 *   @n  NOne.
 *
 *   <b> Post Condition </b>
 *   @n  None
 *
 *   @b Modifies
 *   @n 1.Passed Parameter will be updated with RTC Time related Registers value.
 *
 *   @b Example
 *   @verbatim

            CSL_RtcTime       pGetRtcTime;
            CSL_status         status;

            status = RTC_getTime(&pGetRtcTime);
            ...
     @endverbatim
 *  ===========================================================================
 */
CSL_Status RTC_getTime (
                  CSL_RtcTime     *pGetRtcTime);

/** ===========================================================================
 *   @n@b RTC_setDate
 *
 *   @b Description
 *   @n It configures the RTC registers related to date field as per passed
 *      Time structure.
 *
 *   @b Arguments
 *   @verbatim
            pRtcDate          Pointer to RTC Time structure.

     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK
 *   @li                    CSL_ESYS_INVPARAMS  - Parameter is invalid.
 *
 *   <b> Pre Condition </b>
 *   @n  NOne.
 *
 *   <b> Post Condition </b>
 *   @n  None
 *
 *   @b Modifies
 *   @n 1. RTC Date Related registers will be updated as per passed parameter.
 *
 *   @b Example
 *   @verbatim

            CSL_RtcDate       pRtcDate;
            CSL_status         status;

    pRtcDate.year = 8;
    pRtcDate.month = 10;
    pRtcDate.day = 16;

            status = RTC_setDate(&pRtcDate);
            ...
     @endverbatim
 *  ===========================================================================
 */
CSL_Status RTC_setDate (
               CSL_RtcDate    *pRtcDate);

/** ===========================================================================
 *   @n@b RTC_getDate
 *
 *   @b Description
 *   @n It get the RTC registers value related to Date Field.
 *
 *   @b Arguments
 *   @verbatim
            pGetRtcDate      Pointer to RTC Date structure.

     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK
 *   @li                    CSL_ESYS_INVPARAMS  - Parameter is invalid.
 *
 *   <b> Pre Condition </b>
 *   @n  NOne.
 *
 *   <b> Post Condition </b>
 *   @n  None
 *
 *   @b Modifies
 *   @n 1.Passed Parameter will be updated with RTC Date related Registers value.
 *
 *   @b Example
 *   @verbatim

            CSL_RtcDate       pGetRtcDate;
            CSL_status         status;

            status = RTC_getDate(&pGetRtcDate);
            ...
     @endverbatim
 *  ===========================================================================
 */
CSL_Status RTC_getDate (
                  CSL_RtcDate     *pGetRtcDate);

/** ===========================================================================
 *   @n@b RTC_setAlarm
 *
 *   @b Description
 *   @n It configures the RTC Alarm registers as per passed Alarm Parameter.
 *
 *   @b Arguments
 *   @verbatim
            pRtcAlarm          Pointer to RTC Alarm Time structure.

     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK
 *   @li                    CSL_ESYS_INVPARAMS  - Parameter is invalid.
 *
 *   <b> Pre Condition </b>
 *   @n  NOne.
 *
 *   <b> Post Condition </b>
 *   @n  None
 *
 *   @b Modifies
 *   @n 1. RTC Alarm Related registers will be updated as per passed parameter.
 *
 *   @b Example
 *   @verbatim

            CSL_RtcAlarm  pRtcAlarm;
            CSL_status         status;

    pRtcAlarm.year = 8;
    pRtcAlarm.month = 10;
    pRtcAlarm.day = 16;
    pRtcAlarm.hours = 12;
    pRtcAlarm.mins = 1;
    pRtcAlarm.secs = 5;
    pRtcAlarm.mSecs =0;

            status = RTC_setAlarm(&pRtcAlarm);
            ...
     @endverbatim
 *  ===========================================================================
 */
CSL_Status RTC_setAlarm (
               CSL_RtcAlarm    *pRtcAlarm);

/** ===========================================================================
 *   @n@b RTC_setPeriodicInterval
 *
 *   @b Description
 *   @n It configures the periodic interrrupt for RTC.
 *
 *   @b Arguments
 *   @verbatim
            periodicInterruptType     Type of periodic interrupt.

     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK
 *   @li                    CSL_ESYS_INVPARAMS  - Parameter is invalid.
 *
 *   <b> Pre Condition </b>
 *   @n  NOne.
 *
 *   <b> Post Condition </b>
 *   @n  None
 *
 *   @b Modifies
 *   @n None
 *
 *   @b Example
 *   @verbatim

            periodicInterruptType   periodicInterruptType;
            CSL_status              status;

     periodicInterruptType = CSL_RTC_SEC_PERIODIC_INTERRUPT;

            status = RTC_setPeriodicInterval(periodicInterruptType);
            ...
     @endverbatim
 *  ===========================================================================
 */
CSL_Status RTC_setPeriodicInterval (
               CSL_RTCPeriodicInterruptType    periodicInterruptType);

/** ===========================================================================
 *   @n@b RTC_eventEnable
 *
 *   @b Description
 *   @n It enable the RTC Interrupt as per passed event paraeter.
 *
 *   @b Arguments
 *   @verbatim
            rtcEvent          rtc event type.

     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK
 *   @li                    CSL_ESYS_INVPARAMS  - Parameter is invalid.
 *
 *   <b> Pre Condition </b>
 *   @n  None.
 *
 *   <b> Post Condition </b>
 *   @n  None
 *
 *   @b Modifies
 *   @n  None
 *
 *   @b Example
 *   @verbatim

            CSL_RTCEventType   rtcEvent;
            CSL_status         status;

     rtcEvent = CSL_RTC_EXTEVENT_INTERRUPT;

            status = RTC_eventEnable(rtcEvent);
            ...
     @endverbatim
 *  ===========================================================================
 */
CSL_Status RTC_eventEnable (
               CSL_RTCEventType    rtcEvent);

/** ===========================================================================
 *   @n@b RTC_eventDisable
 *
 *   @b Description
 *   @n It disable the RTC Interrupt as per passed event paraeter.
 *
 *   @b Arguments
 *   @verbatim
            rtcEvent          rtc event type.

     @endverbatim
 *
 *   <b> Return Value </b>  CSL_Status
 *   @li                    CSL_SOK
 *   @li                    CSL_ESYS_INVPARAMS  - Parameter is invalid.
 *
 *   <b> Pre Condition </b>
 *   @n  NOne.
 *
 *   <b> Post Condition </b>
 *   @n  None
 *
 *   @b Modifies
 *   @n  None
 *
 *   @b Example
 *   @verbatim

            CSL_RTCEventType   rtcEvent;
            CSL_status         status;

     rtcEvent = CSL_RTC_EXTEVENT_INTERRUPT;

            status = RTC_eventDisable(rtcEvent);
            ...
     @endverbatim
 *  ===========================================================================
 */
CSL_Status RTC_eventDisable (
               CSL_RTCEventType    rtcEvent);

/** ===========================================================================
 *   @n@b RTC_getEventId
 *
 *   @b Description
 *   @n Function return the events of RTC(Status of RTC Event).
 *
 *   @b Arguments
 *   @verbatim
     @endverbatim
 *
 *   <b> Return Value </b>  CSL_RTCEventType
 *
 *   <b> Pre Condition </b>
 *    @n None
 *
 *   <b> Post Condition </b>
 *    @n None
 *
 *   @b Modifies
 *   @n None
 *
 *   @b Example
 *   @verbatim

            CSL_RTCEventType   eventId;

            eventId = RTC_getEventId();

     @endverbatim
 *  ===========================================================================
 */
CSL_RTCEventType RTC_getEventId(void);

/** ===========================================================================
 *   @n@b RTC_decToBcd
 *
 *   @b Description
 *   @n This function will convert decimal value to corresponding value for
 *      BCD compatible.
 *
 *   @b Arguments
 *   @verbatim
            decVal         value(RTC time and Date Fields) passed by user.

     @endverbatim
 *
 *   <b> Return Value </b>  Uint16
 *
 *   <b> Pre Condition </b>
 *   @n  NOne.
 *
 *   <b> Post Condition </b>
 *   @n  None
 *
 *   @b Modifies
 *   @n  None
 *
 *   @b Example
 *   @verbatim

            Uint16   decVal;
            Uint16   bcdVal;
    decVal = 10;
            bcdVal = RTC_decToBcd(rtcEvent);
    bcdVal = 16;
            ...
     @endverbatim
 *  ===========================================================================
 */
Uint16 RTC_decToBcd (
               Uint16    decVal);

/** ===========================================================================
 *   @n@b RTC_bcdToDec
 *
 *   @b Description
 *   @n This function will convert hex value to corresponding value for
 *      BCD compatible.
 *
 *   @b Arguments
 *   @verbatim
            hexVal          rtc(time and Date Fields) register value.

     @endverbatim
 *
 *   <b> Return Value </b>  Uint16
 *
 *   <b> Pre Condition </b>
 *   @n  NOne.
 *
 *   <b> Post Condition </b>
 *   @n  None
 *
 *   @b Modifies
 *   @n  None
 *
 *   @b Example
 *   @verbatim

            Uint16   hexVal;
            Uint16   decVal;

    hexVal = 16;
            decVal = RTC_bcdToDec(rtcEvent);
    decVal = 10;
            ...
     @endverbatim
 *  ===========================================================================
 */
Uint16 RTC_bcdToDec (
               Uint16    hexVal);

/** ============================================================================
 *   @n@b RTC_setCallback
 *
 *   @b Description
 *   @n RTC_setCallback will set call back functions in isrDispatchTable.
 *
 *   @b Arguments
 *   @verbatim
         rtcDispatchTable - pointer to the     CSL_RtcIsrDispatchTable.
         isrAddr          - pointer to the     RTC_IsrAddr.
      @endverbatim
 *
 *   <b> Return Value </b> CSL_Status
 *   @li                    CSL_SOK - RTC_setCallback call is successful.
 *   @li                    CSL_ESYS_INVPARAMS - bad parameter.
 *   <b> Pre Condition </b>
 *   @n
 *   <b> Post Condition </b>
 *   @n  Setup call back functions in the RTC_setCallback
 *
 *   @b Modifies
 *   @n rtcDispatchTable
 *
 *   @b Example
 *   @verbatim
          CSL_Status             status;
         RTC_IsrAddr             isrAddr;
         CSL_RtcIsrDispatchTable rtcDispatchTable

        status =   RTC_setCallback(&rtcDispatchTable, &isrAddr);

     @endverbatim
 *  ============================================================================
 */

CSL_Status RTC_setCallback(
               CSL_RtcIsrDispatchTable      *rtcDispatchTable,
               CSL_RtcIsrAddr                  *isrAddr           );

/** ============================================================================
 *   @n@b RTC_setCompensation
 *
 *   @b Description
 *   @n RTC_setCompensation will set RTC oscillator drift compensation register
 *      The RTC can compensate for oscillator drift due to temperature by adding
 *      or subtracting the value of the compensation registers (in milliseconds)
 *      to the 32KHz counter every hour.
 *      Compensation values range from 0 to 1024. Compenastion values passed
 *      to the RTC_setCompensation function are converted into BCD form before
 *      writing to the register.
 *      Zero compensation value is also allowed so that same API is used to
 *      remove the compensation
 *
 *      NOTE: RTC clock positive compensation doesn't work for compensation
 *      values that are multiples of 10 on the chip C5505 and C5504.
 *      This API returns error for such values. This issue is not present on
 *      chip C5515 and C5514.
 *
 *   @b Arguments
 *   @verbatim
         compType     - Type of the RTC compenstation
         compVal      - Value of the compensation
      @endverbatim
 *
 *   <b> Return Value </b> CSL_Status
 *   @li                    CSL_SOK - RTC compensation set is succesful
 *   @li                    CSL_ESYS_INVPARAMS - bad parameter.
 *
 *   <b> Pre Condition </b>
 *   @n RTC should be configured and started
 *   <b> Post Condition </b>
 *   @n  Sets the RTC compensation value
 *
 *   @b Modifies
 *   @n RTC Oscillator Drift Compensation Register
 *
 *   @b Example
 *   @verbatim
          CSL_Status    status;
         Uint16        compVal;

         compVal = 35;

        status = RTC_setCompensation(CSL_RTC_COMPENSATION_POSITIVE, compVal);

     @endverbatim
 *  ============================================================================
 */
CSL_Status RTC_setCompensation(CSL_RTCCompType    compType,
                               Uint16             compVal);


/**
@} */


#ifdef __cplusplus
}
#endif
#endif /* _CSL_RTC_H_ */


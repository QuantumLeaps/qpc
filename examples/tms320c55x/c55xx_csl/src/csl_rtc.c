/*  ===========================================================================
 *   Copyright (c) Texas Instruments Inc 2002, 2003, 2004, 2005, 2008
 *
 *   Use of this software is controlled by the terms and conditions found in
 *   the license agreement under which this software has been supplied.
 *  ===========================================================================
 */

/** @file csl_rtc.c
 *
 *  @brief RTC functional layer API defenition file
 *
 *  Path: \(CSLPATH)/src
 */

/* ============================================================================
 * Revision History
 * ================
 * 15-Oct-2008 Added RTC for CSL.
 * 04-May-2009 Added RTC_setCompensation API for C5505 PG1.4
 * 25-May-2009 Modified based on the review comments
 * ============================================================================
 */

#include "csl_rtc.h"

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
void RTC_reset(void)
{
    CSL_FINST(CSL_RTC_REGS->RTCOSC, RTC_RTCOSC_RTCRESET, SET);
}

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
void RTC_start(void)
{
    /* clear all the status bit of RTC STATUS register */
    CSL_RTC_REGS->RTCINTFL |= CSL_RTC_STATUS_MASK;
    /* enable rtc interrupt */
    CSL_FINST(CSL_RTC_REGS->RTCINTEN, RTC_RTCINTEN_RTCINTEN, ENABLED);
    /* enable rtc clock out  */
    CSL_FINST(CSL_RTC_REGS->RTCPMGT, RTC_RTCPMGT_RTCCLKOUTEN, ENABLE);
}

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
void RTC_stop(void)
{
    /* Desable the vbus interrupt for RTC */
    CSL_FINST(CSL_RTC_REGS->RTCINTEN, RTC_RTCINTEN_RTCINTEN, DISABLED);
}

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
               CSL_RtcConfig    *pConfig)
{
    volatile Uint16 timeOut;

    if (NULL == pConfig)
    {
        return CSL_ESYS_INVPARAMS;
    }
    else
    {
        timeOut = CSL_RTC_WAIT_CYCLE;
        /* before writing the time to RTC Registers clear time update bit */
        CSL_FINST(CSL_RTC_REGS->RTCUPDATE, RTC_RTCUPDATE_TIMEUPDT, CLEAR);

        /* set time registers */
        CSL_RTC_REGS->RTCMIL   = (pConfig->rtcmSec);
        CSL_RTC_REGS->RTCSEC   = (pConfig->rtcsec);
        CSL_RTC_REGS->RTCMIN   = (pConfig->rtcmin);
        CSL_RTC_REGS->RTCHOUR  = (pConfig->rtchour);
        CSL_RTC_REGS->RTCDAY   = (pConfig->rtcday);
        CSL_RTC_REGS->RTCMONTH = (pConfig->rtcmonth);
        CSL_RTC_REGS->RTCYEAR  = (pConfig->rtcyear);
        /* Time updates enabled */
        CSL_FINST(CSL_RTC_REGS->RTCUPDATE, RTC_RTCUPDATE_TIMEUPDT, SET);
        while((CSL_FEXT(CSL_RTC_REGS->RTCUPDATE, RTC_RTCUPDATE_TIMEUPDT)
                                != CSL_RTC_BIT_RESET) && (--timeOut != 0));

        timeOut = CSL_RTC_WAIT_CYCLE;
        /* before writing the Alarm time to RTC Registers clear alarm update bit */
        CSL_FINST(CSL_RTC_REGS->RTCUPDATE, RTC_RTCUPDATE_ALARMUPDT, CLEAR);

        /* set alarm registers */
        CSL_RTC_REGS->RTCMILA   = (pConfig->rtcmSeca);
        CSL_RTC_REGS->RTCSECA   = (pConfig->rtcseca);
        CSL_RTC_REGS->RTCMINA   = (pConfig->rtcmina);
        CSL_RTC_REGS->RTCHOURA  = (pConfig->rtchoura);
        CSL_RTC_REGS->RTCDAYA   = (pConfig->rtcdaya);
        CSL_RTC_REGS->RTCMONTHA = (pConfig->rtcmontha);
        CSL_RTC_REGS->RTCYEARA  = (pConfig->rtcyeara);
        /* Time Alarm updates enabled */
        CSL_FINST(CSL_RTC_REGS->RTCUPDATE, RTC_RTCUPDATE_ALARMUPDT, SET);
        while((CSL_FEXT(CSL_RTC_REGS->RTCUPDATE, RTC_RTCUPDATE_ALARMUPDT)
                                != CSL_RTC_BIT_RESET) && (--timeOut != 0));
        /* set interrupt register */
        CSL_RTC_REGS->RTCINTREG = (pConfig->rtcintcr);

        return CSL_SOK;
    }
}

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
               CSL_RtcConfig    *pGetConfig)
{
    if (NULL == pGetConfig)
    {
        return CSL_ESYS_INVPARAMS;
    }
    else
    {
        /* get time registers field*/
        pGetConfig->rtcmSec         = CSL_RTC_REGS->RTCMIL ;
        pGetConfig->rtcsec         = CSL_RTC_REGS->RTCSEC ;
        pGetConfig->rtcmin         = CSL_RTC_REGS->RTCMIN ;
        pGetConfig->rtchour        = CSL_RTC_REGS->RTCHOUR ;
        pGetConfig->rtcday        = CSL_RTC_REGS->RTCDAY ;
        pGetConfig->rtcmonth     = CSL_RTC_REGS->RTCMONTH ;
        pGetConfig->rtcyear     = CSL_RTC_REGS->RTCYEAR ;

        /* get alarm registers field */
        pGetConfig->rtcmSeca    = CSL_RTC_REGS->RTCMILA ;
        pGetConfig->rtcseca     = CSL_RTC_REGS->RTCSECA ;
        pGetConfig->rtcmina     = CSL_RTC_REGS->RTCMINA ;
        pGetConfig->rtchoura     = CSL_RTC_REGS->RTCHOURA ;
        pGetConfig->rtcdaya     = CSL_RTC_REGS->RTCDAYA ;
        pGetConfig->rtcmontha     = CSL_RTC_REGS->RTCMONTHA ;
        pGetConfig->rtcyeara     = CSL_RTC_REGS->RTCYEARA ;

        /* get interrupt register fields */
        pGetConfig->rtcintcr = CSL_RTC_REGS->RTCINTREG ;

        return CSL_SOK;
    }
}

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
               CSL_RtcTime    *pRtcTime)
{
    volatile Uint16 timeOut;

    if (NULL == pRtcTime)
    {
        return CSL_ESYS_INVPARAMS;
    }
    else
    {
        timeOut = CSL_RTC_WAIT_CYCLE;
        /* before writing the time to RTC Registers clear time update bit */
        CSL_FINST(CSL_RTC_REGS->RTCUPDATE, RTC_RTCUPDATE_TIMEUPDT, CLEAR);

        CSL_RTC_REGS->RTCMIL  = RTC_decToBcd((pRtcTime)->mSecs);
        CSL_RTC_REGS->RTCSEC  = RTC_decToBcd((pRtcTime)->secs);
        CSL_RTC_REGS->RTCMIN  = RTC_decToBcd((pRtcTime)->mins);
        CSL_RTC_REGS->RTCHOUR = RTC_decToBcd((pRtcTime)->hours);

        /* Time updates enabled */
        CSL_FINST(CSL_RTC_REGS->RTCUPDATE, RTC_RTCUPDATE_TIMEUPDT, SET);
        while((CSL_FEXT(CSL_RTC_REGS->RTCUPDATE, RTC_RTCUPDATE_TIMEUPDT)
                                != CSL_RTC_BIT_RESET) && (--timeOut != 0));
        return CSL_SOK;
    }
}

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
                  CSL_RtcTime     *pGetRtcTime)
{
    if (NULL == pGetRtcTime)
    {
        return CSL_ESYS_INVPARAMS;
    }
    else
    {
        pGetRtcTime->mSecs  = RTC_bcdToDec(CSL_RTC_REGS->RTCMIL);
        pGetRtcTime->secs   = RTC_bcdToDec(CSL_RTC_REGS->RTCSEC);
        pGetRtcTime->mins   = RTC_bcdToDec(CSL_RTC_REGS->RTCMIN);
        pGetRtcTime->hours  = RTC_bcdToDec(CSL_RTC_REGS->RTCHOUR);

        return CSL_SOK;
    }
}

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
               CSL_RtcDate    *pRtcDate)
{
    volatile Uint16 timeOut;

    if (NULL == pRtcDate)
    {
        return CSL_ESYS_INVPARAMS;
    }
    else
    {
        timeOut = CSL_RTC_WAIT_CYCLE;
        /* before writing the time to RTC Registers clear time update bit */
        CSL_FINST(CSL_RTC_REGS->RTCUPDATE, RTC_RTCUPDATE_TIMEUPDT, CLEAR);

        CSL_RTC_REGS->RTCDAY   = RTC_decToBcd((pRtcDate)->day);
        CSL_RTC_REGS->RTCMONTH = RTC_decToBcd((pRtcDate)->month);
        CSL_RTC_REGS->RTCYEAR  = RTC_decToBcd((pRtcDate)->year);

        /* Time updates enabled */
        CSL_FINST(CSL_RTC_REGS->RTCUPDATE, RTC_RTCUPDATE_TIMEUPDT, SET);
        while((CSL_FEXT(CSL_RTC_REGS->RTCUPDATE, RTC_RTCUPDATE_TIMEUPDT)
                                != CSL_RTC_BIT_RESET) && (--timeOut != 0));
        return CSL_SOK;
    }
}

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
                  CSL_RtcDate     *pGetRtcDate)
{
    if (NULL == pGetRtcDate)
    {
        return CSL_ESYS_INVPARAMS;
    }
    else
    {
        pGetRtcDate->day    = RTC_bcdToDec(CSL_RTC_REGS->RTCDAY);
        pGetRtcDate->month  = RTC_bcdToDec(CSL_RTC_REGS->RTCMONTH);
        pGetRtcDate->year   = RTC_bcdToDec(CSL_RTC_REGS->RTCYEAR);

        return CSL_SOK;
    }
}

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
               CSL_RtcAlarm    *pRtcAlarm)
{
    volatile Uint16 timeOut;

    if (NULL == pRtcAlarm)
    {
        return CSL_ESYS_INVPARAMS;
    }
    else
    {
        timeOut = CSL_RTC_WAIT_CYCLE;

        /* before writing the Alarm time to RTC Registers clear alarm update bit */
        CSL_FINST(CSL_RTC_REGS->RTCUPDATE, RTC_RTCUPDATE_ALARMUPDT, CLEAR);

        CSL_RTC_REGS->RTCMILA   = RTC_decToBcd((pRtcAlarm)->mSecs);
        CSL_RTC_REGS->RTCSECA   = RTC_decToBcd((pRtcAlarm)->secs);
        CSL_RTC_REGS->RTCMINA   = RTC_decToBcd((pRtcAlarm)->mins);
        CSL_RTC_REGS->RTCHOURA  = RTC_decToBcd((pRtcAlarm)->hours);
        CSL_RTC_REGS->RTCDAYA   = RTC_decToBcd((pRtcAlarm)->day);
        CSL_RTC_REGS->RTCMONTHA = RTC_decToBcd((pRtcAlarm)->month);
        CSL_RTC_REGS->RTCYEARA  = RTC_decToBcd((pRtcAlarm)->year);

        /* Time Alarm updates enabled */
        CSL_FINST(CSL_RTC_REGS->RTCUPDATE, RTC_RTCUPDATE_ALARMUPDT, SET);
        while((CSL_FEXT(CSL_RTC_REGS->RTCUPDATE, RTC_RTCUPDATE_ALARMUPDT)
                                != CSL_RTC_BIT_RESET) && (--timeOut != 0));
        /* Enable Alarm Interrupt */
        CSL_FINST(CSL_RTC_REGS->RTCINTREG, RTC_RTCINTREG_ALARMINTEN, SET);

        /* delay is required to stablize the register value */
        for (timeOut = 0; timeOut < CSL_RTC_WAIT_CYCLE; timeOut++);

        /* Disable PMGT direction field for Wake up the corazon */
        CSL_FINST(CSL_RTC_REGS->RTCPMGT, RTC_RTCPMGT_WU_DIR, IN);

        return CSL_SOK;
    }
}

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
               CSL_RTCPeriodicInterruptType    periodicInterruptType)
{
    volatile Uint16 delay;

    switch (periodicInterruptType)
    {
        case CSL_RTC_DAY_PERIODIC_INTERRUPT:
        {
            CSL_FINST(CSL_RTC_REGS->RTCINTREG, RTC_RTCINTREG_DAYINTEN, SET);
            break;
        }

        case CSL_RTC_HR_PERIODIC_INTERRUPT:
        {
            CSL_FINST(CSL_RTC_REGS->RTCINTREG, RTC_RTCINTREG_HOURINTEN, SET);
            break;
        }

        case CSL_RTC_MINS_PERIODIC_INTERRUPT:
        {
            CSL_FINST(CSL_RTC_REGS->RTCINTREG, RTC_RTCINTREG_MININTEN, SET);
            break;
        }

        case CSL_RTC_SEC_PERIODIC_INTERRUPT:
        {
            CSL_FINST(CSL_RTC_REGS->RTCINTREG, RTC_RTCINTREG_SECINTEN, SET);
            break;
        }

        case CSL_RTC_MS_PERIODIC_INTERRUPT:
        {
            CSL_FINST(CSL_RTC_REGS->RTCINTREG, RTC_RTCINTREG_MSINTEN, SET);
            break;
        }

        default:
        {
            return CSL_ESYS_INVPARAMS;
        }
    }
    /* delay is required to stablize the register value */
    for (delay = 0; delay < CSL_RTC_WAIT_CYCLE; delay++);
    return CSL_SOK;
}

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
               CSL_RTCEventType    rtcEvent)
{
    volatile Uint16 delay;

    switch (rtcEvent)
    {
        case CSL_RTC_ALARM_INTERRUPT:
        {
            CSL_FINST(CSL_RTC_REGS->RTCINTREG, RTC_RTCINTREG_ALARMINTEN, SET);
            break;
        }

        case CSL_RTC_EXTEVENT_INTERRUPT:
        {
            CSL_FINST(CSL_RTC_REGS->RTCINTREG, RTC_RTCINTREG_EXTINTEN, SET);
            break;
        }

        case CSL_RTC_DAYEVENT_INTERRUPT:
        {
            CSL_FINST(CSL_RTC_REGS->RTCINTREG, RTC_RTCINTREG_DAYINTEN, SET);
            break;
        }

        case CSL_RTC_HREVENT_INTERRUPT:
        {
            CSL_FINST(CSL_RTC_REGS->RTCINTREG, RTC_RTCINTREG_HOURINTEN, SET);
            break;
        }

        case CSL_RTC_MINSEVENT_INTERRUPT:
        {
            CSL_FINST(CSL_RTC_REGS->RTCINTREG, RTC_RTCINTREG_MININTEN, SET);
            break;
        }

        case CSL_RTC_SECEVENT_INTERRUPT:
        {
            CSL_FINST(CSL_RTC_REGS->RTCINTREG, RTC_RTCINTREG_SECINTEN, SET);
            break;
        }

        case CSL_RTC_MSEVENT_INTERRUPT:
        {
            CSL_FINST(CSL_RTC_REGS->RTCINTREG, RTC_RTCINTREG_MSINTEN, SET);
            break;
        }

        default:
        {
            return CSL_ESYS_INVPARAMS;
        }
    }
    /* delay is required to stablize the register value */
    for (delay = 0; delay < CSL_RTC_WAIT_CYCLE; delay++);
    return CSL_SOK;
}

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
               CSL_RTCEventType    rtcEvent)
{
    volatile Uint16 delay;

    switch (rtcEvent)
    {
        case CSL_RTC_ALARM_INTERRUPT:
        {
            CSL_FINST(CSL_RTC_REGS->RTCINTREG, RTC_RTCINTREG_ALARMINTEN, CLEAR);
            break;
        }

        case CSL_RTC_EXTEVENT_INTERRUPT:
        {
            CSL_FINST(CSL_RTC_REGS->RTCINTREG, RTC_RTCINTREG_EXTINTEN, CLEAR);
            break;
        }

        case CSL_RTC_DAYEVENT_INTERRUPT:
        {
            CSL_FINST(CSL_RTC_REGS->RTCINTREG, RTC_RTCINTREG_DAYINTEN, CLEAR);
            break;
        }

        case CSL_RTC_HREVENT_INTERRUPT:
        {
            CSL_FINST(CSL_RTC_REGS->RTCINTREG, RTC_RTCINTREG_HOURINTEN, CLEAR);
            break;
        }

        case CSL_RTC_MINSEVENT_INTERRUPT:
        {
            CSL_FINST(CSL_RTC_REGS->RTCINTREG, RTC_RTCINTREG_MININTEN, CLEAR);
            break;
        }

        case CSL_RTC_SECEVENT_INTERRUPT:
        {
            CSL_FINST(CSL_RTC_REGS->RTCINTREG, RTC_RTCINTREG_SECINTEN, CLEAR);
            break;
        }

        case CSL_RTC_MSEVENT_INTERRUPT:
        {
            CSL_FINST(CSL_RTC_REGS->RTCINTREG, RTC_RTCINTREG_MSINTEN, CLEAR);
            break;
        }

        default:
        {
            return CSL_ESYS_INVPARAMS;
        }
    }
    /* delay is required to stablize the register value */
    for (delay = 0; delay < CSL_RTC_WAIT_CYCLE; delay++);
    return CSL_SOK;
}

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
CSL_RTCEventType RTC_getEventId(void)
{

    if(CSL_RTC_REGS->RTCINTFL & CSL_RTC_RTCINTFL_ALARMFL_MASK)
        return CSL_RTC_ALARM_INTERRUPT;
    if(CSL_RTC_REGS->RTCINTFL & CSL_RTC_RTCINTFL_EXTFL_MASK)
        return CSL_RTC_EXTEVENT_INTERRUPT;
    if(CSL_RTC_REGS->RTCINTFL & CSL_RTC_RTCINTFL_DAYFL_MASK)
        return CSL_RTC_DAYEVENT_INTERRUPT;
    if(CSL_RTC_REGS->RTCINTFL & CSL_RTC_RTCINTFL_HOURFL_MASK)
        return CSL_RTC_HREVENT_INTERRUPT;
    if(CSL_RTC_REGS->RTCINTFL & CSL_RTC_RTCINTFL_MINFL_MASK)
        return CSL_RTC_MINSEVENT_INTERRUPT;
    if(CSL_RTC_REGS->RTCINTFL & CSL_RTC_RTCINTFL_SECFL_MASK)
        return CSL_RTC_SECEVENT_INTERRUPT;
    if(CSL_RTC_REGS->RTCINTFL & CSL_RTC_RTCINTFL_MSFL_MASK)
        return CSL_RTC_MSEVENT_INTERRUPT;

    return CSL_RTC_INTERRUPT_NONE;
}

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
               Uint16    decVal)
{
    Uint16 hexVal;
    Uint16 tempMod;
    Uint16 i;

    hexVal = 0;
    for (i = 0; decVal; i++)
    {
         tempMod  = rtcModuloDivFun(decVal, 10);
        decVal   = rtcDivFun(decVal, 10);
        hexVal   = hexVal + (tempMod * rtc_pow(16,i));
    }
    return hexVal;
}

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
               Uint16    hexVal)
{
    Uint16 decVal;
    Uint16 tempMod;
    Uint16 i;

    decVal = 0;
    for ( i = 0; hexVal; i++)
    {
         tempMod = rtcModuloDivFun(hexVal, 16);
        hexVal  = rtcDivFun(hexVal, 16);
        decVal  = decVal + (tempMod * rtc_pow(10, i));
    }
    return decVal;
}

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
               CSL_RtcIsrAddr                  *isrAddr           )
{
    if((NULL != rtcDispatchTable) && (NULL != isrAddr))
    {
        rtcDispatchTable->isr[CSL_RTC_MSEVENT_INTERRUPT]   = isrAddr->MilEvtAddr;
        rtcDispatchTable->isr[CSL_RTC_SECEVENT_INTERRUPT]  = isrAddr->SecEvtAddr;
        rtcDispatchTable->isr[CSL_RTC_MINSEVENT_INTERRUPT] = isrAddr->MinEvtAddr;
        rtcDispatchTable->isr[CSL_RTC_HREVENT_INTERRUPT]   = isrAddr->HourEvtAddr;
        rtcDispatchTable->isr[CSL_RTC_DAYEVENT_INTERRUPT]  = isrAddr->DayEvtAddr;
        rtcDispatchTable->isr[CSL_RTC_EXTEVENT_INTERRUPT]  = isrAddr->ExtEvtAddr;
        rtcDispatchTable->isr[CSL_RTC_ALARM_INTERRUPT]     = isrAddr->AlarmEvtAddr;
        return CSL_SOK;
    }
    else
    {
        if(NULL == rtcDispatchTable)
        {
            return CSL_ESYS_BADHANDLE;
        }
        else
        {
            return CSL_ESYS_INVPARAMS;
        }
    }
}

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
                               Uint16             compVal)
{
    CSL_Status    status;
    Uint16        regVal;

    status = CSL_SOK;
    regVal = 0;

    /*
     * Verify the input parameters
     * Compensation value should be less than or equal to 1024
     * Compensation type should be POSITIVE or NEGATIVE
     */
     /*
      * compVal should not be multiple of 10 for POSITIVE compensation
      * on the chip C5505
      */
#if (defined(CHIP_C5505_C5515) || defined(CHIP_C5504_C5514))

    if((compVal <= CSL_RTC_COMPENSATION_MAX) &&
       ((CSL_RTC_COMPENSATION_NEGATIVE == compType) ||
        (CSL_RTC_COMPENSATION_POSITIVE == compType)))

#else

    if((compVal <= CSL_RTC_COMPENSATION_MAX) &&
       ((CSL_RTC_COMPENSATION_NEGATIVE == compType) ||
       ((CSL_RTC_COMPENSATION_POSITIVE == compType) &&
       ((compVal % 10) != 0))))

#endif
    {
        regVal = CSL_FMK(RTC_RTCDRIFT_DRIFT, compType);

        regVal |= RTC_decToBcd(compVal);

        /* Configure the RTC compensation register */
        CSL_RTC_REGS->RTCDRIFT = regVal;
    }
    else
    {
        status = CSL_ESYS_INVPARAMS;
    }

    return (status);
}


/**
* @file
* @brief QF/C generic port to TI-RTOS, all supported compilers
* @ingroup ports
* @cond
******************************************************************************
* Last updated for version 6.4.0
* Last updated on  2019-02-07
*
*                    Q u a n t u m  L e a P s
*                    ------------------------
*                    Modern Embedded Software
*
* Copyright (C) 2005-2019 Quantum Leaps, LLC. All rights reserved.
*
* This program is open source software: you can redistribute it and/or
* modify it under the terms of the GNU General Public License as published
* by the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* Alternatively, this program may be distributed and modified under the
* terms of Quantum Leaps commercial licenses, which expressly supersede
* the GNU General Public License and are specifically designed for
* licensees interested in retaining the proprietary status of their code.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*
* Contact information:
* https://www.state-machine.com
* mailto:info@state-machine.com
******************************************************************************
* @endcond
*/
#define QP_IMPL           /* this is QP implementation */
#include "qf_port.h"      /* QF port */
#include "qf_pkg.h"
#include "qassert.h"
#ifdef Q_SPY              /* QS software tracing enabled? */
    #include "qs_port.h"  /* include QS port */
#else
    #include "qs_dummy.h" /* disable the QS software tracing */
#endif /* Q_SPY */

#include <ti/sysbios/BIOS.h> /* SYS/BIOS API (BIOS_start()) */


Q_DEFINE_THIS_MODULE("qf_port")

/* Local objects -----------------------------------------------------------*/
static void swi_function(UArg arg0, UArg arg1); /* TI-RTOS Swi signature */

/*..........................................................................*/
void QF_init(void) {
}
/*..........................................................................*/
int_t QF_run(void) {
    QF_onStartup();  /* configure & start interrupts */
    BIOS_start();    /* start TI-RTOT (SYS/BIOS) */

    Q_ERROR_ID(100); /* BIOS_start() should never return */
    return (int_t)0; /* this unreachable return keeps the compiler happy */
}
/*..........................................................................*/
void QF_stop(void) {
    QF_onCleanup();  /* cleanup callback */
}

/*..........................................................................*/
void QActive_start_(QActive * const me, uint_fast8_t prio,
                    QEvt const *qSto[], uint_fast16_t qLen,
                    void *stkSto, uint_fast16_t stkSize,
                    QEvt const *ie)
{
    Swi_Params swiParams;

    /** @pre the priority must be in range and the stack storage must not
    * be provided, because the TI-RTOS Swis do not need per-Swi stacks.
    */
    Q_REQUIRE_ID(400, ((uint_fast8_t)0 < prio)
                 && (prio <= (uint_fast8_t)QF_MAX_ACTIVE)
                 && (stkSto == (void *)0));

    (void)stkSize; /* avoid the "unused parameter" compiler warning */

    QEQueue_init(&me->eQueue, qSto, qLen);
    me->prio = prio; /* set QF priority of this AO before adding it to QF */
    QF_add_(me);     /* make QF aware of this active object */
    QHSM_INIT(&me->super, ie); /* take the top-most initial tran. */

    QS_FLUSH(); /* flush the QS trace buffer to the host */

    /* create TI-RTOS Swi to run this active object... */
    Swi_Params_init(&swiParams);
    swiParams.arg0 = (xdc_UArg)me; /* the "me" pointer */
    swiParams.arg1 = 0;        /* not used */
    swiParams.priority = prio; /* TI-RTOS Swis can use the QP priority */
    swiParams.trigger = 0;     /* not used */
    Swi_construct(&me->osObject, &swi_function, &swiParams, NULL);
    me->thread = Swi_handle(&me->osObject);

    /* TI-RTOS Swi must be created correctly */
    Q_ENSURE_ID(490, me->thread != 0);
}

/*..........................................................................*/
static void swi_function(UArg arg0, UArg arg1) { /* TI-RTOS Swi signature */
    QActive *act = (QActive *)arg0;
    QEvt const *e = QActive_get_(act);
    QHSM_DISPATCH(&act->super, e); /* dispatch to the AO's SM */
    QF_gc(e); /* check if the event is garbage, and collect it if so */
    (void)arg1; /* unused parameter */

    /* are events still available for this AO? */
    if (act->eQueue.frontEvt != (QEvt *)0) {
        Swi_post(act->thread);
    }
}

/*****************************************************************************
*/

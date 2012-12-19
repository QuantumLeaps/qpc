/*****************************************************************************
* Product: DPP example
* Last Updated for Version: 4.5.02
* Date of the Last Update:  May 18, 2012
*
*                    Q u a n t u m     L e a P s
*                    ---------------------------
*                    innovating embedded systems
*
* Copyright (C) 2002-2012 Quantum Leaps, LLC. All rights reserved.
*
* This program is open source software: you can redistribute it and/or
* modify it under the terms of the GNU General Public License as published
* by the Free Software Foundation, either version 2 of the License, or
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
* Quantum Leaps Web sites: http://www.quantum-leaps.com
*                          http://www.state-machine.com
* e-mail:                  info@quantum-leaps.com
*****************************************************************************/
#include "qp_port.h"
#include "bsp.h"
#include "dpp.h"
#include "display.h"

#include <stdlib.h>
#include <android/log.h>

Q_DEFINE_THIS_MODULE("bsp")

/* Local-objects -----------------------------------------------------------*/

#ifdef Q_SPY
    enum {
        PHILO_STAT = QS_USER
    };

    static uint8_t const l_clock_tick = 0U;
    static void *idleThread(void *par);
#endif

static uint8_t const l_ticker = 0U;

static struct engine l_engine;         /* display engine status and config  */
static volatile uint8_t l_running;                      /* main loop status */
static volatile uint8_t l_tick_paused;                    /* Ticker paused? */
static uint32_t l_rnd;                  /* random seed                      */

static pthread_mutex_t tick_routine_mutex;    /* tick routine control mutex */
static pthread_cond_t  tick_routine_cond; /* tick routine control condition */
static pthread_t tick_thread;             /* tick routine pthread id        */

static int32_t handle_input(struct android_app* app, AInputEvent* event);
static void handle_cmd(struct android_app* app, int32_t cmd);
static void *tick_routine(void *arg);
static void pause_tick_routine(void);
static void resume_tick_routine(void);

extern char __bss_start;                                      /* See NOTE01 */
extern char __bss_end__;

/*..........................................................................*/
void Q_onAssert(char const * const file, int line) {
                               /* log assert terminates process with Sig 11 */
    __android_log_assert("Q_onAssert", "dpp", "Q_onAssert file: %s, line: %d",
                         file, line);
    QF_stop();
}

/*..........................................................................*/
uint32_t BSP_random(void) {
    /* a very cheap pseudo-random-number generator */
    /* "Super-Duper" Linear Congruential Generator (LCG)
    * LCG(2^32, 3*7*11*13*23, 0, seed) */
    l_rnd = l_rnd * (3*7*11*13*23);
    return l_rnd >> 8;
}

/*..........................................................................*/
void BSP_randomSeed(uint32_t seed) {
    l_rnd = seed;
}

/*..........................................................................*/
void BSP_terminate(int16_t result) {
    (void)result;
    QF_stop();
}

/*..........................................................................*/
void BSP_displayPhilStat(uint8_t n, char const *stat) {

    l_engine.state.philo_status[n] = stat[0];

    QS_BEGIN(PHILO_STAT, AO_Philo[n])  /* application-specific record begin */
        QS_U8(1, n);                                  /* Philosopher number */
        QS_STR(stat);                                 /* Philosopher status */
    QS_END()
}

/*..........................................................................*/
void BSP_displayPaused(uint8_t paused) {
    l_engine.state.table_paused = paused;
}

/*..........................................................................*/
void BSP_init(void) {
#if defined(Q_SPY)
    if (!QS_INIT((void*) 0)) {
        __android_log_assert("bsp_init", "dpp", "QS_INIT failed");
    }
#endif
}

/**
 * Process the next input event.
 */
static int32_t handle_input(struct android_app* app,
                                   AInputEvent* event) {
    struct engine* engine = (struct engine*)app->userData;
    int32_t status = 0;

    if (AInputEvent_getType(event) == AINPUT_EVENT_TYPE_MOTION) {

        switch (AMOTION_EVENT_ACTION_MASK & AMotionEvent_getAction( event )) {

            case AMOTION_EVENT_ACTION_DOWN:
            case AMOTION_EVENT_ACTION_POINTER_DOWN: {
                             /* "tap" on the screen causes pause operation */
                QF_PUBLISH(Q_NEW(QEvt, PAUSE_SIG), &handle_input);
                break;
            }
            default:
                break;
        }
        engine->animating = 1U;       /* any motion event resumes animation */
        status = 1;                                  /* event was processed */
    }
    return status;
}

/*..........................................................................*/
static void pause_tick_routine(void) {
    Q_ALLEGE(pthread_mutex_lock(&tick_routine_mutex) == 0);
    l_tick_paused = 1U;
    Q_ALLEGE(pthread_cond_signal(&tick_routine_cond) == 0);
    Q_ALLEGE(pthread_mutex_unlock(&tick_routine_mutex) == 0);
}

/*..........................................................................*/
static void resume_tick_routine(void) {
    Q_ALLEGE(pthread_mutex_lock(&tick_routine_mutex) == 0);
    l_tick_paused = 0U;
    Q_ALLEGE(pthread_cond_signal(&tick_routine_cond) == 0);
    Q_ALLEGE(pthread_mutex_unlock(&tick_routine_mutex) == 0);
}

/**
 * Process the next main command.
 */
static void handle_cmd(struct android_app* app, int32_t cmd)
{
    struct engine* engine = (struct engine*)app->userData;

    switch (cmd) {
        case APP_CMD_SAVE_STATE: {      /* The system hints saving of state */
            engine->app->savedState = malloc(sizeof(struct saved_state));
            *((struct saved_state*)engine->app->savedState) = engine->state;
            engine->app->savedStateSize = sizeof(struct saved_state);
            break;
        }
        case APP_CMD_INIT_WINDOW: {            /* The window is being shown */
            if (engine->app->window != NULL) {
                init_display(engine);
                draw_frame(engine);
            }
            break;
        }
        case APP_CMD_TERM_WINDOW: { /* The window is being hidden or closed */
            term_display(engine);
            break;
        }
        case APP_CMD_GAINED_FOCUS: {
            engine->animating = 1U;
            resume_tick_routine();
            break;
        }
        case APP_CMD_LOST_FOCUS: {
            engine->animating = 0U;
            draw_frame(engine);
            pause_tick_routine();
            break;
        }
    }
}


/*..........................................................................*/
static void *tick_routine(void *arg) {

    struct timespec tim, tim2;
    tim.tv_sec = 0;
    tim.tv_nsec = (1000000000LL) / (BSP_TICKS_PER_SEC);

    while (l_running != 0U) {
        QF_TICK(&l_ticker);                          /* process a time tick */
        nanosleep(&tim , &tim2);

        /* If we are paused: wait for condition to avoid CPU burn */
        Q_ALLEGE(pthread_mutex_lock(&tick_routine_mutex) == 0);
        while ((l_running != 0U) && (l_tick_paused != 0U)) {
            Q_ALLEGE(pthread_cond_wait(&tick_routine_cond,
                                       &tick_routine_mutex) == 0);
        }
        Q_ALLEGE(pthread_mutex_unlock(&tick_routine_mutex) == 0);
    };

    return (void *)0;                                     /* return success */
}

/*..........................................................................*/
void QF_init(void) {
    Q_ALLEGE(pthread_mutex_init(&QF_pThreadMutex_, NULL) == 0);
    Q_ALLEGE(pthread_mutex_init(&tick_routine_mutex, NULL) == 0);
    Q_ALLEGE(pthread_cond_init(&tick_routine_cond, NULL) == 0);
}

/*..........................................................................*/
void QF_stop(void) {
    l_running = 0U;
}

/*..........................................................................*/
void QF_onStartup(void) {
    l_running = 1U;
    l_tick_paused = 0U;
    Q_ALLEGE(pthread_create(&tick_thread, NULL, tick_routine, (void*) 0)== 0);

}

/*..........................................................................*/
void QF_onCleanup(void) {
    int status, n;

    term_display(&l_engine);

    /* Ensure tick thread is not sitting at paused state. */
    Q_ALLEGE(pthread_mutex_lock(&tick_routine_mutex) == 0);
    l_tick_paused = 0U;
    Q_ALLEGE(pthread_cond_signal(&tick_routine_cond) == 0);
    Q_ALLEGE(pthread_mutex_unlock(&tick_routine_mutex) == 0);

    /* Wait until tick thread finishes */
    Q_ALLEGE(pthread_join(tick_thread, (void*) &status) == 0);

    Q_ALLEGE(pthread_cond_destroy(&tick_routine_cond) == 0);
    Q_ALLEGE(pthread_mutex_destroy(&tick_routine_mutex) == 0);

    /* Wait until AO threads finish */
    Q_ALLEGE(pthread_join(AO_Table->thread.thread, (void*) &status) == 0);

    for (n = 0; n < N_PHILO; n++) {
        Q_ALLEGE(pthread_join(AO_Philo[n]->thread.thread,
                 (void*) &status) == 0);
    }

    Q_ALLEGE(pthread_mutex_destroy(&QF_pThreadMutex_) == 0);

    QS_EXIT();
}


/*..........................................................................*/
int16_t QF_run(void) {

    QF_onStartup();                              /* invoke startup callback */

    QS_OBJ_DICTIONARY(&l_ticker);       /* the QS dictionary for the ticker */

    while (l_running != 0U) {

        int ident;
        int events;
        struct android_poll_source* source;

        /* If not animating, we will block forever waiting for events.
          If animating, we loop until all events are read, then continue
          to draw the next frame of animation. */
        while ((ident=ALooper_pollAll(l_engine.animating ? 0 : -1,
                                      NULL,
                                      &events,
                                      (void**)&source)) >= 0) {

            if (source != NULL) {
                source->process(l_engine.app, source);
            }

            // Check for exit
            if (l_engine.app->destroyRequested != 0) {
                QF_PUBLISH(Q_NEW(QEvt, TERMINATE_SIG), &QF_run);
                QF_stop();
                break;
            }
        }


        if (l_engine.animating) {
            /* draw next animation frame */
            draw_frame(&l_engine);
        }
    }

    QF_onCleanup();
    return 0;
}

/**
 * This is the main entry point of a native application that is using
 * android_native_app_glue.  It runs in its own thread, with its own
 * event loop for receiving input events and doing other things.
 */
void android_main(struct android_app* state) {

    app_dummy();           /* make sure android native glue is not stripped */

    {
        char *ptr;

        for (ptr = &__bss_start; ptr != &__bss_end__; ptr++ ) {/* See NOTE01*/
            *ptr = 0;
        }
    }

    memset(&l_engine, 0, sizeof(l_engine));
    state->userData = &l_engine;
    state->onAppCmd = handle_cmd;
    state->onInputEvent = handle_input;

    l_engine.app = state;
    l_running = 0U;
    l_tick_paused = 0U;

    if (state->savedState != NULL) {
           /* We are starting with a previous saved state; restore from it. */
        l_engine.state = *(struct saved_state*)state->savedState;
    }

    {
        char **param = {NULL};
        main(0, param);               /* call main with expected parameters */
    }
}


/*--------------------------------------------------------------------------*/
#ifdef Q_SPY                                         /* define QS callbacks */

#include "qspy.h"
#include <android/log.h>

#define LOGI(...) \
    ((void)__android_log_print(ANDROID_LOG_INFO, "dpp", __VA_ARGS__))

static volatile uint8_t l_idle_running;
static pthread_t idle_thread;

/*..........................................................................*/
static void *idleThread(void *par) {     /* the expected P-Thread signature */
    (void)par;
    static struct timespec tim, tim2;
    tim.tv_sec = 0;
    tim.tv_nsec = 10000000UL;

    l_idle_running = (uint8_t)1;

    while (l_idle_running) {
        uint16_t nBytes = 256;
        uint8_t const *block;

        QF_CRIT_ENTRY(dummy);
        block = QS_getBlock(&nBytes);
        QF_CRIT_EXIT(dummy);

        if (block != (uint8_t *)0) {
            QSPY_parse(block, nBytes);
        }
        else {
            nanosleep(&tim, &tim2);
        }

    }

    return 0;                                             /* return success */
}
/*..........................................................................*/
uint8_t QS_onStartup(void const *arg) {
    static uint8_t qsBuf[10*1024];             /* 4K buffer for Quantum Spy */

    QS_initBuf(qsBuf, sizeof(qsBuf));

    QSPY_config((QP_VERSION >> 8),            /* version                    */
                QS_OBJ_PTR_SIZE,              /* objPtrSize                 */
                QS_FUN_PTR_SIZE,              /* funPtrSize                 */
                QS_TIME_SIZE,                 /* tstampSize                 */
                Q_SIGNAL_SIZE,                /* sigSize,                   */
                QF_EVENT_SIZ_SIZE,            /* evtSize                    */
                QF_EQUEUE_CTR_SIZE,           /* queueCtrSize               */
                QF_MPOOL_CTR_SIZE,            /* poolCtrSize                */
                QF_MPOOL_SIZ_SIZE,            /* poolBlkSize                */
                QF_TIMEEVT_CTR_SIZE,          /* tevtCtrSize                */
                (void *)0,                    /* matFile,                   */
                (void *)0,
                (QSPY_CustParseFun)0);        /* customized parser function */

    QS_FILTER_ON(QS_ALL_RECORDS);

//    QS_FILTER_OFF(QS_QEP_STATE_EMPTY);
//    QS_FILTER_OFF(QS_QEP_STATE_ENTRY);
//    QS_FILTER_OFF(QS_QEP_STATE_EXIT);
//    QS_FILTER_OFF(QS_QEP_STATE_INIT);
//    QS_FILTER_OFF(QS_QEP_INIT_TRAN);
//    QS_FILTER_OFF(QS_QEP_INTERN_TRAN);
//    QS_FILTER_OFF(QS_QEP_TRAN);
//    QS_FILTER_OFF(QS_QEP_IGNORED);
//    QS_FILTER_OFF(QS_QEP_DISPATCH);
//    QS_FILTER_OFF(QS_QEP_UNHANDLED);

    QS_FILTER_OFF(QS_QF_ACTIVE_ADD);
    QS_FILTER_OFF(QS_QF_ACTIVE_REMOVE);
    QS_FILTER_OFF(QS_QF_ACTIVE_SUBSCRIBE);
//    QS_FILTER_OFF(QS_QF_ACTIVE_UNSUBSCRIBE);
    QS_FILTER_OFF(QS_QF_ACTIVE_POST_FIFO);
//    QS_FILTER_OFF(QS_QF_ACTIVE_POST_LIFO);
    QS_FILTER_OFF(QS_QF_ACTIVE_GET);
    QS_FILTER_OFF(QS_QF_ACTIVE_GET_LAST);
    QS_FILTER_OFF(QS_QF_EQUEUE_INIT);
    QS_FILTER_OFF(QS_QF_EQUEUE_POST_FIFO);
    QS_FILTER_OFF(QS_QF_EQUEUE_POST_LIFO);
    QS_FILTER_OFF(QS_QF_EQUEUE_GET);
    QS_FILTER_OFF(QS_QF_EQUEUE_GET_LAST);
    QS_FILTER_OFF(QS_QF_MPOOL_INIT);
    QS_FILTER_OFF(QS_QF_MPOOL_GET);
    QS_FILTER_OFF(QS_QF_MPOOL_PUT);
    QS_FILTER_OFF(QS_QF_PUBLISH);
    QS_FILTER_OFF(QS_QF_NEW);
    QS_FILTER_OFF(QS_QF_GC_ATTEMPT);
    QS_FILTER_OFF(QS_QF_GC);
//    QS_FILTER_OFF(QS_QF_TICK);
//    QS_FILTER_OFF(QS_QF_TIMEEVT_ARM);
//    QS_FILTER_OFF(QS_QF_TIMEEVT_AUTO_DISARM);
//    QS_FILTER_OFF(QS_QF_TIMEEVT_DISARM_ATTEMPT);
//    QS_FILTER_OFF(QS_QF_TIMEEVT_DISARM);
//    QS_FILTER_OFF(QS_QF_TIMEEVT_REARM);
//    QS_FILTER_OFF(QS_QF_TIMEEVT_POST);
//    QS_FILTER_OFF(QS_QF_CRIT_ENTRY);
//    QS_FILTER_OFF(QS_QF_CRIT_EXIT);
    QS_FILTER_OFF(QS_QF_ISR_ENTRY);
    QS_FILTER_OFF(QS_QF_ISR_EXIT);

    QS_RESET();

    {
        pthread_attr_t attr;
        struct sched_param param;

        Q_ALLEGE(pthread_attr_init(&attr) == 0);
        Q_ALLEGE(pthread_attr_setschedpolicy(&attr, SCHED_FIFO) == 0);
        param.sched_priority = sched_get_priority_min(SCHED_FIFO);

        Q_ALLEGE(pthread_attr_setschedparam(&attr, &param) == 0);
        Q_ALLEGE(pthread_attr_setdetachstate(&attr,
                                             PTHREAD_CREATE_JOINABLE) == 0);

        Q_ALLEGE(pthread_create(&idle_thread, &attr, &idleThread, 0) == 0);
        Q_ALLEGE(pthread_attr_destroy(&attr) == 0);
    }

    return (uint8_t)1;
}

/*..........................................................................*/
void QS_onCleanup(void) {
    int status;
    l_idle_running = (uint8_t)0;

    /* wait until idle thread finishes */
    Q_ALLEGE(pthread_join(idle_thread, (void*) &status) == 0);
    QSPY_stop();
}

/*..........................................................................*/
void QSPY_onPrintLn(void) {
    LOGI("%s\n", QSPY_line);
}

#endif                                                             /* Q_SPY */
/*--------------------------------------------------------------------------*/

/*****************************************************************************
* NOTE01:
* The android lifecycle model of android_main does NOT zero the uninitialized
* .bss variables after first start. Since QP relies on the clearing
* of the static uninitialized variables, the critical QP objects are cleared
* explicitly in this BSP.
*/



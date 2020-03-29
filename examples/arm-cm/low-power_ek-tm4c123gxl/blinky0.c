/*.$file${.::blinky0.c} vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv*/
/*
* Model: low-power.qm
* File:  ${.::blinky0.c}
*
* This code has been generated by QM 4.6.0 <www.state-machine.com/qm/>.
* DO NOT EDIT THIS FILE MANUALLY. All your changes will be lost.
*
* This program is open source software: you can redistribute it and/or
* modify it under the terms of the GNU General Public License as published
* by the Free Software Foundation.
*
* This program is distributed in the hope that it will be useful, but
* WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
* or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
* for more details.
*/
/*.$endhead${.::blinky0.c} ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
#include "qpc.h"
#include "low_power.h"
#include "bsp.h"

//Q_DEFINE_THIS_FILE

/*.$declare${AOs::Blinky0} vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv*/
/*.${AOs::Blinky0} .........................................................*/
typedef struct Blinky0 {
/* protected: */
    QActive super;

/* private: */
    QTimeEvt timeEvt0;

/* public: */

/* private: */
    uint8_t count;
} Blinky0;
extern Blinky0 Blinky0_instance;

/* protected: */
static QState Blinky0_initial(Blinky0 * const me, QEvt const * const e);
static QState Blinky0_active(Blinky0 * const me, QEvt const * const e);
static QState Blinky0_on(Blinky0 * const me, QEvt const * const e);
static QState Blinky0_off(Blinky0 * const me, QEvt const * const e);
static QState Blinky0_inactive(Blinky0 * const me, QEvt const * const e);
/*.$enddecl${AOs::Blinky0} ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/

/*.$skip${QP_VERSION} vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv*/
/*. Check for the minimum required QP version */
#if (QP_VERSION < 650U) || (QP_VERSION != ((QP_RELEASE^4294967295U) % 0x3E8U))
#error qpc version 6.5.0 or higher required
#endif
/*.$endskip${QP_VERSION} ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
/*.$define${AOs::AO_Blinky0} vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv*/
/*.${AOs::AO_Blinky0} ......................................................*/
QActive * const AO_Blinky0 = &Blinky0_instance.super;
/*.$enddef${AOs::AO_Blinky0} ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/

/*.$define${AOs::Blinky0} vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv*/
/*.${AOs::Blinky0} .........................................................*/
Blinky0 Blinky0_instance;
/*.${AOs::Blinky0::SM} .....................................................*/
static QState Blinky0_initial(Blinky0 * const me, QEvt const * const e) {
    /*.${AOs::Blinky0::SM::initial} */
    (void)e; /* unused parameter */
    QActive_subscribe(&me->super, BTN_PRESSED_SIG);

    QS_FUN_DICTIONARY(&Blinky0_active);
    QS_FUN_DICTIONARY(&Blinky0_on);
    QS_FUN_DICTIONARY(&Blinky0_off);
    QS_FUN_DICTIONARY(&Blinky0_inactive);

    return Q_TRAN(&Blinky0_active);
}
/*.${AOs::Blinky0::SM::active} .............................................*/
static QState Blinky0_active(Blinky0 * const me, QEvt const * const e) {
    QState status_;
    switch (e->sig) {
        /*.${AOs::Blinky0::SM::active} */
        case Q_ENTRY_SIG: {
            BSP_tickRate0_on();
            me->count = 4U;
            QTimeEvt_armX(&me->timeEvt0, 2U, 2U);
            status_ = Q_HANDLED();
            break;
        }
        /*.${AOs::Blinky0::SM::active} */
        case Q_EXIT_SIG: {
            QTimeEvt_disarm(&me->timeEvt0);
            status_ = Q_HANDLED();
            break;
        }
        /*.${AOs::Blinky0::SM::active::initial} */
        case Q_INIT_SIG: {
            status_ = Q_TRAN(&Blinky0_on);
            break;
        }
        default: {
            status_ = Q_SUPER(&QHsm_top);
            break;
        }
    }
    return status_;
}
/*.${AOs::Blinky0::SM::active::on} .........................................*/
static QState Blinky0_on(Blinky0 * const me, QEvt const * const e) {
    QState status_;
    switch (e->sig) {
        /*.${AOs::Blinky0::SM::active::on} */
        case Q_ENTRY_SIG: {
            BSP_led0_on();
            status_ = Q_HANDLED();
            break;
        }
        /*.${AOs::Blinky0::SM::active::on} */
        case Q_EXIT_SIG: {
            BSP_led0_off();
            status_ = Q_HANDLED();
            break;
        }
        /*.${AOs::Blinky0::SM::active::on::TIMEOUT0} */
        case TIMEOUT0_SIG: {
            --me->count;
            /*.${AOs::Blinky0::SM::active::on::TIMEOUT0::[me->count>0]} */
            if (me->count > 0) {
                status_ = Q_TRAN(&Blinky0_off);
            }
            /*.${AOs::Blinky0::SM::active::on::TIMEOUT0::[else]} */
            else {
                status_ = Q_TRAN(&Blinky0_inactive);
            }
            break;
        }
        default: {
            status_ = Q_SUPER(&Blinky0_active);
            break;
        }
    }
    return status_;
}
/*.${AOs::Blinky0::SM::active::off} ........................................*/
static QState Blinky0_off(Blinky0 * const me, QEvt const * const e) {
    QState status_;
    switch (e->sig) {
        /*.${AOs::Blinky0::SM::active::off::TIMEOUT0} */
        case TIMEOUT0_SIG: {
            status_ = Q_TRAN(&Blinky0_on);
            break;
        }
        default: {
            status_ = Q_SUPER(&Blinky0_active);
            break;
        }
    }
    return status_;
}
/*.${AOs::Blinky0::SM::inactive} ...........................................*/
static QState Blinky0_inactive(Blinky0 * const me, QEvt const * const e) {
    QState status_;
    switch (e->sig) {
        /*.${AOs::Blinky0::SM::inactive::BTN_PRESSED} */
        case BTN_PRESSED_SIG: {
            status_ = Q_TRAN(&Blinky0_active);
            break;
        }
        default: {
            status_ = Q_SUPER(&QHsm_top);
            break;
        }
    }
    return status_;
}
/*.$enddef${AOs::Blinky0} ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/

/*.$define${AOs::Blinky0_ctor} vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv*/
/*.${AOs::Blinky0_ctor} ....................................................*/
void Blinky0_ctor(void) {
    Blinky0 *me = &Blinky0_instance;
    QActive_ctor(&me->super, Q_STATE_CAST(&Blinky0_initial));
    QTimeEvt_ctorX(&me->timeEvt0, &me->super, TIMEOUT0_SIG, 0U);
}
/*.$enddef${AOs::Blinky0_ctor} ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/

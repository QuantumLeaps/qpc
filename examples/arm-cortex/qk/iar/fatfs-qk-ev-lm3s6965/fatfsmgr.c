/*****************************************************************************
* Product: FatFs-Manager Active Object
* Last Updated for Version: 4.5.00
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
#include "qp_port.h"                                             /* QP-port */
#include "fatfsmgr.h"
#include "bsp.h"                                   /* Board Support Package */

//#include "ff.h"                                                  /* FatFS */

Q_DEFINE_THIS_FILE

/* Active object class -----------------------------------------------------*/
typedef struct FatFsMgrTag {
    QActive super;                                   /* derive from QActive */

} FatFsMgr;

static QState FatFsMgr_initial(FatFsMgr *me, QEvt const *e);
static QState FatFsMgr_running(FatFsMgr *me, QEvt const *e);

/* Local objects -----------------------------------------------------------*/
static FatFsMgr l_FatFsMgr;           /* the single instance of FatFsMgr AO */

/* Global-scope objects ----------------------------------------------------*/
QActive * const AO_FatFsMgr= (QActive *)&l_FatFsMgr;    /* "opaque" pointer */

/*..........................................................................*/
void FatFsMgr_ctor(void) {
    FatFsMgr*me = &l_FatFsMgr;

    QActive_ctor(&me->super, (QStateHandler)&FatFsMgr_initial);
}
/*..........................................................................*/
QState FatFsMgr_initial(FatFsMgr*me, QEvt const *e) {

    QS_OBJ_DICTIONARY(&l_FatFsMgr);
    QS_FUN_DICTIONARY(&QHsm_top);
    QS_FUN_DICTIONARY(&LwIPMgr_initial);
    QS_FUN_DICTIONARY(&LwIPMgr_running);

    return Q_TRAN(&FatFsMgr_running);
}
/*..........................................................................*/
QState FatFsMgr_running(FatFsMgr *me, QEvt const *e) {
    switch (e->sig) {
    }
    return Q_SUPER(&QHsm_top);
}

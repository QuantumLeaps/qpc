/*****************************************************************************
* Product: QMsmTst Example, Workstation-console
* Last updated for version 6.4.0
* Last updated on  2019-02-08
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
*****************************************************************************/
#include "qpc.h"
#include "qmsmtst.h"

#include <stdlib.h>
#include <stdio.h>

Q_DEFINE_THIS_FILE

/* local objects -----------------------------------------------------------*/
static FILE *l_outFile = (FILE *)0;
static void dispatch(QSignal sig);

/*..........................................................................*/
int main(int argc, char *argv[]) {
#ifdef Q_SPY
    uint8_t qsBuf[128];
    QS_initBuf(qsBuf, sizeof(qsBuf));
#endif

    QF_init();
    QF_onStartup();

    QMsmTst_ctor(); /* instantiate the QMsmTst object */

    if (argc > 1) {   /* file name provided? */
        l_outFile = fopen(argv[1], "w");
    }

    if (l_outFile == (FILE *)0) { /* interactive version? */
        l_outFile = stdout; /* use the stdout as the output file */

        printf("QMsmTst example, built on %s at %s,\n"
               "QP: %s.\nPress ESC to quit...\n",
               __DATE__, __TIME__, QP_VERSION_STR);

        QHSM_INIT(the_msm, (QEvt *)0); /* the top-most initial tran. */

        for (;;) { /* event loop */
            QEvt e;
            int c;

            printf("\n");
            c = (uint8_t)QF_consoleWaitForKey();
            printf("%c: ", (c >= ' ') ? c : 'X');

            if ('a' <= c && c <= 'i') { /* in range? */
                e.sig = (QSignal)(c - 'a' + A_SIG);
            }
            else if ('A' <= c && c <= 'I') { /* in range? */
                e.sig = (QSignal)(c - 'A' + A_SIG);
            }
            else if (c == '\33') { /* the ESC key? */
                e.sig = TERMINATE_SIG; /* terminate the interactive test */
            }
            else {
                e.sig = IGNORE_SIG;
            }

            QHSM_DISPATCH(the_msm, &e); /* dispatch the event */
        }
    }
    else { /* batch version */
        printf("QMsmTst example, built on %s at %s, QP %s\n"
               "output saved to %s\n",
               __DATE__, __TIME__, QP_VERSION_STR,
               argv[1]);

        fprintf(l_outFile, "QMsmTst example, QP %s\n",
                QP_VERSION_STR);

        QHSM_INIT(the_msm, (QEvt *)0); /* the top-most initial tran. */

        /* testing of dynamic transitions... */
        dispatch(A_SIG);
        dispatch(B_SIG);
        dispatch(D_SIG);
        dispatch(E_SIG);
        dispatch(I_SIG);
        dispatch(F_SIG);
        dispatch(I_SIG);
        dispatch(I_SIG);
        dispatch(F_SIG);
        dispatch(A_SIG);
        dispatch(B_SIG);
        dispatch(D_SIG);
        dispatch(D_SIG);
        dispatch(E_SIG);
        dispatch(G_SIG);
        dispatch(H_SIG);
        dispatch(H_SIG);
        dispatch(C_SIG);
        dispatch(G_SIG);
        dispatch(C_SIG);
        dispatch(C_SIG);

        fclose(l_outFile);
    }

    QF_onCleanup();
    return 0;
}
/*..........................................................................*/
void Q_onAssert(char const * const module, int loc) {
    fprintf(stderr, "Assertion failed in %s:%d\n", module, loc);
    QF_onCleanup();
    exit(-1);
}
/*..........................................................................*/
void BSP_display(char const *msg) {
    fprintf(l_outFile, msg);
    fflush(l_outFile);
}
/*..........................................................................*/
void BSP_exit(void) {
    printf("Bye, Bye!\n");
    QF_onCleanup();
    exit(0);
}
/*..........................................................................*/
static void dispatch(QSignal sig) {
    QEvt e;
    Q_REQUIRE((A_SIG <= sig) && (sig <= I_SIG));
    e.sig = sig;
    fprintf(l_outFile, "\n%c:", 'A' + sig - A_SIG);
    QHSM_DISPATCH(the_msm, &e); /* dispatch the event */
}

/*--------------------------------------------------------------------------*/
void QF_onStartup(void) {
    QF_consoleSetup();
}
/*..........................................................................*/
void QF_onCleanup(void) {
    QF_consoleCleanup();
}
/*..........................................................................*/
void QF_onClockTick(void) {
}

/*--------------------------------------------------------------------------*/
#ifdef Q_SPY /* define QS callbacks */

/*! callback function to execute user commands */
void QS_onCommand(uint8_t cmdId,
                  uint32_t param1, uint32_t param2, uint32_t param3)
{
    switch (cmdId) {
       case 0U: {
           break;
       }
       default:
           break;
    }

    /* unused parameters */
    (void)param1;
    (void)param2;
    (void)param3;
}

#endif /* Q_SPY */
/*--------------------------------------------------------------------------*/


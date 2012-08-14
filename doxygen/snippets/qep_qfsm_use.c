#include "qep.h"                                  /* QEP/C public interface */
#include "qbomb.h"                           /* QBomb FSM derived from QFsm */

static QBomb l_qbomb;                           /* an instance of QBomb FSM */

int main() {
    QBombInitEvt ie;                  /* initialization event for QBomb FSM */

    QBomb_ctor(&l_qbomb);   /* QBomb FSM "constructor" invokes QFsm_ctor_() */

    /* set the initialization event ie */

    QFsm_init((QFsm *)&l_qbomb, &ie);         /* trigger initial transition */

    for (;;) {                                                /* event loop */
        QEvent e;
        . . .
        /* wait for the next event and assign it to the event object e */
        . . .
        QFsm_dispatch((QFsm *)&l_qbomb, &e);       /* dispatch e to l_qbomb */
    }
    return 0;
}

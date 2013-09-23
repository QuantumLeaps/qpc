#include "qep.h"       /* QEP/C public interface */
#include "qbomb.h"     /* QBomb derived from QFsm */

static QBomb l_qbomb;  /* an instance of QBomb FSM */

int main() {
    QBomb_ctor(&l_bomb);   /* QBomb "constructor" invokes QFsm_ctor() */

    QMSM_INIT(&l_qbomb.super, (QEvt *)0); /* trigger initial transition */

    for (;;) {              /* event loop */
        QEvt e;
        . . .
        /* wait for the next event and assign it to the event object e */
        . . .
        QMSM_DISPATCH(&l_qbomb.super, &e);  /* dispatch e */
    }
    return 0;
}

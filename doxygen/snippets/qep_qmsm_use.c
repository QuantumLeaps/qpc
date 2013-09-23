#include "qep.h"            /* QEP/C public interface */
#include "qcalc.h"          /* QCalc derived from QMsm */

static QCalc l_qcalc;       /* an instance of QCalc SM */

int main() {
    QCalc_ctor(&l_qcalc);   /* QCalc "constructor" invokes QMsm_ctor_() */

    QMSM_INIT(&l_qcalc.super, (QEvt *)0); /* trigger initial transition */

    for (;;) {              /* event loop */
        QEvt e;
        . . .
        /* wait for the next event and assign it to the event object e */
        . . .
        QMSM_DISPATCH(&l_qcalc.super, &e);  /* dispatch e */
    }
    return 0;
}

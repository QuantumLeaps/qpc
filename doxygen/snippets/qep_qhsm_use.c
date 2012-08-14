#include "qep.h"                                  /* QEP/C public interface */
#include "qcalc.h"                           /* QCalc HSM derived from QHsm */

static QCalc l_qcalc;                           /* an instance of QCalc HSM */

int main() {
    QCalc_ctor(&l_qcalc);   /* QCalc HSM "constructor" invokes QHsm_ctor_() */

    QHsm_init((QHsm *)&l_qcalc, (QEvent *)0); /* trigger initial transition */

    for (;;) {                                                /* event loop */
        QEvent e;
        . . .
        /* wait for the next event and assign it to the event object e */
        . . .
        QHsm_dispatch((QHsm *)&l_qcalc, (QEvent *)&e);        /* dispatch e */
    }
    return 0;
}

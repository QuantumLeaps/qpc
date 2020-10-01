#include "qpc.h"   /* QP/C public interface */
#include "calc.h"  /* Calc derived from QMsm */

Q_DEFINE_THIS_FILE

static Calc l_calc;   /* an instance of Calc SM */

int main() {
    Calc_ctor(&l_calc);   /* Calc "constructor" invokes QMsm_ctor() */

    QHSM_INIT(&l_calc.super, (void *)0, 0U); /* trigger initial transition */

    for (;;) { /* event loop */
        QEvt e;
        . . .
        /* wait for the next event and assign it to the event object e */
        . . .
        QHSM_DISPATCH(&l_calc.super, &e, 0U); /* dispatch e */
    }
    return 0; /* never reached, needed for some compilers */
}

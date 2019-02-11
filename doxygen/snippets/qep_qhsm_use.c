#include "qpc.h"   /* QP/C public interface */
#include "calc.h"  /* Calc derived from QHsm */

Q_DEFINE_THIS_FILE

static Calc l_calc;  /* an instance of Calc SM */

int main() {
    Calc_ctor(&l_calc);   /* Calc "constructor" invokes QHsm_ctor() */

    QMSM_INIT(&l_calc.super, (QEvt *)0); /* trigger initial transition */

    for (;;) {  /* event loop */
        QEvt e;
        . . .
        /* wait for the next event and assign it to the event object e */
        . . .
        QMSM_DISPATCH(&l_calc.super, &e);  /* dispatch e */
    }
    return 0;
}

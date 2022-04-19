#include "qpc.h"   /* QP/C public interface */
#include "calc.h"  /* Calc derived from QHsm */

Q_DEFINE_THIS_FILE

static Calc Calc_inst;  /* an instance of Calc SM */

int main() {
    Calc_ctor(&Calc_inst);  /* Calc "constructor" invokes QHsm_ctor() */

    QMSM_INIT(&Calc_inst.super, (QEvt *)0); /* trigger initial transition */

    for (;;) {  /* event loop */
        QEvt e;
        . . .
        /* wait for the next event and assign it to the event object e */
        . . .
        QMSM_DISPATCH(&Calc_inst.super, &e);  /* dispatch e */
    }
    return 0;
}

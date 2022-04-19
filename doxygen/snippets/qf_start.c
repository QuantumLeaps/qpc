#include "qpc.h"

Q_DEFINE_THIS_FILE

int main() {

    QF_init();  /* initialize the framework and the underlying RT kernel */
    BSP_init(); /* initialize the Board Support Package */

    . . .

    /* instantiate and start the active objects... */
    Blinky_ctor();
    static QEvt const *l_blinkyQSto[10]; /* Event queue storage for Blinky */
    QACTIVE_START(AO_Blinky,      /* AO pointer to start */
                  1U,             /* unique QP priority of the AO */
                  l_blinkyQSto,   /* storage for the AO's queue */
                  Q_DIM(l_blinkyQSto), /* lenght of the queue [entries] */
                  (void *)0,      /* stack storage (not used in QK) */
                  0U,             /* stack size [bytes] (not used in QK) */
                  (void *)0);     /* initialization parameter (or 0) */
    . . .
    return QF_run(); /* run the QF application */
}
 
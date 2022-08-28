#include "qpc.h"

Q_DEFINE_THIS_FILE

int main() {
    . . .
    QF_init();    /* initialize the framework */
    BSP_init();   /* initialize the Board Support Package */

    /* initialize publish-subscribe... */
    QF_psInit(subscrSto, Q_DIM(subscrSto));

    /* initialize event pools... */
    QF_poolInit(smlPoolSto, sizeof(smlPoolSto), sizeof(smlPoolSto[0]));

    /* start the active objects (basic threads)... */
    Table_ctor(); /* instantiate the Table AO */
    QACTIVE_START(AO_Table,    /* AO to start */
        Q_PRIO(n + 1U, 1U),    /* QF-priority/preemption-thre. */
        tableQueueSto,         /* event queue storage */
        Q_DIM(tableQueueSto),  /* queue length [events] */
        tableStackSto,         /* stack storage */
        sizeof(tableStackSto), /* stack size [bytes] */
        (void *)0);            /* initialization param */
    . . .

    /* start the extended-threads... */
    Test_ctor();  /* instantiate the Test extended thread */
    QXTHREAD_START(XT_Test,    /* Thread to start */
        Q_PRIO(10U, 2U),       /* QF-priority/preemption-thre. */
        testQueueSto,          /* message queue storage */
        Q_DIM(testQueueSto),   /* message length [events] */
        testStackSto,          /* stack storage */
        sizeof(testStackSto),  /* stack size [bytes] */
        (void *)0);            /* initialization param */

    return QF_run(); /* run the QF application */
}

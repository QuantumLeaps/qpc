#include "qpc.h"

Q_DEFINE_THIS_FILE

int main() {
    . . .
    /* stack for the QXK's idle thread */
    static uint64_t idleStackSto[32];

    Test_ctor();  /* instantiate the Test "naked" thread */

    QF_init();    /* initialize the framework */
    BSP_init();   /* initialize the Board Support Package */

    /* initialize QXK... */
    QXK_init(idleStackSto, sizeof(idleStackSto));

    /* initialize publish-subscribe... */
    QF_psInit(subscrSto, Q_DIM(subscrSto));

    /* initialize event pools... */
    QF_poolInit(smlPoolSto, sizeof(smlPoolSto), sizeof(smlPoolSto[0]));

    /* start the active objects (basic-threads)... */
    QACTIVE_START(AO_Table,              /* AO to start */
                  N_PHILO + 2U,          /* QP priority of the AO */
                  tableQueueSto,         /* event queue storage */
                  Q_DIM(tableQueueSto),  /* queue length [events] */
                  tableStackSto,         /* stack storage */
                  sizeof(tableStackSto), /* stack size [bytes] */
                  (void *)0);            /* initialization param */
    . . .

    /* start the extended-threads... */
    QXTHREAD_START(XT_Test,              /* Thread to start */
                  10U,                   /* QP priority of the thread */
                  testQueueSto,          /* message queue storage */
                  Q_DIM(testQueueSto),   /* message length [events] */
                  testStackSto,          /* stack storage */
                  sizeof(testStackSto),  /* stack size [bytes] */
                  (void *)0);            /* initialization param */

    return QF_run(); /* run the QF application */
}

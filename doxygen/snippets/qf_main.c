#include "qpc.h"

Q_DEFINE_THIS_FILE

int main(void) {

    QF_init();  /* initialize the framework and the underlying RT kernel */
    BSP_init(); /* initialize the BSP */

    /* init publish-subscribe... */
    static QSubscrList l_subscrSto[MAX_PUB_SIG];
    QF_psInit(l_subscrSto, Q_DIM(l_subscrSto));

    /* initialize event pools... */
    static QF_MPOOL_EL(TableEvt) l_smlPoolSto[2*N_PHILO]; /* small pool */
    QF_poolInit(l_smlPoolSto, sizeof(l_smlPoolSto), sizeof(l_smlPoolSto[0]));

    /* start the active objects... */
    Philo_ctor(); /* instantiate all Philosopher active objects */

    static QEvt const *l_philoQueueSto[N_PHILO][N_PHILO];
    for (uint8_t n = 0U; n < N_PHILO; ++n) {
        QACTIVE_START(AO_Philo[n],
        Q_PRIO(n + 1U, 1U), /* QF-priority/preemption-thre. */
        l_philoQueueSto[n], Q_DIM(l_philoQueueSto[n]),
        (void *)0, 0U,
        (void *)0);
    }

    Table_ctor(); /* instantiate the Table active object */

    static QEvt const *l_tableQueueSto[N_PHILO];
    QACTIVE_START(AO_Table,
        Q_PRIO(N_PHILO + 1U, 2U), /* QF-priority/preemption-thre. */
        l_tableQueueSto, Q_DIM(l_tableQueueSto),
        (void *)0, 0U,
        (void *)0);

    return QF_run(); /* run the QF application, QF_run() does not return  */
}

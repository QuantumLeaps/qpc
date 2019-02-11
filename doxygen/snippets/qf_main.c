#include "qpc.h"

Q_DEFINE_THIS_FILE

int main(void) {
    static QEvt const *l_tableQueueSto[N_PHILO];
    static QEvt const *l_philoQueueSto[N_PHILO][N_PHILO];
    static QSubscrList l_subscrSto[MAX_PUB_SIG];

    /* storage for event pools... */
    static QF_MPOOL_EL(TableEvt) l_smlPoolSto[2*N_PHILO]; /* small pool */

    uint8_t n;

    Philo_ctor(); /* instantiate all Philosopher active objects */
    Table_ctor(); /* instantiate the Table active object */

    QF_init();  /* initialize the framework and the underlying RT kernel */
    BSP_init(); /* initialize the BSP */

    /* send object dictionaries for event pools... */
    QS_OBJ_DICTIONARY(l_smlPoolSto);

   /* init publish-subscribe... */
    QF_psInit(l_subscrSto, Q_DIM(l_subscrSto));

    /* initialize event pools... */
    QF_poolInit(l_smlPoolSto, sizeof(l_smlPoolSto), sizeof(l_smlPoolSto[0]));

    /* start the active objects... */
    for (n = 0; n < N_PHILO; ++n) {
        QACTIVE_START(AO_Philo[n], (uint8_t)(n + 1),
                      l_philoQueueSto[n], Q_DIM(l_philoQueueSto[n]),
                      (void *)0, 0U, (QEvt *)0);
    }
    QACTIVE_START(AO_Table, (uint8_t)(N_PHILO + 1),
                  l_tableQueueSto, Q_DIM(l_tableQueueSto),
                  (void *)0, 0U, (QEvt *)0);

    return QF_run(); /* run the QF application, QF_run() does not return  */
}

/* allocate storage for active objects, event queues, event pools,
* subscriber lists, and stacks.
*/
static QEvent const *l_tableQueueSto[N_PHILO];
static QEvent const *l_philoQueueSto[N_PHILO][N_PHILO];
static QSubscrList   l_subscrSto[MAX_PUB_SIG];

static union SmallEvent {
    void *min_size;
    TableEvt te;
    /* other event types to go into this pool */
} l_smlPoolSto[2*N_PHILO];              /* storage for the small event pool */

/*..........................................................................*/
int main(int argc, char *argv[]) {
    uint8_t n;

    Philo_ctor();             /* instantiate all Philosopher active objects */
    Table_ctor();                    /* instantiate the Table active object */

    BSP_init(argc, argv);           /* initialize the Board Support Package */

    QF_init();     /* initialize the framework and the underlying RT kernel */

    QF_psInit(l_subscrSto, Q_DIM(l_subscrSto));   /* init publish-subscribe */

                                               /* initialize event pools... */
    QF_poolInit(l_smlPoolSto, sizeof(l_smlPoolSto), sizeof(l_smlPoolSto[0]));

    for (n = 0; n < N_PHILO; ++n) {          /* start the active objects... */
        QActive_start(AO_Philo[n], (uint8_t)(n + 1),
                      l_philoQueueSto[n], Q_DIM(l_philoQueueSto[n]),
                      (void *)0, 0, (QEvent *)0);
    }
    QActive_start(AO_Table, (uint8_t)(N_PHILO + 1),
                  l_tableQueueSto, Q_DIM(l_tableQueueSto),
                  (void *)0, 0, (QEvent *)0);
    QF_run();                                     /* run the QF application */

    return 0;
}

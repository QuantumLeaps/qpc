static Philosopher l_philo[N];              /* N Philosopher active objects */
static QEvent const *l_philQueueSto[N][N];/* storage for Philo event queues */
static int l_philoStk[N][256]; /* stacks for the Philosopher active objects */

main() {
    . . .
    for (n = 0; n < N; ++n) {
        TableEvt ie;        /* initialization event for the Philosopher HSM */
        ie.philNum = n;
        Philosopher_ctor(&l_philo[n]);
        QActive_start((QActive *)&l_philo[n],       /* Philosopher number n */
                      (uint8_t)(n*10 + 1),                      /* priority */
                      l_philoQueueSto[n], Q_DIM(l_philoQueueSto[n]),/*queue */
                      l_philoStk[n], sizeof(l_philoStk[n]),/*uC/OS-II stack */
                      (QEvent const *)&ie);         /* initialization event */
    }
    . . .
} 
static Philo l_philo[N];                  /* N Philo active objects */
static QEvt const *l_philQueueSto[N][N];  /* storage for Philo event queues */
static int l_philoStk[N][256];       /* stacks for the Philo active objects */

int main() {
    . . .
    for (n = 0; n < N; ++n) {
        TableEvt ie;               /* initialization event for the Philo SM */
        ie.philNum = n;
        Philo_ctor(&l_philo[n]);
        QActive_start((QActive *)&l_philo[n],              /* Philo pointer */
            (uint8_t)(n*10 + 1),                           /* priority */
            l_philoQueueSto[n], Q_DIM(l_philoQueueSto[n]), /* queue */
            l_philoStk[n], sizeof(l_philoStk[n]),          /* uC/OS-II stack */
            &ie.super);                              /* initialization event */
    }
    . . .
} 
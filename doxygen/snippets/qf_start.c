#include "qpc.h"

Q_DEFINE_THIS_FILE

int main() {
    static Philo l_philo[N]; /* N Philo active objects */
    static QEvt const *l_philQueueSto[N][N]; /* storage for event queues */
    static int l_philoStk[N][256];  /* stacks for the Philo active objects */

    . . .
    for (n = 0; n < N; ++n) {
        TableEvt ie;  /* initialization event for the Philo SM */
        ie.philNum = n;
        Philo_ctor(&l_philo[n]);
        QACTIVE_START((QActive *)&l_philo[n],  /* Philo pointer */
            (uint_fast8_t)(n*10 + 1),          /* priority */
            l_philoQueueSto[n], Q_DIM(l_philoQueueSto[n]), /* queue */
            l_philoStk[n], sizeof(l_philoStk[n]), /* per AO stack */
            &ie.super);                        /* initialization event */
    }
    . . .
} 
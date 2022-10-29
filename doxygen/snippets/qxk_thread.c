#include "qpc.h"

Q_DEFINE_THIS_FILE

QXThread blinky;  /* QXK extended-thread object */

void main_blinky(QXThread * const me) { /* thread function */
    while (1) {
        BSP_ledOn();
        QXThread_delay(100U); /* BLOCK */
        BSP_ledOff();
        QXThread_delay(200U); /* BLOCK */
    }
}

int main() {
    . . .
    /* initialize and start blinky thread */
    QXThread_ctor(&blinky, &main_blinky, 0);

    static uint64_t stack_blinky[40]; /* stack for the thread */
    QXTHREAD_START(&blinky,
                   5U, /* priority */
                   (void *)0, 0, /* message queue (not used) */
                   stack_blinky, sizeof(stack_blinky), /* stack */
                   (void *)0); /* extra parameter (not used) */
    . . .
    return QF_run(); /* run the application */
}

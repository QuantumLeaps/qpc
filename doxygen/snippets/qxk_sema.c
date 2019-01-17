QXSemaphore BTN_sema; /* semaphore to signal a button press */

int main() {
    . . .
    /* initialize the BTN_sema semaphore as binary, signaling semaphore */
    QXSemaphore_init(&BTN_sema, /* pointer to semaphore to initialize */
                     0U,  /* initial semaphore count (singaling semaphore) */
                     1U); /* maximum semaphore count (binary semaphore) */
    . . .
}

void main_threadXYZ(QXThread * const me) {
    while (1) {
        . . .
        QXSemaphore_wait(&BTN_sema,  /* pointer to semaphore to wait on */
                         QXTHREAD_NO_TIMEOUT); /* timeout for waiting */
        . . .
    }
}

void GPIO_Handler(void) {
    . . .
    QXSemaphore_signal(&BTN_sema); /* pointer to semaphore to signal */
    . . .
}

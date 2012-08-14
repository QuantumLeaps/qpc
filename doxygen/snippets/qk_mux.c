    QMutex mux;
    . . .
    mux = QK_mutexLock(PRIO_CEILING);

    /* access the shared resource */

    QK_mutexUnlock(mux);

    . . . 
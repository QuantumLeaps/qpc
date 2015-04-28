#ifdef Q_SPY

uint8_t QS_onStartup(void const *arg) {
    static uint8_t qsBuf[4*1024];   /* buffer for Quantum Spy */

    QS_initBuf(qsBuf, sizeof(qsBuf)); /* configure the Q-Spy buffer */

    /* configure UART 0/1 for QSPY output ... */
    if (*(char const *)arg == '0') { /* use UART 0 */
        /* configure UART 0 for QSPY output ... */
        . . .
        return (uint8_t)1; /* UART 0 successfully opened */
    }
    else { /* use UART 1 */
        /* configure UART 1 for QSPY output ... */
        . . .
        return (uint8_t)1; /* UART 1 successfully opened */
    }
}

. . .

#endif /* Q_SPY */

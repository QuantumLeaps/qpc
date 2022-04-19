#ifdef Q_SPY

QSTimeCtr QS_onGetTime(void) { /* <--- callback */
    return (QSTimeCtr)clock();
}

. . .

#endif /* Q_SPY */

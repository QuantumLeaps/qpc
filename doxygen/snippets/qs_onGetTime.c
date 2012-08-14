#ifdef Q_SPY

QSTimeCtr QS_onGetTime(void) {
    return (QSTimeCtr)clock();
}

. . .

#endif /* Q_SPY */

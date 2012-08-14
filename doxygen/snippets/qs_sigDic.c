QState Table_initial(Table *me, QEvent const *e) {
    uint8_t n;
    (void)e;                               /* suppress the compiler warning */

    QS_SIG_DICTIONARY(HUNGRY_SIG, me);/* output signal dictionary QS record */
    QS_SIG_DICTIONARY(DONE_SIG, me);  /* output signal dictionary QS record */
    QS_SIG_DICTIONARY(EAT_SIG, 0);    /* output signal dictionary QS record */

    QS_FUN_DICTIONARY(Table_serving);

    QActive_subscribe((QActive *)me, HUNGRY_SIG);
    QActive_subscribe((QActive *)me, DONE_SIG);
    QActive_subscribe((QActive *)me, TERMINATE_SIG);

    for (n = 0; n < N; ++n) {
        me->fork[n] = FREE;
        me->isHungry[n] = 0;
    }
    return Q_TRAN(&Table_serving);
}

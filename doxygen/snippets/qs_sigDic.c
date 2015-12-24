QState Table_initial(Table * const me, QEvt const *) {
    uint8_t n;

    QS_SIG_DICTIONARY(HUNGRY_SIG, me); /* output signal dictionary QS record */
    QS_SIG_DICTIONARY(DONE_SIG, me);   /* output signal dictionary QS record */
    QS_SIG_DICTIONARY(EAT_SIG, 0);     /* output signal dictionary QS record */

    QS_FUN_DICTIONARY(&Table_serving);

    QActive_subscribe(&me->super, HUNGRY_SIG);
    QActive_subscribe(&me->super, DONE_SIG);
    QActive_subscribe(&me->super, TERMINATE_SIG);

    for (n = 0U; n < N; ++n) {
        me->fork[n] = FREE;
        me->isHungry[n] = 0U;
    }
    return Q_TRAN(&Table_serving);
}

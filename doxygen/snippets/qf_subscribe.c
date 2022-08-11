QState Table_initial(Table * const me, QEvt const * const e) {
    (void)e; /* unused parameter */

    /* subscribe to event signals... */
    QActive_subscribe(&me->super, (enum_t)HUNGRY_SIG);
    QActive_subscribe(&me->super, (enum_t)DONE_SIG);
    QActive_subscribe(&me->super, (enum_t)TERMINATE_SIG);

    for (uint8_t n = 0U; n < N; ++n) {
        me->fork[n] = FREE;
        me->isHungry[n] = false;
    }
    return Q_TRAN(&Table_serving);
}

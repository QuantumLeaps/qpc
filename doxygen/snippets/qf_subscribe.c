QState Table_initial(Table * const me, QEvt const * const e) {
    uint8_t n;
    (void)e;                               /* suppress the compiler warning */

    QActive_subscribe((QActive *)me, HUNGRY_SIG);   /* subscribe to HUNGRY */
    QActive_subscribe((QActive *)me, DONE_SIG);              /*... to DONE */
    QActive_subscribe((QActive *)me, TERMINATE_SIG); /*...and to TERMINATE */
    for (n = 0; n < N; ++n) {
        me->fork[n] = FREE;
        me->isHungry[n] = 0;
    }
    return Q_TRAN(&Table_serving);
}

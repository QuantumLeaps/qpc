QState Table_initial(Table * const me, QEvt const *) {

    /* signals for all state machines~~~ */
    QS_SIG_DICTIONARY(EAT_SIG,    (void *)0);
    . . .


    /* signals for this specific state machine "me"~~~ */
    QS_SIG_DICTIONARY(HUNGRY_SIG, me);
    QS_SIG_DICTIONARY(DONE_SIG,   me);
    . . .

    return Q_TRAN(&Table_serving);
}

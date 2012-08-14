QState Philosopher_eating(Philosopher *me, QEvent const *e) {
    TableEvt *pe;
    switch (e->sig) {
        case Q_ENTRY_SIG: {
            QTimeEvt_postIn(&me->timeEvt, (QActive *)me, EAT_TIME);
            return Q_HANDLED();
        }
        case TIMEOUT_SIG: {
            return Q_TRAN(&Philosopher_thinking);
        }
        case Q_EXIT_SIG: {
            pe = Q_NEW(TableEvt, DONE_SIG);
            pe->philNum = me->num;
            QF_publish((QEvent *)pe);
            return Q_HANDLED();
        }
    }
    return Q_SUPER(&QHsm_top);
}

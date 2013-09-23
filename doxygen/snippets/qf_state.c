QState Philo_eating(Philo * const me, QEvt const * const e) {
    TableEvt *pe;
    switch (e->sig) {
        case Q_ENTRY_SIG: {
            QTimeEvt_armX(&me->timeEvt, EAT_TIME, 0U); /* one shot */
            return Q_HANDLED();
        }
        case Q_EXIT_SIG: {
            QTimeEvt_disarm(&me->timeEvt);
            pe = Q_NEW(TableEvt, DONE_SIG);
            pe->philNum = me->num;
            QF_PUBLISH((QEvt *)pe, me);
            return Q_HANDLED();
        }
        case TIMEOUT_SIG: {
            return Q_TRAN(&Philosopher_thinking);
        }
    }
    return Q_SUPER(&QHsm_top);
}

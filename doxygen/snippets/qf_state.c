QState Philo_eating(Philo * const me, QEvt const * const e) {
    QState status;
    switch (e->sig) {
        case Q_ENTRY_SIG: {
            QTimeEvt_armX(&me->timeEvt, EAT_TIME, 0U); /* one shot */
            status = Q_HANDLED();
            break;
        }
        case Q_EXIT_SIG: {
            TableEvt *pe;
            QTimeEvt_disarm(&me->timeEvt);
            pe = Q_NEW(TableEvt, DONE_SIG);
            pe->philNum = me->num;
            QF_PUBLISH((QEvt *)pe, me);
            status = Q_HANDLED();
            break;
        }
        case TIMEOUT_SIG: {
            status = Q_TRAN(&Philosopher_thinking);
            break;
        }
        default: {
            status = Q_SUPER(&QHsm_top);
            break;
        }
    }

    return status;
}

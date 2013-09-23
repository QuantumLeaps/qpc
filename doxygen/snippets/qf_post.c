extern QActive *AO_Table;

QState Philoso_hungry(Philo * const me, QEvt const * const e) {
    switch (e->sig) {
        case Q_ENTRY_SIG: {
            TableEvt *pe = Q_NEW(TableEvt, HUNGRY_SIG); /* dynamic alloc */
            pe->philNum = me->num;
            QACTIVE_POST(AO_Table, &pe->super, me); /* direct posting */
            return Q_HANDLED();
        }
        . . .
    }
    return Q_SUPER(&QHsm_top);
}

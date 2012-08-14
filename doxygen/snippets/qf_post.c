extern QActive *AO_Table;

QState Philosopher_hungry(Philosopher *me, QEvent const *e) {
    TableEvt *pe;
    switch (e->sig) {
        case Q_ENTRY_SIG: {
            pe = Q_NEW(TableEvt, HUNGRY_SIG); /* dynamically allocate event */
            pe->philNum = me->num;
            QActive_postFIFO(AO_Table, (QEvent *)pe);     /* direct posting */
            return Q_HANDLED();
        }
        . . .
    }
    return Q_SUPER(&QHsm_top);
}

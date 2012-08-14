/* initial pseudostate of the QBomb FSM ....................................*/
QState QBomb_initial(QBomb *me, QEvent const *e) {
    Q_REQUIRE(e != (QEvent *)0);           /* initialization event expected */
    QBomb_updateState(me, "top-INIT");
    me->timeout = INIT_TIMEOUT;
    me->defuse  = ((QBombInitEvt const *)e)->defuse;

    return Q_TRAN(&QBomb_setting);                    /* initial transition */
}

/* state handler function for the QCalc HSM ................................*/
QState QCalc_on(QCalc *me, QEvent const *e) {
    switch (e->sig) {
        . . .
        case Q_INIT_SIG: {
            QCalc_updateState(me, "on-INIT");
            return Q_TRAN(&QCalc_ready);              /* initial transition */
        }
        . . .
    }
    return Q_SUPER(&QHsm_top);
}


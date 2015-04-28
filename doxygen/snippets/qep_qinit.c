/* initial pseudostate of the Bomb FSM ....................................*/
QState Bomb_initial(Bomb * const me, QEvt const * const e) {
    Q_REQUIRE(e != (QEvt *)0); /* initialization event expected */
    Bomb_updateState(me, "top-INIT");
    me->timeout = INIT_TIMEOUT;
    me->defuse  = Q_EVT_CAST(BombInitEvt)->defuse;

    return Q_TRAN(&Bomb_setting); /* initial transition */
}

/* state handler function for the Calc HSM ................................*/
QState Calc_on(Calc * const me, QEvt const * const e) {
    QState status;
    switch (e->sig) {
        . . .
        case Q_INIT_SIG: {
            QCalc_updateState(me, "on-INIT");
            status = Q_TRAN(&Calc_ready);  /* initial transition */
            break;
        }
        . . .
        default: {
            status = Q_SUPER(&QHsm_top);
            break;
        }
    }
    return status;
}


/* state handler function for the QBomb FSM ................................*/
QState QBomb_setting(QBomb *me, QEvent const *e) {
    switch (e->sig) {
        . . .
        case ARM_SIG: {
            return Q_TRAN(&QBomb_timing);         /* transition to "timing" */
        }
    }
    return Q_IGNORED();
}

/* state handler function for the QCalc HSM ................................*/
QState QCalc_begin(QCalc *me, QEvent const *e) {
    switch (e->sig) {
        . . .
        case OPER_SIG: {
            if (((QCalcEvt *)e)->keyId == KEY_MINUS) {   /* guard condition */
                return Q_TRAN(&QCalc_negated1); /* transition to "negated1" */
            }
            break;
        }
    }
    return Q_SUPER(&QCalc_ready);
}


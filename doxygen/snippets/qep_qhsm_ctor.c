void QCalc_ctor(QCalc *me) {
    QHsm_ctor(&me->super, Q_STATE_CAST(&QCalc_initial)); /* superclass' ctor */

    me->operand1 = 0.0;
    me->operand2 = 0.0;
    me->len      = 0U;
    me->opKey    = 0U;
}

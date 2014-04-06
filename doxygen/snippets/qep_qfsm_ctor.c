void QBomb_ctor(QBomb * const me) {
    QFsm_ctor(&me->super, Q_STATE_CAST(&QBomb_initial)); /* superclass ctor */

    me->timeout = 0U;
    me->defuse  = 0U;
    me->code    = 0U;
}

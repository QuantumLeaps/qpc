void Bomb_ctor(Bomb * const me) {
    /* superclass' ctor */
    QFsm_ctor(&me->super, Q_STATE_CAST(&Bomb_initial));

    me->timeout = 0U;
    me->defuse  = 0U;
    me->code    = 0U;
}

static QState Game_show_logo(Tunnel * const me, QEvt const * const e) {
    QState status_;
    switch (e->sig) {
        /*.${AOs::Tunnel::SM::active::show_logo} */
        case Q_ENTRY_SIG: {
            /* arm periodic time event */
            QTimeEvt_armX(&me->blinkTimeEvt,
                          BSP_TICKS_PER_SEC/2U,  /* one-time delay */
                          BSP_TICKS_PER_SEC/2U); /* interval */
            /* arm a one-shot time event */
            QTimeEvt_armX(&me->screenTimeEvt,
                          BSP_TICKS_PER_SEC*5U,  /* one-time delay */
                          0U);      /* interval (0 == no interval) */
            . . .
            status_ = Q_HANDLED();
            break;
        }
        . . .

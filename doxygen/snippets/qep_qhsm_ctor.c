void QCalc_ctor(QCalc *me) {
    QHsm_ctor(&me->super, (QStateHandler)&QCalc_initial);
}

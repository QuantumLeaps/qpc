void QBomb_ctor(QBomb *me) {
    QFsm_ctor(&me->super, (QStateHandler)&QBomb_initial);/* superclass ctor */
}

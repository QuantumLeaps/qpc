void Philosopher_ctor(Philosopher *me) {
    QActive_ctor(&me->super, (QStateHandler)&Philosopher_initial);
    QTimeEvt_ctor(&me->timeEvt, TIMEOUT_SIG);
}

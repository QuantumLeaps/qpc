typedef struct {
    QActive super;    /* inherits QActive */

    QTimeEvt timeEvt; /* to timeout the blinking */
} Blinky;
. . .

void Blinky_ctor(Blinky * const me) {
    /* constructor of the superclass <--- */
    QActive_ctor(&me->super, Q_STATE_CAST(&Blinky_initial));

    /* constructor(s) of the members */
    QTimeEvt_ctorX(&me->timeEvt, &me->super, TIMEOUT_SIG, 0U);
}

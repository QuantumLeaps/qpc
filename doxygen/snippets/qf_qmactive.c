typedef struct {
    QMActive super;   /* inherits QMActive */

    QTimeEvt timeEvt; /* to timeout the blinking */
} Blinky;
. . .

void Blinky_ctor(Blinky * const me) {
    /* constructor of the superclass <--- */
    QMActive_ctor(&me->super, Q_STATE_CAST(&Blinky_initial));

    /* constructor(s) of the members */
    QTimeEvt_ctorX(&me->timeEvt, &me->super, TIMEOUT_SIG, 0U);
}

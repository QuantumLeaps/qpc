typedef struct  {
    QHsm super;  /* inherit QHsm */

    QStateHandler hist_doorClosed; /* history of doorClosed */
} ToastOven;

/*..........................................................*/
static QState ToastOven_doorClosed(ToastOven * const me,
                                   QEvt const * const e)
{
    QState status;
    switch (e->sig) {
        ~ ~ ~
        case Q_EXIT_SIG: {
            me->hist_doorClosed = QHsm_state(&me->super);
            status = Q_HANDLED();
            break;
        }
    }
    return status;
}
/*..........................................................*/
static QState ToastOven_doorOpen(ToastOven * const me,
                                 QEvt const * const e)
{
    QState status;
    switch (e->sig) {
        ~ ~ ~
        case CLOSE_SIG: {
            status = Q_TRAN_HIST(hist_doorClosed); /*<== */
            break;
        }
        default: {
            status = Q_SUPER(&QHsm_top);
            break;
        }
    }
    return status;
}

main () {
    . . .

    if (!QS_INIT("1")) {                   /* Initialize QSpy to use UART 1 */
        return -1;                             /* Unable to initialize QSpy */
    }
    QS_FILTER_IN(QS_ALL_RECORDS);     /* start with enabling all QS records */

    QS_FILTER_OUT(QS_QK_INT_LOCK);      /* disable QS output of this record */
    QS_FILTER_OUT(QS_QK_INT_UNLOCK);    /* disable QS output of this record */
    QS_FILTER_OUT(QS_QK_ISR_ENTRY);     /* disable QS output of this record */
    QS_FILTER_OUT(QS_QK_ISR_EXIT);      /* disable QS output of this record */

    QS_FILTER_SM_OBJ(&philo[3]);    /* trace only this state machine object */

    QS_FILTER_AO_OBJ(&philo[3]);           /* trace only this active object */

    QS_FILTER_MP_OBJ(regSizePoolSto);         /* trace only this event pool */

    QS_FILTER_EQ_OBJ(&rawQueue);             /* trace only this event queue */

    QS_FILTER_TE_OBJ(&philo[3].timeEvt);      /* trace only this time event */

    . . .
}
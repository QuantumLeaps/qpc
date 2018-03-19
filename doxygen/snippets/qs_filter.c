int main(int arc, char *argv[]) {
    . . .

    if (!QS_INIT(argv)) {         /* Initialize QS target component */
        return -1;                /* unable to initialize QSpy */
    }

    /* apply the QS global filters... */
    QS_FILTER_ON(QS_SM_RECORDS);  /* all state machine records */
    QS_FILTER_ON(QS_AO_RECORDS);  /* all active object records */
    QS_FILTER_ON(QS_UA_RECORDS);  /* all user records */

    /* apply the QS local filters... */
    QS_FILTER_SM_OBJ(&philo[3]);  /* trace only this state machine object */
    QS_FILTER_AO_OBJ(&philo[3]);  /* trace only this active object */
    QS_FILTER_MP_OBJ(regSizePoolSto); /* trace only this event pool */
    QS_FILTER_EQ_OBJ(&rawQueue);  /* trace only this event queue */
    QS_FILTER_TE_OBJ(&philo[3].m_timeEvt); /* trace only this time event */
    . . .
}
extern QActive *AO_Table;

    . . .
    /* typically inside a state machine action */
    TableEvt *pe;
    Q_NEW_X(pe, TableEvt, 5U, HUNGRY_SIG); /* dynamic alloc, margin==5 */
    if (pe != (TableEvt *)0) {
        pe->philNum = me->num;
        QACTIVE_POST_X(AO_Table, &pe->super, 3U, me); /* margin==3 */
    }
    . . .

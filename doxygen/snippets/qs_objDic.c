static Table l_table;

/* top-most initial transition */
static QState Table_initial(Table * const me, QEvt const * const e) {
    . . .
    QS_OBJ_DICTIONARY(&l_table); /* provide object dictionary for table */
    . . .
}

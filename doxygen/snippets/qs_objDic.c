Table Table_inst; /* the instance of the Table Active Object */

/* top-most initial transition */
static QState Table_initial(Table * const me, QEvt const * const e) {
    . . .
    /* provide object dictionary for Table */
    QS_OBJ_DICTIONARY(&Table_inst); /* <--- */
    . . .
}

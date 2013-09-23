void tableStart(uint8_t prio,
                QEvt const *qSto[], uint32_t qLen,
                void *stkSto, uint32_t stkSize)
{
    static Table table;

    QS_OBJ_DICTIONARY(table);        /* provide object dictionary for table */

    Table_ctor(&table);
    QActive_start((QActive *)&table, prio,
                  qSto, qLen, stkSto, stkSize,
                  (QEvt *)0);
}

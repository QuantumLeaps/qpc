static Table l_table;

void tableStart(uint8_t prio,
                QEvt const *qSto[], uint32_t qLen,
                void *stkSto, uint32_t stkSize)
{

    QS_OBJ_DICTIONARY(&l_table); /* provide object dictionary for table */

    table.start(prio,
                qSto, qLen, stkSto, stkSize,
                (QEvt *)0);
}

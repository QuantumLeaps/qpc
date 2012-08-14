enum UserSpyRecords {
    . . .
    QS_DPP_DISPLAY = QS_USER               /* define a user QS record types */
    . . .
};

void displyPhilStat(uint8_t n, char const *stat) {
    . . .

    QS_BEGIN(QS_DPP_DISPLAY);                    /* output a user QS record */
        QS_U8(1, n);
        QS_STR(stat);
    QS_END();
}

enum UserSpyRecords {
    . . .
    PHILO_STAT = QS_USER, /* define a user QS record types */
    . . .
};

void displyPhilStat(uint8_t n, char const *stat) {
    . . .

    /* application-specific record */
    QS_BEGIN_ID(PHILO_STAT, AO_Philo[n]->prio);
        QS_U8(1, n);  /* Philosopher number */
        QS_STR(stat); /* Philosopher status */
    QS_END();
}

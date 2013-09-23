typedef struct PhiloTag {
    QActive super;       /* inherits QActive */

    QTimeEvt timeEvt;    /* to timeout thining or eating */
    uint8_t num;         /* this philosopher's number */
} Philo;

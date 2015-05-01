typedef struct {
    QMActive super;   /* inherits QMActive */

    QTimeEvt timeEvt; /* to timeout thining or eating */
    uint8_t num;      /* this philosopher's number */
} Philo;

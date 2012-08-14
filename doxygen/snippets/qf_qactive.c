typedef struct PhilosopherTag {
    QActive super;                                  /* derives from QActive */
    uint8_t num;                              /* number of this philosopher */
    QTimeEvt timeEvt;                       /* to timeout thining or eating */
} Philosopher;

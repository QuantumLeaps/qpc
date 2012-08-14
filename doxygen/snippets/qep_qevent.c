typedef struct QCalcEvtTag {
    QEvent super;                                    /* derives from QEvent */
    uint8_t keyId;                               /* ID of the key depressed */
} QCalcEvt;

typedef struct {
    QEvt super; /* inherits QEvt */
    uint8_t keyId; /* ID of the key depressed */
} CalcEvt;

typedef struct {
    QMsm super; /* inherits QMsm */

    double operand1;
    double operand2;
    char display[DISP_WIDTH + 1];
    uint8_t len;
    uint8_t opKey;
} Calc;

typedef struct QCalcTag {
    QHsm super;       /* inhertits QHsm */

    double operand1;
    double operand2;
    char display[DISP_WIDTH + 1];
    uint8_t len;
    uint8_t opKey;
} QCalc;

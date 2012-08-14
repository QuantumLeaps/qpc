typedef struct QCalcTag {
    QHsm super;                                        /* derives from QHsm */
    double operand1;
    double operand2;
    char display[DISP_WIDTH + 1];
    uint8_t len;
    uint8_t opKey;
} QCalc;

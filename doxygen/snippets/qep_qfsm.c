typedef struct {
    QFsm super;      /* inherits QFsm */
    uint8_t timeout; /* number of seconds till explosion */
    uint8_t defuse;  /* the secret defuse code */
    uint8_t code;    /* the current defuse code entry */
} Bomb;

#ifndef MY_APP_H
#define MY_APP_H

enum MySignals {
    MY_EVT0_SIG = Q_USER_SIG,
    MY_EVT1_SIG,
    MAX_PUB_SIG,    /* the last published signal */

    MY_EVT2_SIG,
    MY_EVT3_SIG,
    MAX_SIG         /* the last signal */
};

typedef struct {
    QEvt super; /* inherit QEvt */

    uint32_t u32;
} MyEvt1;

typedef struct {
    QEvt super; /* inherit QEvt */

    uint32_t u32;
    uint16_t u16;
} MyEvt2;

typedef struct {
    QEvt super; /* inherit QEvt */

    uint32_t u32;
    uint16_t u16;
    uint8_t  u8;
} MyEvt3;


void MyAO_ctor(void);
extern QActive * const AO_MyAO;

#endif /* MY_APP_H */

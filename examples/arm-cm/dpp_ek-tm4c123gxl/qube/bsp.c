#include "qpc.h"
#include "bsp.h"
#include "dpp.h"

//Q_DEFINE_THIS_FILE

enum AppRecords { /* application-specific trace records */
    PHILO_STAT = QS_USER,
    PAUSED_STAT,
};

static uint32_t l_rnd;  /* random seed */

/*..........................................................................*/
void BSP_init(void) {
    /* QS dictionaries... */
    QS_USR_DICTIONARY(PHILO_STAT);
    QS_USR_DICTIONARY(PAUSED_STAT);

    /* QS filters... */
    QS_GLB_FILTER(QS_ALL_RECORDS);
}
/*..........................................................................*/
void BSP_displayPhilStat(uint8_t n, char const *stat) {
    QS_BEGIN_ID(PHILO_STAT, AO_Philo[n]->prio) /* app-specific record */
        QS_U8(1, n);  /* Philosopher number */
        QS_STR(stat); /* Philosopher status */
    QS_END()
}
/*..........................................................................*/
void BSP_displayPaused(uint8_t paused) {
    QS_BEGIN_ID(PAUSED_STAT, 0U) /* app-specific record */
        QS_U8(1, paused);  /* Paused status */
    QS_END()
}
/*..........................................................................*/
uint32_t BSP_random(void) { /* a very cheap pseudo-random-number generator */
    /* Some flating point code is to exercise the VFP... */
    float volatile x = 3.1415926F;
    x = x + 2.7182818F;

    /* lock scheduler around shared seed */
    /* "Super-Duper" Linear Congruential Generator (LCG)
    * LCG(2^32, 3*7*11*13*23, 0, seed)
    */
    l_rnd = l_rnd * (3U*7U*11U*13U*23U);

    return l_rnd >> 8;
}
/*..........................................................................*/
void BSP_randomSeed(uint32_t seed) {
    l_rnd = seed;
}
/*..........................................................................*/
void BSP_terminate(int16_t result) {
    (void)result;
}

/*..........................................................................*/
/*! callback function to generate an event for this application */
QEvt const* Qube_onGenEvt(QSignal sig, char const* params) {
    Q_UNUSED_PAR(params); /* QHsmTst does not use params */

    static QEvt evt = { 0U, 0U, 0U };
    evt.sig = sig;
    return &evt;
}

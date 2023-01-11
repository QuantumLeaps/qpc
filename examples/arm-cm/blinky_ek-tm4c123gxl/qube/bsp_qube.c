#include "qpc.h"
#include "bsp.h"
#include "blinky.h"

Q_DEFINE_THIS_FILE

/*..........................................................................*/
void BSP_init(void) {
    /* QS dictionaries... */
    QS_SIG_DICTIONARY(TIMEOUT_SIG, (void*)0);
    /* QS filters... */
    QS_GLB_FILTER(QS_ALL_RECORDS);
}
/*..........................................................................*/
void BSP_exit(void) {
    QF_stop();
}
/*..........................................................................*/
void BSP_ledOff(void) {
    QS_BEGIN_ID(QS_USER, 0U)
        QS_STR("OFF");
    QS_END()
}
/*..........................................................................*/
void BSP_ledOn(void) {
    QS_BEGIN_ID(QS_USER, 0U)
        QS_STR("ON");
    QS_END()
}
/*..........................................................................*/
/*! callback function to generate an event for this application */
QEvt const* Qube_onGenEvt(QSignal sig, char const* params) {
    Q_UNUSED_PAR(params); /* QHsmTst does not use params */

    static QEvt evt = { 0U, 0U, 0U };
    evt.sig = sig;
    return &evt;
}

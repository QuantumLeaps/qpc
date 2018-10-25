#include "qpc.h"
#include "my_app.h"

//Q_DEFINE_THIS_FILE

/* MyAO declaration --------------------------------------------------------*/
typedef struct {
    QActive super;

/* private: */
} MyAO;

/* protected: */
static QState MyAO_initial(MyAO * const me, QEvt const * const e);
static QState MyAO_active(MyAO * const me, QEvt const * const e);

/* Local objects -----------------------------------------------------------*/
static MyAO l_MyAO; /* the single instance of the MyAO active object */

/* Global-scope objects ----------------------------------------------------*/
QActive * const AO_MyAO = &l_MyAO.super; /* "opaque" AO pointer */

/* MyAO_ctor ...............................................................*/
void MyAO_ctor(void) {
    MyAO *me = &l_MyAO;
    QActive_ctor(&me->super, Q_STATE_CAST(&MyAO_initial));
}

/* MyAO::SM ................................................................*/
static QState MyAO_initial(MyAO * const me, QEvt const * const e) {
    (void)e; /* unused parameter */

    QS_FUN_DICTIONARY(&QHsm_top);
    QS_FUN_DICTIONARY(&MyAO_initial);
    QS_FUN_DICTIONARY(&MyAO_active);

    QS_SIG_DICTIONARY(MY_EVT0_SIG,   (void *)0);
    QS_SIG_DICTIONARY(MY_EVT1_SIG,   (void *)0);
    QS_SIG_DICTIONARY(MY_EVT2_SIG,   (void *)0);
    QS_SIG_DICTIONARY(MY_EVT3_SIG,   (void *)0);

    return Q_TRAN(&MyAO_active);
}
/*${AOs::MyAO::SM::active} ................................................*/
static QState MyAO_active(MyAO * const me, QEvt const * const e) {
    QState status_;
    switch (e->sig) {
        case MY_EVT0_SIG: {
            status_ = Q_HANDLED();
            break;
        }
        case MY_EVT1_SIG: {
            status_ = Q_HANDLED();
            break;
        }
        case MY_EVT2_SIG: {
            status_ = Q_HANDLED();
            break;
        }
        case MY_EVT3_SIG: {
            status_ = Q_HANDLED();
            break;
        }
        default: {
            status_ = Q_SUPER(&QHsm_top);
            break;
        }
    }
    return status_;
}

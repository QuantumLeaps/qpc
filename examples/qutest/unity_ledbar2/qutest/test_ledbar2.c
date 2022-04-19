#include "qpc.h"  /* QUTest interface */
#include "LedBar2.h" /* CUT interface */

Q_DEFINE_THIS_FILE

/*--------------------------------------------------------------------------*/
void Led_DICTIONARY(void); /* dictionaries for the Led "spy " test double */
void Spi_DICTIONARY(void); /* dictionaries for the SPI "spy " test double */

/*--------------------------------------------------------------------------*/
int main(int argc, char *argv[]) {

    QF_init();  /* initialize the framework */

    /* initialize the QS software tracing */
    Q_ALLEGE(QS_INIT(argc > 1 ? argv[1] : (void *)0));

    /* dictionaries... */
    Led_DICTIONARY();
    Spi_DICTIONARY();
    QS_FUN_DICTIONARY(&LedBar2_setPercent);

    /* filter setup... */
    QS_GLB_FILTER(QS_ALL_RECORDS);

    return QF_run(); /* run the tests */
}

/*--------------------------------------------------------------------------*/
void QS_onTestSetup(void) {
}
/*..........................................................................*/
void QS_onTestTeardown(void) {
}

/*..........................................................................*/
void QS_onCommand(uint8_t cmdId,
                  uint32_t param1, uint32_t param2, uint32_t param3)
{
    switch (cmdId) {
        case 0: { /* call LedBar2_setPercent */
            uint32_t ret = LedBar2_setPercent((uint8_t)param1);
            QS_BEGIN_ID(QS_USER + cmdId, 0U) /* app-specific record */
                QS_FUN(&LedBar2_setPercent); /* function called */
                QS_U32(0, ret); /* value returned */
                QS_U8 (0, (uint8_t)param1); /* parameter */
            QS_END()
            break;
        }
        default:
            break;
    }

    /* unused parametrers... */
    (void)param2;
    (void)param3;
}
/*..........................................................................*/
/* host callback function to "massage" the event, if necessary */
void QS_onTestEvt(QEvt *e) {
    (void)e;
#ifdef Q_HOST  /* is this test compiled for a desktop Host computer? */
#else /* this test is compiled for an embedded Target system */
#endif
}
/*..........................................................................*/
/*! callback function to output the posted QP events (not used here) */
void QS_onTestPost(void const *sender, QActive *recipient,
                   QEvt const *e, bool status)
{
    (void)sender;
    (void)recipient;
    (void)e;
    (void)status;
}

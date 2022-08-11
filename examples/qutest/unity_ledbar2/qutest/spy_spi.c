#include "qpc.h"  /* QUTest interface */
#include "Led.h"  /* original interface */

//Q_DEFINE_THIS_FILE

enum {
    SPI_MOD = QS_USER2 /* QS app-specific record for the SPI module */
};

void SPI_beginTransaction(void);
void SPI_transfer(uint32_t data);
void SPI_endTransaction(void);

/*--------------------------------------------------------------------------*/
void Spi_DICTIONARY(void) {
    QS_FUN_DICTIONARY(&SPI_beginTransaction);
    QS_FUN_DICTIONARY(&SPI_transfer);
    QS_FUN_DICTIONARY(&SPI_endTransaction);
    QS_USR_DICTIONARY(SPI_MOD);
}

/*--------------------------------------------------------------------------*/
void SPI_beginTransaction(void) {
    QS_BEGIN_ID(SPI_MOD, 0U) /* app-specific record */
        QS_FUN(&SPI_beginTransaction); /* function called */
    QS_END()
}

void SPI_transfer(uint32_t data) {
    QS_BEGIN_ID(SPI_MOD, 0U) /* app-specific record */
        QS_FUN(&SPI_transfer); /* function called */
        QS_U32(0, data); /* parameter */
    QS_END()
}

void SPI_endTransaction(void) {
    QS_BEGIN_ID(SPI_MOD, 0U) /* app-specific record */
        QS_FUN(&SPI_endTransaction); /* function called */
    QS_END()
}

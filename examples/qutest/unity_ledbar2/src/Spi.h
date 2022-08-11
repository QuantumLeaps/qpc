#ifndef SPI_H
#define SPI_H

void SPI_beginTransaction(void);
void SPI_transfer(uint32_t data);
void SPI_endTransaction(void);

#endif /* SPI_H */
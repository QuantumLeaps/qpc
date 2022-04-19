#ifndef LED_H
#define LED_H

enum { MAX_LED = 5 };

/* turns a given LED on and retruns the power drawn by it in uW */
uint32_t Led_on(uint8_t index);

/* turns a given LED off */
void Led_off(uint8_t index);

#endif /* LED_H */
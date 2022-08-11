/* bsp.h ----------------- */
extern QTicker ticker0; /* "ticker" AO for clock rate 0 */
extern QTicker ticker1; /* "ticker" AO for clock rate 1 */


/* bsp.c ----------------- */
QTicker ticker0; /* "ticker" AO for clock rate 0 */
QTicker ticker1; /* "ticker" AO for clock rate 1 */

/* system clock tick ISR for clock rate 0 */
void SysTick_Handler(void) {
    . . .
    QACTIVE_POST(&ticker0.super, (QEvt *)0, &qs_tick0_id);
    . . .
}

/* system clock tick ISR for clock rate 1 */
void Timer0A_IRQHandler(void) {
    . . .
    QACTIVE_POST(&ticker1.super, (QEvt *)0, &qs_tick1_id);
    . . .
}


/* main.c --------------- */
main () {
    . . .
    QACTIVE_START(&ticker0.super,
                  1U, /* priority */
                  0, 0, 0, 0, 0); /* not used */

    QACTIVE_START(&ticker1.super,
                  2U, /* priority */
                  0, 0, 0, 0, 0); /* not used */
    . . .
}

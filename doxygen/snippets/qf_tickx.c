/* case 1: Interrupt Controller available,
* "unconditional interrupt unlocking" critical section policy
* (nesting of critical sections _not_ allowed)
*/
interrupt void ISR_timer() { /* entered with interrupts disabled in hardware */
    QF_INT_ENABLE();  /* enable interrupts */

    QF_TICK_X(0U, &l_ISR_timer); /*<-- QF tick processing for rate 0 */

    QF_INT_DISABLE(); /* disable interrupts again */
    /* send the EOI instruction to the Interrupt Controller */
}

/* case 2: Interrupt Controller not used,
* "saving and restoring interrupt status" critical section policy
* (nesting of critical sections allowed)
*/
interrupt void ISR_timer() {
    QF_TICK_X(1U, &l_ISR_timer); /*<-- QF tick processing for rate 1 */
}

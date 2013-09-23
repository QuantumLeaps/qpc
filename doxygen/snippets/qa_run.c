/* thread routine for Win32 _beginthread() */
static void __cdecl run(void *me) { /* the exact signature for _beginthread */
    do {
        QEvt const *e;

        QACTIVE_GET_((QActive *)me, e);               /* wait for the event */
        QF_ACTIVE_DISPATCH_(me, e);   /* dispatch to the active object's SM */
        QF_gc(e);    /* check if the event is garbage, and collect it if so */

    } while (((QActive *)me)->prio > (uint8_t)0);

    QActive_unsubscribeAll((QActive *)me);  /* unsubscribe from all signals */
    QF_remove_((QActive *)me); /* remove this object from any subscriptions */

    _endthread();    /* cleanup after the thead and close the thread handle */
}

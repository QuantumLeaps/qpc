#include "qpc.h"   /* QP/C public interface */
#include "bomb.h"  /* Bomb derived from QHsm */

Q_DEFINE_THIS_FILE

static Bomb l_bomb;  /* an instance of Bomb HSM */

int main() {
    Bomb_ctor(&l_bomb); /* Bomb "constructor" invokes QFsm_ctor() */

    QMSM_INIT(&l_bomb.super, (QEvt *)0); /* trigger initial transition */

    for (;;) {  /* event loop */
        QEvt e;
        . . .
        /* wait for the next event and assign it to the event object e */
        . . .
        QMSM_DISPATCH(&l_bomb.super, &e);  /* dispatch e */
    }
    return 0; /* never reached, needed for some compilers */
}

/*****************************************************************************
* Product: No heap support
* Date of the Last Update:  Jun 29, 2007
*
*                    Q u a n t u m     L e a P s
*                    ---------------------------
*                    innovating embedded systems
*
* Copyright (C) 2002-2007 Quantum Leaps, LLC. All rights reserved.
*
* Contact information:
* Quantum Leaps Web site:  http://www.quantum-leaps.com
* e-mail:                  info@quantum-leaps.com
*****************************************************************************/

#include <stdlib.h>                /* for prototypes of malloc() and free() */

/*..........................................................................*/
void *malloc(size_t size) {
    return (void *)0;
}
/*..........................................................................*/
void free(void *ptr) {
}

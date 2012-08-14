/*****************************************************************************
* Product: Number data entry
* Last Updated for Version: 4.0.01
* Date of the Last Update:  Sep 22, 2008
*
*                    Q u a n t u m     L e a P s
*                    ---------------------------
*                    innovating embedded systems
*
* Copyright (C) 2002-2008 Quantum Leaps, LLC. All rights reserved.
*
* This software may be distributed and modified under the terms of the GNU
* General Public License version 2 (GPL) as published by the Free Software
* Foundation and appearing in the file GPL.TXT included in the packaging of
* this file. Please note that GPL Section 2[b] requires that all works based
* on this software must also be made publicly available under the terms of
* the GPL ("Copyleft").
*
* Alternatively, this software may be distributed and modified under the
* terms of Quantum Leaps commercial licenses, which expressly supersede
* the GPL and are specifically designed for licensees interested in
* retaining the proprietary status of their code.
*
* Contact information:
* Quantum Leaps Web site:  http://www.quantum-leaps.com
* e-mail:                  info@quantum-leaps.com
*****************************************************************************/
#ifndef num_ent_h
#define num_ent_h

#define NUM_STR_WIDTH  13

typedef struct NumEntryTag {
    QHsm    super;                                      /* derive from QHsm */
    char    str[NUM_STR_WIDTH + 1];  /* string representation of the number */
    uint8_t len;                          /* number of displayed characters */
    uint8_t x;            /* x-coordinate of the number entry on the screen */
    uint8_t y;            /* y-coordinate of the number entry on the screen */
    uint8_t color;    /* foreground color of the number entry on the screen */
} NumEntry;

void  NumEntry_ctor  (NumEntry *me);                                /* ctor */
void  NumEntry_xtor  (NumEntry *me);                                /* xtor */
void  NumEntry_config(NumEntry *me, uint8_t x, uint8_t y, uint8_t color);
void  NumEntry_insert(NumEntry *me, uint8_t keyId);
double NumEntry_get  (NumEntry *me);  /* return the value from number entry */

#endif                                                         /* num_ent_h */

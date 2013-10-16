/*****************************************************************************
* Product: "Fly'n'Shoot" game example
* Last Updated for Version: 4.5.02
* Date of the Last Update:  Jul 04, 2012
*
*                    Q u a n t u m     L e a P s
*                    ---------------------------
*                    innovating embedded systems
*
* Copyright (C) 2002-2012 Quantum Leaps, LLC. All rights reserved.
*
* This program is open source software: you can redistribute it and/or
* modify it under the terms of the GNU General Public License as published
* by the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* Alternatively, this program may be distributed and modified under the
* terms of Quantum Leaps commercial licenses, which expressly supersede
* the GNU General Public License and are specifically designed for
* licensees interested in retaining the proprietary status of their code.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*
* Contact information:
* Quantum Leaps Web sites: http://www.quantum-leaps.com
*                          http://www.state-machine.com
* e-mail:                  info@quantum-leaps.com
*****************************************************************************/
#include "qp_port.h"
#include "game.h"

/*..........................................................................*/
ObjectPosEvt *ObjectPosEvt_ctor(ObjectPosEvt * const me, QSignal sig,
                                uint8_t x, uint8_t y)
{
    me->super.sig = sig;
    me->x = x;
    me->y = y;
    return me;
}
/*..........................................................................*/
ObjectImageEvt *ObjectImageEvt_ctor(ObjectImageEvt * const me, QSignal sig,
                                uint8_t x, int8_t y, uint8_t bmp)
{
    me->super.sig = sig;
    me->x = x;
    me->y = y;
    me->bmp = bmp;
    return me;
}
/*..........................................................................*/
MineEvt *MineEvt_ctor(MineEvt * const me, QSignal sig,
                      uint8_t id)
{
    me->super.sig = sig;
    me->id = id;
    return me;
}
/*..........................................................................*/
ScoreEvt *ScoreEvt_ctor(ScoreEvt * const me, QSignal sig,
                        uint16_t score)
{
    me->super.sig = sig;
    me->score = score;
    return me;
}


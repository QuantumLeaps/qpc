/*****************************************************************************
* Product: DPP example
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
* by the Free Software Foundation, either version 2 of the License, or
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
#ifndef display_h
#define display_h

#include "qp_port.h"
#include "dpp.h"
#include <stdint.h>
#include <EGL/egl.h>
#include <GLES/gl.h>


/**
 * Saved state, persisted by Android system
 */
struct saved_state {
    char philo_status[N_PHILO];    /* hint for drawing  philosophers status */
    uint8_t table_paused;          /* hint for drawing  table status        */
};


/**
 * Shared state for our app.
 */
struct engine {
    struct android_app *app;                 /* application instance        */

    EGLDisplay display;                      /* OpenGL display              */
    EGLSurface surface;                      /* OpenGL surface              */
    EGLContext context;                      /* OpenGL context              */

    uint8_t animating;                       /* are we currently animating? */
    int32_t width;                           /* width of screen, pixels     */
    int32_t height;                          /* height of screen, pixels    */

    struct saved_state state;                /* state data                  */
};


int init_display(struct engine *engine);
void draw_frame(struct engine *engine);
void term_display(struct engine *engine);

#endif                                                         /* display_h */

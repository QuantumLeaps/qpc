/*****************************************************************************
* Product: DPP example
* Last Updated for Version: 4.5.02
* Date of the Last Update:  May 18, 2012
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
#include "display.h"

#include <math.h>
#include <android_native_app_glue.h>

Q_DEFINE_THIS_MODULE("display")

/* Local-objects -----------------------------------------------------------*/
#define M_PI2           (2.0f*M_PI)
#define CIRCLE_FACES    30
#define CIRCLE_VERTEXES (CIRCLE_FACES + 2)       /* +2 = origo + end vertex */

static void draw_circle(float x, float y, float radius, float red,
                        float green, float blue, float alpha);

/*
* Helper function for drawing a circle in OpenGL
*/
static void draw_circle(float x, float y, float radius, float red,
                        float green, float blue, float alpha) {
    GLfloat coordinate_data[CIRCLE_VERTEXES * 2] = {0.0f};
    int n;

    /* First coordinate of triangle fan is the center coordinate */
    coordinate_data[0] = x;
    coordinate_data[1] = y;

    /* Place other coordinates in even circle */
    for (n = 0; n < CIRCLE_VERTEXES - 1; n++) {
        float ratio;
        int index;

        ratio = (float)n / CIRCLE_FACES;
        index = (n*2) + 2;

        coordinate_data[index]     = x + cos(ratio * M_PI2) * radius;
        coordinate_data[index + 1] = y + sin(ratio * M_PI2) * radius;
    }

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glEnableClientState(GL_VERTEX_ARRAY);

    glColor4f(red, green, blue, alpha);
    glVertexPointer(2, GL_FLOAT, 0, coordinate_data);
    glDrawArrays(GL_TRIANGLE_FAN, 0, CIRCLE_VERTEXES);

    glDisableClientState(GL_VERTEX_ARRAY);

}

/*..........................................................................*/
int init_display(struct engine* engine) {

    /*
     * Here specify the attributes of the desired configuration.
     * Below, we select an EGLConfig with at least 8 bits per color
     * component compatible with on-screen windows
     */
    const EGLint attribs[] = {
            EGL_SURFACE_TYPE,
            EGL_WINDOW_BIT,
            EGL_BLUE_SIZE, 8,
            EGL_GREEN_SIZE, 8,
            EGL_RED_SIZE, 8,
            EGL_NONE
    };

    EGLint w, h, format;
    EGLint numConfigs;
    EGLConfig config;
    EGLSurface surface;
    EGLContext context;

    EGLDisplay display = eglGetDisplay(EGL_DEFAULT_DISPLAY);

    eglInitialize(display, 0, 0);

    /* Here, the application chooses the configuration it desires. In this
     * sample, we have a very simplified selection process, where we pick
     * the first EGLConfig that matches our criteria */
    eglChooseConfig(display, attribs, &config, 1, &numConfigs);

    /* EGL_NATIVE_VISUAL_ID is an attribute of the EGLConfig that is
     * guaranteed to be accepted by ANativeWindow_setBuffersGeometry().
     * As soon as we picked a EGLConfig, we can safely reconfigure the
     * ANativeWindow buffers to match, using EGL_NATIVE_VISUAL_ID. */
    eglGetConfigAttrib(display, config, EGL_NATIVE_VISUAL_ID, &format);

    ANativeWindow_setBuffersGeometry(engine->app->window, 0, 0, format);

    surface = eglCreateWindowSurface(display, config,
                                     engine->app->window, NULL);
    context = eglCreateContext(display, config, NULL, NULL);

    Q_ALLEGE(eglMakeCurrent(display, surface, surface, context) != EGL_FALSE);

    eglQuerySurface(display, surface, EGL_WIDTH, &w);
    eglQuerySurface(display, surface, EGL_HEIGHT, &h);

    engine->display = display;
    engine->context = context;
    engine->surface = surface;
    engine->width = w;
    engine->height = h;

    /* Initialize GL state. */
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);
    glDisable(GL_TEXTURE_2D);

    /* configure simple drawing model */
    glShadeModel(GL_FLAT);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);

    /* setup simple viewport, where coordinates are based on screen size */
    glViewport(0,0, w, h);
    glOrthof(0, (float)w, (float)h, 0, 0, 1);

    return 0;
}

/*..........................................................................*/
void draw_frame(struct engine* engine) {

    if (engine->display == NULL) {
        return;
    }

    /* Calculate circle radiuses based on screen size */
    float philo_radius = engine->width / (2.0 * N_PHILO);
    float table_radius = engine->width / 6.0;
    int n;

    /* Clear screen with black */
    glClearColor(0.0f, 0.0f, 0.0f, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    if (engine->state.table_paused) {
        draw_circle(engine->width / 2.0,        /* Draw table in the middle */
                    engine->height / 2.0,
                    table_radius,
                    0.5,
                    0.5,
                    0.5,
                    1.0);
    }
    else {
        draw_circle(engine->width / 2.0,
                    engine->height / 2.0,
                    table_radius,
                    1.0,
                    1.0,
                    1.0,
                    1.0);
    }

    for (n = 0; n < N_PHILO; n++) {       /* Draw philosophers around table */
        float r, g, b;
        float x, y;
        float radius = table_radius * 1.3f;
        float ratio;

        x = engine->width / 2.0f;
        y = engine->height / 2.0f;
        ratio = (float)n / (float) N_PHILO;

        /* distribute philosophers evenly */
        x += cos(ratio * M_PI2) * radius;
        y += sin(ratio * M_PI2) * radius;

        switch (engine->state.philo_status[n]) {
            default:
            case 'h': {
                /* hungry */
                r = 1.0f;
                g = 0.0f;
                b = 0.0f;
                break;
            }

            case 'e': {
                /* eating */
                r = 0.0f;
                g = 1.0f;
                b = 0.0f;
                break;
            }

            case 't': {
                /* thinking */
                r = 0.0f;
                g = 0.0f;
                b = 1.0f;
                break;
            }
        }
        draw_circle(x, y, philo_radius, r, g, b, 1.0f);
    }
    eglSwapBuffers(engine->display, engine->surface);
}

/*..........................................................................*/
void term_display(struct engine* engine)  {

    if (engine->display != EGL_NO_DISPLAY) {
        eglMakeCurrent(engine->display, EGL_NO_SURFACE,
                       EGL_NO_SURFACE, EGL_NO_CONTEXT);

        if (engine->context != EGL_NO_CONTEXT) {
            eglDestroyContext(engine->display, engine->context);
        }

        if (engine->surface != EGL_NO_SURFACE) {
            eglDestroySurface(engine->display, engine->surface);
        }
        eglTerminate(engine->display);
    }

    engine->animating = 0U;
    engine->display = EGL_NO_DISPLAY;
    engine->context = EGL_NO_CONTEXT;
    engine->surface = EGL_NO_SURFACE;
}

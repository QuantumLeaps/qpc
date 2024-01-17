//============================================================================
// Product: DPP example, GTK3+-GUI
// Last updated for version: 7.3.0
// Date of the Last Update:  2023-08-21
//
//                   Q u a n t u m  L e a P s
//                   ------------------------
//                   Modern Embedded Software
//
// Copyright (C) 2005 Quantum Leaps, LLC. All rights reserved.
//
// This program is open source software: you can redistribute it and/or
// modify it under the terms of the GNU General Public License as published
// by the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Alternatively, this program may be distributed and modified under the
// terms of Quantum Leaps commercial licenses, which expressly supersede
// the GNU General Public License and are specifically designed for
// licensees interested in retaining the proprietary status of their code.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <www.gnu.org/licenses/>.
//
// Contact information:
// <www.state-machine.com/licensing>
// <info@state-machine.com>
//============================================================================
#include "qpc.h"
#include "dpp.h"
#include "bsp.h"

#include <glib.h>
#include <gtk/gtk.h>
#include "dppImages.h"

#include "safe_std.h" // portable "safe" <stdio.h>/<string.h> facilities

Q_DEFINE_THIS_FILE

// local variables -----------------------------------------------------------
static GtkApplication* l_app;
static GMainContext *l_mainContext;
static GtkWidget *l_hwnd;
static GtkWidget *grid;
static GtkWidget *texts[2] = {NULL};
static GtkWidget *philos[N_PHILO] = {NULL};

static GMutex mymutex;
static gboolean requestQuit = FALSE;
static GRand *myRnd;
static guint32 l_rnd;
static gchar *l_cmdline = NULL;
static gboolean run = TRUE;
static gchar const *play =  "media-playback-start";
static gchar const *pause = "media-playback-pause";
static const gchar *philoPics[3] = {
    "/dpp/example/pics/think",
    "/dpp/example/pics/hungry",
    "/dpp/example/pics/eat"
};
static guint8 philoPos[N_PHILO][2] = {
    {2, 0},
    {0, 1}, {4, 1},
    {1, 2}, {3, 2} 
};
static gint philoState[N_PHILO] = {-1, -1, -1, -1, -1};

// Local functions -----------------------------------------------------------
static gint theApp(GtkApplication* app, gpointer user_data);
static gint MessageBox(GtkWidget *hwnd, const gchar *text, const gchar *caption, gint type);
static void ButtonClicked(GtkButton *btn, GtkWidget *widget);
static void copy(GtkButton *btn, GtkWidget *widget);
static void destroy(GtkWidget *widget, gpointer data);
static gboolean eventHandler(GtkWidget *widget);

static void appThread(GTask *task, gpointer source_object, gpointer task_data, GCancellable *cancellable);
static void appThreadReadyCallback (GObject *source_object, GAsyncResult *res, gpointer user_data);

//............................................................................
// simulte Win32 function MessageBox(...)
static gint MessageBox(GtkWidget *hwnd, const gchar *text, const gchar *caption, gint type)
{
    GtkWidget *dialog = NULL;
    gint result = GTK_RESPONSE_NO;

    if (type & GTK_BUTTONS_YES_NO)
    {
        dialog = gtk_message_dialog_new(GTK_WINDOW(hwnd), GTK_DIALOG_MODAL, GTK_MESSAGE_QUESTION, GTK_BUTTONS_YES_NO, text);
    } else {
        dialog = gtk_message_dialog_new(GTK_WINDOW(hwnd), GTK_DIALOG_MODAL, GTK_MESSAGE_WARNING, GTK_BUTTONS_OK, text);
    }

    gtk_window_set_title(GTK_WINDOW(dialog), caption);
    result = gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(GTK_WIDGET(dialog));

    switch(result)
    {
        case GTK_RESPONSE_YES:
        case GTK_RESPONSE_OK:
            return GTK_RESPONSE_OK;
        default:
            return GTK_RESPONSE_NO;
    }

    return GTK_RESPONSE_OK;
}

//............................................................................
// thread function for running the application main_gui()
static void appThreadReadyCallback(GObject *source_object, GAsyncResult *res, gpointer user_data)
{
    (void)source_object;
    (void)res;
    (void)user_data;
}

static void appThread(GTask *task, gpointer source_object, gpointer task_data, GCancellable *cancellable)
{
    guint32 result;

    (void)source_object;
    (void)task_data;
    (void)cancellable;

    result = (guint32)main_gui(); /* run the QF application */

    g_task_return_int(task, result);
}

//............................................................................
Q_NORETURN Q_onError(char const * const module, int_t const id) {
    char message[80];
    QF_stop(); // stop ticking

    QS_ASSERTION(module, id, 10000U); // report assertion to QS
    SNPRINTF_S(message, Q_DIM(message) - 1U,
               "Assertion failed in module %s location %d", module, id);
    MessageBox(l_hwnd, message, "!!! ASSERTION !!!", GTK_BUTTONS_OK);
    g_application_quit(G_APPLICATION(l_app));
}
//............................................................................
void assert_failed(char const * const module, int_t const id); // prototype
void assert_failed(char const * const module, int_t const id)
{
    Q_onError(module, id);
}

//----------------------------------------------------------------------------
#ifdef main
#undef main
#endif
int main(int argc, char *argv[])
{
    int status;

    if(argc > 1)
    {
        l_cmdline = argv[1];
    }

    GtkApplication *app = gtk_application_new ("com.state-machine.example", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect (app, "activate", G_CALLBACK (theApp), NULL);
    g_signal_connect (app, "shutdown", G_CALLBACK (destroy), app);
    status = g_application_run (G_APPLICATION (app), argc, argv);
    g_object_unref (app);

    return status;
}

//............................................................................
static gint theApp(GtkApplication* app, gpointer user_data)
{
    GTask     *tlThread;
    GtkWidget *window = gtk_application_window_new(app);

    myRnd = g_rand_new();
    l_mainContext = g_main_context_default();
    l_hwnd = window;
    l_app = app;
    g_mutex_init(&mymutex);

    GdkPixbuf *icon = gdk_pixbuf_new_from_resource("/dpp/example/icons/qp", NULL);

    GtkWidget *smLabel = gtk_label_new(NULL);
    GtkWidget *empty = gtk_label_new("");

    GtkWidget *button = gtk_button_new();
    GtkWidget *btnCopy = gtk_button_new_with_label("Copy");
    GtkWidget *btnQuit = gtk_button_new_from_icon_name("application-exit", GTK_ICON_SIZE_BUTTON);

    GtkWidget *btnBox = gtk_button_box_new(GTK_ORIENTATION_HORIZONTAL);
    GtkWidget *editBox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 4);

    grid = gtk_grid_new();

    gtk_label_set_markup(GTK_LABEL(smLabel), "<span foreground=\"blue\" size=\"smaller\"><i>https://www.state-machine.com</i></span>");

    texts[0] = gtk_entry_new();
    texts[1] = gtk_entry_new();
    gtk_entry_set_text(GTK_ENTRY(texts[0]), "Edit1...");
    gtk_entry_set_text(GTK_ENTRY(texts[1]), gtk_entry_get_text(GTK_ENTRY(texts[0])));

    gtk_window_set_title (GTK_WINDOW (window), "DPP Example");
    gtk_window_set_default_size (GTK_WINDOW (window), 400, 250);
    gtk_window_set_resizable(GTK_WINDOW (window), FALSE);

    gtk_window_set_icon(GTK_WINDOW (window), icon);

    gtk_button_set_always_show_image(GTK_BUTTON(btnQuit), TRUE);
    gtk_button_set_image(GTK_BUTTON(button), gtk_image_new_from_icon_name(pause, GTK_ICON_SIZE_BUTTON));
    gtk_button_set_always_show_image(GTK_BUTTON(button), TRUE);

    gtk_button_box_set_layout(GTK_BUTTON_BOX(btnBox), GTK_BUTTONBOX_END);
    // gtk_box_pack_end(GTK_BOX(btnBox), btnQuit, TRUE, TRUE, 5);

    gtk_box_pack_start(GTK_BOX(editBox), texts[0], TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(editBox), btnCopy, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(editBox), texts[1], TRUE, TRUE, 0);

    for(gint i = 0; i < N_PHILO; i++)
    {
        philos[i] = gtk_image_new_from_resource("/dpp/example/pics/think");
        gtk_grid_attach(GTK_GRID(grid), philos[i], philoPos[i][0], philoPos[i][1], 1, 1);
    }

    gtk_grid_attach(GTK_GRID(grid), btnQuit, 4, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), button, 2, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), empty, 0, 3, 5, 1);
    gtk_grid_attach(GTK_GRID(grid), editBox, 0, 4, 5, 1);
    gtk_grid_attach(GTK_GRID(grid), smLabel, 0, 5, 5, 1);

    g_signal_connect(G_OBJECT(button), "clicked", (GCallback)ButtonClicked, NULL);
    g_signal_connect(G_OBJECT(btnQuit), "clicked", (GCallback)destroy, app);
    g_signal_connect(G_OBJECT(btnCopy), "clicked", (GCallback)copy, NULL);
    g_signal_connect(G_OBJECT(window), "destroy", (GCallback)destroy, app);

    gtk_container_add(GTK_CONTAINER(window), GTK_WIDGET(grid));

    g_timeout_add_full(G_PRIORITY_HIGH, 100, (GSourceFunc)eventHandler, app, NULL);

    tlThread = g_task_new(NULL, NULL, appThreadReadyCallback, NULL);
    if ((GTask *)(0) == tlThread)
    {
        Q_ERROR();
    }
    g_task_set_task_data(tlThread, app, NULL);
    g_task_set_return_on_cancel (tlThread, TRUE);
    g_task_run_in_thread (tlThread, appThread);
#if 0
    if(NULL == g_thread_try_new(NULL, (GThreadFunc)appThread, NULL, NULL))
    {
        Q_ERROR();
    }
#endif
    gtk_widget_show_all (window);

    return 0;
}

//............................................................................
static gboolean eventHandler(GtkWidget *data)
{
    if(requestQuit) {
        g_application_quit(G_APPLICATION(data));
        return FALSE;
    }

    if(g_mutex_trylock(&mymutex))
    {
        for(gint n = 0; n < N_PHILO; n++)
        {
            gint l = philoState[n];
            if(l >= 0)
            {
                gtk_image_set_from_resource(GTK_IMAGE(philos[n]), philoPics[l]);
                philoState[n] = -1;
            }
        }
        g_mutex_unlock(&mymutex);
    }
    return TRUE;
}

//............................................................................
static void ButtonClicked(GtkButton *btn, GtkWidget *widget)
{
    static GtkWidget *btnImg;
    static const QEvt pe[2] = {
        QEVT_INITIALIZER(PAUSE_SIG),
        QEVT_INITIALIZER(SERVE_SIG)
    };
    gint peIdx;

    g_mutex_lock(&mymutex);
    if(run)
    {
        peIdx = 0;
        btnImg = gtk_image_new_from_icon_name(play, GTK_ICON_SIZE_BUTTON);
    }
    else
    {
        peIdx = 1;
        btnImg = gtk_image_new_from_icon_name(pause, GTK_ICON_SIZE_BUTTON);
    }

    gtk_button_set_image(GTK_BUTTON(btn), btnImg);
    run = !run;
    g_mutex_unlock(&mymutex);

    QACTIVE_POST(AO_Table, &pe[peIdx], (void *)0);
}

//............................................................................
static void copy(GtkButton *btn, GtkWidget *widget)
{
    g_mutex_lock(&mymutex);
    gtk_entry_set_text(GTK_ENTRY(texts[1]), gtk_entry_get_text(GTK_ENTRY(texts[0])));
    g_mutex_unlock(&mymutex);
}

//............................................................................
static void destroy(GtkWidget *widget, gpointer data)
{
    g_application_quit(G_APPLICATION(data));
}

//............................................................................
void QF_onStartup(void) {
    QF_setTickRate(BSP_TICKS_PER_SEC, 30); // set the desired tick rate
}
//............................................................................
void QF_onCleanup(void) {
}
//............................................................................
void QF_onClockTick(void) {
    QTIMEEVT_TICK_X(0U, &l_clock_tick); // QF clock tick processing

    QS_RX_INPUT(); // handle the QS-RX input
    QS_OUTPUT();   // handle the QS output
}

//............................................................................
void BSP_init(void) {
    if (!QS_INIT(l_cmdLine)) { // QS initialization failed?
        MessageBox(l_hwnd,
                   "Cannot connect to QSPY via TCP/IP\n"
                   "Please make sure that 'qspy -t' is running",
                   "QS_INIT() Error",
                   GTK_BUTTONS_OK);
    }

    // send the QS dictionaries...
    QS_OBJ_DICTIONARY(&l_clock_tick);
    QS_USR_DICTIONARY(PHILO_STAT);

    // setup the QS filters...
    QS_GLB_FILTER(QS_ALL_RECORDS);
    QS_GLB_FILTER(-QS_QF_TICK);
}
//............................................................................
void BSP_start(void) {
    // initialize event pools
    static QF_MPOOL_EL(TableEvt) smlPoolSto[2*N_PHILO];
    QF_poolInit(smlPoolSto, sizeof(smlPoolSto), sizeof(smlPoolSto[0]));

    // initialize publish-subscribe
    static QSubscrList subscrSto[MAX_PUB_SIG];
    QActive_psInit(subscrSto, Q_DIM(subscrSto));

    // instantiate and start AOs/threads...

    static QEvt const *philoQueueSto[N_PHILO][10];
    for (uint8_t n = 0U; n < N_PHILO; ++n) {
        Philo_ctor(n);
        QACTIVE_START(AO_Philo[n],
            n + 3U,                  // QF-prio/pthre. see NOTE1
            philoQueueSto[n],        // event queue storage
            Q_DIM(philoQueueSto[n]), // queue length [events]
            (void *)0, 0U,           // no stack storage
            (void *)0);              // no initialization param
    }

    static QEvt const *tableQueueSto[N_PHILO];
    Table_ctor();
    QACTIVE_START(AO_Table,
        N_PHILO + 7U,                // QP prio. of the AO
        tableQueueSto,               // event queue storage
        Q_DIM(tableQueueSto),        // queue length [events]
        (void *)0, 0U,               // no stack storage
        (void *)0);                  // no initialization param
}
//............................................................................
void BSP_terminate(gint16 result) {
    QF_stop(); // stop the main QF application

    // cleanup all QWIN resources...

    // end the main dialog
    requestQuit = TRUE;
    g_thread_exit(0);

    //g_application_quit(G_APPLICATION(l_app));
}
//............................................................................
void BSP_displayPhilStat(uint8_t n, char const *stat)
{
    guint8 bitmapNum = 0;

    Q_REQUIRE(n < N_PHILO);

    switch (stat[0]) {
        case 't': bitmapNum = 0U; break;
        case 'h': bitmapNum = 1U; break;
        case 'e': bitmapNum = 2U; break;
        default: Q_ERROR();  break;
    }
    // set the "segment" # n to the bitmap # 'bitmapNum'
    // gtk_image_set_from_resource(GTK_IMAGE(philos[n]), philoPics[bitmapNum]);
    g_mutex_lock(&mymutex);
    philoState[n] = bitmapNum;
    g_mutex_unlock(&mymutex);
    
    QS_BEGIN_ID(PHILO_STAT, AO_Philo[n]->prio) // app-specific record
        QS_U8(1, n);  // Philosopher number
        QS_STR(stat); // Philosopher status
    QS_END()
}
//............................................................................
void BSP_displayPaused(guint8 paused)
{
    // already done in button callback
}
//............................................................................
guint32 BSP_random(void) {  // a very cheap pseudo-random-number generator
    // "Super-Duper" Linear Congruential Generator (LCG)
    // LCG(2^32, 3*7*11*13*23, 0, seed)
    //
    l_rnd = g_rand_int(myRnd);
    return l_rnd >> 8;
}
//............................................................................
void BSP_randomSeed(guint32 seed) {
    g_rand_set_seed (myRnd, seed);
}


//----------------------------------------------------------------------------
#ifdef Q_SPY // define QS callbacks

//! callback function to execute user commands
void QS_onCommand(uint8_t cmdId,
                  uint32_t param1, uint32_t param2, uint32_t param3)
{
    switch (cmdId) {
       case 0U: {
           break;
       }
       default:
           break;
    }

    // unused parameters
    (void)param1;
    (void)param2;
    (void)param3;
}

#endif // Q_SPY
//----------------------------------------------------------------------------

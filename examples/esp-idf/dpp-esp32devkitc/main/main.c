#include "esp_log.h"
#include "qpc.h"
#include "dpp.h"
#include "freertos/FreeRTOS.h"

Q_DEFINE_THIS_FILE
static const char * TAG = "main";

/*
 * small event memory pool
 */
static QF_MPOOL_EL(QEvt) smallPoolSto[CONFIG_QPC_SMALL_POOL_SIZE];

/*
 * medium size event memory pool
 * size: QEvt + CONFIG_QPC_MEDIUM_POOL_ENTRY_SIZE words
 */
typedef struct {
    QEvt super;
    uint32_t data[CONFIG_QPC_MEDIUM_POOL_ENTRY_SIZE];
} mediumPool;
static QF_MPOOL_EL(mediumPool) mediumPoolSto[CONFIG_QPC_MEDIUM_POOL_SIZE];

/*
 * large size event memory pool
 * size: QEvt + CONFIG_QPC_LARGE_POOL_ENTRY_SIZE words
 */
typedef struct {
    QEvt super;
    uint32_t data[CONFIG_QPC_LARGE_POOL_ENTRY_SIZE];
} largePool;
static QF_MPOOL_EL(largePool) largePoolSto[CONFIG_QPC_LARGE_POOL_SIZE];

static QEvt const *tableQueueSto[N_PHILO];
static QEvt const *philoQueueSto[N_PHILO][N_PHILO];
static QSubscrList subscrSto[MAX_PUB_SIG];
static StackType_t philoStack[N_PHILO][2048];
static StackType_t tableStack[2048];

void app_main()
{
    uint8_t n;

    Philo_ctor(); /* instantiate all Philosopher active objects */
    Table_ctor(); /* instantiate the Table active object */

    /* Initialzie QP/C Framework */
    QF_init();

    /* initialize publish-subscribe... */
    QF_psInit(subscrSto, Q_DIM(subscrSto));

    /* Initialize Event Memory Pool */
    QF_poolInit(smallPoolSto, sizeof(smallPoolSto), sizeof(smallPoolSto[0]));
    QF_poolInit(mediumPoolSto, sizeof(mediumPoolSto), sizeof(mediumPoolSto[0]));
    QF_poolInit(largePoolSto, sizeof(largePoolSto), sizeof(largePoolSto[0]));

    /* start the active objects... */
    for (n = 0U; n < N_PHILO; ++n) {
        QActive_setAttr(AO_Philo[n], TASK_NAME_ATTR, "Philo");
        QACTIVE_START(AO_Philo[n],   /* AO to start */
            (uint_fast8_t)(n + 1),   /* QP priority of the AO */
            philoQueueSto[n],        /* event queue storage */
            Q_DIM(philoQueueSto[n]), /* queue length [events] */
            philoStack[n],           /* stack storage */
            sizeof(philoStack[n]),   /* stack size [bytes] */
            (QEvt *)0);              /* initialization event (not used) */
    }

    QActive_setAttr(AO_Table, TASK_NAME_ATTR, "Table");
    QACTIVE_START(AO_Table,          /* AO to start */
        (uint_fast8_t)(N_PHILO + 1), /* QP priority of the AO */
        tableQueueSto,               /* event queue storage */
        Q_DIM(tableQueueSto),        /* queue length [events] */
        tableStack,                  /* stack storage */
        sizeof(tableStack),          /* stack size [bytes] */
        (QEvt *)0);                  /* initialization event (not used) */

    QF_run();

    ESP_LOGI(TAG, "Goodbye app_main()");
}

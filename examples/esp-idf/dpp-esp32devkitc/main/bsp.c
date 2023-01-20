#include "qpc.h"
#include "esp_log.h"
#include "esp_freertos_hooks.h"

static const char * TAG = "bsp";

int_t qf_run_active = 0;

static IRAM_ATTR void freertos_tick_hook(void)
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    if(qf_run_active != 0) {
        // process time events for rate 0
        QTIMEEVT_TICK_FROM_ISR(0U, &xHigherPriorityTaskWoken, &freertos_tick_hook);
        // notify FreeRTOS to perform context switch from ISR, if needed
        if(xHigherPriorityTaskWoken) {
            portYIELD_FROM_ISR();
        }
    }
}

void QF_onStartup(void)
{
    esp_register_freertos_tick_hook_for_cpu(freertos_tick_hook, QPC_CPU_NUM);

    // enable QF ticks from tick hook
    qf_run_active = 100;

    ESP_LOGI(TAG, "QF started.");

    // Note: Additional hook stuff can be placed here
}

IRAM_ATTR void Q_onError(char const * const module, int_t const id)
{
    ESP_LOGE(TAG, "ERROR in %s:%d\n", module, id);
}
//............................................................................
void assert_failed(char const * const module, int_t const id); // prototype
void assert_failed(char const * const module, int_t const id) {
    Q_onError(module, id);
}

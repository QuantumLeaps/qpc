#include "qpc.h"
#include "bsp.h"

#include <drivers/gpio.h>
/* other drivers, if necessary ... */

Q_DEFINE_THIS_FILE

/* The devicetree node identifier for the "led0" alias. */
#define LED0_NODE DT_ALIAS(led0)

#if DT_NODE_HAS_STATUS(LED0_NODE, okay)
#define LED0    DT_GPIO_LABEL(LED0_NODE, gpios)
#define PIN     DT_GPIO_PIN(LED0_NODE, gpios)
#define FLAGS   DT_GPIO_FLAGS(LED0_NODE, gpios)
#else
/* A build error here means your board isn't set up to blink an LED. */
#error "Unsupported board: led0 devicetree alias is not defined"
#define LED0    ""
#define PIN     0
#define FLAGS   0
#endif

static struct device const *dev_LED0;
static struct k_timer QF_tick_timer;
static void QF_tick_function(struct k_timer *tid) {
    (void)tid; /* unused parameter */
    QF_TICK_X(0U, (void *)0);
}

void BSP_init(void) {
    dev_LED0 = device_get_binding(LED0);
    Q_ASSERT(dev_LED0 != NULL);

    int ret = gpio_pin_configure(dev_LED0, PIN, GPIO_OUTPUT_ACTIVE | FLAGS);
    Q_ASSERT(ret >= 0);

    k_timer_init(&QF_tick_timer, &QF_tick_function, NULL);
}

void BSP_ledOn(void) {
    printk("BSP_ledOn\n");
    gpio_pin_set(dev_LED0, PIN, true);
}

void BSP_ledOff(void) {
    printk("BSP_ledOff\n");
    gpio_pin_set(dev_LED0, PIN, false);
}

/* QF callbacks ============================================================*/
void QF_onStartup(void) {
    k_timer_start(&QF_tick_timer, K_MSEC(1), K_MSEC(1));
    printk("QF_onStartup\n");
}
/*..........................................................................*/
void QF_onCleanup(void) {
    printk("QF_onCleanup\n");
}

/*..........................................................................*/
Q_NORETURN Q_onAssert(char const * const module, int_t const loc) {
    /*
    * NOTE: add here your application-specific error handling
    */
    printk("\nASSERTION in %s:%d\n", module, loc);
    QS_ASSERTION(module, loc, 10000U); /* report assertion to QS */
#ifndef NDEBUG
    k_panic(); /* debug build: halt the system for error search... */
#else
    syst_reboot(); /* release build: reboot the system */
#endif
}


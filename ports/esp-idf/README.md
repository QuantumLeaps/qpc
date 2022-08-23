This directory contains the "experimental" port to the [Espressif ESP-IDF][1]
IoT Framework, which is loosely based on the [FreeRTOS kernel][2].


---------------------------------------------------------------------------
# About QP/C Port to ESP-IDF
"Experimental" means that the port has not been thouroughly tested at
Quantum Leaps and no working examples are provided.


---------------------------------------------------------------------------
# About Espressif ESP-IDF

The [Espressif ESP-IDF][1] is based on a
[significantly changed version of the FreeRTOS kernel][3]
developed by Espressif to support the ESP32 multi-core CPUs (see [ESP-IDF][1]).

The Espressif version of FreeRTOS is __NOT__ compatible with the baseline [FreeRTOS][2]
and it needs to be treated as a separate RTOS kernel. According to the comments
in the Espressif source code, FreeRTOS-ESP-IDF is based on FreeRTOS V8.2.0, but
apparently FreeRTOS-ESP32 has been updated with the newer features introduced to
the original FreeRTOS in the later versions. For example, FreeRTOS-ESP-IDF supports
the "static allocation", first introduced in baseline FreeRTOS V9.x. This QP port
to FreeRTOS-ESP-IDF takes advantage of the "static allocation".


[1]: https://www.espressif.com/en/products/sdks/esp-idf
[2]: https://freertos.org
[3]: https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/system/freertos.html

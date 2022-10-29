# About this Example
This example has been contributed by developer with GitHub handle
[gh4emb](https://github.com/gh4emb).

# ESP32 QP/C Test
Test QP/C experimental port for the ESP32, using the [Dining Philosophers
Problem (DPP)](https://www.state-machine.com/qpc/tut_dpp.html) application.

The example originated from https://github.com/sicrisembay/ESP32_QPC_Test

The sdkconfig file for ESP-IDF configuration settings is included for reference

### Step 1: setup ESP-IDF environment for your machine
```
follow instructions at https://docs.espressif.com/projects/esp-idf/en/latest/esp32s3/get-started/index.html
```
### Step 2: Build and Flash
```
cd qpc/examples/esp-idf/dpp-esp32devkitc
idf.py set-target esp32
idf.py menuconfig
idf.py build
idf.py flash monitor
```
Test result (serial output from ESP_LOGx() logs through UART0 on Micro USB connector of DevKitC board)
```
I (0) cpu_start: App cpu up.
I (201) cpu_start: Pro cpu start user code
I (201) cpu_start: cpu freq: 160000000
I (201) cpu_start: Application information:
I (205) cpu_start: Project name:     dpp-esp32devkitc
I (211) cpu_start: App version:      362c8e8
I (216) cpu_start: Compile time:     Jun 01 1215 Midday
I (222) cpu_start: ELF file SHA256:  d91119d102b29e8e...
I (228) cpu_start: ESP-IDF:          v4.3.2
I (233) heap_init: Initializing. RAM available for dynamic allocation:
I (240) heap_init: At 3FFAE6E0 len 00001920 (6 KiB): DRAM
I (246) heap_init: At 3FFB8798 len 00027868 (158 KiB): DRAM
I (252) heap_init: At 3FFE0440 len 00003AE0 (14 KiB): D/IRAM
I (259) heap_init: At 3FFE4350 len 0001BCB0 (111 KiB): D/IRAM
I (265) heap_init: At 4008BA4C len 000145B4 (81 KiB): IRAM
I (272) spi_flash: detected chip: gd
I (276) spi_flash: flash io: dio
W (279) spi_flash: Detected size(4096k) larger than the size in the binary image header(2048k). Using the size in the binary image header.
I (293) cpu_start: Starting scheduler on PRO CPU.
I (0) cpu_start: Starting scheduler on APP CPU.
I (304) table: Philo0 thinking
I (304) table: Philo1 thinking
I (314) table: Philo2 thinking
I (314) table: Philo3 thinking
I (324) table: Philo4 thinking
I (324) bsp: QF started.
I (324) main: Goodbye app_main()
I (914) table: Philo4 hungry
I (914) table: Philo4 eating
I (1194) table: Philo0 hungry
I (1234) table: Philo2 hungry
I (1234) table: Philo2 eating
I (1364) table: Philo1 hungry
I (1454) table: Philo3 hungry
I (2194) table: Philo4 thinking
I (2194) table: Philo0 eating
I (2514) table: Philo2 thinking
I (2514) table: Philo3 eating
I (3544) table: Philo4 hungry
I (3664) table: Philo2 hungry
I (3774) table: Philo0 thinking
```

## Build Tools

| Tool | Description | Version | Notes |
| --- | --- | --- | --- |
| ESP-IDF | Espressif Development Framework | v4.3.2 | Refer to ESP-IDF "Get Started" Guide to properly install prerequisite tools. Commit hash: 8bf14a9 |
| QP/C | Real-time Embedded Framework | v7.1.0-beta3 | Commit hash: 1c63248 |
| QM | QP Modeler (Model-based Design Tool) | v5.1.3 |    |
| ESP32 DevKitC | Development board for ESP32 | Rev 1 |   |

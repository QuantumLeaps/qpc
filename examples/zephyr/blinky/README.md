# Blinky example

# Overview

Blinks an on-board LED once per second. The blinking is performed
 by an Active Object (Blinky) with a state machine.



# Building and Running (Linux)

Open a terminal in the directory of this example (`qpc/examples/zephry/blinky`).

Depending on your Zephyr installation, you might need to shell the script
to configure Zephyr environment:

```
source ~/zephyrproject/zephyr/zephyr-env.sh
```

To build the example, type:
```
west build -b <board>
```
where `<board>` is one of the
[boards supported by Zephyr](https://docs.zephyrproject.org/latest/boards/index.html).
The `dpp` example has been tested with the following boards:

```
nucleo_l152re
nucleo_h743zi
```

To flush the board, type:
```
west flash
```

# Sample Output
The application also produces ASCII output to the serial terminal
(if suppoted by the board):

```
*** Booting Zephyr OS build v2.6.0-rc2-88-g3d39f72a88b3  ***
BSP_ledOff
QF_onStartup
BSP_ledOn
BSP_ledOff
BSP_ledOn
BSP_ledOff
```


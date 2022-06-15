Blinky example
###################

Overview
********
Blinks an on-board LED once per second.

The blinking is done by an Active Object (Blinky) wiht a state
machine.



Building and Running
********************
Linux:
Open a terminal in the blinky directory.

type:
source ~/zephyrproject/zephyr/zephyr-env.sh

type:
west build -b <board>

type:
west flush


Sample Output
=============
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


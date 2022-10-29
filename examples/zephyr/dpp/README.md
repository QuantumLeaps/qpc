# DPP example

# Overview
Dining Philosophers Problem (DPP) example with multiple
active objects.


# Building and Running (Linux)
Open a terminal in the directory of this example (`qpc/examples/zephry/dpp`).

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
The `dpp` example has been tested with the following boards.

Specific example for the `nucleo_h743zi` board:

```
west build -b nucleo_h743zi
```

## QP/Spy Build Configuration
The [QP/Spy software tracing](https://www.state-machine.com/qtools/qpspy.html)
is supported in this example and can be enabled by providing the command-line
option `-DQSPY=ON` to the build process:

Specific example
```
west build -b nucleo_h743zi -- -DQSPY=ON
```

> NOTE: The QP/Spy software tracing uses the Zephyr's console UART.
This means that the Zephyr `printk()` facility cannot be used while
QP/Spy is configured.


# Flashing the Board
To flush the board, type:
```
west flush
```


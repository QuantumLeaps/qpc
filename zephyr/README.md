# qpc Zephyr Module
This directory defines the
[QP/C Real-Time Embedded Framework](https://github.com/QuantumLeaps/qpc)
as a [Zephyr module](https://docs.zephyrproject.org/latest/develop/modules.html).

# How to use
Example of use is provided in the related repository
[qpc-zephyr-app](https://github.com/QuantumLeaps/qpc-zephyr-app)


## Configuring the QPC Zephyr Module
The `Kconfig` file provides configuration `QPC` to activate the QPC module
in Zephyr. To do so, you need to add the following line to your `prj.conf`:

```ini
CONFIG_QPC=y
```

## Configuring the QSPY Software Tracing
If you wish to enable
[QSPY Software Tracing](https://www.state-machine.com/qtools/qpspy.html),
you need to add also the following line to your `prj.conf`:

```ini
CONFIG_QSPY=y
```

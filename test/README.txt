The `qpc/test/` directory is planned to contain a growing number of system-level tests, which are based on QUTest, but *without* the QP-stub. The tests take advantage of the new QUTest configuration, where the QP-stub is NOT included (because the actual QP framework is linked). This configuration is activated by defining macro `Q_UTEST=0`.

@attention
Many tests provided in the `qpc/test/` directory run only on embedded targets and cannot run on the host machine.

This directory contains tests of the QK kernel.
These are SYSTEM tests, where QP is not stubbed-out, but
rather the actual QP code is used instead.

NOTE:
The tests run only on embedded targets and do NOT
run on the host computer.

NOTE:
To avoid stubbing-out the QP framework, the tests
define Q_UTEST=0



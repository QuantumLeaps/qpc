This QP port to POSIX with a single thread executing all active
objects (like the cooperative QV kernel).

If you are interested in using a POSIX target for deployment,
consider the following QP port:

- posix  multithreaded (P-threads) QP port to POSIX


If you are interested in testing your embedded QP applications
on a POSIX host, consider the following QP port:
 
- posix-qutest  for running QUTest unit testing harness


NOTE:
Building of the QP libraries on the POSIX targets or hosts
is no longer necessary. The example projects for POSIX are
built directly from QP source files and don't need a library.

Quantum Leaps
04/05/2018
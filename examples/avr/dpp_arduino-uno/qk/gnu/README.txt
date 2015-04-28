About this Example
==================
This example can be built from the command prompt with the provided
Makefile. The example can also be invoked directly (via the make
utility) from the QM modeling tool.


The Makefile
============
The provided Makefile should be easy to adapt for your own projects.
It contains three build configurations: Debug (default), Release, and
Spy.

The various build configurations are built as follows:

make
make CONF=rel
make CONF=spy

make clean
make CONF=rel clean
make CONF=spy clean


Warning: '__progmem__' attribute ignored
========================================
AVR is a Harvard architecture that uses different program and data spaces as
well as different CPU instructions to access these two address spaces (data
memory is 8-bit wide while program memory is 16-bit wide). Consequently, one
of the biggest concerns in AVR programs is to correctly allocate and access
constant data in the program space (ROM) rather than the precious RAM. Since
the ROM resides in a different address space, you need to tell the compiler
to place variables there. You also need a way to access the data (i.e., the
compiler has to use the lpm instruction.)

The GNU-AVR compiler supports data in the program memory only partially
through the attribute((progmem)) extension (typically used through the
PROGMEM macro). By tagging a variable with attribute((progmem)), you can
force it to reside in the ROM. However, the compiler support ends at this
point. It is the responsibility of the developer to correctly access the
PROGMEM variables. The GCC-AVR provides merely a bunch of macros (all
defined in the avr\pgmspace.h header file).

The bottom line is that the GNU-AVR compiler support for ROM-allocated
objects through the PROGMEM attribute is both incomplete and inconsistent
with the correct implementation in other compilers (such as IAR-AVR).
For example, pointers pointing to PROGMEM objects are not treated correctly,
meaning that the compiler will not generate the right code to access such
ROM objects (the GNU-AVR compiler needs a kludge in form of macros such as
pgm_read_byte_near and others). To recognize that fact, the GNU-AVR
generates the warnings "'progmem' attribute ignored ".

However, the QP source code is written for a standard correct implementation
(how else would you achieve portability?). For example, the IAR-AVR compiler
uses the attribute '__flash' to designate objects and pointers to ROM
objects. The compiler then generates the right code for all such flash
objects and pointers, without any kludges necessary for the GNU compiler.

In summary, the warnings "'progmem' attribute ignored" should be ignored
in the QP library build. To avoid these warnings, the QP source code would
have to be modified in such a way that it would be incorrect for other
compilers that handle these declarations correctly.

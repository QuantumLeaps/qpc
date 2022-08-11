QUTest Integration Testing of the DPP Application
=================================================
The tests included in the qutest/dpp/test_dpp directory perform integration
testing of the DPP application. The difference from pure unit testing is
that multiple active objects (AOs) are started and the tests exercise
interactions among these active objects.

The Test Fixture
----------------
The QUTest test fixture in this case is built from the same components as
the final application (except that it is linked with the "QUTest stub" of
the QP framework, instead of the fully-functional version).

Specifically, the test fixture in this case uses the same main() function
(the same main.c module, even) as the final DPP application.

The Test Scripts
----------------
The test scripts (*.py files for testing with Python and *.tcl files for
testing with Tcl) demonstrate injecting events and clock ticks to drive
the application.


The Test Targets
================
The makefiles provided with this test support several targets. Each
supported target system requires building a separate QUTest fixture,
but all targets can be tested with the *same* test scripts.

The following sub-sections describe how to build the code and run
the tests.

Host Executable
---------------
The provided Makefile builds the test fixture for the host executable,
which can be Windows, Linux or MacOSX.

Before running the Makefile, the QSPY host utility needs to be stated
from command-prompt/terminal as follows:

qspy -u -t

Next, you need to open another command-prompt/terminal, change to this
directory (qutest/dpp/test_dpp), and run:

make

The provided Makefile both builds the fixture and run the test
scripts. By default, the Python test scripts are used. If you wish
to run Tcl test scripts instead, please type:

make SCRIPT=tcl

Embedded Target: EFM32
----------------------
The provided makefile make_efm32 builds the test fixture for the
EFM32-SLSTK3401A board (ARM Cortex-M4). The makefile works on Windows hosts
only.

Before running make -f make_efm32, the QSPY host utility needs to
be stated from command-prompt/terminal as follows:

qspy -u -c COM2

where you need to adjust the COM port number to the virtual COM port
number assigned to the EFM32-SLSTK3401A board on your system.

Next, you need to open another command-prompt/terminal, change to this
directory (qutest/dpp/test_dpp), and run:

make -f make_efm32

The provided Makefile both builds the fixture and run the test
scripts. By default, the Python test scripts are used. If you wish
to run Tcl test scripts instead, please type:

make -f make_efm32 SCRIPT=tcl


Embedded Target: TM4C123
------------------------
The provided makefile make_tm4c123 builds the test fixture for the
EK-TM4C123GXL board (ARM Cortex-M4). The makefile works on Windows hosts
only.

Before running make -f make_efm32, the QSPY host utility needs to
be stated from command-prompt/terminal as follows:

qspy -u -c COM3

where you need to adjust the COM port number to the virtual COM port
number assigned to the EK-TM4C123GXL TivaC LaunchPad board on your system.

Next, you need to open another command-prompt/terminal, change to this
directory (qutest/dpp/test_dpp), and run:

make -f make_efm32

The provided Makefile both builds the fixture and run the test scripts.
By default, the Python test scripts are used. Alternatively, if you wish
to run Tcl test scripts instead, please type:

make -f make_efm32 SCRIPT=tcl


Embedded Target: MSP430
------------------------
The provided CCS (Code Composer Studio) project files
.project/.csproject/.ccsproject build the test fixture for the
MSP-EXP430F5529LP LauchPad board (MSP430X CPU).

NOTE: Currently testing on the MSP430 target is NOT fully automated,
because it is based on the CCS IDE. This requires building and loading
the test fixture *manually* from CCS, and only then by a separate step
of running the test scripts.

After building the fixture and loading it into the flash memory
of the MSP-EXP430F5529LP LauchPad board, you need to open the QSPY host
utility from command-prompt/terminal as follows:

qspy -u -c COM4 -b9600

where you need to adjust the COM port number to the virtual COM port
number assigned to the MSP-EXP430F5529LP LauchPad board on your system.

Next, you need to open another command-prompt/terminal, change to this
directory (qutest/dpp/test_dpp), and run:

python %QTOOLS%\qspy\py\qutest.py

This will run the test scripts in Python. Alternatively, if you wish
to run Tcl test scripts instead, please type:

tclsh %QTOOLS%\qspy\tcl\qutest.tcl


Contact Information
===================
https://www.state-machine.com
mailto:info@state-machine.com
 
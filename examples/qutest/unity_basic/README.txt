About This Example
==================
This example is based on "example_1" from the Unity unit testing framework.
The purpose is to illustrate testing of the same code (src/ProductionCode.c)
with Unity and with QUTest.

*** NOTE ***
The provided Makefiles for building and running the tests are cross-platform
and will work on Windows as well as POSIX workstations (Linux/MacOS). Also,
it is assumed that you have installed the QTools collection on your computer
and that you have added the QTools/bin directory to your PATH, see:
   https://www.state-machine.com/qtools


Directories and Files
---------------------

qpc/examples/qutest/unity_basic/ - this example
|
+-src/               - code under test (CUT)
| +-ProductionCode.h
| +-ProductionCode.c
|
+-test/              - testing with QUTest
| +-conftest.py      - configuration file for Python
| +-Makefile         - makefile for building and running the tests
| +-make_ef32        - makefile for building and running the tests EFM32 target
| +-make_tm4c123     - makefile for building and running the tests TM4C123 target
| +-test_ProductionCode.c   - QUTest test fixture in C
| +-test_ProductionCode.py  - QUTest test script in Python
|
+-test_unity/        - testing with Unity
| +-Makefile         - makefile for building and running the tests
| +-TestProductionCode.c        - Unity test fixture
| +-TestProductionCode_Runner.c - Unity test runner
|


Building the Code and Testing with Unity
----------------------------------------
Open a terminal/command-prompt. Change directory to test_unity
(qpc/examples/qutest/unity_basic/test_unity).

Type "make". The provided Makefile will build the code and run
the tests using the Unity framework.

*** NOTE ***
For Windows, the make utility, the C compiler (MinGW) are included
in the QTools collection. For all platforms, you will also need
the Unity source code, which is also included in the QTools collection
and the provided Makefile takes it from there.

*** NOTE ***
Some of the Unity tests fail, which is intentional to demonstrate
various failure modes.


Building the Code and Testing with QUTest
-----------------------------------------
Open terminal / command-prompt and launch the QSPY host utility
by typing:

qspy

Open *another* terminal / command-prompt and change directory to
test (qpc/examples/qutest/unity_example1/test).
Type:

make

to build the code with the provided Makefile and run the tests
using the Python test script (test_ProductionCode.py)

*** NOTE ***
For Windows, the Python interpreter is included in the QTools collection.

*** NOTE ***
Some of the QUTest tests fail, which is intentional to demonstrate
various failure modes, exactly as it is in the case of Unity tests.


Contact Information
-------------------
https://www.state-machine.com
mailto:info@state-machine.com


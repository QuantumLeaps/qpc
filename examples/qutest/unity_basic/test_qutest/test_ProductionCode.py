# QUTEST test script corresponding to the test_ProductionCode.c test fixture.
# see https://www.state-machine.com/qtools/qutest.html

import sys
import pytest
import struct
from qspypy.qspy import *

# preambe...
def on_setup(qutest):
    # This is run before EACH TEST
    qutest.current_obj(QS_OBJ_KIND.AP, 'Counter')
    qutest.poke(0, 4, struct.pack('<L', 0x5A5A))


# tests...
def test_FindFunction_WhichIsBroken_ShouldReturnZeroIfItemIsNotInList_WhichWorksEvenInOurBrokenCode(qutest):
    # All of these should pass
    qutest.command(0, 78)
    qutest.expect("0000000001 USER+000 FindFunction_WhichIsBroken 0 78")
    qutest.expect("0000000002 Trg-Done QS_RX_COMMAND")
    qutest.command(0, 1)
    qutest.expect("0000000003 USER+000 FindFunction_WhichIsBroken 0 1")
    qutest.expect("0000000004 Trg-Done QS_RX_COMMAND")
    qutest.command(0, 33)
    qutest.expect("%timestamp USER+000 FindFunction_WhichIsBroken 0 33")
    qutest.expect("%timestamp Trg-Done QS_RX_COMMAND")
    qutest.command(0, 999)
    qutest.expect("%timestamp USER+000 FindFunction_WhichIsBroken 0 999")
    qutest.expect("%timestamp Trg-Done QS_RX_COMMAND")
    qutest.command(0, (-1) & 0xFFFFFFFF)
    qutest.expect("%timestamp USER+000 FindFunction_WhichIsBroken 0 -1")
    qutest.expect("%timestamp Trg-Done QS_RX_COMMAND")


def test_FindFunction_WhichIsBroken_ShouldReturnTheIndexForItemsInList_WhichWillFailBecauseOurFunctionUnderTestIsBroken(qutest):
    qutest.command(0, 34)
    qutest.expect("%timestamp USER+000 FindFunction_WhichIsBroken 1 34")
    qutest.expect("%timestamp Trg-Done QS_RX_COMMAND")
    # Notice the rest of these didn't get a chance to run because the line above failed.
    # Unit tests abort each test function on the first sign of trouble.
    # Then NEXT test function runs as normal.
    qutest.command(0, 8888)
    qutest.expect("%timestamp USER+000 FindFunction_WhichIsBroken 8 8888")
    qutest.expect("%timestamp Trg-Done QS_RX_COMMAND")


def test_FunctionWhichReturnsLocalVariable_ShouldReturnTheCurrentCounterValue(qutest):
    qutest.command(1)
    qutest.expect("%timestamp USER+001 FunctionWhichReturnsLocalVariable 0x5A5A")
    qutest.expect("%timestamp Trg-Done QS_RX_COMMAND")
    # This should be true because we can still change our answer
    qutest.poke(0, 4, struct.pack('<L', 0x1234))
    qutest.command(1)
    qutest.expect("%timestamp USER+001 FunctionWhichReturnsLocalVariable 0x1234")
    qutest.expect("%timestamp Trg-Done QS_RX_COMMAND")


def test_FunctionWhichReturnsLocalVariable_ShouldReturnTheCurrentCounterValueAgain(qutest):
    # This should be true again because setup was rerun before this test (and after we changed it to 0x1234)
    qutest.command(1)
    qutest.expect("%timestamp USER+001 FunctionWhichReturnsLocalVariable 0x5A5A")
    qutest.expect("%timestamp Trg-Done QS_RX_COMMAND")


def test_FunctionWhichReturnsLocalVariable_ShouldReturnCurrentCounter_ButFailsBecauseThisTestIsActuallyFlawed(qutest):
    # Sometimes you get the test wrong.  When that happens, you get a failure too... and a quick look should tell
    # you what actually happened...which in this case was a failure to setup the initial condition.
    qutest.command(1)
    qutest.expect("%timestamp USER+001 FunctionWhichReturnsLocalVariable 0x1234")
    qutest.expect("%timestamp Trg-Done QS_RX_COMMAND")


# end...
if __name__ == "__main__":
    options = ['-x', '-v', '--tb=short']
    options.extend(sys.argv)
    pytest.main(options)

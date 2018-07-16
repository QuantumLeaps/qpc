# QUTEST test script corresponding to the test_LedDriver.c test fixture.
# see https://www.state-machine.com/qtools/qutest.html
#
# This example corresponds to LedDriver.c from Chapters 3 & 4 "Starting
# a C Module" & "Testing Your Way to Done" of the book: "Test-Driven
# Development for Embedded Systems" by James W. Grenning

import sys
import pytest
import struct
from qspypy.qspy import FILTER, QS_OBJ_KIND


def test_LEDs_off_after_Create(qutest):
    qutest.command("LEDDRIVER_CREATE")
    qutest.expect("%timestamp LEDDRIVER_CREATE 0x0000")
    qutest.expect("%timestamp Trg-Done QS_RX_COMMAND")

def test_Turn_on_LED_1(qutest_noreset):
    qutest = qutest_noreset # name change
    qutest.command("LEDDRIVER_TURNON",1)
    qutest.expect("%timestamp LEDDRIVER_TURNON 0x0001")
    qutest.expect("%timestamp Trg-Done QS_RX_COMMAND")

def test_Turn_off_LED_1(qutest_noreset):
    qutest = qutest_noreset # name change
    qutest.command("LEDDRIVER_TURNOFF",1)
    qutest.expect("%timestamp LEDDRIVER_TURNOFF 0x0000")
    qutest.expect("%timestamp Trg-Done QS_RX_COMMAND")

def test_Turn_on_multiple_LEDs(qutest_noreset):
    qutest = qutest_noreset # name change
    qutest.command("LEDDRIVER_TURNON",9)
    qutest.expect("%timestamp LEDDRIVER_TURNON 0x0100")
    qutest.expect("%timestamp Trg-Done QS_RX_COMMAND")
    qutest.command("LEDDRIVER_TURNON",8)
    qutest.expect("%timestamp LEDDRIVER_TURNON 0x0180")
    qutest.expect("%timestamp Trg-Done QS_RX_COMMAND")

def test_Turn_off_multiple_LEDs(qutest_noreset):
    qutest = qutest_noreset # name change
    qutest.command("LEDDRIVER_TURNALLON")
    qutest.expect("%timestamp LEDDRIVER_TURNALLON 0xFFFF")
    qutest.expect("%timestamp Trg-Done QS_RX_COMMAND")
    qutest.command("LEDDRIVER_TURNOFF",9)
    qutest.expect("%timestamp LEDDRIVER_TURNOFF 0xFEFF")
    qutest.expect("%timestamp Trg-Done QS_RX_COMMAND")
    qutest.command("LEDDRIVER_TURNOFF",8)
    qutest.expect("%timestamp LEDDRIVER_TURNOFF 0xFE7F")
    qutest.expect("%timestamp Trg-Done QS_RX_COMMAND")

def test_Out_of_bounds_produces_Runtime_Error(qutest_noreset):
    qutest = qutest_noreset # name change
    qutest.command("LEDDRIVER_TURNON", (-1 & 0xFFFFFFFF))
    qutest.expect("%timestamp =ASSERT= Mod=LedDriver,Loc=100")

def test_Turn_off_any_LED(qutest):
    qutest.command("LEDDRIVER_TURNALLON")
    qutest.expect("%timestamp LEDDRIVER_TURNALLON 0xFFFF")
    qutest.expect("%timestamp Trg-Done QS_RX_COMMAND")
    qutest.command("LEDDRIVER_TURNOFF",8)
    qutest.expect("%timestamp LEDDRIVER_TURNOFF 0xFF7F")
    qutest.expect("%timestamp Trg-Done QS_RX_COMMAND")

def test_LED_memory_is_not_readable(qutest_noreset):
    qutest = qutest_noreset # name change
    qutest.current_obj(QS_OBJ_KIND.AP,"virtualLeds")
    qutest.poke(0,2,struct.pack('<H',0xFFFF))
    qutest.peek(0,2,1)
    qutest.expect("%timestamp Trg-Peek Offs=0,Size=2,Num=1,Data=<FFFF>")
    qutest.command("LEDDRIVER_TURNON",8)
    qutest.expect("%timestamp LEDDRIVER_TURNON 0x0080")
    qutest.expect("%timestamp Trg-Done QS_RX_COMMAND")



if __name__ == "__main__":
    options = ['-x', '-v', '--tb=short']
    options.extend(sys.argv)
    pytest.main(options)

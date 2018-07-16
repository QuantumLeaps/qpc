# QUTEST test script corresponding to the test_Sprintf.c test fixture.
# see https://www.state-machine.com/qtools/qutest.html
#
# This example corresponds to SprintfTest.c from Chapter 2 "Test-Driving
# Tools and Conventions" of the book: "Test-Driven Development for Embedded
# Systems" by James W. Grenning

import sys
import pytest
import struct
from qspypy.qspy import FILTER, QS_OBJ_KIND

def test_No_format_operations(qutest):
    qutest.glb_filter(FILTER.UA)
    qutest.current_obj(QS_OBJ_KIND.AP,"format")
    qutest.poke(0,1,bytes("Hello World!\0",'ascii'))
    qutest.command(0)
    qutest.expect("0000000001 SPRINTF_CALL 12 Hello World!")
    qutest.expect("0000000002 Trg-Done QS_RX_COMMAND")

def test_Insert_decimal(qutest_noreset):
    qutest = qutest_noreset # name change
    qutest.current_obj(QS_OBJ_KIND.AP,"format")
    qutest.poke(0,1,bytes("dec=%d\0",'ascii'))
    qutest.command(0,1234)
    qutest.expect("0000000001 SPRINTF_CALL 8 dec=1234")
    qutest.expect("0000000002 Trg-Done QS_RX_COMMAND")

def test_Insert_decimal_and_string(qutest_noreset):
    qutest = qutest_noreset # name change
    qutest.current_obj(QS_OBJ_KIND.AP,"format")
    qutest.poke(0,1,bytes("dec=%d, str=%s\0",'ascii'))
    qutest.current_obj(QS_OBJ_KIND.AP,"string")
    qutest.poke(0,1,bytes("Hello\0",'ascii'))
    qutest.current_obj(QS_OBJ_KIND.AP,"output")
    qutest.fill(0,1,100,0xAA)
    qutest.command(0,4321)
    qutest.expect("0000000001 SPRINTF_CALL 19 dec=4321, str=Hello")
    qutest.expect("0000000002 Trg-Done QS_RX_COMMAND")

def test_Overflow_underflow(qutest_noreset):
    qutest = qutest_noreset # name change
    qutest.current_obj(QS_OBJ_KIND.AP,"output")
    qutest.peek(26,1,4)
    qutest.expect("%timestamp Trg-Peek Offs=26,Size=1,Num=4,Data=<6F,00,AA,AA>")
    qutest.peek(0,1,10)
    qutest.expect("%timestamp Trg-Peek Offs=0,Size=1,Num=10,Data=<AA,AA,AA,AA,AA,AA,AA,AA,*")


if __name__ == "__main__":
    options = ['-x', '-v', '--tb=short']
    options.extend(sys.argv)
    pytest.main(options)

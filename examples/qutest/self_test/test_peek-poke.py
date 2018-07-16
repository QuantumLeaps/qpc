# test-script for QUTest unit testing harness
# see https://www.state-machine.com/qtools/qutest.html

import sys
import pytest
import struct
from qspypy.qspy import FILTER, QS_OBJ_KIND


def on_setup(qutest):
    qutest.expect("%timestamp ON_TEST_SETUP")

def on_teardown(qutest):
    qutest.expect("%timestamp ON_TEST_TEARDOWN")



def test_Peek_Poke_Fill_uint8_t(qutest):
    qutest.current_obj(QS_OBJ_KIND.AP,"buffer")
    qutest.fill(0,1,100,0x1A)
    qutest.peek(0,1,5)
    qutest.expect("%timestamp Trg-Peek Offs=0,Size=1,Num=5,Data=<1A,1A,1A,1A,1A>")
    qutest.peek(95,1,5)
    qutest.expect("%timestamp Trg-Peek Offs=95,Size=1,Num=5,Data=<1A,1A,1A,1A,1A>")
    qutest.fill(2,1,95,0x1B)
    qutest.peek(0,1,5)
    qutest.expect("%timestamp Trg-Peek Offs=0,Size=1,Num=5,Data=<1A,1A,1B,1B,1B>")
    qutest.peek(95,1,5)
    qutest.expect("%timestamp Trg-Peek Offs=95,Size=1,Num=5,Data=<1B,1B,1A,1A,1A>")
    qutest.fill(0,1,100,0xA1)
    qutest.poke(2,1,struct.pack('<BBBB', 0xB1,0xC1,0xD1,0xE1))
    qutest.peek(0,1,7)
    qutest.expect("%timestamp Trg-Peek Offs=0,Size=1,Num=7,Data=<A1,A1,B1,C1,D1,E1,A1>")

def test_Peek_Poke_Fill_uint16_t(qutest_noreset):
    qutest = qutest_noreset # name change
    qutest.fill(0,2,50,0x2A2B)
    qutest.peek(0,2,3)
    qutest.expect("%timestamp Trg-Peek Offs=0,Size=2,Num=3,Data=<2A2B,2A2B,2A2B>")
    qutest.peek(94,2,3)
    qutest.expect("%timestamp Trg-Peek Offs=94,Size=2,Num=3,Data=<2A2B,2A2B,2A2B>")
    qutest.fill(2,2,48,0x2C2D)
    qutest.peek(0,2,3)
    qutest.expect("%timestamp Trg-Peek Offs=0,Size=2,Num=3,Data=<2A2B,2C2D,2C2D>")
    qutest.peek(94,2,3)
    qutest.expect("%timestamp Trg-Peek Offs=94,Size=2,Num=3,Data=<2C2D,2C2D,2A2B>")
    qutest.fill(0,2,50,0xA2B2)
    qutest.poke(2,2,struct.pack('<HH',0xB2C2,0xD2E2))
    qutest.peek(0,2,4)
    qutest.expect("%timestamp Trg-Peek Offs=0,Size=2,Num=4,Data=<A2B2,B2C2,D2E2,A2B2>")

def test_Peek_Poke_Fill_uint32_t(qutest_noreset):
    qutest = qutest_noreset # name change
    qutest.fill(0,4,25,0x4A4B4C4D)
    qutest.peek(0,4,3)
    qutest.expect("%timestamp Trg-Peek Offs=0,Size=4,Num=3,Data=<4A4B4C4D,4A4B4C4D,4A4B4C4D>")
    qutest.peek(88,4,3)
    qutest.expect("%timestamp Trg-Peek Offs=88,Size=4,Num=3,Data=<4A4B4C4D,4A4B4C4D,4A4B4C4D>")
    qutest.fill(4,4,23,0x4C4D4E4F)
    qutest.peek(0,4,3)
    qutest.expect("%timestamp Trg-Peek Offs=0,Size=4,Num=3,Data=<4A4B4C4D,4C4D4E4F,4C4D4E4F>")
    qutest.peek(88,4,3)
    qutest.expect("%timestamp Trg-Peek Offs=88,Size=4,Num=3,Data=<4C4D4E4F,4C4D4E4F,4A4B4C4D>")
    qutest.fill(0,4,25,0xA4B4C4D4)
    qutest.poke(4,4,struct.pack('<II',0xB4C4D4E4,0xB5C5D5E5))
    qutest.peek(0,4,4)
    qutest.expect("%timestamp Trg-Peek Offs=0,Size=4,Num=4,Data=<A4B4C4D4,B4C4D4E4,B5C5D5E5,A4B4C4D4>")


if __name__ == "__main__":
    options = ['-x', '-v', '--tb=short']
    options.extend(sys.argv)
    pytest.main(options)

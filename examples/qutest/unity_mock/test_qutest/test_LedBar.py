# QUTEST test script corresponding to the test_LedBar.c test fixture.
# see https://www.state-machine.com/qtools/qutest.html

import sys
import pytest
import struct
from qspypy.qspy import *

# preambe...

# tests...

def test_LedBar_0_percent_all_off(qutest):
    qutest.command(0, 0)
    qutest.expect("%timestamp LED_MOD Led_off 0")
    qutest.expect("%timestamp LED_MOD Led_off 1")
    qutest.expect("%timestamp LED_MOD Led_off 2")
    qutest.expect("%timestamp LED_MOD Led_off 3")
    qutest.expect("%timestamp LED_MOD Led_off 4")
    qutest.expect("%timestamp USER+000 LedBar_setPercent 0 0")
    qutest.expect("%timestamp Trg-Done QS_RX_COMMAND")

def test_LedBar_100_percent_all_on(qutest_noreset):
    qutest = qutest_noreset # name change
    qutest.command(0, 100)
    qutest.expect("%timestamp LED_MOD Led_on 10 0")
    qutest.expect("%timestamp LED_MOD Led_on 20 1")
    qutest.expect("%timestamp LED_MOD Led_on 10 2")
    qutest.expect("%timestamp LED_MOD Led_on 20 3")
    qutest.expect("%timestamp LED_MOD Led_on 10 4")
    qutest.expect("%timestamp USER+000 LedBar_setPercent 70 100")
    qutest.expect("%timestamp Trg-Done QS_RX_COMMAND")

def test_LedBar_19_percent_all_off(qutest_noreset):
    qutest = qutest_noreset # name change
    qutest.command(0, 19)
    qutest.expect("%timestamp LED_MOD Led_off 0")
    qutest.expect("%timestamp LED_MOD Led_off 1")
    qutest.expect("%timestamp LED_MOD Led_off 2")
    qutest.expect("%timestamp LED_MOD Led_off 3")
    qutest.expect("%timestamp LED_MOD Led_off 4")
    qutest.expect("%timestamp USER+000 LedBar_setPercent 0 19")
    qutest.expect("%timestamp Trg-Done QS_RX_COMMAND")

def test_LedBar_20_percent_one_on(qutest_noreset):
    qutest = qutest_noreset # name change
    qutest.command(0, 20)
    qutest.expect("%timestamp LED_MOD Led_on 10 0")
    qutest.expect("%timestamp LED_MOD Led_off 1")
    qutest.expect("%timestamp LED_MOD Led_off 2")
    qutest.expect("%timestamp LED_MOD Led_off 3")
    qutest.expect("%timestamp LED_MOD Led_off 4")
    qutest.expect("%timestamp USER+000 LedBar_setPercent 10 20")
    qutest.expect("%timestamp Trg-Done QS_RX_COMMAND")

def test_LedBar_50_percent_two_on(qutest_noreset):
    qutest = qutest_noreset # name change
    qutest.current_obj(QS_OBJ_KIND.AP, 'led_power')
    qutest.poke(0, 4, struct.pack('<LL', 25, 15))
    qutest.command(0, 50)
    qutest.expect("%timestamp LED_MOD Led_on 25 0")
    qutest.expect("%timestamp LED_MOD Led_on 15 1")
    qutest.expect("%timestamp LED_MOD Led_off 2")
    qutest.expect("%timestamp LED_MOD Led_off 3")
    qutest.expect("%timestamp LED_MOD Led_off 4")
    qutest.expect("%timestamp USER+000 LedBar_setPercent 40 50")
    qutest.expect("%timestamp Trg-Done QS_RX_COMMAND")

def test_LedBar_99_percent_four_on(qutest_noreset):
    qutest = qutest_noreset # name change
    qutest.probe('Led_on', 17)
    qutest.probe('Led_on', 13)
    qutest.command(0, 99)
    qutest.expect("%timestamp TstProbe Fun=Led_on,Data=17")
    qutest.expect("%timestamp LED_MOD Led_on 17 0")
    qutest.expect("%timestamp TstProbe Fun=Led_on,Data=13")
    qutest.expect("%timestamp LED_MOD Led_on 13 1")
    qutest.expect("%timestamp LED_MOD Led_on 10 2")
    qutest.expect("%timestamp LED_MOD Led_on 20 3")
    qutest.expect("%timestamp LED_MOD Led_off 4")
    qutest.expect("%timestamp USER+000 LedBar_setPercent 60 99")
    qutest.expect("%timestamp Trg-Done QS_RX_COMMAND")


# end...
if __name__ == "__main__":
    options = ['-x', '-v', '--tb=short']
    options.extend(sys.argv)
    pytest.main(options)

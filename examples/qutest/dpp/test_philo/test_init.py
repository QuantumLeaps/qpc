# test-script for QUTest unit testing harness
# see https://www.state-machine.com/qtools/qutest.html

import sys
import pytest
import struct
from qspypy.qspy import *

# preamble...
def on_reset(qutest):
    qutest.expect_pause()
    qutest.glb_filter(FILTER.ON)
    qutest.Continue()  # note continue in lower case. is a reserved word in python
    qutest.expect("%timestamp AO-Subsc Obj=AO_Philo<2>,Sig=EAT_SIG")
    qutest.expect("%timestamp AO-Subsc Obj=AO_Philo<2>,Sig=TEST_SIG")
    qutest.expect("===RTC===> St-Init  Obj=AO_Philo<2>,State=QHsm_top->Philo_thinking")
    qutest.expect("%timestamp BSP_CALL BSP_random *")
    qutest.expect("%timestamp TE0-Arm  Obj=l_philo<2>.timeEvt,AO=AO_Philo<2>,Tim=*,Int=0")
    qutest.expect("===RTC===> St-Entry Obj=AO_Philo<2>,State=Philo_thinking")
    qutest.expect("%timestamp Init===> Obj=AO_Philo<2>,State=Philo_thinking")

# tests...
def test_init(qutest):
    pass # empty (runs only on_reset)

# end...
if __name__ == "__main__":
    # stop on first failure, verbose output but small stack trace
    options = ['-x', '-v', '--tb=short']
    options.extend(sys.argv)
    pytest.main(options)


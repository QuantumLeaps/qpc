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
    qutest.expect("===RTC===> St-Init  Obj=AO_Philo<0>,State=QHsm_top->NULL")
    qutest.expect("%timestamp AO-Subsc Obj=AO_Philo<0>,Sig=EAT_SIG")
    qutest.expect("===RTC===> St-Init  Obj=AO_Philo<1>,State=QHsm_top->NULL")
    qutest.expect("%timestamp AO-Subsc Obj=AO_Philo<1>,Sig=EAT_SIG")
    qutest.expect("===RTC===> St-Init  Obj=AO_Philo<2>,State=QHsm_top->NULL")
    qutest.expect("%timestamp AO-Subsc Obj=AO_Philo<2>,Sig=EAT_SIG")
    qutest.expect("===RTC===> St-Init  Obj=AO_Philo<3>,State=QHsm_top->NULL")
    qutest.expect("%timestamp AO-Subsc Obj=AO_Philo<3>,Sig=EAT_SIG")
    qutest.expect("===RTC===> St-Init  Obj=AO_Philo<4>,State=QHsm_top->NULL")
    qutest.expect("%timestamp AO-Subsc Obj=AO_Philo<4>,Sig=EAT_SIG")
    qutest.expect("%timestamp AO-Subsc Obj=AO_Table,Sig=DONE_SIG")
    qutest.expect("%timestamp AO-Subsc Obj=AO_Table,Sig=PAUSE_SIG")
    qutest.expect("%timestamp AO-Subsc Obj=AO_Table,Sig=SERVE_SIG")
    qutest.expect("%timestamp AO-Subsc Obj=AO_Table,Sig=TEST_SIG")
    qutest.expect("%timestamp BSP_CALL BSP_displayPhilStat 0 thinking")
    qutest.expect("%timestamp BSP_CALL BSP_displayPhilStat 1 thinking")
    qutest.expect("%timestamp BSP_CALL BSP_displayPhilStat 2 thinking")
    qutest.expect("%timestamp BSP_CALL BSP_displayPhilStat 3 thinking")
    qutest.expect("%timestamp BSP_CALL BSP_displayPhilStat 4 thinking")
    qutest.expect("===RTC===> St-Init  Obj=AO_Table,State=QHsm_top->Table_serving")
    qutest.expect("===RTC===> St-Entry Obj=AO_Table,State=Table_serving")
    qutest.expect("%timestamp Init===> Obj=AO_Table,State=Table_serving")

# tests...
def test_init(qutest):
    pass # empty (runs only on_reset)

# end...
if __name__ == "__main__":
    # stop on first failure, verbose output but small stack trace
    options = ['-x', '-v', '--tb=short']
    options.extend(sys.argv)
    pytest.main(options)


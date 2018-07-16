# test-script for QUTest unit testing harness
# see https://www.state-machine.com/qtools/qutest.html

import sys
import pytest
import struct
from qspypy.qspy import FILTER, QS_OBJ_KIND


def on_reset(qutest):
    qutest.expect_pause()
    qutest.glb_filter(FILTER.ON)
    qutest.Continue()
    qutest.expect("%timestamp TE0-Arm  Obj=l_blinky.timeEvt,AO=l_blinky,*")
    qutest.expect("===RTC===> St-Init  Obj=l_blinky,State=QHsm_top->Blinky_off")
    qutest.expect("%timestamp LED 0")
    qutest.expect("===RTC===> St-Entry Obj=l_blinky,State=Blinky_off")
    qutest.expect("%timestamp Init===> Obj=l_blinky,State=Blinky_off")
    qutest.current_obj(QS_OBJ_KIND.SM_AO,"l_blinky")


def test_TIMEOUT_SIG_l_blinky(qutest):
    qutest.post("TIMEOUT_SIG")
    qutest.expect("%timestamp QF-New   Sig=TIMEOUT_SIG,*")
    qutest.expect("%timestamp MP-Get   Obj=smlPoolSto,*")
    qutest.expect("%timestamp AO-Post  Sdr=QS_RX,Obj=l_blinky,Evt<Sig=TIMEOUT_SIG,*")
    qutest.expect("%timestamp AO-GetL  Obj=l_blinky,Evt<Sig=TIMEOUT_SIG,*")
    qutest.expect("%timestamp Disp===> Obj=l_blinky,Sig=TIMEOUT_SIG,State=Blinky_off")
    qutest.expect("%timestamp LED 1")
    qutest.expect("===RTC===> St-Entry Obj=l_blinky,State=Blinky_on")
    qutest.expect("%timestamp ===>Tran Obj=l_blinky,Sig=TIMEOUT_SIG,State=Blinky_off->Blinky_on")
    qutest.expect("%timestamp QF-gc    Evt<Sig=TIMEOUT_SIG,*")
    qutest.expect("%timestamp MP-Put   Obj=smlPoolSto,*")
    qutest.expect("%timestamp Trg-Done QS_RX_EVENT")

def test_timeEvt_Blinky_off_tick(qutest):
    qutest.current_obj(QS_OBJ_KIND.TE,"l_blinky.timeEvt")
    qutest.tick()
    qutest.expect("%timestamp TE0-Post Obj=l_blinky.timeEvt,Sig=TIMEOUT_SIG,AO=l_blinky")
    qutest.expect("%timestamp AO-Post  Sdr=QS_RX,Obj=l_blinky,Evt<Sig=TIMEOUT_SIG,*")
    qutest.expect("%timestamp AO-GetL  Obj=l_blinky,Evt<Sig=TIMEOUT_SIG,*")
    qutest.expect("%timestamp Disp===> Obj=l_blinky,Sig=TIMEOUT_SIG,State=Blinky_off")
    qutest.expect("%timestamp LED 1")
    qutest.expect("===RTC===> St-Entry Obj=l_blinky,State=Blinky_on")
    qutest.expect("%timestamp ===>Tran Obj=l_blinky,Sig=TIMEOUT_SIG,State=Blinky_off->Blinky_on")
    qutest.expect("%timestamp Trg-Done QS_RX_TICK")

def test_timeEvt_Blinky_on_tick(qutest_noreset):
    qutest = qutest_noreset # name change
    qutest.tick()
    qutest.expect("%timestamp TE0-Post Obj=l_blinky.timeEvt,Sig=TIMEOUT_SIG,AO=l_blinky")
    qutest.expect("%timestamp AO-Post  Sdr=QS_RX,Obj=l_blinky,Evt<Sig=TIMEOUT_SIG,*")
    qutest.expect("%timestamp AO-GetL  Obj=l_blinky,Evt<Sig=TIMEOUT_SIG,*")
    qutest.expect("%timestamp Disp===> Obj=l_blinky,Sig=TIMEOUT_SIG,State=Blinky_on")
    qutest.expect("%timestamp LED 0")
    qutest.expect("===RTC===> St-Entry Obj=l_blinky,State=Blinky_off")
    qutest.expect("%timestamp ===>Tran Obj=l_blinky,Sig=TIMEOUT_SIG,State=Blinky_on->Blinky_off")
    qutest.expect("%timestamp Trg-Done QS_RX_TICK")


if __name__ == "__main__":
    options = ['-x', '-v', '--tb=short']
    options.extend(sys.argv)
    pytest.main(options)

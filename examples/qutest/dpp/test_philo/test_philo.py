# test-script for QUTest unit testing harness
# see https://www.state-machine.com/qtools/qutest.html

import sys
import pytest
import struct
from qspypy.qspy import *

# preamble...
def on_reset(qutest):
    qutest.expect_pause()
    qutest.glb_filter(FILTER.SM)
    qutest.loc_filter(QS_OBJ_KIND.SM_AO, 'AO_Philo<2>')
    qutest.Continue()  # note continue in lower case. is a reserved word in python
    qutest.expect("===RTC===> St-Init  Obj=AO_Philo<2>,State=QHsm_top->Philo_thinking")
    qutest.expect("===RTC===> St-Entry Obj=AO_Philo<2>,State=Philo_thinking")
    qutest.expect("%timestamp Init===> Obj=AO_Philo<2>,State=Philo_thinking")
    qutest.glb_filter(FILTER.SM, FILTER.AO, FILTER.UA)
    qutest.current_obj(QS_OBJ_KIND.SM_AO, 'AO_Philo<2>')

# tests...
def test_TIMEOUT_Philo_post(qutest):
    qutest.post('TIMEOUT_SIG')
    qutest.expect("%timestamp AO-Post  Sdr=QS_RX,Obj=AO_Philo<2>,Evt<Sig=TIMEOUT_SIG*")
    qutest.expect("%timestamp AO-GetL  Obj=AO_Philo<2>,Evt<Sig=TIMEOUT_SIG,*")
    qutest.expect("%timestamp Disp===> Obj=AO_Philo<2>,Sig=TIMEOUT_SIG,State=Philo_thinking")
    qutest.expect("===RTC===> St-Exit  Obj=AO_Philo<2>,State=Philo_thinking")
    qutest.expect("===RTC===> St-Entry Obj=AO_Philo<2>,State=Philo_hungry")
    qutest.expect("%timestamp ===>Tran Obj=AO_Philo<2>,Sig=TIMEOUT_SIG,State=Philo_thinking->Philo_hungry")
    qutest.expect("%timestamp Trg-Done QS_RX_EVENT")

def test_publish_EAT_2(qutest_noreset):
    qutest = qutest_noreset # Rename for consistancy
    qutest.loc_filter(QS_OBJ_KIND.SM_AO, 'AO_Philo<2>')
    qutest.publish('EAT_SIG',  struct.pack('< B', 2)) # Send byte of value 2
    qutest.expect("%timestamp AO-Post  Sdr=QS_RX,Obj=AO_Philo<2>,Evt<Sig=EAT_SIG,*")
    qutest.expect("%timestamp QUTEST_ON_POST EAT_SIG,Obj=AO_Philo<2> 2")
    qutest.expect("%timestamp Trg-Done QS_RX_EVENT")
    qutest.expect("%timestamp AO-GetL  Obj=AO_Philo<2>,Evt<Sig=EAT_SIG,*")
    qutest.expect("%timestamp Disp===> Obj=AO_Philo<2>,Sig=EAT_SIG,State=Philo_hungry")
    qutest.expect("%timestamp BSP_CALL BSP_random *")
    qutest.expect("===RTC===> St-Entry Obj=AO_Philo<2>,State=Philo_eating")
    qutest.expect("%timestamp ===>Tran Obj=AO_Philo<2>,Sig=EAT_SIG,State=Philo_hungry->Philo_eating")
    qutest.expect("%timestamp Trg-Done QS_RX_EVENT")

def test_TIMEOUT_Philo_thinking_ASSERT(qutest):
    qutest.probe('QActive_post_', 1)
    qutest.dispatch('TIMEOUT_SIG')
    qutest.expect("%timestamp Disp===> Obj=AO_Philo<2>,Sig=TIMEOUT_SIG,State=Philo_thinking")
    qutest.expect("===RTC===> St-Exit  Obj=AO_Philo<2>,State=Philo_thinking")
    qutest.expect("%timestamp TstProbe Fun=QActive_post_,Data=1")
    qutest.expect("%timestamp =ASSERT= Mod=qf_actq,Loc=110")

def test_TIMEOUT_Philo_eating_PUBLISH_from_AO(qutest):
    qutest.glb_filter(FILTER.OFF)
    qutest.dispatch('TIMEOUT_SIG')
    qutest.expect("%timestamp Trg-Done QS_RX_EVENT")
    qutest.dispatch('EAT_SIG', struct.pack('< B', 2))
    qutest.expect("%timestamp Trg-Done QS_RX_EVENT")
    qutest.glb_filter(FILTER.SM, FILTER.AO, FILTER.QF)
    qutest.dispatch('TIMEOUT_SIG')
    qutest.expect("%timestamp QF-New   Sig=TIMEOUT_SIG,*")
    qutest.expect("%timestamp Disp===> Obj=AO_Philo<2>,Sig=TIMEOUT_SIG,State=Philo_eating")
    qutest.expect("%timestamp QF-New   Sig=DONE_SIG,*")
    qutest.expect("%timestamp QF-Pub   Sdr=AO_Philo<2>,Evt<Sig=DONE_SIG,Pool=1,Ref=0>")
    qutest.expect("%timestamp QF-gc?   Evt<Sig=DONE_SIG,*")
    qutest.expect("===RTC===> St-Exit  Obj=AO_Philo<2>,State=Philo_eating")
    qutest.expect("===RTC===> St-Entry Obj=AO_Philo<2>,State=Philo_thinking")
    qutest.expect("%timestamp ===>Tran Obj=AO_Philo<2>,Sig=TIMEOUT_SIG,State=Philo_eating->Philo_thinking")
    qutest.expect("%timestamp QF-gc?   Evt<Sig=TIMEOUT_SIG,*")
    qutest.expect("%timestamp Trg-Done QS_RX_EVENT")

def test_timeEvt_Philo_tick(qutest):
    qutest.glb_filter(FILTER.SM, FILTER.AO, FILTER.TE)
    qutest.current_obj(QS_OBJ_KIND.TE, 'l_philo<2>.timeEvt')
    qutest.tick()
    qutest.expect("           TE0-ADis Obj=l_philo<2>.timeEvt,AO=AO_Philo<2>")
    qutest.expect("%timestamp TE0-Post Obj=l_philo<2>.timeEvt,Sig=TIMEOUT_SIG,AO=AO_Philo<2>")
    qutest.expect("%timestamp AO-Post  Sdr=QS_RX,Obj=AO_Philo<2>,Evt<Sig=TIMEOUT_SIG*")
    qutest.expect("%timestamp AO-GetL  Obj=AO_Philo<2>,Evt<Sig=TIMEOUT_SIG*")
    qutest.expect("%timestamp Disp===> Obj=AO_Philo<2>,Sig=TIMEOUT_SIG,State=Philo_thinking")
    qutest.expect("%timestamp TE0-DisA Obj=l_philo<2>.timeEvt,AO=AO_Philo<2>")
    qutest.expect("===RTC===> St-Exit  Obj=AO_Philo<2>,State=Philo_thinking")
    qutest.expect("===RTC===> St-Entry Obj=AO_Philo<2>,State=Philo_hungry")
    qutest.expect("%timestamp ===>Tran Obj=AO_Philo<2>,Sig=TIMEOUT_SIG,State=Philo_thinking->Philo_hungry")
    qutest.expect("%timestamp Trg-Done QS_RX_TICK")

# end...
if __name__ == "__main__":
    # stop on first failure, verbose output but small stack trace
    options = ['-x', '-v', '--tb=short']
    options.extend(sys.argv)
    pytest.main(options)


# test-script for QUTest unit testing harness
# see https://www.state-machine.com/qtools/qutest.html

import sys
import pytest
import struct
from qspypy.qspy import *

# preamble...
def on_reset(qutest):
    qutest.expect_pause()
    qutest.Continue()  # note continue in lower case. is a reserved word in python

# tests...
def test_tick(qutest):
    qutest.glb_filter(FILTER.ON)
    qutest.current_obj(QS_OBJ_KIND.TE,'l_philo<2>.timeEvt')
    qutest.tick()
    qutest.expect("           Tick<0>  Ctr=*")
    qutest.expect("           TE0-ADis Obj=l_philo<2>.timeEvt,AO=AO_Philo<2>")
    qutest.expect("%timestamp TE0-Post Obj=l_philo<2>.timeEvt,Sig=TIMEOUT_SIG,AO=AO_Philo<2>")
    qutest.expect("%timestamp AO-Post  Sdr=QS_RX,Obj=AO_Philo<2>,Evt<Sig=TIMEOUT_SIG,Pool=0,Ref=0>,*")
    qutest.expect("%timestamp AO-GetL  Obj=AO_Philo<2>,Evt<Sig=TIMEOUT_SIG,Pool=0,Ref=0>")
    qutest.expect("%timestamp Disp===> Obj=AO_Philo<2>,Sig=TIMEOUT_SIG,State=Philo_thinking")
    qutest.expect("%timestamp TE0-DisA Obj=l_philo<2>.timeEvt,AO=AO_Philo<2>")
    qutest.expect("===RTC===> St-Exit  Obj=AO_Philo<2>,State=Philo_thinking")
    qutest.expect("%timestamp QF-New   Sig=HUNGRY_SIG,*")
    qutest.expect("%timestamp MP-Get   Obj=EvtPool1,*")
    qutest.expect("%timestamp AO-Post  Sdr=AO_Philo<2>,Obj=AO_Table,Evt<Sig=HUNGRY_SIG,Pool=1,Ref=1>,*")
    qutest.expect("%timestamp QUTEST_ON_POST HUNGRY_SIG,Obj=AO_Table 2")
    qutest.expect("===RTC===> St-Entry Obj=AO_Philo<2>,State=Philo_hungry")
    qutest.expect("%timestamp ===>Tran Obj=AO_Philo<2>,Sig=TIMEOUT_SIG,State=Philo_thinking->Philo_hungry")
    qutest.expect("%timestamp AO-GetL  Obj=AO_Table,Evt<Sig=HUNGRY_SIG,Pool=1,Ref=1>")
    qutest.expect("%timestamp Disp===> Obj=AO_Table,Sig=HUNGRY_SIG,State=Table_serving")
    qutest.expect("%timestamp BSP_CALL BSP_displayPhilStat 2 hungry  ")
    qutest.expect("%timestamp QF-New   Sig=EAT_SIG,*")
    qutest.expect("%timestamp MP-Get   Obj=EvtPool1,*")
    qutest.expect("%timestamp QF-Pub   Sdr=AO_Table,Evt<Sig=EAT_SIG,Pool=1,Ref=0>")
    qutest.expect("%timestamp AO-Post  Sdr=AO_Table,Obj=AO_Philo<4>,Evt<Sig=EAT_SIG,Pool=1,Ref=2>,*")
    qutest.expect("%timestamp QUTEST_ON_POST EAT_SIG,Obj=AO_Philo<4> 2")
    qutest.expect("%timestamp AO-Post  Sdr=AO_Table,Obj=AO_Philo<3>,Evt<Sig=EAT_SIG,Pool=1,Ref=3>,*")
    qutest.expect("%timestamp QUTEST_ON_POST EAT_SIG,Obj=AO_Philo<3> 2")
    qutest.expect("%timestamp AO-Post  Sdr=AO_Table,Obj=AO_Philo<2>,Evt<Sig=EAT_SIG,Pool=1,Ref=4>,*")
    qutest.expect("%timestamp QUTEST_ON_POST EAT_SIG,Obj=AO_Philo<2> 2")
    qutest.expect("%timestamp AO-Post  Sdr=AO_Table,Obj=AO_Philo<1>,Evt<Sig=EAT_SIG,Pool=1,Ref=5>,*")
    qutest.expect("%timestamp QUTEST_ON_POST EAT_SIG,Obj=AO_Philo<1> 2")
    qutest.expect("%timestamp AO-Post  Sdr=AO_Table,Obj=AO_Philo<0>,Evt<Sig=EAT_SIG,Pool=1,Ref=6>,*")
    qutest.expect("%timestamp QUTEST_ON_POST EAT_SIG,Obj=AO_Philo<0> 2")
    qutest.expect("%timestamp QF-gcA   Evt<Sig=EAT_SIG,Pool=1,*")
    qutest.expect("%timestamp BSP_CALL BSP_displayPhilStat 2 eating  ")
    qutest.expect("%timestamp =>Intern Obj=AO_Table,Sig=HUNGRY_SIG,State=Table_serving")
    qutest.expect("%timestamp QF-gc    Evt<Sig=HUNGRY_SIG,Pool=1,Ref=1>")
    qutest.expect("%timestamp MP-Put   Obj=EvtPool1,*")
    qutest.expect("%timestamp AO-GetL  Obj=AO_Philo<4>,Evt<Sig=EAT_SIG,Pool=1,*")
    qutest.expect("%timestamp Disp===> Obj=AO_Philo<4>,Sig=EAT_SIG,State=Philo_thinking")
    qutest.expect("%timestamp =>Intern Obj=AO_Philo<4>,Sig=EAT_SIG,State=Philo_thinking")
    qutest.expect("%timestamp QF-gcA   Evt<Sig=EAT_SIG,Pool=1,*")
    qutest.expect("%timestamp AO-GetL  Obj=AO_Philo<3>,Evt<Sig=EAT_SIG,Pool=1,*")
    qutest.expect("%timestamp Disp===> Obj=AO_Philo<3>,Sig=EAT_SIG,State=Philo_thinking")
    qutest.expect("%timestamp =>Intern Obj=AO_Philo<3>,Sig=EAT_SIG,State=Philo_thinking")
    qutest.expect("%timestamp QF-gcA   Evt<Sig=EAT_SIG,Pool=1,*")
    qutest.expect("%timestamp AO-GetL  Obj=AO_Philo<2>,Evt<Sig=EAT_SIG,Pool=1,*")
    qutest.expect("%timestamp Disp===> Obj=AO_Philo<2>,Sig=EAT_SIG,State=Philo_hungry")
    qutest.expect("%timestamp BSP_CALL BSP_random *")
    qutest.expect("%timestamp TE0-Arm  Obj=l_philo<2>.timeEvt,AO=AO_Philo<2>,Tim=*,Int=0")
    qutest.expect("===RTC===> St-Entry Obj=AO_Philo<2>,State=Philo_eating")
    qutest.expect("%timestamp ===>Tran Obj=AO_Philo<2>,Sig=EAT_SIG,State=Philo_hungry->Philo_eating")
    qutest.expect("%timestamp QF-gcA   Evt<Sig=EAT_SIG,Pool=1,*")
    qutest.expect("%timestamp AO-GetL  Obj=AO_Philo<1>,Evt<Sig=EAT_SIG,Pool=1,*")
    qutest.expect("%timestamp Disp===> Obj=AO_Philo<1>,Sig=EAT_SIG,State=Philo_thinking")
    qutest.expect("%timestamp =>Intern Obj=AO_Philo<1>,Sig=EAT_SIG,State=Philo_thinking")
    qutest.expect("%timestamp QF-gcA   Evt<Sig=EAT_SIG,Pool=1,*")
    qutest.expect("%timestamp AO-GetL  Obj=AO_Philo<0>,Evt<Sig=EAT_SIG,Pool=1,*")
    qutest.expect("%timestamp Disp===> Obj=AO_Philo<0>,Sig=EAT_SIG,State=Philo_thinking")
    qutest.expect("%timestamp =>Intern Obj=AO_Philo<0>,Sig=EAT_SIG,State=Philo_thinking")
    qutest.expect("%timestamp QF-gc    Evt<Sig=EAT_SIG,Pool=1,*")
    qutest.expect("%timestamp MP-Put   Obj=EvtPool1,*")
    qutest.expect("%timestamp Trg-Done QS_RX_TICK")

# end...
if __name__ == "__main__":
    options = ['-x', '-v', '--tb=short']
    options.extend(sys.argv)
    pytest.main(options)

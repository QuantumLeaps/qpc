# test-script for QUTest unit testing harness
# see https://www.state-machine.com/qtools/qutest.html

import sys
import pytest
import struct
from qspypy.qspy import FILTER, QS_OBJ_KIND


def on_reset(qutest):
    qutest.expect_pause()
    qutest.glb_filter(FILTER.SM,FILTER.AO,FILTER.QF,FILTER.EQ)
    qutest.Continue()
    qutest.expect("===RTC===> St-Init  Obj=l_tserver,State=QHsm_top->TServer_idle")
    qutest.expect("%timestamp AO-RCllA Obj=l_tserver,Que=l_tserver.requestQueue")
    qutest.expect("===RTC===> St-Entry Obj=l_tserver,State=TServer_idle")
    qutest.expect("%timestamp Init===> Obj=l_tserver,State=TServer_idle")
    qutest.current_obj(QS_OBJ_KIND.SM_AO,"l_tserver")


def test_NEW_REQUEST_SIG_l_tserver(qutest):
    qutest.post("NEW_REQUEST_SIG")
    qutest.expect("%timestamp QF-New   Sig=NEW_REQUEST_SIG,*")
    qutest.expect("%timestamp AO-Post  Sdr=QS_RX,Obj=l_tserver,Evt<Sig=NEW_REQUEST_SIG,Pool=1,Ref=1>,*")
    qutest.expect("%timestamp AO-GetL  Obj=l_tserver,Evt<Sig=NEW_REQUEST_SIG,Pool=1,Ref=1>")
    qutest.expect("%timestamp Disp===> Obj=l_tserver,Sig=NEW_REQUEST_SIG,State=TServer_idle")
    qutest.expect("%timestamp QF-NewRf Evt<Sig=NEW_REQUEST_SIG,Pool=1,Ref=2>")
    qutest.expect("===RTC===> St-Entry Obj=l_tserver,State=TServer_receiving")
    qutest.expect("%timestamp ===>Tran Obj=l_tserver,Sig=NEW_REQUEST_SIG,State=TServer_idle->TServer_receiving")
    qutest.expect("%timestamp QF-gcA   Evt<Sig=NEW_REQUEST_SIG,Pool=1,Ref=2>")
    qutest.expect("%timestamp Trg-Done QS_RX_EVENT")

def test_NEW_REQUEST_SIG_l_tserver_defer(qutest_noreset):
    qutest = qutest_noreset # name change
    qutest.post("NEW_REQUEST_SIG")
    qutest.expect("%timestamp QF-New   Sig=NEW_REQUEST_SIG,*")
    qutest.expect("%timestamp AO-Post  Sdr=QS_RX,Obj=l_tserver,Evt<Sig=NEW_REQUEST_SIG,Pool=1,Ref=1>,*")
    qutest.expect("%timestamp AO-GetL  Obj=l_tserver,Evt<Sig=NEW_REQUEST_SIG,Pool=1,Ref=1>")
    qutest.expect("%timestamp Disp===> Obj=l_tserver,Sig=NEW_REQUEST_SIG,State=TServer_receiving")
    qutest.expect("%timestamp EQ-Post  Obj=l_tserver.requestQueue,Evt<Sig=NEW_REQUEST_SIG,Pool=1,Ref=2>,*")
    qutest.expect("%timestamp AO-Defer Obj=l_tserver,Que=l_tserver.requestQueue,Evt<Sig=NEW_REQUEST_SIG,Pool=1,Ref=2>")
    qutest.expect("%timestamp =>Intern Obj=l_tserver,Sig=NEW_REQUEST_SIG,State=TServer_busy")
    qutest.expect("%timestamp QF-gcA   Evt<Sig=NEW_REQUEST_SIG,Pool=1,Ref=2>")
    qutest.expect("%timestamp Trg-Done QS_RX_EVENT")

def test_RECEIVED_SIG_AUTHORIZED_SIG_l_tserver_recall(qutest_noreset):
    qutest = qutest_noreset # name change
    qutest.post("RECEIVED_SIG")
    qutest.expect("%timestamp QF-New   Sig=RECEIVED_SIG,*")
    qutest.expect("%timestamp AO-Post  Sdr=QS_RX,Obj=l_tserver,Evt<Sig=RECEIVED_SIG,Pool=1,Ref=1>,*")
    qutest.expect("%timestamp AO-GetL  Obj=l_tserver,Evt<Sig=RECEIVED_SIG,Pool=1,Ref=1>")
    qutest.expect("%timestamp Disp===> Obj=l_tserver,Sig=RECEIVED_SIG,State=TServer_receiving")
    qutest.expect("===RTC===> St-Exit  Obj=l_tserver,State=TServer_receiving")
    qutest.expect("===RTC===> St-Entry Obj=l_tserver,State=TServer_authorizing")
    qutest.expect("%timestamp ===>Tran Obj=l_tserver,Sig=RECEIVED_SIG,State=TServer_receiving->TServer_authorizing")
    qutest.expect("%timestamp QF-gc    Evt<Sig=RECEIVED_SIG,Pool=1,Ref=1>")
    qutest.expect("%timestamp Trg-Done QS_RX_EVENT")
    qutest.post("AUTHORIZED_SIG")
    qutest.expect("%timestamp QF-New   Sig=AUTHORIZED_SIG,*")
    qutest.expect("%timestamp AO-Post  Sdr=QS_RX,Obj=l_tserver,Evt<Sig=AUTHORIZED_SIG,Pool=1,Ref=1>,*")
    qutest.expect("%timestamp AO-GetL  Obj=l_tserver,Evt<Sig=AUTHORIZED_SIG,Pool=1,Ref=1>")
    qutest.expect("%timestamp Disp===> Obj=l_tserver,Sig=AUTHORIZED_SIG,State=TServer_authorizing")
    qutest.expect("===RTC===> St-Exit  Obj=l_tserver,State=TServer_authorizing")
    qutest.expect("%timestamp QF-DelRf Evt<Sig=NEW_REQUEST_SIG,Pool=1,Ref=1>")
    qutest.expect("%timestamp QF-gc    Evt<Sig=NEW_REQUEST_SIG,Pool=1,Ref=1>")
    qutest.expect("===RTC===> St-Exit  Obj=l_tserver,State=TServer_busy")
    qutest.expect("%timestamp EQ-GetL  Obj=l_tserver.requestQueue,Evt<Sig=NEW_REQUEST_SIG,Pool=1,Ref=1>")
    qutest.expect("%timestamp AO-LIFO  Obj=l_tserver,Evt<Sig=NEW_REQUEST_SIG,Pool=1,Ref=2>,*")
    qutest.expect("%timestamp AO-RCall Obj=l_tserver,Que=l_tserver.requestQueue,Evt<Sig=NEW_REQUEST_SIG,Pool=1,Ref=1>")
    qutest.expect("===RTC===> St-Entry Obj=l_tserver,State=TServer_idle")
    qutest.expect("%timestamp ===>Tran Obj=l_tserver,Sig=AUTHORIZED_SIG,State=TServer_authorizing->TServer_idle")
    qutest.expect("%timestamp QF-gc    Evt<Sig=AUTHORIZED_SIG,Pool=1,Ref=1>")
    qutest.expect("%timestamp AO-GetL  Obj=l_tserver,Evt<Sig=NEW_REQUEST_SIG,Pool=1,Ref=1>")
    qutest.expect("%timestamp Disp===> Obj=l_tserver,Sig=NEW_REQUEST_SIG,State=TServer_idle")
    qutest.expect("%timestamp QF-NewRf Evt<Sig=NEW_REQUEST_SIG,Pool=1,Ref=2>")
    qutest.expect("===RTC===> St-Entry Obj=l_tserver,State=TServer_receiving")
    qutest.expect("%timestamp ===>Tran Obj=l_tserver,Sig=NEW_REQUEST_SIG,State=TServer_idle->TServer_receiving")
    qutest.expect("%timestamp QF-gcA   Evt<Sig=NEW_REQUEST_SIG,Pool=1,Ref=2>")
    qutest.expect("%timestamp Trg-Done QS_RX_EVENT")



if __name__ == "__main__":
    options = ['-x', '-v', '--tb=short']
    options.extend(sys.argv)
    pytest.main(options)

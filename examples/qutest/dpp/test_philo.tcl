# test-script for QUTest unit testing harness
# see https://www.state-machine.com/qtools/qutest.html

# preamble...
proc on_reset {} {
    expect_pause
    glb_filter SM
    loc_filter SM_AO AO_Philo<2>
    continue
    expect "===RTC===> St-Init  Obj=AO_Philo<2>,State=QHsm_top->Philo_thinking"
    expect "===RTC===> St-Entry Obj=AO_Philo<2>,State=Philo_thinking"
    expect "%timestamp Init===> Obj=AO_Philo<2>,State=Philo_thinking"
    glb_filter SM AO UA
    current_obj SM_AO AO_Philo<2>
}

# tests...
test "TIMEOUT->Philo (post)"
post TIMEOUT_SIG
expect "%timestamp AO-Post  Sdr=QS_RX,Obj=AO_Philo<2>,Evt<Sig=TIMEOUT_SIG,*"
expect "%timestamp AO-GetL  Obj=AO_Philo<2>,Evt<Sig=TIMEOUT_SIG,*"
expect "%timestamp Disp===> Obj=AO_Philo<2>,Sig=TIMEOUT_SIG,State=Philo_thinking"
expect "===RTC===> St-Exit  Obj=AO_Philo<2>,State=Philo_thinking"
expect "===RTC===> St-Entry Obj=AO_Philo<2>,State=Philo_hungry"
expect "%timestamp ===>Tran Obj=AO_Philo<2>,Sig=TIMEOUT_SIG,State=Philo_thinking->Philo_hungry"
expect "%timestamp Trg-Done QS_RX_EVENT"

test "publish EAT(2)" -noreset
loc_filter SM_AO AO_Philo<2>
publish EAT_SIG [binary format c 2]
expect "%timestamp AO-Post  Sdr=QS_RX,Obj=AO_Philo<2>,Evt<Sig=EAT_SIG,*"
expect "%timestamp Trg-Done QS_RX_EVENT"
expect "%timestamp AO-GetL  Obj=AO_Philo<2>,Evt<Sig=EAT_SIG,*"
expect "%timestamp Disp===> Obj=AO_Philo<2>,Sig=EAT_SIG,State=Philo_hungry"
expect "%timestamp BSP_CALL BSP_random 123"
expect "===RTC===> St-Entry Obj=AO_Philo<2>,State=Philo_eating"
expect "%timestamp ===>Tran Obj=AO_Philo<2>,Sig=EAT_SIG,State=Philo_hungry->Philo_eating"
expect "%timestamp Trg-Done QS_RX_EVENT"

test "TIMEOUT->Philo_thinking (ASSERT)"
probe QActive_post_ 1
dispatch TIMEOUT_SIG
expect "%timestamp Disp===> Obj=AO_Philo<2>,Sig=TIMEOUT_SIG,State=Philo_thinking"
expect "===RTC===> St-Exit  Obj=AO_Philo<2>,State=Philo_thinking"
expect "%timestamp TstProbe Fun=QActive_post_,Data=1"
expect "%timestamp =ASSERT= Mod=qf_actq,Loc=110"

test "TIMEOUT->Philo_eating (PUBLISH from AO)"
glb_filter OFF
dispatch TIMEOUT_SIG
expect "%timestamp Trg-Done QS_RX_EVENT"
dispatch EAT_SIG [binary format c 2]
expect "%timestamp Trg-Done QS_RX_EVENT"
glb_filter SM AO QF
dispatch TIMEOUT_SIG
expect "%timestamp QF-New   Sig=TIMEOUT_SIG,*"
expect "%timestamp Disp===> Obj=AO_Philo<2>,Sig=TIMEOUT_SIG,State=Philo_eating"
expect "%timestamp QF-New   Sig=DONE_SIG,*"
expect "%timestamp QF-Pub   Sdr=AO_Philo<2>,Evt<Sig=DONE_SIG,Pool=1,Ref=0>"
expect "%timestamp QF-gcA   Evt<Sig=DONE_SIG,Pool=1,Ref=2>"
expect "===RTC===> St-Exit  Obj=AO_Philo<2>,State=Philo_eating"
expect "===RTC===> St-Entry Obj=AO_Philo<2>,State=Philo_thinking"
expect "%timestamp ===>Tran Obj=AO_Philo<2>,Sig=TIMEOUT_SIG,State=Philo_eating->Philo_thinking"
expect "%timestamp QF-gc    Evt<Sig=TIMEOUT_SIG,Pool=1,Ref=1>"
expect "%timestamp Trg-Done QS_RX_EVENT"

test "timeEvt->Philo (tick)"
glb_filter SM AO TE
current_obj TE l_philo<2>.timeEvt
tick
expect "           TE0-ADis Obj=l_philo<2>.timeEvt,AO=AO_Philo<2>"
expect "%timestamp TE0-Post Obj=l_philo<2>.timeEvt,Sig=TIMEOUT_SIG,AO=AO_Philo<2>"
expect "%timestamp AO-Post  Sdr=QS_RX,Obj=AO_Philo<2>,Evt<Sig=TIMEOUT_SIG*"
expect "%timestamp AO-GetL  Obj=AO_Philo<2>,Evt<Sig=TIMEOUT_SIG*"
expect "%timestamp Disp===> Obj=AO_Philo<2>,Sig=TIMEOUT_SIG,State=Philo_thinking"
expect "%timestamp TE0-DisA Obj=l_philo<2>.timeEvt,AO=AO_Philo<2>"
expect "===RTC===> St-Exit  Obj=AO_Philo<2>,State=Philo_thinking"
expect "===RTC===> St-Entry Obj=AO_Philo<2>,State=Philo_hungry"
expect "%timestamp ===>Tran Obj=AO_Philo<2>,Sig=TIMEOUT_SIG,State=Philo_thinking->Philo_hungry"
expect "%timestamp Trg-Done QS_RX_TICK"

# the end
end

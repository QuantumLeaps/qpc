# test script for QUTest unit testing harness
# see https://www.state-machine.com/qtools/qutest.html/qutest.html

# preamble...
def on_reset():
    expect_pause()
    continue_test()
    expect_run()

# tests...
test("tick")
glb_filter(GRP_ALL)
current_obj(OBJ_TE, "Philo_inst[2].timeEvt")
tick()
expect("           Tick<0>  Ctr=*")
expect("           TE0-ADis Obj=Philo_inst[2].timeEvt,AO=Table_inst")
expect("@timestamp TE0-Post Obj=Philo_inst[2].timeEvt,Sig=TIMEOUT_SIG,AO=Table_inst")
expect("@timestamp AO-Post  Sdr=QS_RX,Obj=Table_inst,Evt<Sig=TIMEOUT_SIG,Pool=0,Ref=0>,*")
expect("@timestamp AO-GetL  Obj=Table_inst,Evt<Sig=TIMEOUT_SIG,Pool=0,Ref=0>")
expect("@timestamp Disp===> Obj=Table_inst,Sig=TIMEOUT_SIG,State=Table_serving")
expect("@timestamp Disp===> Obj=Philo_inst[2],Sig=TIMEOUT_SIG,State=Philo_thinking")
expect("@timestamp TE0-DisA Obj=Philo_inst[2].timeEvt,AO=Table_inst")
expect("===RTC===> St-Exit  Obj=Philo_inst[2],State=Philo_thinking")
expect("@timestamp MP-Get   Obj=EvtPool1,*")
expect("@timestamp QF-New   Sig=HUNGRY_SIG,*")
expect("@timestamp AO-LIFO  Obj=Table_inst,Evt<Sig=HUNGRY_SIG,Pool=1,*")
expect("@timestamp QUTEST_ON_POST HUNGRY_SIG,Obj=Table_inst 2")
expect("===RTC===> St-Entry Obj=Philo_inst[2],State=Philo_hungry")
expect("@timestamp ===>Tran Obj=Philo_inst[2],Sig=TIMEOUT_SIG,State=Philo_thinking->Philo_hungry")
expect("@timestamp =>Intern Obj=Table_inst,Sig=TIMEOUT_SIG,State=Table_active")
expect("@timestamp AO-GetL  Obj=Table_inst,Evt<Sig=HUNGRY_SIG,Pool=1,*")
expect("@timestamp Disp===> Obj=Table_inst,Sig=HUNGRY_SIG,State=Table_serving")
expect("@timestamp BSP_CALL BSP_displayPhilStat 2 hungry  ")
expect("@timestamp Disp===> Obj=Philo_inst[2],Sig=EAT_SIG,State=Philo_hungry")
expect("@timestamp BSP_CALL BSP_random *")
expect("@timestamp TE0-Arm  Obj=Philo_inst[2].timeEvt,AO=Table_inst,Tim=*,Int=0")
expect("===RTC===> St-Entry Obj=Philo_inst[2],State=Philo_eating")
expect("@timestamp ===>Tran Obj=Philo_inst[2],Sig=EAT_SIG,State=Philo_hungry->Philo_eating")
expect("@timestamp BSP_CALL BSP_displayPhilStat 2 eating  ")
expect("@timestamp =>Intern Obj=Table_inst,Sig=HUNGRY_SIG,State=Table_serving")
expect("@timestamp QF-gc    Evt<Sig=HUNGRY_SIG,Pool=1,*")
expect("@timestamp MP-Put   Obj=EvtPool1,*")
expect("@timestamp Trg-Done QS_RX_TICK")


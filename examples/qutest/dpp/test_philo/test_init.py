# test script for QUTest unit testing harness
# see https://www.state-machine.com/qtools/qutest.html

# preamble...
def on_reset():
    expect_pause()
    glb_filter(GRP_ALL)
    continue_test()
    expect("@timestamp AO-Subsc Obj=Philo_inst[2],Sig=EAT_SIG")
    expect("@timestamp AO-Subsc Obj=Philo_inst[2],Sig=TEST_SIG")
    expect("===RTC===> St-Init  Obj=Philo_inst[2],State=QHsm_top->Philo_thinking")
    expect("@timestamp BSP_CALL BSP_random *")
    expect("@timestamp TE0-Arm  Obj=Philo_inst[2].timeEvt,AO=Philo_inst[2],Tim=*,Int=0")
    expect("===RTC===> St-Entry Obj=Philo_inst[2],State=Philo_thinking")
    expect("@timestamp Init===> Obj=Philo_inst[2],State=Philo_thinking")
    expect("===RTC===> St-Init  Obj=Table_inst,State=QHsm_top->NULL")
    expect_run()

# tests...
test("init")

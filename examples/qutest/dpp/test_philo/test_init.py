# test script for QUTest unit testing harness
# see https://www.state-machine.com/qtools/qutest.html

# preamble...
def on_reset():
    expect_pause()
    glb_filter(GRP_ALL)
    continue_test()
    expect("@timestamp AO-Subsc Obj=AO_Philo[2],Sig=EAT_SIG")
    expect("@timestamp AO-Subsc Obj=AO_Philo[2],Sig=TEST_SIG")
    expect("===RTC===> St-Init  Obj=AO_Philo[2],State=QHsm_top->Philo_thinking")
    expect("@timestamp BSP_CALL BSP_random *")
    expect("@timestamp TE0-Arm  Obj=l_philo[2].timeEvt,AO=AO_Philo[2],Tim=*,Int=0")
    expect("===RTC===> St-Entry Obj=AO_Philo[2],State=Philo_thinking")
    expect("@timestamp Init===> Obj=AO_Philo[2],State=Philo_thinking")
    expect("===RTC===> St-Init  Obj=AO_Table,State=QHsm_top->NULL")
    expect_run()

# tests...
test("init")

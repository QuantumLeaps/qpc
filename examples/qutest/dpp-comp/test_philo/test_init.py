# test script for QUTest unit testing harness
# see https://www.state-machine.com/qtools/qutest.html

# preamble...
def on_reset():
    expect_pause()
    glb_filter(GRP_ALL)
    continue_test()
    expect("===RTC===> St-Init  Obj=Philo_inst[0],State=QHsm_top->Philo_thinking")
    expect("@timestamp BSP_CALL BSP_random *")
    expect("@timestamp TE0-Arm  Obj=Philo_inst[0].timeEvt,AO=Table_dummy,Tim=*,Int=0")
    expect("===RTC===> St-Entry Obj=Philo_inst[0],State=Philo_thinking")
    expect("@timestamp Init===> Obj=Philo_inst[0],State=Philo_thinking")
    expect("===RTC===> St-Init  Obj=Philo_inst[1],State=QHsm_top->Philo_thinking")
    expect("@timestamp BSP_CALL BSP_random *")
    expect("@timestamp TE0-Arm  Obj=Philo_inst[1].timeEvt,AO=Table_dummy,Tim=*,Int=0")
    expect("===RTC===> St-Entry Obj=Philo_inst[1],State=Philo_thinking")
    expect("@timestamp Init===> Obj=Philo_inst[1],State=Philo_thinking")
    expect("===RTC===> St-Init  Obj=Philo_inst[2],State=QHsm_top->Philo_thinking")
    expect("@timestamp BSP_CALL BSP_random *")
    expect("@timestamp TE0-Arm  Obj=Philo_inst[2].timeEvt,AO=Table_dummy,Tim=*,Int=0")
    expect("===RTC===> St-Entry Obj=Philo_inst[2],State=Philo_thinking")
    expect("@timestamp Init===> Obj=Philo_inst[2],State=Philo_thinking")
    expect("===RTC===> St-Init  Obj=Philo_inst[3],State=QHsm_top->Philo_thinking")
    expect("@timestamp BSP_CALL BSP_random *")
    expect("@timestamp TE0-Arm  Obj=Philo_inst[3].timeEvt,AO=Table_dummy,Tim=*,Int=0")
    expect("===RTC===> St-Entry Obj=Philo_inst[3],State=Philo_thinking")
    expect("@timestamp Init===> Obj=Philo_inst[3],State=Philo_thinking")
    expect("===RTC===> St-Init  Obj=Philo_inst[4],State=QHsm_top->Philo_thinking")
    expect("@timestamp BSP_CALL BSP_random *")
    expect("@timestamp TE0-Arm  Obj=Philo_inst[4].timeEvt,AO=Table_dummy,Tim=*,Int=0")
    expect("===RTC===> St-Entry Obj=Philo_inst[4],State=Philo_thinking")
    expect("@timestamp Init===> Obj=Philo_inst[4],State=Philo_thinking")
    expect("===RTC===> St-Init  Obj=Table_dummy,State=QHsm_top->NULL")
    expect_run()

# tests...
test("init")

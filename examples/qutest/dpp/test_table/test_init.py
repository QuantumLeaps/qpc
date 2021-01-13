# test-script for QUTest unit testing harness
# see https://www.state-machine.com/qtools/qutest.html

# preamble...
def on_reset():
    expect_pause()
    glb_filter(GRP_ALL)
    continue_test()
    expect("===RTC===> St-Init  Obj=AO_Philo[0],State=QHsm_top->NULL")
    expect("@timestamp AO-Subsc Obj=AO_Philo[0],Sig=EAT_SIG")
    expect("===RTC===> St-Init  Obj=AO_Philo[1],State=QHsm_top->NULL")
    expect("@timestamp AO-Subsc Obj=AO_Philo[1],Sig=EAT_SIG")
    expect("===RTC===> St-Init  Obj=AO_Philo[2],State=QHsm_top->NULL")
    expect("@timestamp AO-Subsc Obj=AO_Philo[2],Sig=EAT_SIG")
    expect("===RTC===> St-Init  Obj=AO_Philo[3],State=QHsm_top->NULL")
    expect("@timestamp AO-Subsc Obj=AO_Philo[3],Sig=EAT_SIG")
    expect("===RTC===> St-Init  Obj=AO_Philo[4],State=QHsm_top->NULL")
    expect("@timestamp AO-Subsc Obj=AO_Philo[4],Sig=EAT_SIG")
    expect("@timestamp AO-Subsc Obj=AO_Table,Sig=DONE_SIG")
    expect("@timestamp AO-Subsc Obj=AO_Table,Sig=PAUSE_SIG")
    expect("@timestamp AO-Subsc Obj=AO_Table,Sig=SERVE_SIG")
    expect("@timestamp AO-Subsc Obj=AO_Table,Sig=TEST_SIG")
    expect("@timestamp BSP_CALL BSP_displayPhilStat 0 thinking")
    expect("@timestamp BSP_CALL BSP_displayPhilStat 1 thinking")
    expect("@timestamp BSP_CALL BSP_displayPhilStat 2 thinking")
    expect("@timestamp BSP_CALL BSP_displayPhilStat 3 thinking")
    expect("@timestamp BSP_CALL BSP_displayPhilStat 4 thinking")
    expect("===RTC===> St-Init  Obj=AO_Table,State=QHsm_top->Table_serving")
    expect("===RTC===> St-Entry Obj=AO_Table,State=Table_serving")
    expect("@timestamp Init===> Obj=AO_Table,State=Table_serving")
    expect_run()

# tests...
test("init")


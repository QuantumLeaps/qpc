# test-script for QUTest unit testing harness
# see https://www.state-machine.com/qtools/qutest.html

# preamble...
def on_reset():
    expect_pause()
    glb_filter(GRP_ALL)
    loc_filter(IDS_ALL, -IDS_AP)
    continue_test()
    expect("@timestamp AO-Subsc Obj=Table_inst,Sig=PAUSE_SIG")
    expect("@timestamp AO-Subsc Obj=Table_inst,Sig=SERVE_SIG")
    expect("@timestamp AO-Subsc Obj=Table_inst,Sig=TEST_SIG")
    expect("@timestamp BSP_CALL BSP_displayPhilStat 0 thinking")
    expect("@timestamp BSP_CALL BSP_displayPhilStat 1 thinking")
    expect("@timestamp BSP_CALL BSP_displayPhilStat 2 thinking")
    expect("@timestamp BSP_CALL BSP_displayPhilStat 3 thinking")
    expect("@timestamp BSP_CALL BSP_displayPhilStat 4 thinking")
    expect("===RTC===> St-Init  Obj=Table_inst,State=QHsm_top->Table_serving")
    expect("===RTC===> St-Entry Obj=Table_inst,State=Table_serving")
    expect("@timestamp Init===> Obj=Table_inst,State=Table_serving")
    expect_run()

# tests...
test("init")


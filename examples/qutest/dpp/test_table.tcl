# test-script for QUTest unit testing harness
# see https://www.state-machine.com/qtools/qutest.html

# preamble...
proc on_reset {} {
    expect_pause
    continue
    glb_filter SM AO UA
    loc_filter AO AO_Table
    current_obj SM_AO AO_Table
}

proc on_setup {} {
    #puts "on_setup"
}

proc on_teardown {} {
    #puts "on_teardown"
}


# tests...
test "PAUSE->Table"
dispatch PAUSE_SIG
expect "%timestamp Disp===> Obj=AO_Table,Sig=PAUSE_SIG,State=Table_serving"
expect "%timestamp BSP_CALL BSP_displayPaused 1"
expect "===RTC===> St-Entry Obj=AO_Table,State=Table_paused"
expect "%timestamp ===>Tran Obj=AO_Table,Sig=PAUSE_SIG,State=Table_serving->Table_paused"
expect "%timestamp Trg-Done QS_RX_EVENT"

test "SERVE->Table (1)"
command 1
expect "%timestamp Disp===> Obj=AO_Table,Sig=SERVE_SIG,State=Table_serving"
expect "%timestamp =>Ignore Obj=AO_Table,Sig=SERVE_SIG,State=Table_serving"
expect "%timestamp Trg-Done QS_RX_COMMAND"

test "SERVE->Table (2)" -noreset
probe BSP_displayPaused 1
dispatch PAUSE_SIG
expect "%timestamp Disp===> Obj=AO_Table,Sig=PAUSE_SIG,State=Table_serving"
expect "%timestamp TstProbe Fun=BSP_displayPaused,Data=1"
expect "%timestamp =ASSERT= Mod=bsp,Loc=100"

# the end
end

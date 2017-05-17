# QUTEST for 'Test-Probes'

# preamble...
proc on_setup {} {
    expect "%timestamp ON_TEST_SETUP"
}
proc on_teardown {} {
    expect "%timestamp ON_TEST_TEARDOWN"
}

# tests...

#----------
test "Single Test Probe"
command COMMAND_X
expect "%timestamp COMMAND_X 0"
expect "%timestamp Trg-Done QS_RX_COMMAND"
probe myFun 1
command COMMAND_X
expect "%timestamp TstProbe Fun=myFun,Data=1"
expect "%timestamp COMMAND_X 1"
expect "%timestamp Trg-Done QS_RX_COMMAND"
command COMMAND_X
expect "%timestamp COMMAND_X 0"
expect "%timestamp Trg-Done QS_RX_COMMAND"

#----------
test "Multiple Test Probes" -noreset
probe myFun 100022
probe myFun 200033
command COMMAND_X
expect "%timestamp TstProbe Fun=myFun,Data=100022"
expect "%timestamp COMMAND_X 100022"
expect "%timestamp Trg-Done QS_RX_COMMAND"
command COMMAND_X
expect "%timestamp TstProbe Fun=myFun,Data=200033"
expect "%timestamp COMMAND_X 200033"
expect "%timestamp Trg-Done QS_RX_COMMAND"
command COMMAND_X
expect "%timestamp COMMAND_X 0"
expect "%timestamp Trg-Done QS_RX_COMMAND"

# the end
end


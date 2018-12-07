# test-script for QUTest unit testing harness
# see https://www.state-machine.com/qtools/qutest.html

# preamble...
proc on_reset {} {
    expect_pause
    glb_filter UA
    current_obj SM_AO AO_MyAO
    continue
}

# tests...
#----------
test "EVT0"
dispatch MY_EVT0_SIG
expect "@timestamp MY_EVT0"
expect "@timestamp Trg-Done QS_RX_EVENT"

#----------
test "EVT1"
dispatch MY_EVT1_SIG [binary format i 12345]
expect "@timestamp MY_EVT1 12345"
expect "@timestamp Trg-Done QS_RX_EVENT"

#----------
test "EVT2"
dispatch MY_EVT2_SIG [binary format is 0x1234 678]
expect "@timestamp MY_EVT2 4660 678"
expect "@timestamp Trg-Done QS_RX_EVENT"

#----------
test "EVT2"
dispatch MY_EVT3_SIG [binary format isc 87654 0xdead 123]
expect "@timestamp MY_EVT3 87654 57005 123"
expect "@timestamp Trg-Done QS_RX_EVENT"

# the end
end


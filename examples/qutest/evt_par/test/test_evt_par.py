# test-script for QUTest unit testing harness
# see https://www.state-machine.com/qtools/qutest.html

# preamble...
def on_reset():
    expect_pause()
    glb_filter(GRP_UA)
    current_obj(OBJ_SM_AO, "AO_MyAO")
    continue_test()
    expect_run()

# tests...
test("EVT0")
dispatch("MY_EVT0_SIG")
expect("@timestamp MY_EVT0")
expect("@timestamp Trg-Done QS_RX_EVENT")

test("EVT1")
dispatch("MY_EVT1_SIG", pack('<I', 12345))
expect("@timestamp MY_EVT1 12345")
expect("@timestamp Trg-Done QS_RX_EVENT")

test("EVT2")
dispatch("MY_EVT2_SIG", pack('<IH',0x1234,678))
expect("@timestamp MY_EVT2 4660 678")
expect("@timestamp Trg-Done QS_RX_EVENT")

test("EVT3")
dispatch("MY_EVT3_SIG", pack('<IHB',87654,0xDEAD, 123))
expect("@timestamp MY_EVT3 87654 57005 123")
expect("@timestamp Trg-Done QS_RX_EVENT")


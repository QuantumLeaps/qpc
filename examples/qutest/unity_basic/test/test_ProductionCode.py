# QUTEST script corresponding to the test_ProductionCode.c test fixture.
# This test fixture corresponds to ../test_unity/TestProductionCode.c fixture.
# see https://www.state-machine.com/qtools/qutest.html/qutest.html

# preamble...
def on_setup():
    # This is run before EACH TEST
    current_obj(OBJ_AP, "Counter")
    poke(0, 4, pack('<L', 0x5A5A))

# tests...
test("FindFunction_WhichIsBroken() Should Return Zero If Item Is Not In List, Which Works Even In Our Broken Code")
# All of these should pass
command(0, 78)
expect("0000000001 USER+000 FindFunction_WhichIsBroken 0 78")
expect("0000000002 Trg-Done QS_RX_COMMAND")
command(0, 2)
expect("0000000003 USER+000 FindFunction_WhichIsBroken 0 2")
expect("0000000004 Trg-Done QS_RX_COMMAND")
command(0, 33)
expect("@timestamp USER+000 FindFunction_WhichIsBroken 0 33")
expect("@timestamp Trg-Done QS_RX_COMMAND")
command(0, 999)
expect("@timestamp USER+000 FindFunction_WhichIsBroken 0 999")
expect("@timestamp Trg-Done QS_RX_COMMAND")
command(0, (-1) & 0xFFFFFFFF)
expect("@timestamp USER+000 FindFunction_WhichIsBroken 0 -1")
expect("@timestamp Trg-Done QS_RX_COMMAND")

test("FindFunction_WhichIsBroken() Should Return The Index For Items In List, Which Will Fail Because Our Function Under Test Is Broken")
command(0, 34)
expect("@timestamp USER+000 FindFunction_WhichIsBroken 1 34")
# Notice the rest of these didn't get a chance to run because the line above failed.
# Unit tests abort each test on the first sign of trouble.
# Then NEXT test runs as normal.
expect("@timestamp Trg-Done QS_RX_COMMAND")
command(0, 8888)
expect("@timestamp USER+000 FindFunction_WhichIsBroken 8 8888")
expect("@timestamp Trg-Done QS_RX_COMMAND")

test("FunctionWhichReturnsLocalVariable() Should Return The Current Counter Value")
command(1)
expect("@timestamp USER+001 FunctionWhichReturnsLocalVariable 0x5A5A")
expect("@timestamp Trg-Done QS_RX_COMMAND")
# This should be true because we can still change our answer
poke(0, 4, pack('<L', 0x1234))
command(1)
expect("@timestamp USER+001 FunctionWhichReturnsLocalVariable 0x1234")
expect("@timestamp Trg-Done QS_RX_COMMAND")

test("FunctionWhichReturnsLocalVariable() Should Return The Current Counter Value Again", NORESET)
# This should be true again because setup was rerun before this test (and after we changed it to 0x1234)
command(1)
expect("@timestamp USER+001 FunctionWhichReturnsLocalVariable 0x5A5A")
expect("@timestamp Trg-Done QS_RX_COMMAND")

test("FunctionWhichReturnsLocalVariable() Should Return Current Counter, But Fails Because This Test Is Actually Flawed", NORESET)
# Sometimes you get the test wrong.  When that happens, you get a failure too... and a quick look should tell
# you what actually happened...which in this case was a failure to setup the initial condition.
command(1)
expect("@timestamp USER+001 FunctionWhichReturnsLocalVariable 0x1234")
expect("@timestamp Trg-Done QS_RX_COMMAND")


# test-script for QUTest unit testing harness
# see https://www.state-machine.com/qtools/qutest.html

include("test_include.pyi")

note('''
This test group contains tests that intenionally FAIL,
to exercise failure modes of the QUTest system.
''')

def on_reset():
    #note("on_reset()")
    expect("           QF_RUN")

# tests...
test("Expected assertion")
command("CMD_A", 0)
expect("@timestamp =ASSERT= Mod=test_qutest,Loc=100")

test("Unexpected assertion (should FAIL!)")
command("CMD_A", 0)
expect("@timestamp COMMAND CMD_A 0")
expect("@timestamp Trg-Done QS_RX_COMMAND")

test("Simple passing test")
command("CMD_A", 1)
expect("@timestamp COMMAND CMD_A 1")
expect("@timestamp Trg-Done QS_RX_COMMAND")

test("Wrong assertion expectation (should FAIL!)")
command("CMD_A", 0)
expect("@timestamp =ASSERT= Mod=test_qutest,Loc=200")

test("Simple passing test")
command("CMD_A", 1)
expect("@timestamp COMMAND CMD_A 1")
expect("@timestamp Trg-Done QS_RX_COMMAND")

# test-script for QUTest unit testing harness
# see https://www.state-machine.com/qtools/qutest.html

# preamble...
def on_setup():
    expect("@timestamp FIXTURE_SETUP")

def on_teardown():
    expect("@timestamp FIXTURE_TEARDOWN")

# tests...
test("Assert 0")
command("COMMAND_A", 0)
expect("@timestamp =ASSERT= Mod=test_qutest,Loc=100")

test("Assert 1")
command("COMMAND_A", 1)
expect("@timestamp COMMAND_A 1")
expect("@timestamp Trg-Done QS_RX_COMMAND")

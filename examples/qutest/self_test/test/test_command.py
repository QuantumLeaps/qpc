# test-script for QUTest unit testing harness
# see https://www.state-machine.com/qtools/qutest.html

note('''
This test group tests the command() command, which
invokes the QS_onCommand() callback inside the target.
''')

def on_setup():
    expect("@timestamp FIXTURE_SETUP")

def on_teardown():
    expect("@timestamp FIXTURE_TEARDOWN")

# tests...
test("Command")
note('''
this test invokes command(1,1,1)
''')
command("CMD_X", 1, 2, 3)
expect("@timestamp COMMAND CMD_X 0")
expect("@timestamp Trg-Done QS_RX_COMMAND")


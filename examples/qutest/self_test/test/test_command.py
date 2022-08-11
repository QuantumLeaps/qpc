# test-script for QUTest unit testing harness
# see https://www.state-machine.com/qtools/qutest.html

include("test_include.pyi")

# tests...
test("Command")
command_x(1, 2, 3)
expect("@timestamp COMMAND_X 0")
expect("@timestamp Trg-Done QS_RX_COMMAND")


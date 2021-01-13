# test-script for QUTest unit testing harness
# see https://www.state-machine.com/qtools/qutest.html

# preamble...
def on_setup():
    expect("@timestamp FIXTURE_SETUP")

def on_teardown():
    expect("@timestamp FIXTURE_TEARDOWN")


# tests...
test("MEM/STR")
current_obj(OBJ_AP,"buffer")
fill(0,1,16,0x1A)
poke(33,1,b"Hello World!\0")
command("COMMAND_Y", 16)
expect("@timestamp COMMAND_Y myFun  1A 1A 1A 1A 1A 1A 1A 1A 1A 1A 1A 1A 1A 1A 1A 1A Hello World!")
expect("@timestamp Trg-Done QS_RX_COMMAND")

test("MEM/STR empty string")
current_obj(OBJ_AP,"buffer")
fill(0,1,16,0x1A)
poke(33,1,b"\0")
command("COMMAND_Y", 16)
expect("@timestamp COMMAND_Y myFun  1A 1A 1A 1A 1A 1A 1A 1A 1A 1A 1A 1A 1A 1A 1A 1A ''")
expect("@timestamp Trg-Done QS_RX_COMMAND")

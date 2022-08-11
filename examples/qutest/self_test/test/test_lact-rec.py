# test-script for QUTest unit testing harness
# see https://www.state-machine.com/qtools/qutest.html

include("test_include.pyi")

def print_params():
    global p1, p2, p3
    print("params:", p1, p2, p3, p4)

# tests...
test("Last-record")
command("COMMAND_B", 123, 23456, 3456789)
expect("@timestamp COMMAND_B *")
last = last_rec().split()
p1 = int(last[2])
s1 = last[3]
p2 = int(last[4])
s2 = last[5]
p3 = int(last[6])
p4 = float(last[7])
print_params()
expect("@timestamp Trg-Done QS_RX_COMMAND")

# test-script for QUTest unit testing harness
# see https://www.state-machine.com/qtools/qutest.html

note('''
This test group exercises the las_rec() command
''')

include("test_include.pyi")

def display_params():
    global p1, p2, p3, p4
    note("params: %d %d %d %e"%(p1, p2, p3, p4))

# tests...
test("Last-record")
command("CMD_B", 123, 23456, 3456789)
expect("@timestamp COMMAND CMD_B *")
last = last_rec().split()
p1 = int(last[3])
s1 = last[3]
p2 = int(last[5])
s2 = last[5]
p3 = int(last[7])
p4 = float(last[8])
display_params() # locally defined function
expect("@timestamp Trg-Done QS_RX_COMMAND")

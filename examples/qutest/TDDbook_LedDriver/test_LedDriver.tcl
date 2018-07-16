# QUTEST test script corresponding to the test_LedDriver.c test fixture.
# see https://www.state-machine.com/qtools/qutest.html
#
# This example corresponds to LedDriver.c from Chapters 3 & 4 "Starting
# a C Module" & "Testing Your Way to Done" of the book: "Test-Driven
# Development for Embedded Systems" by James W. Grenning

# tests...

#----------
test "LEDs off after Create"
command LEDDRIVER_CREATE
expect "%timestamp LEDDRIVER_CREATE 0x0000"
expect "%timestamp Trg-Done QS_RX_COMMAND"

#----------
test "Turn on LED(1)" -noreset
command LEDDRIVER_TURNON 1
expect "%timestamp LEDDRIVER_TURNON 0x0001"
expect "%timestamp Trg-Done QS_RX_COMMAND"

#----------
test "Turn off LED(1)" -noreset
command LEDDRIVER_TURNOFF 1
expect "%timestamp LEDDRIVER_TURNOFF 0x0000"
expect "%timestamp Trg-Done QS_RX_COMMAND"

#----------
test "Turn on multiple LEDs" -noreset
command LEDDRIVER_TURNON 9
expect "%timestamp LEDDRIVER_TURNON 0x0100"
expect "%timestamp Trg-Done QS_RX_COMMAND"
command LEDDRIVER_TURNON 8
expect "%timestamp LEDDRIVER_TURNON 0x0180"
expect "%timestamp Trg-Done QS_RX_COMMAND"

#----------
test "Turn off multiple LEDs" -noreset
command LEDDRIVER_TURNALLON
expect "%timestamp LEDDRIVER_TURNALLON 0xFFFF"
expect "%timestamp Trg-Done QS_RX_COMMAND"
command LEDDRIVER_TURNOFF 9
expect "%timestamp LEDDRIVER_TURNOFF 0xFEFF"
expect "%timestamp Trg-Done QS_RX_COMMAND"
command LEDDRIVER_TURNOFF 8
expect "%timestamp LEDDRIVER_TURNOFF 0xFE7F"
expect "%timestamp Trg-Done QS_RX_COMMAND"

#----------
test "Out of bounds produces Runtime Error" -noreset
command LEDDRIVER_TURNON -1
expect "%timestamp =ASSERT= Mod=LedDriver,Loc=100"

#----------
test "Turn off any LED"
command LEDDRIVER_TURNALLON
expect "%timestamp LEDDRIVER_TURNALLON 0xFFFF"
expect "%timestamp Trg-Done QS_RX_COMMAND"
command LEDDRIVER_TURNOFF 8
expect "%timestamp LEDDRIVER_TURNOFF 0xFF7F"
expect "%timestamp Trg-Done QS_RX_COMMAND"

#----------
test "LED memory is not readable" -noreset
current_obj AP virtualLeds
poke 0 2 [binary format s 0xFFFF]
peek 0 2 1
expect "%timestamp Trg-Peek Offs=0,Size=2,Num=1,Data=<FFFF>"
command LEDDRIVER_TURNON 8
expect "%timestamp LEDDRIVER_TURNON 0x0080"
expect "%timestamp Trg-Done QS_RX_COMMAND"

# the end
end


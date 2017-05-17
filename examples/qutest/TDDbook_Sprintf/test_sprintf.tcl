# QUTEST test script corresponding to the test_Sprintf.c test fixture.
# This example corresponds to SprintfTest.c from Chapter 2 "Test-Driving
# Tools and Conventions" of the book: "Test-Driven Development for Embedded
# Systems" by James W. Grenning

# tests...

#----------
test "No format operations"
glb_filter UA
current_obj AP format
poke 0 1 "Hello World!\0"
command 0
expect "0000000001 SPRINTF_CALL  12 Hello World!"
expect "0000000002 Trg-Done QS_RX_COMMAND"

#----------
test "Insert decimal" -noreset
current_obj AP format
poke 0 1 "dec=%d\0"
command 0 1234
expect "0000000001 SPRINTF_CALL   8 dec=1234"
expect "0000000002 Trg-Done QS_RX_COMMAND"

#----------
test "Insert decimal and string" -noreset
current_obj AP format
poke 0 1 "dec=%d, str=%s\0"
current_obj AP string
poke 0 1 "Hello\0"
current_obj AP output
fill 0 1 100 0xAA
command 0 4321
expect "0000000001 SPRINTF_CALL  19 dec=4321, str=Hello"
expect "0000000002 Trg-Done QS_RX_COMMAND"

#----------
test "Overflow/underflow" -noreset
current_obj AP output
peek 26 1 4
expect "%timestamp Trg-Peek Offs=26,Size=1,Num=4,Data=<??,00,AA,AA>"
peek 0 1 10
expect "%timestamp Trg-Peek Offs=0,Size=1,Num=10,Data=<AA,AA,AA,AA,AA,AA,AA,AA,??,??>"

# the end
end


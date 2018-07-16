# QUTEST test script corresponding to the test_Flash.c test fixture.
# see https://www.state-machine.com/qtools/qutest.html
#
# This example corresponds to FlashTest.cpp test from Chapter 10 "The Mock
# Object" of the book: "Test-Driven Development for Embedded Systems" by
# James W. Grenning

# preamble...
proc on_setup {} {
    command FAKE_MICROTIME_INIT 0 1
    expect "%timestamp FAKE_MICROTIME_INIT"
    expect "%timestamp Trg-Done QS_RX_COMMAND"
}

set address [expr 0x1000]
set data    [expr 0xBEEF]

# constants from "Flash.h"
set FLASH_SUCCESS                0
set FLASH_VPP_ERROR              1
set FLASH_PROGRAM_ERROR          2
set FLASH_PROTECTED_BLOCK_ERROR  3
set FLASH_UNKNOWN_PROGRAM_ERROR  4
set FLASH_READ_BACK_ERROR        5
set FLASH_TIMEOUT_ERROR          6

# constants from "m28w160ect.h"
set CommandRegister         [expr 0x0]
set StatusRegister          [expr 0x0]

set ProgramCommand          [expr 0x40]
set Reset                   [expr 0xFF]

set ReadyBit                [expr 1<<7]
set EraseSuspendBit         [expr 1<<6]
set EraseErrorBit           [expr 1<<5]
set ProgramErrorBit         [expr 1<<4]
set VppErrorBit             [expr 1<<3]
set ProgramSuspendBit       [expr 1<<2]
set BlockProtectionErrorBit [expr 1<<1]
set ReservedBit             [expr 1<<0]

# tests...

#----------
test "Write success ready immediately"
probe IO_Read $ReadyBit
probe IO_Read $data
command FLASH_WRITE $address $data
expect "%timestamp IO_CALL IO_Write $CommandRegister $ProgramCommand"
expect "%timestamp IO_CALL IO_Write $address $data"
expect "%timestamp TstProbe Fun=IO_Read,Data=128"
expect "%timestamp IO_CALL IO_Read $ReadyBit $StatusRegister"
expect "%timestamp TstProbe Fun=IO_Read,Data=$data"
expect "%timestamp IO_CALL IO_Read [expr $data] $address"
expect "%timestamp FLASH_WRITE $FLASH_SUCCESS"
expect "%timestamp Trg-Done QS_RX_COMMAND"

#----------
test "Write success not immediately ready" -noreset
probe IO_Read 0
probe IO_Read 0
probe IO_Read 0
probe IO_Read $ReadyBit
probe IO_Read $data
command FLASH_WRITE $address $data
expect "%timestamp IO_CALL IO_Write $CommandRegister $ProgramCommand"
expect "%timestamp IO_CALL IO_Write $address $data"
expect "%timestamp TstProbe Fun=IO_Read,Data=0"
expect "%timestamp IO_CALL IO_Read 0 $StatusRegister"
expect "%timestamp TstProbe Fun=IO_Read,Data=0"
expect "%timestamp IO_CALL IO_Read 0 $StatusRegister"
expect "%timestamp TstProbe Fun=IO_Read,Data=0"
expect "%timestamp IO_CALL IO_Read 0 $StatusRegister"
expect "%timestamp TstProbe Fun=IO_Read,Data=128"
expect "%timestamp IO_CALL IO_Read $ReadyBit $StatusRegister"
expect "%timestamp TstProbe Fun=IO_Read,Data=$data"
expect "%timestamp IO_CALL IO_Read [expr $data] $address"
expect "%timestamp FLASH_WRITE $FLASH_SUCCESS"
expect "%timestamp Trg-Done QS_RX_COMMAND"

#----------
test "Write fails Vpp Error" -noreset
probe IO_Read [expr $ReadyBit | $VppErrorBit]
command FLASH_WRITE $address $data
expect "%timestamp IO_CALL IO_Write $CommandRegister $ProgramCommand"
expect "%timestamp IO_CALL IO_Write $address $data"
expect "%timestamp TstProbe Fun=IO_Read,Data=[expr $ReadyBit | $VppErrorBit]"
expect "%timestamp IO_CALL IO_Read [expr $ReadyBit | $VppErrorBit] $StatusRegister"
expect "%timestamp IO_CALL IO_Write $CommandRegister $Reset"
expect "%timestamp FLASH_WRITE $FLASH_VPP_ERROR"
expect "%timestamp Trg-Done QS_RX_COMMAND"

#----------
test "Write fails Program Error" -noreset
probe IO_Read [expr $ReadyBit | $ProgramErrorBit]
command FLASH_WRITE $address $data
expect "%timestamp IO_CALL IO_Write $CommandRegister $ProgramCommand"
expect "%timestamp IO_CALL IO_Write $address $data"
expect "%timestamp TstProbe Fun=IO_Read,Data=[expr $ReadyBit | $ProgramErrorBit]"
expect "%timestamp IO_CALL IO_Read [expr $ReadyBit | $ProgramErrorBit] $StatusRegister"
expect "%timestamp IO_CALL IO_Write $CommandRegister $Reset"
expect "%timestamp FLASH_WRITE $FLASH_PROGRAM_ERROR"
expect "%timestamp Trg-Done QS_RX_COMMAND"

#----------
test "Write fails Protected-Block Error" -noreset
probe IO_Read [expr $ReadyBit | $BlockProtectionErrorBit]
command FLASH_WRITE $address $data
expect "%timestamp IO_CALL IO_Write $CommandRegister $ProgramCommand"
expect "%timestamp IO_CALL IO_Write $address $data"
expect "%timestamp TstProbe Fun=IO_Read,Data=[expr $ReadyBit | $BlockProtectionErrorBit]"
expect "%timestamp IO_CALL IO_Read [expr $ReadyBit | $BlockProtectionErrorBit] $StatusRegister"
expect "%timestamp IO_CALL IO_Write $CommandRegister $Reset"
expect "%timestamp FLASH_WRITE $FLASH_PROTECTED_BLOCK_ERROR"
expect "%timestamp Trg-Done QS_RX_COMMAND"

#----------
test "Write fails Unknown Program Error" -noreset
probe IO_Read [expr $ReadyBit | $EraseSuspendBit | $EraseErrorBit | $ProgramSuspendBit | $ReservedBit]
command FLASH_WRITE $address $data
expect "%timestamp IO_CALL IO_Write $CommandRegister $ProgramCommand"
expect "%timestamp IO_CALL IO_Write $address $data"
expect "%timestamp TstProbe Fun=IO_Read,Data=[expr $ReadyBit | $EraseSuspendBit | $EraseErrorBit | $ProgramSuspendBit | $ReservedBit]"
expect "%timestamp IO_CALL IO_Read [expr $ReadyBit | $EraseSuspendBit | $EraseErrorBit | $ProgramSuspendBit | $ReservedBit] $StatusRegister"
expect "%timestamp IO_CALL IO_Write $CommandRegister $Reset"
expect "%timestamp FLASH_WRITE $FLASH_UNKNOWN_PROGRAM_ERROR"
expect "%timestamp Trg-Done QS_RX_COMMAND"

#----------
test "Write fails Read-Back Error" -noreset
probe IO_Read $ReadyBit
probe IO_Read [expr $data - 1]
command FLASH_WRITE $address $data
expect "%timestamp IO_CALL IO_Write $CommandRegister $ProgramCommand"
expect "%timestamp IO_CALL IO_Write $address $data"
expect "%timestamp TstProbe Fun=IO_Read,Data=[expr $ReadyBit]"
expect "%timestamp IO_CALL IO_Read $ReadyBit $StatusRegister"
expect "%timestamp TstProbe Fun=IO_Read,Data=[expr $data - 1]"
expect "%timestamp IO_CALL IO_Read [expr $data - 1] $address"
expect "%timestamp FLASH_WRITE $FLASH_READ_BACK_ERROR"
expect "%timestamp Trg-Done QS_RX_COMMAND"

#----------
test "Write sucess Ignores Other Bits Until Ready" -noreset
probe IO_Read [expr ~$ReadyBit]
probe IO_Read $ReadyBit
probe IO_Read $data
command FLASH_WRITE $address $data
expect "%timestamp IO_CALL IO_Write $CommandRegister $ProgramCommand"
expect "%timestamp IO_CALL IO_Write $address $data"
expect "%timestamp TstProbe Fun=IO_Read,Data=[expr ~$ReadyBit]"
expect "%timestamp IO_CALL IO_Read [expr ~$ReadyBit & 0xFFFF] $StatusRegister"
expect "%timestamp TstProbe Fun=IO_Read,Data=$ReadyBit"
expect "%timestamp IO_CALL IO_Read $ReadyBit $StatusRegister"
expect "%timestamp TstProbe Fun=IO_Read,Data=$data"
expect "%timestamp IO_CALL IO_Read $data $address"
expect "%timestamp FLASH_WRITE $FLASH_SUCCESS"
expect "%timestamp Trg-Done QS_RX_COMMAND"

#----------
test "Write fails Timeout" -noreset
command FAKE_MICROTIME_INIT 0 500
expect "%timestamp FAKE_MICROTIME_INIT"
expect "%timestamp Trg-Done QS_RX_COMMAND"
for {set i 0} {$i < 10} {incr i} {
    probe IO_Read [expr ~$ReadyBit]
}
command FLASH_WRITE $address $data
expect "%timestamp IO_CALL IO_Write $CommandRegister $ProgramCommand"
expect "%timestamp IO_CALL IO_Write $address $data"
for {set i 0} {$i < 10} {incr i} {
    expect "%timestamp TstProbe Fun=IO_Read,Data=[expr ~$ReadyBit]"
    expect "%timestamp IO_CALL IO_Read [expr ~$ReadyBit & 0xFFFF] $StatusRegister"
}
expect "%timestamp FLASH_WRITE $FLASH_TIMEOUT_ERROR"
expect "%timestamp Trg-Done QS_RX_COMMAND"

#----------
test "Write fails Timeout at End-Of-Time" -noreset
command FAKE_MICROTIME_INIT 0xFFFFFFFF 500
expect "%timestamp FAKE_MICROTIME_INIT"
expect "%timestamp Trg-Done QS_RX_COMMAND"
for {set i 0} {$i < 10} {incr i} {
    probe IO_Read [expr ~$ReadyBit]
}
command FLASH_WRITE $address $data
expect "%timestamp IO_CALL IO_Write $CommandRegister $ProgramCommand"
expect "%timestamp IO_CALL IO_Write $address $data"
for {set i 0} {$i < 10} {incr i} {
    expect "%timestamp TstProbe Fun=IO_Read,Data=[expr ~$ReadyBit]"
    expect "%timestamp IO_CALL IO_Read [expr ~$ReadyBit & 0xFFFF] $StatusRegister"
}
expect "%timestamp FLASH_WRITE $FLASH_TIMEOUT_ERROR"
expect "%timestamp Trg-Done QS_RX_COMMAND"


# the end
end


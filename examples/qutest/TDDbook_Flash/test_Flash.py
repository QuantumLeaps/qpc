# QUTEST test script corresponding to the test_Flash.c test fixture.
# see https://www.state-machine.com/qtools/qutest.html
#
# This example corresponds to FlashTest.cpp test from Chapter 10 "The Mock
# Object" of the book: "Test-Driven Development for Embedded Systems" by
# James W. Grenning

import sys
import pytest
import struct
from qspypy.qspy import FILTER, QS_OBJ_KIND

# Flash address and data used in this test group
address = 0x1000
data    = 0xBEEF

# constants from "Flash.h"
FLASH_SUCCESS               = 0
FLASH_VPP_ERROR             = 1
FLASH_PROGRAM_ERROR         = 2
FLASH_PROTECTED_BLOCK_ERROR = 3
FLASH_UNKNOWN_PROGRAM_ERROR = 4
FLASH_READ_BACK_ERROR       = 5
FLASH_TIMEOUT_ERROR         = 6

# constants from "m28w160ect.h"
CommandRegister         = 0x0
StatusRegister          = 0x0

ProgramCommand          = 0x40
Reset                   = 0xFF

ReadyBit                = 1<<7
EraseSuspendBit         = 1<<6
EraseErrorBit           = 1<<5
ProgramErrorBit         = 1<<4
VppErrorBit             = 1<<3
ProgramSuspendBit       = 1<<2
BlockProtectionErrorBit = 1<<1
ReservedBit             = 1<<0


def on_setup(qutest):
    qutest.command("FAKE_MICROTIME_INIT",0,1)
    qutest.expect("%timestamp FAKE_MICROTIME_INIT")
    qutest.expect("%timestamp Trg-Done QS_RX_COMMAND")


def test_Write_success_ready_immediately(qutest):
    qutest.probe("IO_Read",ReadyBit)
    qutest.probe("IO_Read",data)
    qutest.command("FLASH_WRITE",address,data)
    qutest.expect(f"%timestamp IO_CALL IO_Write {CommandRegister} {ProgramCommand}")
    qutest.expect(f"%timestamp IO_CALL IO_Write {address} {data}")
    qutest.expect(f"%timestamp TstProbe Fun=IO_Read,Data=128")
    qutest.expect(f"%timestamp IO_CALL IO_Read {ReadyBit} {StatusRegister}")
    qutest.expect(f"%timestamp TstProbe Fun=IO_Read,Data={data}")
    qutest.expect(f"%timestamp IO_CALL IO_Read {data} {address}")
    qutest.expect(f"%timestamp FLASH_WRITE {FLASH_SUCCESS}")
    qutest.expect(f"%timestamp Trg-Done QS_RX_COMMAND")

def test_Write_success_not_immediately_ready(qutest_noreset):
    qutest = qutest_noreset # name change
    qutest.probe("IO_Read",0)
    qutest.probe("IO_Read",0)
    qutest.probe("IO_Read",0)
    qutest.probe("IO_Read",ReadyBit)
    qutest.probe("IO_Read",data)
    qutest.command("FLASH_WRITE",address,data)
    qutest.expect(f"%timestamp IO_CALL IO_Write {CommandRegister} {ProgramCommand}")
    qutest.expect(f"%timestamp IO_CALL IO_Write {address} {data}")
    qutest.expect(f"%timestamp TstProbe Fun=IO_Read,Data=0")
    qutest.expect(f"%timestamp IO_CALL IO_Read 0 {StatusRegister}")
    qutest.expect(f"%timestamp TstProbe Fun=IO_Read,Data=0")
    qutest.expect(f"%timestamp IO_CALL IO_Read 0 {StatusRegister}")
    qutest.expect(f"%timestamp TstProbe Fun=IO_Read,Data=0")
    qutest.expect(f"%timestamp IO_CALL IO_Read 0 {StatusRegister}")
    qutest.expect(f"%timestamp TstProbe Fun=IO_Read,Data=128")
    qutest.expect(f"%timestamp IO_CALL IO_Read {ReadyBit} {StatusRegister}")
    qutest.expect(f"%timestamp TstProbe Fun=IO_Read,Data={data}")
    qutest.expect(f"%timestamp IO_CALL IO_Read {data} {address}")
    qutest.expect(f"%timestamp FLASH_WRITE {FLASH_SUCCESS}")
    qutest.expect(f"%timestamp Trg-Done QS_RX_COMMAND")

def test_Write_fails_Vpp_Error(qutest_noreset):
    qutest = qutest_noreset # name change
    qutest.probe("IO_Read",ReadyBit | VppErrorBit)
    qutest.command("FLASH_WRITE",address,data)
    qutest.expect(f"%timestamp IO_CALL IO_Write {CommandRegister} {ProgramCommand}")
    qutest.expect(f"%timestamp IO_CALL IO_Write {address} {data}")
    qutest.expect(f"%timestamp TstProbe Fun=IO_Read,Data={ReadyBit|VppErrorBit}")
    qutest.expect(f"%timestamp IO_CALL IO_Read {ReadyBit|VppErrorBit} {StatusRegister}")
    qutest.expect(f"%timestamp IO_CALL IO_Write {CommandRegister} {Reset}")
    qutest.expect(f"%timestamp FLASH_WRITE {FLASH_VPP_ERROR}")
    qutest.expect(f"%timestamp Trg-Done QS_RX_COMMAND")

def test_Write_fails_Program_Error(qutest_noreset):
    qutest = qutest_noreset # name change
    qutest.probe("IO_Read",ReadyBit|ProgramErrorBit)
    qutest.command("FLASH_WRITE",address,data)
    qutest.expect(f"%timestamp IO_CALL IO_Write {CommandRegister} {ProgramCommand}")
    qutest.expect(f"%timestamp IO_CALL IO_Write {address} {data}")
    qutest.expect(f"%timestamp TstProbe Fun=IO_Read,Data={ReadyBit|ProgramErrorBit}")
    qutest.expect(f"%timestamp IO_CALL IO_Read {ReadyBit|ProgramErrorBit} {StatusRegister}")
    qutest.expect(f"%timestamp IO_CALL IO_Write {CommandRegister} {Reset}")
    qutest.expect(f"%timestamp FLASH_WRITE {FLASH_PROGRAM_ERROR}")
    qutest.expect(f"%timestamp Trg-Done QS_RX_COMMAND")

def test_Write_fails_Protected_Block_Error(qutest_noreset):
    qutest = qutest_noreset # name change
    qutest.probe("IO_Read",ReadyBit|BlockProtectionErrorBit)
    qutest.command("FLASH_WRITE",address,data)
    qutest.expect(f"%timestamp IO_CALL IO_Write {CommandRegister} {ProgramCommand}")
    qutest.expect(f"%timestamp IO_CALL IO_Write {address} {data}")
    qutest.expect(f"%timestamp TstProbe Fun=IO_Read,Data={ReadyBit|BlockProtectionErrorBit}")
    qutest.expect(f"%timestamp IO_CALL IO_Read {ReadyBit|BlockProtectionErrorBit} {StatusRegister}")
    qutest.expect(f"%timestamp IO_CALL IO_Write {CommandRegister} {Reset}")
    qutest.expect(f"%timestamp FLASH_WRITE {FLASH_PROTECTED_BLOCK_ERROR}")
    qutest.expect(f"%timestamp Trg-Done QS_RX_COMMAND")

def test_Write_fails_Unknown_Program_Error(qutest_noreset):
    qutest = qutest_noreset # name change
    qutest.probe("IO_Read",ReadyBit|EraseSuspendBit|EraseErrorBit|ProgramSuspendBit|ReservedBit)
    qutest.command("FLASH_WRITE",address,data)
    qutest.expect(f"%timestamp IO_CALL IO_Write {CommandRegister} {ProgramCommand}")
    qutest.expect(f"%timestamp IO_CALL IO_Write {address} {data}")
    qutest.expect(f"%timestamp TstProbe Fun=IO_Read,Data={ReadyBit|EraseSuspendBit|EraseErrorBit|ProgramSuspendBit|ReservedBit}")
    qutest.expect(f"%timestamp IO_CALL IO_Read {ReadyBit|EraseSuspendBit|EraseErrorBit|ProgramSuspendBit|ReservedBit} {StatusRegister}")
    qutest.expect(f"%timestamp IO_CALL IO_Write {CommandRegister} {Reset}")
    qutest.expect(f"%timestamp FLASH_WRITE {FLASH_UNKNOWN_PROGRAM_ERROR}")
    qutest.expect(f"%timestamp Trg-Done QS_RX_COMMAND")

def test_Write_fails_Read_Back_Error(qutest_noreset):
    qutest = qutest_noreset # name change
    qutest.probe("IO_Read",ReadyBit)
    qutest.probe("IO_Read",data-1)
    qutest.command("FLASH_WRITE",address,data)
    qutest.expect(f"%timestamp IO_CALL IO_Write {CommandRegister} {ProgramCommand}")
    qutest.expect(f"%timestamp IO_CALL IO_Write {address} {data}")
    qutest.expect(f"%timestamp TstProbe Fun=IO_Read,Data={ReadyBit}")
    qutest.expect(f"%timestamp IO_CALL IO_Read {ReadyBit} {StatusRegister}")
    qutest.expect(f"%timestamp TstProbe Fun=IO_Read,Data={data-1}")
    qutest.expect(f"%timestamp IO_CALL IO_Read {data-1} {address}")
    qutest.expect(f"%timestamp FLASH_WRITE {FLASH_READ_BACK_ERROR}")
    qutest.expect(f"%timestamp Trg-Done QS_RX_COMMAND")

def test_Write_sucess_Ignores_Other_Bits_Until_Ready(qutest_noreset):
    qutest = qutest_noreset # name change
    qutest.probe("IO_Read",~ReadyBit&0xFFFF)
    qutest.probe("IO_Read",ReadyBit)
    qutest.probe("IO_Read",data)
    qutest.command("FLASH_WRITE",address,data)
    qutest.expect(f"%timestamp IO_CALL IO_Write {CommandRegister} {ProgramCommand}")
    qutest.expect(f"%timestamp IO_CALL IO_Write {address} {data}")
    qutest.expect(f"%timestamp TstProbe Fun=IO_Read,Data={~ReadyBit&0xFFFF}")
    qutest.expect(f"%timestamp IO_CALL IO_Read {~ReadyBit&0xFFFF} {StatusRegister}")
    qutest.expect(f"%timestamp TstProbe Fun=IO_Read,Data={ReadyBit}")
    qutest.expect(f"%timestamp IO_CALL IO_Read {ReadyBit} {StatusRegister}")
    qutest.expect(f"%timestamp TstProbe Fun=IO_Read,Data={data}")
    qutest.expect(f"%timestamp IO_CALL IO_Read {data} {address}")
    qutest.expect(f"%timestamp FLASH_WRITE {FLASH_SUCCESS}")
    qutest.expect(f"%timestamp Trg-Done QS_RX_COMMAND")

def test_Write_fails_Timeout(qutest_noreset):
    qutest = qutest_noreset # name change
    qutest.command("FAKE_MICROTIME_INIT",0,500)
    qutest.expect("%timestamp FAKE_MICROTIME_INIT")
    qutest.expect("%timestamp Trg-Done QS_RX_COMMAND")
    for i in range(10):
        qutest.probe("IO_Read",~ReadyBit&0xFFFF)
    qutest.command("FLASH_WRITE",address,data)
    qutest.expect(f"%timestamp IO_CALL IO_Write {CommandRegister} {ProgramCommand}")
    qutest.expect(f"%timestamp IO_CALL IO_Write {address} {data}")
    for i in range(10):
        qutest.expect(f"%timestamp TstProbe Fun=IO_Read,Data={~ReadyBit&0xFFFF}")
        qutest.expect(f"%timestamp IO_CALL IO_Read {~ReadyBit&0xFFFF} {StatusRegister}")
    qutest.expect(f"%timestamp FLASH_WRITE {FLASH_TIMEOUT_ERROR}")
    qutest.expect(f"%timestamp Trg-Done QS_RX_COMMAND")

def test_Write_fails_Timeout_at_End_Of_Time(qutest_noreset):
    qutest = qutest_noreset # name change
    qutest.command("FAKE_MICROTIME_INIT",0xFFFFFFFF,500)
    qutest.expect("%timestamp FAKE_MICROTIME_INIT")
    qutest.expect("%timestamp Trg-Done QS_RX_COMMAND")
    for i in range(10):
        qutest.probe("IO_Read",~ReadyBit&0xFFFF)
    qutest.command("FLASH_WRITE",address,data)
    qutest.expect(f"%timestamp IO_CALL IO_Write {CommandRegister} {ProgramCommand}")
    qutest.expect(f"%timestamp IO_CALL IO_Write {address} {data}")
    for i in range(10):
        qutest.expect(f"%timestamp TstProbe Fun=IO_Read,Data={~ReadyBit&0xFFFF}")
        qutest.expect(f"%timestamp IO_CALL IO_Read {~ReadyBit&0xFFFF} {StatusRegister}")
    qutest.expect(f"%timestamp FLASH_WRITE {FLASH_TIMEOUT_ERROR}")
    qutest.expect(f"%timestamp Trg-Done QS_RX_COMMAND")


if __name__ == "__main__":
    options = ['-x', '-v', '--tb=short']
    options.extend(sys.argv)
    pytest.main(options)

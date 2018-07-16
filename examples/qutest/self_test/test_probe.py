# test-script for QUTest unit testing harness
# see https://www.state-machine.com/qtools/qutest.html

import sys
import pytest
import struct
from qspypy.qspy import FILTER, QS_OBJ_KIND


def on_setup(qutest):
    qutest.expect("%timestamp ON_TEST_SETUP")

def on_teardown(qutest):
    qutest.expect("%timestamp ON_TEST_TEARDOWN")


def test_Single_Test_Probe(qutest):
    qutest.command("COMMAND_X")
    qutest.expect("%timestamp COMMAND_X 0")
    qutest.expect("%timestamp Trg-Done QS_RX_COMMAND")
    qutest.probe("myFun",1)
    qutest.command("COMMAND_X")
    qutest.expect("%timestamp TstProbe Fun=myFun,Data=1")
    qutest.expect("%timestamp COMMAND_X 1")
    qutest.expect("%timestamp Trg-Done QS_RX_COMMAND")
    qutest.command("COMMAND_X")
    qutest.expect("%timestamp COMMAND_X 0")
    qutest.expect("%timestamp Trg-Done QS_RX_COMMAND")

def test_Multiple_Test_Probes(qutest_noreset):
    qutest = qutest_noreset # name change
    qutest.probe("myFun",100022)
    qutest.probe("myFun",200033)
    qutest.command("COMMAND_X")
    qutest.expect("%timestamp TstProbe Fun=myFun,Data=100022")
    qutest.expect("%timestamp COMMAND_X 100022")
    qutest.expect("%timestamp Trg-Done QS_RX_COMMAND")
    qutest.command("COMMAND_X")
    qutest.expect("%timestamp TstProbe Fun=myFun,Data=200033")
    qutest.expect("%timestamp COMMAND_X 200033")
    qutest.expect("%timestamp Trg-Done QS_RX_COMMAND")
    qutest.command("COMMAND_X")
    qutest.expect("%timestamp COMMAND_X 0")
    qutest.expect("%timestamp Trg-Done QS_RX_COMMAND")


if __name__ == "__main__":
    options = ['-x', '-v', '--tb=short']
    options.extend(sys.argv)
    pytest.main(options)

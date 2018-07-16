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



def test_Command(qutest):
    qutest.command("COMMAND_X",1,2,3)
    qutest.expect("%timestamp COMMAND_X 0")
    qutest.expect("%timestamp Trg-Done QS_RX_COMMAND")


if __name__ == "__main__":
    options = ['-x', '-v', '--tb=short']
    options.extend(sys.argv)
    pytest.main(options)


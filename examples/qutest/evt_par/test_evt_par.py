# test-script for QUTest unit testing harness
# see https://www.state-machine.com/qtools/qutest.html

import sys
import pytest
import struct
from qspypy.qspy import FILTER, QS_OBJ_KIND


def on_reset(qutest):
    qutest.expect_pause()
    qutest.glb_filter(FILTER.UA)
    qutest.current_obj(QS_OBJ_KIND.SM_AO,"AO_MyAO")
    qutest.Continue()


def test_EVT0(qutest):
    qutest.dispatch("MY_EVT0_SIG")
    qutest.expect("%timestamp MY_EVT0")
    qutest.expect("%timestamp Trg-Done QS_RX_EVENT")

def test_EVT1(qutest):
    qutest.dispatch("MY_EVT1_SIG",struct.pack('<I', 12345))
    qutest.expect("%timestamp MY_EVT1 12345")
    qutest.expect("%timestamp Trg-Done QS_RX_EVENT")

def test_EVT2(qutest):
    qutest.dispatch("MY_EVT2_SIG",struct.pack('<IH',0x1234,678))
    qutest.expect("%timestamp MY_EVT2 4660 678")
    qutest.expect("%timestamp Trg-Done QS_RX_EVENT")

def test_EVT2(qutest):
    qutest.dispatch("MY_EVT3_SIG",struct.pack('<IHB',87654,0xdead,123))
    qutest.expect("%timestamp MY_EVT3 87654 57005 123")
    qutest.expect("%timestamp Trg-Done QS_RX_EVENT")



if __name__ == "__main__":
    options = ['-x', '-v', '--tb=short']
    options.extend(sys.argv)
    pytest.main(options)

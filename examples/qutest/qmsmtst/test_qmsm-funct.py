# test-script for QUTest unit testing harness
# see https://www.state-machine.com/qtools/qutest.html

import sys
import pytest
import struct
from qspypy.qspy import FILTER, QS_OBJ_KIND


def on_reset(qutest):
    qutest.glb_filter(FILTER.UA)
    qutest.current_obj(QS_OBJ_KIND.SM,"the_msm")


def test_QMsmTst_init(qutest):
    qutest.init()
    qutest.expect("%timestamp BSP_DISPLAY top-INIT;")
    qutest.expect("%timestamp BSP_DISPLAY s-ENTRY;")
    qutest.expect("%timestamp BSP_DISPLAY s2-ENTRY;")
    qutest.expect("%timestamp BSP_DISPLAY s2-INIT;")
    qutest.expect("%timestamp BSP_DISPLAY s21-ENTRY;")
    qutest.expect("%timestamp BSP_DISPLAY s211-ENTRY;")
    qutest.expect("%timestamp Trg-Done QS_RX_EVENT")

def test_QMsmTst_dispatch(qutest_noreset):
    qutest = qutest_noreset # name change

    qutest.dispatch("A_SIG")
    qutest.expect("%timestamp BSP_DISPLAY s21-A;")
    qutest.expect("%timestamp BSP_DISPLAY s211-EXIT;")
    qutest.expect("%timestamp BSP_DISPLAY s21-EXIT;")
    qutest.expect("%timestamp BSP_DISPLAY s21-ENTRY;")
    qutest.expect("%timestamp BSP_DISPLAY s21-INIT;")
    qutest.expect("%timestamp BSP_DISPLAY s211-ENTRY;")
    qutest.expect("%timestamp Trg-Done QS_RX_EVENT")

    qutest.dispatch("B_SIG")
    qutest.expect("%timestamp BSP_DISPLAY s21-B;")
    qutest.expect("%timestamp BSP_DISPLAY s211-EXIT;")
    qutest.expect("%timestamp BSP_DISPLAY s211-ENTRY;")
    qutest.expect("%timestamp Trg-Done QS_RX_EVENT")

    qutest.dispatch("D_SIG")
    qutest.expect("%timestamp BSP_DISPLAY s211-D;")
    qutest.expect("%timestamp BSP_DISPLAY s211-EXIT;")
    qutest.expect("%timestamp BSP_DISPLAY s21-INIT;")
    qutest.expect("%timestamp BSP_DISPLAY s211-ENTRY;")
    qutest.expect("%timestamp Trg-Done QS_RX_EVENT")

    qutest.dispatch("E_SIG")
    qutest.expect("%timestamp BSP_DISPLAY s-E;")
    qutest.expect("%timestamp BSP_DISPLAY s211-EXIT;")
    qutest.expect("%timestamp BSP_DISPLAY s21-EXIT;")
    qutest.expect("%timestamp BSP_DISPLAY s2-EXIT;")
    qutest.expect("%timestamp BSP_DISPLAY s1-ENTRY;")
    qutest.expect("%timestamp BSP_DISPLAY s11-ENTRY;")
    qutest.expect("%timestamp Trg-Done QS_RX_EVENT")

    qutest.dispatch("I_SIG")
    qutest.expect("%timestamp BSP_DISPLAY s1-I;")
    qutest.expect("%timestamp Trg-Done QS_RX_EVENT")

    qutest.dispatch("F_SIG")
    qutest.expect("%timestamp BSP_DISPLAY s1-F;")
    qutest.expect("%timestamp BSP_DISPLAY s11-EXIT;")
    qutest.expect("%timestamp BSP_DISPLAY s1-EXIT;")
    qutest.expect("%timestamp BSP_DISPLAY s2-ENTRY;")
    qutest.expect("%timestamp BSP_DISPLAY s21-ENTRY;")
    qutest.expect("%timestamp BSP_DISPLAY s211-ENTRY;")
    qutest.expect("%timestamp Trg-Done QS_RX_EVENT")

    qutest.dispatch("I_SIG")
    qutest.expect("%timestamp BSP_DISPLAY s2-I;")
    qutest.expect("%timestamp Trg-Done QS_RX_EVENT")

    qutest.dispatch("I_SIG")
    qutest.expect("%timestamp BSP_DISPLAY s-I;")
    qutest.expect("%timestamp Trg-Done QS_RX_EVENT")

    qutest.dispatch("F_SIG")
    qutest.expect("%timestamp BSP_DISPLAY s2-F;")
    qutest.expect("%timestamp BSP_DISPLAY s211-EXIT;")
    qutest.expect("%timestamp BSP_DISPLAY s21-EXIT;")
    qutest.expect("%timestamp BSP_DISPLAY s2-EXIT;")
    qutest.expect("%timestamp BSP_DISPLAY s1-ENTRY;")
    qutest.expect("%timestamp BSP_DISPLAY s11-ENTRY;")
    qutest.expect("%timestamp Trg-Done QS_RX_EVENT")

    qutest.dispatch("A_SIG")
    qutest.expect("%timestamp BSP_DISPLAY s1-A;")
    qutest.expect("%timestamp BSP_DISPLAY s11-EXIT;")
    qutest.expect("%timestamp BSP_DISPLAY s1-EXIT;")
    qutest.expect("%timestamp BSP_DISPLAY s1-ENTRY;")
    qutest.expect("%timestamp BSP_DISPLAY s1-INIT;")
    qutest.expect("%timestamp BSP_DISPLAY s11-ENTRY;")
    qutest.expect("%timestamp Trg-Done QS_RX_EVENT")

    qutest.dispatch("B_SIG")
    qutest.expect("%timestamp BSP_DISPLAY s1-B;")
    qutest.expect("%timestamp BSP_DISPLAY s11-EXIT;")
    qutest.expect("%timestamp BSP_DISPLAY s11-ENTRY;")
    qutest.expect("%timestamp Trg-Done QS_RX_EVENT")

    qutest.dispatch("D_SIG")
    qutest.expect("%timestamp BSP_DISPLAY s1-D;")
    qutest.expect("%timestamp BSP_DISPLAY s11-EXIT;")
    qutest.expect("%timestamp BSP_DISPLAY s1-EXIT;")
    qutest.expect("%timestamp BSP_DISPLAY s-INIT;")
    qutest.expect("%timestamp BSP_DISPLAY s1-ENTRY;")
    qutest.expect("%timestamp BSP_DISPLAY s11-ENTRY;")
    qutest.expect("%timestamp Trg-Done QS_RX_EVENT")

    qutest.dispatch("D_SIG")
    qutest.expect("%timestamp BSP_DISPLAY s11-D;")
    qutest.expect("%timestamp BSP_DISPLAY s11-EXIT;")
    qutest.expect("%timestamp BSP_DISPLAY s1-INIT;")
    qutest.expect("%timestamp BSP_DISPLAY s11-ENTRY;")
    qutest.expect("%timestamp Trg-Done QS_RX_EVENT")

    qutest.dispatch("E_SIG")
    qutest.expect("%timestamp BSP_DISPLAY s-E;")
    qutest.expect("%timestamp BSP_DISPLAY s11-EXIT;")
    qutest.expect("%timestamp BSP_DISPLAY s1-EXIT;")
    qutest.expect("%timestamp BSP_DISPLAY s1-ENTRY;")
    qutest.expect("%timestamp BSP_DISPLAY s11-ENTRY;")
    qutest.expect("%timestamp Trg-Done QS_RX_EVENT")

    qutest.dispatch("G_SIG")
    qutest.expect("%timestamp BSP_DISPLAY s11-G;")
    qutest.expect("%timestamp BSP_DISPLAY s11-EXIT;")
    qutest.expect("%timestamp BSP_DISPLAY s1-EXIT;")
    qutest.expect("%timestamp BSP_DISPLAY s2-ENTRY;")
    qutest.expect("%timestamp BSP_DISPLAY s21-ENTRY;")
    qutest.expect("%timestamp BSP_DISPLAY s211-ENTRY;")
    qutest.expect("%timestamp Trg-Done QS_RX_EVENT")

    qutest.dispatch("H_SIG")
    qutest.expect("%timestamp BSP_DISPLAY s211-H;")
    qutest.expect("%timestamp BSP_DISPLAY s211-EXIT;")
    qutest.expect("%timestamp BSP_DISPLAY s21-EXIT;")
    qutest.expect("%timestamp BSP_DISPLAY s2-EXIT;")
    qutest.expect("%timestamp BSP_DISPLAY s-INIT;")
    qutest.expect("%timestamp BSP_DISPLAY s1-ENTRY;")
    qutest.expect("%timestamp BSP_DISPLAY s11-ENTRY;")
    qutest.expect("%timestamp Trg-Done QS_RX_EVENT")

    qutest.dispatch("H_SIG")
    qutest.expect("%timestamp BSP_DISPLAY s11-H;")
    qutest.expect("%timestamp BSP_DISPLAY s11-EXIT;")
    qutest.expect("%timestamp BSP_DISPLAY s1-EXIT;")
    qutest.expect("%timestamp BSP_DISPLAY s-INIT;")
    qutest.expect("%timestamp BSP_DISPLAY s1-ENTRY;")
    qutest.expect("%timestamp BSP_DISPLAY s11-ENTRY;")
    qutest.expect("%timestamp Trg-Done QS_RX_EVENT")

    qutest.dispatch("C_SIG")
    qutest.expect("%timestamp BSP_DISPLAY s1-C;")
    qutest.expect("%timestamp BSP_DISPLAY s11-EXIT;")
    qutest.expect("%timestamp BSP_DISPLAY s1-EXIT;")
    qutest.expect("%timestamp BSP_DISPLAY s2-ENTRY;")
    qutest.expect("%timestamp BSP_DISPLAY s2-INIT;")
    qutest.expect("%timestamp BSP_DISPLAY s21-ENTRY;")
    qutest.expect("%timestamp BSP_DISPLAY s211-ENTRY;")
    qutest.expect("%timestamp Trg-Done QS_RX_EVENT")

    qutest.dispatch("G_SIG")
    qutest.expect("%timestamp BSP_DISPLAY s21-G;")
    qutest.expect("%timestamp BSP_DISPLAY s211-EXIT;")
    qutest.expect("%timestamp BSP_DISPLAY s21-EXIT;")
    qutest.expect("%timestamp BSP_DISPLAY s2-EXIT;")
    qutest.expect("%timestamp BSP_DISPLAY s1-ENTRY;")
    qutest.expect("%timestamp BSP_DISPLAY s1-INIT;")
    qutest.expect("%timestamp BSP_DISPLAY s11-ENTRY;")
    qutest.expect("%timestamp Trg-Done QS_RX_EVENT")

    qutest.dispatch("C_SIG")
    qutest.expect("%timestamp BSP_DISPLAY s1-C;")
    qutest.expect("%timestamp BSP_DISPLAY s11-EXIT;")
    qutest.expect("%timestamp BSP_DISPLAY s1-EXIT;")
    qutest.expect("%timestamp BSP_DISPLAY s2-ENTRY;")
    qutest.expect("%timestamp BSP_DISPLAY s2-INIT;")
    qutest.expect("%timestamp BSP_DISPLAY s21-ENTRY;")
    qutest.expect("%timestamp BSP_DISPLAY s211-ENTRY;")
    qutest.expect("%timestamp Trg-Done QS_RX_EVENT")

    qutest.dispatch("C_SIG")
    qutest.expect("%timestamp BSP_DISPLAY s2-C;")
    qutest.expect("%timestamp BSP_DISPLAY s211-EXIT;")
    qutest.expect("%timestamp BSP_DISPLAY s21-EXIT;")
    qutest.expect("%timestamp BSP_DISPLAY s2-EXIT;")
    qutest.expect("%timestamp BSP_DISPLAY s1-ENTRY;")
    qutest.expect("%timestamp BSP_DISPLAY s1-INIT;")
    qutest.expect("%timestamp BSP_DISPLAY s11-ENTRY;")
    qutest.expect("%timestamp Trg-Done QS_RX_EVENT")



if __name__ == "__main__":
    options = ['-x', '-v', '--tb=short']
    options.extend(sys.argv)
    pytest.main(options)

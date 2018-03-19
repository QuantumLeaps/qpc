#-----------------------------------------------------------------------------
# Product: QSPY -- test-script example for qutest.tcl
# Last updated for version 6.2.0
# Last updated on  2018-03-19
#
#                    Q u a n t u m     L e a P s
#                    ---------------------------
#                    innovating embedded systems
#
# Copyright (C) 20017-2018 Quantum Leaps, LLC, All rights reserved.
#
# This program is open source software: you can redistribute it and/or
# modify it under the terms of the GNU General Public License as published
# by the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# Alternatively, this program may be distributed and modified under the
# terms of Quantum Leaps commercial licenses, which expressly supersede
# the GNU General Public License and are specifically designed for
# licensees interested in retaining the proprietary status of their code.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program. If not, see <http://www.gnu.org/licenses/>.
#
# Contact information:
# https://www.state-machine.com
# mailto:info@state-machine.com
#-----------------------------------------------------------------------------

# preamble...
proc on_reset {} {
    expect_pause
    continue
    glb_filter SM AO
    current_obj SM_AO l_philo<2>
}

# tests...
test "TIMEOUT->Philo (post)"
post TIMEOUT_SIG
expect "%timestamp AO-Post  Sdr=QS_RX,Obj=l_philo<2>,Evt<Sig=TIMEOUT_SIG,*"
expect "%timestamp AO-GetL  Obj=l_philo<2>,Evt<Sig=TIMEOUT_SIG,*"
expect "%timestamp Disp===> Obj=l_philo<2>,Sig=TIMEOUT_SIG,State=Philo_thinking"
expect "===RTC===> St-Exit  Obj=l_philo<2>,State=Philo_thinking"
expect "%timestamp AO-Post  Sdr=l_philo<2>,Obj=l_table,Evt<Sig=HUNGRY_SIG,*"
expect "===RTC===> St-Entry Obj=l_philo<2>,State=Philo_hungry"
expect "%timestamp ===>Tran Obj=l_philo<2>,Sig=TIMEOUT_SIG,State=Philo_thinking->Philo_hungry"
expect "%timestamp Trg-Done QS_RX_EVENT"

test "publish EAT(2)" -noreset
loc_filter SM_AO l_philo<2>
publish EAT_SIG [binary format c 2]
expect "%timestamp AO-Post  Sdr=QS_RX,Obj=l_philo<2>,Evt<Sig=EAT_SIG,*"
expect "%timestamp Trg-Done QS_RX_EVENT"
expect "%timestamp AO-GetL  Obj=l_philo<2>,Evt<Sig=EAT_SIG,*"
expect "%timestamp Disp===> Obj=l_philo<2>,Sig=EAT_SIG,State=Philo_hungry"
expect "===RTC===> St-Entry Obj=l_philo<2>,State=Philo_eating"
expect "%timestamp ===>Tran Obj=l_philo<2>,Sig=EAT_SIG,State=Philo_hungry->Philo_eating"
expect "%timestamp Trg-Done QS_RX_EVENT"

test "TIMEOUT->Philo_thinking (ASSERT)"
probe QActive_post_ 1
dispatch TIMEOUT_SIG
expect "%timestamp Disp===> Obj=l_philo<2>,Sig=TIMEOUT_SIG,State=Philo_thinking"
expect "===RTC===> St-Exit  Obj=l_philo<2>,State=Philo_thinking"
expect "%timestamp TstProbe Fun=QActive_post_,Data=1"
expect "%timestamp =ASSERT= Mod=qf_actq,Loc=110"

test "TIMEOUT->Philo_eating (PUBLISH from AO)"
glb_filter OFF
dispatch TIMEOUT_SIG
expect "%timestamp Trg-Done QS_RX_EVENT"
dispatch EAT_SIG [binary format c 2]
expect "%timestamp Trg-Done QS_RX_EVENT"
glb_filter SM AO QF
dispatch TIMEOUT_SIG
expect "%timestamp QF-New   Sig=TIMEOUT_SIG,*"
expect "%timestamp Disp===> Obj=l_philo<2>,Sig=TIMEOUT_SIG,State=Philo_eating"
expect "%timestamp QF-New   Sig=DONE_SIG,*"
expect "%timestamp QF-Pub   Sdr=l_philo<2>,Evt<Sig=DONE_SIG,Pool=1,Ref=0>"
expect "%timestamp AO-Post  Sdr=l_philo<2>,Obj=l_table,Evt<Sig=DONE_SIG,Pool=1,Ref=2>,*"
expect "%timestamp QF-gcA   Evt<Sig=DONE_SIG,Pool=1,Ref=2>"
expect "===RTC===> St-Exit  Obj=l_philo<2>,State=Philo_eating"
expect "===RTC===> St-Entry Obj=l_philo<2>,State=Philo_thinking"
expect "%timestamp ===>Tran Obj=l_philo<2>,Sig=TIMEOUT_SIG,State=Philo_eating->Philo_thinking"
expect "%timestamp QF-gc    Evt<Sig=TIMEOUT_SIG,Pool=1,Ref=1>"
expect "%timestamp Trg-Done QS_RX_EVENT"

test "timeEvt->Philo (tick)"
glb_filter SM AO TE
current_obj TE l_philo<2>.timeEvt
tick
expect "           TE0-ADis Obj=l_philo<2>.timeEvt,AO=l_philo<2>"
expect "%timestamp TE0-Post Obj=l_philo<2>.timeEvt,Sig=TIMEOUT_SIG,AO=l_philo<2>"
expect "%timestamp AO-Post  Sdr=QS_RX,Obj=l_philo<2>,Evt<Sig=TIMEOUT_SIG*"
expect "%timestamp AO-GetL  Obj=l_philo<2>,Evt<Sig=TIMEOUT_SIG*"
expect "%timestamp Disp===> Obj=l_philo<2>,Sig=TIMEOUT_SIG,State=Philo_thinking"
expect "%timestamp TE0-DisA Obj=l_philo<2>.timeEvt,AO=l_philo<2>"
expect "===RTC===> St-Exit  Obj=l_philo<2>,State=Philo_thinking"
expect "%timestamp AO-Post  Sdr=l_philo<2>,Obj=l_table,Evt<Sig=HUNGRY_SIG*"
expect "===RTC===> St-Entry Obj=l_philo<2>,State=Philo_hungry"
expect "%timestamp ===>Tran Obj=l_philo<2>,Sig=TIMEOUT_SIG,State=Philo_thinking->Philo_hungry"
expect "%timestamp Trg-Done QS_RX_TICK"

# the end
end

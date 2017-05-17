#-----------------------------------------------------------------------------
# Product: QSPY -- test-script example for qutest.tcl
# Last updated for version 5.9.0
# Last updated on  2017-05-17
#
#                    Q u a n t u m     L e a P s
#                    ---------------------------
#                    innovating embedded systems
#
# Copyright (C) Quantum Leaps, LLC, All rights reserved.
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
# https://state-machine.com
# mailto:info@state-machine.com
#-----------------------------------------------------------------------------

# preamble...
proc on_reset {} {
    #puts on_reset
    expect_pause
    continue
    glb_filter SM AO
    current_obj SM_AO AO_Philo<2>
}

# tests...
test "TIMEOUT->Philo (dispatch)"
dispatch TIMEOUT_SIG
expect "%timestamp Disp===> Obj=AO_Philo<2>,Sig=TIMEOUT_SIG,State=Philo_thinking"
expect "===RTC===> St-Exit  Obj=AO_Philo<2>,State=Philo_thinking"
expect "%timestamp AO-Post  Sdr=AO_Philo<2>,Obj=AO_Table,Evt<Sig=HUNGRY_SIG,*"
expect "===RTC===> St-Entry Obj=AO_Philo<2>,State=Philo_hungry"
expect "%timestamp ===>Tran Obj=AO_Philo<2>,Sig=TIMEOUT_SIG,State=Philo_thinking->Philo_hungry"
expect "%timestamp Trg-Done QS_RX_EVENT"

test "TIMEOUT->Philo (assert)"
probe QActive_post_ 1
dispatch TIMEOUT_SIG
expect "%timestamp Disp===> Obj=AO_Philo<2>,Sig=TIMEOUT_SIG,State=Philo_thinking"
expect "===RTC===> St-Exit  Obj=AO_Philo<2>,State=Philo_thinking"
expect "%timestamp TstProbe Fun=QActive_post_,Data=1"
expect "%timestamp =ASSERT= Mod=qutest,Loc=110"

test "timeEvt->Philo (tick)"
glb_filter SM AO TE
current_obj TE l_philo<2>.timeEvt
tick
expect "           TE0-ADis Obj=l_philo<2>.timeEvt,AO=AO_Philo<2>"
expect "%timestamp TE0-Post Obj=l_philo<2>.timeEvt,Sig=TIMEOUT_SIG,AO=AO_Philo<2>"
expect "%timestamp AO-Post  Sdr=QS_RX,Obj=AO_Philo<2>,Evt<Sig=TIMEOUT_SIG*"
expect "%timestamp Disp===> Obj=AO_Philo<2>,Sig=TIMEOUT_SIG,State=Philo_thinking"
expect "%timestamp TE0-DisA Obj=l_philo<2>.timeEvt,AO=AO_Philo<2>"
expect "===RTC===> St-Exit  Obj=AO_Philo<2>,State=Philo_thinking"
expect "%timestamp AO-Post  Sdr=AO_Philo<2>,Obj=AO_Table,Evt<Sig=HUNGRY_SIG*"
expect "===RTC===> St-Entry Obj=AO_Philo<2>,State=Philo_hungry"
expect "%timestamp ===>Tran Obj=AO_Philo<2>,Sig=TIMEOUT_SIG,State=Philo_thinking->Philo_hungry"
expect "%timestamp Trg-Done QS_RX_TICK"

# the end
end

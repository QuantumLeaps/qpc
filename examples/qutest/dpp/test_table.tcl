#-----------------------------------------------------------------------------
# Product: QSPY -- test-script example for qutest.tcl
# Last updated for version 6.2.0
# Last updated on  2018-03-19
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
# https://www.state-machine.com
# mailto:info@state-machine.com
#-----------------------------------------------------------------------------

# preamble...
proc on_reset {} {
    expect_pause
    continue
    glb_filter SM AO
    loc_filter AO l_table
    current_obj SM_AO l_table
}

# tests...
test "PAUSE->Table"
dispatch PAUSE_SIG
expect "%timestamp Disp===> Obj=l_table,Sig=PAUSE_SIG,State=Table_serving"
expect "===RTC===> St-Entry Obj=l_table,State=Table_paused"
expect "%timestamp ===>Tran Obj=l_table,Sig=PAUSE_SIG,State=Table_serving->Table_paused"
expect "%timestamp Trg-Done QS_RX_EVENT"

test "SERVE->Table (1)"
command 1
expect "%timestamp Disp===> Obj=l_table,Sig=SERVE_SIG,State=Table_serving"
expect "%timestamp =>Ignore Obj=l_table,Sig=SERVE_SIG,State=Table_serving"
expect "%timestamp Trg-Done QS_RX_COMMAND"

test "SERVE->Table (2)" -noreset
probe BSP_displayPaused 1
dispatch PAUSE_SIG
expect "%timestamp Disp===> Obj=l_table,Sig=PAUSE_SIG,State=Table_serving"
expect "%timestamp TstProbe Fun=BSP_displayPaused,Data=1"
expect "%timestamp =ASSERT= Mod=bsp,Loc=100"

# the end
end

#-----------------------------------------------------------------------------
# Product: QSpyView -- Customization example for DPP application
# Last updated for version 6.2.0
# Last updated on  2018-03-14
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

# command handlers ===========================================================
proc onMyCommand {} {
    # do something here, for example:
    # - inject an event to the Target
    # - send a command to the Target
    # - peek memory
    # - poke memory
    # - exectute system tick in the Target
    # - open a dialog box...
    # - etc.

    # as an example, the following code sends a command to the Target
    variable ::qspy::QS_RX
    ::qspy::sendPkt [binary format cciii $qspy::QS_RX(COMMAND) 1 12345 0 0]
}

proc onPause {} {
    global theButtonId theBtnState
    if {[string equal $theBtnState BTN_UP]} {    ;# is DWN?
         set theBtnState BTN_DWN
         ::qspy::sendCurrObj SM_AO l_table
         ::qspy::sendEvent 255 PAUSE_SIG
         #::qspy::sendEvent 6 6 0
    } else {
         set theBtnState BTN_UP
         ::qspy::sendCurrObj SM_AO l_table
         ::qspy::sendEvent 255 SERVE_SIG
         #::qspy::sendEvent 6 7 0
    }
    .canv.c itemconfigure $theButtonId -image ::img::$theBtnState
}

# additinal menu options =====================================================
.mbar.cust add command -label "MyCommand" -command onMyCommand


# specific canvas for DPP ====================================================
set scriptFolder [file dirname [file normalize [info script]]]

image create photo ::img::e       -file $scriptFolder/img/eating.gif
image create photo ::img::h       -file $scriptFolder/img/hungry.gif
image create photo ::img::t       -file $scriptFolder/img/thinking.gif
image create photo ::img::BTN_UP  -file $scriptFolder/img/BTN_UP.gif
image create photo ::img::BTN_DWN -file $scriptFolder/img/BTN_DWN.gif

wm geometry .canv =400x260
.canv.c configure -width  400
.canv.c configure -height 260

set thePhiloId [.canv.c create image 190  57 -image ::img::t]
.canv.c create image 273 100 -image ::img::t
.canv.c create image 237 185 -image ::img::t
.canv.c create image 146 184 -image ::img::t
.canv.c create image 107 100 -image ::img::t


set theButtonId [.canv.c create image 200 120 -image ::img::BTN_UP]
set theBtnState BTN_UP
.canv.c bind $theButtonId <ButtonPress-1> onPause

# QS record handlers =========================================================

# user record handlers [70..0x7C] --------------------------------------------
proc ::qspy::rec70  {} { ;# QS_USER
    variable thePkt
    variable theFmt
    binary scan $thePkt xx$theFmt(tstamp)xcxa* \
           ts philoNum stat

    dispTxt [format "%010u Philo %1d is %s" $ts $philoNum $stat]

    global thePhiloId
    set img [string index $stat 0]
    .canv.c itemconfigure [expr $thePhiloId + $philoNum] -image ::img::$img
}
#.............................................................................
proc ::qspy::rec71  {} { ;# QS_USER + 1
    variable thePkt
    variable theFmt
    binary scan $thePkt xc$theFmt(tstamp)xc \
           cmdId ts par
    dispTxt [format "%010u cmd=%d param=%d" $ts $cmdId $par]
}

proc ::qspy::rec72  {} { ;# QS_USER + 2
}
proc ::qspy::rec73  {} { ;# QS_USER + 3
}
proc ::qspy::rec74  {} { ;# QS_USER + 4
}
proc ::qspy::rec75  {} { ;# QS_USER + 5
}
proc ::qspy::rec76  {} { ;# QS_USER + 6
}
proc ::qspy::rec77  {} { ;# QS_USER + 7
}
proc ::qspy::rec78  {} { ;# QS_USER + 8
}
proc ::qspy::rec79  {} { ;# QS_USER + 9
}
proc ::qspy::rec80  {} { ;# QS_USER + 10
}
proc ::qspy::rec81  {} { ;# QS_USER + 11
}
proc ::qspy::rec82  {} { ;# QS_USER + 12
}
proc ::qspy::rec83  {} { ;# QS_USER + 13
}
proc ::qspy::rec84  {} { ;# QS_USER + 14
}
proc ::qspy::rec85  {} { ;# QS_USER + 15
}
proc ::qspy::rec86  {} { ;# QS_USER + 16
}
proc ::qspy::rec87  {} { ;# QS_USER + 17
}
proc ::qspy::rec88  {} { ;# QS_USER + 18
}
proc ::qspy::rec89  {} { ;# QS_USER + 19
}
proc ::qspy::rec90  {} { ;# QS_USER + 20
}
proc ::qspy::rec91  {} { ;# QS_USER + 21
}
proc ::qspy::rec92  {} { ;# QS_USER + 22
}
proc ::qspy::rec93  {} { ;# QS_USER + 23
}
proc ::qspy::rec94  {} { ;# QS_USER + 24
}
proc ::qspy::rec95  {} { ;# QS_USER + 25
}
proc ::qspy::rec96  {} { ;# QS_USER + 26
}
proc ::qspy::rec97  {} { ;# QS_USER + 27
}
proc ::qspy::rec98  {} { ;# QS_USER + 28
}
proc ::qspy::rec99  {} { ;# QS_USER + 29
}
proc ::qspy::rec100 {} { ;# QS_USER + 30
}
proc ::qspy::rec101 {} { ;# QS_USER + 31
}
proc ::qspy::rec102 {} { ;# QS_USER + 32
}
proc ::qspy::rec103 {} { ;# QS_USER + 33
}
proc ::qspy::rec104 {} { ;# QS_USER + 34
}
proc ::qspy::rec105 {} { ;# QS_USER + 35
}
proc ::qspy::rec106 {} { ;# QS_USER + 36
}
proc ::qspy::rec107 {} { ;# QS_USER + 37
}
proc ::qspy::rec108 {} { ;# QS_USER + 38
}
proc ::qspy::rec109 {} { ;# QS_USER + 39
}
proc ::qspy::rec110 {} { ;# QS_USER + 40
}
proc ::qspy::rec111 {} { ;# QS_USER + 41
}
proc ::qspy::rec112 {} { ;# QS_USER + 42
}
proc ::qspy::rec113 {} { ;# QS_USER + 43
}
proc ::qspy::rec114 {} { ;# QS_USER + 44
}
proc ::qspy::rec115 {} { ;# QS_USER + 45
}
proc ::qspy::rec116 {} { ;# QS_USER + 46
}
proc ::qspy::rec117 {} { ;# QS_USER + 47
}
proc ::qspy::rec118 {} { ;# QS_USER + 48
}
proc ::qspy::rec119 {} { ;# QS_USER + 49
}
proc ::qspy::rec120 {} { ;# QS_USER + 50
}
proc ::qspy::rec121 {} { ;# QS_USER + 51
}
proc ::qspy::rec122 {} { ;# QS_USER + 52
}
proc ::qspy::rec123 {} { ;# QS_USER + 53
}
proc ::qspy::rec124 {} { ;# QS_USER + 54
}


# special record handlers ----------------------------------------------------
proc ::qspy::recRESET {} { ;# target reset callback
}
proc ::qspy::recINFO  {} { ;# target info callback
}

# standard record handlers [1..54] -------------------------------------------
proc ::qspy::rec0 {} { ;# QS_EMPTY
}

# [1] QEP records...
proc ::qspy::rec1 {} { ;# QS_QEP_STATE_ENTRY
}
proc ::qspy::rec2 {} { ;# QS_QEP_STATE_EXIT
}
proc ::qspy::rec3 {} { ;# QS_QEP_STATE_INIT
}
proc ::qspy::rec4 {} { ;# QS_QEP_INIT_TRAN
}
proc ::qspy::rec5 {} { ;# QS_QEP_INTERN_TRAN
}
proc ::qspy::rec6 {} { ;# QS_QEP_TRAN
}
proc ::qspy::rec7 {} { ;# QS_QEP_IGNORED
}
proc ::qspy::rec8 {} { ;# QS_QEP_DISPATCH
}
proc ::qspy::rec9 {} { ;# QS_QEP_UNHANDLED
}

# [10] QF records...
proc ::qspy::rec10 {} { ;# QS_QF_ACTIVE_ADD
}
proc ::qspy::rec11 {} { ;# QS_QF_ACTIVE_REMOVE
}
proc ::qspy::rec12 {} { ;# QS_QF_ACTIVE_SUBSCRIBE
}
proc ::qspy::rec13 {} { ;# QS_QF_ACTIVE_UNSUBSCRIBE
}
proc ::qspy::rec14 {} { ;# QS_QF_ACTIVE_POST_FIFO
}
proc ::qspy::rec15 {} { ;# QS_QF_ACTIVE_POST_LIFO
}
proc ::qspy::rec16 {} { ;# QS_QF_ACTIVE_GET
}
proc ::qspy::rec17 {} { ;# QS_QF_ACTIVE_GET_LAST
}
proc ::qspy::rec18 {} { ;# QS_QF_EQUEUE_INIT
}
proc ::qspy::rec19 {} { ;# QS_QF_EQUEUE_POST_FIFO
}
proc ::qspy::rec20 {} { ;# QS_QF_EQUEUE_POST_LIFO
}
proc ::qspy::rec21 {} { ;# QS_QF_EQUEUE_GET
}
proc ::qspy::rec22 {} { ;# QS_QF_EQUEUE_GET_LAST
}
proc ::qspy::rec23 {} { ;# QS_QF_MPOOL_INIT
}
proc ::qspy::rec24 {} { ;# QS_QF_MPOOL_GET
}
proc ::qspy::rec25 {} { ;# QS_QF_MPOOL_PUT
}
proc ::qspy::rec26 {} { ;# QS_QF_PUBLISH
}
proc ::qspy::rec27 {} { ;# QS_QF_RESERVED8
}
proc ::qspy::rec28 {} { ;# QS_QF_NEW
}
proc ::qspy::rec29 {} { ;# QS_QF_GC_ATTEMPT
}
proc ::qspy::rec30 {} { ;# QS_QF_GC
}
proc ::qspy::rec31 {} { ;# QS_QF_TICK
}
proc ::qspy::rec32 {} { ;# QS_QF_TIMEEVT_ARM
}
proc ::qspy::rec33 {} { ;# QS_QF_TIMEEVT_AUTO_DISARM
}
proc ::qspy::rec34 {} { ;# QS_QF_TIMEEVT_DISARM_ATTEMPT
}
proc ::qspy::rec35 {} { ;# QS_QF_TIMEEVT_DISARM
}
proc ::qspy::rec36 {} { ;# QS_QF_TIMEEVT_REARM
}
proc ::qspy::rec37 {} { ;# QS_QF_TIMEEVT_POST
}
proc ::qspy::rec38 {} { ;# QS_QF_TIMEEVT_CTR
}
proc ::qspy::rec39 {} { ;# QS_QF_CRIT_ENTRY
}
proc ::qspy::rec40 {} { ;# QS_QF_CRIT_EXIT
}
proc ::qspy::rec41 {} { ;# QS_QF_ISR_ENTRY
}
proc ::qspy::rec42 {} { ;# QS_QF_ISR_EXIT
}
proc ::qspy::rec43 {} { ;# QS_QF_INT_DISABLE
}
proc ::qspy::rec44 {} { ;# QS_QF_INT_ENABLE
}
proc ::qspy::rec45 {} { ;# QS_QF_ACTIVE_POST_ATTEMPT
}
proc ::qspy::rec46 {} { ;# QS_QF_EQUEUE_POST_ATTEMPT
}
proc ::qspy::rec47 {} { ;# QS_QF_MPOOL_GET_ATTEMPT
}
proc ::qspy::rec48 {} { ;# QS_QF_RESERVED1
}
proc ::qspy::rec49 {} { ;# QS_QF_RESERVED0
}

# [50] QK/QV records
proc ::qspy::rec50 {} { ;# QS_QK_MUTEX_LOCK
}
proc ::qspy::rec51 {} { ;# QS_QK_MUTEX_UNLOCK
}
proc ::qspy::rec52 {} { ;# QS_QVK_SCHEDULE
}
proc ::qspy::rec53 {} { ;# QS_QVK_IDLE
}
proc ::qspy::rec54 {} { ;# QS_QK_RESUME
}

# [55] Additional QEP records
proc ::qspy::rec55 {} { ;# QS_QEP_TRAN_HIST
}
proc ::qspy::rec56 {} { ;# QS_QEP_TRAN_EP
}
proc ::qspy::rec57 {} { ;# QS_QEP_TRAN_XP
}
proc ::qspy::rec58 {} { ;# QS_QEP_RESERVED1
}
proc ::qspy::rec59 {} { ;# QS_QEP_RESERVED0
}

# Miscellaneous QS records
proc ::qspy::rec60 {} { ;# QS_SIG_DICT
}
proc ::qspy::rec61 {} { ;# QS_OBJ_DICT
}
proc ::qspy::rec62 {} { ;# QS_FUN_DICT
}
proc ::qspy::rec63 {} { ;# QS_USR_DICT
}

# proc ::qspy::64 ;# QS_TARGET_INFO not used, see proc recINFO

proc ::qspy::rec65 {} { ;# QS_RESERVED0
}
proc ::qspy::rec66 {} { ;# QS_RX_STATUS
}
proc ::qspy::rec67 {} { ;# QS_TEST_STATUS
}
proc ::qspy::rec68 {} { ;# QS_PEEK_DATA
}
proc ::qspy::rec69 {} { ;# QS_ASSERT_FAIL
}

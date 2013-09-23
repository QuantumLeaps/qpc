##############################################################################
# Product: TCL make script for DPP, POSIX port, GNU compiler
# Last Updated for Version: 4.5.06
# Date of the Last Update:  Apr 07, 2013
#
#                    Q u a n t u m     L e a P s
#                    ---------------------------
#                    innovating embedded systems
#
# Copyright (C) 2002-2013 Quantum Leaps, LLC. All rights reserved.
#
# This program is open source software: you can redistribute it and/or
# modify it under the terms of the GNU General Public License as published
# by the Free Software Foundation, either version 2 of the License, or
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
# Quantum Leaps Web sites: http://www.quantum-leaps.com
#                          http://www.state-machine.com
# e-mail:                  info@quantum-leaps.com
##############################################################################
#
# usage
# $tclsh make.tcl
# $tclsh make.tcl clean
# $tclsh make.tcl CONF=rel
# $tclsh make.tcl CONF=rel clean
# $tclsh make.tcl CONF=spy
# $tclsh make.tcl CONF=spy clean
#

set PROJECT     "dpp"
set C_FILES     [glob -nocomplain *.c]

set QPC         "[file normalize $env(QPC)]"
set QTOOLS      "[file normalize $env(QTOOLS)]"
set QP_PORT_DIR "$QPC/ports/posix/gnu"
set INCLUDES    "-I. -I$QPC/include -I$QP_PORT_DIR"
set DEFINES     ""

#=============================================================================
# typically no need to change anything below this line
#
set CC    "gcc"
set CPP   "g++"
set LINK  "gcc"
set CFLAGS "-c -Wall -pthread"

set BIN_DIR "dbg"

#.............................................................................
# execute a build command
proc make.exec args {
    set cmd [join $args]
    puts $cmd
    catch {eval exec -- $cmd} msg
    if {[string length $msg]} {
        puts $msg
    }
    global EXEC
    incr EXEC
}
#.............................................................................
proc make.filesAreNewer {depList f} {
    foreach dep $depList {
        if {![file exists $dep] || ![file exists $f]} {
            return 1
        }
        if {[file mtime $dep] > [file mtime $f]} {
            return 1
        }
    }
    return 0
}
#.............................................................................
proc make.fileChangeExt { file ext } {
   return [file tail [file rootname $file]]$ext
}
#.............................................................................
proc make.genDepList {depFile} {
    set fileList {}
    if { [catch {open $depFile "r"} fid] } {
        puts stderr $fid
        return $fileList
    }
    set contentList [split $[read -nonewline $fid] "\n "]
    catch { close $fid }

    foreach str $contentList {
        if { $str != "\\" && $str != " "} {
            set $str [file normalize $str]
            if { [file exists $str] } {
                lappend fileList [file normalize $str]
            }
        }
    }
    return $fileList
}
#.............................................................................
proc make.delete { dir pattern } {
    set fileList [glob -nocomplain -directory $dir $pattern]
    puts -nonewline "deleting $dir/$pattern"
    set n 0
    foreach f $fileList {
        file delete $f
        incr n
    }
    puts "($n)"
}

#=============================================================================
proc makeDeps { srcList depDir } {
    foreach f $srcList {
        set depFile $depDir/[make.fileChangeExt $f ".d"]
        if { [make.filesAreNewer $f $depFile] } {
             global CC INCLUDES DEFINES
             make.exec $CC -MM -MT [make.fileChangeExt $f ".o"] \
                       $INCLUDES $DEFINES $f > $depFile
        }
    }
}
#.............................................................................
proc makeCobjs { srcList depDir binDir } {
    foreach f $srcList {
        set objFile $binDir/[make.fileChangeExt $f ".o"]
        set depList [make.genDepList $depDir/[make.fileChangeExt $f ".d"]]
        if { [make.filesAreNewer $depList $objFile] } {
             global CC CFLAGS INCLUDES DEFINES
             make.exec $CC $CFLAGS $INCLUDES $DEFINES $f -o $objFile
        }
    }
}
#.............................................................................
proc makeExe { srcList binDir } {
    set objList {}
    foreach f $srcList {
        set objFile $binDir/[make.fileChangeExt $f ".o"]
        lappend objList $objFile
    }
    global PROJECT
    set exeFile $binDir/$PROJECT
    if { [make.filesAreNewer $objList $exeFile] } {
        global LINK QP_PORT_DIR
        make.exec $LINK -L$QP_PORT_DIR/$binDir -pthread -o $exeFile \
                  [join $objList] -lqp
    }
}

#-----------------------------------------------------------------------------
if { [lsearch $argv "CONF=*"] >= 0 } {
    set CONF [string range [lindex $argv [lsearch $argv "CONF=*"]] 5 end]
    set BIN_DIR $CONF
    if { [string equal -nocase $CONF "rel"] } {
        append  CFLAGS   " -Os -DNDEBUG"
    } elseif { [string equal -nocase $CONF "spy"] } {
        append  CFLAGS   " -g -DQ_SPY"
        append  INCLUDES " -I$QTOOLS/qspy/include"
        lappend C_FILES $QTOOLS/qspy/source/qspy.c
    }
} else {
    append  CFLAGS " -g"
}

if { 0 <= [lsearch $argv "clean"] } {

    make.delete $BIN_DIR *.d
    make.delete $BIN_DIR *.o
    make.delete $BIN_DIR $PROJECT
    make.delete $BIN_DIR $PROJECT.map

} else {
    set EXEC 0
    if { ![file exists $BIN_DIR] } {
        file mkdir $BIN_DIR
    }
    makeDeps  $C_FILES $BIN_DIR
    makeCobjs $C_FILES $BIN_DIR $BIN_DIR
    makeExe   $C_FILES $BIN_DIR
    if { $EXEC == 0 } {
        puts "UP TO DATE"
    }
}

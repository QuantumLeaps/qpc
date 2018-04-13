##############################################################################
# Product: Makefile for QUTEST self-test; QP/C on Win32 host
# Last updated for version 6.2.0
# Last updated on  2018-04-13
#
#                    Q u a n t u m     L e a P s
#                    ---------------------------
#                    innovating embedded systems
#
# Copyright (C) 2005-2018 Quantum Leaps, LLC. All rights reserved.
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
##############################################################################
#
# examples of invoking this Makefile:
# make         # make and run the tests in the current directory
# make TESTS=test*.tcl  # make and run the selected tests in the curr. dir.
# make HOST=localhost:7705 # connect to host:port
# make norun   # only make but not run the tests
# make clean   # cleanup the build
#
# NOTE:
# To use this Makefile on Windows, you will need the GNU make utility, which
# is included in the Qtools collection for Windows, see:
#    http://sourceforge.net/projects/qpc/files/QTools/
#

#-----------------------------------------------------------------------------
# project name
#
PROJECT := test_qutest

#-----------------------------------------------------------------------------
# project directories
#

# location of the QP/C framework (if not provided in an environemnt var.)
ifeq ($(QPC),)
QPC := ../../..
endif

# location of the QTOOLS directory (if not provided in an environemnt var.)
ifeq ($(QTOOLS),)
QTOOLS := c:/qp/qtools
endif


# QP port used in this project
QP_PORT_DIR := $(QPC)/ports/win32-qutest

# list of all source directories used by this project
VPATH = \
	. \

# list of all include directories needed by this project
INCLUDES  = \
	-I. \
	-I$(QPC)/include

#-----------------------------------------------------------------------------
# files
#

# C source files...
C_SRCS := \
	test_qutest.c

# C++ source files...
CPP_SRCS  :=

LIB_DIRS  :=
LIBS      :=

# defines...
# QP_API_VERSION controls the QP API compatibility; 9999 means the latest API
DEFINES   := -DQP_API_VERSION=9999

#-----------------------------------------------------------------------------
# MinGW toolset (NOTE: assumed to be on your PATH)
#
# NOTE:
# MinGW toolset is included in the Qtools collection for Windows, see:
#    http://sourceforge.net/projects/qpc/files/Qtools/
#
# NOTE:
# This Makefile assumes that the windres utility is available on the
# PATH (NOTE: windres is available in the Qtools collection for Windows)
#
CC    := gcc
CPP   := g++
LINK  := gcc    # for C programs
#LINK  := g++   # for C++ programs

# basic utilities (included in Qtools for Windows), see:
#    http://sourceforge.net/projects/qpc/files/Qtools

MKDIR  := mkdir
RM     := rm
TCLSH  := tclsh
QUTEST := $(QTOOLS)/qspy/tcl/qutest.tcl


#============================================================================
# Typically you should not need to change anything below this line

#-----------------------------------------------------------------------------
# build options
#

BIN_DIR := mingw

CFLAGS = -g -O -Wall -Wstrict-prototypes -W $(INCLUDES) $(DEFINES) \
	-DQ_SPY -DQ_UTEST -DQ_HOST

CPPFLAGS = -g -O -Wall -W -fno-rtti -fno-exceptions $(INCLUDES) $(DEFINES) \
	-DQ_SPY -DQ_UTEST -DQ_HOST

LINKFLAGS := -Wl,-Map,$(BIN_DIR)/$(PROJECT).map,--cref,--gc-sections

#-----------------------------------------------------------------------------
# combine all the soruces...
INCLUDES  += -I$(QP_PORT_DIR)
LIB_DIRS  += -L$(QP_PORT_DIR)/$(BIN_DIR)
LIBS      += -lqp -lwsock32

C_OBJS       := $(patsubst %.c,%.o,   $(C_SRCS))
CPP_OBJS     := $(patsubst %.cpp,%.o, $(CPP_SRCS))

TARGET_EXE   := $(BIN_DIR)/$(PROJECT).exe
C_OBJS_EXT   := $(addprefix $(BIN_DIR)/, $(C_OBJS))
C_DEPS_EXT   := $(patsubst %.o,%.d, $(C_OBJS_EXT))
CPP_OBJS_EXT := $(addprefix $(BIN_DIR)/, $(CPP_OBJS))
CPP_DEPS_EXT := $(patsubst %.o,%.d, $(CPP_OBJS_EXT))

# create $(BIN_DIR) if it does not exist
ifeq ("$(wildcard $(BIN_DIR))","")
$(shell $(MKDIR) $(BIN_DIR))
endif

#-----------------------------------------------------------------------------
# rules
#

.PHONY : run norun

ifeq ($(MAKECMDGOALS),norun)
all : $(TARGET_EXE)
norun : all
else
all : $(TARGET_EXE) run
endif

ifeq (, $(TESTS))
TESTS := *.tcl
endif

$(TARGET_EXE) : $(C_OBJS_EXT) $(CPP_OBJS_EXT)
	$(CC) $(CFLAGS) -c $(QPC)/include/qstamp.c -o $(BIN_DIR)/qstamp.o
	$(LINK) $(LINKFLAGS) $(LIB_DIRS) -o $@ $^ $(BIN_DIR)/qstamp.o $(LIBS)

run : $(TARGET_EXE)
	$(TCLSH) $(QUTEST) $(TESTS) $(TARGET_EXE) $(HOST)

$(BIN_DIR)/%.d : %.cpp
	$(CPP) -MM -MT $(@:.d=.o) $(CPPFLAGS) $< > $@

$(BIN_DIR)/%.d : %.c
	$(CC) -MM -MT $(@:.d=.o) $(CFLAGS) $< > $@

$(BIN_DIR)/%.o : %.cpp
	$(CPP) $(CPPFLAGS) -c $< -o $@

$(BIN_DIR)/%.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY : clean show norun

# include dependency files only if our goal depends on their existence
ifneq ($(MAKECMDGOALS),clean)
  ifneq ($(MAKECMDGOALS),show)
-include $(C_DEPS_EXT) $(CPP_DEPS_EXT)
  endif
endif

clean :
	-$(RM) $(BIN_DIR)/*.o \
	$(BIN_DIR)/*.d \
	$(BIN_DIR)/*.map \
	$(TARGET_EXE)

show :
	@echo PROJECT      = $(PROJECT)
	@echo TARGET_EXE   = $(TARGET_EXE)
	@echo CONF         = $(CONF)
	@echo VPATH        = $(VPATH)
	@echo C_SRCS       = $(C_SRCS)
	@echo CPP_SRCS     = $(CPP_SRCS)
	@echo C_DEPS_EXT   = $(C_DEPS_EXT)
	@echo C_OBJS_EXT   = $(C_OBJS_EXT)
	@echo C_DEPS_EXT   = $(C_DEPS_EXT)
	@echo CPP_DEPS_EXT = $(CPP_DEPS_EXT)
	@echo CPP_OBJS_EXT = $(CPP_OBJS_EXT)
	@echo LIB_DIRS     = $(LIB_DIRS)
	@echo LIBS         = $(LIBS)
	@echo DEFINES      = $(DEFINES)
	@echo QTOOLS       = $(QTOOLS)
	@echo QUTEST       = $(QUTEST)
	@echo TESTS        = $(TESTS)
	@echo HOST         = $(HOST)


##############################################################################
# Product: Makefile for QUTEST-QP/C for Windows and POSIX *HOSTS*
# Last updated for version 6.9.0
# Last updated on  2020-08-01
#
#                    Q u a n t u m  L e a P s
#                    ------------------------
#                    Modern Embedded Software
#
# Copyright (C) 2005-2020 Quantum Leaps, LLC. All rights reserved.
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
# along with this program. If not, see <www.gnu.org/licenses/>.
#
# Contact information:
# <www.state-machine.com/licensing>
# <info@state-machine.com>
##############################################################################
#
# examples of invoking this Makefile:
# make         # make and run the Python tests in the current directory
# make TESTS=test*.py  # make and run the selected tests in the curr. dir.
# make HOST=localhost:7705 # connect to host:port
# make norun   # only make but not run the tests
# make clean   # cleanup the build
# make debug   # only run tests in DEBUG mode
#
# NOTE:
# To use this Makefile on Windows, you will need the GNU make utility, which
# is included in the QTools collection for Windows, see:
#    https://github.com/QuantumLeaps/qtools
#

#-----------------------------------------------------------------------------
# project name:
#
PROJECT := test_qmsmtst

#-----------------------------------------------------------------------------
# project directories:
#

# list of all source directories used by this project
VPATH := . \
	../src

# list of all include directories needed by this project
INCLUDES := -I. \
	-I../src

# location of the QP/C framework (if not provided in an env. variable)
ifeq ($(QPC),)
QPC := ../../../..
endif

# make sure that QTOOLS env. variable is defined...
ifeq ("$(wildcard $(QTOOLS))","")
$(error QTOOLS not found. Please install QTools and define QTOOLS env. variable)
endif

#-----------------------------------------------------------------------------
# project files:
#

# C source files...
C_SRCS := \
	qmsmtst.c \
	test_qmsm.c

# C++ source files...
CPP_SRCS :=

LIB_DIRS :=
LIBS     :=

# defines...
DEFINES  :=

#-----------------------------------------------------------------------------
# add QP/C framework (depends on the OS this Makefile runs on):
#
ifeq ($(OS),Windows_NT)
	QP_PORT_DIR := $(QPC)/ports/win32-qutest
	LIB_DIRS += -L$(QP_PORT_DIR)/mingw
	LIBS     += -lqp -lws2_32
QS_SRCS :=
else
	QP_PORT_DIR := $(QPC)/ports/posix-qutest
	C_SRCS += \
	qep_hsm.c \
	qep_msm.c \
	qf_act.c \
	qf_actq.c \
	qf_defer.c \
	qf_dyn.c \
	qf_mem.c \
	qf_ps.c \
	qf_qact.c \
	qf_qeq.c \
	qf_qmact.c \
	qf_time.c \
	qs.c \
	qs_64bit.c \
	qs_rx.c \
	qs_fp.c \
	qutest.c \
	qutest_port.c

	LIBS += -lpthread
endif

#============================================================================
# Typically you should not need to change anything below this line

VPATH    += $(QPC)/src/qf $(QPC)/src/qs $(QP_PORT_DIR)
INCLUDES += -I$(QPC)/include -I$(QPC)/src -I$(QP_PORT_DIR)

#-----------------------------------------------------------------------------
# GNU toolset:
#
# NOTE:
# GNU toolset (MinGW) is included in the QTools collection for Windows, see:
#     https://www.state-machine.com/qtools
# It is assumed that %QTOOLS%\bin directory is added to the PATH
#
CC    := gcc
CPP   := g++
LINK  := gcc    # for C programs
#LINK  := g++   # for C++ programs

#-----------------------------------------------------------------------------
# QUTest test script utilities (requires QTOOLS):
#
ifeq ("$(wildcard $(QUTEST))","")
QUTEST := python3 $(QTOOLS)/qutest/qutest.py
endif

TESTS  := *.py

#-----------------------------------------------------------------------------
# basic utilities (depends on the OS this Makefile runs on):
#
ifeq ($(OS),Windows_NT)
	MKDIR      := mkdir
	RM         := rm
	TARGET_EXT := .exe
else ifeq ($(OSTYPE),cygwin)
	MKDIR      := mkdir -p
	RM         := rm -f
	TARGET_EXT := .exe
else
	MKDIR      := mkdir -p
	RM         := rm -f
	TARGET_EXT :=
endif

#-----------------------------------------------------------------------------
# build options...

BIN_DIR := build

CFLAGS  := -c -g -O -fno-pie -std=c11 -pedantic -Wall -Wextra -W \
	$(INCLUDES) $(DEFINES) -DQ_SPY -DQ_UTEST -DQ_HOST

CPPFLAGS := -c -g -O -fno-pie -std=c++11 -pedantic -Wall -Wextra \
	-fno-rtti -fno-exceptions \
	$(INCLUDES) $(DEFINES) -DQ_SPY -DQ_UTEST -DQ_HOST

ifndef GCC_OLD
	LINKFLAGS := -no-pie
endif

ifdef GCOV
	CFLAGS    += -fprofile-arcs -ftest-coverage
	CPPFLAGS  += -fprofile-arcs -ftest-coverage
	LINKFLAGS += -lgcov --coverage
endif

#-----------------------------------------------------------------------------
C_OBJS       := $(patsubst %.c,%.o,   $(C_SRCS))
CPP_OBJS     := $(patsubst %.cpp,%.o, $(CPP_SRCS))

TARGET_EXE   := $(BIN_DIR)/$(PROJECT)$(TARGET_EXT)
C_OBJS_EXT   := $(addprefix $(BIN_DIR)/, $(C_OBJS))
C_DEPS_EXT   := $(patsubst %.o,%.d, $(C_OBJS_EXT))
CPP_OBJS_EXT := $(addprefix $(BIN_DIR)/, $(CPP_OBJS))
CPP_DEPS_EXT := $(patsubst %.o,%.d, $(CPP_OBJS_EXT))


#-----------------------------------------------------------------------------
# rules
#

.PHONY : norun debug clean show

ifeq ($(MAKECMDGOALS),norun)
all : $(TARGET_EXE)
norun : all
else
all : $(TARGET_EXE) run
endif

$(TARGET_EXE) : $(C_OBJS_EXT) $(CPP_OBJS_EXT)
	$(CC) $(CFLAGS) $(QPC)/include/qstamp.c -o $(BIN_DIR)/qstamp.o
	$(LINK) $(LINKFLAGS) $(LIB_DIRS) -o $@ $^ $(BIN_DIR)/qstamp.o $(LIBS)

run : $(TARGET_EXE)
	$(QUTEST) $(TESTS) $(TARGET_EXE) $(HOST)

$(BIN_DIR)/%.d : %.cpp
	$(CPP) -MM -MT $(@:.d=.o) $(CPPFLAGS) $< > $@

$(BIN_DIR)/%.d : %.c
	$(CC) -MM -MT $(@:.d=.o) $(CFLAGS) $< > $@

$(BIN_DIR)/%.o : %.c
	$(CC) $(CFLAGS) $< -o $@

$(BIN_DIR)/%.o : %.cpp
	$(CPP) $(CPPFLAGS) $< -o $@

# create BIN_DIR and include dependencies only if needed
ifneq ($(MAKECMDGOALS),clean)
  ifneq ($(MAKECMDGOALS),show)
     ifneq ($(MAKECMDGOALS),debug)
ifeq ("$(wildcard $(BIN_DIR))","")
$(shell $(MKDIR) $(BIN_DIR))
endif
-include $(C_DEPS_EXT) $(CPP_DEPS_EXT)
     endif
  endif
endif

debug :
	$(QUTEST) $(TESTS) DEBUG $(HOST)

clean :
	-$(RM) $(BIN_DIR)/*.*

show :
	@echo PROJECT      = $(PROJECT)
	@echo TARGET_EXE   = $(TARGET_EXE)
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
	@echo HOST         = $(HOST)
	@echo QUTEST       = $(QUTEST)
	@echo TESTS        = $(TESTS)


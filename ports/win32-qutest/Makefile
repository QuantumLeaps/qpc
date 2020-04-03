##############################################################################
# Product: Makefile for QUTEST, QP/C, Win32, MinGW toolset
# Last updated for version 6.8.0
# Last updated on  2020-03-31
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
# along with this program. If not, see <http://www.gnu.org/licenses/>.
#
# Contact information:
# <www.state-machine.com/licensing>
# <info@state-machine.com>
##############################################################################
# examples of invoking this Makefile:
# make
#
# cleaning
# make clean
#
# NOTE:
# To use this Makefile on Windows, you will need the GNU make utility, which
# is included in the Qtools collection for Windows, see:
#    https://sourceforge.net/projects/qpc/files/QTools/
#

#-----------------------------------------------------------------------------
# project name:
#
PROJECT := qp

#-----------------------------------------------------------------------------
# project directories:
#

# location of the QP/C framework
QPC := ../..

# QP port used in this project
QP_PORT_DIR := .


# list of all source directories used by this project
VPATH = \
	$(QPC)/src/qf \
	$(QPC)/src/qs \
	$(QP_PORT_DIR)

# list of all include directories needed by this project
INCLUDES  = \
	-I$(QPC)/include \
	-I$(QPC)/src \
	-I$(QP_PORT_DIR)

#-----------------------------------------------------------------------------
# files
#

# C source files
C_SRCS := \
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

# C++ source files
CPP_SRCS :=

# defines:
DEFINES  := -DQ_SPY -DQ_UTEST -DQ_HOST

#-----------------------------------------------------------------------------
# MinGW toolset (NOTE: assumed to be on your PATH)
#
# NOTE:
# MinGW toolset is included in the Qtools collection for Windows, see:
#    http://sourceforge.net/projects/qpc/files/Qtools/

CC    := gcc
CPP   := g++
LIB   := ar


##############################################################################
# Typically, you should not need to change anything below this line

# basic utilities (included in Qtools for Windows), see:
#    http://sourceforge.net/projects/qpc/files/Qtools

MKDIR := mkdir
RM    := rm

#-----------------------------------------------------------------------------
# build options:
#

LIBFLAGS := rs

BIN_DIR = mingw

# gcc options:
CFLAGS  := -c -g -O -fno-pie -std=c99 -pedantic -Wall -Wextra -W \
	$(INCLUDES) $(DEFINES)

CPPFLAGS := -c -g -O -fno-pie -std=c++11 -pedantic -Wall -Wextra \
	-fno-rtti -fno-exceptions \
	$(INCLUDES) $(DEFINES)

#-----------------------------------------------------------------------------
C_OBJS       := $(patsubst %.c,%.o,  $(notdir $(C_SRCS)))
CPP_OBJS     := $(patsubst %.cpp,%.o,$(notdir $(CPP_SRCS)))

TARGET_LIB   := $(BIN_DIR)/lib$(PROJECT).a
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

all: $(TARGET_LIB)
	-$(RM) $(BIN_DIR)/*.o

$(TARGET_LIB) : $(ASM_OBJS_EXT) $(C_OBJS_EXT) $(CPP_OBJS_EXT)
	$(LIB) $(LIBFLAGS) $@ $^

$(BIN_DIR)/%.o : %.c
	$(CC) $(CFLAGS) $< -o $@

$(BIN_DIR)/%.o : %.cpp
	$(CPP) $(CPPFLAGS) $< -o $@

#-----------------------------------------------------------------------------
# the clean target
#
.PHONY : clean
clean:
	-$(RM) $(BIN_DIR)/*.o $(TARGET_LIB)

#-----------------------------------------------------------------------------
# the show target for debugging
#
show:
	@echo PROJECT = $(PROJECT)
	@echo CONF = $(CONF)
	@echo TARGET_LIB = $(TARGET_LIB)
	@echo C_SRCS = $(C_SRCS)
	@echo CPP_SRCS = $(CPP_SRCS)
	@echo C_OBJS_EXT = $(C_OBJS_EXT)
	@echo C_DEPS_EXT = $(C_DEPS_EXT)
	@echo CPP_OBJS_EXT = $(CPP_OBJS_EXT)
	@echo CPP_DEPS_EXT = $(CPP_DEPS_EXT)

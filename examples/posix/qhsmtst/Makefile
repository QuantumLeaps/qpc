##############################################################################
# Product: Makefile for QP/C, QHsmTst console, POSIX target, GNU compiler
# Last updated for version 6.2.0
# Last updated on  2018-03-16
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
# building configurations: Debug (default), Release, and Spy
# make
# make CONF=rel
# make CONF=spy
#
# cleaning configurations: Debug (default), Release, and Spy
# make clean
# make CONF=rel clean
# make CONF=spy clean

#-----------------------------------------------------------------------------
# project name
#
PROJECT := qhsmtst

#-----------------------------------------------------------------------------
# project directories
#

# location of the QP/C framework (if not provided in an environemnt var.)
ifeq ($(QPC),)
QPC := ../../..
endif

# QP port used in this project
QP_PORT_DIR := $(QPC)/ports/posix

# list of all source directories used by this project
VPATH = \
	. \
	$(QPC)/src/qf \
	$(QPC)/src/qs \
	$(QP_PORT_DIR)

# list of all include directories needed by this project
INCLUDES  = \
	-I. \
	-I$(QPC)/include \
	-I$(QPC)/src \
	-I$(QP_PORT_DIR)

#-----------------------------------------------------------------------------
# files
#

# C source files...
C_SRCS := \
	main.c \
	qhsmtst.c

# C++ source files...
CPP_SRCS :=

QP_SRCS := \
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
	qf_port.c

QS_SRCS := \
	qs.c \
	qs_64bit.c \
	qs_rx.c \
	qs_fp.c

LIB_DIRS  :=
LIBS      :=

# defines...
# QP_API_VERSION controls the QP API compatibility; 9999 means the latest API
DEFINES   := -DQP_API_VERSION=9999

#-----------------------------------------------------------------------------
# GNU toolset
#
CC    := gcc
CPP   := g++
LINK  := gcc    # for C programs
#LINK  := g++   # for C++ programs

MKDIR := mkdir -p
RM    := rm -f

#-----------------------------------------------------------------------------
# build options for various configurations
#
# combine all the soruces...
C_SRCS += $(QP_SRCS)

ifeq (rel, $(CONF)) # Release configuration ..................................

BIN_DIR := rel

CFLAGS = -ffunction-sections -fdata-sections \
	-Os -Wall -W $(INCLUDES) $(DEFINES) -pthread -DNDEBUG

CPPFLAGS =  -fno-rtti -fno-exceptions -ffunction-sections -fdata-sections \
	-Os -Wall -W $(INCLUDES) $(DEFINES) -pthread -DNDEBUG

else ifeq (spy, $(CONF))  # Spy configuration ................................

BIN_DIR := spy

C_SRCS += $(QS_SRCS)

CFLAGS = -g -ffunction-sections -fdata-sections \
	-O -Wall -W $(INCLUDES) $(DEFINES) -pthread -DQ_SPY

CPPFLAGS = -g -fno-rtti -fno-exceptions -ffunction-sections -fdata-sections \
	-O -Wall -W $(INCLUDES) $(DEFINES) -pthread -DQ_SPY

else  # default Debug configuration ..........................................

BIN_DIR := dbg

CFLAGS = -g -ffunction-sections -fdata-sections \
	-O -Wall -W $(INCLUDES) $(DEFINES) -pthread

CPPFLAGS = -g -fno-rtti -fno-exceptions -ffunction-sections -fdata-sections \
	-O -Wall -W $(INCLUDES) $(DEFINES) -pthread

endif  # .....................................................................

LINKFLAGS := -Wl,-Map,$(BIN_DIR)/$(PROJECT).map,--cref,--gc-sections

#-----------------------------------------------------------------------------
# combine all the soruces...
INCLUDES  += -I$(QP_PORT_DIR)
LIB_DIRS  += -L$(QP_PORT_DIR)/$(BIN_DIR)
LIBS      += -lpthread

C_OBJS       := $(patsubst %.c,%.o,   $(C_SRCS))
CPP_OBJS     := $(patsubst %.cpp,%.o, $(CPP_SRCS))

TARGET_BIN   := $(BIN_DIR)/$(PROJECT).bin
TARGET_EXE   := $(BIN_DIR)/$(PROJECT)
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

all: $(TARGET_EXE)
#all: $(TARGET_BIN)

$(TARGET_BIN): $(TARGET_EXE)
	$(BIN) -O binary $< $@

$(TARGET_EXE) : $(C_OBJS_EXT) $(CPP_OBJS_EXT)
	$(CC) $(CFLAGS) -c $(QPC)/include/qstamp.c -o $(BIN_DIR)/qstamp.o
	$(LINK) $(LINKFLAGS) $(LIB_DIRS) -o $@ $^ $(BIN_DIR)/qstamp.o $(LIBS)

$(BIN_DIR)/%.d : %.cpp
	$(CPP) -MM -MT $(@:.d=.o) $(CPPFLAGS) $< > $@

$(BIN_DIR)/%.d : %.c
	$(CC) -MM -MT $(@:.d=.o) $(CFLAGS) $< > $@

$(BIN_DIR)/%.o : %.cpp
	$(CPP) $(CPPFLAGS) -c $< -o $@

$(BIN_DIR)/%.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@

# include dependency files only if our goal depends on their existence
ifneq ($(MAKECMDGOALS),clean)
  ifneq ($(MAKECMDGOALS),show)
-include $(C_DEPS_EXT) $(CPP_DEPS_EXT)
  endif
endif

.PHONY : clean
clean:
	-$(RM) $(BIN_DIR)/*
	
show:
	@echo PROJECT  = $(PROJECT)
	@echo CONF     = $(CONF)
	@echo VPATH    = $(VPATH)
	@echo C_SRCS   = $(C_SRCS)
	@echo CPP_SRCS = $(CPP_SRCS)
	@echo C_OBJS_EXT   = $(C_OBJS_EXT)
	@echo C_DEPS_EXT   = $(C_DEPS_EXT)
	@echo CPP_DEPS_EXT = $(CPP_DEPS_EXT)
	@echo CPP_OBJS_EXT = $(CPP_OBJS_EXT)
	@echo LIB_DIRS = $(LIB_DIRS)
	@echo LIBS     = $(LIBS)


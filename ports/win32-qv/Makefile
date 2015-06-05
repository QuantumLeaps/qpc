##############################################################################
# Product: Makefile for QP/C port to Win32-QV, MinGW toolset
# Last Updated for Version: 5.4.2
# Date of the Last Update:  2015-06-03
#
#                    Q u a n t u m     L e a P s
#                    ---------------------------
#                    innovating embedded systems
#
# Copyright (C) Quantum Leaps, LLC. All rights reserved.
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
# Web:   http://www.state-machine.com
# Email: info@state-machine.com
##############################################################################
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
#
# NOTE:
# To use this Makefile on Windows, you will need the GNU make utility, which
# is included in the Qtools collection for Windows, see:
#    http://sourceforge.net/projects/qpc/files/Qtools/
#

#-----------------------------------------------------------------------------
# project name
#
PROJECT     := qp

#-----------------------------------------------------------------------------
# project directories
#

# location of the QP/C framework
QPC := ../..

# QP port used in this project
QP_PORT_DIR := .


# list of all source directories used by this project
VPATH = \
	$(QPC)/source \
	$(QP_PORT_DIR)

# list of all include directories needed by this project
INCLUDES  = \
	-I$(QPC)/include \
	-I$(QPC)/source \
	-I$(QP_PORT_DIR)

#-----------------------------------------------------------------------------
# files
#

# assembler source files
ASM_SRCS :=

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
	win32_gui.c \
	qf_port.c

C_QS_SRCS := \
	qs.c \
	qs_fp.c \
	qs_64bit.c

# C++ source files
CPP_SRCS :=

# defines
DEFINES  := -DWIN32_GUI

#-----------------------------------------------------------------------------
# MinGW toolset (NOTE: assumed to be on your PATH)
#
# NOTE:
# MinGW toolset is included in the Qtools collection for Windows, see:
#    http://sourceforge.net/projects/qpc/files/Qtools/

CC    := gcc
LIB   := ar


##############################################################################
# Typically, you should not need to change anything below this line

# basic utilities (included in Qtools for Windows), see:
#    http://sourceforge.net/projects/qpc/files/Qtools

MKDIR := mkdir
RM    := rm

#-----------------------------------------------------------------------------
# build options for various configurations
#

LIBFLAGS := rs

ifeq (rel, $(CONF))  # Release configuration .................................

BIN_DIR = rel

# gcc options
CFLAGS = -c -ffunction-sections -fdata-sections \
	-Os $(INCLUDES) $(DEFINES) -Wall -W

else ifeq (spy, $(CONF))  # Spy configuration ................................

BIN_DIR = spy

# gcc options
CFLAGS = -c -g -ffunction-sections -fdata-sections \
	-O $(INCLUDES) $(DEFINES) -Wall -W -DQ_SPY

# add the QS sources...
C_SRCS += $(C_QS_SRCS)

else   # default Debug configuration .........................................

BIN_DIR = dbg

# gcc options
CFLAGS = -c -g -ffunction-sections -fdata-sections \
	-O $(INCLUDES) $(DEFINES) -Wall -W

endif


ASM_OBJS     := $(patsubst %.s,%.o,  $(notdir $(ASM_SRCS)))
C_OBJS       := $(patsubst %.c,%.o,  $(notdir $(C_SRCS)))
CPP_OBJS     := $(patsubst %.cpp,%.o,$(notdir $(CPP_SRCS)))

TARGET_LIB   := $(BIN_DIR)/lib$(PROJECT).a
ASM_OBJS_EXT := $(addprefix $(BIN_DIR)/, $(ASM_OBJS))
C_OBJS_EXT   := $(addprefix $(BIN_DIR)/, $(C_OBJS))
CPP_OBJS_EXT := $(addprefix $(BIN_DIR)/, $(CPP_OBJS))

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

$(BIN_DIR)/%.o : %.s
	$(AS) $(ASFLAGS) $< -o $@

$(BIN_DIR)/%.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(BIN_DIR)/%.o : %.cpp
	$(CPP) $(CPPFLAGS) -c $< -o $@

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
	@echo ASM_SRCS = $(ASM_SRCS)
	@echo C_SRCS = $(C_SRCS)
	@echo CPP_SRCS = $(CPP_SRCS)
	@echo ASM_OBJS_EXT = $(ASM_OBJS_EXT)
	@echo C_OBJS_EXT = $(C_OBJS_EXT)
	@echo C_DEPS_EXT = $(C_DEPS_EXT)
	@echo CPP_DEPS_EXT = $(CPP_DEPS_EXT)

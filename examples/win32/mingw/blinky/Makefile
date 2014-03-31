##############################################################################
# Product: Generic Makefile for QP/C application, Win32, MinGW compiler
# Last updated for version 5.3.0
# Last updated on  2014-03-25
#
#                    Q u a n t u m     L e a P s
#                    ---------------------------
#                    innovating embedded systems
#
# Copyright (C) Quantum Leaps, www.state-machine.com.
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
# Web:   www.state-machine.com
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
##############################################################################
#
# NOTE: Typically, you should have no need to change anything in this Makefile
#
##############################################################################

#-----------------------------------------------------------------------------
# NOTE: the Makefile expects that the QPC environment variable is defined
# and points to the QP/C installation directory
#
ifndef QPC
$(error The QPC environment variable must be defined)
endif

#-----------------------------------------------------------------------------
# NOTE: this Makefile assumes that the MinGW\bin directory is added
# to the PATH variable.
#
CC    := gcc
CPP   := g++
LINK  := gcc                                                 # for C programs
#LINK  := g++                                                # for C++ programs
RM    := rm

#-----------------------------------------------------------------------------
# directories
#
# Project name is derived from the directory name
PROJECT := $(notdir $(CURDIR))

QP_PORT_DIR := $(QPC)/ports/win32
APP_DIR     := .

VPATH = $(APP_DIR)

# include directories
INCLUDES  = -I. \
	-I$(QPC)/include \
	-I$(QP_PORT_DIR)

# defines
DEFINES =

#-----------------------------------------------------------------------------
# files
#

# C source files
C_SRCS := $(wildcard *.c)

# C++ source files
CPP_SRCS := $(wildcard *.cpp)

LD_SCRIPT :=

#-----------------------------------------------------------------------------
# build options for various configurations
#

ifeq (rel, $(CONF))       # Release configuration ............................

BIN_DIR := rel

CFLAGS = -c -Wall -W -O2 $(INCLUDES) $(DEFINES) -DNDEBUG

CPPFLAGS = -c -Wall -W -O2 $(INCLUDES) $(DEFINES) -DNDEBUG

else ifeq (spy, $(CONF))  # Spy configuration ................................

BIN_DIR := spy

INCLUDES +=	-I$(QTOOLS)/qspy/include
VPATH    += $(QTOOLS)/qspy/source
C_SRCS   += qspy.c

CFLAGS = -c -Wall -W -g $(INCLUDES) $(DEFINES) -DQ_SPY

CPPFLAGS = -c -Wall -W -g $(INCLUDES) $(DEFINES) -DQ_SPY

else                     # default Debug configuration .......................

BIN_DIR := dbg

CFLAGS = -c -Wall -W -g $(INCLUDES) $(DEFINES)

CPPFLAGS = -c -Wall -W -g $(INCLUDES) $(DEFINES)

endif

LINKFLAGS = -L$(QP_PORT_DIR)/mingw/$(BIN_DIR) -Wl,-Map,$(BIN_DIR)/$(PROJECT).map

#-----------------------------------------------------------------------------

C_OBJS       := $(patsubst %.c,%.o,$(C_SRCS))
CPP_OBJS     := $(patsubst %.cpp,%.o,$(CPP_SRCS))

TARGET_BIN   := $(BIN_DIR)/$(PROJECT).bin
TARGET_EXE   := $(BIN_DIR)/$(PROJECT).exe
C_OBJS_EXT   := $(addprefix $(BIN_DIR)/, $(C_OBJS))
C_DEPS_EXT   := $(patsubst %.o, %.d, $(C_OBJS_EXT))
CPP_OBJS_EXT := $(addprefix $(BIN_DIR)/, $(CPP_OBJS))
CPP_DEPS_EXT := $(patsubst %.o, %.d, $(CPP_OBJS_EXT))


#-----------------------------------------------------------------------------
# rules
#

all: $(BIN_DIR) $(TARGET_EXE)
#all: $(BIN_DIR) $(TARGET_BIN)

$(BIN_DIR):
	@echo
	mkdir $@

$(TARGET_BIN): $(TARGET_EXE)
	$(BIN) -O binary $< $@

$(TARGET_EXE) : $(C_OBJS_EXT) $(CPP_OBJS_EXT)
	$(LINK) $(LINKFLAGS) -o $@ $^ -lqp

$(BIN_DIR)/%.d : %.c
	$(CC) -MM -MT $(@:.d=.o) $(CFLAGS) $< > $@

$(BIN_DIR)/%.d : %.cpp
	$(CPP) -MM -MT $(@:.d=.o) $(CPPFLAGS) $< > $@

$(BIN_DIR)/%.o : %.s
	$(AS) $(ASFLAGS) $< -o $@

$(BIN_DIR)/%.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(BIN_DIR)/%.o : %.cpp
	$(CPP) $(CPPFLAGS) -c $< -o $@

# include dependency files only if our goal depends on their existence
ifneq ($(MAKECMDGOALS),clean)
  ifneq ($(MAKECMDGOALS),show)
-include $(C_DEPS_EXT) $(CPP_DEPS_EXT)
  endif
endif

.PHONY : clean
clean:
	-$(RM) $(BIN_DIR)/*.o \
	$(BIN_DIR)/*.d \
	$(BIN_DIR)/*.exe \
	$(BIN_DIR)/*.map
	
show:
	@echo PROJECT  = $(PROJECT)
	@echo CONF     = $(CONF)
	@echo VPATH    = $(VPATH)
	@echo C_SRCS   = $(C_SRCS)
	@echo CPP_SRCS = $(CPP_SRCS)
	@echo C_OBJS_EXT   = $(C_OBJS_EXT)
	@echo C_DEPS_EXT   = $(C_DEPS_EXT)
	@echo CPP_DEPS_EXT = $(CPP_DEPS_EXT)

##############################################################################
# Product: Deferred Event example, Win32, MinGW compiler
# Last Updated for Version: 4.5.02
# Date of the Last Update:  Jun 30, 2012
#
#                    Q u a n t u m     L e a P s
#                    ---------------------------
#                    innovating embedded systems
#
# Copyright (C) 2002-2012 Quantum Leaps, LLC. All rights reserved.
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
# NOTE: the Makefile expects that the QPC environment variable is defined
# and points to the QP/C installation directory
#
ifndef QPC
$(error The QPC environment variable must be defined)
endif

#-----------------------------------------------------------------------------
# general utilities
#
RM = del

#-----------------------------------------------------------------------------
# NOTE: this Makefile assumes that the MinGW\bin directory is added
# to the PATH variable.
#
CC    := gcc
CPP   := g++
LINK  := gcc                                                 # for C programs
#LINK  := g++                                                # for C++ programs

#-----------------------------------------------------------------------------
# directories
#
QP_PORT_DIR := $(QPC)\ports\80x86\win32\mingw
APP_DIR     := .

VPATH = $(APP_DIR)

# Output file basename
OUTPUT    := defer

# include directories
INCLUDES  = -I$(QPC)\include	\
	-I$(QP_PORT_DIR) \
	-I.

# defines
DEFINES =

#-----------------------------------------------------------------------------
# files
#

# assembler source files
ASM_SRCS :=

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

ASFLAGS =

CFLAGS = -c -Wall -O2 $(INCLUDES) $(DEFINES) -DNDEBUG

CPPFLAGS = -c -Wall -O2 $(INCLUDES) $(DEFINES) -DNDEBUG

LINKFLAGS = -L$(QP_PORT_DIR)\$(BIN_DIR)

else ifeq (spy, $(CONF))  # Spy configuration ................................

BIN_DIR := spy

ASFLAGS =

CFLAGS = -c -Wall -g $(INCLUDES) $(DEFINES) -DQ_SPY

CPPFLAGS = -c -Wall -g $(INCLUDES) $(DEFINES) -DQ_SPY

LINKFLAGS = -L$(QP_PORT_DIR)\$(BIN_DIR)

else                     # default Debug configuration .......................

BIN_DIR := dbg

ASFLAGS =

CFLAGS = -c -Wall -g $(INCLUDES) $(DEFINES)

CPPFLAGS = -c -Wall -g $(INCLUDES) $(DEFINES)

LINKFLAGS = -L$(QP_PORT_DIR)\$(BIN_DIR)


endif


ASM_OBJS     := $(patsubst %.s,%.o,$(ASM_SRCS))
C_OBJS       := $(patsubst %.c,%.o,$(C_SRCS))
CPP_OBJS     := $(patsubst %.cpp,%.o,$(CPP_SRCS))

TARGET_BIN   := $(BIN_DIR)/$(OUTPUT).bin
TARGET_EXE   := $(BIN_DIR)/$(OUTPUT).exe
ASM_OBJS_EXT := $(addprefix $(BIN_DIR)/, $(ASM_OBJS))
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
	mkdir -p $@

$(TARGET_BIN): $(TARGET_EXE)
	$(BIN) -O binary $< $@

$(TARGET_EXE) : $(ASM_OBJS_EXT) $(C_OBJS_EXT) $(CPP_OBJS_EXT)
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

-include $(C_DEPS_EXT) $(CPP_DEPS_EXT)


.PHONY : clean
clean:
	-$(RM) $(BIN_DIR)\*.o \
	$(BIN_DIR)\*.d \
	$(BIN_DIR)\*.exe \
	$(BIN_DIR)\*.map
	
show:
	@echo CONF = $(CONF)
	@echo ASM_SRCS = $(ASM_SRCS)
	@echo C_SRCS = $(C_SRCS)
	@echo CPP_SRCS = $(CPP_SRCS)
	@echo ASM_OBJS_EXT = $(ASM_OBJS_EXT)
	@echo C_OBJS_EXT = $(C_OBJS_EXT)
	@echo C_DEPS_EXT = $(C_DEPS_EXT)
	@echo CPP_DEPS_EXT = $(CPP_DEPS_EXT)
	@echo TARGET_ELF = $(TARGET_ELF)

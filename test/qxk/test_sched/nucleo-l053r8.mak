##############################################################################
# Product: Makefile for SYSTEM-Level tests of QP/C on NUCLEO-L053R8, GNU-ARM
# Last Updated for Version: 7.3.1
# Date of the Last Update:  2023-11-13
#
#                    Q u a n t u m  L e a P s
#                    ------------------------
#                    Modern Embedded Software
#
# Copyright (C) 2005 Quantum Leaps, LLC. All rights reserved.
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
# along with this program. If not, see <http://www.gnu.org/licenses>.
#
# Contact information:
# <www.state-machine.com/licensing>
# <info@state-machine.com>
##############################################################################
#
# examples of invoking this Makefile:
# make -f nucleo-l053r8.mak USB=g: # make, upload to USB drive, run the tests
# make -f nucleo-l053r8.mak USB=g: TESTS=philo*.py  # make and run the selected tests
# make -f nucleo-l053r8.mak HOST=localhost:7705 # connect to host:port
# make -f nucleo-l053r8.mak norun   # only make but not run the tests
# make -f nucleo-l053r8.mak clean   # cleanup the build
# make -f nucleo-l053r8.mak debug   # only run tests in DEBUG mode
#
# NOTE:
# To use this Makefile on Windows, you will need the GNU make utility, which
# is included in the QTools collection for Windows, see:
#    https://github.com/QuantumLeaps/qtools
#

# location of the QP/C framework (if not provided in an env. variable)
ifeq ($(QPC),)
QPC := ../../..
endif

#-----------------------------------------------------------------------------
# project name, target name, target directory:
#
PROJECT := test_sched
TARGET  := nucleo-l053r8
TARGET_DIR := $(QPC)/3rd_party/nucleo-l053r8/qutest

#-----------------------------------------------------------------------------
# project directories:
#

# QP port used in this project
QP_PORT_DIR := $(QPC)/ports/arm-cm/qxk/gnu

# make sure that QTOOLS env. variable is defined...
ifeq ("$(wildcard $(QTOOLS))","")
$(error QTOOLS not found. Please install QTools and define QTOOLS env. variable)
endif


# list of all source directories used by this project
VPATH := . \
	$(QPC)/src/qf \
	$(QPC)/src/qxk \
	$(QPC)/src/qs \
	$(QP_PORT_DIR) \
	$(TARGET_DIR) \
	$(QPC)/3rd_party/nucleo-l053r8 \
	$(QPC)/3rd_party/nucleo-l053r8/gnu

# list of all include directories needed by this project
INCLUDES  = -I. \
	-I$(QPC)/include \
	-I$(QP_PORT_DIR) \
	-I$(TARGET_DIR) \
	-I$(QPC)/3rd_party/CMSIS/Include \
	-I$(QPC)/3rd_party/nucleo-l053r8

#-----------------------------------------------------------------------------
# project files:
#

# assembler source files
ASM_SRCS :=

# C source files
C_SRCS := \
	test_sched.c \
	bsp_nucleo-l053r8.c \
	system_stm32l0xx.c \
	startup_stm32l053xx.c

# C++ source files
CPP_SRCS :=

OUTPUT    := $(PROJECT)
LD_SCRIPT := $(TARGET_DIR)/qutest.ld

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
	qxk.c \
	qxk_xthr.c \
	qxk_sema.c \
	qxk_mutex.c \
	qxk_port.c \
	qs.c \
	qs_64bit.c \
	qs_rx.c \
	qs_fp.c \
	qutest.c \
	qutest_port.c

QP_ASMS :=

LIB_DIRS  :=
LIBS      :=

# defines
DEFINES   := -DSTM32L053xx \
	-DQF_ON_CONTEXT_SW

# ARM CPU, ARCH, FPU, and Float-ABI types...
# ARM_CPU:   [cortex-m0 | cortex-m0plus | cortex-m1 | cortex-m3 | cortex-m4]
# ARM_FPU:   [ | vfp]
# FLOAT_ABI: [ | soft | softfp | hard]
#
ARM_CPU   := -mcpu=cortex-m0plus
ARM_FPU   :=
FLOAT_ABI :=

#-----------------------------------------------------------------------------
# GNU-ARM toolset (NOTE: You need to adjust to your machine)
# see https://developer.arm.com/open-source/gnu-toolchain/gnu-rm/downloads
#
ifeq ($(GNU_ARM),)
GNU_ARM := $(QTOOLS)/gnu_arm-none-eabi
endif

# make sure that the GNU-ARM toolset exists...
ifeq ("$(wildcard $(GNU_ARM))","")
$(error GNU_ARM toolset not found. Please adjust the Makefile)
endif

CC    := $(GNU_ARM)/bin/arm-none-eabi-gcc
CPP   := $(GNU_ARM)/bin/arm-none-eabi-g++
AS    := $(GNU_ARM)/bin/arm-none-eabi-as
LINK  := $(GNU_ARM)/bin/arm-none-eabi-gcc
BIN   := $(GNU_ARM)/bin/arm-none-eabi-objcopy

#-----------------------------------------------------------------------------
# NOTE: The symbol USB must be provided for the NUCLEO board
# has enumerated as USB drive f:
#
ifeq ($(USB),)
$(error USB drive not provided for the NUCLEO board.)
endif

##############################################################################
# Typically you should not need to change anything below this line

# basic utilities (included in QTools for Windows), see:
#     https://www.state-machine.com/qtools

MKDIR := mkdir
RM    := rm
CP    := cp
SLEEP := sleep

#-----------------------------------------------------------------------------
# QUTest test script utilities (requires QTOOLS):
#
ifeq ("$(wildcard $(QUTEST))","")
QUTEST := python3 $(QTOOLS)/qutest/qutest.py
endif

#-----------------------------------------------------------------------------
# build options
#

# combine all the sources...
C_SRCS += $(QP_SRCS)
ASM_SRCS += $(QP_ASMS)

BIN_DIR := build_$(TARGET)

ASFLAGS = -g $(ARM_CPU) $(ARM_FPU) $(ASM_CPU) $(ASM_FPU)


# NOTE:
# Setting -DQ_UTEST=0 means that QUTest should be built WITHOUT
# the QP-stub for testing QP itself
#
CFLAGS = -c -g $(ARM_CPU) $(ARM_FPU) $(FLOAT_ABI) -mthumb -Wall \
	-ffunction-sections -fdata-sections \
	-O $(INCLUDES) $(DEFINES) -DQ_SPY -DQ_UTEST=0

CPPFLAGS = -c -g $(ARM_CPU) $(ARM_FPU) $(FLOAT_ABI) -mthumb -Wall \
	-ffunction-sections -fdata-sections -fno-rtti -fno-exceptions \
	-O $(INCLUDES) $(DEFINES) -DQ_SPY -DQ_UTEST=0


LINKFLAGS = -T$(LD_SCRIPT) $(ARM_CPU) $(ARM_FPU) $(FLOAT_ABI) -mthumb \
	-specs=nosys.specs -specs=nano.specs \
	-Wl,-Map,$(BIN_DIR)/$(OUTPUT).map,--cref,--gc-sections $(LIB_DIRS)

ASM_OBJS     := $(patsubst %.s,%.o,  $(notdir $(ASM_SRCS)))
C_OBJS       := $(patsubst %.c,%.o,  $(notdir $(C_SRCS)))
CPP_OBJS     := $(patsubst %.cpp,%.o,$(notdir $(CPP_SRCS)))

TARGET_BIN   := $(BIN_DIR)/$(OUTPUT).bin
TARGET_ELF   := $(BIN_DIR)/$(OUTPUT).elf
ASM_OBJS_EXT := $(addprefix $(BIN_DIR)/, $(ASM_OBJS))
C_OBJS_EXT   := $(addprefix $(BIN_DIR)/, $(C_OBJS))
C_DEPS_EXT   := $(patsubst %.o, %.d, $(C_OBJS_EXT))
CPP_OBJS_EXT := $(addprefix $(BIN_DIR)/, $(CPP_OBJS))
CPP_DEPS_EXT := $(patsubst %.o, %.d, $(CPP_OBJS_EXT))

# create $(BIN_DIR) if it does not exist
ifeq ("$(wildcard $(BIN_DIR))","")
$(shell $(MKDIR) $(BIN_DIR))
endif

#-----------------------------------------------------------------------------
# rules
#

.PHONY : run norun debug flash

ifeq ($(MAKECMDGOALS),norun)
all : $(TARGET_BIN)
norun : all
else
all : $(TARGET_BIN) run
endif

$(TARGET_BIN) : $(TARGET_ELF)
	$(BIN) -O binary $< $@
	$(CP) $@ $(USB)
	$(SLEEP) 2

$(TARGET_ELF) : $(ASM_OBJS_EXT) $(C_OBJS_EXT) $(CPP_OBJS_EXT)
	$(CC) $(CFLAGS) $(QPC)/src/qs/qstamp.c -o $(BIN_DIR)/qstamp.o
	$(LINK) $(LINKFLAGS) -o $@ $^ $(BIN_DIR)/qstamp.o $(LIBS)

flash :
	$(CP) $(TARGET_BIN) $(USB)

run : $(TARGET_BIN)
	$(QUTEST) -q$(QSPY) -l$(LOG) -o$(OPT) -- $(TESTS)

$(BIN_DIR)/%.d : %.c
	$(CC) -MM -MT $(@:.d=.o) $(CFLAGS) $< > $@

$(BIN_DIR)/%.d : %.cpp
	$(CPP) -MM -MT $(@:.d=.o) $(CPPFLAGS) $< > $@

$(BIN_DIR)/%.o : %.s
	$(AS) $(ASFLAGS) $< -o $@

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
	$(QUTEST) -edebug -q$(QSPY) -l$(LOG) -o$(OPT) -- $(TESTS)

.PHONY : clean show

clean :
	-$(RM) $(BIN_DIR)/*.o \
	$(BIN_DIR)/*.d \
	$(BIN_DIR)/*.bin \
	$(BIN_DIR)/*.elf \
	$(BIN_DIR)/*.map

show :
	@echo PROJECT      = $(PROJECT)
	@echo MAKECMDGOALS = $(MAKECMDGOALS)
	@echo TESTS        = $(TESTS)
	@echo TARGET_ELF   = $(TARGET_ELF)
	@echo CONF         = $(CONF)
	@echo VPATH        = $(VPATH)
	@echo C_SRCS       = $(C_SRCS)
	@echo CPP_SRCS     = $(CPP_SRCS)
	@echo ASM_SRCS     = $(ASM_SRCS)
	@echo C_DEPS_EXT   = $(C_DEPS_EXT)
	@echo C_OBJS_EXT   = $(C_OBJS_EXT)
	@echo CPP_DEPS_EXT = $(CPP_DEPS_EXT)
	@echo CPP_OBJS_EXT = $(CPP_OBJS_EXT)
	@echo ASM_OBJS_EXT = $(ASM_OBJS_EXT)
	@echo LIB_DIRS     = $(LIB_DIRS)
	@echo LIBS         = $(LIBS)
	@echo DEFINES      = $(DEFINES)
	@echo QTOOLS       = $(QTOOLS)
	@echo HOST         = $(HOST)
	@echo QUTEST       = $(QUTEST)
	@echo TESTS        = $(TESTS)


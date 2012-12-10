##############################################################################
# Product: Makefile for QP/C, AT91SAM7S-EK, DevKit Pro (GNU), Vanilla
# Last Updated for Version: 4.5.02
# Date of the Last Update:  Nov 13, 2012
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
# mingw32-make
# mingw32-make CONF=rel
#
# cleaning configurations: Debug (default), Release, and Spy
# mingw32-make clean
# mingw32-make CONF=rel clean

#-----------------------------------------------------------------------------
# NOTE: the Makefile expects that the QPC environment variable is defined
# and points to the QP/C installation directory
#
ifndef QPC
$(error The QPC environment variable must be defined)
endif

#-----------------------------------------------------------------------------
# tools
#
ifeq ($(GNU_ARM),)
GNU_ARM = C:/tools/devkitPro/devkitARM/bin
endif

CC    := $(GNU_ARM)/arm-none-eabi-gcc
CPP   := $(GNU_ARM)/arm-none-eabi-g++
AS    := $(GNU_ARM)/arm-none-eabi-as
LINK  := $(GNU_ARM)/arm-none-eabi-gcc
BIN   := $(GNU_ARM)/arm-none-eabi-objcopy
RM    := rm -rf
MKDIR := mkdir

#-----------------------------------------------------------------------------
# directories
#
QP_PORT_DIR := $(QPC)/ports/arm/qk/gnu
APP_DIR     := .
LIB_DIR     :=

# source directories
VPATH = $(APP_DIR)

# Output file basename
OUTPUT    := dpp-qk

# include directories
INCLUDES  = -I"$(QPC)"/include	\
	-I$(QP_PORT_DIR) \
	-I.

# defines
DEFINES =

ARM_CORE = arm7tdmi

#-----------------------------------------------------------------------------
# files
#

# assembler source files
ASM_SRCS := $(wildcard *.s)

# C ARM source files
C_ARM_SRCS := isr.c bsp.c

# C THUMB source files
C_THUMB_SRCS := low_level_init.c main.c philo.c table.c


# C++ ARM source files
CPP_ARM_SRCS :=

# C++ THUMB source files
CPP_THUMB_SRCS :=

LD_SCRIPT := dpp-qk.ld
LIBS    := -lqp_$(ARM_CORE)

#-----------------------------------------------------------------------------
# build options for various configurations
#

ifeq (rel, $(CONF))       # Release configuration ............................

BIN_DIR := rel

ASFLAGS = -mcpu=$(ARM_CORE) -mthumb-interwork

CFLAGS = -Os -mcpu=$(ARM_CORE) -mthumb-interwork $(INCLUDES) $(DEFINES) \
	-mlong-calls -ffunction-sections -Wall -DNDBEBUG

CPPFLAGS = -Os -mcpu=$(ARM_CORE) -mthumb-interwork $(INCLUDES) $(DEFINES) \
	-fno-rtti -fno-exceptions \
	-mlong-calls -ffunction-sections -Wall -DNDBEBUG

LINKFLAGS = -T$(LD_SCRIPT) \
	-Wl,-Map,$(BIN_DIR)/$(OUTPUT).map,--cref,--gc-sections \
	-L$(QP_PORT_DIR)/$(BIN_DIR)

else

ifeq (spy, $(CONF))  # Spy configuration ................................

BIN_DIR := spy

ASFLAGS = -g -mcpu=$(ARM_CORE) -mthumb-interwork

CFLAGS = -g -O -mcpu=$(ARM_CORE) -mthumb-interwork $(INCLUDES) $(DEFINES) \
	-mlong-calls -ffunction-sections -Wall -DQ_SPY

CPPFLAGS = -g -O -mcpu=$(ARM_CORE) -mthumb-interwork $(INCLUDES) $(DEFINES) \
	-fno-rtti -fno-exceptions \
	-mlong-calls -ffunction-sections -Wall -DQ_SPY

LINKFLAGS = -T$(LD_SCRIPT) \
	-Wl,-Map,$(BIN_DIR)/$(OUTPUT).map,--cref,--gc-sections \
	-L$(QP_PORT_DIR)/$(BIN_DIR)

else                # default Debug configuration .......................

BIN_DIR := dbg

ASFLAGS = -g -mcpu=$(ARM_CORE) -mthumb-interwork

CFLAGS = -g -O -mcpu=$(ARM_CORE) -mthumb-interwork $(INCLUDES) $(DEFINES) \
	-mlong-calls -ffunction-sections -Wall

CPPFLAGS = -g -O -mcpu=$(ARM_CORE) -mthumb-interwork $(INCLUDES) $(DEFINES) \
	-fno-rtti -fno-exceptions \
	-mlong-calls -ffunction-sections -Wall

LINKFLAGS = -T$(LD_SCRIPT) \
	-Wl,-Map,$(BIN_DIR)/$(OUTPUT).map,--cref,--gc-sections \
	-L$(QP_PORT_DIR)/$(BIN_DIR)

endif

endif


ASM_OBJS       := $(patsubst %.s,%.o,$(ASM_SRCS))
C_ARM_OBJS     := $(patsubst %.c,%.oa,$(C_ARM_SRCS))
CPP_ARM_OBJS   := $(patsubst %.cpp,%.oa,$(CPP_ARM_SRCS))
C_THUMB_OBJS   := $(patsubst %.c,%.ot,$(C_THUMB_SRCS))
CPP_THUMB_OBJS := $(patsubst %.cpp,%.ot,$(CPP_THUMB_SRCS))

TARGET_BIN         := $(BIN_DIR)/$(OUTPUT).bin
TARGET_ELF         := $(BIN_DIR)/$(OUTPUT).elf
ASM_OBJS_EXT       := $(addprefix $(BIN_DIR)/, $(ASM_OBJS))
C_ARM_OBJS_EXT     := $(addprefix $(BIN_DIR)/, $(C_ARM_OBJS))
C_THUMB_OBJS_EXT   := $(addprefix $(BIN_DIR)/, $(C_THUMB_OBJS))
C_DEPS_EXT         := $(patsubst %.oa, %.d, $(C_ARM_OBJS_EXT))
C_DEPS_EXT         += $(patsubst %.ot, %.d, $(C_THUMB_OBJS_EXT))
CPP_ARM_OBJS_EXT   := $(addprefix $(BIN_DIR)/, $(CPP_ARM_OBJS))
CPP_THUMB_OBJS_EXT := $(addprefix $(BIN_DIR)/, $(CPP_THUMB_OBJS))
CPP_DEPS_EXT       := $(patsubst %.oa, %.d, $(CPP_ARM_OBJS_EXT))
CPP_DEPS_EXT       += $(patsubst %.ot, %.d, $(CPP_THUMB_OBJS_EXT))

#-----------------------------------------------------------------------------
# rules
#

all: $(BIN_DIR) $(TARGET_ELF)
#all: $(BIN_DIR) $(TARGET_BIN)

$(BIN_DIR):
	@echo
	mkdir -p $@

$(TARGET_BIN): $(TARGET_ELF)
	$(BIN) -O binary $< $@

$(TARGET_ELF) : $(ASM_OBJS_EXT) $(C_ARM_OBJS_EXT) $(CPP_ARM_OBJS_EXT) \
	$(C_THUMB_OBJS_EXT) $(CPP_THUMB_OBJS_EXT)
	$(LINK) $(LINKFLAGS) -o $@ $^ $(LIBS)

$(BIN_DIR)/%.d : %.c
	$(CC) -MM -MT $(@:.d=.o) $(CFLAGS) $< > $@

$(BIN_DIR)/%.d : %.cpp
	$(CPP) -MM -MT $(@:.d=.o) $(CPPFLAGS) $< > $@

$(BIN_DIR)/%.o : %.s
	$(AS) $(ASFLAGS) $< -o $@

$(BIN_DIR)/%.oa : %.c
	$(CC) $(CFLAGS) -marm -c $< -o $@

$(BIN_DIR)/%.ot : %.c
	$(CC) $(CFLAGS) -mthumb -c $< -o $@

$(BIN_DIR)/%.oa : %.cpp
	$(CPP) $(CPPFLAGS) -marm -c $< -o $@

$(BIN_DIR)/%.ot : %.cpp
	$(CPP) $(CPPFLAGS) -mthumb -c $< -o $@

-include $(C_DEPS_EXT) $(CPP_DEPS_EXT)


.PHONY : clean
clean:
	-$(RM) $(BIN_DIR)/*.o $(BIN_DIR)/*.oa $(BIN_DIR)/*.ot \
	$(BIN_DIR)/*.d \
	$(BIN_DIR)/*.bin \
	$(BIN_DIR)/*.elf \
	$(BIN_DIR)/*.map
	
show:
	@echo CONF = $(CONF)
	@echo ASM_SRCS = $(ASM_SRCS)
	@echo C_ARM_SRCS = $(C_ARM_SRCS)
	@echo C_THUMB_SRCS = $(C_THUMB_SRCS)
	@echo CPP_ARM_SRCS = $(CPP_ARM_SRCS)
	@echo CPP_THUMB_SRCS = $(CPP_THUMB_SRCS)
	@echo ASM_OBJS_EXT = $(ASM_OBJS_EXT)
	@echo C_ARM_OBJS_EXT = $(C_ARM_OBJS_EXT)
	@echo C_THUMB_OBJS_EXT = $(C_THUMB_OBJS_EXT)
	@echo CPP_ARM_OBJS_EXT = $(CPP_ARM_OBJS_EXT)
	@echo CPP_THUMB_OBJS_EXT = $(CPP_THUMB_OBJS_EXT)
	@echo C_DEPS_EXT = $(C_DEPS_EXT)
	@echo CPP_DEPS_EXT = $(CPP_DEPS_EXT)
	@echo TARGET_ELF = $(TARGET_ELF)

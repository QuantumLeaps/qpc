##############################################################################
# Product: QDK/C for AT91SAM7S-EK, QK kernel, GNU/Sourcery G++ toolset
# Last Updated for Version: 4.4.00
# Date of the Last Update:  Feb 26, 2012
#
#                    Q u a n t u m     L e a P s
#                    ---------------------------
#                    innovating embedded systems
#
# Copyright (C) 2002-2012 Quantum Leaps, LLC. All rights reserved.
#
# This software may be distributed and modified under the terms of the GNU
# General Public License version 2 (GPL) as published by the Free Software
# Foundation and appearing in the file GPL.TXT included in the packaging of
# this file. Please note that GPL Section 2[b] requires that all works based
# on this software must also be made publicly available under the terms of
# the GPL ("Copyleft").
#
# Alternatively, this software may be distributed and modified under the
# terms of Quantum Leaps commercial licenses, which expressly supersede
# the GPL and are specifically designed for licensees interested in
# retaining the proprietary status of their code.
#
# Contact information:
# Quantum Leaps Web site:  http://www.quantum-leaps.com
# e-mail:                  info@quantum-leaps.com
##############################################################################

#-----------------------------------------------------------------------------
# tools
#
ifeq ($(GNU_ARM),)
GNU_ARM = C:/tools/devkitPro/devkitARM/bin
endif

CC    := $(GNU_ARM)/arm-eabi-gcc
CPP   := $(GNU_ARM)/arm-eabi-g++
ASM   := $(GNU_ARM)/arm-eabi-as
LINK  := $(GNU_ARM)/arm-eabi-gcc
BIN   := $(GNU_ARM)/arm-eabi-objcopy
RM    := rm -rf
MKDIR := mkdir

QP_INCDIR = $(QPC)/include
QP_PRTDIR = $(QPC)/ports/arm/qk/gnu
QP_LIBDIR = $(QP_PRTDIR)/$(BINDIR)
BLDDIR    = .

CCINC   = -I$(QP_INCDIR) -I$(QP_PRTDIR) -I$(BLDDIR)

APP_DEP = $(BLDDIR)/bsp.h \
	$(BLDDIR)/dpp.h
	
APP_NAME = dpp-qk
ARM_CORE = arm7tdmi


ifeq (rel, $(CONF))       # Release configuration ............................

BINDIR   = rel

CCFLAGS = -c -mcpu=$(ARM_CORE) -mthumb-interwork -Os \
	-mlong-calls -ffunction-sections -Wall -DNDBEBUG -o$@

ASMFLAGS = -mcpu=$(ARM_CORE) -mthumb-interwork -o$@

LINKFLAGS = -T .\$(APP_NAME).ld -o $(BINDIR)\$(APP_NAME).elf \
	-Wl,-Map,$(BINDIR)\$(APP_NAME).map,--cref,--gc-sections

else ifeq (spy, $(CONF))  # Spy configuration ................................

BINDIR   = spy

QS_LIB = $(QP_LIBDIR)/libqs_$(ARM_CORE).a

CCFLAGS = -g -c -mcpu=$(ARM_CORE) -mthumb-interwork -O \
	-mlong-calls -ffunction-sections -Wall -DQ_SPY -o$@

ASMFLAGS = -g -mcpu=$(ARM_CORE) -mthumb-interwork -o$@

LINKFLAGS = -T .\$(APP_NAME).ld -o $(BINDIR)\$(APP_NAME).elf \
	-Wl,-Map,$(BINDIR)\$(APP_NAME).map,--cref,--gc-sections

else                     # default Debug configuration .......................

BINDIR   = dbg

CCFLAGS = -g -c -mcpu=$(ARM_CORE) -mthumb-interwork -O \
	-mlong-calls -ffunction-sections -Wall -o$@

ASMFLAGS = -g -mcpu=$(ARM_CORE) -mthumb-interwork -o$@

LINKFLAGS = -T .\$(APP_NAME).ld -o $(BINDIR)\$(APP_NAME).elf \
	-Wl,-Map,$(BINDIR)\$(APP_NAME).map,--cref,--gc-sections

endif


#.............................................................................
.PHONY: all

all: $(BINDIR)/$(APP_NAME).elf

$(BINDIR)/$(APP_NAME).elf : \
	./$(APP_NAME).ld \
	$(BINDIR)/startup.o \
	$(BINDIR)/low_level_init.o \
	$(BINDIR)/main.o \
	$(BINDIR)/bsp.o \
	$(BINDIR)/isr.o \
	$(BINDIR)/philo.o \
	$(BINDIR)/table.o \
	$(BINDIR)/no_heap.o	\
	$(QP_LIBDIR)/libqk_$(ARM_CORE).a \
	$(QP_LIBDIR)/libqf_$(ARM_CORE).a \
	$(QP_LIBDIR)/libqep_$(ARM_CORE).a \
	$(QS_LIB)
	$(LINK) \
	$(BINDIR)/startup.o \
	$(BINDIR)/low_level_init.o \
	$(BINDIR)/main.o \
	$(BINDIR)/bsp.o \
	$(BINDIR)/isr.o \
	$(BINDIR)/philo.o \
	$(BINDIR)/table.o \
	$(BINDIR)/no_heap.o	\
	$(QP_LIBDIR)/libqk_$(ARM_CORE).a \
	$(QP_LIBDIR)/libqf_$(ARM_CORE).a \
	$(QP_LIBDIR)/libqep_$(ARM_CORE).a \
	$(QS_LIB) \
	$(LINKFLAGS)

$(BINDIR)/startup.o: $(BLDDIR)/startup.s $(APP_DEP)
	$(ASM) $(ASMFLAGS) $<

$(BINDIR)/low_level_init.o: $(BLDDIR)/low_level_init.c $(APP_DEP)
	$(CC) -marm   $(CCFLAGS) $(CCINC) $<

$(BINDIR)/main.o: $(BLDDIR)/main.c $(APP_DEP)
	$(CC) -mthumb $(CCFLAGS) $(CCINC) $<

$(BINDIR)/bsp.o: $(BLDDIR)/bsp.c $(APP_DEP)
	$(CC) -marm   $(CCFLAGS) $(CCINC) $<

$(BINDIR)/isr.o: $(BLDDIR)/isr.c $(APP_DEP)
	$(CC) -marm   $(CCFLAGS) $(CCINC) $<

$(BINDIR)/philo.o: $(BLDDIR)/philo.c $(APP_DEP)
	$(CC) -mthumb $(CCFLAGS) $(CCINC) $<

$(BINDIR)/table.o: $(BLDDIR)/table.c $(APP_DEP)
	$(CC) -mthumb $(CCFLAGS) $(CCINC) $<

$(BINDIR)/no_heap.o: $(BLDDIR)/no_heap.c $(APP_DEP)
	$(CC) -mthumb $(CCFLAGS) $(CCINC) $<


.PHONY : clean
clean:
	-$(RM) $(BINDIR)/*.o \
	$(BINDIR)/*.elf \
	$(BINDIR)/*.map


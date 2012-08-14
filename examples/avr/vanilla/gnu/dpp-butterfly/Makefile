##############################################################################
# Product: DPP example
# Last Updated for Version: 4.0.01
# Date of the Last Update:  Aug 07, 2008
#
#                    Q u a n t u m     L e a P s
#                    ---------------------------
#                    innovating embedded systems
#
# Copyright (C) 2002-2008 Quantum Leaps, LLC. All rights reserved.
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
# general utilities
RM = erase

# GNU-AVR tools directory
GNU_AVR = C:\tools\GNU\WinAVR

# Adjust the Target MCU to your project
TARGET_MCU = atmega169

# Adjust the Application Name to your project
APP_NAME = dpp-butterfly

# The gcc compiler and linker
CC      = $(GNU_AVR)\bin\avr-gcc
LINK    = $(GNU_AVR)\bin\avr-gcc
OBJCOPY = $(GNU_AVR)\bin\avr-objcopy
OBJDUMP = $(GNU_AVR)\bin\avr-objdump

QP_INCDIR = ../../../../../include
QP_PRTDIR = ../../../../../ports/avr/vanilla/gnu
QP_LIBDIR = $(QP_PRTDIR)/$(BINDIR)
BLDDIR    = .
LINKFLAGS =
QS_LIB    =


CCINC    := -I$(QP_INCDIR) -I$(QP_PRTDIR) -I$(BLDDIR)
QP_DEP   := $(QP_INCDIR)/qassert.h \
	$(QP_INCDIR)/qep.h \
	$(QP_PRTDIR)/qep_port.h	\
	$(QP_INCDIR)/qf.h \
	$(QP_PRTDIR)/qf_port.h

APP_DEP  := $(BLDDIR)/dpp.h \
	$(BLDDIR)/bsp.h

# dbg (default target) .......................................................

BINDIR   = dbg

# gcc options
# -c    : compile only
# -g    : source level debugging
CCFLAGS  = -c -gdwarf-2 -std=gnu99 -Os -fsigned-char -fshort-enums \
	-mmcu=$(TARGET_MCU) -Wall -o$@

LINKFLAGS = -mmcu=$(TARGET_MCU)


# release ....................................................................
ifeq ($(MAKECMDGOALS), rel)

BINDIR   = rel

# gcc options
# -c    : compile only
# -O    : optimize
CCFLAGS  = -c -DNDEBUG -std=gnu99 -Os -fsigned-char -fshort-enums \
	-mmcu=$(TARGET_MCU) -Wall -o$@

LINKFLAGS =	-mmcu=$(TARGET_MCU)

endif
ifeq ($(MAKECMDGOALS), rel_clean)

BINDIR   = rel

endif

# spy ........................................................................
ifeq ($(MAKECMDGOALS), spy)

BINDIR   = spy

# gcc options
# -c    : compile only
# -O    : optimize
CCFLAGS  = -c -gdwarf-2 -DQ_SPY -std=gnu99 -Os -fsigned-char -fshort-enums \
	-mmcu=$(TARGET_MCU) -Wall -o$@

LINKFLAGS =	-mmcu=$(TARGET_MCU)
QS_LIB    =	$(QP_LIBDIR)\libqs_$(TARGET_MCU).a

QP_DEP  := $(QP_DEP) \
	$(QP_INCDIR)\qs.h \
	$(QP_PRTDIR)\qs_port.h

endif
ifeq ($(MAKECMDGOALS), spy_clean)

BINDIR   = spy

endif

#.............................................................................

all: $(BINDIR)\$(APP_NAME).hex

$(BINDIR)\$(APP_NAME).hex : $(BINDIR)\$(APP_NAME).elf
	$(OBJCOPY) -j .text -j .data -O ihex $< $@

$(BINDIR)\$(APP_NAME).elf : \
	$(BINDIR)\main.o \
	$(BINDIR)\bsp.o \
	$(BINDIR)\philo.o \
	$(BINDIR)\table.o \
	$(QP_LIBDIR)\libqf_$(TARGET_MCU).a \
	$(QP_LIBDIR)\libqep_$(TARGET_MCU).a $(QS_LIB)
	$(LINK) $(LINKFLAGS) \
	-o $(BINDIR)\$(APP_NAME).elf \
	-Wl,-Map,$(BINDIR)\$(APP_NAME).map,--cref \
	$(BINDIR)\main.o \
	$(BINDIR)\bsp.o \
	$(BINDIR)\philo.o \
	$(BINDIR)\table.o \
	$(QP_LIBDIR)\libqf_$(TARGET_MCU).a \
	$(QP_LIBDIR)\libqep_$(TARGET_MCU).a $(QS_LIB)

$(BINDIR)\main.o: $(BLDDIR)\main.c $(APP_DEP) $(QP_DEP)
	$(CC) $(CCFLAGS) $(CCINC) $<

$(BINDIR)\bsp.o: $(BLDDIR)\bsp.c $(APP_DEP) $(QP_DEP)
	$(CC) $(CCFLAGS) $(CCINC) $<

$(BINDIR)\philo.o: $(BLDDIR)\philo.c $(APP_DEP) $(QP_DEP)
	$(CC) $(CCFLAGS) $(CCINC) $<

$(BINDIR)\table.o: $(BLDDIR)\table.c $(APP_DEP) $(QP_DEP)
	$(CC) $(CCFLAGS) $(CCINC) $<


# build configuration targets...
dbg: all

rel: all

spy: all


# clean targets...

.PHONY: clean rel_clean spy_clean

rel_clean: clean

dbg_clean: clean

spy_clean: clean

clean:
	-$(RM) $(BINDIR)\*.o
	-$(RM) $(BINDIR)\*.elf
	-$(RM) $(BINDIR)\*.hex
	-$(RM) $(BINDIR)\*.map

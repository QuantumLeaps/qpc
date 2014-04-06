##############################################################################
# Product:  QP/C, Win32 build, GNU compiler (MinGW)
# Last Updated for Version: 5.3.0
# Date of the Last Update:  2014-03-30
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
# Web:   www.state-machine.com
# Email: info@state-machine.com
##############################################################################

# examples of invoking this Makefile:
# building configurations: Debug (default), Release, and Spy
# mingw32-make
# mingw32-make CONF=rel
# mingw32-make CONF=spy
#
# cleaning configurations: Debug (default), Release, and Spy
# mingw32-make clean
# mingw32-make CONF=rel clean
# mingw32-make CONF=spy clean

#-----------------------------------------------------------------------------
# general utilities
#

#-----------------------------------------------------------------------------
# NOTE: this Makefile assumes that the MinGW\bin directory is added
# to the PATH variable.
#
CC  = gcc
LIB = ar
RM  = rm

QPC        = ../../..
QP_PRTDIR  = ..
QP_INCDIR  = $(QPC)/include
QEP_SRCDIR = $(QPC)/qep\source
QF_SRCDIR  = $(QPC)/qf\source
QS_SRCDIR  = $(QPC)/qs\source

CCINC      = -I$(QP_PRTDIR) -I$(QP_INCDIR)
LIBFLAGS   = rs

#-----------------------------------------------------------------------------
# build options for various configurations
#
ifeq (rel, $(CONF))       # Release configuration ............................

BINDIR  = rel

# gcc options
# -c    : compile only
# -O    : optimize
CCFLAGS = -c -O2 -Wall -W -o$@

else ifeq (spy, $(CONF))  # Spy configuration ................................

BINDIR   = spy

# gcc options
# -c    : compile only
# -g    : source level debugging
# -D    : define preprocessor macro
CCFLAGS = -c -g -O -Wall -W -DQ_SPY -o$@
QS_OBJS = \
	$(BINDIR)/qs.o \
	$(BINDIR)/qs_.o \
	$(BINDIR)/qs_blk.o \
	$(BINDIR)/qs_byte.o \
	$(BINDIR)/qs_dict.o \
	$(BINDIR)/qs_f32.o \
	$(BINDIR)/qs_f64.o \
	$(BINDIR)/qs_mem.o \
	$(BINDIR)/qs_str.o \
	$(BINDIR)/qs_u64.o

else                     # default Debug configuration .......................

BINDIR  = dbg
QP_DEP  = $(QP_INCDIR)/qassert.h

# gcc options
# -c    : compile only
# -g    : source level debugging
CCFLAGS = -c -g -O -Wall -W -o$@

endif

VPATH = $(QEP_SRCDIR) $(QF_SRCDIR) $(QS_SRCDIR)

# the objects to build =======================================================
QEP_OBJS = \
	$(BINDIR)/qep.o \
	$(BINDIR)/qmsm_ini.o \
	$(BINDIR)/qmsm_dis.o \
	$(BINDIR)/qmsm_in.o \
	$(BINDIR)/qfsm_ini.o \
	$(BINDIR)/qfsm_dis.o \
	$(BINDIR)/qhsm_ini.o \
	$(BINDIR)/qhsm_dis.o \
	$(BINDIR)/qhsm_top.o \
	$(BINDIR)/qhsm_in.o

QF_OBJS = \
	$(BINDIR)/qa_ctor.o \
	$(BINDIR)/qa_defer.o \
	$(BINDIR)/qa_fifo.o \
	$(BINDIR)/qa_lifo.o \
	$(BINDIR)/qa_get_.o \
	$(BINDIR)/qa_sub.o \
	$(BINDIR)/qa_usub.o \
	$(BINDIR)/qa_usuba.o \
	$(BINDIR)/qeq_init.o \
	$(BINDIR)/qeq_fifo.o \
	$(BINDIR)/qeq_lifo.o \
	$(BINDIR)/qeq_get.o \
	$(BINDIR)/qf_act.o \
	$(BINDIR)/qf_gc.o \
	$(BINDIR)/qf_log2.o \
	$(BINDIR)/qf_new.o \
	$(BINDIR)/qf_pool.o \
	$(BINDIR)/qf_psini.o \
	$(BINDIR)/qf_pspub.o \
	$(BINDIR)/qf_pwr2.o \
	$(BINDIR)/qf_tick.o \
	$(BINDIR)/qma_ctor.o \
	$(BINDIR)/qmp_get.o \
	$(BINDIR)/qmp_init.o \
	$(BINDIR)/qmp_put.o \
	$(BINDIR)/qte_ctor.o \
	$(BINDIR)/qte_arm.o \
	$(BINDIR)/qte_darm.o \
	$(BINDIR)/qte_rarm.o \
	$(BINDIR)/qte_ctr.o \
	$(BINDIR)/qf_port.o \
	$(BINDIR)/win32_gui.o

# the targets ================================================================

all: $(BINDIR) $(BINDIR)/libqp.a
	-$(RM) $(BINDIR)/*.o

$(BINDIR):
	mkdir $@

$(BINDIR)/libqp.a: 	$(QEP_OBJS) $(QF_OBJS) $(QS_OBJS)
	-$(RM) $(BINDIR)/libqp.a
	$(LIB) $(LIBFLAGS) $(BINDIR)/libqp.a \
	$(QEP_OBJS) \
	$(QF_OBJS) \
	$(QS_OBJS)

# QEP -----------------------------------------
$(BINDIR)/qep.o: $(QEP_SRCDIR)/qep.c
	$(CC) $(CCFLAGS) $(CCINC) -I$(QEP_SRCDIR) $<

$(BINDIR)/qmsm_ini.o: $(QEP_SRCDIR)/qmsm_ini.c
	$(CC) $(CCFLAGS) $(CCINC) -I$(QEP_SRCDIR) $<

$(BINDIR)/qmsm_dis.o: $(QEP_SRCDIR)/qmsm_dis.c
	$(CC) $(CCFLAGS) $(CCINC) -I$(QEP_SRCDIR) $<

$(BINDIR)/qmsm_in.o: $(QEP_SRCDIR)/qmsm_in.c
	$(CC) $(CCFLAGS) $(CCINC) -I$(QEP_SRCDIR) $<

$(BINDIR)/qfsm_ini.o: $(QEP_SRCDIR)/qfsm_ini.c
	$(CC) $(CCFLAGS) $(CCINC) -I$(QEP_SRCDIR) $<

$(BINDIR)/qfsm_dis.o: $(QEP_SRCDIR)/qfsm_dis.c
	$(CC) $(CCFLAGS) $(CCINC) -I$(QEP_SRCDIR) $<

$(BINDIR)/qhsm_ini.o: $(QEP_SRCDIR)/qhsm_ini.c
	$(CC) $(CCFLAGS) $(CCINC) -I$(QEP_SRCDIR) $<

$(BINDIR)/qhsm_dis.o: $(QEP_SRCDIR)/qhsm_dis.c
	$(CC) $(CCFLAGS) $(CCINC) -I$(QEP_SRCDIR) $<

$(BINDIR)/qhsm_top.o: $(QEP_SRCDIR)/qhsm_top.c
	$(CC) $(CCFLAGS) $(CCINC) -I$(QEP_SRCDIR) $<

$(BINDIR)/qhsm_in.o: $(QEP_SRCDIR)/qhsm_in.c
	$(CC) $(CCFLAGS) $(CCINC) -I$(QEP_SRCDIR) $<

# QF -----------------------------------------
$(BINDIR)/qa_ctor.o: $(QF_SRCDIR)/qa_ctor.c
	$(CC) $(CCFLAGS) $(CCINC) -I$(QF_SRCDIR) $<

$(BINDIR)/qa_defer.o: $(QF_SRCDIR)/qa_defer.c
	$(CC) $(CCFLAGS) $(CCINC) -I$(QF_SRCDIR) $<

$(BINDIR)/qa_fifo.o: $(QF_SRCDIR)/qa_fifo.c
	$(CC) $(CCFLAGS) $(CCINC) -I$(QF_SRCDIR) $<

$(BINDIR)/qa_lifo.o: $(QF_SRCDIR)/qa_lifo.c
	$(CC) $(CCFLAGS) $(CCINC) -I$(QF_SRCDIR) $<

$(BINDIR)/qa_get_.o: $(QF_SRCDIR)/qa_get_.c
	$(CC) $(CCFLAGS) $(CCINC) -I$(QF_SRCDIR) $<

$(BINDIR)/qa_sub.o: $(QF_SRCDIR)/qa_sub.c
	$(CC) $(CCFLAGS) $(CCINC) -I$(QF_SRCDIR) $<

$(BINDIR)/qa_usub.o: $(QF_SRCDIR)/qa_usub.c
	$(CC) $(CCFLAGS) $(CCINC) -I$(QF_SRCDIR) $<

$(BINDIR)/qa_usuba.o: $(QF_SRCDIR)/qa_usuba.c
	$(CC) $(CCFLAGS) $(CCINC) -I$(QF_SRCDIR) $<

$(BINDIR)/qeq_init.o: $(QF_SRCDIR)/qeq_init.c
	$(CC) $(CCFLAGS) $(CCINC) -I$(QF_SRCDIR) $<

$(BINDIR)/qeq_fifo.o: $(QF_SRCDIR)/qeq_fifo.c
	$(CC) $(CCFLAGS) $(CCINC) -I$(QF_SRCDIR) $<

$(BINDIR)/qeq_lifo.o: $(QF_SRCDIR)/qeq_lifo.c
	$(CC) $(CCFLAGS) $(CCINC) -I$(QF_SRCDIR) $<

$(BINDIR)/qeq_get.o: $(QF_SRCDIR)/qeq_get.c
	$(CC) $(CCFLAGS) $(CCINC) -I$(QF_SRCDIR) $<

$(BINDIR)/qf_act.o: $(QF_SRCDIR)/qf_act.c
	$(CC) $(CCFLAGS) $(CCINC) -I$(QF_SRCDIR) $<

$(BINDIR)/qf_gc.o: $(QF_SRCDIR)/qf_gc.c
	$(CC) $(CCFLAGS) $(CCINC) -I$(QF_SRCDIR) $<

$(BINDIR)/qf_log2.o: $(QF_SRCDIR)/qf_log2.c
	$(CC) $(CCFLAGS) $(CCINC) -I$(QF_SRCDIR) $<

$(BINDIR)/qf_new.o: $(QF_SRCDIR)/qf_new.c
	$(CC) $(CCFLAGS) $(CCINC) -I$(QF_SRCDIR) $<

$(BINDIR)/qf_pool.o: $(QF_SRCDIR)/qf_pool.c
	$(CC) $(CCFLAGS) $(CCINC) -I$(QF_SRCDIR) $<

$(BINDIR)/qf_psini.o: $(QF_SRCDIR)/qf_psini.c
	$(CC) $(CCFLAGS) $(CCINC) -I$(QF_SRCDIR) $<

$(BINDIR)/qf_pspub.o: $(QF_SRCDIR)/qf_pspub.c
	$(CC) $(CCFLAGS) $(CCINC) -I$(QF_SRCDIR) $<

$(BINDIR)/qf_pwr2.o: $(QF_SRCDIR)/qf_pwr2.c
	$(CC) $(CCFLAGS) $(CCINC) -I$(QF_SRCDIR) $<

$(BINDIR)/qf_tick.o: $(QF_SRCDIR)/qf_tick.c
	$(CC) $(CCFLAGS) $(CCINC) -I$(QF_SRCDIR) $<

$(BINDIR)/qma_ctor.o: $(QF_SRCDIR)/qma_ctor.c
	$(CC) $(CCFLAGS) $(CCINC) -I$(QF_SRCDIR) $<

$(BINDIR)/qmp_get.o: $(QF_SRCDIR)/qmp_get.c
	$(CC) $(CCFLAGS) $(CCINC) -I$(QF_SRCDIR) $<

$(BINDIR)/qmp_init.o: $(QF_SRCDIR)/qmp_init.c
	$(CC) $(CCFLAGS) $(CCINC) -I$(QF_SRCDIR) $<

$(BINDIR)/qmp_put.o: $(QF_SRCDIR)/qmp_put.c
	$(CC) $(CCFLAGS) $(CCINC) -I$(QF_SRCDIR) $<

$(BINDIR)/qte_ctor.o: $(QF_SRCDIR)/qte_ctor.c
	$(CC) $(CCFLAGS) $(CCINC) -I$(QF_SRCDIR) $<

$(BINDIR)/qte_arm.o: $(QF_SRCDIR)/qte_arm.c
	$(CC) $(CCFLAGS) $(CCINC) -I$(QF_SRCDIR) $<

$(BINDIR)/qte_darm.o: $(QF_SRCDIR)/qte_darm.c
	$(CC) $(CCFLAGS) $(CCINC) -I$(QF_SRCDIR) $<

$(BINDIR)/qte_rarm.o: $(QF_SRCDIR)/qte_rarm.c
	$(CC) $(CCFLAGS) $(CCINC) -I$(QF_SRCDIR) $<

$(BINDIR)/qte_ctr.o: $(QF_SRCDIR)/qte_ctr.c
	$(CC) $(CCFLAGS) $(CCINC) -I$(QF_SRCDIR) $<

$(BINDIR)/qf_port.o: $(QP_PRTDIR)/qf_port.c
	$(CC) $(CCFLAGS) $(CCINC) -I$(QF_SRCDIR) $<

$(BINDIR)/win32_gui.o: $(QP_PRTDIR)/win32_gui.c
	$(CC) $(CCFLAGS) $(CCINC) -I$(QF_SRCDIR) $<

# QS -----------------------------------------

$(BINDIR)/qs.o: $(QS_SRCDIR)/qs.c
	$(CC) $(CCFLAGS) $(CCINC) -I$(QS_SRCDIR) $<

$(BINDIR)/qs_.o: $(QS_SRCDIR)/qs_.c
	$(CC) $(CCFLAGS) $(CCINC) -I$(QS_SRCDIR) $<

$(BINDIR)/qs_blk.o: $(QS_SRCDIR)/qs_blk.c
	$(CC) $(CCFLAGS) $(CCINC) -I$(QS_SRCDIR) $<

$(BINDIR)/qs_byte.o: $(QS_SRCDIR)/qs_byte.c
	$(CC) $(CCFLAGS) $(CCINC) -I$(QS_SRCDIR) $<

$(BINDIR)/qs_dict.o: $(QS_SRCDIR)/qs_dict.c
	$(CC) $(CCFLAGS) $(CCINC) -I$(QS_SRCDIR) $<

$(BINDIR)/qs_f32.o: $(QS_SRCDIR)/qs_f32.c
	$(CC) $(CCFLAGS) $(CCINC) -I$(QS_SRCDIR) $<

$(BINDIR)/qs_f64.o: $(QS_SRCDIR)/qs_f64.c
	$(CC) $(CCFLAGS) $(CCINC) -I$(QS_SRCDIR) $<

$(BINDIR)/qs_mem.o: $(QS_SRCDIR)/qs_mem.c
	$(CC) $(CCFLAGS) $(CCINC) -I$(QS_SRCDIR) $<

$(BINDIR)/qs_str.o: $(QS_SRCDIR)/qs_str.c
	$(CC) $(CCFLAGS) $(CCINC) -I$(QS_SRCDIR) $<

$(BINDIR)/qs_u64.o: $(QS_SRCDIR)/qs_u64.c
	$(CC) $(CCFLAGS) $(CCINC) -I$(QS_SRCDIR) $<


# clean ======================================================================
.PHONY : clean
clean:
	-$(RM) $(BINDIR)/*.o
	-$(RM) $(BINDIR)/*.a


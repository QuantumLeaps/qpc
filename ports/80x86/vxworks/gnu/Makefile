##############################################################################
# Product:  QP/C, VxWorks build
# Last Updated for Version: 4.0.00
# Date of the Last Update:  Apr 11, 2008
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
RM = rm

# The gcc compiler and ar librarian
CC  = gcc
LIB = ar

# the vxWorks directory relative to the port directory
# Please ADJUST for your project!
#
VXWORKS_DIR = ./vxworks

QP_INCDIR  = ../../../../include
QEP_SRCDIR = ../../../../qep/source
QF_SRCDIR  = ../../../../qf/source
QS_SRCDIR  = ../../../../qs/source
QP_PRTDIR  = .
LIBDIR     = $(BLDDIR)
LIBFLAGS   = rs

CCINC      = -I$(QP_PRTDIR) -I$(QP_INCDIR) -I$(VXWORKS_DIR)

# dbg (default target) .......................................................

BINDIR  = dbg
QP_DEP  = $(QP_INCDIR)/qassert.h

# gcc options
# -c    : compile only
# -g    : source level debugging
CCFLAGS  = -g -c -Wall -o$@

# release ....................................................................
ifeq ($(MAKECMDGOALS), rel)

BINDIR   = rel

# gcc options
# -c    : compile only
# -O    : optimize
CCFLAGS  = -O -c -Wall -o$@

endif
ifeq ($(MAKECMDGOALS), rel_clean)

BINDIR   = rel

endif

# spy ........................................................................
ifeq ($(MAKECMDGOALS), spy)

BINDIR   = spy

# gcc options
# -c    : compile only
# -g    : source level debugging
# -D    : define preprocessor macro
CCFLAGS  = -g -c -Wall -DQ_SPY -o$@

endif
ifeq ($(MAKECMDGOALS), spy_clean)

BINDIR   = spy

endif

# ............................................................................

LIBDIR = $(BINDIR)

# ............................................................................

dbg: $(LIBDIR)/libqep.a $(LIBDIR)/libqf.a

rel: $(LIBDIR)/libqep.a $(LIBDIR)/libqf.a

spy: $(LIBDIR)/libqep.a $(LIBDIR)/libqf.a	$(LIBDIR)/libqs.a

dbg_clean: clean

rel_clean: clean

spy_clean: clean

.PHONY: dbg rel spy
.PHONY: clean rel_clean spy_clean


# QEP ========================================================================

$(LIBDIR)/libqep.a: \
	$(BINDIR)/qep.o \
	$(BINDIR)/qfsm_ini.o \
	$(BINDIR)/qfsm_dis.o \
	$(BINDIR)/qhsm_ini.o \
	$(BINDIR)/qhsm_dis.o \
	$(BINDIR)/qhsm_top.o \
	$(BINDIR)/qhsm_in.o
	-$(RM) -f $(LIBDIR)/libqep.a
	$(LIB) $(LIBFLAGS) $(LIBDIR)/libqep.a \
	$(BINDIR)/qep.o \
	$(BINDIR)/qfsm_ini.o \
	$(BINDIR)/qfsm_dis.o \
	$(BINDIR)/qhsm_ini.o \
	$(BINDIR)/qhsm_dis.o \
	$(BINDIR)/qhsm_top.o \
	$(BINDIR)/qhsm_in.o
	-$(RM) $(BINDIR)/*.o

$(BINDIR)/qep.o: $(QEP_SRCDIR)/qep.c
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


# QF =========================================================================

$(LIBDIR)/libqf.a: \
	$(BINDIR)/qa_defer.o \
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
	$(BINDIR)/qmp_get.o \
	$(BINDIR)/qmp_init.o \
	$(BINDIR)/qmp_put.o \
	$(BINDIR)/qte_ctor.o \
	$(BINDIR)/qte_arm.o \
	$(BINDIR)/qte_darm.o \
	$(BINDIR)/qte_rarm.o \
	$(BINDIR)/qf_port.o
	-$(RM) -f $(LIBDIR)/libqf.a
	$(LIB) $(LIBFLAGS) $(LIBDIR)/libqf.a \
	$(BINDIR)/qa_defer.o \
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
	$(BINDIR)/qmp_get.o \
	$(BINDIR)/qmp_init.o \
	$(BINDIR)/qmp_put.o \
	$(BINDIR)/qte_ctor.o \
	$(BINDIR)/qte_arm.o \
	$(BINDIR)/qte_darm.o \
	$(BINDIR)/qte_rarm.o \
	$(BINDIR)/qf_port.o
	-$(RM) $(BINDIR)/*.o

$(BINDIR)/qa_defer.o: $(QF_SRCDIR)/qa_defer.c
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

$(BINDIR)/qf_port.o: src/qf_port.c
	$(CC) $(CCFLAGS) $(CCINC) -I$(QF_SRCDIR) $<


# spy ========================================================================
ifeq ($(MAKECMDGOALS), spy)

$(LIBDIR)/libqs.a: \
	$(BINDIR)/qs.o \
	$(BINDIR)/qs_.o \
	$(BINDIR)/qs_blk.o \
	$(BINDIR)/qs_byte.o \
	$(BINDIR)/qs_f32.o \
	$(BINDIR)/qs_f64.o \
	$(BINDIR)/qs_mem.o \
	$(BINDIR)/qs_str.o
	-$(RM) -f $(LIBDIR)/libqs.a
	$(LIB) $(LIBFLAGS) $(LIBDIR)/libqs.a \
	$(BINDIR)/qs.o \
	$(BINDIR)/qs_.o \
	$(BINDIR)/qs_blk.o \
	$(BINDIR)/qs_byte.o \
	$(BINDIR)/qs_f32.o \
	$(BINDIR)/qs_f64.o \
	$(BINDIR)/qs_mem.o \
	$(BINDIR)/qs_str.o
	-$(RM) $(BINDIR)/*.o

$(BINDIR)/qs.o: $(QS_SRCDIR)/qs.c
	$(CC) $(CCFLAGS) $(CCINC) -I$(QS_SRCDIR) $<

$(BINDIR)/qs_.o: $(QS_SRCDIR)/qs_.c
	$(CC) $(CCFLAGS) $(CCINC) -I$(QS_SRCDIR) $<

$(BINDIR)/qs_blk.o: $(QS_SRCDIR)/qs_blk.c
	$(CC) $(CCFLAGS) $(CCINC) -I$(QS_SRCDIR) $<

$(BINDIR)/qs_byte.o: $(QS_SRCDIR)/qs_byte.c
	$(CC) $(CCFLAGS) $(CCINC) -I$(QS_SRCDIR) $<

$(BINDIR)/qs_f32.o: $(QS_SRCDIR)/qs_f32.c
	$(CC) $(CCFLAGS) $(CCINC) -I$(QS_SRCDIR) $<

$(BINDIR)/qs_f64.o: $(QS_SRCDIR)/qs_f64.c
	$(CC) $(CCFLAGS) $(CCINC) -I$(QS_SRCDIR) $<

$(BINDIR)/qs_mem.o: $(QS_SRCDIR)/qs_mem.c
	$(CC) $(CCFLAGS) $(CCINC) -I$(QS_SRCDIR) $<

$(BINDIR)/qs_str.o: $(QS_SRCDIR)/qs_str.c
	$(CC) $(CCFLAGS) $(CCINC) -I$(QS_SRCDIR) $<

endif    # spy

clean:
	-$(RM) $(BINDIR)/*.o
	-$(RM) $(LIBDIR)/*.a


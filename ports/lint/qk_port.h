/*****************************************************************************
* Product:  QK/C port for Lint, Generic C compiler
* Last Updated for Version: 4.4.02
* Date of the Last Update:  Apr 13, 2012
*
*                    Q u a n t u m     L e a P s
*                    ---------------------------
*                    innovating embedded systems
*
* Copyright (C) 2002-2012 Quantum Leaps, LLC. All rights reserved.
*
* This software may be distributed and modified under the terms of the GNU
* General Public License version 2 (GPL) as published by the Free Software
* Foundation and appearing in the file GPL.TXT included in the packaging of
* this file. Please note that GPL Section 2[b] requires that all works based
* on this software must also be made publicly available under the terms of
* the GPL ("Copyleft").
*
* Alternatively, this software may be distributed and modified under the
* terms of Quantum Leaps commercial licenses, which expressly supersede
* the GPL and are specifically designed for licensees interested in
* retaining the proprietary status of their code.
*
* Contact information:
* Quantum Leaps Web site:  http://www.quantum-leaps.com
* e-mail:                  info@quantum-leaps.com
*****************************************************************************/
#ifndef qk_port_h
#define qk_port_h

/**
* \file
* \ingroup qk
* \brief QK/C port to QK for a "generic" C compiler.
*
* \note This is just an EXAMPLE of a QK port used for "linting" the QK.
*/

/****************************************************************************/
/* QK extended context save/restore */

/** \brief Define the method for saving the extended context (e.g.,
* the context of a floating-point co-processor).
*
* \note This is just an example of #QK_EXT_SAVE macro. You need to define
* the macro appropriately for the co-processor you're using. This macro
* is only used in the extended QK scheduler QK_scheduleExt_(). If you define
* #QK_EXT_SAVE, you also need to provide #QK_EXT_RESTORE and #QK_EXT_TYPE.
*/
#define QK_EXT_SAVE(act_)      FPU_save((FPU_context *)(act_)->thread)

/** \brief Define the method for restoring the extended context (e.g.,
* the context of a floating-point co-processor).
*
* \note This is just an example of #QK_EXT_RESTORE macro. You need to define
* the macro appropriately for the co-processor you're using. This macro
* is only used in the extended QK scheduler QK_scheduleExt_(). If you define
* #QK_EXT_RESTORE, you also need to provide #QK_EXT_SAVE and #QK_EXT_TYPE.
*/
#define QK_EXT_RESTORE(act_)   FPU_restore((FPU_context *)(act_)->thread)

typedef struct FPU_contextTag {
    uint8_t  fpu[108];               /* the x87 FPU context takes 108-bytes */
} FPU_context;
void FPU_save(FPU_context *ctx);                     /* defined in assembly */
void FPU_restore(FPU_context *ctx);                  /* defined in assembly */

/****************************************************************************/
/* Thread-Local-Storage switching */

/** \brief Define the method for switching the Thread-Local-Storage for
* for a given thread.
*
* \note This is just an example of #QK_TLS macro. You need to define
* the macro appropriately for the runtime library you're using. This macro
* is optional and you don't need to define it. The macro is used in both the
* regular QK scheduler QK_sched_() and the extended QK scheduler
* QK_schedExt_().
*/
#define QK_TLS(act_)        \
    (impure_ptr = (reent *)(act_)->thread)

/* fake struct reent and _impure_ptr elements of Newlib... */
typedef struct reentTag {
    uint32_t foo[32];
} reent;
extern reent *impure_ptr;

/****************************************************************************/
/* QK interrupt entry and exit */

/** \brief Define the ISR entry sequence, if the compiler supports writing
* interrupts in C.
*
* \note This is just an example of #QK_ISR_ENTRY. You need to define
* the macro appropriately for the CPU/compiler you're using. Also, some
* QK ports will not define this macro, but instead will provide ISR
* skeleton code in assembly.
*/
#define QK_ISR_ENTRY() do { \
    ++QK_intNest_; \
    QF_QS_ISR_ENTRY(QK_intNest_, QK_currPrio_); \
} while (0)


/** \brief Define the ISR exit sequence, if the compiler supports writing
* interrupts in C.
*
* \note This is just an example of #QK_ISR_EXIT. You need to define
* the macro appropriately for the CPU/compiler you're using. Also, some
* QK ports will not define this macro, but instead will provide ISR
* skeleton code in assembly.
*/
#define QK_ISR_EXIT() do { \
    send End-Of-Interrupt instruction to the Interrupt Controller; \
    QF_QS_ISR_EXIT(QK_intNest_, QK_currPrio_); \
    --QK_intNest_; \
    if (QK_intNest_ == (uint8_t)0) { \
        QK_scheduleExt_(); \
    } \
} while (0)


#include "qk.h"                 /* QK platform-independent public interface */

#endif                                                         /* qk_port_h */


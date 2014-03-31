/**
* \file
* \brief QK/C port example for a "generic" C compiler.
* \ingroup qk
* \cond
******************************************************************************
* Product: QF/C
* Last updated for version 5.3.0
* Last updated on  2014-03-01
*
*                    Q u a n t u m     L e a P s
*                    ---------------------------
*                    innovating embedded systems
*
* Copyright (C) Quantum Leaps, www.state-machine.com.
*
* This program is open source software: you can redistribute it and/or
* modify it under the terms of the GNU General Public License as published
* by the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* Alternatively, this program may be distributed and modified under the
* terms of Quantum Leaps commercial licenses, which expressly supersede
* the GNU General Public License and are specifically designed for
* licensees interested in retaining the proprietary status of their code.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*
* Contact information:
* Web:   www.state-machine.com
* Email: info@state-machine.com
******************************************************************************
* \endcond
*/
#ifndef qk_port_h
#define qk_port_h

/****************************************************************************/
/* QK extended context save/restore */

/*! Define the method for saving the extended context (e.g., the context
* of a floating-point co-processor).
*/
/**
* \description
* \note This is just an example of #QK_EXT_SAVE macro. You need to define
* the macro appropriately for the co-processor you're using. This macro
* is only used in the extended QK scheduler QK_scheduleExt_(). If you define
* #QK_EXT_SAVE, you also need to provide #QK_EXT_RESTORE and #QK_EXT_TYPE.
*/
#define QK_EXT_SAVE(act_)      (FPU_save((void *)(act_)->thread))

/*! Define the method for restoring the extended context (e.g.,
* the context of a floating-point co-processor).
*/
/**
* \note
* This is just an example of #QK_EXT_RESTORE macro. You need to define
* the macro appropriately for the co-processor you're using. This macro
* is only used in the extended QK scheduler QK_scheduleExt_(). If you define
* #QK_EXT_RESTORE, you also need to provide #QK_EXT_SAVE and #QK_EXT_TYPE.
*/
#define QK_EXT_RESTORE(act_)   (FPU_restore((void *)(act_)->thread))

/****************************************************************************/
/* Thread-Local-Storage switching */

/*! Define the method for switching the Thread-Local-Storage for
* for a given thread.
*/
/**
* \note This is just an example of #QK_TLS macro. You need to define
* the macro appropriately for the runtime library you're using. This macro
* is optional and you don't need to define it. The macro is used in both the
* regular QK scheduler QK_sched_() and the extended QK scheduler
* QK_schedExt_().
*/
#define QK_TLS(act_)        \
    (impure_ptr = (void *)(act_)->thread)

/****************************************************************************/
/* QK interrupt entry and exit */

/*! Define the ISR entry sequence, if the compiler supports writing
* interrupts in C.
*/
/**
* \note This is just an example of #QK_ISR_ENTRY. You need to define
* the macro appropriately for the CPU/compiler you're using. Also, some
* QK ports will not define this macro, but instead will provide ISR
* skeleton code in assembly.
*/
#define QK_ISR_ENTRY() do { \
    ++QK_intNest_; \
    QF_QS_ISR_ENTRY(QK_intNest_, QK_currPrio_); \
} while (0)


/*! Define the ISR exit sequence, if the compiler supports writing
* interrupts in C.
*/
/**
* \note
* This is just an example of #QK_ISR_EXIT. You need to define the macro
* appropriately for the CPU/compiler you're using. Also, some QK ports will
* not define this macro, but instead will provide ISR skeleton in assembly.
*/
#define QK_ISR_EXIT() do { \
    send End-Of-Interrupt instruction to the Interrupt Controller; \
    QF_QS_ISR_EXIT(QK_intNest_, QK_currPrio_); \
    --QK_intNest_; \
    if (QK_intNest_ == (uint8_t)0) { \
        QK_scheduleExt_(); \
    } \
} while (0)

void FPU_save(void *ctx);   /*!< example of extended context save */
void FPU_restore(void *ctx);/*!< example of extended context restore */
extern void *impure_ptr;    /*!< example of TLS pointer */

#include "qk.h" /* QK platform-independent public interface */

#endif /* qk_port_h */


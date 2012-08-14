/*****************************************************************************
* Product: QP/C
* Last Updated for Version: 4.5.02
* Date of the Last Update:  Jul 25, 2012
*
*                    Q u a n t u m     L e a P s
*                    ---------------------------
*                    innovating embedded systems
*
* Copyright (C) 2002-2012 Quantum Leaps, LLC. All rights reserved.
*
* This program is open source software: you can redistribute it and/or
* modify it under the terms of the GNU General Public License as published
* by the Free Software Foundation, either version 2 of the License, or
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
* Quantum Leaps Web sites: http://www.quantum-leaps.com
*                          http://www.state-machine.com
* e-mail:                  info@quantum-leaps.com
*****************************************************************************/
#ifndef qevt_h
#define qevt_h

/**
* \file
* \ingroup qep qf qk
* \brief QEvt class and basic macros used by all QP components.
*
* This header file must be included, perhaps indirectly, in all modules
* (*.c files) that use any component of QP/C (such as QEP, QF, or QK).
*/

/****************************************************************************/
/** \brief The current QP version number
*
* \return version of the QP as a hex constant constant 0xXYZZ, where X is
* a 1-digit major version number, Y is a 1-digit minor version number, and
* ZZ is a 2-digit release number.
*/
#define QP_VERSION      0x4502U

#ifndef Q_ROM
    /** \brief Macro to specify compiler-specific directive for placing a
    * constant object in ROM.
    *
    * Many compilers for 8-bit Harvard-architecture MCUs provide non-stanard
    * extensions to support placement of objects in different memories.
    * In order to conserve the precious RAM, QP uses the Q_ROM macro for
    * all constant objects that can be allocated in ROM.
    *
    * To override the following empty definition, you need to define the
    * Q_ROM macro in the qep_port.h header file. Some examples of valid
    * Q_ROM macro definitions are: __code (IAR 8051 compiler), code (Keil
    * Cx51 compiler), PROGMEM (gcc for AVR), __flash (IAR for AVR).
    */
    #define Q_ROM
#endif
#ifndef Q_ROM_VAR         /* if NOT defined, provide the default definition */

    /** \brief Macro to specify compiler-specific directive for accessing a
    * constant object in ROM.
    *
    * Many compilers for 8-bit MCUs provide different size pointers for
    * accessing objects in various memories. Constant objects allocated
    * in ROM (see #Q_ROM macro) often mandate the use of specific-size
    * pointers (e.g., far pointers) to get access to ROM objects. The
    * macro Q_ROM_VAR specifies the kind of the pointer to be used to access
    * the ROM objects.
    *
    * To override the following empty definition, you need to define the
    * Q_ROM_VAR macro in the qep_port.h header file. An example of valid
    * Q_ROM_VAR macro definition is: __far (Freescale HC(S)08 compiler).
    */
    #define Q_ROM_VAR
#endif
#ifndef Q_ROM_BYTE
    /** \brief Macro to access a byte allocated in ROM
    *
    * Some compilers for Harvard-architecture MCUs, such as gcc for AVR, do
    * not generate correct code for accessing data allocated in the program
    * space (ROM). The workaround for such compilers is to explictly add
    * assembly code to access each data element allocated in the program
    * space. The macro Q_ROM_BYTE() retrieves a byte from the given ROM
    * address.
    *
    * The Q_ROM_BYTE() macro should be defined for the compilers that
    * cannot handle correctly data allocated in ROM (such as the gcc).
    * If the macro is left undefined, the default definition simply returns
    * the argument and lets the compiler generate the correct code.
    */
    #define Q_ROM_BYTE(rom_var_)   (rom_var_)
#endif

/****************************************************************************/
#ifndef Q_SIGNAL_SIZE

    /** \brief The size (in bytes) of the signal of an event. Valid values:
    * 1, 2, or 4; default 1
    *
    * This macro can be defined in the QEP port file (qep_port.h) to
    * configure the ::QSignal type. When the macro is not defined, the
    * default of 2 bytes is applied.
    */
    #define Q_SIGNAL_SIZE 2
#endif
#if (Q_SIGNAL_SIZE == 1)
    typedef uint8_t QSignal;
#elif (Q_SIGNAL_SIZE == 2)
    /** \brief QSignal represents the signal of an event.
    *
    * The relationship between an event and a signal is as follows. A signal
    * in UML is the specification of an asynchronous stimulus that triggers
    * reactions [<A HREF="http://www.omg.org/docs/ptc/03-08-02.pdf">UML
    * document ptc/03-08-02</A>], and as such is an essential part of an
    * event. (The signal conveys the type of the occurrence-what happened?)
    * However, an event can also contain additional quantitative information
    * about the occurrence in form of event parameters.
    */
    typedef uint16_t QSignal;
#elif (Q_SIGNAL_SIZE == 4)
    typedef uint32_t QSignal;
#else
    #error "Q_SIGNAL_SIZE defined incorrectly, expected 1, 2, or 4"
#endif

/****************************************************************************/
/* typedefs for basic numerical types; MISRA-C 2004 rule 6.3(req).          */
/** \brief typedef for character strings.
*
* This typedef specifies character type for exclusive use in character
* strings. Use of this type, rather than plain 'char', is in compliance
* with the MISRA-C 2004 Rules 6.1(req), 6.3(adv).
*/
typedef char char_t;

/** typedef for 32-bit IEEE 754 floating point numbers */
typedef float float32_t;

/** typedef for 64-bit IEEE 754 floating point numbers */
typedef double float64_t;

/** typedef for enumerations used for event signals */
typedef int enum_t;

/****************************************************************************/
/** \brief Event structure.
*
* QEvt represents events without parameters and serves as the base structure
* for derivation of events with parameters.
*
* The following example illustrates how to add an event parameter by
* derivation of the QEvt structure. Please note that the QEvt member
* super_ is defined as the FIRST member of the derived struct.
* \include qep_qevent.c
*
* \sa \ref derivation
*/
typedef struct QEvtTag {
    QSignal sig;                          /**< signal of the event instance */
    uint8_t poolId_;                      /**< pool ID (0 for static event) */
    uint8_t refCtr_;                                 /**< reference counter */
} QEvt;

#ifdef Q_EVT_CTOR            /* Provide the constructor for the QEvt class? */

QEvt *QEvt_ctor(QEvt * const me, enum_t const sig);

#endif

/****************************************************************************/
/** helper macro to calculate static dimension of a 1-dim array \a array_ */
#define Q_DIM(array_) (sizeof(array_) / sizeof(array_[0]))

/** \brief Perform cast from unsigned integer \a uint_ to pointer
* of type \a type_.
*
* This macro encapsulates the cast to (type_ *), which QP ports or
* application might use to access embedded hardware registers.
* Such uses can trigger PC-Lint "Note 923: cast from int to pointer" and
* this macro helps to encapsulate this deviation.
*/
#define Q_UINT2PTR_CAST(type_, uint_)  ((type_ *)(uint_))

/****************************************************************************/
#ifndef Q_NQEVENT
    typedef QEvt QEvent;             /**< typedef for backwards compatibity */
#endif

#endif                                                            /* qevt_h */

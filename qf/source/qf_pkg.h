/**
* \file
* \brief Internal (package scope) QF/C interface.
* \ingroup qf
* \cond
******************************************************************************
* Product: QF/C
* Last updated for version 5.3.0
* Last updated on  2014-04-03
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
#ifndef qf_pkg_h
#define qf_pkg_h

/****************************************************************************/
/* QF-specific critical section */
#ifndef QF_CRIT_STAT_TYPE
    /*! This is an internal macro for defining the critical section
    * status type. */
    /**
    * \description
    * The purpose of this macro is to enable writing the same code for the
    * case when critical section status type is defined and when it is not.
    * If the macro #QF_CRIT_STAT_TYPE is defined, this internal macro
    * provides the definition of the critical section status variable.
    * Otherwise this macro is empty.
    * \sa #QF_CRIT_STAT_TYPE
    */
    #define QF_CRIT_STAT_

    /*! This is an internal macro for entering a critical section. */
    /**
    * \description
    * The purpose of this macro is to enable writing the same code for the
    * case when critical section status type is defined and when it is not.
    * If the macro #QF_CRIT_STAT_TYPE is defined, this internal macro
    * invokes #QF_CRIT_ENTRY passing the key variable as the parameter.
    * Otherwise #QF_CRIT_ENTRY is invoked with a dummy parameter.
    * \sa #QF_CRIT_ENTRY
    */
    #define QF_CRIT_ENTRY_()    QF_CRIT_ENTRY(dummy)

    /*! This is an internal macro for exiting a critical section. */
    /**
    * \description
    * The purpose of this macro is to enable writing the same code for the
    * case when critical section status type is defined and when it is not.
    * If the macro #QF_CRIT_STAT_TYPE is defined, this internal macro
    * invokes #QF_CRIT_EXIT passing the key variable as the parameter.
    * Otherwise #QF_CRIT_EXIT is invoked with a dummy parameter.
    * \sa #QF_CRIT_EXIT
    */
    #define QF_CRIT_EXIT_()     QF_CRIT_EXIT(dummy)

#else
    #define QF_CRIT_STAT_       QF_CRIT_STAT_TYPE critStat_;
    #define QF_CRIT_ENTRY_()    QF_CRIT_ENTRY(critStat_)
    #define QF_CRIT_EXIT_()     QF_CRIT_EXIT(critStat_)
#endif

/* package-scope objects ****************************************************/

/*! heads of linked lists of time events, one for every clock tick rate */
extern QTimeEvt QF_timeEvtHead_[QF_MAX_TICK_RATE];

extern QF_EPOOL_TYPE_ QF_pool_[QF_MAX_EPOOL]; /*!< allocate event pools */
extern uint_fast8_t QF_maxPool_;     /*!< # of initialized event pools */
extern QSubscrList *QF_subscrList_;  /*!< the subscriber list array */
extern enum_t QF_maxSignal_;         /*!< the maximum published signal */

/*! structure representing a free block in the Native QF Memory Pool */
typedef struct QFreeBlock {
    struct QFreeBlock * volatile next;
} QFreeBlock;

/* internal helper macros ***************************************************/

/*! increment the refCtr of an event \a e_ casting const away */
#define QF_EVT_REF_CTR_INC_(e_) (++((QEvt *)(e_))->refCtr_)

/*! decrement the refCtr of an event \a e_ casting const away */
#define QF_EVT_REF_CTR_DEC_(e_) (--((QEvt *)(e_))->refCtr_)

/*! access element at index \a i_ from the base pointer \a base_ */
#define QF_PTR_AT_(base_, i_)   (base_[(i_)])

/**
* \description
* This macro is specifically and exclusively used for checking the range
* of a block pointer returned to the pool. Such a check must rely on the
* pointer arithmetic not compliant with the MISRA-C 2004 rules 17.2(req) and
* 17.3(req). Defining a specific macro for this purpose allows to selectively
* disable the warnings for this particular case.
*/
#define QF_PTR_RANGE_(x_, min_, max_)  (((min_) <= (x_)) && ((x_) <= (max_)))

/****************************************************************************/
#ifdef Q_SPY  /* QS software tracing enabled? */

    #if (QF_EQUEUE_CTR_SIZE == 1)

        /*! Internal QS macro to output an unformatted event queue counter
        * data element. */
        /**
        * \note the counter size depends on the macro #QF_EQUEUE_CTR_SIZE.
        */
        #define QS_EQC_(ctr_)       QS_u8_((uint8_t)(ctr_))
    #elif (QF_EQUEUE_CTR_SIZE == 2)
        #define QS_EQC_(ctr_)       QS_u16_((uint16_t)(ctr_))
    #elif (QF_EQUEUE_CTR_SIZE == 4)
        #define QS_EQC_(ctr_)       QS_u32_((uint32_t)(ctr_))
    #else
        #error "QF_EQUEUE_CTR_SIZE not defined"
    #endif


    #if (QF_EVENT_SIZ_SIZE == 1)

        /*! Internal QS macro to output an unformatted event size
        * data element. */
        * \note the event size depends on the macro #QF_EVENT_SIZ_SIZE.
        */
        #define QS_EVS_(size_)      QS_u8_((uint8_t)(size_))
    #elif (QF_EVENT_SIZ_SIZE == 2)
        #define QS_EVS_(size_)      QS_u16_((uint16_t)(size_))
    #elif (QF_EVENT_SIZ_SIZE == 4)
        #define QS_EVS_(size_)      QS_u32_((uint32_t)(size_))
    #endif


    #if (QF_MPOOL_SIZ_SIZE == 1)

        /*! Internal QS macro to output an unformatted memory pool
        * block-size data element */
        /**
        * \note the block-size depends on the macro #QF_MPOOL_SIZ_SIZE.
        */
        #define QS_MPS_(size_)      QS_u8_((uint8_t)(size_))
    #elif (QF_MPOOL_SIZ_SIZE == 2)
        #define QS_MPS_(size_)      QS_u16_((uint16_t)(size_))
    #elif (QF_MPOOL_SIZ_SIZE == 4)
        #define QS_MPS_(size_)      QS_u32_((uint32_t)(size_))
    #endif

    #if (QF_MPOOL_CTR_SIZE == 1)

        /*! Internal QS macro to output an unformatted memory pool
        * block-counter data element. */
        /**
        * \note the counter size depends on the macro #QF_MPOOL_CTR_SIZE.
        */
        #define QS_MPC_(ctr_)       QS_u8_((uint8_t)(ctr_))
    #elif (QF_MPOOL_CTR_SIZE == 2)
        #define QS_MPC_(ctr_)       QS_u16_((uint16_t)(ctr_))
    #elif (QF_MPOOL_CTR_SIZE == 4)
        #define QS_MPC_(ctr_)       QS_u32_((uint16_t)(ctr_))
    #endif


    #if (QF_TIMEEVT_CTR_SIZE == 1)

        /*! Internal QS macro to output an unformatted time event
        * tick-counter data element */
        /**
        * \note the counter size depends on the macro #QF_TIMEEVT_CTR_SIZE.
        */
        #define QS_TEC_(ctr_)       QS_u8_((uint8_t)(ctr_))
    #elif (QF_TIMEEVT_CTR_SIZE == 2)
        #define QS_TEC_(ctr_)       QS_u16_((uint16_t)(ctr_))
    #elif (QF_TIMEEVT_CTR_SIZE == 4)
        #define QS_TEC_(ctr_)       QS_u32_((uint32_t)(ctr_))
    #endif

#endif /* Q_SPY */

#endif /* qf_pkg_h */

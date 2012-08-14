/*****************************************************************************
* Product: Capstone Scuba Diving computer example
* Last Updated for Version: 4.5.00
* Date of the Last Update:  May 18, 2012
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
#ifndef capstone_h
#define capstone_h

enum CapstoneSignals {
   B1_DOWN_SIG = Q_USER_SIG,                       /**< button B1 depressed */
   B1_UP_SIG,                                      /**< button B1 released  */
   B2_DOWN_SIG,                                    /**< button B2 depressed */
   B2_UP_SIG,                                      /**< button B2 released  */
   MAX_PUB_SIG,                              /**< the last published signal */

   ASCENT_RATE_ADC_SIG,                    /**< raw ascent rate ADC reading */
   HEARTBEAT_SIG,      /**< heartbeat of the Capstone scuba diving computer */
   DT_TTS_SIG,   /**< signal to alternate dive time/time to surface display */
   ADD_GAS_SIG,                          /**< signal to add gas to the tank */

   ALARM_REQUEST_SIG,                        /**< alarm request to AlarmMgr */
   ALARM_SILENCE_SIG,                        /**< alarm silence to AlarmMgr */
   TIMEOUT_SIG,                 /**< timeout for playing a note in AlarmMgr */

   MAX_SIG                                             /**< the last signal */
};

typedef struct ADCEvtTag {
    QEvt super;                                    /* derives from QEvt */
    uint16_t raw;                                        /* raw ADC reading */
} ADCEvt;

typedef struct AscentRateEvtTag {
    QEvt super;                                    /* derives from QEvt */
    int16_t ascent_m_per_min;                       /* ascent rate in m/min */
} AscentRateEvt;

typedef struct AlarmEvtTag {
    QEvt super;                                    /* derives from QEvt */
    uint8_t alarm_type;                                       /* alarm type */
} AlarmEvt;

enum AlarmTypes {          /* arranged in ascending order of alarm priority */
    ALL_ALARMS,
    DEPTH_ALARM,
    ASCENT_RATE_ALARM,
    OUT_OF_AIR_ALARM,
    MAX_ALARM                                           /* keep always last */
};

/*..........................................................................*/
void Capstone_ctor(void);
void AlarmMgr_ctor(void);

extern QActive * const AO_Capstone;      /* "opaque" pointer to Capstone AO */
extern QActive * const AO_AlarmMgr;      /* "opaque" pointer to AlarmMgr AO */

#endif                                                        /* capstone_h */

//*****************************************************************************
//
// rom.h - Macros to facilitate calling functions in the ROM.
//
// Copyright (c) 2007-2013 Texas Instruments Incorporated.  All rights reserved.
// Software License Agreement
//
//   Redistribution and use in source and binary forms, with or without
//   modification, are permitted provided that the following conditions
//   are met:
//
//   Redistributions of source code must retain the above copyright
//   notice, this list of conditions and the following disclaimer.
//
//   Redistributions in binary form must reproduce the above copyright
//   notice, this list of conditions and the following disclaimer in the
//   documentation and/or other materials provided with the
//   distribution.
//
//   Neither the name of Texas Instruments Incorporated nor the names of
//   its contributors may be used to endorse or promote products derived
//   from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// This is part of revision 1.0 of the Tiva Peripheral Driver Library.
//
//*****************************************************************************

#ifndef __ROM_H__
#define __ROM_H__

//*****************************************************************************
//
// Pointers to the main API tables.
//
//*****************************************************************************
#define ROM_APITABLE            ((uint32_t *)0x01000010)
#define ROM_VERSION             (ROM_APITABLE[0])
#define ROM_UARTTABLE           ((uint32_t *)(ROM_APITABLE[1]))
#define ROM_SSITABLE            ((uint32_t *)(ROM_APITABLE[2]))
#define ROM_I2CTABLE            ((uint32_t *)(ROM_APITABLE[3]))
#define ROM_GPIOTABLE           ((uint32_t *)(ROM_APITABLE[4]))
#define ROM_ADCTABLE            ((uint32_t *)(ROM_APITABLE[5]))
#define ROM_COMPARATORTABLE     ((uint32_t *)(ROM_APITABLE[6]))
#define ROM_FLASHTABLE          ((uint32_t *)(ROM_APITABLE[7]))
#define ROM_PWMTABLE            ((uint32_t *)(ROM_APITABLE[8]))
#define ROM_QEITABLE            ((uint32_t *)(ROM_APITABLE[9]))
#define ROM_SYSTICKTABLE        ((uint32_t *)(ROM_APITABLE[10]))
#define ROM_TIMERTABLE          ((uint32_t *)(ROM_APITABLE[11]))
#define ROM_WATCHDOGTABLE       ((uint32_t *)(ROM_APITABLE[12]))
#define ROM_SYSCTLTABLE         ((uint32_t *)(ROM_APITABLE[13]))
#define ROM_INTERRUPTTABLE      ((uint32_t *)(ROM_APITABLE[14]))
#define ROM_USBTABLE            ((uint32_t *)(ROM_APITABLE[16]))
#define ROM_UDMATABLE           ((uint32_t *)(ROM_APITABLE[17]))
#define ROM_CANTABLE            ((uint32_t *)(ROM_APITABLE[18]))
#define ROM_HIBERNATETABLE      ((uint32_t *)(ROM_APITABLE[19]))
#define ROM_MPUTABLE            ((uint32_t *)(ROM_APITABLE[20]))
#define ROM_SOFTWARETABLE       ((uint32_t *)(ROM_APITABLE[21]))
#define ROM_EEPROMTABLE         ((uint32_t *)(ROM_APITABLE[24]))
#define ROM_FANTABLE            ((uint32_t *)(ROM_APITABLE[25]))
#define ROM_FPUTABLE            ((uint32_t *)(ROM_APITABLE[26]))
#define ROM_LPCTABLE            ((uint32_t *)(ROM_APITABLE[27]))
#define ROM_PECITABLE           ((uint32_t *)(ROM_APITABLE[28]))
#define ROM_SMBUSTABLE          ((uint32_t *)(ROM_APITABLE[29]))
#define ROM_SYSEXCTABLE         ((uint32_t *)(ROM_APITABLE[30]))

//*****************************************************************************
//
// Macros for calling ROM functions in the ADC API.
//
//*****************************************************************************
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_ADCSequenceDataGet                                                \
        ((int32_t (*)(uint32_t ui32Base,                                      \
                      uint32_t ui32SequenceNum,                               \
                      uint32_t *pui32Buffer))ROM_ADCTABLE[0])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_ADCIntDisable                                                     \
        ((void (*)(uint32_t ui32Base,                                         \
                   uint32_t ui32SequenceNum))ROM_ADCTABLE[1])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_ADCIntEnable                                                      \
        ((void (*)(uint32_t ui32Base,                                         \
                   uint32_t ui32SequenceNum))ROM_ADCTABLE[2])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_ADCIntStatus                                                      \
        ((uint32_t (*)(uint32_t ui32Base,                                     \
                       uint32_t ui32SequenceNum,                              \
                       bool bMasked))ROM_ADCTABLE[3])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_ADCIntClear                                                       \
        ((void (*)(uint32_t ui32Base,                                         \
                   uint32_t ui32SequenceNum))ROM_ADCTABLE[4])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_ADCSequenceEnable                                                 \
        ((void (*)(uint32_t ui32Base,                                         \
                   uint32_t ui32SequenceNum))ROM_ADCTABLE[5])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_ADCSequenceDisable                                                \
        ((void (*)(uint32_t ui32Base,                                         \
                   uint32_t ui32SequenceNum))ROM_ADCTABLE[6])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_ADCSequenceConfigure                                              \
        ((void (*)(uint32_t ui32Base,                                         \
                   uint32_t ui32SequenceNum,                                  \
                   uint32_t ui32Trigger,                                      \
                   uint32_t ui32Priority))ROM_ADCTABLE[7])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_ADCSequenceStepConfigure                                          \
        ((void (*)(uint32_t ui32Base,                                         \
                   uint32_t ui32SequenceNum,                                  \
                   uint32_t ui32Step,                                         \
                   uint32_t ui32Config))ROM_ADCTABLE[8])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_ADCSequenceOverflow                                               \
        ((int32_t (*)(uint32_t ui32Base,                                      \
                      uint32_t ui32SequenceNum))ROM_ADCTABLE[9])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_ADCSequenceOverflowClear                                          \
        ((void (*)(uint32_t ui32Base,                                         \
                   uint32_t ui32SequenceNum))ROM_ADCTABLE[10])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_ADCSequenceUnderflow                                              \
        ((int32_t (*)(uint32_t ui32Base,                                      \
                      uint32_t ui32SequenceNum))ROM_ADCTABLE[11])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_ADCSequenceUnderflowClear                                         \
        ((void (*)(uint32_t ui32Base,                                         \
                   uint32_t ui32SequenceNum))ROM_ADCTABLE[12])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_ADCProcessorTrigger                                               \
        ((void (*)(uint32_t ui32Base,                                         \
                   uint32_t ui32SequenceNum))ROM_ADCTABLE[13])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_ADCHardwareOversampleConfigure                                    \
        ((void (*)(uint32_t ui32Base,                                         \
                   uint32_t ui32Factor))ROM_ADCTABLE[14])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_ADCComparatorConfigure                                            \
        ((void (*)(uint32_t ui32Base,                                         \
                   uint32_t ui32Comp,                                         \
                   uint32_t ui32Config))ROM_ADCTABLE[15])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_ADCComparatorRegionSet                                            \
        ((void (*)(uint32_t ui32Base,                                         \
                   uint32_t ui32Comp,                                         \
                   uint32_t ui32LowRef,                                       \
                   uint32_t ui32HighRef))ROM_ADCTABLE[16])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_ADCComparatorReset                                                \
        ((void (*)(uint32_t ui32Base,                                         \
                   uint32_t ui32Comp,                                         \
                   bool bTrigger,                                             \
                   bool bInterrupt))ROM_ADCTABLE[17])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_ADCComparatorIntDisable                                           \
        ((void (*)(uint32_t ui32Base,                                         \
                   uint32_t ui32SequenceNum))ROM_ADCTABLE[18])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_ADCComparatorIntEnable                                            \
        ((void (*)(uint32_t ui32Base,                                         \
                   uint32_t ui32SequenceNum))ROM_ADCTABLE[19])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_ADCComparatorIntStatus                                            \
        ((uint32_t (*)(uint32_t ui32Base))ROM_ADCTABLE[20])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_ADCComparatorIntClear                                             \
        ((void (*)(uint32_t ui32Base,                                         \
                   uint32_t ui32Status))ROM_ADCTABLE[21])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_ADCReferenceSet                                                   \
        ((void (*)(uint32_t ui32Base,                                         \
                   uint32_t ui32Ref))ROM_ADCTABLE[22])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_ADCReferenceGet                                                   \
        ((uint32_t (*)(uint32_t ui32Base))ROM_ADCTABLE[23])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_ADCPhaseDelaySet                                                  \
        ((void (*)(uint32_t ui32Base,                                         \
                   uint32_t ui32Phase))ROM_ADCTABLE[24])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_ADCPhaseDelayGet                                                  \
        ((uint32_t (*)(uint32_t ui32Base))ROM_ADCTABLE[25])
#endif

//*****************************************************************************
//
// Macros for calling ROM functions in the CAN API.
//
//*****************************************************************************
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_CANIntClear                                                       \
        ((void (*)(uint32_t ui32Base,                                         \
                   uint32_t ui32IntClr))ROM_CANTABLE[0])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_CANInit                                                           \
        ((void (*)(uint32_t ui32Base))ROM_CANTABLE[1])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_CANEnable                                                         \
        ((void (*)(uint32_t ui32Base))ROM_CANTABLE[2])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_CANDisable                                                        \
        ((void (*)(uint32_t ui32Base))ROM_CANTABLE[3])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_CANBitTimingSet                                                   \
        ((void (*)(uint32_t ui32Base,                                         \
                   tCANBitClkParms *psClkParms))ROM_CANTABLE[4])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_CANBitTimingGet                                                   \
        ((void (*)(uint32_t ui32Base,                                         \
                   tCANBitClkParms *psClkParms))ROM_CANTABLE[5])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_CANMessageSet                                                     \
        ((void (*)(uint32_t ui32Base,                                         \
                   uint32_t ui32ObjID,                                        \
                   tCANMsgObject *psMsgObject,                                \
                   tMsgObjType eMsgType))ROM_CANTABLE[6])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_CANMessageGet                                                     \
        ((void (*)(uint32_t ui32Base,                                         \
                   uint32_t ui32ObjID,                                        \
                   tCANMsgObject *psMsgObject,                                \
                   bool bClrPendingInt))ROM_CANTABLE[7])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_CANStatusGet                                                      \
        ((uint32_t (*)(uint32_t ui32Base,                                     \
                       tCANStsReg eStatusReg))ROM_CANTABLE[8])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_CANMessageClear                                                   \
        ((void (*)(uint32_t ui32Base,                                         \
                   uint32_t ui32ObjID))ROM_CANTABLE[9])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_CANIntEnable                                                      \
        ((void (*)(uint32_t ui32Base,                                         \
                   uint32_t ui32IntFlags))ROM_CANTABLE[10])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_CANIntDisable                                                     \
        ((void (*)(uint32_t ui32Base,                                         \
                   uint32_t ui32IntFlags))ROM_CANTABLE[11])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_CANIntStatus                                                      \
        ((uint32_t (*)(uint32_t ui32Base,                                     \
                       tCANIntStsReg eIntStsReg))ROM_CANTABLE[12])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_CANRetryGet                                                       \
        ((bool (*)(uint32_t ui32Base))ROM_CANTABLE[13])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_CANRetrySet                                                       \
        ((void (*)(uint32_t ui32Base,                                         \
                   bool bAutoRetry))ROM_CANTABLE[14])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_CANErrCntrGet                                                     \
        ((bool (*)(uint32_t ui32Base,                                         \
                   uint32_t *pui32RxCount,                                    \
                   uint32_t *pui32TxCount))ROM_CANTABLE[15])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_CANBitRateSet                                                     \
        ((uint32_t (*)(uint32_t ui32Base,                                     \
                       uint32_t ui32SourceClock,                              \
                       uint32_t ui32BitRate))ROM_CANTABLE[16])
#endif

//*****************************************************************************
//
// Macros for calling ROM functions in the Comparator API.
//
//*****************************************************************************
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_ComparatorIntClear                                                \
        ((void (*)(uint32_t ui32Base,                                         \
                   uint32_t ui32Comp))ROM_COMPARATORTABLE[0])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_ComparatorConfigure                                               \
        ((void (*)(uint32_t ui32Base,                                         \
                   uint32_t ui32Comp,                                         \
                   uint32_t ui32Config))ROM_COMPARATORTABLE[1])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_ComparatorRefSet                                                  \
        ((void (*)(uint32_t ui32Base,                                         \
                   uint32_t ui32Ref))ROM_COMPARATORTABLE[2])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_ComparatorValueGet                                                \
        ((bool (*)(uint32_t ui32Base,                                         \
                   uint32_t ui32Comp))ROM_COMPARATORTABLE[3])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_ComparatorIntEnable                                               \
        ((void (*)(uint32_t ui32Base,                                         \
                   uint32_t ui32Comp))ROM_COMPARATORTABLE[4])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_ComparatorIntDisable                                              \
        ((void (*)(uint32_t ui32Base,                                         \
                   uint32_t ui32Comp))ROM_COMPARATORTABLE[5])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_ComparatorIntStatus                                               \
        ((bool (*)(uint32_t ui32Base,                                         \
                   uint32_t ui32Comp,                                         \
                   bool bMasked))ROM_COMPARATORTABLE[6])
#endif

//*****************************************************************************
//
// Macros for calling ROM functions in the EEPROM API.
//
//*****************************************************************************
#if defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_EEPROMRead                                                        \
        ((void (*)(uint32_t *pui32Data,                                       \
                   uint32_t ui32Address,                                      \
                   uint32_t ui32Count))ROM_EEPROMTABLE[0])
#endif
#if defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_EEPROMBlockCountGet                                               \
        ((uint32_t (*)(void))ROM_EEPROMTABLE[1])
#endif
#if defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_EEPROMBlockHide                                                   \
        ((void (*)(uint32_t ui32Block))ROM_EEPROMTABLE[2])
#endif
#if defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_EEPROMBlockLock                                                   \
        ((uint32_t (*)(uint32_t ui32Block))ROM_EEPROMTABLE[3])
#endif
#if defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_EEPROMBlockPasswordSet                                            \
        ((uint32_t (*)(uint32_t ui32Block,                                    \
                       uint32_t *pui32Password,                               \
                       uint32_t ui32Count))ROM_EEPROMTABLE[4])
#endif
#if defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_EEPROMBlockProtectGet                                             \
        ((uint32_t (*)(uint32_t ui32Block))ROM_EEPROMTABLE[5])
#endif
#if defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_EEPROMBlockProtectSet                                             \
        ((uint32_t (*)(uint32_t ui32Block,                                    \
                       uint32_t ui32Protect))ROM_EEPROMTABLE[6])
#endif
#if defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_EEPROMBlockUnlock                                                 \
        ((uint32_t (*)(uint32_t ui32Block,                                    \
                       uint32_t *pui32Password,                               \
                       uint32_t ui32Count))ROM_EEPROMTABLE[7])
#endif
#if defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_EEPROMIntClear                                                    \
        ((void (*)(uint32_t ui32IntFlags))ROM_EEPROMTABLE[8])
#endif
#if defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_EEPROMIntDisable                                                  \
        ((void (*)(uint32_t ui32IntFlags))ROM_EEPROMTABLE[9])
#endif
#if defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_EEPROMIntEnable                                                   \
        ((void (*)(uint32_t ui32IntFlags))ROM_EEPROMTABLE[10])
#endif
#if defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_EEPROMIntStatus                                                   \
        ((uint32_t (*)(bool bMasked))ROM_EEPROMTABLE[11])
#endif
#if defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_EEPROMMassErase                                                   \
        ((uint32_t (*)(void))ROM_EEPROMTABLE[12])
#endif
#if defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_EEPROMProgram                                                     \
        ((uint32_t (*)(uint32_t *pui32Data,                                   \
                       uint32_t ui32Address,                                  \
                       uint32_t ui32Count))ROM_EEPROMTABLE[13])
#endif
#if defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_EEPROMProgramNonBlocking                                          \
        ((uint32_t (*)(uint32_t ui32Data,                                     \
                       uint32_t ui32Address))ROM_EEPROMTABLE[14])
#endif
#if defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_EEPROMSizeGet                                                     \
        ((uint32_t (*)(void))ROM_EEPROMTABLE[15])
#endif
#if defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_EEPROMStatusGet                                                   \
        ((uint32_t (*)(void))ROM_EEPROMTABLE[16])
#endif
#if defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_EEPROMInit                                                        \
        ((uint32_t (*)(void))ROM_EEPROMTABLE[17])
#endif

//*****************************************************************************
//
// Macros for calling ROM functions in the Fan API.
//
//*****************************************************************************
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_FanIntClear                                                       \
        ((void (*)(uint32_t ui32Base,                                         \
                   uint32_t ui32Flags))ROM_FANTABLE[0])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_FanChannelConfigAuto                                              \
        ((void (*)(uint32_t ui32Base,                                         \
                   uint32_t ui32Channel,                                      \
                   uint32_t ui32Config))ROM_FANTABLE[1])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_FanChannelConfigManual                                            \
        ((void (*)(uint32_t ui32Base,                                         \
                   uint32_t ui32Channel,                                      \
                   uint32_t ui32Config))ROM_FANTABLE[2])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_FanChannelDisable                                                 \
        ((void (*)(uint32_t ui32Base,                                         \
                   uint32_t ui32Channel))ROM_FANTABLE[3])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_FanChannelDutyGet                                                 \
        ((uint32_t (*)(uint32_t ui32Base,                                     \
                       uint32_t ui32Channel))ROM_FANTABLE[4])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_FanChannelDutySet                                                 \
        ((void (*)(uint32_t ui32Base,                                         \
                   uint32_t ui32Channel,                                      \
                   uint32_t ui32Duty))ROM_FANTABLE[5])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_FanChannelEnable                                                  \
        ((void (*)(uint32_t ui32Base,                                         \
                   uint32_t ui32Channel))ROM_FANTABLE[6])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_FanChannelRPMGet                                                  \
        ((uint32_t (*)(uint32_t ui32Base,                                     \
                       uint32_t ui32Channel))ROM_FANTABLE[7])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_FanChannelRPMSet                                                  \
        ((void (*)(uint32_t ui32Base,                                         \
                   uint32_t ui32Channel,                                      \
                   uint32_t ui32RPM))ROM_FANTABLE[8])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_FanChannelStatus                                                  \
        ((uint32_t (*)(uint32_t ui32Base,                                     \
                       uint32_t ui32Channel))ROM_FANTABLE[9])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_FanChannelsGet                                                    \
        ((uint32_t (*)(uint32_t ui32Base))ROM_FANTABLE[10])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_FanIntDisable                                                     \
        ((void (*)(uint32_t ui32Base,                                         \
                   uint32_t ui32Flags))ROM_FANTABLE[11])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_FanIntEnable                                                      \
        ((void (*)(uint32_t ui32Base,                                         \
                   uint32_t ui32Flags))ROM_FANTABLE[12])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_FanIntStatus                                                      \
        ((uint32_t (*)(uint32_t ui32Base,                                     \
                       bool bMasked))ROM_FANTABLE[13])
#endif

//*****************************************************************************
//
// Macros for calling ROM functions in the Flash API.
//
//*****************************************************************************
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_FlashProgram                                                      \
        ((int32_t (*)(uint32_t *pui32Data,                                    \
                      uint32_t ui32Address,                                   \
                      uint32_t ui32Count))ROM_FLASHTABLE[0])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_FlashErase                                                        \
        ((int32_t (*)(uint32_t ui32Address))ROM_FLASHTABLE[3])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_FlashProtectGet                                                   \
        ((tFlashProtection (*)(uint32_t ui32Address))ROM_FLASHTABLE[4])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_FlashProtectSet                                                   \
        ((int32_t (*)(uint32_t ui32Address,                                   \
                      tFlashProtection eProtect))ROM_FLASHTABLE[5])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_FlashProtectSave                                                  \
        ((int32_t (*)(void))ROM_FLASHTABLE[6])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_FlashUserGet                                                      \
        ((int32_t (*)(uint32_t *pui32User0,                                   \
                      uint32_t *pui32User1))ROM_FLASHTABLE[7])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_FlashUserSet                                                      \
        ((int32_t (*)(uint32_t ui32User0,                                     \
                      uint32_t ui32User1))ROM_FLASHTABLE[8])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_FlashUserSave                                                     \
        ((int32_t (*)(void))ROM_FLASHTABLE[9])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_FlashIntEnable                                                    \
        ((void (*)(uint32_t ui32IntFlags))ROM_FLASHTABLE[10])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_FlashIntDisable                                                   \
        ((void (*)(uint32_t ui32IntFlags))ROM_FLASHTABLE[11])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_FlashIntStatus                                                    \
        ((uint32_t (*)(bool bMasked))ROM_FLASHTABLE[12])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_FlashIntClear                                                     \
        ((void (*)(uint32_t ui32IntFlags))ROM_FLASHTABLE[13])
#endif

//*****************************************************************************
//
// Macros for calling ROM functions in the FPU API.
//
//*****************************************************************************
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_FPUEnable                                                         \
        ((void (*)(void))ROM_FPUTABLE[0])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_FPUDisable                                                        \
        ((void (*)(void))ROM_FPUTABLE[1])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_FPUFlushToZeroModeSet                                             \
        ((void (*)(uint32_t ui32Mode))ROM_FPUTABLE[2])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_FPUHalfPrecisionModeSet                                           \
        ((void (*)(uint32_t ui32Mode))ROM_FPUTABLE[3])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_FPULazyStackingEnable                                             \
        ((void (*)(void))ROM_FPUTABLE[4])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_FPUNaNModeSet                                                     \
        ((void (*)(uint32_t ui32Mode))ROM_FPUTABLE[5])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_FPURoundingModeSet                                                \
        ((void (*)(uint32_t ui32Mode))ROM_FPUTABLE[6])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_FPUStackingDisable                                                \
        ((void (*)(void))ROM_FPUTABLE[7])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_FPUStackingEnable                                                 \
        ((void (*)(void))ROM_FPUTABLE[8])
#endif

//*****************************************************************************
//
// Macros for calling ROM functions in the GPIO API.
//
//*****************************************************************************
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_GPIOPinWrite                                                      \
        ((void (*)(uint32_t ui32Port,                                         \
                   uint8_t ui8Pins,                                           \
                   uint8_t ui8Val))ROM_GPIOTABLE[0])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_GPIODirModeSet                                                    \
        ((void (*)(uint32_t ui32Port,                                         \
                   uint8_t ui8Pins,                                           \
                   uint32_t ui32PinIO))ROM_GPIOTABLE[1])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_GPIODirModeGet                                                    \
        ((uint32_t (*)(uint32_t ui32Port,                                     \
                       uint8_t ui8Pin))ROM_GPIOTABLE[2])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_GPIOIntTypeSet                                                    \
        ((void (*)(uint32_t ui32Port,                                         \
                   uint8_t ui8Pins,                                           \
                   uint32_t ui32IntType))ROM_GPIOTABLE[3])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_GPIOIntTypeGet                                                    \
        ((uint32_t (*)(uint32_t ui32Port,                                     \
                       uint8_t ui8Pin))ROM_GPIOTABLE[4])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_GPIOPadConfigSet                                                  \
        ((void (*)(uint32_t ui32Port,                                         \
                   uint8_t ui8Pins,                                           \
                   uint32_t ui32Strength,                                     \
                   uint32_t ui32PadType))ROM_GPIOTABLE[5])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_GPIOPadConfigGet                                                  \
        ((void (*)(uint32_t ui32Port,                                         \
                   uint8_t ui8Pin,                                            \
                   uint32_t *pui32Strength,                                   \
                   uint32_t *pui32PadType))ROM_GPIOTABLE[6])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_GPIOPinRead                                                       \
        ((int32_t (*)(uint32_t ui32Port,                                      \
                      uint8_t ui8Pins))ROM_GPIOTABLE[11])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_GPIOPinTypeCAN                                                    \
        ((void (*)(uint32_t ui32Port,                                         \
                   uint8_t ui8Pins))ROM_GPIOTABLE[12])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_GPIOPinTypeComparator                                             \
        ((void (*)(uint32_t ui32Port,                                         \
                   uint8_t ui8Pins))ROM_GPIOTABLE[13])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_GPIOPinTypeGPIOInput                                              \
        ((void (*)(uint32_t ui32Port,                                         \
                   uint8_t ui8Pins))ROM_GPIOTABLE[14])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_GPIOPinTypeGPIOOutput                                             \
        ((void (*)(uint32_t ui32Port,                                         \
                   uint8_t ui8Pins))ROM_GPIOTABLE[15])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_GPIOPinTypeI2C                                                    \
        ((void (*)(uint32_t ui32Port,                                         \
                   uint8_t ui8Pins))ROM_GPIOTABLE[16])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_GPIOPinTypePWM                                                    \
        ((void (*)(uint32_t ui32Port,                                         \
                   uint8_t ui8Pins))ROM_GPIOTABLE[17])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_GPIOPinTypeQEI                                                    \
        ((void (*)(uint32_t ui32Port,                                         \
                   uint8_t ui8Pins))ROM_GPIOTABLE[18])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_GPIOPinTypeSSI                                                    \
        ((void (*)(uint32_t ui32Port,                                         \
                   uint8_t ui8Pins))ROM_GPIOTABLE[19])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_GPIOPinTypeTimer                                                  \
        ((void (*)(uint32_t ui32Port,                                         \
                   uint8_t ui8Pins))ROM_GPIOTABLE[20])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_GPIOPinTypeUART                                                   \
        ((void (*)(uint32_t ui32Port,                                         \
                   uint8_t ui8Pins))ROM_GPIOTABLE[21])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_GPIOPinTypeGPIOOutputOD                                           \
        ((void (*)(uint32_t ui32Port,                                         \
                   uint8_t ui8Pins))ROM_GPIOTABLE[22])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_GPIOPinTypeADC                                                    \
        ((void (*)(uint32_t ui32Port,                                         \
                   uint8_t ui8Pins))ROM_GPIOTABLE[23])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_GPIOPinTypeUSBDigital                                             \
        ((void (*)(uint32_t ui32Port,                                         \
                   uint8_t ui8Pins))ROM_GPIOTABLE[24])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_GPIOPinConfigure                                                  \
        ((void (*)(uint32_t ui32PinConfig))ROM_GPIOTABLE[26])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_GPIOPinTypeUSBAnalog                                              \
        ((void (*)(uint32_t ui32Port,                                         \
                   uint8_t ui8Pins))ROM_GPIOTABLE[28])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_GPIODMATriggerEnable                                              \
        ((void (*)(uint32_t ui32Port,                                         \
                   uint8_t ui8Pins))ROM_GPIOTABLE[31])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_GPIODMATriggerDisable                                             \
        ((void (*)(uint32_t ui32Port,                                         \
                   uint8_t ui8Pins))ROM_GPIOTABLE[32])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_GPIOADCTriggerEnable                                              \
        ((void (*)(uint32_t ui32Port,                                         \
                   uint8_t ui8Pins))ROM_GPIOTABLE[33])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_GPIOADCTriggerDisable                                             \
        ((void (*)(uint32_t ui32Port,                                         \
                   uint8_t ui8Pins))ROM_GPIOTABLE[34])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_GPIOPinTypeFan                                                    \
        ((void (*)(uint32_t ui32Port,                                         \
                   uint8_t ui8Pins))ROM_GPIOTABLE[35])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_GPIOPinTypeLPC                                                    \
        ((void (*)(uint32_t ui32Port,                                         \
                   uint8_t ui8Pins))ROM_GPIOTABLE[36])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_GPIOPinTypePECIRx                                                 \
        ((void (*)(uint32_t ui32Port,                                         \
                   uint8_t ui8Pins))ROM_GPIOTABLE[37])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_GPIOPinTypePECITx                                                 \
        ((void (*)(uint32_t ui32Port,                                         \
                   uint8_t ui8Pins))ROM_GPIOTABLE[38])
#endif
#if defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_GPIOPinTypeI2CSCL                                                 \
        ((void (*)(uint32_t ui32Port,                                         \
                   uint8_t ui8Pins))ROM_GPIOTABLE[39])
#endif

//*****************************************************************************
//
// Macros for calling ROM functions in the Hibernate API.
//
//*****************************************************************************
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_HibernateIntClear                                                 \
        ((void (*)(uint32_t ui32IntFlags))ROM_HIBERNATETABLE[0])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_HibernateEnableExpClk                                             \
        ((void (*)(uint32_t ui32HibClk))ROM_HIBERNATETABLE[1])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_HibernateDisable                                                  \
        ((void (*)(void))ROM_HIBERNATETABLE[2])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_HibernateRTCEnable                                                \
        ((void (*)(void))ROM_HIBERNATETABLE[4])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_HibernateRTCDisable                                               \
        ((void (*)(void))ROM_HIBERNATETABLE[5])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_HibernateWakeSet                                                  \
        ((void (*)(uint32_t ui32WakeFlags))ROM_HIBERNATETABLE[6])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_HibernateWakeGet                                                  \
        ((uint32_t (*)(void))ROM_HIBERNATETABLE[7])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_HibernateLowBatSet                                                \
        ((void (*)(uint32_t ui32LowBatFlags))ROM_HIBERNATETABLE[8])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_HibernateLowBatGet                                                \
        ((uint32_t (*)(void))ROM_HIBERNATETABLE[9])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_HibernateRTCSet                                                   \
        ((void (*)(uint32_t ui32RTCValue))ROM_HIBERNATETABLE[10])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_HibernateRTCGet                                                   \
        ((uint32_t (*)(void))ROM_HIBERNATETABLE[11])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_HibernateRTCTrimSet                                               \
        ((void (*)(uint32_t ui32Trim))ROM_HIBERNATETABLE[16])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_HibernateRTCTrimGet                                               \
        ((uint32_t (*)(void))ROM_HIBERNATETABLE[17])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_HibernateDataSet                                                  \
        ((void (*)(uint32_t *pui32Data,                                       \
                   uint32_t ui32Count))ROM_HIBERNATETABLE[18])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_HibernateDataGet                                                  \
        ((void (*)(uint32_t *pui32Data,                                       \
                   uint32_t ui32Count))ROM_HIBERNATETABLE[19])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_HibernateRequest                                                  \
        ((void (*)(void))ROM_HIBERNATETABLE[20])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_HibernateIntEnable                                                \
        ((void (*)(uint32_t ui32IntFlags))ROM_HIBERNATETABLE[21])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_HibernateIntDisable                                               \
        ((void (*)(uint32_t ui32IntFlags))ROM_HIBERNATETABLE[22])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_HibernateIntStatus                                                \
        ((uint32_t (*)(bool bMasked))ROM_HIBERNATETABLE[23])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_HibernateIsActive                                                 \
        ((uint32_t (*)(void))ROM_HIBERNATETABLE[24])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_HibernateRTCSSGet                                                 \
        ((uint32_t (*)(void))ROM_HIBERNATETABLE[27])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_HibernateClockConfig                                              \
        ((void (*)(uint32_t ui32Config))ROM_HIBERNATETABLE[28])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_HibernateBatCheckStart                                            \
        ((void (*)(void))ROM_HIBERNATETABLE[29])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_HibernateBatCheckDone                                             \
        ((uint32_t (*)(void))ROM_HIBERNATETABLE[30])
#endif

//*****************************************************************************
//
// Macros for calling ROM functions in the I2C API.
//
//*****************************************************************************
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_I2CMasterDataPut                                                  \
        ((void (*)(uint32_t ui32Base,                                         \
                   uint8_t ui8Data))ROM_I2CTABLE[0])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_I2CMasterInitExpClk                                               \
        ((void (*)(uint32_t ui32Base,                                         \
                   uint32_t ui32I2CClk,                                       \
                   bool bFast))ROM_I2CTABLE[1])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_I2CMasterEnable                                                   \
        ((void (*)(uint32_t ui32Base))ROM_I2CTABLE[3])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_I2CMasterDisable                                                  \
        ((void (*)(uint32_t ui32Base))ROM_I2CTABLE[5])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_I2CMasterIntEnable                                                \
        ((void (*)(uint32_t ui32Base))ROM_I2CTABLE[7])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_I2CMasterIntDisable                                               \
        ((void (*)(uint32_t ui32Base))ROM_I2CTABLE[9])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_I2CMasterIntStatus                                                \
        ((bool (*)(uint32_t ui32Base,                                         \
                   bool bMasked))ROM_I2CTABLE[11])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_I2CMasterIntClear                                                 \
        ((void (*)(uint32_t ui32Base))ROM_I2CTABLE[13])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_I2CMasterSlaveAddrSet                                             \
        ((void (*)(uint32_t ui32Base,                                         \
                   uint8_t ui8SlaveAddr,                                      \
                   bool bReceive))ROM_I2CTABLE[15])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_I2CMasterBusy                                                     \
        ((bool (*)(uint32_t ui32Base))ROM_I2CTABLE[16])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_I2CMasterBusBusy                                                  \
        ((bool (*)(uint32_t ui32Base))ROM_I2CTABLE[17])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_I2CMasterControl                                                  \
        ((void (*)(uint32_t ui32Base,                                         \
                   uint32_t ui32Cmd))ROM_I2CTABLE[18])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_I2CMasterErr                                                      \
        ((uint32_t (*)(uint32_t ui32Base))ROM_I2CTABLE[19])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_I2CMasterDataGet                                                  \
        ((uint32_t (*)(uint32_t ui32Base))ROM_I2CTABLE[20])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_UpdateI2C                                                         \
        ((void (*)(void))ROM_I2CTABLE[24])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_I2CMasterIntEnableEx                                              \
        ((void (*)(uint32_t ui32Base,                                         \
                   uint32_t ui32IntFlags))ROM_I2CTABLE[29])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_I2CMasterIntDisableEx                                             \
        ((void (*)(uint32_t ui32Base,                                         \
                   uint32_t ui32IntFlags))ROM_I2CTABLE[30])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_I2CMasterIntStatusEx                                              \
        ((uint32_t (*)(uint32_t ui32Base,                                     \
                       bool bMasked))ROM_I2CTABLE[31])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_I2CMasterIntClearEx                                               \
        ((void (*)(uint32_t ui32Base,                                         \
                   uint32_t ui32IntFlags))ROM_I2CTABLE[32])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_I2CMasterTimeoutSet                                               \
        ((void (*)(uint32_t ui32Base,                                         \
                   uint32_t ui32Value))ROM_I2CTABLE[33])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_I2CMasterLineStateGet                                             \
        ((uint32_t (*)(uint32_t ui32Base))ROM_I2CTABLE[38])
#endif

//*****************************************************************************
//
// Macros for calling ROM functions in the Interrupt API.
//
//*****************************************************************************
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_IntEnable                                                         \
        ((void (*)(uint32_t ui32Interrupt))ROM_INTERRUPTTABLE[0])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_IntMasterEnable                                                   \
        ((bool (*)(void))ROM_INTERRUPTTABLE[1])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_IntMasterDisable                                                  \
        ((bool (*)(void))ROM_INTERRUPTTABLE[2])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_IntDisable                                                        \
        ((void (*)(uint32_t ui32Interrupt))ROM_INTERRUPTTABLE[3])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_IntPriorityGroupingSet                                            \
        ((void (*)(uint32_t ui32Bits))ROM_INTERRUPTTABLE[4])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_IntPriorityGroupingGet                                            \
        ((uint32_t (*)(void))ROM_INTERRUPTTABLE[5])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_IntPrioritySet                                                    \
        ((void (*)(uint32_t ui32Interrupt,                                    \
                   uint8_t ui8Priority))ROM_INTERRUPTTABLE[6])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_IntPriorityGet                                                    \
        ((int32_t (*)(uint32_t ui32Interrupt))ROM_INTERRUPTTABLE[7])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_IntPendSet                                                        \
        ((void (*)(uint32_t ui32Interrupt))ROM_INTERRUPTTABLE[8])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_IntPendClear                                                      \
        ((void (*)(uint32_t ui32Interrupt))ROM_INTERRUPTTABLE[9])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_IntPriorityMaskSet                                                \
        ((void (*)(uint32_t ui32PriorityMask))ROM_INTERRUPTTABLE[10])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_IntPriorityMaskGet                                                \
        ((uint32_t (*)(void))ROM_INTERRUPTTABLE[11])
#endif
#if defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_IntIsEnabled                                                      \
        ((uint32_t (*)(uint32_t ui32Interrupt))ROM_INTERRUPTTABLE[12])
#endif

//*****************************************************************************
//
// Macros for calling ROM functions in the LPC API.
//
//*****************************************************************************
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_LPCIntClear                                                       \
        ((void (*)(uint32_t ui32Base,                                         \
                   uint32_t ui32IntFlags))ROM_LPCTABLE[0])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_LPCByteRead                                                       \
        ((uint8_t (*)(uint32_t ui32Base,                                      \
                      uint32_t ui32Offset))ROM_LPCTABLE[1])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_LPCByteWrite                                                      \
        ((void (*)(uint32_t ui32Base,                                         \
                   uint32_t ui32Offset,                                       \
                   uint8_t ui8Data))ROM_LPCTABLE[2])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_LPCChannelConfigCOMxSet                                           \
        ((void (*)(uint32_t ui32Base,                                         \
                   uint32_t ui32Channel,                                      \
                   uint32_t ui32Config,                                       \
                   uint32_t ui32Address,                                      \
                   uint32_t ui32Offset,                                       \
                   uint32_t ui32COMxMode))ROM_LPCTABLE[3])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_LPCChannelConfigGet                                               \
        ((uint32_t (*)(uint32_t ui32Base,                                     \
                       uint32_t ui32Channel,                                  \
                       uint32_t *pui32Address,                                \
                       uint32_t *pui32Offset,                                 \
                       uint32_t *pui32COMxMode))ROM_LPCTABLE[4])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_LPCChannelConfigEPSet                                             \
        ((void (*)(uint32_t ui32Base,                                         \
                   uint32_t ui32Channel,                                      \
                   uint32_t ui32Config,                                       \
                   uint32_t ui32Address,                                      \
                   uint32_t ui32Offset))ROM_LPCTABLE[5])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_LPCChannelConfigMBSet                                             \
        ((void (*)(uint32_t ui32Base,                                         \
                   uint32_t ui32Channel,                                      \
                   uint32_t ui32Config,                                       \
                   uint32_t ui32Address,                                      \
                   uint32_t ui32Offset))ROM_LPCTABLE[6])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_LPCChannelDMAConfigGet                                            \
        ((uint32_t (*)(uint32_t ui32Base))ROM_LPCTABLE[7])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_LPCChannelDMAConfigSet                                            \
        ((void (*)(uint32_t ui32Base,                                         \
                   uint32_t ui32Config,                                       \
                   uint32_t ui32Mask))ROM_LPCTABLE[8])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_LPCChannelDisable                                                 \
        ((void (*)(uint32_t ui32Base,                                         \
                   uint32_t ui32Channel))ROM_LPCTABLE[9])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_LPCChannelEnable                                                  \
        ((void (*)(uint32_t ui32Base,                                         \
                   uint32_t ui32Channel))ROM_LPCTABLE[10])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_LPCChannelStatusClear                                             \
        ((void (*)(uint32_t ui32Base,                                         \
                   uint32_t ui32Channel,                                      \
                   uint32_t ui32Status))ROM_LPCTABLE[11])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_LPCChannelStatusGet                                               \
        ((uint32_t (*)(uint32_t ui32Base,                                     \
                       uint32_t ui32Channel))ROM_LPCTABLE[12])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_LPCChannelStatusSet                                               \
        ((void (*)(uint32_t ui32Base,                                         \
                   uint32_t ui32Channel,                                      \
                   uint32_t ui32Status))ROM_LPCTABLE[13])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_LPCCOMxIntClear                                                   \
        ((void (*)(uint32_t ui32Base,                                         \
                   uint32_t ui32IntFlags))ROM_LPCTABLE[14])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_LPCCOMxIntDisable                                                 \
        ((void (*)(uint32_t ui32Base,                                         \
                   uint32_t ui32IntFlags))ROM_LPCTABLE[15])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_LPCCOMxIntEnable                                                  \
        ((void (*)(uint32_t ui32Base,                                         \
                   uint32_t ui32IntFlags))ROM_LPCTABLE[16])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_LPCCOMxIntStatus                                                  \
        ((uint32_t (*)(uint32_t ui32Base,                                     \
                       bool bMasked))ROM_LPCTABLE[17])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_LPCConfigGet                                                      \
        ((uint32_t (*)(uint32_t ui32Base))ROM_LPCTABLE[18])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_LPCConfigSet                                                      \
        ((void (*)(uint32_t ui32Base,                                         \
                   uint32_t ui32Config))ROM_LPCTABLE[19])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_LPCHalfWordRead                                                   \
        ((uint16_t (*)(uint32_t ui32Base,                                     \
                       uint32_t ui32Offset))ROM_LPCTABLE[20])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_LPCHalfWordWrite                                                  \
        ((void (*)(uint32_t ui32Base,                                         \
                   uint32_t ui32Offset,                                       \
                   uint16_t ui16Data))ROM_LPCTABLE[21])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_LPCIRQClear                                                       \
        ((void (*)(uint32_t ui32Base,                                         \
                   uint32_t ui32IRQ))ROM_LPCTABLE[22])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_LPCIRQConfig                                                      \
        ((void (*)(uint32_t ui32Base,                                         \
                   bool bIRQPulse,                                            \
                   bool bIRQOnChange))ROM_LPCTABLE[23])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_LPCIRQGet                                                         \
        ((uint32_t (*)(uint32_t ui32Base))ROM_LPCTABLE[24])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_LPCIRQSend                                                        \
        ((void (*)(uint32_t ui32Base))ROM_LPCTABLE[25])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_LPCIRQSet                                                         \
        ((void (*)(uint32_t ui32Base,                                         \
                   uint32_t ui32IRQ))ROM_LPCTABLE[26])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_LPCIntDisable                                                     \
        ((void (*)(uint32_t ui32Base,                                         \
                   uint32_t ui32IntFlags))ROM_LPCTABLE[27])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_LPCIntEnable                                                      \
        ((void (*)(uint32_t ui32Base,                                         \
                   uint32_t ui32IntFlags))ROM_LPCTABLE[28])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_LPCIntStatus                                                      \
        ((uint32_t (*)(uint32_t ui32Base,                                     \
                       bool bMasked))ROM_LPCTABLE[29])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_LPCSCIAssert                                                      \
        ((void (*)(uint32_t ui32Base,                                         \
                   uint32_t ui32Count))ROM_LPCTABLE[30])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_LPCStatusGet                                                      \
        ((uint32_t (*)(uint32_t ui32Base,                                     \
                       uint32_t *pui32Count,                                  \
                       uint32_t *pui32PoolSize))ROM_LPCTABLE[31])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_LPCWordRead                                                       \
        ((uint32_t (*)(uint32_t ui32Base,                                     \
                       uint32_t ui32Offset))ROM_LPCTABLE[32])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_LPCWordWrite                                                      \
        ((void (*)(uint32_t ui32Base,                                         \
                   uint32_t ui32Offset,                                       \
                   uint32_t ui32Data))ROM_LPCTABLE[33])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_LPCChannelPoolAddressGet                                          \
        ((uint32_t (*)(uint32_t ui32Base,                                     \
                       uint32_t ui32Channel))ROM_LPCTABLE[34])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_LPCStatusBlockAddressGet                                          \
        ((uint32_t (*)(uint32_t ui32Base))ROM_LPCTABLE[35])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_LPCStatusBlockAddressSet                                          \
        ((void (*)(uint32_t ui32Base,                                         \
                   uint32_t ui32Address,                                      \
                   bool bEnabled))ROM_LPCTABLE[36])
#endif

//*****************************************************************************
//
// Macros for calling ROM functions in the MPU API.
//
//*****************************************************************************
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_MPUEnable                                                         \
        ((void (*)(uint32_t ui32MPUConfig))ROM_MPUTABLE[0])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_MPUDisable                                                        \
        ((void (*)(void))ROM_MPUTABLE[1])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_MPURegionCountGet                                                 \
        ((uint32_t (*)(void))ROM_MPUTABLE[2])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_MPURegionEnable                                                   \
        ((void (*)(uint32_t ui32Region))ROM_MPUTABLE[3])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_MPURegionDisable                                                  \
        ((void (*)(uint32_t ui32Region))ROM_MPUTABLE[4])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_MPURegionSet                                                      \
        ((void (*)(uint32_t ui32Region,                                       \
                   uint32_t ui32Addr,                                         \
                   uint32_t ui32Flags))ROM_MPUTABLE[5])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_MPURegionGet                                                      \
        ((void (*)(uint32_t ui32Region,                                       \
                   uint32_t *pui32Addr,                                       \
                   uint32_t *pui32Flags))ROM_MPUTABLE[6])
#endif

//*****************************************************************************
//
// Macros for calling ROM functions in the PECI API.
//
//*****************************************************************************
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_PECIIntClear                                                      \
        ((void (*)(uint32_t ui32Base,                                         \
                   uint32_t ui32IntFlags))ROM_PECITABLE[0])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_PECIAdvCmdSend                                                    \
        ((void (*)(uint32_t ui32Base,                                         \
                   uint8_t ui8Cmd,                                            \
                   uint8_t ui8HidRe,                                          \
                   uint8_t ui8Domain,                                         \
                   uint8_t ui8Proi8Add,                                       \
                   uint32_t ui32Arg,                                          \
                   uint8_t ui8Size,                                           \
                   uint32_t ui32Data0,                                        \
                   uint32_t ui32Data1))ROM_PECITABLE[1])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_PECIAdvCmdSendNonBlocking                                         \
        ((uint32_t (*)(uint32_t ui32Base,                                     \
                       uint8_t ui8Cmd,                                        \
                       uint8_t ui8HidRe,                                      \
                       uint8_t ui8Domain,                                     \
                       uint8_t ui8Proi8Add,                                   \
                       uint32_t ui32Arg,                                      \
                       uint8_t ui8Size,                                       \
                       uint32_t ui32Data0,                                    \
                       uint32_t ui32Data1))ROM_PECITABLE[2])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_PECIAdvCmdStatusGet                                               \
        ((uint32_t (*)(uint32_t ui32Base,                                     \
                       uint32_t *pui32Data0,                                  \
                       uint32_t *pui32Data1))ROM_PECITABLE[3])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_PECIConfigGet                                                     \
        ((void (*)(uint32_t ui32Base,                                         \
                   uint32_t ui32PECIClk,                                      \
                   uint32_t *pui32Baud,                                       \
                   uint32_t *pui32Poll,                                       \
                   uint32_t *pui32Offset,                                     \
                   uint32_t *pui32Retry))ROM_PECITABLE[4])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_PECIConfigSet                                                     \
        ((void (*)(uint32_t ui32Base,                                         \
                   uint32_t ui32PECIClk,                                      \
                   uint32_t ui32Baud,                                         \
                   uint32_t ui32Poll,                                         \
                   uint32_t ui32Offset,                                       \
                   uint32_t ui32Retry))ROM_PECITABLE[5])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_PECIDomainMaxReadClear                                            \
        ((void (*)(uint32_t ui32Base,                                         \
                   uint32_t ui32Domain))ROM_PECITABLE[6])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_PECIDomainValueClear                                              \
        ((void (*)(uint32_t ui32Base,                                         \
                   uint32_t ui32Domain))ROM_PECITABLE[7])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_PECIDomainConfigGet                                               \
        ((void (*)(uint32_t ui32Base,                                         \
                   uint32_t ui32Domain,                                       \
                   uint32_t *pui32High,                                       \
                   uint32_t *pui32Low))ROM_PECITABLE[8])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_PECIDomainConfigSet                                               \
        ((void (*)(uint32_t ui32Base,                                         \
                   uint32_t ui32Domain,                                       \
                   uint32_t ui32High,                                         \
                   uint32_t ui32Low))ROM_PECITABLE[9])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_PECIDomainDisable                                                 \
        ((void (*)(uint32_t ui32Base,                                         \
                   uint32_t ui32Domain))ROM_PECITABLE[10])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_PECIDomainEnable                                                  \
        ((void (*)(uint32_t ui32Base,                                         \
                   uint32_t ui32Domain))ROM_PECITABLE[11])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_PECIDomainMaxReadGet                                              \
        ((uint32_t (*)(uint32_t ui32Base,                                     \
                       uint32_t ui32Domain))ROM_PECITABLE[12])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_PECIDomainValueGet                                                \
        ((uint32_t (*)(uint32_t ui32Base,                                     \
                       uint32_t ui32Domain))ROM_PECITABLE[13])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_PECIIntDisable                                                    \
        ((void (*)(uint32_t ui32Base,                                         \
                   uint32_t ui32IntFlags))ROM_PECITABLE[14])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_PECIIntEnable                                                     \
        ((void (*)(uint32_t ui32Base,                                         \
                   uint32_t ui32IntFlags,                                     \
                   uint32_t ui32IntMode))ROM_PECITABLE[15])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_PECIIntStatus                                                     \
        ((uint32_t (*)(uint32_t ui32Base,                                     \
                       bool bMasked))ROM_PECITABLE[16])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_PECIBypassEnable                                                  \
        ((void (*)(uint32_t ui32Base))ROM_PECITABLE[17])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_PECIBypassDisable                                                 \
        ((void (*)(uint32_t ui32Base))ROM_PECITABLE[18])
#endif

//*****************************************************************************
//
// Macros for calling ROM functions in the PWM API.
//
//*****************************************************************************
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_PWMPulseWidthSet                                                  \
        ((void (*)(uint32_t ui32Base,                                         \
                   uint32_t ui32PWMOut,                                       \
                   uint32_t ui32Width))ROM_PWMTABLE[0])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_PWMGenConfigure                                                   \
        ((void (*)(uint32_t ui32Base,                                         \
                   uint32_t ui32Gen,                                          \
                   uint32_t ui32Config))ROM_PWMTABLE[1])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_PWMGenPeriodSet                                                   \
        ((void (*)(uint32_t ui32Base,                                         \
                   uint32_t ui32Gen,                                          \
                   uint32_t ui32Period))ROM_PWMTABLE[2])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_PWMGenPeriodGet                                                   \
        ((uint32_t (*)(uint32_t ui32Base,                                     \
                       uint32_t ui32Gen))ROM_PWMTABLE[3])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_PWMGenEnable                                                      \
        ((void (*)(uint32_t ui32Base,                                         \
                   uint32_t ui32Gen))ROM_PWMTABLE[4])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_PWMGenDisable                                                     \
        ((void (*)(uint32_t ui32Base,                                         \
                   uint32_t ui32Gen))ROM_PWMTABLE[5])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_PWMPulseWidthGet                                                  \
        ((uint32_t (*)(uint32_t ui32Base,                                     \
                       uint32_t ui32PWMOut))ROM_PWMTABLE[6])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_PWMDeadBandEnable                                                 \
        ((void (*)(uint32_t ui32Base,                                         \
                   uint32_t ui32Gen,                                          \
                   uint16_t ui16Rise,                                         \
                   uint16_t ui16Fall))ROM_PWMTABLE[7])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_PWMDeadBandDisable                                                \
        ((void (*)(uint32_t ui32Base,                                         \
                   uint32_t ui32Gen))ROM_PWMTABLE[8])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_PWMSyncUpdate                                                     \
        ((void (*)(uint32_t ui32Base,                                         \
                   uint32_t ui32GenBits))ROM_PWMTABLE[9])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_PWMSyncTimeBase                                                   \
        ((void (*)(uint32_t ui32Base,                                         \
                   uint32_t ui32GenBits))ROM_PWMTABLE[10])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_PWMOutputState                                                    \
        ((void (*)(uint32_t ui32Base,                                         \
                   uint32_t ui32PWMOutBits,                                   \
                   bool bEnable))ROM_PWMTABLE[11])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_PWMOutputInvert                                                   \
        ((void (*)(uint32_t ui32Base,                                         \
                   uint32_t ui32PWMOutBits,                                   \
                   bool bInvert))ROM_PWMTABLE[12])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_PWMOutputFault                                                    \
        ((void (*)(uint32_t ui32Base,                                         \
                   uint32_t ui32PWMOutBits,                                   \
                   bool bFaultSuppress))ROM_PWMTABLE[13])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_PWMGenIntTrigEnable                                               \
        ((void (*)(uint32_t ui32Base,                                         \
                   uint32_t ui32Gen,                                          \
                   uint32_t ui32IntTrig))ROM_PWMTABLE[14])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_PWMGenIntTrigDisable                                              \
        ((void (*)(uint32_t ui32Base,                                         \
                   uint32_t ui32Gen,                                          \
                   uint32_t ui32IntTrig))ROM_PWMTABLE[15])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_PWMGenIntStatus                                                   \
        ((uint32_t (*)(uint32_t ui32Base,                                     \
                       uint32_t ui32Gen,                                      \
                       bool bMasked))ROM_PWMTABLE[16])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_PWMGenIntClear                                                    \
        ((void (*)(uint32_t ui32Base,                                         \
                   uint32_t ui32Gen,                                          \
                   uint32_t ui32Ints))ROM_PWMTABLE[17])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_PWMIntEnable                                                      \
        ((void (*)(uint32_t ui32Base,                                         \
                   uint32_t ui32GenFault))ROM_PWMTABLE[18])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_PWMIntDisable                                                     \
        ((void (*)(uint32_t ui32Base,                                         \
                   uint32_t ui32GenFault))ROM_PWMTABLE[19])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_PWMFaultIntClear                                                  \
        ((void (*)(uint32_t ui32Base))ROM_PWMTABLE[20])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_PWMIntStatus                                                      \
        ((uint32_t (*)(uint32_t ui32Base,                                     \
                       bool bMasked))ROM_PWMTABLE[21])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_PWMOutputFaultLevel                                               \
        ((void (*)(uint32_t ui32Base,                                         \
                   uint32_t ui32PWMOutBits,                                   \
                   bool bDriveHigh))ROM_PWMTABLE[22])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_PWMFaultIntClearExt                                               \
        ((void (*)(uint32_t ui32Base,                                         \
                   uint32_t ui32FaultInts))ROM_PWMTABLE[23])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_PWMGenFaultConfigure                                              \
        ((void (*)(uint32_t ui32Base,                                         \
                   uint32_t ui32Gen,                                          \
                   uint32_t ui32MinFaultPeriod,                               \
                   uint32_t ui32FaultSenses))ROM_PWMTABLE[24])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_PWMGenFaultTriggerSet                                             \
        ((void (*)(uint32_t ui32Base,                                         \
                   uint32_t ui32Gen,                                          \
                   uint32_t ui32Group,                                        \
                   uint32_t ui32FaultTriggers))ROM_PWMTABLE[25])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_PWMGenFaultTriggerGet                                             \
        ((uint32_t (*)(uint32_t ui32Base,                                     \
                       uint32_t ui32Gen,                                      \
                       uint32_t ui32Group))ROM_PWMTABLE[26])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_PWMGenFaultStatus                                                 \
        ((uint32_t (*)(uint32_t ui32Base,                                     \
                       uint32_t ui32Gen,                                      \
                       uint32_t ui32Group))ROM_PWMTABLE[27])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_PWMGenFaultClear                                                  \
        ((void (*)(uint32_t ui32Base,                                         \
                   uint32_t ui32Gen,                                          \
                   uint32_t ui32Group,                                        \
                   uint32_t ui32FaultTriggers))ROM_PWMTABLE[28])
#endif

//*****************************************************************************
//
// Macros for calling ROM functions in the QEI API.
//
//*****************************************************************************
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_QEIPositionGet                                                    \
        ((uint32_t (*)(uint32_t ui32Base))ROM_QEITABLE[0])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_QEIEnable                                                         \
        ((void (*)(uint32_t ui32Base))ROM_QEITABLE[1])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_QEIDisable                                                        \
        ((void (*)(uint32_t ui32Base))ROM_QEITABLE[2])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_QEIConfigure                                                      \
        ((void (*)(uint32_t ui32Base,                                         \
                   uint32_t ui32Config,                                       \
                   uint32_t ui32MaxPosition))ROM_QEITABLE[3])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_QEIPositionSet                                                    \
        ((void (*)(uint32_t ui32Base,                                         \
                   uint32_t ui32Position))ROM_QEITABLE[4])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_QEIDirectionGet                                                   \
        ((int32_t (*)(uint32_t ui32Base))ROM_QEITABLE[5])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_QEIErrorGet                                                       \
        ((bool (*)(uint32_t ui32Base))ROM_QEITABLE[6])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_QEIVelocityEnable                                                 \
        ((void (*)(uint32_t ui32Base))ROM_QEITABLE[7])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_QEIVelocityDisable                                                \
        ((void (*)(uint32_t ui32Base))ROM_QEITABLE[8])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_QEIVelocityConfigure                                              \
        ((void (*)(uint32_t ui32Base,                                         \
                   uint32_t ui32PreDiv,                                       \
                   uint32_t ui32Period))ROM_QEITABLE[9])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_QEIVelocityGet                                                    \
        ((uint32_t (*)(uint32_t ui32Base))ROM_QEITABLE[10])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_QEIIntEnable                                                      \
        ((void (*)(uint32_t ui32Base,                                         \
                   uint32_t ui32IntFlags))ROM_QEITABLE[11])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_QEIIntDisable                                                     \
        ((void (*)(uint32_t ui32Base,                                         \
                   uint32_t ui32IntFlags))ROM_QEITABLE[12])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_QEIIntStatus                                                      \
        ((uint32_t (*)(uint32_t ui32Base,                                     \
                       bool bMasked))ROM_QEITABLE[13])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_QEIIntClear                                                       \
        ((void (*)(uint32_t ui32Base,                                         \
                   uint32_t ui32IntFlags))ROM_QEITABLE[14])
#endif

//*****************************************************************************
//
// Macros for calling ROM functions in the SMBus API.
//
//*****************************************************************************
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_SMBusMasterIntProcess                                             \
        ((tSMBusStatus (*)(tSMBus *psSMBus))ROM_SMBUSTABLE[0])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_SMBusARPDisable                                                   \
        ((void (*)(tSMBus *psSMBus))ROM_SMBUSTABLE[1])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_SMBusARPEnable                                                    \
        ((void (*)(tSMBus *psSMBus))ROM_SMBUSTABLE[2])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_SMBusARPUDIDPacketDecode                                          \
        ((void (*)(tSMBusUDID *pUDID,                                         \
                   uint8_t *pui8Address,                                      \
                   uint8_t *pui8Data))ROM_SMBUSTABLE[3])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_SMBusARPUDIDPacketEncode                                          \
        ((void (*)(tSMBusUDID *pUDID,                                         \
                   uint8_t ui8Address,                                        \
                   uint8_t *pui8Data))ROM_SMBUSTABLE[4])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_SMBusMasterARPAssignAddress                                       \
        ((tSMBusStatus (*)(tSMBus *psSMBus,                                   \
                           uint8_t *pui8Data))ROM_SMBUSTABLE[5])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_SMBusMasterARPGetUDIDDir                                          \
        ((tSMBusStatus (*)(tSMBus *psSMBus,                                   \
                           uint8_t ui8TargetAddress,                          \
                           uint8_t *pui8Data))ROM_SMBUSTABLE[6])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_SMBusMasterARPGetUDIDGen                                          \
        ((tSMBusStatus (*)(tSMBus *psSMBus,                                   \
                           uint8_t *pui8Data))ROM_SMBUSTABLE[7])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_SMBusMasterARPNotifyMaster                                        \
        ((tSMBusStatus (*)(tSMBus *psSMBus,                                   \
                           uint8_t *pui8Data))ROM_SMBUSTABLE[8])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_SMBusMasterARPPrepareToARP                                        \
        ((tSMBusStatus (*)(tSMBus *psSMBus))ROM_SMBUSTABLE[9])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_SMBusMasterARPResetDeviceDir                                      \
        ((tSMBusStatus (*)(tSMBus *psSMBus,                                   \
                           uint8_t ui8TargetAddress))ROM_SMBUSTABLE[10])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_SMBusMasterARPResetDeviceGen                                      \
        ((tSMBusStatus (*)(tSMBus *psSMBus))ROM_SMBUSTABLE[11])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_SMBusMasterBlockProcessCall                                       \
        ((tSMBusStatus (*)(tSMBus *psSMBus,                                   \
                           uint8_t ui8TargetAddress,                          \
                           uint8_t ui8Command,                                \
                           uint8_t *pui8TxData,                               \
                           uint8_t ui8TxSize,                                 \
                           uint8_t *pui8RxData))ROM_SMBUSTABLE[12])
#endif
#if defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_SMBusMasterBlockRead                                              \
        ((tSMBusStatus (*)(tSMBus *psSMBus,                                   \
                           uint8_t ui8TargetAddress,                          \
                           uint8_t ui8Command,                                \
                           uint8_t *pui8Data))ROM_SMBUSTABLE[13])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_SMBusMasterBlockWrite                                             \
        ((tSMBusStatus (*)(tSMBus *psSMBus,                                   \
                           uint8_t ui8TargetAddress,                          \
                           uint8_t ui8Command,                                \
                           uint8_t *pui8Data,                                 \
                           uint8_t ui8Size))ROM_SMBUSTABLE[14])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_SMBusMasterByteReceive                                            \
        ((tSMBusStatus (*)(tSMBus *psSMBus,                                   \
                           uint8_t ui8TargetAddress,                          \
                           uint8_t *pui8Data))ROM_SMBUSTABLE[15])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_SMBusMasterByteSend                                               \
        ((tSMBusStatus (*)(tSMBus *psSMBus,                                   \
                           uint8_t ui8TargetAddress,                          \
                           uint8_t ui8Data))ROM_SMBUSTABLE[16])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_SMBusMasterByteWordRead                                           \
        ((tSMBusStatus (*)(tSMBus *psSMBus,                                   \
                           uint8_t ui8TargetAddress,                          \
                           uint8_t ui8Command,                                \
                           uint8_t *pui8Data,                                 \
                           uint8_t ui8Size))ROM_SMBUSTABLE[17])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_SMBusMasterByteWordWrite                                          \
        ((tSMBusStatus (*)(tSMBus *psSMBus,                                   \
                           uint8_t ui8TargetAddress,                          \
                           uint8_t ui8Command,                                \
                           uint8_t *pui8Data,                                 \
                           uint8_t ui8Size))ROM_SMBUSTABLE[18])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_SMBusMasterHostNotify                                             \
        ((tSMBusStatus (*)(tSMBus *psSMBus,                                   \
                           uint8_t ui8OwnSlaveAddress,                        \
                           uint8_t *pui8Data))ROM_SMBUSTABLE[19])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_SMBusMasterI2CRead                                                \
        ((tSMBusStatus (*)(tSMBus *psSMBus,                                   \
                           uint8_t ui8TargetAddress,                          \
                           uint8_t *pui8Data,                                 \
                           uint8_t ui8Size))ROM_SMBUSTABLE[20])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_SMBusMasterI2CWrite                                               \
        ((tSMBusStatus (*)(tSMBus *psSMBus,                                   \
                           uint8_t ui8TargetAddress,                          \
                           uint8_t *pui8Data,                                 \
                           uint8_t ui8Size))ROM_SMBUSTABLE[21])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_SMBusMasterI2CWriteRead                                           \
        ((tSMBusStatus (*)(tSMBus *psSMBus,                                   \
                           uint8_t ui8TargetAddress,                          \
                           uint8_t *pui8TxData,                               \
                           uint8_t ui8TxSize,                                 \
                           uint8_t *pui8RxData,                               \
                           uint8_t ui8RxSize))ROM_SMBUSTABLE[22])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_SMBusMasterInit                                                   \
        ((void (*)(tSMBus *psSMBus,                                           \
                   uint32_t ui32I2CBase,                                      \
                   uint32_t ui32SMBusClock))ROM_SMBUSTABLE[23])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_SMBusMasterIntEnable                                              \
        ((void (*)(tSMBus *psSMBus))ROM_SMBUSTABLE[24])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_SMBusMasterProcessCall                                            \
        ((tSMBusStatus (*)(tSMBus *psSMBus,                                   \
                           uint8_t ui8TargetAddress,                          \
                           uint8_t ui8Command,                                \
                           uint8_t *pui8TxData,                               \
                           uint8_t *pui8RxData))ROM_SMBUSTABLE[25])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_SMBusMasterQuickCommand                                           \
        ((tSMBusStatus (*)(tSMBus *psSMBus,                                   \
                           uint8_t ui8TargetAddress,                          \
                           bool bData))ROM_SMBUSTABLE[26])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_SMBusPECDisable                                                   \
        ((void (*)(tSMBus *psSMBus))ROM_SMBUSTABLE[27])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_SMBusPECEnable                                                    \
        ((void (*)(tSMBus *psSMBus))ROM_SMBUSTABLE[28])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_SMBusRxPacketSizeGet                                              \
        ((uint8_t (*)(tSMBus *psSMBus))ROM_SMBUSTABLE[29])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_SMBusSlaveACKSend                                                 \
        ((void (*)(tSMBus *psSMBus,                                           \
                   bool bACK))ROM_SMBUSTABLE[30])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_SMBusSlaveAddressSet                                              \
        ((void (*)(tSMBus *psSMBus,                                           \
                   uint8_t ui8AddressNum,                                     \
                   uint8_t ui8SlaveAddress))ROM_SMBUSTABLE[31])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_SMBusSlaveARPFlagARGet                                            \
        ((bool (*)(tSMBus *psSMBus))ROM_SMBUSTABLE[32])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_SMBusSlaveARPFlagARSet                                            \
        ((void (*)(tSMBus *psSMBus,                                           \
                   bool bValue))ROM_SMBUSTABLE[33])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_SMBusSlaveARPFlagAVGet                                            \
        ((bool (*)(tSMBus *psSMBus))ROM_SMBUSTABLE[34])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_SMBusSlaveARPFlagAVSet                                            \
        ((void (*)(tSMBus *psSMBus,                                           \
                   bool bValue))ROM_SMBUSTABLE[35])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_SMBusSlaveBlockTransferDisable                                    \
        ((void (*)(tSMBus *psSMBus))ROM_SMBUSTABLE[36])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_SMBusSlaveBlockTransferEnable                                     \
        ((void (*)(tSMBus *psSMBus))ROM_SMBUSTABLE[37])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_SMBusSlaveCommandGet                                              \
        ((uint8_t (*)(tSMBus *psSMBus))ROM_SMBUSTABLE[38])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_SMBusSlaveI2CDisable                                              \
        ((void (*)(tSMBus *psSMBus))ROM_SMBUSTABLE[39])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_SMBusSlaveI2CEnable                                               \
        ((void (*)(tSMBus *psSMBus))ROM_SMBUSTABLE[40])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_SMBusSlaveInit                                                    \
        ((void (*)(tSMBus *psSMBus,                                           \
                   uint32_t ui32I2CBase))ROM_SMBUSTABLE[41])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_SMBusSlaveIntAddressGet                                           \
        ((tSMBusStatus (*)(tSMBus *psSMBus))ROM_SMBUSTABLE[42])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_SMBusSlaveIntEnable                                               \
        ((void (*)(tSMBus *psSMBus))ROM_SMBUSTABLE[43])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_SMBusSlaveIntProcess                                              \
        ((tSMBusStatus (*)(tSMBus *psSMBus))ROM_SMBUSTABLE[44])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_SMBusSlaveManualACKDisable                                        \
        ((void (*)(tSMBus *psSMBus))ROM_SMBUSTABLE[45])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_SMBusSlaveManualACKEnable                                         \
        ((void (*)(tSMBus *psSMBus))ROM_SMBUSTABLE[46])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_SMBusSlaveManualACKStatusGet                                      \
        ((bool (*)(tSMBus *psSMBus))ROM_SMBUSTABLE[47])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_SMBusSlaveProcessCallDisable                                      \
        ((void (*)(tSMBus *psSMBus))ROM_SMBUSTABLE[48])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_SMBusSlaveProcessCallEnable                                       \
        ((void (*)(tSMBus *psSMBus))ROM_SMBUSTABLE[49])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_SMBusSlaveRxBufferSet                                             \
        ((void (*)(tSMBus *psSMBus,                                           \
                   uint8_t *pui8Data,                                         \
                   uint8_t ui8Size))ROM_SMBUSTABLE[50])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_SMBusSlaveTransferInit                                            \
        ((void (*)(tSMBus *psSMBus))ROM_SMBUSTABLE[51])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_SMBusSlaveTxBufferSet                                             \
        ((void (*)(tSMBus *psSMBus,                                           \
                   uint8_t *pui8Data,                                         \
                   uint8_t ui8Size))ROM_SMBUSTABLE[52])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_SMBusSlaveUDIDSet                                                 \
        ((void (*)(tSMBus *psSMBus,                                           \
                   tSMBusUDID *pUDID))ROM_SMBUSTABLE[53])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_SMBusStatusGet                                                    \
        ((tSMBusStatus (*)(tSMBus *psSMBus))ROM_SMBUSTABLE[54])
#endif
#if defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_SMBusSlaveDataSend                                                \
        ((tSMBusStatus (*)(tSMBus *psSMBus))ROM_SMBUSTABLE[55])
#endif

//*****************************************************************************
//
// Macros for calling ROM functions in the SSI API.
//
//*****************************************************************************
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_SSIDataPut                                                        \
        ((void (*)(uint32_t ui32Base,                                         \
                   uint32_t ui32Data))ROM_SSITABLE[0])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_SSIConfigSetExpClk                                                \
        ((void (*)(uint32_t ui32Base,                                         \
                   uint32_t ui32SSIClk,                                       \
                   uint32_t ui32Protocol,                                     \
                   uint32_t ui32Mode,                                         \
                   uint32_t ui32BitRate,                                      \
                   uint32_t ui32DataWidth))ROM_SSITABLE[1])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_SSIEnable                                                         \
        ((void (*)(uint32_t ui32Base))ROM_SSITABLE[2])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_SSIDisable                                                        \
        ((void (*)(uint32_t ui32Base))ROM_SSITABLE[3])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_SSIIntEnable                                                      \
        ((void (*)(uint32_t ui32Base,                                         \
                   uint32_t ui32IntFlags))ROM_SSITABLE[4])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_SSIIntDisable                                                     \
        ((void (*)(uint32_t ui32Base,                                         \
                   uint32_t ui32IntFlags))ROM_SSITABLE[5])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_SSIIntStatus                                                      \
        ((uint32_t (*)(uint32_t ui32Base,                                     \
                       bool bMasked))ROM_SSITABLE[6])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_SSIIntClear                                                       \
        ((void (*)(uint32_t ui32Base,                                         \
                   uint32_t ui32IntFlags))ROM_SSITABLE[7])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_SSIDataPutNonBlocking                                             \
        ((int32_t (*)(uint32_t ui32Base,                                      \
                      uint32_t ui32Data))ROM_SSITABLE[8])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_SSIDataGet                                                        \
        ((void (*)(uint32_t ui32Base,                                         \
                   uint32_t *pui32Data))ROM_SSITABLE[9])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_SSIDataGetNonBlocking                                             \
        ((int32_t (*)(uint32_t ui32Base,                                      \
                      uint32_t *pui32Data))ROM_SSITABLE[10])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_UpdateSSI                                                         \
        ((void (*)(void))ROM_SSITABLE[11])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_SSIDMAEnable                                                      \
        ((void (*)(uint32_t ui32Base,                                         \
                   uint32_t ui32DMAFlags))ROM_SSITABLE[12])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_SSIDMADisable                                                     \
        ((void (*)(uint32_t ui32Base,                                         \
                   uint32_t ui32DMAFlags))ROM_SSITABLE[13])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_SSIBusy                                                           \
        ((bool (*)(uint32_t ui32Base))ROM_SSITABLE[14])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_SSIClockSourceGet                                                 \
        ((uint32_t (*)(uint32_t ui32Base))ROM_SSITABLE[15])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_SSIClockSourceSet                                                 \
        ((void (*)(uint32_t ui32Base,                                         \
                   uint32_t ui32Source))ROM_SSITABLE[16])
#endif

//*****************************************************************************
//
// Macros for calling ROM functions in the SysCtl API.
//
//*****************************************************************************
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_SysCtlSleep                                                       \
        ((void (*)(void))ROM_SYSCTLTABLE[0])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_SysCtlSRAMSizeGet                                                 \
        ((uint32_t (*)(void))ROM_SYSCTLTABLE[1])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_SysCtlFlashSizeGet                                                \
        ((uint32_t (*)(void))ROM_SYSCTLTABLE[2])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_SysCtlPeripheralPresent                                           \
        ((bool (*)(uint32_t ui32Peripheral))ROM_SYSCTLTABLE[4])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_SysCtlPeripheralReset                                             \
        ((void (*)(uint32_t ui32Peripheral))ROM_SYSCTLTABLE[5])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_SysCtlPeripheralEnable                                            \
        ((void (*)(uint32_t ui32Peripheral))ROM_SYSCTLTABLE[6])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_SysCtlPeripheralDisable                                           \
        ((void (*)(uint32_t ui32Peripheral))ROM_SYSCTLTABLE[7])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_SysCtlPeripheralSleepEnable                                       \
        ((void (*)(uint32_t ui32Peripheral))ROM_SYSCTLTABLE[8])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_SysCtlPeripheralSleepDisable                                      \
        ((void (*)(uint32_t ui32Peripheral))ROM_SYSCTLTABLE[9])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_SysCtlPeripheralDeepSleepEnable                                   \
        ((void (*)(uint32_t ui32Peripheral))ROM_SYSCTLTABLE[10])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_SysCtlPeripheralDeepSleepDisable                                  \
        ((void (*)(uint32_t ui32Peripheral))ROM_SYSCTLTABLE[11])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_SysCtlPeripheralClockGating                                       \
        ((void (*)(bool bEnable))ROM_SYSCTLTABLE[12])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_SysCtlIntEnable                                                   \
        ((void (*)(uint32_t ui32Ints))ROM_SYSCTLTABLE[13])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_SysCtlIntDisable                                                  \
        ((void (*)(uint32_t ui32Ints))ROM_SYSCTLTABLE[14])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_SysCtlIntClear                                                    \
        ((void (*)(uint32_t ui32Ints))ROM_SYSCTLTABLE[15])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_SysCtlIntStatus                                                   \
        ((uint32_t (*)(bool bMasked))ROM_SYSCTLTABLE[16])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_SysCtlReset                                                       \
        ((void (*)(void))ROM_SYSCTLTABLE[19])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_SysCtlDeepSleep                                                   \
        ((void (*)(void))ROM_SYSCTLTABLE[20])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_SysCtlResetCauseGet                                               \
        ((uint32_t (*)(void))ROM_SYSCTLTABLE[21])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_SysCtlResetCauseClear                                             \
        ((void (*)(uint32_t ui32Causes))ROM_SYSCTLTABLE[22])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_SysCtlClockSet                                                    \
        ((void (*)(uint32_t ui32Config))ROM_SYSCTLTABLE[23])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_SysCtlClockGet                                                    \
        ((uint32_t (*)(void))ROM_SYSCTLTABLE[24])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_SysCtlPWMClockSet                                                 \
        ((void (*)(uint32_t ui32Config))ROM_SYSCTLTABLE[25])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_SysCtlPWMClockGet                                                 \
        ((uint32_t (*)(void))ROM_SYSCTLTABLE[26])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_SysCtlADCSpeedSet                                                 \
        ((void (*)(uint32_t ui32Speed))ROM_SYSCTLTABLE[27])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_SysCtlADCSpeedGet                                                 \
        ((uint32_t (*)(void))ROM_SYSCTLTABLE[28])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_SysCtlUSBPLLEnable                                                \
        ((void (*)(void))ROM_SYSCTLTABLE[31])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_SysCtlUSBPLLDisable                                               \
        ((void (*)(void))ROM_SYSCTLTABLE[32])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_SysCtlDelay                                                       \
        ((void (*)(uint32_t ui32Count))ROM_SYSCTLTABLE[34])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_SysCtlPeripheralReady                                             \
        ((bool (*)(uint32_t ui32Peripheral))ROM_SYSCTLTABLE[35])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_SysCtlPeripheralPowerOn                                           \
        ((void (*)(uint32_t ui32Peripheral))ROM_SYSCTLTABLE[36])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_SysCtlPeripheralPowerOff                                          \
        ((void (*)(uint32_t ui32Peripheral))ROM_SYSCTLTABLE[37])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_SysCtlMOSCConfigSet                                               \
        ((void (*)(uint32_t ui32Config))ROM_SYSCTLTABLE[44])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_SysCtlPIOSCCalibrate                                              \
        ((uint32_t (*)(uint32_t ui32Type))ROM_SYSCTLTABLE[45])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_SysCtlDeepSleepClockSet                                           \
        ((void (*)(uint32_t ui32Config))ROM_SYSCTLTABLE[46])
#endif

//*****************************************************************************
//
// Macros for calling ROM functions in the SysExc API.
//
//*****************************************************************************
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_SysExcIntStatus                                                   \
        ((uint32_t (*)(bool bMasked))ROM_SYSEXCTABLE[0])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_SysExcIntClear                                                    \
        ((void (*)(uint32_t ui32IntFlags))ROM_SYSEXCTABLE[1])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_SysExcIntDisable                                                  \
        ((void (*)(uint32_t ui32IntFlags))ROM_SYSEXCTABLE[2])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_SysExcIntEnable                                                   \
        ((void (*)(uint32_t ui32IntFlags))ROM_SYSEXCTABLE[3])
#endif

//*****************************************************************************
//
// Macros for calling ROM functions in the SysTick API.
//
//*****************************************************************************
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_SysTickValueGet                                                   \
        ((uint32_t (*)(void))ROM_SYSTICKTABLE[0])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_SysTickEnable                                                     \
        ((void (*)(void))ROM_SYSTICKTABLE[1])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_SysTickDisable                                                    \
        ((void (*)(void))ROM_SYSTICKTABLE[2])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_SysTickIntEnable                                                  \
        ((void (*)(void))ROM_SYSTICKTABLE[3])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_SysTickIntDisable                                                 \
        ((void (*)(void))ROM_SYSTICKTABLE[4])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_SysTickPeriodSet                                                  \
        ((void (*)(uint32_t ui32Period))ROM_SYSTICKTABLE[5])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_SysTickPeriodGet                                                  \
        ((uint32_t (*)(void))ROM_SYSTICKTABLE[6])
#endif

//*****************************************************************************
//
// Macros for calling ROM functions in the Timer API.
//
//*****************************************************************************
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_TimerIntClear                                                     \
        ((void (*)(uint32_t ui32Base,                                         \
                   uint32_t ui32IntFlags))ROM_TIMERTABLE[0])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_TimerEnable                                                       \
        ((void (*)(uint32_t ui32Base,                                         \
                   uint32_t ui32Timer))ROM_TIMERTABLE[1])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_TimerDisable                                                      \
        ((void (*)(uint32_t ui32Base,                                         \
                   uint32_t ui32Timer))ROM_TIMERTABLE[2])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_TimerConfigure                                                    \
        ((void (*)(uint32_t ui32Base,                                         \
                   uint32_t ui32Config))ROM_TIMERTABLE[3])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_TimerControlLevel                                                 \
        ((void (*)(uint32_t ui32Base,                                         \
                   uint32_t ui32Timer,                                        \
                   bool bInvert))ROM_TIMERTABLE[4])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_TimerControlTrigger                                               \
        ((void (*)(uint32_t ui32Base,                                         \
                   uint32_t ui32Timer,                                        \
                   bool bEnable))ROM_TIMERTABLE[5])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_TimerControlEvent                                                 \
        ((void (*)(uint32_t ui32Base,                                         \
                   uint32_t ui32Timer,                                        \
                   uint32_t ui32Event))ROM_TIMERTABLE[6])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_TimerControlStall                                                 \
        ((void (*)(uint32_t ui32Base,                                         \
                   uint32_t ui32Timer,                                        \
                   bool bStall))ROM_TIMERTABLE[7])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_TimerRTCEnable                                                    \
        ((void (*)(uint32_t ui32Base))ROM_TIMERTABLE[8])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_TimerRTCDisable                                                   \
        ((void (*)(uint32_t ui32Base))ROM_TIMERTABLE[9])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_TimerPrescaleSet                                                  \
        ((void (*)(uint32_t ui32Base,                                         \
                   uint32_t ui32Timer,                                        \
                   uint32_t ui32Value))ROM_TIMERTABLE[10])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_TimerPrescaleGet                                                  \
        ((uint32_t (*)(uint32_t ui32Base,                                     \
                       uint32_t ui32Timer))ROM_TIMERTABLE[11])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_TimerPrescaleMatchSet                                             \
        ((void (*)(uint32_t ui32Base,                                         \
                   uint32_t ui32Timer,                                        \
                   uint32_t ui32Value))ROM_TIMERTABLE[12])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_TimerPrescaleMatchGet                                             \
        ((uint32_t (*)(uint32_t ui32Base,                                     \
                       uint32_t ui32Timer))ROM_TIMERTABLE[13])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_TimerLoadSet                                                      \
        ((void (*)(uint32_t ui32Base,                                         \
                   uint32_t ui32Timer,                                        \
                   uint32_t ui32Value))ROM_TIMERTABLE[14])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_TimerLoadGet                                                      \
        ((uint32_t (*)(uint32_t ui32Base,                                     \
                       uint32_t ui32Timer))ROM_TIMERTABLE[15])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_TimerValueGet                                                     \
        ((uint32_t (*)(uint32_t ui32Base,                                     \
                       uint32_t ui32Timer))ROM_TIMERTABLE[16])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_TimerMatchSet                                                     \
        ((void (*)(uint32_t ui32Base,                                         \
                   uint32_t ui32Timer,                                        \
                   uint32_t ui32Value))ROM_TIMERTABLE[17])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_TimerMatchGet                                                     \
        ((uint32_t (*)(uint32_t ui32Base,                                     \
                       uint32_t ui32Timer))ROM_TIMERTABLE[18])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_TimerIntEnable                                                    \
        ((void (*)(uint32_t ui32Base,                                         \
                   uint32_t ui32IntFlags))ROM_TIMERTABLE[19])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_TimerIntDisable                                                   \
        ((void (*)(uint32_t ui32Base,                                         \
                   uint32_t ui32IntFlags))ROM_TIMERTABLE[20])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_TimerIntStatus                                                    \
        ((uint32_t (*)(uint32_t ui32Base,                                     \
                       bool bMasked))ROM_TIMERTABLE[21])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_TimerControlWaitOnTrigger                                         \
        ((void (*)(uint32_t ui32Base,                                         \
                   uint32_t ui32Timer,                                        \
                   bool bWait))ROM_TIMERTABLE[22])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_TimerLoadSet64                                                    \
        ((void (*)(uint32_t ui32Base,                                         \
                   uint64_t ui64Value))ROM_TIMERTABLE[23])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_TimerLoadGet64                                                    \
        ((uint64_t (*)(uint32_t ui32Base))ROM_TIMERTABLE[24])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_TimerValueGet64                                                   \
        ((uint64_t (*)(uint32_t ui32Base))ROM_TIMERTABLE[25])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_TimerMatchSet64                                                   \
        ((void (*)(uint32_t ui32Base,                                         \
                   uint64_t ui64Value))ROM_TIMERTABLE[26])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_TimerMatchGet64                                                   \
        ((uint64_t (*)(uint32_t ui32Base))ROM_TIMERTABLE[27])
#endif

//*****************************************************************************
//
// Macros for calling ROM functions in the UART API.
//
//*****************************************************************************
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_UARTCharPut                                                       \
        ((void (*)(uint32_t ui32Base,                                         \
                   unsigned char ucData))ROM_UARTTABLE[0])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_UARTParityModeSet                                                 \
        ((void (*)(uint32_t ui32Base,                                         \
                   uint32_t ui32Parity))ROM_UARTTABLE[1])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_UARTParityModeGet                                                 \
        ((uint32_t (*)(uint32_t ui32Base))ROM_UARTTABLE[2])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_UARTFIFOLevelSet                                                  \
        ((void (*)(uint32_t ui32Base,                                         \
                   uint32_t ui32TxLevel,                                      \
                   uint32_t ui32RxLevel))ROM_UARTTABLE[3])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_UARTFIFOLevelGet                                                  \
        ((void (*)(uint32_t ui32Base,                                         \
                   uint32_t *pui32TxLevel,                                    \
                   uint32_t *pui32RxLevel))ROM_UARTTABLE[4])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_UARTConfigSetExpClk                                               \
        ((void (*)(uint32_t ui32Base,                                         \
                   uint32_t ui32UARTClk,                                      \
                   uint32_t ui32Baud,                                         \
                   uint32_t ui32Config))ROM_UARTTABLE[5])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_UARTConfigGetExpClk                                               \
        ((void (*)(uint32_t ui32Base,                                         \
                   uint32_t ui32UARTClk,                                      \
                   uint32_t *pui32Baud,                                       \
                   uint32_t *pui32Config))ROM_UARTTABLE[6])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_UARTEnable                                                        \
        ((void (*)(uint32_t ui32Base))ROM_UARTTABLE[7])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_UARTDisable                                                       \
        ((void (*)(uint32_t ui32Base))ROM_UARTTABLE[8])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_UARTEnableSIR                                                     \
        ((void (*)(uint32_t ui32Base,                                         \
                   bool bLowPower))ROM_UARTTABLE[9])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_UARTDisableSIR                                                    \
        ((void (*)(uint32_t ui32Base))ROM_UARTTABLE[10])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_UARTCharsAvail                                                    \
        ((bool (*)(uint32_t ui32Base))ROM_UARTTABLE[11])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_UARTSpaceAvail                                                    \
        ((bool (*)(uint32_t ui32Base))ROM_UARTTABLE[12])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_UARTCharGetNonBlocking                                            \
        ((int32_t (*)(uint32_t ui32Base))ROM_UARTTABLE[13])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_UARTCharGet                                                       \
        ((int32_t (*)(uint32_t ui32Base))ROM_UARTTABLE[14])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_UARTCharPutNonBlocking                                            \
        ((bool (*)(uint32_t ui32Base,                                         \
                   unsigned char ucData))ROM_UARTTABLE[15])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_UARTBreakCtl                                                      \
        ((void (*)(uint32_t ui32Base,                                         \
                   bool bBreakState))ROM_UARTTABLE[16])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_UARTIntEnable                                                     \
        ((void (*)(uint32_t ui32Base,                                         \
                   uint32_t ui32IntFlags))ROM_UARTTABLE[17])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_UARTIntDisable                                                    \
        ((void (*)(uint32_t ui32Base,                                         \
                   uint32_t ui32IntFlags))ROM_UARTTABLE[18])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_UARTIntStatus                                                     \
        ((uint32_t (*)(uint32_t ui32Base,                                     \
                       bool bMasked))ROM_UARTTABLE[19])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_UARTIntClear                                                      \
        ((void (*)(uint32_t ui32Base,                                         \
                   uint32_t ui32IntFlags))ROM_UARTTABLE[20])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_UpdateUART                                                        \
        ((void (*)(void))ROM_UARTTABLE[21])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_UARTDMAEnable                                                     \
        ((void (*)(uint32_t ui32Base,                                         \
                   uint32_t ui32DMAFlags))ROM_UARTTABLE[22])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_UARTDMADisable                                                    \
        ((void (*)(uint32_t ui32Base,                                         \
                   uint32_t ui32DMAFlags))ROM_UARTTABLE[23])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_UARTFIFOEnable                                                    \
        ((void (*)(uint32_t ui32Base))ROM_UARTTABLE[24])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_UARTFIFODisable                                                   \
        ((void (*)(uint32_t ui32Base))ROM_UARTTABLE[25])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_UARTBusy                                                          \
        ((bool (*)(uint32_t ui32Base))ROM_UARTTABLE[26])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_UARTTxIntModeSet                                                  \
        ((void (*)(uint32_t ui32Base,                                         \
                   uint32_t ui32Mode))ROM_UARTTABLE[27])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_UARTTxIntModeGet                                                  \
        ((uint32_t (*)(uint32_t ui32Base))ROM_UARTTABLE[28])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_UARTRxErrorGet                                                    \
        ((uint32_t (*)(uint32_t ui32Base))ROM_UARTTABLE[29])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_UARTRxErrorClear                                                  \
        ((void (*)(uint32_t ui32Base))ROM_UARTTABLE[30])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_UARTClockSourceSet                                                \
        ((void (*)(uint32_t ui32Base,                                         \
                   uint32_t ui32Source))ROM_UARTTABLE[31])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_UARTClockSourceGet                                                \
        ((uint32_t (*)(uint32_t ui32Base))ROM_UARTTABLE[32])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_UART9BitEnable                                                    \
        ((void (*)(uint32_t ui32Base))ROM_UARTTABLE[33])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_UART9BitDisable                                                   \
        ((void (*)(uint32_t ui32Base))ROM_UARTTABLE[34])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_UART9BitAddrSet                                                   \
        ((void (*)(uint32_t ui32Base,                                         \
                   uint8_t ui8Addr,                                           \
                   uint8_t ui8Mask))ROM_UARTTABLE[35])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_UART9BitAddrSend                                                  \
        ((void (*)(uint32_t ui32Base,                                         \
                   uint8_t ui8Addr))ROM_UARTTABLE[36])
#endif

//*****************************************************************************
//
// Macros for calling ROM functions in the uDMA API.
//
//*****************************************************************************
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_uDMAChannelTransferSet                                            \
        ((void (*)(uint32_t ui32ChannelStructIndex,                           \
                   uint32_t ui32Mode,                                         \
                   void *pvSrcAddr,                                           \
                   void *pvDstAddr,                                           \
                   uint32_t ui32TransferSize))ROM_UDMATABLE[0])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_uDMAEnable                                                        \
        ((void (*)(void))ROM_UDMATABLE[1])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_uDMADisable                                                       \
        ((void (*)(void))ROM_UDMATABLE[2])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_uDMAErrorStatusGet                                                \
        ((uint32_t (*)(void))ROM_UDMATABLE[3])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_uDMAErrorStatusClear                                              \
        ((void (*)(void))ROM_UDMATABLE[4])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_uDMAChannelEnable                                                 \
        ((void (*)(uint32_t ui32ChannelNum))ROM_UDMATABLE[5])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_uDMAChannelDisable                                                \
        ((void (*)(uint32_t ui32ChannelNum))ROM_UDMATABLE[6])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_uDMAChannelIsEnabled                                              \
        ((bool (*)(uint32_t ui32ChannelNum))ROM_UDMATABLE[7])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_uDMAControlBaseSet                                                \
        ((void (*)(void *pControlTable))ROM_UDMATABLE[8])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_uDMAControlBaseGet                                                \
        ((void * (*)(void))ROM_UDMATABLE[9])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_uDMAChannelRequest                                                \
        ((void (*)(uint32_t ui32ChannelNum))ROM_UDMATABLE[10])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_uDMAChannelAttributeEnable                                        \
        ((void (*)(uint32_t ui32ChannelNum,                                   \
                   uint32_t ui32Attr))ROM_UDMATABLE[11])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_uDMAChannelAttributeDisable                                       \
        ((void (*)(uint32_t ui32ChannelNum,                                   \
                   uint32_t ui32Attr))ROM_UDMATABLE[12])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_uDMAChannelAttributeGet                                           \
        ((uint32_t (*)(uint32_t ui32ChannelNum))ROM_UDMATABLE[13])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_uDMAChannelControlSet                                             \
        ((void (*)(uint32_t ui32ChannelStructIndex,                           \
                   uint32_t ui32Control))ROM_UDMATABLE[14])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_uDMAChannelSizeGet                                                \
        ((uint32_t (*)(uint32_t ui32ChannelStructIndex))ROM_UDMATABLE[15])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_uDMAChannelModeGet                                                \
        ((uint32_t (*)(uint32_t ui32ChannelStructIndex))ROM_UDMATABLE[16])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_uDMAChannelSelectSecondary                                        \
        ((void (*)(uint32_t ui32SecPeriphs))ROM_UDMATABLE[17])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_uDMAChannelSelectDefault                                          \
        ((void (*)(uint32_t ui32DefPeriphs))ROM_UDMATABLE[18])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_uDMAIntStatus                                                     \
        ((uint32_t (*)(void))ROM_UDMATABLE[19])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_uDMAIntClear                                                      \
        ((void (*)(uint32_t ui32ChanMask))ROM_UDMATABLE[20])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_uDMAControlAlternateBaseGet                                       \
        ((void * (*)(void))ROM_UDMATABLE[21])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_uDMAChannelScatterGatherSet                                       \
        ((void (*)(uint32_t ui32ChannelNum,                                   \
                   uint32_t ui32TaskCount,                                    \
                   void *pvTaskList,                                          \
                   uint32_t ui32IsPeriphSG))ROM_UDMATABLE[22])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_uDMAChannelAssign                                                 \
        ((void (*)(uint32_t ui32Mapping))ROM_UDMATABLE[23])
#endif

//*****************************************************************************
//
// Macros for calling ROM functions in the USB API.
//
//*****************************************************************************
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_USBDevAddrGet                                                     \
        ((uint32_t (*)(uint32_t ui32Base))ROM_USBTABLE[1])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_USBDevAddrSet                                                     \
        ((void (*)(uint32_t ui32Base,                                         \
                   uint32_t ui32Address))ROM_USBTABLE[2])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_USBDevConnect                                                     \
        ((void (*)(uint32_t ui32Base))ROM_USBTABLE[3])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_USBDevDisconnect                                                  \
        ((void (*)(uint32_t ui32Base))ROM_USBTABLE[4])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_USBDevEndpointConfigSet                                           \
        ((void (*)(uint32_t ui32Base,                                         \
                   uint32_t ui32Endpoint,                                     \
                   uint32_t ui32MaxPacketSize,                                \
                   uint32_t ui32Flags))ROM_USBTABLE[5])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_USBDevEndpointDataAck                                             \
        ((void (*)(uint32_t ui32Base,                                         \
                   uint32_t ui32Endpoint,                                     \
                   bool bIsLastPacket))ROM_USBTABLE[6])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_USBDevEndpointStall                                               \
        ((void (*)(uint32_t ui32Base,                                         \
                   uint32_t ui32Endpoint,                                     \
                   uint32_t ui32Flags))ROM_USBTABLE[7])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_USBDevEndpointStallClear                                          \
        ((void (*)(uint32_t ui32Base,                                         \
                   uint32_t ui32Endpoint,                                     \
                   uint32_t ui32Flags))ROM_USBTABLE[8])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_USBDevEndpointStatusClear                                         \
        ((void (*)(uint32_t ui32Base,                                         \
                   uint32_t ui32Endpoint,                                     \
                   uint32_t ui32Flags))ROM_USBTABLE[9])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_USBEndpointDataGet                                                \
        ((int32_t (*)(uint32_t ui32Base,                                      \
                      uint32_t ui32Endpoint,                                  \
                      uint8_t *pui8Data,                                      \
                      uint32_t *pui32Size))ROM_USBTABLE[10])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_USBEndpointDataPut                                                \
        ((int32_t (*)(uint32_t ui32Base,                                      \
                      uint32_t ui32Endpoint,                                  \
                      uint8_t *pui8Data,                                      \
                      uint32_t ui32Size))ROM_USBTABLE[11])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_USBEndpointDataSend                                               \
        ((int32_t (*)(uint32_t ui32Base,                                      \
                      uint32_t ui32Endpoint,                                  \
                      uint32_t ui32TransType))ROM_USBTABLE[12])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_USBEndpointDataToggleClear                                        \
        ((void (*)(uint32_t ui32Base,                                         \
                   uint32_t ui32Endpoint,                                     \
                   uint32_t ui32Flags))ROM_USBTABLE[13])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_USBEndpointStatus                                                 \
        ((uint32_t (*)(uint32_t ui32Base,                                     \
                       uint32_t ui32Endpoint))ROM_USBTABLE[14])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_USBFIFOAddrGet                                                    \
        ((uint32_t (*)(uint32_t ui32Base,                                     \
                       uint32_t ui32Endpoint))ROM_USBTABLE[15])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_USBFIFOConfigGet                                                  \
        ((void (*)(uint32_t ui32Base,                                         \
                   uint32_t ui32Endpoint,                                     \
                   uint32_t *pui32FIFOAddress,                                \
                   uint32_t *pui32FIFOSize,                                   \
                   uint32_t ui32Flags))ROM_USBTABLE[16])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_USBFIFOConfigSet                                                  \
        ((void (*)(uint32_t ui32Base,                                         \
                   uint32_t ui32Endpoint,                                     \
                   uint32_t ui32FIFOAddress,                                  \
                   uint32_t ui32FIFOSize,                                     \
                   uint32_t ui32Flags))ROM_USBTABLE[17])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_USBFIFOFlush                                                      \
        ((void (*)(uint32_t ui32Base,                                         \
                   uint32_t ui32Endpoint,                                     \
                   uint32_t ui32Flags))ROM_USBTABLE[18])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_USBFrameNumberGet                                                 \
        ((uint32_t (*)(uint32_t ui32Base))ROM_USBTABLE[19])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_USBHostAddrGet                                                    \
        ((uint32_t (*)(uint32_t ui32Base,                                     \
                       uint32_t ui32Endpoint,                                 \
                       uint32_t ui32Flags))ROM_USBTABLE[20])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_USBHostAddrSet                                                    \
        ((void (*)(uint32_t ui32Base,                                         \
                   uint32_t ui32Endpoint,                                     \
                   uint32_t ui32Addr,                                         \
                   uint32_t ui32Flags))ROM_USBTABLE[21])
#endif
#if defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_USBHostEndpointConfig                                             \
        ((void (*)(uint32_t ui32Base,                                         \
                   uint32_t ui32Endpoint,                                     \
                   uint32_t ui32MaxPacketSize,                                \
                   uint32_t ui32NAKPollInterval,                              \
                   uint32_t ui32TargetEndpoint,                               \
                   uint32_t ui32Flags))ROM_USBTABLE[22])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_USBHostEndpointDataAck                                            \
        ((void (*)(uint32_t ui32Base,                                         \
                   uint32_t ui32Endpoint))ROM_USBTABLE[23])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_USBHostEndpointDataToggle                                         \
        ((void (*)(uint32_t ui32Base,                                         \
                   uint32_t ui32Endpoint,                                     \
                   bool bDataToggle,                                          \
                   uint32_t ui32Flags))ROM_USBTABLE[24])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_USBHostEndpointStatusClear                                        \
        ((void (*)(uint32_t ui32Base,                                         \
                   uint32_t ui32Endpoint,                                     \
                   uint32_t ui32Flags))ROM_USBTABLE[25])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_USBHostHubAddrGet                                                 \
        ((uint32_t (*)(uint32_t ui32Base,                                     \
                       uint32_t ui32Endpoint,                                 \
                       uint32_t ui32Flags))ROM_USBTABLE[26])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_USBHostHubAddrSet                                                 \
        ((void (*)(uint32_t ui32Base,                                         \
                   uint32_t ui32Endpoint,                                     \
                   uint32_t ui32Addr,                                         \
                   uint32_t ui32Flags))ROM_USBTABLE[27])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_USBHostPwrDisable                                                 \
        ((void (*)(uint32_t ui32Base))ROM_USBTABLE[28])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_USBHostPwrEnable                                                  \
        ((void (*)(uint32_t ui32Base))ROM_USBTABLE[29])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_USBHostPwrConfig                                                  \
        ((void (*)(uint32_t ui32Base,                                         \
                   uint32_t ui32Flags))ROM_USBTABLE[30])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_USBHostPwrFaultDisable                                            \
        ((void (*)(uint32_t ui32Base))ROM_USBTABLE[31])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_USBHostPwrFaultEnable                                             \
        ((void (*)(uint32_t ui32Base))ROM_USBTABLE[32])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_USBHostRequestIN                                                  \
        ((void (*)(uint32_t ui32Base,                                         \
                   uint32_t ui32Endpoint))ROM_USBTABLE[33])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_USBHostRequestStatus                                              \
        ((void (*)(uint32_t ui32Base))ROM_USBTABLE[34])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_USBHostReset                                                      \
        ((void (*)(uint32_t ui32Base,                                         \
                   bool bStart))ROM_USBTABLE[35])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_USBHostResume                                                     \
        ((void (*)(uint32_t ui32Base,                                         \
                   bool bStart))ROM_USBTABLE[36])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_USBHostSpeedGet                                                   \
        ((uint32_t (*)(uint32_t ui32Base))ROM_USBTABLE[37])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_USBHostSuspend                                                    \
        ((void (*)(uint32_t ui32Base))ROM_USBTABLE[38])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_USBDevEndpointConfigGet                                           \
        ((void (*)(uint32_t ui32Base,                                         \
                   uint32_t ui32Endpoint,                                     \
                   uint32_t *pui32MaxPacketSize,                              \
                   uint32_t *pui32Flags))ROM_USBTABLE[41])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_USBEndpointDMAEnable                                              \
        ((void (*)(uint32_t ui32Base,                                         \
                   uint32_t ui32Endpoint,                                     \
                   uint32_t ui32Flags))ROM_USBTABLE[42])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_USBEndpointDMADisable                                             \
        ((void (*)(uint32_t ui32Base,                                         \
                   uint32_t ui32Endpoint,                                     \
                   uint32_t ui32Flags))ROM_USBTABLE[43])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_USBEndpointDataAvail                                              \
        ((uint32_t (*)(uint32_t ui32Base,                                     \
                       uint32_t ui32Endpoint))ROM_USBTABLE[44])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_USBModeGet                                                        \
        ((uint32_t (*)(uint32_t ui32Base))ROM_USBTABLE[46])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_USBEndpointDMAChannel                                             \
        ((void (*)(uint32_t ui32Base,                                         \
                   uint32_t ui32Endpoint,                                     \
                   uint32_t ui32Channel))ROM_USBTABLE[47])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_USBIntDisableControl                                              \
        ((void (*)(uint32_t ui32Base,                                         \
                   uint32_t ui32IntFlags))ROM_USBTABLE[48])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_USBIntEnableControl                                               \
        ((void (*)(uint32_t ui32Base,                                         \
                   uint32_t ui32IntFlags))ROM_USBTABLE[49])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_USBIntStatusControl                                               \
        ((uint32_t (*)(uint32_t ui32Base))ROM_USBTABLE[50])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_USBIntDisableEndpoint                                             \
        ((void (*)(uint32_t ui32Base,                                         \
                   uint32_t ui32IntFlags))ROM_USBTABLE[51])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_USBIntEnableEndpoint                                              \
        ((void (*)(uint32_t ui32Base,                                         \
                   uint32_t ui32IntFlags))ROM_USBTABLE[52])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_USBIntStatusEndpoint                                              \
        ((uint32_t (*)(uint32_t ui32Base))ROM_USBTABLE[53])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_USBHostMode                                                       \
        ((void (*)(uint32_t ui32Base))ROM_USBTABLE[54])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_USBDevMode                                                        \
        ((void (*)(uint32_t ui32Base))ROM_USBTABLE[55])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_USBPHYPowerOff                                                    \
        ((void (*)(uint32_t ui32Base))ROM_USBTABLE[56])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_USBPHYPowerOn                                                     \
        ((void (*)(uint32_t ui32Base))ROM_USBTABLE[57])
#endif
#if defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_UpdateUSB                                                         \
        ((void (*)(uint8_t *pui8DescriptorInfo))ROM_USBTABLE[58])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_USBOTGMode                                                        \
        ((void (*)(uint32_t ui32Base))ROM_USBTABLE[59])
#endif
#if defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_USBHostRequestINClear                                             \
        ((void (*)(uint32_t ui32Base,                                         \
                   uint32_t ui32Endpoint))ROM_USBTABLE[60])
#endif
#if defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_USBNumEndpointsGet                                                \
        ((uint32_t (*)(uint32_t ui32Base))ROM_USBTABLE[61])
#endif

//*****************************************************************************
//
// Macros for calling ROM functions in the Watchdog API.
//
//*****************************************************************************
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_WatchdogIntClear                                                  \
        ((void (*)(uint32_t ui32Base))ROM_WATCHDOGTABLE[0])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_WatchdogRunning                                                   \
        ((bool (*)(uint32_t ui32Base))ROM_WATCHDOGTABLE[1])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_WatchdogEnable                                                    \
        ((void (*)(uint32_t ui32Base))ROM_WATCHDOGTABLE[2])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_WatchdogResetEnable                                               \
        ((void (*)(uint32_t ui32Base))ROM_WATCHDOGTABLE[3])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_WatchdogResetDisable                                              \
        ((void (*)(uint32_t ui32Base))ROM_WATCHDOGTABLE[4])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_WatchdogLock                                                      \
        ((void (*)(uint32_t ui32Base))ROM_WATCHDOGTABLE[5])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_WatchdogUnlock                                                    \
        ((void (*)(uint32_t ui32Base))ROM_WATCHDOGTABLE[6])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_WatchdogLockState                                                 \
        ((bool (*)(uint32_t ui32Base))ROM_WATCHDOGTABLE[7])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_WatchdogReloadSet                                                 \
        ((void (*)(uint32_t ui32Base,                                         \
                   uint32_t ui32LoadVal))ROM_WATCHDOGTABLE[8])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_WatchdogReloadGet                                                 \
        ((uint32_t (*)(uint32_t ui32Base))ROM_WATCHDOGTABLE[9])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_WatchdogValueGet                                                  \
        ((uint32_t (*)(uint32_t ui32Base))ROM_WATCHDOGTABLE[10])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_WatchdogIntEnable                                                 \
        ((void (*)(uint32_t ui32Base))ROM_WATCHDOGTABLE[11])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_WatchdogIntStatus                                                 \
        ((uint32_t (*)(uint32_t ui32Base,                                     \
                       bool bMasked))ROM_WATCHDOGTABLE[12])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_WatchdogStallEnable                                               \
        ((void (*)(uint32_t ui32Base))ROM_WATCHDOGTABLE[13])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_WatchdogStallDisable                                              \
        ((void (*)(uint32_t ui32Base))ROM_WATCHDOGTABLE[14])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_WatchdogIntTypeSet                                                \
        ((void (*)(uint32_t ui32Base,                                         \
                   uint32_t ui32Type))ROM_WATCHDOGTABLE[15])
#endif

//*****************************************************************************
//
// Macros for calling ROM functions in the Software API.
//
//*****************************************************************************
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_Crc16Array                                                        \
        ((uint16_t (*)(uint32_t ui32WordLen,                                  \
                       const uint32_t *pui32Data))ROM_SOFTWARETABLE[1])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_Crc16Array3                                                       \
        ((void (*)(uint32_t ui32WordLen,                                      \
                   const uint32_t *pui32Data,                                 \
                   uint16_t *pui16Crc3))ROM_SOFTWARETABLE[2])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_Crc16                                                             \
        ((uint16_t (*)(uint16_t ui16Crc,                                      \
                       const uint8_t *pui8Data,                               \
                       uint32_t ui32Count))ROM_SOFTWARETABLE[3])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_Crc8CCITT                                                         \
        ((uint8_t (*)(uint8_t ui8Crc,                                         \
                      const uint8_t *pui8Data,                                \
                      uint32_t ui32Count))ROM_SOFTWARETABLE[4])
#endif
#if defined(TARGET_IS_BLIZZARD_RA1) ||                                        \
    defined(TARGET_IS_BLIZZARD_RA3) ||                                        \
    defined(TARGET_IS_BLIZZARD_RB1)
#define ROM_pvAESTable                                                        \
        ((void *)&(ROM_SOFTWARETABLE[7]))
#endif

#endif // __ROM_H__

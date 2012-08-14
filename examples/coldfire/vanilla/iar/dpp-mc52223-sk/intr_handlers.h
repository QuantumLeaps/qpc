/*************************************************************************
 *
 *    Used with ICCCF and ACF.
 *
 *    (c) Copyright IAR Systems 2007
 *
 *    File name   : intr_handlers.h
 *    Description : Interrupt Handlers include file
 *
 *    History :
 *    1. Date        : 6, September 2007
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *    $Revision: 814 $
 **************************************************************************/

#ifndef __INTR_HANDLERS_H
#define __INTR_HANDLERS_H

__interrupt void ReservedHandler(void);
__interrupt void AccessErrorHandler(void);
__interrupt void AddressErrorHandler(void);
__interrupt void InllegalInstrHandler(void);
__interrupt void DivByZeroHandler(void);
__interrupt void PrivViolationHandler(void);
__interrupt void TraceHandler(void);
__interrupt void UnimplemLineA_OpcOdeHandler(void);
__interrupt void UnimplemLineF_OpcOdeHandler(void);
__interrupt void DebugHandler(void);
__interrupt void FormatErrorHandler(void);
__interrupt void SpuriousIntrHandler(void);
__interrupt void Trap0Handler(void);
__interrupt void Trap1Handler(void);
__interrupt void Trap2Handler(void);
__interrupt void Trap3Handler(void);
__interrupt void Trap4Handler(void);
__interrupt void Trap5Handler(void);
__interrupt void Trap6Handler(void);
__interrupt void Trap7Handler(void);
__interrupt void Trap8Handler(void);
__interrupt void Trap9Handler(void);
__interrupt void Trap10Handler(void);
__interrupt void Trap11Handler(void);
__interrupt void Trap12Handler(void);
__interrupt void Trap13Handler(void);
__interrupt void Trap14Handler(void);
__interrupt void Trap15Handler(void);

#endif // __INTR_HANDLERS_H

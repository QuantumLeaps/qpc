;\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
;\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
;******************************************************************************
;Copyright (c) Texas Instruments Inc 2002, 2003, 2004, 2005, 2008
;*
;*  Use of this software is controlled by the terms and conditions found in the
;*  license agreement under which this software has been supplied.
;******************************************************************************
; Module: INTC
; Function: IRQ_PLUG
; File: csl_irqplug.asm
;
;
;--------------------------------------------------------
; Description:
;   This function writes the address of the given ISR into
; the correct location in the interrupt vector table.
;
; Arguments:
;     T0 - contains EventId of associated vector
;     AC0 - contains address of ISR
;
; Local Function Variables:
;     SP(#0)   - location in vector table where ISR address
;                needs to be placed
;     SP(#2)   - ISR function address
;     SP(#4)   - IVPD pointer value
;     SP(#5)   - Bit in IER corresponding to event
;     SP(#6)   - Mask used to clear event in IFR
;     SP(#7)   - EventId
;     SP(#8)   - old_intm, old value of INTM bit
;////////////////////////////////////////////////////////
    .asg 123, CSL_DATA_PTR
    .asg 05E80h, NOP16_Operator
    .asg 0, VecLoc
    .asg 2, IsrAddr
    .asg 4, Iptr
    .asg 5, EventBit
    .asg 6, EventMask
    .asg 7, EventId
    .asg 8, Old_INTM
    .asg 9, LocalFrameSz
    .asg 12, BiosPresentFlag
    .asg 049h, IVPD_ADDR
    .asg 04Ah, IVPH_ADDR
    .asg 001h, IFR0_ADDR
    .asg 046h, IFR1_ADDR

    .mmregs

    .def _IRQ_plug

    .ref _IRQ_globalDisable
    .ref _IRQ_globalRestore
      .cpl_on
_IRQ_plug:
      PSHBOTH(XAR3)
      PSHBOTH(XAR2)
      SP = SP - #LocalFrameSz          ; Reserve local space
      dbl(*SP(#IsrAddr)) = AC0         ; Store ISR function address
      *SP(#EventId) = T0               ; Store EventId
      AC0 = #0                         ; Clear XAR2/XAR3
      XAR2 = AC0                       ;
      XAR3 = AC0                       ;

      CALL _IRQ_globalDisable          ; Disable Interrupts
      *SP(#Old_INTM) = T0              ; Save OLD VALUE of INTM
      NOP
      NOP
      NOP
      NOP
      NOP
      XAR2 = #IVPD_ADDR                ; Get IVPD value
      NOP
      NOP
      NOP
      NOP
      NOP
      AC1 = *SP(#EventId)              ; EventId Gets placed in AC1

      AC3  = AC1 & #018h               ; Check for ISRs 16-23
      AC3 = AC3 - #16                  ; If ISR is 16-23 then use
      if (AC3 == 0) execute (AD_UNIT)  ; IVPH value for determining
      XAR2 = #IVPH_ADDR                ; vector address

      AC1 = AC1 << #3                  ; Multiply EventId by 8 to get Vector Offset
      AC0 = *AR2                       ; AC0 should now have IVPD value
      AC0 = AC0 <<  #4                 ; Shift up to get Vector Base Address
         AC0 = AC0 <<  #4                 ; Shift up to get Vector Base Address
      AC0 = AC0 | AC1                  ; AC0 now has Vector Address
      AC0 = AC0 << #-1                 ; Shift to get word address
      dbl(*SP(#VecLoc)) = AC0          ; Store Vector Address
      AC0 = dbl(*SP(#IsrAddr))         ; AC0 now has ISR address
      XAR3 = dbl(*SP(#VecLoc))         ; XAR3 has Vector Address
      AR1 = *SP(#EventId)              ; AR1 gets EventId
      AR2 = AR1 & #15                  ; Get only lower bits
      dbl(*AR3) = AC0                  ; Store ISR address at Vector Location
      AC3 = NOP16_Operator             ; Load NOP instruction into AC3
      AR3 += 2                         ; Set byte address to next location
      *AR3 = AC3                       ; Plug remaining part of vector with nop
      AR3 += 1                         ; Increment to next part of vector location
      *AR3 = AC3                       ; Plug remaining part of vector with nop
      T0 = AR2                         ; Use lower Bits as shift value
      AR1 = AR1 & #16                  ; Check to see if event is flagged in IFR1
      AC0 = #1                         ; Set AC0 = 1
      AC0 = AC0 <<< T0                 ; Shift AC0 by Bit value
      AC3 = AC0                        ; This is mask for IFR

      if (AR1 != #0) goto LIFR1        ; IF AR1 != 0, mask IFR1
      AR2 = #IFR0_ADDR                 ; Point AR2 to IFR0
      *AR2 = AC3                       ; Mask IFR0
      goto RESTORE_INTM
LIFR1:
      AR2 = #IFR1_ADDR                 ; Point AR2 to IFR1
      *AR2 = AC3                       ; Mask IFR1
RESTORE_INTM:
      T0 = *SP(#Old_INTM)              ; Set T0 to old INTM Value
      CALL _IRQ_globalRestore          ; Call IRQ function to restore old_intm
EPILOGUE:
      SP = SP + #LocalFrameSz          ; Restore Stack Pointer
      XAR2 = POPBOTH()                 ; POP XAR regs
      XAR3 = POPBOTH()
      RETURN
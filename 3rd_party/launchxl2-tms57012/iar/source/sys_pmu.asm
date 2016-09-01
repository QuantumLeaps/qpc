;-------------------------------------------------------------------------------
; sys_pmu.asm
;
; Copyright (C) 2009-2016 Texas Instruments Incorporated - www.ti.com
;
;
;  Redistribution and use in source and binary forms, with or without
;  modification, are permitted provided that the following conditions
;  are met:
;
;    Redistributions of source code must retain the above copyright
;    notice, this list of conditions and the following disclaimer.
;
;    Redistributions in binary form must reproduce the above copyright
;    notice, this list of conditions and the following disclaimer in the
;    documentation and/or other materials provided with the
;    distribution.
;
;    Neither the name of Texas Instruments Incorporated nor the names of
;    its contributors may be used to endorse or promote products derived
;    from this software without specific prior written permission.
;
;  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
;  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
;  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
;  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
;  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
;  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
;  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
;  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
;  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
;  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
;  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
;
;
;

    section .text:CODE
    arm


;-------------------------------------------------------------------------------
; Initialize Pmu
; Note: It will reset all counters
; SourceId : PMU_SourceId_001
; DesignId : PMU_DesignId_001
; Requirements : HL_SR484

    public     _pmuInit_


_pmuInit_

        ; set control register
        mrc   p15, #0, r0, c9, c12, #0
        orr   r0,  r0, #(1 << 4) + 6 + 1
        mcr   p15, #0, r0, c9, c12, #0
        ; clear flags
        mov   r0,  #0
        sub   r0,  r0,  #1
        mcr   p15, #0, r0, c9, c12, #3
        ; select counter 0 event
        mov   r0,  #0
        mcr   p15, #0, r0, c9, c12, #5 ; select counter
        mov   r0,  #0x11
        mcr   p15, #0, r0, c9, c13, #1 ; select event
        ; select counter 1 event
        mov   r0,  #1
        mcr   p15, #0, r0, c9, c12, #5 ; select counter
        mov   r0,  #0x11
        mcr   p15, #0, r0, c9, c13, #1 ; select event
        ; select counter 2 event
        mov   r0,  #2
        mcr   p15, #0, r0, c9, c12, #5 ; select counter
        mov   r0,  #0x11
        mcr   p15, #0, r0, c9, c13, #1 ; select event
        bx    lr




;-------------------------------------------------------------------------------
; Enable Counters Global [Cycle, Event [0..2]]
; Note: It will reset all counters
; SourceId : PMU_SourceId_002
; DesignId : PMU_DesignId_002
; Requirements : HL_SR485

    public     _pmuEnableCountersGlobal_


_pmuEnableCountersGlobal_

        mrc   p15, #0, r0, c9, c12, #0
        orr   r0,  r0, #7
        mcr   p15, #0, r0, c9, c12, #0
        bx    lr



;-------------------------------------------------------------------------------
; Disable Counters Global [Cycle, Event [0..2]]
; SourceId : PMU_SourceId_003
; DesignId : PMU_DesignId_003
; Requirements : HL_SR485

    public     _pmuDisableCountersGlobal_


_pmuDisableCountersGlobal_

        mrc   p15, #0, r0, c9, c12, #0
        bic   r0,  r0, #1
        mcr   p15, #0, r0, c9, c12, #0
        bx    lr



;-------------------------------------------------------------------------------
; Reset Cycle Counter
; SourceId : PMU_SourceId_004
; DesignId : PMU_DesignId_004
; Requirements : HL_SR485

    public     _pmuResetCycleCounter_


_pmuResetCycleCounter_

        mrc   p15, #0, r0, c9, c12, #0
        orr   r0,  r0, #4
        mcr   p15, #0, r0, c9, c12, #0
        bx    lr



;-------------------------------------------------------------------------------
; Reset Event Counters [0..2]
; SourceId : PMU_SourceId_005
; DesignId : PMU_DesignId_005
; Requirements : HL_SR485

    public     _pmuResetEventCounters_


_pmuResetEventCounters_

        mrc   p15, #0, r0, c9, c12, #0
        orr   r0,  r0, #2
        mcr   p15, #0, r0, c9, c12, #0
        bx    lr



;-------------------------------------------------------------------------------
; Reset Cycle Counter abd Event Counters [0..2]
; SourceId : PMU_SourceId_006
; DesignId : PMU_DesignId_006
; Requirements : HL_SR485

    public     _pmuResetCounters_


_pmuResetCounters_

        mrc   p15, #0, r0, c9, c12, #0
        orr   r0,  r0, #6
        mcr   p15, #0, r0, c9, c12, #0
        bx    lr



;-------------------------------------------------------------------------------
; Start Counters [Cycle, 0..2]
; SourceId : PMU_SourceId_007
; DesignId : PMU_DesignId_007
; Requirements : HL_SR485

    public     _pmuStartCounters_


_pmuStartCounters_

        mcr   p15, #0, r0, c9, c12, #1
        bx    lr



;-------------------------------------------------------------------------------
; Stop Counters [Cycle, 0..2]
; SourceId : PMU_SourceId_008
; DesignId : PMU_DesignId_008
; Requirements : HL_SR485

    public     _pmuStopCounters_


_pmuStopCounters_

        mcr   p15, #0, r0, c9, c12, #2
        bx    lr



;-------------------------------------------------------------------------------
; Set Count event
; SourceId : PMU_SourceId_009
; DesignId : PMU_DesignId_009
; Requirements : HL_SR485

    public     _pmuSetCountEvent_


_pmuSetCountEvent_

        mcr   p15, #0, r0, c9, c12, #5 ; select counter
        mcr   p15, #0, r1, c9, c13, #1 ; select event
        bx    lr



;-------------------------------------------------------------------------------
; Get Cycle Count
; SourceId : PMU_SourceId_010
; DesignId : PMU_DesignId_010
; Requirements : HL_SR486

    public     _pmuGetCycleCount_


_pmuGetCycleCount_

        mrc   p15, #0, r0, c9, c13, #0
        bx    lr



;-------------------------------------------------------------------------------
; Get Event Counter Count Value
; SourceId : PMU_SourceId_011
; DesignId : PMU_DesignId_011
; Requirements : HL_SR486

    public     _pmuGetEventCount_


_pmuGetEventCount_

        mcr   p15, #0, r0, c9, c12, #5 ; select counter
        mrc   p15, #0, r0, c9, c13, #2 ; read event counter
        bx    lr



;-------------------------------------------------------------------------------
; Get Overflow Flags
; SourceId : PMU_SourceId_012
; DesignId : PMU_DesignId_012
; Requirements : HL_SR486

    public     _pmuGetOverflow_


_pmuGetOverflow_

        mrc   p15, #0, r0, c9, c12, #3 ; read overflow
        mov   r1,  #0
        sub   r1,  r1,  #1
        mcr   p15, #0, r1, c9, c12, #3 ; clear flags
        bx    lr



    end

;-------------------------------------------------------------------------------


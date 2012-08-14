;********************************************************************************************************
;                                               uC/OS-II
;                                         The Real-Time Kernel
;
;                          (c) Copyright 1992-2002, Jean J. Labrosse, Weston, FL
;                                          All Rights Reserved
;
;
;                                       80x86/80x88 Specific code
;                                          LARGE MEMORY MODEL
;
;                                       Borland Turbo Assembler 3.1
;                                       (IBM/PC Compatible Target)
;
; File         : OS_CPU_A.ASM
; By           : Jean J. Labrosse
; Updated By   : Miro Samek
;********************************************************************************************************

;********************************************************************************************************
;                                    PUBLIC and EXTERNAL REFERENCES
;********************************************************************************************************

            PUBLIC _OSTickISR
            PUBLIC _OSStartHighRdy
            PUBLIC _OSCtxSw
            PUBLIC _OSIntCtxSw

            PUBLIC _OSCPUDisable
            PUBLIC _OSCPUEnable
            PUBLIC _OSCPUSaveSR
            PUBLIC _OSCPURestoreSR

            EXTRN  _OSIntExit:FAR
            EXTRN  _OSTimeTick:FAR
            EXTRN  _OSTaskSwHook:FAR

            EXTRN  _OSIntNesting:BYTE
            EXTRN  _OSTickDOSCtr:BYTE
            EXTRN  _OSPrioHighRdy:BYTE
            EXTRN  _OSPrioCur:BYTE
            EXTRN  _OSRunning:BYTE
            EXTRN  _OSTCBCur:DWORD
            EXTRN  _OSTCBHighRdy:DWORD

.MODEL      LARGE
.CODE
.186
            PAGE                                  ; /*$PAGE*/

;*********************************************************************************************************
;
;   void OSCPUDisable(void)
;
;*********************************************************************************************************

_OSCPUDisable PROC FAR
            CLI               ; clear the I flag
            RET               ; return to the caller
_OSCPUDisable ENDP

;*********************************************************************************************************
;
;   void OSCPUDisable(void)
;
;*********************************************************************************************************

_OSCPUEnable PROC FAR
            STI               ; set the I flag
            RET               ; return to the caller
_OSCPUEnable ENDP

;*********************************************************************************************************
;
;   int OSCPUSaveSR(void)
;
;*********************************************************************************************************

_OSCPUSaveSR PROC FAR
            PUSHF             ; push the flags
            POP AX            ; pop the flags into the return value AX
            CLI               ; clear the I flag
            RET               ; return to the caller
_OSCPUSaveSR ENDP

;*********************************************************************************************************
;
;    void OSCPURestoreSR(int key)
;
;*********************************************************************************************************

_OSCPURestoreSR PROC FAR
            PUSH BP
            MOV  BP,SP
            MOV  AX, WORD PTR[BP+6]
            PUSH AX
            POPF
            POP  BP
            RET
_OSCPURestoreSR  ENDP

;*********************************************************************************************************
;                                          START MULTITASKING
;                                       void OSStartHighRdy(void)
;
; The stack frame is assumed to look as follows:
;
; OSTCBHighRdy->OSTCBStkPtr --> DS                               (Low memory)
;                               ES
;                               DI
;                               SI
;                               BP
;                               SP
;                               BX
;                               DX
;                               CX
;                               AX
;                               OFFSET  of task code address
;                               SEGMENT of task code address
;                               Flags to load in PSW
;                               OFFSET  of task code address
;                               SEGMENT of task code address
;                               OFFSET  of 'pdata'
;                               SEGMENT of 'pdata'               (High memory)
;
; Note : OSStartHighRdy() MUST:
;           a) Call OSTaskSwHook() then,
;           b) Set OSRunning to TRUE,
;           c) Switch to the highest priority task.
;*********************************************************************************************************

_OSStartHighRdy  PROC FAR

            MOV    AX, SEG _OSTCBHighRdy          ; Reload DS
            MOV    DS, AX                         ;
;
            CALL   FAR PTR _OSTaskSwHook          ; Call user defined task switch hook
;
            MOV    AL, 1                          ; OSRunning = TRUE;
            MOV    BYTE PTR DS:_OSRunning, AL     ;   (Indicates that multitasking has started)
;
            LES    BX, DWORD PTR DS:_OSTCBHighRdy ; SS:SP = OSTCBHighRdy->OSTCBStkPtr
            MOV    SS, ES:[BX+2]                  ;
            MOV    SP, ES:[BX+0]                  ;
;
            POP    DS                             ; Load task's context
            POP    ES                             ;
            POPA                                  ;
;
            IRET                                  ; Run task

_OSStartHighRdy  ENDP

            PAGE                                  ; /*$PAGE*/
;*********************************************************************************************************
;                                PERFORM A CONTEXT SWITCH (From task level)
;                                           void OSCtxSw(void)
;
; Note(s): 1) Upon entry,
;             OSTCBCur     points to the OS_TCB of the task to suspend
;             OSTCBHighRdy points to the OS_TCB of the task to resume
;
;          2) The stack frame of the task to suspend looks as follows:
;
;                 SP -> OFFSET  of task to suspend    (Low memory)
;                       SEGMENT of task to suspend
;                       PSW     of task to suspend    (High memory)
;
;          3) The stack frame of the task to resume looks as follows:
;
;                 OSTCBHighRdy->OSTCBStkPtr --> DS                               (Low memory)
;                                               ES
;                                               DI
;                                               SI
;                                               BP
;                                               SP
;                                               BX
;                                               DX
;                                               CX
;                                               AX
;                                               OFFSET  of task code address
;                                               SEGMENT of task code address
;                                               Flags to load in PSW             (High memory)
;*********************************************************************************************************

_OSCtxSw    PROC   FAR
;
            PUSHA                                  ; Save current task's context
            PUSH   ES                              ;
            PUSH   DS                              ;
;
            MOV    AX, SEG _OSTCBCur               ; Reload DS in case it was altered
            MOV    DS, AX                          ;
;
            LES    BX, DWORD PTR DS:_OSTCBCur      ; OSTCBCur->OSTCBStkPtr = SS:SP
            MOV    ES:[BX+2], SS                   ;
            MOV    ES:[BX+0], SP                   ;
;
            CALL   FAR PTR _OSTaskSwHook           ; Call user defined task switch hook
;
            MOV    AX, WORD PTR DS:_OSTCBHighRdy+2 ; OSTCBCur = OSTCBHighRdy
            MOV    DX, WORD PTR DS:_OSTCBHighRdy   ;
            MOV    WORD PTR DS:_OSTCBCur+2, AX     ;
            MOV    WORD PTR DS:_OSTCBCur, DX       ;
;
            MOV    AL, BYTE PTR DS:_OSPrioHighRdy  ; OSPrioCur = OSPrioHighRdy
            MOV    BYTE PTR DS:_OSPrioCur, AL      ;
;
            LES    BX, DWORD PTR DS:_OSTCBHighRdy  ; SS:SP = OSTCBHighRdy->OSTCBStkPtr
            MOV    SS, ES:[BX+2]                   ;
            MOV    SP, ES:[BX]                     ;
;
            POP    DS                              ; Load new task's context
            POP    ES                              ;
            POPA                                   ;
;
            IRET                                   ; Return to new task
;
_OSCtxSw    ENDP

            PAGE                                   ; /*$PAGE*/
;*********************************************************************************************************
;                                PERFORM A CONTEXT SWITCH (From an ISR)
;                                        void OSIntCtxSw(void)
;
; Note(s): 1) Upon entry,
;             OSTCBCur     points to the OS_TCB of the task to suspend
;             OSTCBHighRdy points to the OS_TCB of the task to resume
;
;          2) The stack frame of the task to suspend looks as follows:
;
;             OSTCBCur->OSTCBStkPtr ------>  DS                              (Low memory)
;                                            ES
;                                            DI
;                                            SI
;                                            BP
;                                            SP
;                                            BX
;                                            DX
;                                            CX
;                                            AX
;                                            OFFSET  of task code address
;                                            SEGMENT of task code address
;                                            Flags to load in PSW            (High memory)
;
;
;          3) The stack frame of the task to resume looks as follows:
;
;             OSTCBHighRdy->OSTCBStkPtr --> DS                               (Low memory)
;                                           ES
;                                           DI
;                                           SI
;                                           BP
;                                           SP
;                                           BX
;                                           DX
;                                           CX
;                                           AX
;                                           OFFSET  of task code address
;                                           SEGMENT of task code address
;                                           Flags to load in PSW             (High memory)
;*********************************************************************************************************

_OSIntCtxSw PROC   FAR
;
            CALL   FAR PTR _OSTaskSwHook           ; Call user defined task switch hook
;
            MOV    AX, SEG _OSTCBCur               ; Reload DS in case it was altered
            MOV    DS, AX                          ;
;
            MOV    AX, WORD PTR DS:_OSTCBHighRdy+2 ; OSTCBCur = OSTCBHighRdy
            MOV    DX, WORD PTR DS:_OSTCBHighRdy   ;
            MOV    WORD PTR DS:_OSTCBCur+2, AX     ;
            MOV    WORD PTR DS:_OSTCBCur, DX       ;
;
            MOV    AL, BYTE PTR DS:_OSPrioHighRdy  ; OSPrioCur = OSPrioHighRdy
            MOV    BYTE PTR DS:_OSPrioCur, AL
;
            LES    BX, DWORD PTR DS:_OSTCBHighRdy  ; SS:SP = OSTCBHighRdy->OSTCBStkPtr
            MOV    SS, ES:[BX+2]                   ;
            MOV    SP, ES:[BX]                     ;
;
            POP    DS                              ; Load new task's context
            POP    ES                              ;
            POPA                                   ;
;
            IRET                                   ; Return to new task
;
_OSIntCtxSw ENDP

            PAGE                                   ; /*$PAGE*/
;*********************************************************************************************************
;                                            HANDLE TICK ISR
;
; Description: This function is called 199.99 times per second or, 11 times faster than the normal DOS
;              tick rate of 18.20648 Hz.  Thus every 11th time, the normal DOS tick handler is called.
;              This is called chaining.  10 times out of 11, however, the interrupt controller on the PC
;              must be cleared to allow for the next interrupt.
;
; Arguments  : none
;
; Returns    : none
;
; Note(s)    : The following C-like pseudo-code describe the operation being performed in the code below.
;
;              Save all registers on the current task's stack;
;              OSIntNesting++;
;              if (OSIntNesting == 1) {
;                 OSTCBCur->OSTCBStkPtr = SS:SP
;              }
;              OSTickDOSCtr--;
;              if (OSTickDOSCtr == 0) {
;                  OSTickDOSCtr = 11;
;                  INT 81H;               Chain into DOS every 54.925 mS
;                                         (Interrupt will be cleared by DOS)
;              } else {
;                  Send EOI to PIC;       Clear tick interrupt by sending an End-Of-Interrupt to the 8259
;                                         PIC (Priority Interrupt Controller)
;              }
;              OSTimeTick();              Notify uC/OS-II that a tick has occured
;              OSIntExit();               Notify uC/OS-II about end of ISR
;              Restore all registers that were save on the current task's stack;
;              Return from Interrupt;
;*********************************************************************************************************
;
_OSTickISR  PROC   FAR
;
            PUSHA                                ; Save interrupted task's context
            PUSH   ES
            PUSH   DS
;
            MOV    AX, SEG(_OSIntNesting)        ; Reload DS
            MOV    DS, AX
            INC    BYTE PTR DS:_OSIntNesting     ; Notify uC/OS-II of ISR
;
            CMP    BYTE PTR DS:_OSIntNesting, 1     ; if (OSIntNesting == 1)
            JNE    SHORT _OSTickISR1
            MOV    AX, SEG(_OSTCBCur)            ;     Reload DS
            MOV    DS, AX
            LES    BX, DWORD PTR DS:_OSTCBCur    ;     OSTCBCur->OSTCBStkPtr = SS:SP
            MOV    ES:[BX+2], SS                 ;
            MOV    ES:[BX+0], SP                 ;
;
_OSTickISR1:
            MOV    AX, SEG(_OSTickDOSCtr)        ; Reload DS
            MOV    DS, AX
            DEC    BYTE PTR DS:_OSTickDOSCtr
            CMP    BYTE PTR DS:_OSTickDOSCtr, 0
            JNE    SHORT _OSTickISR2             ; Every 11 ticks (~199.99 Hz), chain into DOS
;
            MOV    BYTE PTR DS:_OSTickDOSCtr, 11
            INT    081H                          ; Chain into DOS's tick ISR
            JMP    SHORT _OSTickISR3

_OSTickISR2:
            MOV    AL, 20H                       ; Move EOI code into AL.
            MOV    DX, 20H                       ; Address of 8259 PIC in DX.
            OUT    DX, AL                        ; Send EOI to PIC if not processing DOS timer.
;
_OSTickISR3:
            CALL   FAR PTR _OSTimeTick           ; Process system tick
;
            CALL   FAR PTR _OSIntExit            ; Notify uC/OS-II of end of ISR
;
            POP    DS                            ; Restore interrupted task's context
            POP    ES
            POPA
;
            IRET                                 ; Return to interrupted task
;
_OSTickISR  ENDP
;
;
            END

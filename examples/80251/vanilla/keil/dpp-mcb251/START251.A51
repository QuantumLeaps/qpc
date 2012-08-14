;------------------------------------------------------------------------------
;  This file is part of the C251 Compiler package
;  Copyright KEIL ELEKTRONIK GmbH 1995 - 2000
;------------------------------------------------------------------------------
;  START251.A51:  This code is executed after processor reset.
;
;  To translate this file use A251 with the following invocation:
;
;     A251 START251.A51  [ MODSRC ] [ SET (ROMHUGE) ]
;  whereby:
;     MODSRC         defines the 251 Source Mode  (default is Binary Mode)
;     SET (ROMHUGE)  defines the ROM (HUGE) Mode  (default is ROM(LARGE) )
;
;  To link the modified STARTUP.OBJ file to your application use the following
;  L251 invocation:
;
;     L251 START251.OBJ, <your object file list> <controls>
;
;------------------------------------------------------------------------------
;
;  251 Configuration Bytes Definition for off-chip (external) config bytes
;
$SET (CONFIGB = 0)   ; Set this variable if you want to set external config
;                    ; bytes at address FF:FFF8 and FF:FFF9.
;
; Wait State for PSEN#/RD#/WR# signal except region 01:xxxx (WSA1 & WSA0 Bits)
; WSA        Val  Description
; ---        ---  -----------
WSA  EQU 3  ; 3 = 0 wait state for all regions except region 01:xxxx
;           ; 2 = extended to 1 wait state  for all regions except 01:xxxx
;           ; 1 = extended to 2 wait states for all regions except 01:xxxx
;           ; 0 = extended to 3 wait states for all regions except 01:xxxx
;
; Extend ALE pulse
; XALE       Val  Description
; ----       ---  -----------
XALE EQU 1  ; 1 = ALE pulse is one TOSC
;           ; 0 = ALE pulse is three TOSC, this adds one external wait state
;
; RD# and PSEN# Function Select  (RD1 and RD0 Bits)
; RD         Val  RD Range   PSEN Range  P1.7 Func  Features
; --         ---  --------   ----------  ---------  --------
RDRG EQU 3  ; 3 = <=7F:FFFF  >=80:FFFF   P1.7/CEX4  Compatible with 8051
;           ; 2 = P3.7 only  All address P1.7/CEX4  One additional port pin
;           ; 1 = RD#=A16    All address P1.7/CEX4  128K External Address space
;           ; 0 = RD#=A16    All address P1.7=A17   256K External Address space
;
; Page Mode Select
; PAGE       Val  Description
; ----       ---  -----------
PAGM EQU 1  ; 1 = Non-page Mode (A15:8 on P2, A7:0/D7:0 on P0, 8051 compatible)
;           ; 0 = Page Mode (A15:8/D7:0 on P2, A7:0 on P0)
;
; Interrupt Mode Select
; INTR       Val  Description
; ----       ---  -----------
INTR EQU 1  ; 1 = Interrupt pushes 4 bytes onto the stack (PC & PSW1)
;           ; 0 = Interrupt pushes 2 bytes onto the stack (PCL & PCH only)
;
; Extended Data Float (EDF) Timing Feature
; EDF        Val  Description
; ----       ---  -----------
EDF  EQU 1  ; 1 = Standard (Compatibility) Mode
;           ; 0 = extend data float timing for slow memory devices
;
; Wait State for PSEN#/RD#/WR# signal for region 01:xxxx (WSB1 & WSB0 Bits)
; WSB        Val  Description
; ---        ---  -----------
WSB  EQU 3  ; 3 = 0 wait state for region 01:xxxx
;           ; 2 = extended to 1 wait state  for regions 01:xxxx
;           ; 1 = extended to 2 wait states for regions 01:xxxx
;           ; 0 = extended to 3 wait states for regions 01:xxxx
;
; EPROM/ROM Mapping
; WSA        Val  Description
; ---        ---  -----------
EMAP EQU 1 ;  1 = Map internal ROM only to region FF:xxxx
;          ;  0 = Map higher 8KB of internal ROM to region 00:E000 - 00:FFFF
;
;  Note:  the bit SRC is defined with the A251 directive MODSRC/MODBIN 
; 
;------------------------------------------------------------------------------
;
;  User-defined Power-On Zero Initialization of Memory
;
;  With the following EQU statements the zero initialization of memory
;  at processor reset can be defined:
;
;		; the absolute start-address of EDATA memory is always 0
EDATALEN	EQU	420H	; the length of EDATA memory in bytes.
;
XDATASTART	EQU	10000H	; the absolute start-address of XDATA memory
XDATALEN	EQU	0	; the length of XDATA memory in bytes.
;
HDATASTART	EQU	10000H	; the absolute start-address of HDATA memory
HDATALEN	EQU	0	; the length of HDATA memory in bytes.
;
;  Note:  The EDATA space overlaps physically the DATA, IDATA, BIT and EBIT
;         areas of the 251 CPU.
;
;------------------------------------------------------------------------------
;
;  CPU Stack Size Definition 
;
;  The following EQU statement defines the stack space available for the
;  251 application program.  It should be noted that the stack space must
;  be adjusted according the actual requirements of the application.
;
STACKSIZE	EQU	300	; adjust to your application
;
;------------------------------------------------------------------------------
;
;  Reentrant Stack Initilization 
;
;  Note:  the defintions below are only required when you application contains
;         reentrant code which is written with C251 Version 1 or C51.  You
;         should not enable IBPSTACK or XBPSTACK for reentrant code written
;         with C251 Version 2 since this compiler is using the hardware stack
;         of the 251 rather than a simulated stack area.
;
;  The following EQU statements define the stack pointer for reentrant
;  functions and initialized it:
;
;  Stack Space for reentrant functions in the SMALL model.
IBPSTACK	EQU	0	; set to 1 if small reentrant is used.
IBPSTACKTOP	EQU	0FFH+1	; set top of stack to highest location+1.
;
;  Stack Space for reentrant functions in the LARGE model.	
XBPSTACK	EQU	0	; set to 1 if large reentrant is used.
XBPSTACKTOP	EQU	0FFFFH+1; set top of stack to highest location+1.
;
;------------------------------------------------------------------------------

$IF ROMHUGE
Prefix	LIT '?'
Model   LIT 'FAR'
PRSeg	LIT 'ECODE'
$ELSE
Prefix  LIT ''
Model   LIT 'NEAR'
PRSeg	LIT 'CODE'
$ENDIF

$include (reg251s.inc)

EXTRN NUMBER (?C?XDATASEG)		; Start of XDATA Segment

		NAME	?C_START{Prefix}


; Setting of the Chip Configuration Bytes
$IF __MODSRC__
SRCM		EQU	1  ; Select Source Mode
$ELSE
SRCM		EQU	0  ; Select Binary Mode
$ENDIF

$IF (CONFIGB)
CONFIG0		EQU     (WSA*20H)+(XALE*10H)+(RDRG*4)+(PAGM*2)+SRCM+080H
CONFIG1		EQU	(INTR*10H)+(EDF*8)+(WSB*2)+EMAP+0E0H
	
		CSEG	AT	0FFF8H
		DB	CONFIG0		; Config Byte 0
		DB	CONFIG1		; Config Byte 1
$ENDIF


?C_C51STARTUP	SEGMENT   CODE
?C_C51STARTUP?3 SEGMENT   CODE

?STACK		SEGMENT   EDATA

		RSEG	?STACK
		DS	STACKSIZE	; Stack Space 100H Bytes

		EXTRN PRSeg (MAIN{Prefix})
		PUBLIC	?C_STARTUP{Prefix}
		PUBLIC	?C?STARTUP{Prefix}

		CSEG	AT	0
?C?STARTUP{Prefix}:
?C_STARTUP{Prefix}:
		LJMP	STARTUP1

		RSEG	?C_C51STARTUP

STARTUP1:
		MOV	DPXL,#?C?XDATASEG

IF EDATALEN <> 0
		MOV	WR8,#EDATALEN - 1
		CLR	A
EDATALOOP:	MOV	@WR8,R11
		DEC	WR8,#1
		JNE	EDATALOOP
ENDIF

IF XDATALEN <> 0
		MOV	DPTR,#WORD0 XDATASTART
		MOV	WR6,#XDATALEN
		CLR	A
XDATALOOP:	MOVX	@DPTR,A
		INC	DPTR
		DEC	WR6,#1
		JNE	XDATALOOP
ENDIF

IF HDATALEN <> 0
		MOV	DR16,#WORD0 HDATALEN
IF (WORD2 HDATALEN) <> 0
		MOV	WR16,#WORD2 HDATALEN
ENDIF
		MOV	WR12,#WORD2 HDATASTART
		MOV	WR14,#WORD0 HDATASTART
		CLR	A
HDATALOOP:	MOV	@DR12,R11
		INC	DR12,#1
		DEC	DR16,#1
		JNE	HDATALOOP
ENDIF

IF IBPSTACK <> 0
EXTRN DATA (?C_IBP)

		MOV	?C_IBP,#LOW IBPSTACKTOP
ENDIF

IF XBPSTACK <> 0
EXTRN DATA (?C_XBP)

		MOV	?C_XBP,#HIGH XBPSTACKTOP
		MOV	?C_XBP+1,#LOW XBPSTACKTOP
ENDIF

		MOV	DR60,#WORD0 (?STACK-1)

		RSEG	?C_C51STARTUP?3
		JMP	Model MAIN{Prefix}

		END

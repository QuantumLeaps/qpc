/******************** (C) COPYRIGHT 2006 STMicroelectronics ******************
* File Name          : memcopy.s
* Date First Issued  : 03/08/2006
* Author             : MCD Application Team, San Jose, CA
* Description        : This file contains optimized memcopy functions
******************************************************************************
* History:
*  03/08/2006        : V0.1 Initially created
*  03/09/2006        : V0.2 Additional functions
******************************************************************************
* THE PRESENT SOFTWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
* TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
* DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
* FROM THE CONTENT OF SUCH SOFTWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
* CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*****************************************************************************/

        MODULE    MEMCOPY
        SECTION MEMCOPY:CODE(2)
        CODE32

        EXPORT  MEMCOPY_L2S_BY4
        EXPORT  MEMCOPY_S2L_BY4
        EXPORT  MEMCOPY_L2L_BY4

/*****************************************************************************
* void  MEMCOPY_L2S_BY4 (void *pBUFOUT, void *pBUFIN, long nBytes)
*  Copy N bytes from a 32-bit aligned buffer to a 16-bit aligned buffer.
* Resources:
*  R0,R1,R2: parameters
*  R3-R10: locals
* Notes:
* - N Minimum is 16 bytes
* - pointer source and dest cannot be exactly the same; but an offset of -4
*   between dst and source will work
*****************************************************************************/

MEMCOPY_L2S_BY4
        STMFD       SP!, {R4-R10}

d_main
        LDMIA       R1!,{R3,R5,R7,R9}
        MOV         R4, R3, LSR #16
        MOV         R6, R5, LSR #16
        MOV         R8, R7, LSR #16
        MOV         R10, R9, LSR #16
        STRH        R3, [R0], #2
        STRH        R4, [R0], #2
        STRH        R5, [R0], #2
        STRH        R6, [R0], #2
        STRH        R7, [R0], #2
        STRH        R8, [R0], #2
        STRH        R9, [R0], #2
        STRH        R10, [R0], #2
        SUB         R2, R2, #16
        CMP         R2,#16        ; if ( remaining_bytes >= 16)
        BHS         d_main        ; keep on looping
                                  ; else switch(remaining_bytes) d_swit d_0123
        CMP         R2,#3         ; remaining <= 3 bytes?
        BLS         d_switch

d_4567
        LDR         R3, [R1], #4
        MOV         R4, R3, LSR #16
        STRH        R3, [R0], #2
        STRH        R4, [R0], #2
        SUB         R2, R2, #4
        CMP         R2,#3         ; remaining <= 3 bytes?
        BLS         d_switch      ;    switch(remaining_bytes 0,1,2,3)

d_891011
        LDR         R3, [R1], #4
        MOV         R4, R3, LSR #16
        STRH        R3, [R0], #2
        STRH        R4, [R0], #2
        SUB         R2, R2, #4
        CMP         R2,#3         ; remaining <= 3 bytes?
        BLS         d_switch      ;    switch(remaining_bytes 0,1,2,3)

d_12131415

        LDR         R3, [R1], #4
        MOV         R4, R3, LSR #16
        STRH        R3, [R0], #2
        STRH        R4, [R0], #2
        SUB         R2, R2, #4

d_switch
        LDR         PC,[PC, R2, LSL#2]
        B           d_END
        DC32        d_END
        DC32        d_1B
        DC32        d_2B
        DC32        d_3B

d_1B

        LDRB        R3, [R1]
        STRB        R3, [R0]
        B           d_END

d_2B

        LDRH        R3, [R1]
        STRH        R3, [R0]
        B           d_END

d_3B
        LDRH        R3, [R1], #2
        LDRB        R4, [R1]
        STRH        R3, [R0], #2
        STRB        R4, [R0]

d_END

        LDMFD       SP!, {R4-R10}
        BX          LR


/*****************************************************************************
* void  MEMCOPY_L2L_BY4 (void *pBUFOUT, void *pBUFIN, long nBytes)
*  Copy N bytes from a 32-bit aligned buffer to a 32-bit aligned buffer.
* Resources:
*  R0,R1,R2: parameters
*  R3-R10: locals
* Notes:
* - N Minimum is 16 bytes
* - pointer source and dest cannot be exactly the same; but an offset of -4
*   between dst and source will work
*****************************************************************************/

MEMCOPY_L2L_BY4
        STMFD       SP!, {R4-R10}

e_main
        LDMIA       R1!,{R3,R5,R7,R9}
        STMIA       R0!,{R3,R5,R7,R9}
        SUB         R2, R2, #16
        CMP         R2,#16        ; if ( remaining_bytes >= 16)
        BHS         e_main        ; keep on looping
                                  ; else switch(remaining_bytes)
e_swit
e_0123
        CMP         R2,#3         ; remaining <= 3 bytes?
        BLS         e_switch
e_4567

        LDR         R3, [R1], #4
        STR         R3, [R0], #4
        SUB         R2, R2, #4
        CMP         R2,#3         ; remaining <= 3 bytes?
        BLS         e_switch      ; switch(remaining_bytes 0,1,2,3)
e_891011
        LDR         R3, [R1], #4
        STR         R3, [R0], #4
        SUB         R2, R2, #4
        CMP         R2,#3         ; remaining <= 3 bytes?
        BLS         e_switch      ;    switch(remaining_bytes 0,1,2,3)
e_12131415
        LDR         R3, [R1], #4
        STR         R3, [R0], #4
        SUB         R2, R2, #4

e_switch
        LDR         PC,[PC, R2, LSL#2]
        B           e_END
        DCW         e_END
        DCW         e_1B
        DCW         e_2B
        DCW         e_3B

e_1B
        LDRB        R3, [R1]
        STRB        R3, [R0]
        B           e_END
e_2B
        LDRH        R3, [R1]
        STRH        R3, [R0]
        B           e_END
e_3B
        LDRH        R3, [R1], #2
        LDRB        R4, [R1]
        STRH        R3, [R0], #2
        STRB        R4, [R0]
        B           e_END
e_END
        LDMFD       SP!, {R4-R10}
        BX LR

/*****************************************************************************
* void  MEMCOPY_S2L_BY4 (void *pBUFOUT, void *pBUFIN, long nBytes)
*  Copy N bytes from a 32-bit aligned buffer to a 16-bit aligned buffer.
* Resources:
*  R0,R1,R2: parameters
*  R3-R10: locals
* Notes:
* - N Minimum is 16 bytes
* - pointer source and dest cannot be exactly the same; but an offset of -4
*   between dst and source will work
*****************************************************************************/
MEMCOPY_S2L_BY4
        STMFD       SP!, {R4-R10}

f_main
        LDRH        R3, [R1], #2
        LDRH        R4, [R1], #2
        LDRH        R5, [R1], #2
        LDRH        R6, [R1], #2
        LDRH        R7, [R1], #2
        LDRH        R8, [R1], #2
        LDRH        R9, [R1], #2
        LDRH        R10, [R1], #2
        ORR         R3,R3,R4,  LSL#16
        ORR         R5,R5,R6,  LSL#16
        ORR         R7,R7,R8,  LSL#16
        ORR         R9,R9,R10, LSL#16
        STMIA       R0!,{R3,R5,R7,R9}
        SUB         R2, R2, #16
        CMP         R2,#16        ; if ( remaining_bytes >= 16)
        BHS         f_main        ; keep on looping
                                  ; else switch(remaining_bytes)
f_swit
f_0123
        CMP         R2,#3         ; remaining <= 3 bytes?
        BLS         f_switch

f_4567
        LDRH        R3, [R1], #2
        LDRH        R4, [R1], #2
        ORR         R3,R3,R4, LSL#16
        STR         R3, [R0], #4
        SUB         R2, R2, #4
        CMP         R2,#3         ; remaining <= 3 bytes?
        BLS         f_switch      ;    switch(remaining_bytes 0,1,2,3)

f_891011
        LDRH        R3, [R1], #2
        LDRH        R4, [R1], #2
        ORR         R3,R3,R4, LSL#16
        STR         R3, [R0], #4
        SUB         R2, R2, #4
        CMP         R2,#3         ; remaining <= 3 bytes?
        BLS         f_switch      ;    switch(remaining_bytes 0,1,2,3)

f_12131415
        LDRH        R3, [R1], #2
        LDRH        R4, [R1], #2
        ORR         R3,R3,R4, LSL#16
        STR         R3, [R0], #4
        SUB         R2, R2, #4

f_switch
        LDR         PC,[PC, R2, LSL#2]
        B           f_END
        DCW         f_END
        DCW         f_1B
        DCW         f_2B
        DCW         f_3B

f_1B
        LDRB        R3, [R1]
        STRB        R3, [R0]
        B           f_END
f_2B
        LDRH        R3, [R1]
        STRH        R3, [R0]
        B           f_END
f_3B
        LDRH        R3, [R1], #2
        LDRB        R4, [R1]
        STRH        R3, [R0], #2
        STRB        R4, [R0]
        B           f_END
f_END
        LDMFD       SP!, {R4-R10}
        BX LR

        END




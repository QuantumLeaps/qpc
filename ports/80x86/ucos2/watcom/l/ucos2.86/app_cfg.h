/*
*********************************************************************************************************
*
*                                             EXAMPLE CODE
*
*                          (c) Copyright 2003-2006; Micrium, Inc.; Weston, FL
*
*               All rights reserved.  Protected by international copyright laws.
*
*               Knowledge of the source code may NOT be used to develop a similar product.
*
*               Please help us continue to provide the Embedded community with the finest
*               software available.  Your honesty is greatly appreciated.
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*
*                                       APPLICATION CONFIGURATION
*
*                                           Atmel AT91SAM7S
*                                                on the
*                                     AT91SAM7S-EK Evaluation Board
*
* Filename      : app_cfg.h
* Version       : V1.00
* Programmer(s) : Jean Labrosse
*********************************************************************************************************
*/

#ifndef  __APP_CFG_H__
#define  __APP_CFG_H__

/*
*********************************************************************************************************
*                                     ADDITIONAL uC/MODULE ENABLES
*********************************************************************************************************
*/

#define  OS_VIEW_MODULE                  DEF_ENABLED            /* DEF_ENABLED = Present, DEF_DISABLED = Not Present        */

/*
*********************************************************************************************************
*                                            TASK PRIORITIES
*********************************************************************************************************
*/

#define  APP_TASK_KBD_PRIO                     4
#define  APP_TASK_START_PRIO                   5

#define  OS_VIEW_TASK_PRIO                     6
#define  OS_VIEW_TASK_ID                       6

#define  OS_TASK_TMR_PRIO             (OS_LOWEST_PRIO - 2)

/*
*********************************************************************************************************
*                                            TASK STACK SIZES
*                            Size of the task stacks (# of OS_STK entries)
*********************************************************************************************************
*/

#define  APP_TASK_START_STK_SIZE             128
#define  APP_TASK_KBD_STK_SIZE               128

#define  OS_VIEW_TASK_STK_SIZE               128

/*
*********************************************************************************************************
*                                         uC/OS-View CONFIGURATION
*********************************************************************************************************
*/

#define  OS_VIEW_PARSE_TASK                     1                     /* Parsing of received packets will be done by a task   */
#define  OS_VIEW_TMR_32_BITS                    0                     /* uC/OS-View timer is 32 bits                          */

#define  OS_VIEW_UART_0                         0                     /* UART0 selected (DBG USART1 uses Parity only!         */
#define  OS_VIEW_TIMER_SEL                      2                     /* Choose an internal timer to use for OSView 0, 1, 2   */

#define  OS_VIEW_COMM_SEL                  OS_VIEW_UART_0             /* Select UART1 for uC/OS-View to use                   */

/*
*********************************************************************************************************
*                                      uC/OS-II DCC CONFIGURATION
*********************************************************************************************************
*/

#define  OS_CPU_ARM_DCC_EN                     0

/*
*********************************************************************************************************
*                                     TRACE / DEBUG CONFIGURATION
*********************************************************************************************************
*/

#define  TRACE_LEVEL_OFF                       0
#define  TRACE_LEVEL_INFO                      1
#define  TRACE_LEVEL_DEBUG                     2

#define  APP_TRACE_LEVEL                TRACE_LEVEL_DEBUG
#define  APP_TRACE

#define  APP_TRACE_INFO(x)            ((APP_TRACE_LEVEL >= TRACE_LEVEL_INFO)  ? (void)(APP_TRACE x) : (void)0)
#define  APP_TRACE_DEBUG(x)           ((APP_TRACE_LEVEL >= TRACE_LEVEL_DEBUG) ? (void)(APP_TRACE x) : (void)0)




#endif

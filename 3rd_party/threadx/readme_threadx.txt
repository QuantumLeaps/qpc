           Express Logic's ThreadX for STM3240G-EVAL (Cortex-M4 FPU) Evaluation Board

                                   Using the IAR Tools


                                  *** DEMO VERSION ***


0. About This Version

This version of ThreadX is for demonstration purposes only and may not
be used for any product development, either directly or indirectly. In 
addition, this demonstration may not be used for any competitive purpose.


1.  Installation

ThreadX for the Cortex-M4 is delivered on a single CD-ROM compatible disk. 
The entire distribution can be found in the sub-directory:

\threadx

To install ThreadX to your hard-disk, either run the supplied installer 
program Setup.exe or copy the distribution from the CD manually. 

To copy the ThreadX distribution manually, make a ThreadX directory on your 
hard-disk (we recommend c:\threadx\stm3240g-eval\iar) and copy all the contents 
of the ThreadX sub-directory on the distribution disk. The following 
is an example MS-DOS copy command from the distribution directory
(assuming source is d: and c: is your hard-drive):


d:\threadx> xcopy /S *.* c:\threadx\stm3240g-eval\iar


2.  Building the ThreadX run-time Library

This demonstration version contains a pre-built ThreadX library, tx.a. The library is 
intended for demonstration purposes only and thus has the following limitations:

                10 Threads
                9  Timers
                2  Event Flag Groups
                2  Mutexes
                3  Queues
                2  Semaphores
                1  Block Pool
                1  Byte Pool



3.  Demonstration System

The ThreadX demonstration is designed to execute under the IAR debugger on the
STM3240G-EVAL evaluation board (STM32F407 processor).

Building the demonstration is easy; simply open the threadx.www workspace file,
make the demo_threadx.ewp project the "active project" in the IAR Embedded 
Workbench, and select the "Make" button.

You should observe the compilation of demo_threadx.c (which is the demonstration 
application) and linking with tx.a. The resulting file demo_threadx.out is a 
binary ELF file that can be downloaded to flash and executed on the STM3240G-EVAL 
evaluation board connected via a USB cable to the IAR J-Link JTAG probe.


4.  System Initialization

The entry point in ThreadX for the Cortex-M4 using IAR tools is at label 
__iar_program_start. This is defined within the IAR compiler's startup code. 
In addition, this is where all static and global preset C variable 
initialization processing takes place.

The ThreadX tx_initialize_low_level.s file is responsible for setting up 
various system data structures, and a periodic timer interrupt source. 
By default, the vector area is defined at the top of startup_stm32f40x.s, 
which is a slightly modified from the base ST/IAR file. 

The _tx_initialize_low_level function inside of tx_initialize_low_level.s
also determines the first available address for use by the application, which 
is supplied as the sole input parameter to your application definition function, 
tx_application_define. To accomplish this, a section is created in 
tx_initialize_low_level.s called FREE_MEM, which must be located after all 
other RAM sections in memory.


5.  Register Usage and Stack Frames

The following defines the saved context stack frames for context switches
that occur as a result of interrupt handling or from thread-level API calls.
All suspended threads have the same stack frame in the Cortex-M4 version of
ThreadX. The top of the suspended thread's stack is pointed to by 
tx_thread_stack_ptr in the associated thread control block TX_THREAD.


Non-FPU Stack Frame:

  Stack Offset     Stack Contents 

     0x00               r4          
     0x04               r5          
     0x08               r6          
     0x0C               r7          
     0x10               r8          
     0x14               r9          
     0x18               r10 (sl)    
     0x1C               r11         
     0x20               r0          (Hardware stack starts here!!)
     0x24               r1          
     0x28               r2          
     0x2C               r3          
     0x30               r12         
     0x34               lr          
     0x38               pc          
     0x3C               xPSR        

FPU Stack Frame (only interrupted thread with FPU enabled):

  Stack Offset     Stack Contents 

     0x00               s0
     0x04               s1
     0x08               s2
     0x0C               s3
     0x10               s4
     0x14               s5
     0x18               s6
     0x1C               s7
     0x20               s8
     0x24               s9
     0x28               s10
     0x2C               s11
     0x30               s12
     0x34               s13
     0x38               s14
     0x3C               s15
     0x40               s16
     0x44               s17
     0x48               s18
     0x4C               s19
     0x50               s20
     0x54               s21
     0x58               s22
     0x5C               s23
     0x60               s24
     0x64               s25
     0x68               s26
     0x6C               s27
     0x70               s28
     0x74               s29
     0x78               s30
     0x7C               s31
     0x80               fpscr
     0x84               r4          
     0x88               r5          
     0x8C               r6          
     0x90               r7          
     0x94               r8          
     0x98               r9          
     0x9C               r10 (sl)    
     0xA0               r11         
     0xA4               r0          (Hardware stack starts here!!)
     0xA8               r1          
     0xAC               r2          
     0xB0               r3          
     0xB4               r12         
     0xB8               lr          
     0xBC               pc          
     0xC0               xPSR        


6.  Improving Performance

The distribution version of ThreadX is built without any compiler 
optimizations. This makes it easy to debug because you can trace or set 
breakpoints inside of ThreadX itself. Of course, this costs some 
performance. To make it run faster, you can change the ThreadX library
project to enable various compiler optimizations. 

In addition, you can eliminate the ThreadX basic API error checking by 
compiling your application code with the symbol TX_DISABLE_ERROR_CHECKING 
defined. 


7.  Interrupt Handling

ThreadX provides complete and high-performance interrupt handling for Cortex-M4
targets. There are a certain set of requirements that are defined in the 
following sub-sections:


7.1  Vector Area

The Cortex-M4 vectors start at the label __tx_vectors and is defined in cstartup_M.s. 
The application may modify the vector area according to its needs.


7.2 Managed Interrupts

A ThreadX managed interrupt is defined below. By following these conventions, the 
application ISR is then allowed access to various ThreadX services from the ISR.
Here is the standard template for managed ISRs in ThreadX:


        PUBLIC  __tx_IntHandler
__tx_IntHandler:
; VOID InterruptHandler (VOID)
; {
        PUSH    {lr}
        BL  _tx_thread_context_save
        
;    /* Do interrupt handler work here */
;    /* .... */

        B       _tx_thread_context_restore
; }


8.  IAR Thread-safe Library Support

Thread-safe support for the IAR tools is easily enabled by building the ThreadX library
and the application with TX_ENABLE_IAR_LIBRARY_SUPPORT. Also, the linker control file
should have the following line added (if not already in place):

initialize by copy with packing = none { section __DLIB_PERTHREAD }; // Required in a multi-threaded application


9. FPU Support

By default, FPU support is disabled for each thread. If saving the context of the FPU registers
is needed, the following API call must be made from the context of the application thread - before 
the FPU usage:

void    tx_thread_fpu_enable(void);

After this API is called in the application, FPU registers will be saved/restored for this thread if it
is preempted via an interrupt. All other suspension of the this thread will not require the FPU registers
to be saved/restored.

To disable FPU register context saving, simply call the following API:

void    tx_thread_fpu_disable(void);


9.  Revision History

01/24/2012  Initial ThreadX demo version for ST's STM3240G-EVAL board using IAR's ARM tools.


Copyright(c) 1996-2012 Express Logic, Inc.


Express Logic, Inc.
11423 West Bernardo Court
San Diego, CA  92127

www.expresslogic.com


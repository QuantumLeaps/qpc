/***********************************************************************
* This file has been modified for the DPP application by Quantum Leaps
* from the generated file 'hardware_setup.c'. To avoid confustion, this
* file has been renamed to 'startup_qp.c'
*
*  1. added definition of the istack array to allocate RAM for the stack.
*  2. added definition of the __main() function used in C++.
*
*  DATE: Mar 06,2012
***********************************************************************/

#include "iodefine.h"

/* define the space for the stack (adjust to your application!!!) */
int istack[1024] __attribute__ ((section (".istack")));


#ifdef CPPAPP
//Initialize global constructors
// The GNURX uses a different method to initialize global constructors
// The below call is not invoked in case of RX target
extern "C" void __main()
{
  static int initialized;
  if (! initialized)
    {
      typedef void (*pfunc) ();
      extern pfunc __ctors[];
      extern pfunc __ctors_end[];
      pfunc *p;

      initialized = 1;
      for (p = __ctors_end; p > __ctors; )
    (*--p) ();

    }
}
#endif

#ifdef __cplusplus
extern "C" {
#endif
extern void HardwareSetup(void);
#ifdef __cplusplus
}
#endif

void HardwareSetup(void)
{
/*
  SCI.SMR.BYTE = 0;
  SCI.SMR.BIT.CA   = 1;
  SCI.SMR.BIT.CHR  = 1;
  SCI.SMR.BIT.OE   = 1;
  SCI.SMR.BIT.STOP = 1;
  SCI.SMR.BIT.MP   = 1;
  SCI.SMR.BIT.CKS  = 3;
  SCI.BRR = 0;
  SCI.SCR.BYTE = 0;
  SCI.SCR.BIT.TIE  = 1;
  SCI.SCR.BIT.RIE  = 1;
  SCI.SCR.BIT.TE   = 1;
  SCI.SCR.BIT.RE   = 1;
  SCI.SCR.BIT.MPIE = 1;
  SCI.SCR.BIT.TEIE = 1;
  SCI.SCR.BIT.CKE  = 3;
  SCI.TDR = 0;
  SCI.SSR.BYTE = 0;
  SCI.SSR.BIT.TDRE = 1;
  SCI.SSR.BIT.RDRF = 1;
  SCI.SSR.BIT.ORER = 1;
  SCI.SSR.BIT.FER  = 1;
  SCI.SSR.BIT.PER  = 1;
  SCI.SSR.BIT.TEND = 1;
  SCI.SSR.BIT.MPB  = 1;
  SCI.SSR.BIT.MPBT = 1;
  SCI.RDR = 0;

*/
}

#ifndef _vxworks_windDefs_h
#define _vxworks_windDefs_h

#define STATUS int
#define BOOL   int

#define OK    0
#define ERROR (-1)

typedef void (*FUNCPTR)(int a0, int a1, int a2, int a3,
                        int a4, int a5, int a6, int a7, int a8, int a9);

typedef FUNCPTR VOIDFUNCPTR;

#define NO_WAIT         0
#define WAIT_FOREVER    (-1)

#endif    /* !_vxworks_windDefs_h */


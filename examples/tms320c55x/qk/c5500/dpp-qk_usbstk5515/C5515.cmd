-stack    1000       /* PRIMARY STACK SIZE    */
-sysstack 1000       /* SECONDARY STACK SIZE  */
-heap     0          /* HEAP AREA SIZE        */

MEMORY {
    MMR      (RW) : origin = 0000000h length = 0000c0h      /* MMRs  */
    DARAM    (RW) : origin = 00000c0h length = 00FF40h      /* on-chip DARAM */
    SARAM    (RW) : origin = 0010000h length = 03E000h      /* on-chip SARAM */

    EMIF_CS0 (RW) : origin = 0050000h  length = 07B0000h    /* mSDR          */
    EMIF_CS2 (RW) : origin = 0800000h  length = 0400000h    /* ASYNC1 : NAND */
    EMIF_CS3 (RW) : origin = 0C00000h  length = 0200000h    /* ASYNC2 : NAND */
    EMIF_CS4 (RW) : origin = 0E00000h  length = 0100000h    /* ASYNC3 : NOR  */
    EMIF_CS5 (RW) : origin = 0F00000h  length = 00E0000h    /* ASYNC4 : SRAM */
}

SECTIONS {
    vectors (NOLOAD)
    vector      : > DARAM  ALIGN = 256
    .bss        : > SARAM
    .stack      : > SARAM
    .sysstack   : > SARAM
    .text       : > SARAM  ALIGN = 4
    .cinit      : > SARAM
    .const      : {} > SARAM PAGE 0
    .data       : {} > SARAM PAGE 0
    .cio        : {} > SARAM
    .pinit      : {} > SARAM

    .emif_cs0   : > EMIF_CS0
    .emif_cs2   : > EMIF_CS2
    .emif_cs3   : > EMIF_CS3
    .emif_cs4   : > EMIF_CS4
    .emif_cs5   : > EMIF_CS5
}



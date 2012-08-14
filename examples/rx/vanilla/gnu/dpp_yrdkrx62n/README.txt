Notes for managing HEW projects for Renesas RX with GNU-RX (KPIT)
=================================================================

The GNU-RX project generator in HEW seems to have several problems and
therefore starting new projects by generating them is NOT recommended.

The problems are incorrect compiler and linker options and incorrect
stack section layout to name a few.

Instead of the new project generator, the following workaround procedure
can be used:

1. Copy the entire project directory (e.g., dpp_yrdkrx62n) to 
   a location for the new project.

2. Rename the copied directory <old-project> to the new project
   (<new-project>).

3. Inside the <new-project> directory, delete all the <old-project>.*
   files except <old-project>.hws and <old-project>.hwp files.

4. Rename <old-project>.hws and <old-project>.hwp files to
   <new-project>.hws and <new-project>.hwp, respectively.

5. Open <new-project>.hws in a text editor and automatically replace
   all <old-project> strings with <new-project>. (You should see some
   eight replacements.)

6. Open <new-project>.hwp in a text editor and automatically replace
   all <old-project> strings with <new-project>. (You should see thirty-
   something replacements.)


Now you can open the <new-project>.hws workspace in HEW, add/delete the
files inside the project, and build/debug the project.


Standard files in the project
-----------------------------

The following files are intended for reuse in most QP projects
based on GNU-RX:

1. reset_qp.asm    - generic for a given RX-type (no changes necessary)
2. startup_qp.c    - generic for a given RX-type (no changes necessary) 
3. vector_table.c  - generic for a given RX-type (no changes necessary) 
4. iodefine.h      - generic for a given RX-type (no changes necessary)
5. interrupt_handlers.h - generic for a given RX-type (no changes necessary)
6. interrupt_handlers.c - *** application specific ***
7. bsp.h                - *** application specific ***
8. bsp.c                - *** application specific ***


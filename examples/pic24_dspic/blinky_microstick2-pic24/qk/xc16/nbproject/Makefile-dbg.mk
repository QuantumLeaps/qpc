#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Include project Makefile
ifeq "${IGNORE_LOCAL}" "TRUE"
# do not include local makefile. User is passing all local related variables already
else
include Makefile
# Include makefile containing local settings
ifeq "$(wildcard nbproject/Makefile-local-dbg.mk)" "nbproject/Makefile-local-dbg.mk"
include nbproject/Makefile-local-dbg.mk
endif
endif

# Environment
MKDIR=gnumkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=dbg
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=elf
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/xc16.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/xc16.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

ifeq ($(COMPARE_BUILD), true)
COMPARISON_BUILD=-mafrlcsj
else
COMPARISON_BUILD=
endif

ifdef SUB_IMAGE_ADDRESS
SUB_IMAGE_ADDRESS_COMMAND=--image-address $(SUB_IMAGE_ADDRESS)
else
SUB_IMAGE_ADDRESS_COMMAND=
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=../../../../../src/qf/qep_hsm.c ../../../../../src/qf/qep_msm.c ../../../../../src/qf/qf_act.c ../../../../../src/qf/qf_actq.c ../../../../../src/qf/qf_defer.c ../../../../../src/qf/qf_dyn.c ../../../../../src/qf/qf_mem.c ../../../../../src/qf/qf_ps.c ../../../../../src/qf/qf_qact.c ../../../../../src/qf/qf_qeq.c ../../../../../src/qf/qf_qmact.c ../../../../../src/qf/qf_time.c ../../../../../src/qk/qk.c ../../../../../ports/pic24_dspic/qk/xc16/qk_port.c bsp.c ../../../../../include/qstamp.c ../../main.c ../../blinky.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/_ext/820665871/qep_hsm.o ${OBJECTDIR}/_ext/820665871/qep_msm.o ${OBJECTDIR}/_ext/820665871/qf_act.o ${OBJECTDIR}/_ext/820665871/qf_actq.o ${OBJECTDIR}/_ext/820665871/qf_defer.o ${OBJECTDIR}/_ext/820665871/qf_dyn.o ${OBJECTDIR}/_ext/820665871/qf_mem.o ${OBJECTDIR}/_ext/820665871/qf_ps.o ${OBJECTDIR}/_ext/820665871/qf_qact.o ${OBJECTDIR}/_ext/820665871/qf_qeq.o ${OBJECTDIR}/_ext/820665871/qf_qmact.o ${OBJECTDIR}/_ext/820665871/qf_time.o ${OBJECTDIR}/_ext/820665876/qk.o ${OBJECTDIR}/_ext/1875001806/qk_port.o ${OBJECTDIR}/bsp.o ${OBJECTDIR}/_ext/726959463/qstamp.o ${OBJECTDIR}/_ext/43898991/main.o ${OBJECTDIR}/_ext/43898991/blinky.o
POSSIBLE_DEPFILES=${OBJECTDIR}/_ext/820665871/qep_hsm.o.d ${OBJECTDIR}/_ext/820665871/qep_msm.o.d ${OBJECTDIR}/_ext/820665871/qf_act.o.d ${OBJECTDIR}/_ext/820665871/qf_actq.o.d ${OBJECTDIR}/_ext/820665871/qf_defer.o.d ${OBJECTDIR}/_ext/820665871/qf_dyn.o.d ${OBJECTDIR}/_ext/820665871/qf_mem.o.d ${OBJECTDIR}/_ext/820665871/qf_ps.o.d ${OBJECTDIR}/_ext/820665871/qf_qact.o.d ${OBJECTDIR}/_ext/820665871/qf_qeq.o.d ${OBJECTDIR}/_ext/820665871/qf_qmact.o.d ${OBJECTDIR}/_ext/820665871/qf_time.o.d ${OBJECTDIR}/_ext/820665876/qk.o.d ${OBJECTDIR}/_ext/1875001806/qk_port.o.d ${OBJECTDIR}/bsp.o.d ${OBJECTDIR}/_ext/726959463/qstamp.o.d ${OBJECTDIR}/_ext/43898991/main.o.d ${OBJECTDIR}/_ext/43898991/blinky.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/_ext/820665871/qep_hsm.o ${OBJECTDIR}/_ext/820665871/qep_msm.o ${OBJECTDIR}/_ext/820665871/qf_act.o ${OBJECTDIR}/_ext/820665871/qf_actq.o ${OBJECTDIR}/_ext/820665871/qf_defer.o ${OBJECTDIR}/_ext/820665871/qf_dyn.o ${OBJECTDIR}/_ext/820665871/qf_mem.o ${OBJECTDIR}/_ext/820665871/qf_ps.o ${OBJECTDIR}/_ext/820665871/qf_qact.o ${OBJECTDIR}/_ext/820665871/qf_qeq.o ${OBJECTDIR}/_ext/820665871/qf_qmact.o ${OBJECTDIR}/_ext/820665871/qf_time.o ${OBJECTDIR}/_ext/820665876/qk.o ${OBJECTDIR}/_ext/1875001806/qk_port.o ${OBJECTDIR}/bsp.o ${OBJECTDIR}/_ext/726959463/qstamp.o ${OBJECTDIR}/_ext/43898991/main.o ${OBJECTDIR}/_ext/43898991/blinky.o

# Source Files
SOURCEFILES=../../../../../src/qf/qep_hsm.c ../../../../../src/qf/qep_msm.c ../../../../../src/qf/qf_act.c ../../../../../src/qf/qf_actq.c ../../../../../src/qf/qf_defer.c ../../../../../src/qf/qf_dyn.c ../../../../../src/qf/qf_mem.c ../../../../../src/qf/qf_ps.c ../../../../../src/qf/qf_qact.c ../../../../../src/qf/qf_qeq.c ../../../../../src/qf/qf_qmact.c ../../../../../src/qf/qf_time.c ../../../../../src/qk/qk.c ../../../../../ports/pic24_dspic/qk/xc16/qk_port.c bsp.c ../../../../../include/qstamp.c ../../main.c ../../blinky.c


CFLAGS=
ASFLAGS=
LDLIBSOPTIONS=

############# Tool locations ##########################################
# If you copy a project from one host to another, the path where the  #
# compiler is installed may be different.                             #
# If you open this project with MPLAB X in the new host, this         #
# makefile will be regenerated and the paths will be corrected.       #
#######################################################################
# fixDeps replaces a bunch of sed/cat/printf statements that slow down the build
FIXDEPS=fixDeps

.build-conf:  ${BUILD_SUBPROJECTS}
ifneq ($(INFORMATION_MESSAGE), )
	@echo $(INFORMATION_MESSAGE)
endif
	${MAKE}  -f nbproject/Makefile-dbg.mk dist/${CND_CONF}/${IMAGE_TYPE}/xc16.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=24FJ64GB002
MP_LINKER_FILE_OPTION=,--script=p24FJ64GB002.gld
# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/_ext/820665871/qep_hsm.o: ../../../../../src/qf/qep_hsm.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/820665871" 
	@${RM} ${OBJECTDIR}/_ext/820665871/qep_hsm.o.d 
	@${RM} ${OBJECTDIR}/_ext/820665871/qep_hsm.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../../../../src/qf/qep_hsm.c  -o ${OBJECTDIR}/_ext/820665871/qep_hsm.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/820665871/qep_hsm.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1    -omf=elf -DXPRJ_dbg=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -O0 -I"../.." -I"../../../../../include" -I"../../../../../src" -I"../../../../../ports/pic24_dspic/qk/xc16" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/_ext/820665871/qep_hsm.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/820665871/qep_msm.o: ../../../../../src/qf/qep_msm.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/820665871" 
	@${RM} ${OBJECTDIR}/_ext/820665871/qep_msm.o.d 
	@${RM} ${OBJECTDIR}/_ext/820665871/qep_msm.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../../../../src/qf/qep_msm.c  -o ${OBJECTDIR}/_ext/820665871/qep_msm.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/820665871/qep_msm.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1    -omf=elf -DXPRJ_dbg=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -O0 -I"../.." -I"../../../../../include" -I"../../../../../src" -I"../../../../../ports/pic24_dspic/qk/xc16" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/_ext/820665871/qep_msm.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/820665871/qf_act.o: ../../../../../src/qf/qf_act.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/820665871" 
	@${RM} ${OBJECTDIR}/_ext/820665871/qf_act.o.d 
	@${RM} ${OBJECTDIR}/_ext/820665871/qf_act.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../../../../src/qf/qf_act.c  -o ${OBJECTDIR}/_ext/820665871/qf_act.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/820665871/qf_act.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1    -omf=elf -DXPRJ_dbg=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -O0 -I"../.." -I"../../../../../include" -I"../../../../../src" -I"../../../../../ports/pic24_dspic/qk/xc16" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/_ext/820665871/qf_act.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/820665871/qf_actq.o: ../../../../../src/qf/qf_actq.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/820665871" 
	@${RM} ${OBJECTDIR}/_ext/820665871/qf_actq.o.d 
	@${RM} ${OBJECTDIR}/_ext/820665871/qf_actq.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../../../../src/qf/qf_actq.c  -o ${OBJECTDIR}/_ext/820665871/qf_actq.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/820665871/qf_actq.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1    -omf=elf -DXPRJ_dbg=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -O0 -I"../.." -I"../../../../../include" -I"../../../../../src" -I"../../../../../ports/pic24_dspic/qk/xc16" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/_ext/820665871/qf_actq.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/820665871/qf_defer.o: ../../../../../src/qf/qf_defer.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/820665871" 
	@${RM} ${OBJECTDIR}/_ext/820665871/qf_defer.o.d 
	@${RM} ${OBJECTDIR}/_ext/820665871/qf_defer.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../../../../src/qf/qf_defer.c  -o ${OBJECTDIR}/_ext/820665871/qf_defer.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/820665871/qf_defer.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1    -omf=elf -DXPRJ_dbg=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -O0 -I"../.." -I"../../../../../include" -I"../../../../../src" -I"../../../../../ports/pic24_dspic/qk/xc16" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/_ext/820665871/qf_defer.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/820665871/qf_dyn.o: ../../../../../src/qf/qf_dyn.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/820665871" 
	@${RM} ${OBJECTDIR}/_ext/820665871/qf_dyn.o.d 
	@${RM} ${OBJECTDIR}/_ext/820665871/qf_dyn.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../../../../src/qf/qf_dyn.c  -o ${OBJECTDIR}/_ext/820665871/qf_dyn.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/820665871/qf_dyn.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1    -omf=elf -DXPRJ_dbg=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -O0 -I"../.." -I"../../../../../include" -I"../../../../../src" -I"../../../../../ports/pic24_dspic/qk/xc16" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/_ext/820665871/qf_dyn.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/820665871/qf_mem.o: ../../../../../src/qf/qf_mem.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/820665871" 
	@${RM} ${OBJECTDIR}/_ext/820665871/qf_mem.o.d 
	@${RM} ${OBJECTDIR}/_ext/820665871/qf_mem.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../../../../src/qf/qf_mem.c  -o ${OBJECTDIR}/_ext/820665871/qf_mem.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/820665871/qf_mem.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1    -omf=elf -DXPRJ_dbg=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -O0 -I"../.." -I"../../../../../include" -I"../../../../../src" -I"../../../../../ports/pic24_dspic/qk/xc16" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/_ext/820665871/qf_mem.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/820665871/qf_ps.o: ../../../../../src/qf/qf_ps.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/820665871" 
	@${RM} ${OBJECTDIR}/_ext/820665871/qf_ps.o.d 
	@${RM} ${OBJECTDIR}/_ext/820665871/qf_ps.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../../../../src/qf/qf_ps.c  -o ${OBJECTDIR}/_ext/820665871/qf_ps.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/820665871/qf_ps.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1    -omf=elf -DXPRJ_dbg=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -O0 -I"../.." -I"../../../../../include" -I"../../../../../src" -I"../../../../../ports/pic24_dspic/qk/xc16" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/_ext/820665871/qf_ps.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/820665871/qf_qact.o: ../../../../../src/qf/qf_qact.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/820665871" 
	@${RM} ${OBJECTDIR}/_ext/820665871/qf_qact.o.d 
	@${RM} ${OBJECTDIR}/_ext/820665871/qf_qact.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../../../../src/qf/qf_qact.c  -o ${OBJECTDIR}/_ext/820665871/qf_qact.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/820665871/qf_qact.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1    -omf=elf -DXPRJ_dbg=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -O0 -I"../.." -I"../../../../../include" -I"../../../../../src" -I"../../../../../ports/pic24_dspic/qk/xc16" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/_ext/820665871/qf_qact.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/820665871/qf_qeq.o: ../../../../../src/qf/qf_qeq.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/820665871" 
	@${RM} ${OBJECTDIR}/_ext/820665871/qf_qeq.o.d 
	@${RM} ${OBJECTDIR}/_ext/820665871/qf_qeq.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../../../../src/qf/qf_qeq.c  -o ${OBJECTDIR}/_ext/820665871/qf_qeq.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/820665871/qf_qeq.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1    -omf=elf -DXPRJ_dbg=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -O0 -I"../.." -I"../../../../../include" -I"../../../../../src" -I"../../../../../ports/pic24_dspic/qk/xc16" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/_ext/820665871/qf_qeq.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/820665871/qf_qmact.o: ../../../../../src/qf/qf_qmact.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/820665871" 
	@${RM} ${OBJECTDIR}/_ext/820665871/qf_qmact.o.d 
	@${RM} ${OBJECTDIR}/_ext/820665871/qf_qmact.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../../../../src/qf/qf_qmact.c  -o ${OBJECTDIR}/_ext/820665871/qf_qmact.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/820665871/qf_qmact.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1    -omf=elf -DXPRJ_dbg=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -O0 -I"../.." -I"../../../../../include" -I"../../../../../src" -I"../../../../../ports/pic24_dspic/qk/xc16" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/_ext/820665871/qf_qmact.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/820665871/qf_time.o: ../../../../../src/qf/qf_time.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/820665871" 
	@${RM} ${OBJECTDIR}/_ext/820665871/qf_time.o.d 
	@${RM} ${OBJECTDIR}/_ext/820665871/qf_time.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../../../../src/qf/qf_time.c  -o ${OBJECTDIR}/_ext/820665871/qf_time.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/820665871/qf_time.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1    -omf=elf -DXPRJ_dbg=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -O0 -I"../.." -I"../../../../../include" -I"../../../../../src" -I"../../../../../ports/pic24_dspic/qk/xc16" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/_ext/820665871/qf_time.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/820665876/qk.o: ../../../../../src/qk/qk.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/820665876" 
	@${RM} ${OBJECTDIR}/_ext/820665876/qk.o.d 
	@${RM} ${OBJECTDIR}/_ext/820665876/qk.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../../../../src/qk/qk.c  -o ${OBJECTDIR}/_ext/820665876/qk.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/820665876/qk.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1    -omf=elf -DXPRJ_dbg=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -O0 -I"../.." -I"../../../../../include" -I"../../../../../src" -I"../../../../../ports/pic24_dspic/qk/xc16" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/_ext/820665876/qk.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1875001806/qk_port.o: ../../../../../ports/pic24_dspic/qk/xc16/qk_port.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1875001806" 
	@${RM} ${OBJECTDIR}/_ext/1875001806/qk_port.o.d 
	@${RM} ${OBJECTDIR}/_ext/1875001806/qk_port.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../../../../ports/pic24_dspic/qk/xc16/qk_port.c  -o ${OBJECTDIR}/_ext/1875001806/qk_port.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1875001806/qk_port.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1    -omf=elf -DXPRJ_dbg=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -O0 -I"../.." -I"../../../../../include" -I"../../../../../src" -I"../../../../../ports/pic24_dspic/qk/xc16" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/_ext/1875001806/qk_port.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/bsp.o: bsp.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/bsp.o.d 
	@${RM} ${OBJECTDIR}/bsp.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  bsp.c  -o ${OBJECTDIR}/bsp.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/bsp.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1    -omf=elf -DXPRJ_dbg=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -O0 -I"../.." -I"../../../../../include" -I"../../../../../src" -I"../../../../../ports/pic24_dspic/qk/xc16" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/bsp.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/726959463/qstamp.o: ../../../../../include/qstamp.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/726959463" 
	@${RM} ${OBJECTDIR}/_ext/726959463/qstamp.o.d 
	@${RM} ${OBJECTDIR}/_ext/726959463/qstamp.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../../../../include/qstamp.c  -o ${OBJECTDIR}/_ext/726959463/qstamp.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/726959463/qstamp.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1    -omf=elf -DXPRJ_dbg=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -O0 -I"../.." -I"../../../../../include" -I"../../../../../src" -I"../../../../../ports/pic24_dspic/qk/xc16" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/_ext/726959463/qstamp.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/43898991/main.o: ../../main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/43898991" 
	@${RM} ${OBJECTDIR}/_ext/43898991/main.o.d 
	@${RM} ${OBJECTDIR}/_ext/43898991/main.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../main.c  -o ${OBJECTDIR}/_ext/43898991/main.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/43898991/main.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1    -omf=elf -DXPRJ_dbg=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -O0 -I"../.." -I"../../../../../include" -I"../../../../../src" -I"../../../../../ports/pic24_dspic/qk/xc16" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/_ext/43898991/main.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/43898991/blinky.o: ../../blinky.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/43898991" 
	@${RM} ${OBJECTDIR}/_ext/43898991/blinky.o.d 
	@${RM} ${OBJECTDIR}/_ext/43898991/blinky.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../blinky.c  -o ${OBJECTDIR}/_ext/43898991/blinky.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/43898991/blinky.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1    -omf=elf -DXPRJ_dbg=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -O0 -I"../.." -I"../../../../../include" -I"../../../../../src" -I"../../../../../ports/pic24_dspic/qk/xc16" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/_ext/43898991/blinky.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
else
${OBJECTDIR}/_ext/820665871/qep_hsm.o: ../../../../../src/qf/qep_hsm.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/820665871" 
	@${RM} ${OBJECTDIR}/_ext/820665871/qep_hsm.o.d 
	@${RM} ${OBJECTDIR}/_ext/820665871/qep_hsm.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../../../../src/qf/qep_hsm.c  -o ${OBJECTDIR}/_ext/820665871/qep_hsm.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/820665871/qep_hsm.o.d"        -g -omf=elf -DXPRJ_dbg=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -O0 -I"../.." -I"../../../../../include" -I"../../../../../src" -I"../../../../../ports/pic24_dspic/qk/xc16" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/_ext/820665871/qep_hsm.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/820665871/qep_msm.o: ../../../../../src/qf/qep_msm.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/820665871" 
	@${RM} ${OBJECTDIR}/_ext/820665871/qep_msm.o.d 
	@${RM} ${OBJECTDIR}/_ext/820665871/qep_msm.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../../../../src/qf/qep_msm.c  -o ${OBJECTDIR}/_ext/820665871/qep_msm.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/820665871/qep_msm.o.d"        -g -omf=elf -DXPRJ_dbg=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -O0 -I"../.." -I"../../../../../include" -I"../../../../../src" -I"../../../../../ports/pic24_dspic/qk/xc16" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/_ext/820665871/qep_msm.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/820665871/qf_act.o: ../../../../../src/qf/qf_act.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/820665871" 
	@${RM} ${OBJECTDIR}/_ext/820665871/qf_act.o.d 
	@${RM} ${OBJECTDIR}/_ext/820665871/qf_act.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../../../../src/qf/qf_act.c  -o ${OBJECTDIR}/_ext/820665871/qf_act.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/820665871/qf_act.o.d"        -g -omf=elf -DXPRJ_dbg=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -O0 -I"../.." -I"../../../../../include" -I"../../../../../src" -I"../../../../../ports/pic24_dspic/qk/xc16" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/_ext/820665871/qf_act.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/820665871/qf_actq.o: ../../../../../src/qf/qf_actq.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/820665871" 
	@${RM} ${OBJECTDIR}/_ext/820665871/qf_actq.o.d 
	@${RM} ${OBJECTDIR}/_ext/820665871/qf_actq.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../../../../src/qf/qf_actq.c  -o ${OBJECTDIR}/_ext/820665871/qf_actq.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/820665871/qf_actq.o.d"        -g -omf=elf -DXPRJ_dbg=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -O0 -I"../.." -I"../../../../../include" -I"../../../../../src" -I"../../../../../ports/pic24_dspic/qk/xc16" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/_ext/820665871/qf_actq.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/820665871/qf_defer.o: ../../../../../src/qf/qf_defer.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/820665871" 
	@${RM} ${OBJECTDIR}/_ext/820665871/qf_defer.o.d 
	@${RM} ${OBJECTDIR}/_ext/820665871/qf_defer.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../../../../src/qf/qf_defer.c  -o ${OBJECTDIR}/_ext/820665871/qf_defer.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/820665871/qf_defer.o.d"        -g -omf=elf -DXPRJ_dbg=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -O0 -I"../.." -I"../../../../../include" -I"../../../../../src" -I"../../../../../ports/pic24_dspic/qk/xc16" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/_ext/820665871/qf_defer.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/820665871/qf_dyn.o: ../../../../../src/qf/qf_dyn.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/820665871" 
	@${RM} ${OBJECTDIR}/_ext/820665871/qf_dyn.o.d 
	@${RM} ${OBJECTDIR}/_ext/820665871/qf_dyn.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../../../../src/qf/qf_dyn.c  -o ${OBJECTDIR}/_ext/820665871/qf_dyn.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/820665871/qf_dyn.o.d"        -g -omf=elf -DXPRJ_dbg=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -O0 -I"../.." -I"../../../../../include" -I"../../../../../src" -I"../../../../../ports/pic24_dspic/qk/xc16" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/_ext/820665871/qf_dyn.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/820665871/qf_mem.o: ../../../../../src/qf/qf_mem.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/820665871" 
	@${RM} ${OBJECTDIR}/_ext/820665871/qf_mem.o.d 
	@${RM} ${OBJECTDIR}/_ext/820665871/qf_mem.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../../../../src/qf/qf_mem.c  -o ${OBJECTDIR}/_ext/820665871/qf_mem.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/820665871/qf_mem.o.d"        -g -omf=elf -DXPRJ_dbg=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -O0 -I"../.." -I"../../../../../include" -I"../../../../../src" -I"../../../../../ports/pic24_dspic/qk/xc16" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/_ext/820665871/qf_mem.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/820665871/qf_ps.o: ../../../../../src/qf/qf_ps.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/820665871" 
	@${RM} ${OBJECTDIR}/_ext/820665871/qf_ps.o.d 
	@${RM} ${OBJECTDIR}/_ext/820665871/qf_ps.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../../../../src/qf/qf_ps.c  -o ${OBJECTDIR}/_ext/820665871/qf_ps.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/820665871/qf_ps.o.d"        -g -omf=elf -DXPRJ_dbg=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -O0 -I"../.." -I"../../../../../include" -I"../../../../../src" -I"../../../../../ports/pic24_dspic/qk/xc16" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/_ext/820665871/qf_ps.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/820665871/qf_qact.o: ../../../../../src/qf/qf_qact.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/820665871" 
	@${RM} ${OBJECTDIR}/_ext/820665871/qf_qact.o.d 
	@${RM} ${OBJECTDIR}/_ext/820665871/qf_qact.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../../../../src/qf/qf_qact.c  -o ${OBJECTDIR}/_ext/820665871/qf_qact.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/820665871/qf_qact.o.d"        -g -omf=elf -DXPRJ_dbg=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -O0 -I"../.." -I"../../../../../include" -I"../../../../../src" -I"../../../../../ports/pic24_dspic/qk/xc16" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/_ext/820665871/qf_qact.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/820665871/qf_qeq.o: ../../../../../src/qf/qf_qeq.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/820665871" 
	@${RM} ${OBJECTDIR}/_ext/820665871/qf_qeq.o.d 
	@${RM} ${OBJECTDIR}/_ext/820665871/qf_qeq.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../../../../src/qf/qf_qeq.c  -o ${OBJECTDIR}/_ext/820665871/qf_qeq.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/820665871/qf_qeq.o.d"        -g -omf=elf -DXPRJ_dbg=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -O0 -I"../.." -I"../../../../../include" -I"../../../../../src" -I"../../../../../ports/pic24_dspic/qk/xc16" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/_ext/820665871/qf_qeq.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/820665871/qf_qmact.o: ../../../../../src/qf/qf_qmact.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/820665871" 
	@${RM} ${OBJECTDIR}/_ext/820665871/qf_qmact.o.d 
	@${RM} ${OBJECTDIR}/_ext/820665871/qf_qmact.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../../../../src/qf/qf_qmact.c  -o ${OBJECTDIR}/_ext/820665871/qf_qmact.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/820665871/qf_qmact.o.d"        -g -omf=elf -DXPRJ_dbg=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -O0 -I"../.." -I"../../../../../include" -I"../../../../../src" -I"../../../../../ports/pic24_dspic/qk/xc16" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/_ext/820665871/qf_qmact.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/820665871/qf_time.o: ../../../../../src/qf/qf_time.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/820665871" 
	@${RM} ${OBJECTDIR}/_ext/820665871/qf_time.o.d 
	@${RM} ${OBJECTDIR}/_ext/820665871/qf_time.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../../../../src/qf/qf_time.c  -o ${OBJECTDIR}/_ext/820665871/qf_time.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/820665871/qf_time.o.d"        -g -omf=elf -DXPRJ_dbg=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -O0 -I"../.." -I"../../../../../include" -I"../../../../../src" -I"../../../../../ports/pic24_dspic/qk/xc16" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/_ext/820665871/qf_time.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/820665876/qk.o: ../../../../../src/qk/qk.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/820665876" 
	@${RM} ${OBJECTDIR}/_ext/820665876/qk.o.d 
	@${RM} ${OBJECTDIR}/_ext/820665876/qk.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../../../../src/qk/qk.c  -o ${OBJECTDIR}/_ext/820665876/qk.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/820665876/qk.o.d"        -g -omf=elf -DXPRJ_dbg=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -O0 -I"../.." -I"../../../../../include" -I"../../../../../src" -I"../../../../../ports/pic24_dspic/qk/xc16" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/_ext/820665876/qk.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1875001806/qk_port.o: ../../../../../ports/pic24_dspic/qk/xc16/qk_port.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1875001806" 
	@${RM} ${OBJECTDIR}/_ext/1875001806/qk_port.o.d 
	@${RM} ${OBJECTDIR}/_ext/1875001806/qk_port.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../../../../ports/pic24_dspic/qk/xc16/qk_port.c  -o ${OBJECTDIR}/_ext/1875001806/qk_port.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1875001806/qk_port.o.d"        -g -omf=elf -DXPRJ_dbg=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -O0 -I"../.." -I"../../../../../include" -I"../../../../../src" -I"../../../../../ports/pic24_dspic/qk/xc16" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/_ext/1875001806/qk_port.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/bsp.o: bsp.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/bsp.o.d 
	@${RM} ${OBJECTDIR}/bsp.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  bsp.c  -o ${OBJECTDIR}/bsp.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/bsp.o.d"        -g -omf=elf -DXPRJ_dbg=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -O0 -I"../.." -I"../../../../../include" -I"../../../../../src" -I"../../../../../ports/pic24_dspic/qk/xc16" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/bsp.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/726959463/qstamp.o: ../../../../../include/qstamp.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/726959463" 
	@${RM} ${OBJECTDIR}/_ext/726959463/qstamp.o.d 
	@${RM} ${OBJECTDIR}/_ext/726959463/qstamp.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../../../../include/qstamp.c  -o ${OBJECTDIR}/_ext/726959463/qstamp.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/726959463/qstamp.o.d"        -g -omf=elf -DXPRJ_dbg=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -O0 -I"../.." -I"../../../../../include" -I"../../../../../src" -I"../../../../../ports/pic24_dspic/qk/xc16" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/_ext/726959463/qstamp.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/43898991/main.o: ../../main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/43898991" 
	@${RM} ${OBJECTDIR}/_ext/43898991/main.o.d 
	@${RM} ${OBJECTDIR}/_ext/43898991/main.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../main.c  -o ${OBJECTDIR}/_ext/43898991/main.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/43898991/main.o.d"        -g -omf=elf -DXPRJ_dbg=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -O0 -I"../.." -I"../../../../../include" -I"../../../../../src" -I"../../../../../ports/pic24_dspic/qk/xc16" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/_ext/43898991/main.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/43898991/blinky.o: ../../blinky.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/43898991" 
	@${RM} ${OBJECTDIR}/_ext/43898991/blinky.o.d 
	@${RM} ${OBJECTDIR}/_ext/43898991/blinky.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../blinky.c  -o ${OBJECTDIR}/_ext/43898991/blinky.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/43898991/blinky.o.d"        -g -omf=elf -DXPRJ_dbg=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -O0 -I"../.." -I"../../../../../include" -I"../../../../../src" -I"../../../../../ports/pic24_dspic/qk/xc16" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/_ext/43898991/blinky.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assemblePreproc
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
dist/${CND_CONF}/${IMAGE_TYPE}/xc16.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -o dist/${CND_CONF}/${IMAGE_TYPE}/xc16.${IMAGE_TYPE}.${OUTPUT_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}      -mcpu=$(MP_PROCESSOR_OPTION)        -D__DEBUG=__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -omf=elf -DXPRJ_dbg=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)   -mreserve=data@0x800:0x81F -mreserve=data@0x820:0x821 -mreserve=data@0x822:0x823 -mreserve=data@0x824:0x825 -mreserve=data@0x826:0x84F   -Wl,,,--defsym=__MPLAB_BUILD=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,-D__DEBUG=__DEBUG,--defsym=__MPLAB_DEBUGGER_PK3=1,$(MP_LINKER_FILE_OPTION),--stack=16,--check-sections,--data-init,--pack-data,--handles,--isr,--no-gc-sections,--fill-upper=0,--stackguard=16,--no-force-link,--smart-io,--report-mem,--memorysummary,dist/${CND_CONF}/${IMAGE_TYPE}/memoryfile.xml$(MP_EXTRA_LD_POST) 
	
else
dist/${CND_CONF}/${IMAGE_TYPE}/xc16.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -o dist/${CND_CONF}/${IMAGE_TYPE}/xc16.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}      -mcpu=$(MP_PROCESSOR_OPTION)        -omf=elf -DXPRJ_dbg=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -Wl,,,--defsym=__MPLAB_BUILD=1,$(MP_LINKER_FILE_OPTION),--stack=16,--check-sections,--data-init,--pack-data,--handles,--isr,--no-gc-sections,--fill-upper=0,--stackguard=16,--no-force-link,--smart-io,--report-mem,--memorysummary,dist/${CND_CONF}/${IMAGE_TYPE}/memoryfile.xml$(MP_EXTRA_LD_POST) 
	${MP_CC_DIR}\\xc16-bin2hex dist/${CND_CONF}/${IMAGE_TYPE}/xc16.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} -a  -omf=elf  
	
endif


# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/dbg
	${RM} -r dist/dbg

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(shell mplabwildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif

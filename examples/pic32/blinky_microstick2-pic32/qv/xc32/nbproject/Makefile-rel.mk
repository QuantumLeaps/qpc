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
ifeq "$(wildcard nbproject/Makefile-local-rel.mk)" "nbproject/Makefile-local-rel.mk"
include nbproject/Makefile-local-rel.mk
endif
endif

# Environment
MKDIR=gnumkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=rel
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=elf
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/xc32.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/xc32.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

ifeq ($(COMPARE_BUILD), true)
COMPARISON_BUILD=-mafrlcsj
else
COMPARISON_BUILD=
endif

ifdef SUB_IMAGE_ADDRESS

else
SUB_IMAGE_ADDRESS_COMMAND=
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=../../../../../src/qf/qep_hsm.c ../../../../../src/qf/qep_msm.c ../../../../../src/qf/qf_act.c ../../../../../src/qf/qf_actq.c ../../../../../src/qf/qf_defer.c ../../../../../src/qf/qf_dyn.c ../../../../../src/qf/qf_mem.c ../../../../../src/qf/qf_ps.c ../../../../../src/qf/qf_qact.c ../../../../../src/qf/qf_qeq.c ../../../../../src/qf/qf_qmact.c ../../../../../src/qf/qf_time.c ../../../../../src/qv/qv.c bsp.c ../../blinky.c ../../main.c ../../../../../include/qstamp.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/_ext/820665871/qep_hsm.o ${OBJECTDIR}/_ext/820665871/qep_msm.o ${OBJECTDIR}/_ext/820665871/qf_act.o ${OBJECTDIR}/_ext/820665871/qf_actq.o ${OBJECTDIR}/_ext/820665871/qf_defer.o ${OBJECTDIR}/_ext/820665871/qf_dyn.o ${OBJECTDIR}/_ext/820665871/qf_mem.o ${OBJECTDIR}/_ext/820665871/qf_ps.o ${OBJECTDIR}/_ext/820665871/qf_qact.o ${OBJECTDIR}/_ext/820665871/qf_qeq.o ${OBJECTDIR}/_ext/820665871/qf_qmact.o ${OBJECTDIR}/_ext/820665871/qf_time.o ${OBJECTDIR}/_ext/820665887/qv.o ${OBJECTDIR}/bsp.o ${OBJECTDIR}/_ext/43898991/blinky.o ${OBJECTDIR}/_ext/43898991/main.o ${OBJECTDIR}/_ext/726959463/qstamp.o
POSSIBLE_DEPFILES=${OBJECTDIR}/_ext/820665871/qep_hsm.o.d ${OBJECTDIR}/_ext/820665871/qep_msm.o.d ${OBJECTDIR}/_ext/820665871/qf_act.o.d ${OBJECTDIR}/_ext/820665871/qf_actq.o.d ${OBJECTDIR}/_ext/820665871/qf_defer.o.d ${OBJECTDIR}/_ext/820665871/qf_dyn.o.d ${OBJECTDIR}/_ext/820665871/qf_mem.o.d ${OBJECTDIR}/_ext/820665871/qf_ps.o.d ${OBJECTDIR}/_ext/820665871/qf_qact.o.d ${OBJECTDIR}/_ext/820665871/qf_qeq.o.d ${OBJECTDIR}/_ext/820665871/qf_qmact.o.d ${OBJECTDIR}/_ext/820665871/qf_time.o.d ${OBJECTDIR}/_ext/820665887/qv.o.d ${OBJECTDIR}/bsp.o.d ${OBJECTDIR}/_ext/43898991/blinky.o.d ${OBJECTDIR}/_ext/43898991/main.o.d ${OBJECTDIR}/_ext/726959463/qstamp.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/_ext/820665871/qep_hsm.o ${OBJECTDIR}/_ext/820665871/qep_msm.o ${OBJECTDIR}/_ext/820665871/qf_act.o ${OBJECTDIR}/_ext/820665871/qf_actq.o ${OBJECTDIR}/_ext/820665871/qf_defer.o ${OBJECTDIR}/_ext/820665871/qf_dyn.o ${OBJECTDIR}/_ext/820665871/qf_mem.o ${OBJECTDIR}/_ext/820665871/qf_ps.o ${OBJECTDIR}/_ext/820665871/qf_qact.o ${OBJECTDIR}/_ext/820665871/qf_qeq.o ${OBJECTDIR}/_ext/820665871/qf_qmact.o ${OBJECTDIR}/_ext/820665871/qf_time.o ${OBJECTDIR}/_ext/820665887/qv.o ${OBJECTDIR}/bsp.o ${OBJECTDIR}/_ext/43898991/blinky.o ${OBJECTDIR}/_ext/43898991/main.o ${OBJECTDIR}/_ext/726959463/qstamp.o

# Source Files
SOURCEFILES=../../../../../src/qf/qep_hsm.c ../../../../../src/qf/qep_msm.c ../../../../../src/qf/qf_act.c ../../../../../src/qf/qf_actq.c ../../../../../src/qf/qf_defer.c ../../../../../src/qf/qf_dyn.c ../../../../../src/qf/qf_mem.c ../../../../../src/qf/qf_ps.c ../../../../../src/qf/qf_qact.c ../../../../../src/qf/qf_qeq.c ../../../../../src/qf/qf_qmact.c ../../../../../src/qf/qf_time.c ../../../../../src/qv/qv.c bsp.c ../../blinky.c ../../main.c ../../../../../include/qstamp.c


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
	${MAKE}  -f nbproject/Makefile-rel.mk dist/${CND_CONF}/${IMAGE_TYPE}/xc32.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=32MX250F128B
MP_LINKER_FILE_OPTION=
# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assembleWithPreprocess
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/_ext/820665871/qep_hsm.o: ../../../../../src/qf/qep_hsm.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/820665871" 
	@${RM} ${OBJECTDIR}/_ext/820665871/qep_hsm.o.d 
	@${RM} ${OBJECTDIR}/_ext/820665871/qep_hsm.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/820665871/qep_hsm.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O3 -DNDEBUG -I"../.." -I"../../../../../include" -I"../../../../../src" -I"../../../../../ports/pic32/qv/xc32" -MMD -MF "${OBJECTDIR}/_ext/820665871/qep_hsm.o.d" -o ${OBJECTDIR}/_ext/820665871/qep_hsm.o ../../../../../src/qf/qep_hsm.c    -DXPRJ_rel=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/820665871/qep_msm.o: ../../../../../src/qf/qep_msm.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/820665871" 
	@${RM} ${OBJECTDIR}/_ext/820665871/qep_msm.o.d 
	@${RM} ${OBJECTDIR}/_ext/820665871/qep_msm.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/820665871/qep_msm.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O3 -DNDEBUG -I"../.." -I"../../../../../include" -I"../../../../../src" -I"../../../../../ports/pic32/qv/xc32" -MMD -MF "${OBJECTDIR}/_ext/820665871/qep_msm.o.d" -o ${OBJECTDIR}/_ext/820665871/qep_msm.o ../../../../../src/qf/qep_msm.c    -DXPRJ_rel=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/820665871/qf_act.o: ../../../../../src/qf/qf_act.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/820665871" 
	@${RM} ${OBJECTDIR}/_ext/820665871/qf_act.o.d 
	@${RM} ${OBJECTDIR}/_ext/820665871/qf_act.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/820665871/qf_act.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O3 -DNDEBUG -I"../.." -I"../../../../../include" -I"../../../../../src" -I"../../../../../ports/pic32/qv/xc32" -MMD -MF "${OBJECTDIR}/_ext/820665871/qf_act.o.d" -o ${OBJECTDIR}/_ext/820665871/qf_act.o ../../../../../src/qf/qf_act.c    -DXPRJ_rel=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/820665871/qf_actq.o: ../../../../../src/qf/qf_actq.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/820665871" 
	@${RM} ${OBJECTDIR}/_ext/820665871/qf_actq.o.d 
	@${RM} ${OBJECTDIR}/_ext/820665871/qf_actq.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/820665871/qf_actq.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O3 -DNDEBUG -I"../.." -I"../../../../../include" -I"../../../../../src" -I"../../../../../ports/pic32/qv/xc32" -MMD -MF "${OBJECTDIR}/_ext/820665871/qf_actq.o.d" -o ${OBJECTDIR}/_ext/820665871/qf_actq.o ../../../../../src/qf/qf_actq.c    -DXPRJ_rel=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/820665871/qf_defer.o: ../../../../../src/qf/qf_defer.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/820665871" 
	@${RM} ${OBJECTDIR}/_ext/820665871/qf_defer.o.d 
	@${RM} ${OBJECTDIR}/_ext/820665871/qf_defer.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/820665871/qf_defer.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O3 -DNDEBUG -I"../.." -I"../../../../../include" -I"../../../../../src" -I"../../../../../ports/pic32/qv/xc32" -MMD -MF "${OBJECTDIR}/_ext/820665871/qf_defer.o.d" -o ${OBJECTDIR}/_ext/820665871/qf_defer.o ../../../../../src/qf/qf_defer.c    -DXPRJ_rel=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/820665871/qf_dyn.o: ../../../../../src/qf/qf_dyn.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/820665871" 
	@${RM} ${OBJECTDIR}/_ext/820665871/qf_dyn.o.d 
	@${RM} ${OBJECTDIR}/_ext/820665871/qf_dyn.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/820665871/qf_dyn.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O3 -DNDEBUG -I"../.." -I"../../../../../include" -I"../../../../../src" -I"../../../../../ports/pic32/qv/xc32" -MMD -MF "${OBJECTDIR}/_ext/820665871/qf_dyn.o.d" -o ${OBJECTDIR}/_ext/820665871/qf_dyn.o ../../../../../src/qf/qf_dyn.c    -DXPRJ_rel=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/820665871/qf_mem.o: ../../../../../src/qf/qf_mem.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/820665871" 
	@${RM} ${OBJECTDIR}/_ext/820665871/qf_mem.o.d 
	@${RM} ${OBJECTDIR}/_ext/820665871/qf_mem.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/820665871/qf_mem.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O3 -DNDEBUG -I"../.." -I"../../../../../include" -I"../../../../../src" -I"../../../../../ports/pic32/qv/xc32" -MMD -MF "${OBJECTDIR}/_ext/820665871/qf_mem.o.d" -o ${OBJECTDIR}/_ext/820665871/qf_mem.o ../../../../../src/qf/qf_mem.c    -DXPRJ_rel=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/820665871/qf_ps.o: ../../../../../src/qf/qf_ps.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/820665871" 
	@${RM} ${OBJECTDIR}/_ext/820665871/qf_ps.o.d 
	@${RM} ${OBJECTDIR}/_ext/820665871/qf_ps.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/820665871/qf_ps.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O3 -DNDEBUG -I"../.." -I"../../../../../include" -I"../../../../../src" -I"../../../../../ports/pic32/qv/xc32" -MMD -MF "${OBJECTDIR}/_ext/820665871/qf_ps.o.d" -o ${OBJECTDIR}/_ext/820665871/qf_ps.o ../../../../../src/qf/qf_ps.c    -DXPRJ_rel=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/820665871/qf_qact.o: ../../../../../src/qf/qf_qact.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/820665871" 
	@${RM} ${OBJECTDIR}/_ext/820665871/qf_qact.o.d 
	@${RM} ${OBJECTDIR}/_ext/820665871/qf_qact.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/820665871/qf_qact.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O3 -DNDEBUG -I"../.." -I"../../../../../include" -I"../../../../../src" -I"../../../../../ports/pic32/qv/xc32" -MMD -MF "${OBJECTDIR}/_ext/820665871/qf_qact.o.d" -o ${OBJECTDIR}/_ext/820665871/qf_qact.o ../../../../../src/qf/qf_qact.c    -DXPRJ_rel=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/820665871/qf_qeq.o: ../../../../../src/qf/qf_qeq.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/820665871" 
	@${RM} ${OBJECTDIR}/_ext/820665871/qf_qeq.o.d 
	@${RM} ${OBJECTDIR}/_ext/820665871/qf_qeq.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/820665871/qf_qeq.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O3 -DNDEBUG -I"../.." -I"../../../../../include" -I"../../../../../src" -I"../../../../../ports/pic32/qv/xc32" -MMD -MF "${OBJECTDIR}/_ext/820665871/qf_qeq.o.d" -o ${OBJECTDIR}/_ext/820665871/qf_qeq.o ../../../../../src/qf/qf_qeq.c    -DXPRJ_rel=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/820665871/qf_qmact.o: ../../../../../src/qf/qf_qmact.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/820665871" 
	@${RM} ${OBJECTDIR}/_ext/820665871/qf_qmact.o.d 
	@${RM} ${OBJECTDIR}/_ext/820665871/qf_qmact.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/820665871/qf_qmact.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O3 -DNDEBUG -I"../.." -I"../../../../../include" -I"../../../../../src" -I"../../../../../ports/pic32/qv/xc32" -MMD -MF "${OBJECTDIR}/_ext/820665871/qf_qmact.o.d" -o ${OBJECTDIR}/_ext/820665871/qf_qmact.o ../../../../../src/qf/qf_qmact.c    -DXPRJ_rel=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/820665871/qf_time.o: ../../../../../src/qf/qf_time.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/820665871" 
	@${RM} ${OBJECTDIR}/_ext/820665871/qf_time.o.d 
	@${RM} ${OBJECTDIR}/_ext/820665871/qf_time.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/820665871/qf_time.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O3 -DNDEBUG -I"../.." -I"../../../../../include" -I"../../../../../src" -I"../../../../../ports/pic32/qv/xc32" -MMD -MF "${OBJECTDIR}/_ext/820665871/qf_time.o.d" -o ${OBJECTDIR}/_ext/820665871/qf_time.o ../../../../../src/qf/qf_time.c    -DXPRJ_rel=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/820665887/qv.o: ../../../../../src/qv/qv.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/820665887" 
	@${RM} ${OBJECTDIR}/_ext/820665887/qv.o.d 
	@${RM} ${OBJECTDIR}/_ext/820665887/qv.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/820665887/qv.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O3 -DNDEBUG -I"../.." -I"../../../../../include" -I"../../../../../src" -I"../../../../../ports/pic32/qv/xc32" -MMD -MF "${OBJECTDIR}/_ext/820665887/qv.o.d" -o ${OBJECTDIR}/_ext/820665887/qv.o ../../../../../src/qv/qv.c    -DXPRJ_rel=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/bsp.o: bsp.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/bsp.o.d 
	@${RM} ${OBJECTDIR}/bsp.o 
	@${FIXDEPS} "${OBJECTDIR}/bsp.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O3 -DNDEBUG -I"../.." -I"../../../../../include" -I"../../../../../src" -I"../../../../../ports/pic32/qv/xc32" -MMD -MF "${OBJECTDIR}/bsp.o.d" -o ${OBJECTDIR}/bsp.o bsp.c    -DXPRJ_rel=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/43898991/blinky.o: ../../blinky.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/43898991" 
	@${RM} ${OBJECTDIR}/_ext/43898991/blinky.o.d 
	@${RM} ${OBJECTDIR}/_ext/43898991/blinky.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/43898991/blinky.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O3 -DNDEBUG -I"../.." -I"../../../../../include" -I"../../../../../src" -I"../../../../../ports/pic32/qv/xc32" -MMD -MF "${OBJECTDIR}/_ext/43898991/blinky.o.d" -o ${OBJECTDIR}/_ext/43898991/blinky.o ../../blinky.c    -DXPRJ_rel=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/43898991/main.o: ../../main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/43898991" 
	@${RM} ${OBJECTDIR}/_ext/43898991/main.o.d 
	@${RM} ${OBJECTDIR}/_ext/43898991/main.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/43898991/main.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O3 -DNDEBUG -I"../.." -I"../../../../../include" -I"../../../../../src" -I"../../../../../ports/pic32/qv/xc32" -MMD -MF "${OBJECTDIR}/_ext/43898991/main.o.d" -o ${OBJECTDIR}/_ext/43898991/main.o ../../main.c    -DXPRJ_rel=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/726959463/qstamp.o: ../../../../../include/qstamp.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/726959463" 
	@${RM} ${OBJECTDIR}/_ext/726959463/qstamp.o.d 
	@${RM} ${OBJECTDIR}/_ext/726959463/qstamp.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/726959463/qstamp.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O3 -DNDEBUG -I"../.." -I"../../../../../include" -I"../../../../../src" -I"../../../../../ports/pic32/qv/xc32" -MMD -MF "${OBJECTDIR}/_ext/726959463/qstamp.o.d" -o ${OBJECTDIR}/_ext/726959463/qstamp.o ../../../../../include/qstamp.c    -DXPRJ_rel=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
else
${OBJECTDIR}/_ext/820665871/qep_hsm.o: ../../../../../src/qf/qep_hsm.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/820665871" 
	@${RM} ${OBJECTDIR}/_ext/820665871/qep_hsm.o.d 
	@${RM} ${OBJECTDIR}/_ext/820665871/qep_hsm.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/820665871/qep_hsm.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O3 -DNDEBUG -I"../.." -I"../../../../../include" -I"../../../../../src" -I"../../../../../ports/pic32/qv/xc32" -MMD -MF "${OBJECTDIR}/_ext/820665871/qep_hsm.o.d" -o ${OBJECTDIR}/_ext/820665871/qep_hsm.o ../../../../../src/qf/qep_hsm.c    -DXPRJ_rel=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/820665871/qep_msm.o: ../../../../../src/qf/qep_msm.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/820665871" 
	@${RM} ${OBJECTDIR}/_ext/820665871/qep_msm.o.d 
	@${RM} ${OBJECTDIR}/_ext/820665871/qep_msm.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/820665871/qep_msm.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O3 -DNDEBUG -I"../.." -I"../../../../../include" -I"../../../../../src" -I"../../../../../ports/pic32/qv/xc32" -MMD -MF "${OBJECTDIR}/_ext/820665871/qep_msm.o.d" -o ${OBJECTDIR}/_ext/820665871/qep_msm.o ../../../../../src/qf/qep_msm.c    -DXPRJ_rel=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/820665871/qf_act.o: ../../../../../src/qf/qf_act.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/820665871" 
	@${RM} ${OBJECTDIR}/_ext/820665871/qf_act.o.d 
	@${RM} ${OBJECTDIR}/_ext/820665871/qf_act.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/820665871/qf_act.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O3 -DNDEBUG -I"../.." -I"../../../../../include" -I"../../../../../src" -I"../../../../../ports/pic32/qv/xc32" -MMD -MF "${OBJECTDIR}/_ext/820665871/qf_act.o.d" -o ${OBJECTDIR}/_ext/820665871/qf_act.o ../../../../../src/qf/qf_act.c    -DXPRJ_rel=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/820665871/qf_actq.o: ../../../../../src/qf/qf_actq.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/820665871" 
	@${RM} ${OBJECTDIR}/_ext/820665871/qf_actq.o.d 
	@${RM} ${OBJECTDIR}/_ext/820665871/qf_actq.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/820665871/qf_actq.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O3 -DNDEBUG -I"../.." -I"../../../../../include" -I"../../../../../src" -I"../../../../../ports/pic32/qv/xc32" -MMD -MF "${OBJECTDIR}/_ext/820665871/qf_actq.o.d" -o ${OBJECTDIR}/_ext/820665871/qf_actq.o ../../../../../src/qf/qf_actq.c    -DXPRJ_rel=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/820665871/qf_defer.o: ../../../../../src/qf/qf_defer.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/820665871" 
	@${RM} ${OBJECTDIR}/_ext/820665871/qf_defer.o.d 
	@${RM} ${OBJECTDIR}/_ext/820665871/qf_defer.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/820665871/qf_defer.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O3 -DNDEBUG -I"../.." -I"../../../../../include" -I"../../../../../src" -I"../../../../../ports/pic32/qv/xc32" -MMD -MF "${OBJECTDIR}/_ext/820665871/qf_defer.o.d" -o ${OBJECTDIR}/_ext/820665871/qf_defer.o ../../../../../src/qf/qf_defer.c    -DXPRJ_rel=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/820665871/qf_dyn.o: ../../../../../src/qf/qf_dyn.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/820665871" 
	@${RM} ${OBJECTDIR}/_ext/820665871/qf_dyn.o.d 
	@${RM} ${OBJECTDIR}/_ext/820665871/qf_dyn.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/820665871/qf_dyn.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O3 -DNDEBUG -I"../.." -I"../../../../../include" -I"../../../../../src" -I"../../../../../ports/pic32/qv/xc32" -MMD -MF "${OBJECTDIR}/_ext/820665871/qf_dyn.o.d" -o ${OBJECTDIR}/_ext/820665871/qf_dyn.o ../../../../../src/qf/qf_dyn.c    -DXPRJ_rel=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/820665871/qf_mem.o: ../../../../../src/qf/qf_mem.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/820665871" 
	@${RM} ${OBJECTDIR}/_ext/820665871/qf_mem.o.d 
	@${RM} ${OBJECTDIR}/_ext/820665871/qf_mem.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/820665871/qf_mem.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O3 -DNDEBUG -I"../.." -I"../../../../../include" -I"../../../../../src" -I"../../../../../ports/pic32/qv/xc32" -MMD -MF "${OBJECTDIR}/_ext/820665871/qf_mem.o.d" -o ${OBJECTDIR}/_ext/820665871/qf_mem.o ../../../../../src/qf/qf_mem.c    -DXPRJ_rel=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/820665871/qf_ps.o: ../../../../../src/qf/qf_ps.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/820665871" 
	@${RM} ${OBJECTDIR}/_ext/820665871/qf_ps.o.d 
	@${RM} ${OBJECTDIR}/_ext/820665871/qf_ps.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/820665871/qf_ps.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O3 -DNDEBUG -I"../.." -I"../../../../../include" -I"../../../../../src" -I"../../../../../ports/pic32/qv/xc32" -MMD -MF "${OBJECTDIR}/_ext/820665871/qf_ps.o.d" -o ${OBJECTDIR}/_ext/820665871/qf_ps.o ../../../../../src/qf/qf_ps.c    -DXPRJ_rel=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/820665871/qf_qact.o: ../../../../../src/qf/qf_qact.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/820665871" 
	@${RM} ${OBJECTDIR}/_ext/820665871/qf_qact.o.d 
	@${RM} ${OBJECTDIR}/_ext/820665871/qf_qact.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/820665871/qf_qact.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O3 -DNDEBUG -I"../.." -I"../../../../../include" -I"../../../../../src" -I"../../../../../ports/pic32/qv/xc32" -MMD -MF "${OBJECTDIR}/_ext/820665871/qf_qact.o.d" -o ${OBJECTDIR}/_ext/820665871/qf_qact.o ../../../../../src/qf/qf_qact.c    -DXPRJ_rel=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/820665871/qf_qeq.o: ../../../../../src/qf/qf_qeq.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/820665871" 
	@${RM} ${OBJECTDIR}/_ext/820665871/qf_qeq.o.d 
	@${RM} ${OBJECTDIR}/_ext/820665871/qf_qeq.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/820665871/qf_qeq.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O3 -DNDEBUG -I"../.." -I"../../../../../include" -I"../../../../../src" -I"../../../../../ports/pic32/qv/xc32" -MMD -MF "${OBJECTDIR}/_ext/820665871/qf_qeq.o.d" -o ${OBJECTDIR}/_ext/820665871/qf_qeq.o ../../../../../src/qf/qf_qeq.c    -DXPRJ_rel=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/820665871/qf_qmact.o: ../../../../../src/qf/qf_qmact.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/820665871" 
	@${RM} ${OBJECTDIR}/_ext/820665871/qf_qmact.o.d 
	@${RM} ${OBJECTDIR}/_ext/820665871/qf_qmact.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/820665871/qf_qmact.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O3 -DNDEBUG -I"../.." -I"../../../../../include" -I"../../../../../src" -I"../../../../../ports/pic32/qv/xc32" -MMD -MF "${OBJECTDIR}/_ext/820665871/qf_qmact.o.d" -o ${OBJECTDIR}/_ext/820665871/qf_qmact.o ../../../../../src/qf/qf_qmact.c    -DXPRJ_rel=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/820665871/qf_time.o: ../../../../../src/qf/qf_time.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/820665871" 
	@${RM} ${OBJECTDIR}/_ext/820665871/qf_time.o.d 
	@${RM} ${OBJECTDIR}/_ext/820665871/qf_time.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/820665871/qf_time.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O3 -DNDEBUG -I"../.." -I"../../../../../include" -I"../../../../../src" -I"../../../../../ports/pic32/qv/xc32" -MMD -MF "${OBJECTDIR}/_ext/820665871/qf_time.o.d" -o ${OBJECTDIR}/_ext/820665871/qf_time.o ../../../../../src/qf/qf_time.c    -DXPRJ_rel=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/820665887/qv.o: ../../../../../src/qv/qv.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/820665887" 
	@${RM} ${OBJECTDIR}/_ext/820665887/qv.o.d 
	@${RM} ${OBJECTDIR}/_ext/820665887/qv.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/820665887/qv.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O3 -DNDEBUG -I"../.." -I"../../../../../include" -I"../../../../../src" -I"../../../../../ports/pic32/qv/xc32" -MMD -MF "${OBJECTDIR}/_ext/820665887/qv.o.d" -o ${OBJECTDIR}/_ext/820665887/qv.o ../../../../../src/qv/qv.c    -DXPRJ_rel=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/bsp.o: bsp.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/bsp.o.d 
	@${RM} ${OBJECTDIR}/bsp.o 
	@${FIXDEPS} "${OBJECTDIR}/bsp.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O3 -DNDEBUG -I"../.." -I"../../../../../include" -I"../../../../../src" -I"../../../../../ports/pic32/qv/xc32" -MMD -MF "${OBJECTDIR}/bsp.o.d" -o ${OBJECTDIR}/bsp.o bsp.c    -DXPRJ_rel=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/43898991/blinky.o: ../../blinky.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/43898991" 
	@${RM} ${OBJECTDIR}/_ext/43898991/blinky.o.d 
	@${RM} ${OBJECTDIR}/_ext/43898991/blinky.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/43898991/blinky.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O3 -DNDEBUG -I"../.." -I"../../../../../include" -I"../../../../../src" -I"../../../../../ports/pic32/qv/xc32" -MMD -MF "${OBJECTDIR}/_ext/43898991/blinky.o.d" -o ${OBJECTDIR}/_ext/43898991/blinky.o ../../blinky.c    -DXPRJ_rel=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/43898991/main.o: ../../main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/43898991" 
	@${RM} ${OBJECTDIR}/_ext/43898991/main.o.d 
	@${RM} ${OBJECTDIR}/_ext/43898991/main.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/43898991/main.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O3 -DNDEBUG -I"../.." -I"../../../../../include" -I"../../../../../src" -I"../../../../../ports/pic32/qv/xc32" -MMD -MF "${OBJECTDIR}/_ext/43898991/main.o.d" -o ${OBJECTDIR}/_ext/43898991/main.o ../../main.c    -DXPRJ_rel=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/726959463/qstamp.o: ../../../../../include/qstamp.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/726959463" 
	@${RM} ${OBJECTDIR}/_ext/726959463/qstamp.o.d 
	@${RM} ${OBJECTDIR}/_ext/726959463/qstamp.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/726959463/qstamp.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O3 -DNDEBUG -I"../.." -I"../../../../../include" -I"../../../../../src" -I"../../../../../ports/pic32/qv/xc32" -MMD -MF "${OBJECTDIR}/_ext/726959463/qstamp.o.d" -o ${OBJECTDIR}/_ext/726959463/qstamp.o ../../../../../include/qstamp.c    -DXPRJ_rel=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compileCPP
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
dist/${CND_CONF}/${IMAGE_TYPE}/xc32.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE) -g -mdebugger -D__MPLAB_DEBUGGER_PK3=1 -mprocessor=$(MP_PROCESSOR_OPTION)  -o dist/${CND_CONF}/${IMAGE_TYPE}/xc32.${IMAGE_TYPE}.${OUTPUT_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}          -DXPRJ_rel=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)   -mreserve=data@0x0:0x1FC -mreserve=boot@0x1FC00490:0x1FC00BEF  -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,-D=__DEBUG_D,--defsym=__MPLAB_DEBUGGER_PK3=1,--defsym=_min_heap_size=0,--no-code-in-dinit,--no-dinit-in-serial-mem,--report-mem,--warn-section-align,--memorysummary,dist/${CND_CONF}/${IMAGE_TYPE}/memoryfile.xml
	
else
dist/${CND_CONF}/${IMAGE_TYPE}/xc32.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -mprocessor=$(MP_PROCESSOR_OPTION)  -o dist/${CND_CONF}/${IMAGE_TYPE}/xc32.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}          -DXPRJ_rel=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--defsym=_min_heap_size=0,--no-code-in-dinit,--no-dinit-in-serial-mem,--report-mem,--warn-section-align,--memorysummary,dist/${CND_CONF}/${IMAGE_TYPE}/memoryfile.xml
	${MP_CC_DIR}\\xc32-bin2hex dist/${CND_CONF}/${IMAGE_TYPE}/xc32.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} 
endif


# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/rel
	${RM} -r dist/rel

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(shell mplabwildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif

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
ifeq "$(wildcard nbproject/Makefile-local-default.mk)" "nbproject/Makefile-local-default.mk"
include nbproject/Makefile-local-default.mk
endif
endif

# Environment
MKDIR=gnumkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=default
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=elf
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/eForth_test.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/eForth_test.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=eForth_test.c ../EF.S ../../../hardware/serial_comm.c ../../../hardware/HardwareProfile.c ../../../sound.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/eForth_test.o ${OBJECTDIR}/_ext/1472/EF.o ${OBJECTDIR}/_ext/1188675511/serial_comm.o ${OBJECTDIR}/_ext/1188675511/HardwareProfile.o ${OBJECTDIR}/_ext/2124829536/sound.o
POSSIBLE_DEPFILES=${OBJECTDIR}/eForth_test.o.d ${OBJECTDIR}/_ext/1472/EF.o.d ${OBJECTDIR}/_ext/1188675511/serial_comm.o.d ${OBJECTDIR}/_ext/1188675511/HardwareProfile.o.d ${OBJECTDIR}/_ext/2124829536/sound.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/eForth_test.o ${OBJECTDIR}/_ext/1472/EF.o ${OBJECTDIR}/_ext/1188675511/serial_comm.o ${OBJECTDIR}/_ext/1188675511/HardwareProfile.o ${OBJECTDIR}/_ext/2124829536/sound.o

# Source Files
SOURCEFILES=eForth_test.c ../EF.S ../../../hardware/serial_comm.c ../../../hardware/HardwareProfile.c ../../../sound.c


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
	${MAKE} ${MAKE_OPTIONS} -f nbproject/Makefile-default.mk dist/${CND_CONF}/${IMAGE_TYPE}/eForth_test.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=32MX150F128B
MP_LINKER_FILE_OPTION=
# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assembleWithPreprocess
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/_ext/1472/EF.o: ../EF.S  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1472 
	@${RM} ${OBJECTDIR}/_ext/1472/EF.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/EF.o.ok ${OBJECTDIR}/_ext/1472/EF.o.err 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/EF.o.d" "${OBJECTDIR}/_ext/1472/EF.o.asm.d" -t $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC} $(MP_EXTRA_AS_PRE)  -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1472/EF.o.d"  -o ${OBJECTDIR}/_ext/1472/EF.o ../EF.S  -Wa,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_AS_POST),-MD="${OBJECTDIR}/_ext/1472/EF.o.asm.d",--defsym=__ICD2RAM=1,--defsym=__MPLAB_DEBUG=1,--gdwarf-2,--defsym=__DEBUG=1,--defsym=__MPLAB_DEBUGGER_PK3=1
	
else
${OBJECTDIR}/_ext/1472/EF.o: ../EF.S  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1472 
	@${RM} ${OBJECTDIR}/_ext/1472/EF.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/EF.o.ok ${OBJECTDIR}/_ext/1472/EF.o.err 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/EF.o.d" "${OBJECTDIR}/_ext/1472/EF.o.asm.d" -t $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC} $(MP_EXTRA_AS_PRE)  -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1472/EF.o.d"  -o ${OBJECTDIR}/_ext/1472/EF.o ../EF.S  -Wa,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_AS_POST),-MD="${OBJECTDIR}/_ext/1472/EF.o.asm.d",--gdwarf-2
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/eForth_test.o: eForth_test.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/eForth_test.o.d 
	@${FIXDEPS} "${OBJECTDIR}/eForth_test.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/eForth_test.o.d" -o ${OBJECTDIR}/eForth_test.o eForth_test.c   
	
${OBJECTDIR}/_ext/1188675511/serial_comm.o: ../../../hardware/serial_comm.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1188675511 
	@${RM} ${OBJECTDIR}/_ext/1188675511/serial_comm.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1188675511/serial_comm.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1188675511/serial_comm.o.d" -o ${OBJECTDIR}/_ext/1188675511/serial_comm.o ../../../hardware/serial_comm.c   
	
${OBJECTDIR}/_ext/1188675511/HardwareProfile.o: ../../../hardware/HardwareProfile.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1188675511 
	@${RM} ${OBJECTDIR}/_ext/1188675511/HardwareProfile.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1188675511/HardwareProfile.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1188675511/HardwareProfile.o.d" -o ${OBJECTDIR}/_ext/1188675511/HardwareProfile.o ../../../hardware/HardwareProfile.c   
	
${OBJECTDIR}/_ext/2124829536/sound.o: ../../../sound.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/2124829536 
	@${RM} ${OBJECTDIR}/_ext/2124829536/sound.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/2124829536/sound.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/2124829536/sound.o.d" -o ${OBJECTDIR}/_ext/2124829536/sound.o ../../../sound.c   
	
else
${OBJECTDIR}/eForth_test.o: eForth_test.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/eForth_test.o.d 
	@${FIXDEPS} "${OBJECTDIR}/eForth_test.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/eForth_test.o.d" -o ${OBJECTDIR}/eForth_test.o eForth_test.c   
	
${OBJECTDIR}/_ext/1188675511/serial_comm.o: ../../../hardware/serial_comm.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1188675511 
	@${RM} ${OBJECTDIR}/_ext/1188675511/serial_comm.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1188675511/serial_comm.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1188675511/serial_comm.o.d" -o ${OBJECTDIR}/_ext/1188675511/serial_comm.o ../../../hardware/serial_comm.c   
	
${OBJECTDIR}/_ext/1188675511/HardwareProfile.o: ../../../hardware/HardwareProfile.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1188675511 
	@${RM} ${OBJECTDIR}/_ext/1188675511/HardwareProfile.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1188675511/HardwareProfile.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1188675511/HardwareProfile.o.d" -o ${OBJECTDIR}/_ext/1188675511/HardwareProfile.o ../../../hardware/HardwareProfile.c   
	
${OBJECTDIR}/_ext/2124829536/sound.o: ../../../sound.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/2124829536 
	@${RM} ${OBJECTDIR}/_ext/2124829536/sound.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/2124829536/sound.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/2124829536/sound.o.d" -o ${OBJECTDIR}/_ext/2124829536/sound.o ../../../sound.c   
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compileCPP
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
dist/${CND_CONF}/${IMAGE_TYPE}/eForth_test.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -mdebugger -D__MPLAB_DEBUGGER_PK3=1 -mprocessor=$(MP_PROCESSOR_OPTION)  -o dist/${CND_CONF}/${IMAGE_TYPE}/eForth_test.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}          -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--defsym=__ICD2RAM=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,--defsym=__MPLAB_DEBUGGER_PK3=1,--gc-sections,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map"
	
else
dist/${CND_CONF}/${IMAGE_TYPE}/eForth_test.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -mprocessor=$(MP_PROCESSOR_OPTION)  -o dist/${CND_CONF}/${IMAGE_TYPE}/eForth_test.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}          -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--gc-sections,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map"
	${MP_CC_DIR}\\xc32-bin2hex dist/${CND_CONF}/${IMAGE_TYPE}/eForth_test.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} 
endif


# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/default
	${RM} -r dist/default

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(shell mplabwildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif

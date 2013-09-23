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
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/DiosForth.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/DiosForth.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=source/Main.c source/VM/VMcore.c source/VM/VMword.c source/NVMem/NVMem.c source/OCMP/OCmp.c source/PIN/Pin.c source/TIMER/MTimer.c source/UART/UartBuf.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/source/Main.o ${OBJECTDIR}/source/VM/VMcore.o ${OBJECTDIR}/source/VM/VMword.o ${OBJECTDIR}/source/NVMem/NVMem.o ${OBJECTDIR}/source/OCMP/OCmp.o ${OBJECTDIR}/source/PIN/Pin.o ${OBJECTDIR}/source/TIMER/MTimer.o ${OBJECTDIR}/source/UART/UartBuf.o
POSSIBLE_DEPFILES=${OBJECTDIR}/source/Main.o.d ${OBJECTDIR}/source/VM/VMcore.o.d ${OBJECTDIR}/source/VM/VMword.o.d ${OBJECTDIR}/source/NVMem/NVMem.o.d ${OBJECTDIR}/source/OCMP/OCmp.o.d ${OBJECTDIR}/source/PIN/Pin.o.d ${OBJECTDIR}/source/TIMER/MTimer.o.d ${OBJECTDIR}/source/UART/UartBuf.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/source/Main.o ${OBJECTDIR}/source/VM/VMcore.o ${OBJECTDIR}/source/VM/VMword.o ${OBJECTDIR}/source/NVMem/NVMem.o ${OBJECTDIR}/source/OCMP/OCmp.o ${OBJECTDIR}/source/PIN/Pin.o ${OBJECTDIR}/source/TIMER/MTimer.o ${OBJECTDIR}/source/UART/UartBuf.o

# Source Files
SOURCEFILES=source/Main.c source/VM/VMcore.c source/VM/VMword.c source/NVMem/NVMem.c source/OCMP/OCmp.c source/PIN/Pin.c source/TIMER/MTimer.c source/UART/UartBuf.c


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
	${MAKE} ${MAKE_OPTIONS} -f nbproject/Makefile-default.mk dist/${CND_CONF}/${IMAGE_TYPE}/DiosForth.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

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
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/source/Main.o: source/Main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/source 
	@${RM} ${OBJECTDIR}/source/Main.o.d 
	@${FIXDEPS} "${OBJECTDIR}/source/Main.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/source/Main.o.d" -o ${OBJECTDIR}/source/Main.o source/Main.c   
	
${OBJECTDIR}/source/VM/VMcore.o: source/VM/VMcore.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/source/VM 
	@${RM} ${OBJECTDIR}/source/VM/VMcore.o.d 
	@${FIXDEPS} "${OBJECTDIR}/source/VM/VMcore.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/source/VM/VMcore.o.d" -o ${OBJECTDIR}/source/VM/VMcore.o source/VM/VMcore.c   
	
${OBJECTDIR}/source/VM/VMword.o: source/VM/VMword.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/source/VM 
	@${RM} ${OBJECTDIR}/source/VM/VMword.o.d 
	@${FIXDEPS} "${OBJECTDIR}/source/VM/VMword.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/source/VM/VMword.o.d" -o ${OBJECTDIR}/source/VM/VMword.o source/VM/VMword.c   
	
${OBJECTDIR}/source/NVMem/NVMem.o: source/NVMem/NVMem.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/source/NVMem 
	@${RM} ${OBJECTDIR}/source/NVMem/NVMem.o.d 
	@${FIXDEPS} "${OBJECTDIR}/source/NVMem/NVMem.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/source/NVMem/NVMem.o.d" -o ${OBJECTDIR}/source/NVMem/NVMem.o source/NVMem/NVMem.c   
	
${OBJECTDIR}/source/OCMP/OCmp.o: source/OCMP/OCmp.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/source/OCMP 
	@${RM} ${OBJECTDIR}/source/OCMP/OCmp.o.d 
	@${FIXDEPS} "${OBJECTDIR}/source/OCMP/OCmp.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/source/OCMP/OCmp.o.d" -o ${OBJECTDIR}/source/OCMP/OCmp.o source/OCMP/OCmp.c   
	
${OBJECTDIR}/source/PIN/Pin.o: source/PIN/Pin.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/source/PIN 
	@${RM} ${OBJECTDIR}/source/PIN/Pin.o.d 
	@${FIXDEPS} "${OBJECTDIR}/source/PIN/Pin.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/source/PIN/Pin.o.d" -o ${OBJECTDIR}/source/PIN/Pin.o source/PIN/Pin.c   
	
${OBJECTDIR}/source/TIMER/MTimer.o: source/TIMER/MTimer.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/source/TIMER 
	@${RM} ${OBJECTDIR}/source/TIMER/MTimer.o.d 
	@${FIXDEPS} "${OBJECTDIR}/source/TIMER/MTimer.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/source/TIMER/MTimer.o.d" -o ${OBJECTDIR}/source/TIMER/MTimer.o source/TIMER/MTimer.c   
	
${OBJECTDIR}/source/UART/UartBuf.o: source/UART/UartBuf.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/source/UART 
	@${RM} ${OBJECTDIR}/source/UART/UartBuf.o.d 
	@${FIXDEPS} "${OBJECTDIR}/source/UART/UartBuf.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/source/UART/UartBuf.o.d" -o ${OBJECTDIR}/source/UART/UartBuf.o source/UART/UartBuf.c   
	
else
${OBJECTDIR}/source/Main.o: source/Main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/source 
	@${RM} ${OBJECTDIR}/source/Main.o.d 
	@${FIXDEPS} "${OBJECTDIR}/source/Main.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/source/Main.o.d" -o ${OBJECTDIR}/source/Main.o source/Main.c   
	
${OBJECTDIR}/source/VM/VMcore.o: source/VM/VMcore.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/source/VM 
	@${RM} ${OBJECTDIR}/source/VM/VMcore.o.d 
	@${FIXDEPS} "${OBJECTDIR}/source/VM/VMcore.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/source/VM/VMcore.o.d" -o ${OBJECTDIR}/source/VM/VMcore.o source/VM/VMcore.c   
	
${OBJECTDIR}/source/VM/VMword.o: source/VM/VMword.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/source/VM 
	@${RM} ${OBJECTDIR}/source/VM/VMword.o.d 
	@${FIXDEPS} "${OBJECTDIR}/source/VM/VMword.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/source/VM/VMword.o.d" -o ${OBJECTDIR}/source/VM/VMword.o source/VM/VMword.c   
	
${OBJECTDIR}/source/NVMem/NVMem.o: source/NVMem/NVMem.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/source/NVMem 
	@${RM} ${OBJECTDIR}/source/NVMem/NVMem.o.d 
	@${FIXDEPS} "${OBJECTDIR}/source/NVMem/NVMem.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/source/NVMem/NVMem.o.d" -o ${OBJECTDIR}/source/NVMem/NVMem.o source/NVMem/NVMem.c   
	
${OBJECTDIR}/source/OCMP/OCmp.o: source/OCMP/OCmp.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/source/OCMP 
	@${RM} ${OBJECTDIR}/source/OCMP/OCmp.o.d 
	@${FIXDEPS} "${OBJECTDIR}/source/OCMP/OCmp.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/source/OCMP/OCmp.o.d" -o ${OBJECTDIR}/source/OCMP/OCmp.o source/OCMP/OCmp.c   
	
${OBJECTDIR}/source/PIN/Pin.o: source/PIN/Pin.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/source/PIN 
	@${RM} ${OBJECTDIR}/source/PIN/Pin.o.d 
	@${FIXDEPS} "${OBJECTDIR}/source/PIN/Pin.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/source/PIN/Pin.o.d" -o ${OBJECTDIR}/source/PIN/Pin.o source/PIN/Pin.c   
	
${OBJECTDIR}/source/TIMER/MTimer.o: source/TIMER/MTimer.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/source/TIMER 
	@${RM} ${OBJECTDIR}/source/TIMER/MTimer.o.d 
	@${FIXDEPS} "${OBJECTDIR}/source/TIMER/MTimer.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/source/TIMER/MTimer.o.d" -o ${OBJECTDIR}/source/TIMER/MTimer.o source/TIMER/MTimer.c   
	
${OBJECTDIR}/source/UART/UartBuf.o: source/UART/UartBuf.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/source/UART 
	@${RM} ${OBJECTDIR}/source/UART/UartBuf.o.d 
	@${FIXDEPS} "${OBJECTDIR}/source/UART/UartBuf.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/source/UART/UartBuf.o.d" -o ${OBJECTDIR}/source/UART/UartBuf.o source/UART/UartBuf.c   
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compileCPP
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
dist/${CND_CONF}/${IMAGE_TYPE}/DiosForth.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -mdebugger -D__MPLAB_DEBUGGER_PK3=1 -mprocessor=$(MP_PROCESSOR_OPTION)  -o dist/${CND_CONF}/${IMAGE_TYPE}/DiosForth.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}          -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--defsym=__ICD2RAM=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,--defsym=__MPLAB_DEBUGGER_PK3=1,--gc-sections,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--report-mem,--cref
	
else
dist/${CND_CONF}/${IMAGE_TYPE}/DiosForth.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -mprocessor=$(MP_PROCESSOR_OPTION)  -o dist/${CND_CONF}/${IMAGE_TYPE}/DiosForth.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}          -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--gc-sections,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--report-mem,--cref
	${MP_CC_DIR}\\xc32-bin2hex dist/${CND_CONF}/${IMAGE_TYPE}/DiosForth.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} 
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

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
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/VPC-32.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/VPC-32.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=DiosForth.X/source/VM/VMcore.c DiosForth.X/source/VM/VMword.c DiosForth.X/source/NVMem/NVMem.c hardware/ntsc.c hardware/HardwareProfile.c hardware/serial_comm.c hardware/keyboard.c hardware/Pinguino/sdmmc.c hardware/Pinguino/diskio.c hardware/QWERTY.c console.c font.c hardware/Pinguino/fileio.c hardware/Pinguino/ff.c sound.c editor.c shell.c vpc-32.c graphics.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/DiosForth.X/source/VM/VMcore.o ${OBJECTDIR}/DiosForth.X/source/VM/VMword.o ${OBJECTDIR}/DiosForth.X/source/NVMem/NVMem.o ${OBJECTDIR}/hardware/ntsc.o ${OBJECTDIR}/hardware/HardwareProfile.o ${OBJECTDIR}/hardware/serial_comm.o ${OBJECTDIR}/hardware/keyboard.o ${OBJECTDIR}/hardware/Pinguino/sdmmc.o ${OBJECTDIR}/hardware/Pinguino/diskio.o ${OBJECTDIR}/hardware/QWERTY.o ${OBJECTDIR}/console.o ${OBJECTDIR}/font.o ${OBJECTDIR}/hardware/Pinguino/fileio.o ${OBJECTDIR}/hardware/Pinguino/ff.o ${OBJECTDIR}/sound.o ${OBJECTDIR}/editor.o ${OBJECTDIR}/shell.o ${OBJECTDIR}/vpc-32.o ${OBJECTDIR}/graphics.o
POSSIBLE_DEPFILES=${OBJECTDIR}/DiosForth.X/source/VM/VMcore.o.d ${OBJECTDIR}/DiosForth.X/source/VM/VMword.o.d ${OBJECTDIR}/DiosForth.X/source/NVMem/NVMem.o.d ${OBJECTDIR}/hardware/ntsc.o.d ${OBJECTDIR}/hardware/HardwareProfile.o.d ${OBJECTDIR}/hardware/serial_comm.o.d ${OBJECTDIR}/hardware/keyboard.o.d ${OBJECTDIR}/hardware/Pinguino/sdmmc.o.d ${OBJECTDIR}/hardware/Pinguino/diskio.o.d ${OBJECTDIR}/hardware/QWERTY.o.d ${OBJECTDIR}/console.o.d ${OBJECTDIR}/font.o.d ${OBJECTDIR}/hardware/Pinguino/fileio.o.d ${OBJECTDIR}/hardware/Pinguino/ff.o.d ${OBJECTDIR}/sound.o.d ${OBJECTDIR}/editor.o.d ${OBJECTDIR}/shell.o.d ${OBJECTDIR}/vpc-32.o.d ${OBJECTDIR}/graphics.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/DiosForth.X/source/VM/VMcore.o ${OBJECTDIR}/DiosForth.X/source/VM/VMword.o ${OBJECTDIR}/DiosForth.X/source/NVMem/NVMem.o ${OBJECTDIR}/hardware/ntsc.o ${OBJECTDIR}/hardware/HardwareProfile.o ${OBJECTDIR}/hardware/serial_comm.o ${OBJECTDIR}/hardware/keyboard.o ${OBJECTDIR}/hardware/Pinguino/sdmmc.o ${OBJECTDIR}/hardware/Pinguino/diskio.o ${OBJECTDIR}/hardware/QWERTY.o ${OBJECTDIR}/console.o ${OBJECTDIR}/font.o ${OBJECTDIR}/hardware/Pinguino/fileio.o ${OBJECTDIR}/hardware/Pinguino/ff.o ${OBJECTDIR}/sound.o ${OBJECTDIR}/editor.o ${OBJECTDIR}/shell.o ${OBJECTDIR}/vpc-32.o ${OBJECTDIR}/graphics.o

# Source Files
SOURCEFILES=DiosForth.X/source/VM/VMcore.c DiosForth.X/source/VM/VMword.c DiosForth.X/source/NVMem/NVMem.c hardware/ntsc.c hardware/HardwareProfile.c hardware/serial_comm.c hardware/keyboard.c hardware/Pinguino/sdmmc.c hardware/Pinguino/diskio.c hardware/QWERTY.c console.c font.c hardware/Pinguino/fileio.c hardware/Pinguino/ff.c sound.c editor.c shell.c vpc-32.c graphics.c


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
	${MAKE} ${MAKE_OPTIONS} -f nbproject/Makefile-default.mk dist/${CND_CONF}/${IMAGE_TYPE}/VPC-32.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

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
${OBJECTDIR}/DiosForth.X/source/VM/VMcore.o: DiosForth.X/source/VM/VMcore.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/DiosForth.X/source/VM 
	@${RM} ${OBJECTDIR}/DiosForth.X/source/VM/VMcore.o.d 
	@${RM} ${OBJECTDIR}/DiosForth.X/source/VM/VMcore.o 
	@${FIXDEPS} "${OBJECTDIR}/DiosForth.X/source/VM/VMcore.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -O1 -DSD_DEBUG -MMD -MF "${OBJECTDIR}/DiosForth.X/source/VM/VMcore.o.d" -o ${OBJECTDIR}/DiosForth.X/source/VM/VMcore.o DiosForth.X/source/VM/VMcore.c   
	
${OBJECTDIR}/DiosForth.X/source/VM/VMword.o: DiosForth.X/source/VM/VMword.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/DiosForth.X/source/VM 
	@${RM} ${OBJECTDIR}/DiosForth.X/source/VM/VMword.o.d 
	@${RM} ${OBJECTDIR}/DiosForth.X/source/VM/VMword.o 
	@${FIXDEPS} "${OBJECTDIR}/DiosForth.X/source/VM/VMword.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -O1 -DSD_DEBUG -MMD -MF "${OBJECTDIR}/DiosForth.X/source/VM/VMword.o.d" -o ${OBJECTDIR}/DiosForth.X/source/VM/VMword.o DiosForth.X/source/VM/VMword.c   
	
${OBJECTDIR}/DiosForth.X/source/NVMem/NVMem.o: DiosForth.X/source/NVMem/NVMem.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/DiosForth.X/source/NVMem 
	@${RM} ${OBJECTDIR}/DiosForth.X/source/NVMem/NVMem.o.d 
	@${RM} ${OBJECTDIR}/DiosForth.X/source/NVMem/NVMem.o 
	@${FIXDEPS} "${OBJECTDIR}/DiosForth.X/source/NVMem/NVMem.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -O1 -DSD_DEBUG -MMD -MF "${OBJECTDIR}/DiosForth.X/source/NVMem/NVMem.o.d" -o ${OBJECTDIR}/DiosForth.X/source/NVMem/NVMem.o DiosForth.X/source/NVMem/NVMem.c   
	
${OBJECTDIR}/hardware/ntsc.o: hardware/ntsc.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/hardware 
	@${RM} ${OBJECTDIR}/hardware/ntsc.o.d 
	@${RM} ${OBJECTDIR}/hardware/ntsc.o 
	@${FIXDEPS} "${OBJECTDIR}/hardware/ntsc.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -O1 -DSD_DEBUG -MMD -MF "${OBJECTDIR}/hardware/ntsc.o.d" -o ${OBJECTDIR}/hardware/ntsc.o hardware/ntsc.c   
	
${OBJECTDIR}/hardware/HardwareProfile.o: hardware/HardwareProfile.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/hardware 
	@${RM} ${OBJECTDIR}/hardware/HardwareProfile.o.d 
	@${RM} ${OBJECTDIR}/hardware/HardwareProfile.o 
	@${FIXDEPS} "${OBJECTDIR}/hardware/HardwareProfile.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -O1 -DSD_DEBUG -MMD -MF "${OBJECTDIR}/hardware/HardwareProfile.o.d" -o ${OBJECTDIR}/hardware/HardwareProfile.o hardware/HardwareProfile.c   
	
${OBJECTDIR}/hardware/serial_comm.o: hardware/serial_comm.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/hardware 
	@${RM} ${OBJECTDIR}/hardware/serial_comm.o.d 
	@${RM} ${OBJECTDIR}/hardware/serial_comm.o 
	@${FIXDEPS} "${OBJECTDIR}/hardware/serial_comm.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -O1 -DSD_DEBUG -MMD -MF "${OBJECTDIR}/hardware/serial_comm.o.d" -o ${OBJECTDIR}/hardware/serial_comm.o hardware/serial_comm.c   
	
${OBJECTDIR}/hardware/keyboard.o: hardware/keyboard.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/hardware 
	@${RM} ${OBJECTDIR}/hardware/keyboard.o.d 
	@${RM} ${OBJECTDIR}/hardware/keyboard.o 
	@${FIXDEPS} "${OBJECTDIR}/hardware/keyboard.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -O1 -DSD_DEBUG -MMD -MF "${OBJECTDIR}/hardware/keyboard.o.d" -o ${OBJECTDIR}/hardware/keyboard.o hardware/keyboard.c   
	
${OBJECTDIR}/hardware/Pinguino/sdmmc.o: hardware/Pinguino/sdmmc.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/hardware/Pinguino 
	@${RM} ${OBJECTDIR}/hardware/Pinguino/sdmmc.o.d 
	@${RM} ${OBJECTDIR}/hardware/Pinguino/sdmmc.o 
	@${FIXDEPS} "${OBJECTDIR}/hardware/Pinguino/sdmmc.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -O1 -DSD_DEBUG -MMD -MF "${OBJECTDIR}/hardware/Pinguino/sdmmc.o.d" -o ${OBJECTDIR}/hardware/Pinguino/sdmmc.o hardware/Pinguino/sdmmc.c   
	
${OBJECTDIR}/hardware/Pinguino/diskio.o: hardware/Pinguino/diskio.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/hardware/Pinguino 
	@${RM} ${OBJECTDIR}/hardware/Pinguino/diskio.o.d 
	@${RM} ${OBJECTDIR}/hardware/Pinguino/diskio.o 
	@${FIXDEPS} "${OBJECTDIR}/hardware/Pinguino/diskio.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -O1 -DSD_DEBUG -MMD -MF "${OBJECTDIR}/hardware/Pinguino/diskio.o.d" -o ${OBJECTDIR}/hardware/Pinguino/diskio.o hardware/Pinguino/diskio.c   
	
${OBJECTDIR}/hardware/QWERTY.o: hardware/QWERTY.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/hardware 
	@${RM} ${OBJECTDIR}/hardware/QWERTY.o.d 
	@${RM} ${OBJECTDIR}/hardware/QWERTY.o 
	@${FIXDEPS} "${OBJECTDIR}/hardware/QWERTY.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -O1 -DSD_DEBUG -MMD -MF "${OBJECTDIR}/hardware/QWERTY.o.d" -o ${OBJECTDIR}/hardware/QWERTY.o hardware/QWERTY.c   
	
${OBJECTDIR}/console.o: console.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/console.o.d 
	@${RM} ${OBJECTDIR}/console.o 
	@${FIXDEPS} "${OBJECTDIR}/console.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -O1 -DSD_DEBUG -MMD -MF "${OBJECTDIR}/console.o.d" -o ${OBJECTDIR}/console.o console.c   
	
${OBJECTDIR}/font.o: font.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/font.o.d 
	@${RM} ${OBJECTDIR}/font.o 
	@${FIXDEPS} "${OBJECTDIR}/font.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -O1 -DSD_DEBUG -MMD -MF "${OBJECTDIR}/font.o.d" -o ${OBJECTDIR}/font.o font.c   
	
${OBJECTDIR}/hardware/Pinguino/fileio.o: hardware/Pinguino/fileio.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/hardware/Pinguino 
	@${RM} ${OBJECTDIR}/hardware/Pinguino/fileio.o.d 
	@${RM} ${OBJECTDIR}/hardware/Pinguino/fileio.o 
	@${FIXDEPS} "${OBJECTDIR}/hardware/Pinguino/fileio.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -O1 -DSD_DEBUG -MMD -MF "${OBJECTDIR}/hardware/Pinguino/fileio.o.d" -o ${OBJECTDIR}/hardware/Pinguino/fileio.o hardware/Pinguino/fileio.c   
	
${OBJECTDIR}/hardware/Pinguino/ff.o: hardware/Pinguino/ff.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/hardware/Pinguino 
	@${RM} ${OBJECTDIR}/hardware/Pinguino/ff.o.d 
	@${RM} ${OBJECTDIR}/hardware/Pinguino/ff.o 
	@${FIXDEPS} "${OBJECTDIR}/hardware/Pinguino/ff.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -O1 -DSD_DEBUG -MMD -MF "${OBJECTDIR}/hardware/Pinguino/ff.o.d" -o ${OBJECTDIR}/hardware/Pinguino/ff.o hardware/Pinguino/ff.c   
	
${OBJECTDIR}/sound.o: sound.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/sound.o.d 
	@${RM} ${OBJECTDIR}/sound.o 
	@${FIXDEPS} "${OBJECTDIR}/sound.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -O1 -DSD_DEBUG -MMD -MF "${OBJECTDIR}/sound.o.d" -o ${OBJECTDIR}/sound.o sound.c   
	
${OBJECTDIR}/editor.o: editor.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/editor.o.d 
	@${RM} ${OBJECTDIR}/editor.o 
	@${FIXDEPS} "${OBJECTDIR}/editor.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -O1 -DSD_DEBUG -MMD -MF "${OBJECTDIR}/editor.o.d" -o ${OBJECTDIR}/editor.o editor.c   
	
${OBJECTDIR}/shell.o: shell.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/shell.o.d 
	@${RM} ${OBJECTDIR}/shell.o 
	@${FIXDEPS} "${OBJECTDIR}/shell.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -O1 -DSD_DEBUG -MMD -MF "${OBJECTDIR}/shell.o.d" -o ${OBJECTDIR}/shell.o shell.c   
	
${OBJECTDIR}/vpc-32.o: vpc-32.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/vpc-32.o.d 
	@${RM} ${OBJECTDIR}/vpc-32.o 
	@${FIXDEPS} "${OBJECTDIR}/vpc-32.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -O1 -DSD_DEBUG -MMD -MF "${OBJECTDIR}/vpc-32.o.d" -o ${OBJECTDIR}/vpc-32.o vpc-32.c   
	
${OBJECTDIR}/graphics.o: graphics.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/graphics.o.d 
	@${RM} ${OBJECTDIR}/graphics.o 
	@${FIXDEPS} "${OBJECTDIR}/graphics.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -O1 -DSD_DEBUG -MMD -MF "${OBJECTDIR}/graphics.o.d" -o ${OBJECTDIR}/graphics.o graphics.c   
	
else
${OBJECTDIR}/DiosForth.X/source/VM/VMcore.o: DiosForth.X/source/VM/VMcore.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/DiosForth.X/source/VM 
	@${RM} ${OBJECTDIR}/DiosForth.X/source/VM/VMcore.o.d 
	@${RM} ${OBJECTDIR}/DiosForth.X/source/VM/VMcore.o 
	@${FIXDEPS} "${OBJECTDIR}/DiosForth.X/source/VM/VMcore.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -O1 -DSD_DEBUG -MMD -MF "${OBJECTDIR}/DiosForth.X/source/VM/VMcore.o.d" -o ${OBJECTDIR}/DiosForth.X/source/VM/VMcore.o DiosForth.X/source/VM/VMcore.c   
	
${OBJECTDIR}/DiosForth.X/source/VM/VMword.o: DiosForth.X/source/VM/VMword.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/DiosForth.X/source/VM 
	@${RM} ${OBJECTDIR}/DiosForth.X/source/VM/VMword.o.d 
	@${RM} ${OBJECTDIR}/DiosForth.X/source/VM/VMword.o 
	@${FIXDEPS} "${OBJECTDIR}/DiosForth.X/source/VM/VMword.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -O1 -DSD_DEBUG -MMD -MF "${OBJECTDIR}/DiosForth.X/source/VM/VMword.o.d" -o ${OBJECTDIR}/DiosForth.X/source/VM/VMword.o DiosForth.X/source/VM/VMword.c   
	
${OBJECTDIR}/DiosForth.X/source/NVMem/NVMem.o: DiosForth.X/source/NVMem/NVMem.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/DiosForth.X/source/NVMem 
	@${RM} ${OBJECTDIR}/DiosForth.X/source/NVMem/NVMem.o.d 
	@${RM} ${OBJECTDIR}/DiosForth.X/source/NVMem/NVMem.o 
	@${FIXDEPS} "${OBJECTDIR}/DiosForth.X/source/NVMem/NVMem.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -O1 -DSD_DEBUG -MMD -MF "${OBJECTDIR}/DiosForth.X/source/NVMem/NVMem.o.d" -o ${OBJECTDIR}/DiosForth.X/source/NVMem/NVMem.o DiosForth.X/source/NVMem/NVMem.c   
	
${OBJECTDIR}/hardware/ntsc.o: hardware/ntsc.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/hardware 
	@${RM} ${OBJECTDIR}/hardware/ntsc.o.d 
	@${RM} ${OBJECTDIR}/hardware/ntsc.o 
	@${FIXDEPS} "${OBJECTDIR}/hardware/ntsc.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -O1 -DSD_DEBUG -MMD -MF "${OBJECTDIR}/hardware/ntsc.o.d" -o ${OBJECTDIR}/hardware/ntsc.o hardware/ntsc.c   
	
${OBJECTDIR}/hardware/HardwareProfile.o: hardware/HardwareProfile.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/hardware 
	@${RM} ${OBJECTDIR}/hardware/HardwareProfile.o.d 
	@${RM} ${OBJECTDIR}/hardware/HardwareProfile.o 
	@${FIXDEPS} "${OBJECTDIR}/hardware/HardwareProfile.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -O1 -DSD_DEBUG -MMD -MF "${OBJECTDIR}/hardware/HardwareProfile.o.d" -o ${OBJECTDIR}/hardware/HardwareProfile.o hardware/HardwareProfile.c   
	
${OBJECTDIR}/hardware/serial_comm.o: hardware/serial_comm.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/hardware 
	@${RM} ${OBJECTDIR}/hardware/serial_comm.o.d 
	@${RM} ${OBJECTDIR}/hardware/serial_comm.o 
	@${FIXDEPS} "${OBJECTDIR}/hardware/serial_comm.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -O1 -DSD_DEBUG -MMD -MF "${OBJECTDIR}/hardware/serial_comm.o.d" -o ${OBJECTDIR}/hardware/serial_comm.o hardware/serial_comm.c   
	
${OBJECTDIR}/hardware/keyboard.o: hardware/keyboard.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/hardware 
	@${RM} ${OBJECTDIR}/hardware/keyboard.o.d 
	@${RM} ${OBJECTDIR}/hardware/keyboard.o 
	@${FIXDEPS} "${OBJECTDIR}/hardware/keyboard.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -O1 -DSD_DEBUG -MMD -MF "${OBJECTDIR}/hardware/keyboard.o.d" -o ${OBJECTDIR}/hardware/keyboard.o hardware/keyboard.c   
	
${OBJECTDIR}/hardware/Pinguino/sdmmc.o: hardware/Pinguino/sdmmc.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/hardware/Pinguino 
	@${RM} ${OBJECTDIR}/hardware/Pinguino/sdmmc.o.d 
	@${RM} ${OBJECTDIR}/hardware/Pinguino/sdmmc.o 
	@${FIXDEPS} "${OBJECTDIR}/hardware/Pinguino/sdmmc.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -O1 -DSD_DEBUG -MMD -MF "${OBJECTDIR}/hardware/Pinguino/sdmmc.o.d" -o ${OBJECTDIR}/hardware/Pinguino/sdmmc.o hardware/Pinguino/sdmmc.c   
	
${OBJECTDIR}/hardware/Pinguino/diskio.o: hardware/Pinguino/diskio.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/hardware/Pinguino 
	@${RM} ${OBJECTDIR}/hardware/Pinguino/diskio.o.d 
	@${RM} ${OBJECTDIR}/hardware/Pinguino/diskio.o 
	@${FIXDEPS} "${OBJECTDIR}/hardware/Pinguino/diskio.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -O1 -DSD_DEBUG -MMD -MF "${OBJECTDIR}/hardware/Pinguino/diskio.o.d" -o ${OBJECTDIR}/hardware/Pinguino/diskio.o hardware/Pinguino/diskio.c   
	
${OBJECTDIR}/hardware/QWERTY.o: hardware/QWERTY.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/hardware 
	@${RM} ${OBJECTDIR}/hardware/QWERTY.o.d 
	@${RM} ${OBJECTDIR}/hardware/QWERTY.o 
	@${FIXDEPS} "${OBJECTDIR}/hardware/QWERTY.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -O1 -DSD_DEBUG -MMD -MF "${OBJECTDIR}/hardware/QWERTY.o.d" -o ${OBJECTDIR}/hardware/QWERTY.o hardware/QWERTY.c   
	
${OBJECTDIR}/console.o: console.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/console.o.d 
	@${RM} ${OBJECTDIR}/console.o 
	@${FIXDEPS} "${OBJECTDIR}/console.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -O1 -DSD_DEBUG -MMD -MF "${OBJECTDIR}/console.o.d" -o ${OBJECTDIR}/console.o console.c   
	
${OBJECTDIR}/font.o: font.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/font.o.d 
	@${RM} ${OBJECTDIR}/font.o 
	@${FIXDEPS} "${OBJECTDIR}/font.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -O1 -DSD_DEBUG -MMD -MF "${OBJECTDIR}/font.o.d" -o ${OBJECTDIR}/font.o font.c   
	
${OBJECTDIR}/hardware/Pinguino/fileio.o: hardware/Pinguino/fileio.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/hardware/Pinguino 
	@${RM} ${OBJECTDIR}/hardware/Pinguino/fileio.o.d 
	@${RM} ${OBJECTDIR}/hardware/Pinguino/fileio.o 
	@${FIXDEPS} "${OBJECTDIR}/hardware/Pinguino/fileio.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -O1 -DSD_DEBUG -MMD -MF "${OBJECTDIR}/hardware/Pinguino/fileio.o.d" -o ${OBJECTDIR}/hardware/Pinguino/fileio.o hardware/Pinguino/fileio.c   
	
${OBJECTDIR}/hardware/Pinguino/ff.o: hardware/Pinguino/ff.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/hardware/Pinguino 
	@${RM} ${OBJECTDIR}/hardware/Pinguino/ff.o.d 
	@${RM} ${OBJECTDIR}/hardware/Pinguino/ff.o 
	@${FIXDEPS} "${OBJECTDIR}/hardware/Pinguino/ff.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -O1 -DSD_DEBUG -MMD -MF "${OBJECTDIR}/hardware/Pinguino/ff.o.d" -o ${OBJECTDIR}/hardware/Pinguino/ff.o hardware/Pinguino/ff.c   
	
${OBJECTDIR}/sound.o: sound.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/sound.o.d 
	@${RM} ${OBJECTDIR}/sound.o 
	@${FIXDEPS} "${OBJECTDIR}/sound.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -O1 -DSD_DEBUG -MMD -MF "${OBJECTDIR}/sound.o.d" -o ${OBJECTDIR}/sound.o sound.c   
	
${OBJECTDIR}/editor.o: editor.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/editor.o.d 
	@${RM} ${OBJECTDIR}/editor.o 
	@${FIXDEPS} "${OBJECTDIR}/editor.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -O1 -DSD_DEBUG -MMD -MF "${OBJECTDIR}/editor.o.d" -o ${OBJECTDIR}/editor.o editor.c   
	
${OBJECTDIR}/shell.o: shell.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/shell.o.d 
	@${RM} ${OBJECTDIR}/shell.o 
	@${FIXDEPS} "${OBJECTDIR}/shell.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -O1 -DSD_DEBUG -MMD -MF "${OBJECTDIR}/shell.o.d" -o ${OBJECTDIR}/shell.o shell.c   
	
${OBJECTDIR}/vpc-32.o: vpc-32.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/vpc-32.o.d 
	@${RM} ${OBJECTDIR}/vpc-32.o 
	@${FIXDEPS} "${OBJECTDIR}/vpc-32.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -O1 -DSD_DEBUG -MMD -MF "${OBJECTDIR}/vpc-32.o.d" -o ${OBJECTDIR}/vpc-32.o vpc-32.c   
	
${OBJECTDIR}/graphics.o: graphics.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/graphics.o.d 
	@${RM} ${OBJECTDIR}/graphics.o 
	@${FIXDEPS} "${OBJECTDIR}/graphics.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -O1 -DSD_DEBUG -MMD -MF "${OBJECTDIR}/graphics.o.d" -o ${OBJECTDIR}/graphics.o graphics.c   
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compileCPP
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
dist/${CND_CONF}/${IMAGE_TYPE}/VPC-32.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -mdebugger -D__MPLAB_DEBUGGER_PK3=1 -mprocessor=$(MP_PROCESSOR_OPTION)  -o dist/${CND_CONF}/${IMAGE_TYPE}/VPC-32.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}          -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--defsym=__ICD2RAM=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,--defsym=__MPLAB_DEBUGGER_PK3=1,--defsym=_min_heap_size=16384,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--report-mem
	
else
dist/${CND_CONF}/${IMAGE_TYPE}/VPC-32.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -mprocessor=$(MP_PROCESSOR_OPTION)  -o dist/${CND_CONF}/${IMAGE_TYPE}/VPC-32.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}          -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--defsym=_min_heap_size=16384,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--report-mem
	${MP_CC_DIR}\\xc32-bin2hex dist/${CND_CONF}/${IMAGE_TYPE}/VPC-32.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} 
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

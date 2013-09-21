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
SOURCEFILES_QUOTED_IF_SPACED=hardware/ntsc.c hardware/HardwareProfile.c hardware/serial_comm.c hardware/keyboard.c hardware/Pinguino/sdmmc.c hardware/Pinguino/diskio.c hardware/QWERTY.c vpForth/vpForth.c vpForth/vm.S vpc-32.c console.c font.c hardware/Pinguino/fileio.c hardware/Pinguino/ff.c sound.c editor.c shell.c as.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/hardware/ntsc.o ${OBJECTDIR}/hardware/HardwareProfile.o ${OBJECTDIR}/hardware/serial_comm.o ${OBJECTDIR}/hardware/keyboard.o ${OBJECTDIR}/hardware/Pinguino/sdmmc.o ${OBJECTDIR}/hardware/Pinguino/diskio.o ${OBJECTDIR}/hardware/QWERTY.o ${OBJECTDIR}/vpForth/vpForth.o ${OBJECTDIR}/vpForth/vm.o ${OBJECTDIR}/vpc-32.o ${OBJECTDIR}/console.o ${OBJECTDIR}/font.o ${OBJECTDIR}/hardware/Pinguino/fileio.o ${OBJECTDIR}/hardware/Pinguino/ff.o ${OBJECTDIR}/sound.o ${OBJECTDIR}/editor.o ${OBJECTDIR}/shell.o ${OBJECTDIR}/as.o
POSSIBLE_DEPFILES=${OBJECTDIR}/hardware/ntsc.o.d ${OBJECTDIR}/hardware/HardwareProfile.o.d ${OBJECTDIR}/hardware/serial_comm.o.d ${OBJECTDIR}/hardware/keyboard.o.d ${OBJECTDIR}/hardware/Pinguino/sdmmc.o.d ${OBJECTDIR}/hardware/Pinguino/diskio.o.d ${OBJECTDIR}/hardware/QWERTY.o.d ${OBJECTDIR}/vpForth/vpForth.o.d ${OBJECTDIR}/vpForth/vm.o.d ${OBJECTDIR}/vpc-32.o.d ${OBJECTDIR}/console.o.d ${OBJECTDIR}/font.o.d ${OBJECTDIR}/hardware/Pinguino/fileio.o.d ${OBJECTDIR}/hardware/Pinguino/ff.o.d ${OBJECTDIR}/sound.o.d ${OBJECTDIR}/editor.o.d ${OBJECTDIR}/shell.o.d ${OBJECTDIR}/as.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/hardware/ntsc.o ${OBJECTDIR}/hardware/HardwareProfile.o ${OBJECTDIR}/hardware/serial_comm.o ${OBJECTDIR}/hardware/keyboard.o ${OBJECTDIR}/hardware/Pinguino/sdmmc.o ${OBJECTDIR}/hardware/Pinguino/diskio.o ${OBJECTDIR}/hardware/QWERTY.o ${OBJECTDIR}/vpForth/vpForth.o ${OBJECTDIR}/vpForth/vm.o ${OBJECTDIR}/vpc-32.o ${OBJECTDIR}/console.o ${OBJECTDIR}/font.o ${OBJECTDIR}/hardware/Pinguino/fileio.o ${OBJECTDIR}/hardware/Pinguino/ff.o ${OBJECTDIR}/sound.o ${OBJECTDIR}/editor.o ${OBJECTDIR}/shell.o ${OBJECTDIR}/as.o

# Source Files
SOURCEFILES=hardware/ntsc.c hardware/HardwareProfile.c hardware/serial_comm.c hardware/keyboard.c hardware/Pinguino/sdmmc.c hardware/Pinguino/diskio.c hardware/QWERTY.c vpForth/vpForth.c vpForth/vm.S vpc-32.c console.c font.c hardware/Pinguino/fileio.c hardware/Pinguino/ff.c sound.c editor.c shell.c as.c


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
${OBJECTDIR}/vpForth/vm.o: vpForth/vm.S  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/vpForth 
	@${RM} ${OBJECTDIR}/vpForth/vm.o.d 
	@${RM} ${OBJECTDIR}/vpForth/vm.o.ok ${OBJECTDIR}/vpForth/vm.o.err 
	@${FIXDEPS} "${OBJECTDIR}/vpForth/vm.o.d" "${OBJECTDIR}/vpForth/vm.o.asm.d" -t $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC} $(MP_EXTRA_AS_PRE)  -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/vpForth/vm.o.d"  -o ${OBJECTDIR}/vpForth/vm.o vpForth/vm.S  -Wa,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_AS_POST),-MD="${OBJECTDIR}/vpForth/vm.o.asm.d",--defsym=__ICD2RAM=1,--defsym=__MPLAB_DEBUG=1,--gdwarf-2,--defsym=__DEBUG=1,--defsym=__MPLAB_DEBUGGER_PK3=1
	
else
${OBJECTDIR}/vpForth/vm.o: vpForth/vm.S  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/vpForth 
	@${RM} ${OBJECTDIR}/vpForth/vm.o.d 
	@${RM} ${OBJECTDIR}/vpForth/vm.o.ok ${OBJECTDIR}/vpForth/vm.o.err 
	@${FIXDEPS} "${OBJECTDIR}/vpForth/vm.o.d" "${OBJECTDIR}/vpForth/vm.o.asm.d" -t $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC} $(MP_EXTRA_AS_PRE)  -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/vpForth/vm.o.d"  -o ${OBJECTDIR}/vpForth/vm.o vpForth/vm.S  -Wa,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_AS_POST),-MD="${OBJECTDIR}/vpForth/vm.o.asm.d",--gdwarf-2
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/hardware/ntsc.o: hardware/ntsc.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/hardware 
	@${RM} ${OBJECTDIR}/hardware/ntsc.o.d 
	@${FIXDEPS} "${OBJECTDIR}/hardware/ntsc.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -DSD_DEBUG -O1 -MMD -MF "${OBJECTDIR}/hardware/ntsc.o.d" -o ${OBJECTDIR}/hardware/ntsc.o hardware/ntsc.c   
	
${OBJECTDIR}/hardware/HardwareProfile.o: hardware/HardwareProfile.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/hardware 
	@${RM} ${OBJECTDIR}/hardware/HardwareProfile.o.d 
	@${FIXDEPS} "${OBJECTDIR}/hardware/HardwareProfile.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -DSD_DEBUG -O1 -MMD -MF "${OBJECTDIR}/hardware/HardwareProfile.o.d" -o ${OBJECTDIR}/hardware/HardwareProfile.o hardware/HardwareProfile.c   
	
${OBJECTDIR}/hardware/serial_comm.o: hardware/serial_comm.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/hardware 
	@${RM} ${OBJECTDIR}/hardware/serial_comm.o.d 
	@${FIXDEPS} "${OBJECTDIR}/hardware/serial_comm.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -DSD_DEBUG -O1 -MMD -MF "${OBJECTDIR}/hardware/serial_comm.o.d" -o ${OBJECTDIR}/hardware/serial_comm.o hardware/serial_comm.c   
	
${OBJECTDIR}/hardware/keyboard.o: hardware/keyboard.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/hardware 
	@${RM} ${OBJECTDIR}/hardware/keyboard.o.d 
	@${FIXDEPS} "${OBJECTDIR}/hardware/keyboard.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -DSD_DEBUG -O1 -MMD -MF "${OBJECTDIR}/hardware/keyboard.o.d" -o ${OBJECTDIR}/hardware/keyboard.o hardware/keyboard.c   
	
${OBJECTDIR}/hardware/Pinguino/sdmmc.o: hardware/Pinguino/sdmmc.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/hardware/Pinguino 
	@${RM} ${OBJECTDIR}/hardware/Pinguino/sdmmc.o.d 
	@${FIXDEPS} "${OBJECTDIR}/hardware/Pinguino/sdmmc.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -DSD_DEBUG -O1 -MMD -MF "${OBJECTDIR}/hardware/Pinguino/sdmmc.o.d" -o ${OBJECTDIR}/hardware/Pinguino/sdmmc.o hardware/Pinguino/sdmmc.c   
	
${OBJECTDIR}/hardware/Pinguino/diskio.o: hardware/Pinguino/diskio.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/hardware/Pinguino 
	@${RM} ${OBJECTDIR}/hardware/Pinguino/diskio.o.d 
	@${FIXDEPS} "${OBJECTDIR}/hardware/Pinguino/diskio.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -DSD_DEBUG -O1 -MMD -MF "${OBJECTDIR}/hardware/Pinguino/diskio.o.d" -o ${OBJECTDIR}/hardware/Pinguino/diskio.o hardware/Pinguino/diskio.c   
	
${OBJECTDIR}/hardware/QWERTY.o: hardware/QWERTY.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/hardware 
	@${RM} ${OBJECTDIR}/hardware/QWERTY.o.d 
	@${FIXDEPS} "${OBJECTDIR}/hardware/QWERTY.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -DSD_DEBUG -O1 -MMD -MF "${OBJECTDIR}/hardware/QWERTY.o.d" -o ${OBJECTDIR}/hardware/QWERTY.o hardware/QWERTY.c   
	
${OBJECTDIR}/vpForth/vpForth.o: vpForth/vpForth.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/vpForth 
	@${RM} ${OBJECTDIR}/vpForth/vpForth.o.d 
	@${FIXDEPS} "${OBJECTDIR}/vpForth/vpForth.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -DSD_DEBUG -O1 -MMD -MF "${OBJECTDIR}/vpForth/vpForth.o.d" -o ${OBJECTDIR}/vpForth/vpForth.o vpForth/vpForth.c   
	
${OBJECTDIR}/vpc-32.o: vpc-32.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/vpc-32.o.d 
	@${FIXDEPS} "${OBJECTDIR}/vpc-32.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -DSD_DEBUG -O1 -MMD -MF "${OBJECTDIR}/vpc-32.o.d" -o ${OBJECTDIR}/vpc-32.o vpc-32.c   
	
${OBJECTDIR}/console.o: console.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/console.o.d 
	@${FIXDEPS} "${OBJECTDIR}/console.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -DSD_DEBUG -O1 -MMD -MF "${OBJECTDIR}/console.o.d" -o ${OBJECTDIR}/console.o console.c   
	
${OBJECTDIR}/font.o: font.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/font.o.d 
	@${FIXDEPS} "${OBJECTDIR}/font.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -DSD_DEBUG -O1 -MMD -MF "${OBJECTDIR}/font.o.d" -o ${OBJECTDIR}/font.o font.c   
	
${OBJECTDIR}/hardware/Pinguino/fileio.o: hardware/Pinguino/fileio.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/hardware/Pinguino 
	@${RM} ${OBJECTDIR}/hardware/Pinguino/fileio.o.d 
	@${FIXDEPS} "${OBJECTDIR}/hardware/Pinguino/fileio.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -DSD_DEBUG -O1 -MMD -MF "${OBJECTDIR}/hardware/Pinguino/fileio.o.d" -o ${OBJECTDIR}/hardware/Pinguino/fileio.o hardware/Pinguino/fileio.c   
	
${OBJECTDIR}/hardware/Pinguino/ff.o: hardware/Pinguino/ff.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/hardware/Pinguino 
	@${RM} ${OBJECTDIR}/hardware/Pinguino/ff.o.d 
	@${FIXDEPS} "${OBJECTDIR}/hardware/Pinguino/ff.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -DSD_DEBUG -O1 -MMD -MF "${OBJECTDIR}/hardware/Pinguino/ff.o.d" -o ${OBJECTDIR}/hardware/Pinguino/ff.o hardware/Pinguino/ff.c   
	
${OBJECTDIR}/sound.o: sound.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/sound.o.d 
	@${FIXDEPS} "${OBJECTDIR}/sound.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -DSD_DEBUG -O1 -MMD -MF "${OBJECTDIR}/sound.o.d" -o ${OBJECTDIR}/sound.o sound.c   
	
${OBJECTDIR}/editor.o: editor.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/editor.o.d 
	@${FIXDEPS} "${OBJECTDIR}/editor.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -DSD_DEBUG -O1 -MMD -MF "${OBJECTDIR}/editor.o.d" -o ${OBJECTDIR}/editor.o editor.c   
	
${OBJECTDIR}/shell.o: shell.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/shell.o.d 
	@${FIXDEPS} "${OBJECTDIR}/shell.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -DSD_DEBUG -O1 -MMD -MF "${OBJECTDIR}/shell.o.d" -o ${OBJECTDIR}/shell.o shell.c   
	
${OBJECTDIR}/as.o: as.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/as.o.d 
	@${FIXDEPS} "${OBJECTDIR}/as.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -DSD_DEBUG -O1 -MMD -MF "${OBJECTDIR}/as.o.d" -o ${OBJECTDIR}/as.o as.c   
	
else
${OBJECTDIR}/hardware/ntsc.o: hardware/ntsc.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/hardware 
	@${RM} ${OBJECTDIR}/hardware/ntsc.o.d 
	@${FIXDEPS} "${OBJECTDIR}/hardware/ntsc.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -DSD_DEBUG -O1 -MMD -MF "${OBJECTDIR}/hardware/ntsc.o.d" -o ${OBJECTDIR}/hardware/ntsc.o hardware/ntsc.c   
	
${OBJECTDIR}/hardware/HardwareProfile.o: hardware/HardwareProfile.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/hardware 
	@${RM} ${OBJECTDIR}/hardware/HardwareProfile.o.d 
	@${FIXDEPS} "${OBJECTDIR}/hardware/HardwareProfile.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -DSD_DEBUG -O1 -MMD -MF "${OBJECTDIR}/hardware/HardwareProfile.o.d" -o ${OBJECTDIR}/hardware/HardwareProfile.o hardware/HardwareProfile.c   
	
${OBJECTDIR}/hardware/serial_comm.o: hardware/serial_comm.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/hardware 
	@${RM} ${OBJECTDIR}/hardware/serial_comm.o.d 
	@${FIXDEPS} "${OBJECTDIR}/hardware/serial_comm.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -DSD_DEBUG -O1 -MMD -MF "${OBJECTDIR}/hardware/serial_comm.o.d" -o ${OBJECTDIR}/hardware/serial_comm.o hardware/serial_comm.c   
	
${OBJECTDIR}/hardware/keyboard.o: hardware/keyboard.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/hardware 
	@${RM} ${OBJECTDIR}/hardware/keyboard.o.d 
	@${FIXDEPS} "${OBJECTDIR}/hardware/keyboard.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -DSD_DEBUG -O1 -MMD -MF "${OBJECTDIR}/hardware/keyboard.o.d" -o ${OBJECTDIR}/hardware/keyboard.o hardware/keyboard.c   
	
${OBJECTDIR}/hardware/Pinguino/sdmmc.o: hardware/Pinguino/sdmmc.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/hardware/Pinguino 
	@${RM} ${OBJECTDIR}/hardware/Pinguino/sdmmc.o.d 
	@${FIXDEPS} "${OBJECTDIR}/hardware/Pinguino/sdmmc.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -DSD_DEBUG -O1 -MMD -MF "${OBJECTDIR}/hardware/Pinguino/sdmmc.o.d" -o ${OBJECTDIR}/hardware/Pinguino/sdmmc.o hardware/Pinguino/sdmmc.c   
	
${OBJECTDIR}/hardware/Pinguino/diskio.o: hardware/Pinguino/diskio.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/hardware/Pinguino 
	@${RM} ${OBJECTDIR}/hardware/Pinguino/diskio.o.d 
	@${FIXDEPS} "${OBJECTDIR}/hardware/Pinguino/diskio.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -DSD_DEBUG -O1 -MMD -MF "${OBJECTDIR}/hardware/Pinguino/diskio.o.d" -o ${OBJECTDIR}/hardware/Pinguino/diskio.o hardware/Pinguino/diskio.c   
	
${OBJECTDIR}/hardware/QWERTY.o: hardware/QWERTY.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/hardware 
	@${RM} ${OBJECTDIR}/hardware/QWERTY.o.d 
	@${FIXDEPS} "${OBJECTDIR}/hardware/QWERTY.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -DSD_DEBUG -O1 -MMD -MF "${OBJECTDIR}/hardware/QWERTY.o.d" -o ${OBJECTDIR}/hardware/QWERTY.o hardware/QWERTY.c   
	
${OBJECTDIR}/vpForth/vpForth.o: vpForth/vpForth.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/vpForth 
	@${RM} ${OBJECTDIR}/vpForth/vpForth.o.d 
	@${FIXDEPS} "${OBJECTDIR}/vpForth/vpForth.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -DSD_DEBUG -O1 -MMD -MF "${OBJECTDIR}/vpForth/vpForth.o.d" -o ${OBJECTDIR}/vpForth/vpForth.o vpForth/vpForth.c   
	
${OBJECTDIR}/vpc-32.o: vpc-32.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/vpc-32.o.d 
	@${FIXDEPS} "${OBJECTDIR}/vpc-32.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -DSD_DEBUG -O1 -MMD -MF "${OBJECTDIR}/vpc-32.o.d" -o ${OBJECTDIR}/vpc-32.o vpc-32.c   
	
${OBJECTDIR}/console.o: console.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/console.o.d 
	@${FIXDEPS} "${OBJECTDIR}/console.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -DSD_DEBUG -O1 -MMD -MF "${OBJECTDIR}/console.o.d" -o ${OBJECTDIR}/console.o console.c   
	
${OBJECTDIR}/font.o: font.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/font.o.d 
	@${FIXDEPS} "${OBJECTDIR}/font.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -DSD_DEBUG -O1 -MMD -MF "${OBJECTDIR}/font.o.d" -o ${OBJECTDIR}/font.o font.c   
	
${OBJECTDIR}/hardware/Pinguino/fileio.o: hardware/Pinguino/fileio.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/hardware/Pinguino 
	@${RM} ${OBJECTDIR}/hardware/Pinguino/fileio.o.d 
	@${FIXDEPS} "${OBJECTDIR}/hardware/Pinguino/fileio.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -DSD_DEBUG -O1 -MMD -MF "${OBJECTDIR}/hardware/Pinguino/fileio.o.d" -o ${OBJECTDIR}/hardware/Pinguino/fileio.o hardware/Pinguino/fileio.c   
	
${OBJECTDIR}/hardware/Pinguino/ff.o: hardware/Pinguino/ff.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/hardware/Pinguino 
	@${RM} ${OBJECTDIR}/hardware/Pinguino/ff.o.d 
	@${FIXDEPS} "${OBJECTDIR}/hardware/Pinguino/ff.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -DSD_DEBUG -O1 -MMD -MF "${OBJECTDIR}/hardware/Pinguino/ff.o.d" -o ${OBJECTDIR}/hardware/Pinguino/ff.o hardware/Pinguino/ff.c   
	
${OBJECTDIR}/sound.o: sound.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/sound.o.d 
	@${FIXDEPS} "${OBJECTDIR}/sound.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -DSD_DEBUG -O1 -MMD -MF "${OBJECTDIR}/sound.o.d" -o ${OBJECTDIR}/sound.o sound.c   
	
${OBJECTDIR}/editor.o: editor.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/editor.o.d 
	@${FIXDEPS} "${OBJECTDIR}/editor.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -DSD_DEBUG -O1 -MMD -MF "${OBJECTDIR}/editor.o.d" -o ${OBJECTDIR}/editor.o editor.c   
	
${OBJECTDIR}/shell.o: shell.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/shell.o.d 
	@${FIXDEPS} "${OBJECTDIR}/shell.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -DSD_DEBUG -O1 -MMD -MF "${OBJECTDIR}/shell.o.d" -o ${OBJECTDIR}/shell.o shell.c   
	
${OBJECTDIR}/as.o: as.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/as.o.d 
	@${FIXDEPS} "${OBJECTDIR}/as.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -DSD_DEBUG -O1 -MMD -MF "${OBJECTDIR}/as.o.d" -o ${OBJECTDIR}/as.o as.c   
	
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
	${MP_CC} $(MP_EXTRA_LD_PRE)  -mdebugger -D__MPLAB_DEBUGGER_PK3=1 -mprocessor=$(MP_PROCESSOR_OPTION)  -o dist/${CND_CONF}/${IMAGE_TYPE}/VPC-32.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}          -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--defsym=__ICD2RAM=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,--defsym=__MPLAB_DEBUGGER_PK3=1,--defsym=_min_heap_size=4096,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--report-mem
	
else
dist/${CND_CONF}/${IMAGE_TYPE}/VPC-32.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -mprocessor=$(MP_PROCESSOR_OPTION)  -o dist/${CND_CONF}/${IMAGE_TYPE}/VPC-32.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}          -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--defsym=_min_heap_size=4096,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--report-mem
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

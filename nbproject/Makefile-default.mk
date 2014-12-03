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
MKDIR=mkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=default
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=elf
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/vpc-32.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/vpc-32.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=hardware/ps2_kbd/QWERTY.c hardware/ps2_kbd/keyboard.c hardware/Pinguino/sdmmc.c hardware/Pinguino/diskio.c hardware/serial_comm/serial_comm.c hardware/sound/sound.c hardware/spiram/spiram.c hardware/store/store_spi.c hardware/tvout/ntsc.c hardware/HardwareProfile.c console.c font.c hardware/Pinguino/fileio.c hardware/Pinguino/ff.c editor.c shell.c vpc-32.c graphics.c vpcForth/TinyForth.c vpcForth/vm.S

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/hardware/ps2_kbd/QWERTY.o ${OBJECTDIR}/hardware/ps2_kbd/keyboard.o ${OBJECTDIR}/hardware/Pinguino/sdmmc.o ${OBJECTDIR}/hardware/Pinguino/diskio.o ${OBJECTDIR}/hardware/serial_comm/serial_comm.o ${OBJECTDIR}/hardware/sound/sound.o ${OBJECTDIR}/hardware/spiram/spiram.o ${OBJECTDIR}/hardware/store/store_spi.o ${OBJECTDIR}/hardware/tvout/ntsc.o ${OBJECTDIR}/hardware/HardwareProfile.o ${OBJECTDIR}/console.o ${OBJECTDIR}/font.o ${OBJECTDIR}/hardware/Pinguino/fileio.o ${OBJECTDIR}/hardware/Pinguino/ff.o ${OBJECTDIR}/editor.o ${OBJECTDIR}/shell.o ${OBJECTDIR}/vpc-32.o ${OBJECTDIR}/graphics.o ${OBJECTDIR}/vpcForth/TinyForth.o ${OBJECTDIR}/vpcForth/vm.o
POSSIBLE_DEPFILES=${OBJECTDIR}/hardware/ps2_kbd/QWERTY.o.d ${OBJECTDIR}/hardware/ps2_kbd/keyboard.o.d ${OBJECTDIR}/hardware/Pinguino/sdmmc.o.d ${OBJECTDIR}/hardware/Pinguino/diskio.o.d ${OBJECTDIR}/hardware/serial_comm/serial_comm.o.d ${OBJECTDIR}/hardware/sound/sound.o.d ${OBJECTDIR}/hardware/spiram/spiram.o.d ${OBJECTDIR}/hardware/store/store_spi.o.d ${OBJECTDIR}/hardware/tvout/ntsc.o.d ${OBJECTDIR}/hardware/HardwareProfile.o.d ${OBJECTDIR}/console.o.d ${OBJECTDIR}/font.o.d ${OBJECTDIR}/hardware/Pinguino/fileio.o.d ${OBJECTDIR}/hardware/Pinguino/ff.o.d ${OBJECTDIR}/editor.o.d ${OBJECTDIR}/shell.o.d ${OBJECTDIR}/vpc-32.o.d ${OBJECTDIR}/graphics.o.d ${OBJECTDIR}/vpcForth/TinyForth.o.d ${OBJECTDIR}/vpcForth/vm.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/hardware/ps2_kbd/QWERTY.o ${OBJECTDIR}/hardware/ps2_kbd/keyboard.o ${OBJECTDIR}/hardware/Pinguino/sdmmc.o ${OBJECTDIR}/hardware/Pinguino/diskio.o ${OBJECTDIR}/hardware/serial_comm/serial_comm.o ${OBJECTDIR}/hardware/sound/sound.o ${OBJECTDIR}/hardware/spiram/spiram.o ${OBJECTDIR}/hardware/store/store_spi.o ${OBJECTDIR}/hardware/tvout/ntsc.o ${OBJECTDIR}/hardware/HardwareProfile.o ${OBJECTDIR}/console.o ${OBJECTDIR}/font.o ${OBJECTDIR}/hardware/Pinguino/fileio.o ${OBJECTDIR}/hardware/Pinguino/ff.o ${OBJECTDIR}/editor.o ${OBJECTDIR}/shell.o ${OBJECTDIR}/vpc-32.o ${OBJECTDIR}/graphics.o ${OBJECTDIR}/vpcForth/TinyForth.o ${OBJECTDIR}/vpcForth/vm.o

# Source Files
SOURCEFILES=hardware/ps2_kbd/QWERTY.c hardware/ps2_kbd/keyboard.c hardware/Pinguino/sdmmc.c hardware/Pinguino/diskio.c hardware/serial_comm/serial_comm.c hardware/sound/sound.c hardware/spiram/spiram.c hardware/store/store_spi.c hardware/tvout/ntsc.c hardware/HardwareProfile.c console.c font.c hardware/Pinguino/fileio.c hardware/Pinguino/ff.c editor.c shell.c vpc-32.c graphics.c vpcForth/TinyForth.c vpcForth/vm.S


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
	${MAKE}  -f nbproject/Makefile-default.mk dist/${CND_CONF}/${IMAGE_TYPE}/vpc-32.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=32MX170F256B
MP_LINKER_FILE_OPTION=
# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assembleWithPreprocess
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/vpcForth/vm.o: vpcForth/vm.S  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/vpcForth 
	@${RM} ${OBJECTDIR}/vpcForth/vm.o.d 
	@${RM} ${OBJECTDIR}/vpcForth/vm.o 
	@${RM} ${OBJECTDIR}/vpcForth/vm.o.ok ${OBJECTDIR}/vpcForth/vm.o.err 
	@${FIXDEPS} "${OBJECTDIR}/vpcForth/vm.o.d" "${OBJECTDIR}/vpcForth/vm.o.asm.d" -t $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC} $(MP_EXTRA_AS_PRE)  -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/vpcForth/vm.o.d"  -o ${OBJECTDIR}/vpcForth/vm.o vpcForth/vm.S  -Wa,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_AS_POST),-MD="${OBJECTDIR}/vpcForth/vm.o.asm.d",--defsym=__ICD2RAM=1,--defsym=__MPLAB_DEBUG=1,--gdwarf-2,--defsym=__DEBUG=1,--defsym=__MPLAB_DEBUGGER_PK3=1
	
else
${OBJECTDIR}/vpcForth/vm.o: vpcForth/vm.S  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/vpcForth 
	@${RM} ${OBJECTDIR}/vpcForth/vm.o.d 
	@${RM} ${OBJECTDIR}/vpcForth/vm.o 
	@${RM} ${OBJECTDIR}/vpcForth/vm.o.ok ${OBJECTDIR}/vpcForth/vm.o.err 
	@${FIXDEPS} "${OBJECTDIR}/vpcForth/vm.o.d" "${OBJECTDIR}/vpcForth/vm.o.asm.d" -t $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC} $(MP_EXTRA_AS_PRE)  -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/vpcForth/vm.o.d"  -o ${OBJECTDIR}/vpcForth/vm.o vpcForth/vm.S  -Wa,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_AS_POST),-MD="${OBJECTDIR}/vpcForth/vm.o.asm.d",--gdwarf-2
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/hardware/ps2_kbd/QWERTY.o: hardware/ps2_kbd/QWERTY.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/hardware/ps2_kbd 
	@${RM} ${OBJECTDIR}/hardware/ps2_kbd/QWERTY.o.d 
	@${RM} ${OBJECTDIR}/hardware/ps2_kbd/QWERTY.o 
	@${FIXDEPS} "${OBJECTDIR}/hardware/ps2_kbd/QWERTY.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -D_DEBUG_ -MMD -MF "${OBJECTDIR}/hardware/ps2_kbd/QWERTY.o.d" -o ${OBJECTDIR}/hardware/ps2_kbd/QWERTY.o hardware/ps2_kbd/QWERTY.c    -D_SUPPRESS_PLIB_WARNING
	
${OBJECTDIR}/hardware/ps2_kbd/keyboard.o: hardware/ps2_kbd/keyboard.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/hardware/ps2_kbd 
	@${RM} ${OBJECTDIR}/hardware/ps2_kbd/keyboard.o.d 
	@${RM} ${OBJECTDIR}/hardware/ps2_kbd/keyboard.o 
	@${FIXDEPS} "${OBJECTDIR}/hardware/ps2_kbd/keyboard.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -D_DEBUG_ -MMD -MF "${OBJECTDIR}/hardware/ps2_kbd/keyboard.o.d" -o ${OBJECTDIR}/hardware/ps2_kbd/keyboard.o hardware/ps2_kbd/keyboard.c    -D_SUPPRESS_PLIB_WARNING
	
${OBJECTDIR}/hardware/Pinguino/sdmmc.o: hardware/Pinguino/sdmmc.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/hardware/Pinguino 
	@${RM} ${OBJECTDIR}/hardware/Pinguino/sdmmc.o.d 
	@${RM} ${OBJECTDIR}/hardware/Pinguino/sdmmc.o 
	@${FIXDEPS} "${OBJECTDIR}/hardware/Pinguino/sdmmc.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -D_DEBUG_ -MMD -MF "${OBJECTDIR}/hardware/Pinguino/sdmmc.o.d" -o ${OBJECTDIR}/hardware/Pinguino/sdmmc.o hardware/Pinguino/sdmmc.c    -D_SUPPRESS_PLIB_WARNING
	
${OBJECTDIR}/hardware/Pinguino/diskio.o: hardware/Pinguino/diskio.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/hardware/Pinguino 
	@${RM} ${OBJECTDIR}/hardware/Pinguino/diskio.o.d 
	@${RM} ${OBJECTDIR}/hardware/Pinguino/diskio.o 
	@${FIXDEPS} "${OBJECTDIR}/hardware/Pinguino/diskio.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -D_DEBUG_ -MMD -MF "${OBJECTDIR}/hardware/Pinguino/diskio.o.d" -o ${OBJECTDIR}/hardware/Pinguino/diskio.o hardware/Pinguino/diskio.c    -D_SUPPRESS_PLIB_WARNING
	
${OBJECTDIR}/hardware/serial_comm/serial_comm.o: hardware/serial_comm/serial_comm.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/hardware/serial_comm 
	@${RM} ${OBJECTDIR}/hardware/serial_comm/serial_comm.o.d 
	@${RM} ${OBJECTDIR}/hardware/serial_comm/serial_comm.o 
	@${FIXDEPS} "${OBJECTDIR}/hardware/serial_comm/serial_comm.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -D_DEBUG_ -MMD -MF "${OBJECTDIR}/hardware/serial_comm/serial_comm.o.d" -o ${OBJECTDIR}/hardware/serial_comm/serial_comm.o hardware/serial_comm/serial_comm.c    -D_SUPPRESS_PLIB_WARNING
	
${OBJECTDIR}/hardware/sound/sound.o: hardware/sound/sound.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/hardware/sound 
	@${RM} ${OBJECTDIR}/hardware/sound/sound.o.d 
	@${RM} ${OBJECTDIR}/hardware/sound/sound.o 
	@${FIXDEPS} "${OBJECTDIR}/hardware/sound/sound.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -D_DEBUG_ -MMD -MF "${OBJECTDIR}/hardware/sound/sound.o.d" -o ${OBJECTDIR}/hardware/sound/sound.o hardware/sound/sound.c    -D_SUPPRESS_PLIB_WARNING
	
${OBJECTDIR}/hardware/spiram/spiram.o: hardware/spiram/spiram.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/hardware/spiram 
	@${RM} ${OBJECTDIR}/hardware/spiram/spiram.o.d 
	@${RM} ${OBJECTDIR}/hardware/spiram/spiram.o 
	@${FIXDEPS} "${OBJECTDIR}/hardware/spiram/spiram.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -D_DEBUG_ -MMD -MF "${OBJECTDIR}/hardware/spiram/spiram.o.d" -o ${OBJECTDIR}/hardware/spiram/spiram.o hardware/spiram/spiram.c    -D_SUPPRESS_PLIB_WARNING
	
${OBJECTDIR}/hardware/store/store_spi.o: hardware/store/store_spi.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/hardware/store 
	@${RM} ${OBJECTDIR}/hardware/store/store_spi.o.d 
	@${RM} ${OBJECTDIR}/hardware/store/store_spi.o 
	@${FIXDEPS} "${OBJECTDIR}/hardware/store/store_spi.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -D_DEBUG_ -MMD -MF "${OBJECTDIR}/hardware/store/store_spi.o.d" -o ${OBJECTDIR}/hardware/store/store_spi.o hardware/store/store_spi.c    -D_SUPPRESS_PLIB_WARNING
	
${OBJECTDIR}/hardware/tvout/ntsc.o: hardware/tvout/ntsc.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/hardware/tvout 
	@${RM} ${OBJECTDIR}/hardware/tvout/ntsc.o.d 
	@${RM} ${OBJECTDIR}/hardware/tvout/ntsc.o 
	@${FIXDEPS} "${OBJECTDIR}/hardware/tvout/ntsc.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -D_DEBUG_ -MMD -MF "${OBJECTDIR}/hardware/tvout/ntsc.o.d" -o ${OBJECTDIR}/hardware/tvout/ntsc.o hardware/tvout/ntsc.c    -D_SUPPRESS_PLIB_WARNING
	
${OBJECTDIR}/hardware/HardwareProfile.o: hardware/HardwareProfile.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/hardware 
	@${RM} ${OBJECTDIR}/hardware/HardwareProfile.o.d 
	@${RM} ${OBJECTDIR}/hardware/HardwareProfile.o 
	@${FIXDEPS} "${OBJECTDIR}/hardware/HardwareProfile.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -D_DEBUG_ -MMD -MF "${OBJECTDIR}/hardware/HardwareProfile.o.d" -o ${OBJECTDIR}/hardware/HardwareProfile.o hardware/HardwareProfile.c    -D_SUPPRESS_PLIB_WARNING
	
${OBJECTDIR}/console.o: console.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/console.o.d 
	@${RM} ${OBJECTDIR}/console.o 
	@${FIXDEPS} "${OBJECTDIR}/console.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -D_DEBUG_ -MMD -MF "${OBJECTDIR}/console.o.d" -o ${OBJECTDIR}/console.o console.c    -D_SUPPRESS_PLIB_WARNING
	
${OBJECTDIR}/font.o: font.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/font.o.d 
	@${RM} ${OBJECTDIR}/font.o 
	@${FIXDEPS} "${OBJECTDIR}/font.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -D_DEBUG_ -MMD -MF "${OBJECTDIR}/font.o.d" -o ${OBJECTDIR}/font.o font.c    -D_SUPPRESS_PLIB_WARNING
	
${OBJECTDIR}/hardware/Pinguino/fileio.o: hardware/Pinguino/fileio.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/hardware/Pinguino 
	@${RM} ${OBJECTDIR}/hardware/Pinguino/fileio.o.d 
	@${RM} ${OBJECTDIR}/hardware/Pinguino/fileio.o 
	@${FIXDEPS} "${OBJECTDIR}/hardware/Pinguino/fileio.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -D_DEBUG_ -MMD -MF "${OBJECTDIR}/hardware/Pinguino/fileio.o.d" -o ${OBJECTDIR}/hardware/Pinguino/fileio.o hardware/Pinguino/fileio.c    -D_SUPPRESS_PLIB_WARNING
	
${OBJECTDIR}/hardware/Pinguino/ff.o: hardware/Pinguino/ff.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/hardware/Pinguino 
	@${RM} ${OBJECTDIR}/hardware/Pinguino/ff.o.d 
	@${RM} ${OBJECTDIR}/hardware/Pinguino/ff.o 
	@${FIXDEPS} "${OBJECTDIR}/hardware/Pinguino/ff.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -D_DEBUG_ -MMD -MF "${OBJECTDIR}/hardware/Pinguino/ff.o.d" -o ${OBJECTDIR}/hardware/Pinguino/ff.o hardware/Pinguino/ff.c    -D_SUPPRESS_PLIB_WARNING
	
${OBJECTDIR}/editor.o: editor.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/editor.o.d 
	@${RM} ${OBJECTDIR}/editor.o 
	@${FIXDEPS} "${OBJECTDIR}/editor.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -D_DEBUG_ -MMD -MF "${OBJECTDIR}/editor.o.d" -o ${OBJECTDIR}/editor.o editor.c    -D_SUPPRESS_PLIB_WARNING
	
${OBJECTDIR}/shell.o: shell.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/shell.o.d 
	@${RM} ${OBJECTDIR}/shell.o 
	@${FIXDEPS} "${OBJECTDIR}/shell.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -D_DEBUG_ -MMD -MF "${OBJECTDIR}/shell.o.d" -o ${OBJECTDIR}/shell.o shell.c    -D_SUPPRESS_PLIB_WARNING
	
${OBJECTDIR}/vpc-32.o: vpc-32.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/vpc-32.o.d 
	@${RM} ${OBJECTDIR}/vpc-32.o 
	@${FIXDEPS} "${OBJECTDIR}/vpc-32.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -D_DEBUG_ -MMD -MF "${OBJECTDIR}/vpc-32.o.d" -o ${OBJECTDIR}/vpc-32.o vpc-32.c    -D_SUPPRESS_PLIB_WARNING
	
${OBJECTDIR}/graphics.o: graphics.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/graphics.o.d 
	@${RM} ${OBJECTDIR}/graphics.o 
	@${FIXDEPS} "${OBJECTDIR}/graphics.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -D_DEBUG_ -MMD -MF "${OBJECTDIR}/graphics.o.d" -o ${OBJECTDIR}/graphics.o graphics.c    -D_SUPPRESS_PLIB_WARNING
	
${OBJECTDIR}/vpcForth/TinyForth.o: vpcForth/TinyForth.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/vpcForth 
	@${RM} ${OBJECTDIR}/vpcForth/TinyForth.o.d 
	@${RM} ${OBJECTDIR}/vpcForth/TinyForth.o 
	@${FIXDEPS} "${OBJECTDIR}/vpcForth/TinyForth.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -D_DEBUG_ -MMD -MF "${OBJECTDIR}/vpcForth/TinyForth.o.d" -o ${OBJECTDIR}/vpcForth/TinyForth.o vpcForth/TinyForth.c    -D_SUPPRESS_PLIB_WARNING
	
else
${OBJECTDIR}/hardware/ps2_kbd/QWERTY.o: hardware/ps2_kbd/QWERTY.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/hardware/ps2_kbd 
	@${RM} ${OBJECTDIR}/hardware/ps2_kbd/QWERTY.o.d 
	@${RM} ${OBJECTDIR}/hardware/ps2_kbd/QWERTY.o 
	@${FIXDEPS} "${OBJECTDIR}/hardware/ps2_kbd/QWERTY.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -D_DEBUG_ -MMD -MF "${OBJECTDIR}/hardware/ps2_kbd/QWERTY.o.d" -o ${OBJECTDIR}/hardware/ps2_kbd/QWERTY.o hardware/ps2_kbd/QWERTY.c    -D_SUPPRESS_PLIB_WARNING
	
${OBJECTDIR}/hardware/ps2_kbd/keyboard.o: hardware/ps2_kbd/keyboard.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/hardware/ps2_kbd 
	@${RM} ${OBJECTDIR}/hardware/ps2_kbd/keyboard.o.d 
	@${RM} ${OBJECTDIR}/hardware/ps2_kbd/keyboard.o 
	@${FIXDEPS} "${OBJECTDIR}/hardware/ps2_kbd/keyboard.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -D_DEBUG_ -MMD -MF "${OBJECTDIR}/hardware/ps2_kbd/keyboard.o.d" -o ${OBJECTDIR}/hardware/ps2_kbd/keyboard.o hardware/ps2_kbd/keyboard.c    -D_SUPPRESS_PLIB_WARNING
	
${OBJECTDIR}/hardware/Pinguino/sdmmc.o: hardware/Pinguino/sdmmc.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/hardware/Pinguino 
	@${RM} ${OBJECTDIR}/hardware/Pinguino/sdmmc.o.d 
	@${RM} ${OBJECTDIR}/hardware/Pinguino/sdmmc.o 
	@${FIXDEPS} "${OBJECTDIR}/hardware/Pinguino/sdmmc.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -D_DEBUG_ -MMD -MF "${OBJECTDIR}/hardware/Pinguino/sdmmc.o.d" -o ${OBJECTDIR}/hardware/Pinguino/sdmmc.o hardware/Pinguino/sdmmc.c    -D_SUPPRESS_PLIB_WARNING
	
${OBJECTDIR}/hardware/Pinguino/diskio.o: hardware/Pinguino/diskio.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/hardware/Pinguino 
	@${RM} ${OBJECTDIR}/hardware/Pinguino/diskio.o.d 
	@${RM} ${OBJECTDIR}/hardware/Pinguino/diskio.o 
	@${FIXDEPS} "${OBJECTDIR}/hardware/Pinguino/diskio.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -D_DEBUG_ -MMD -MF "${OBJECTDIR}/hardware/Pinguino/diskio.o.d" -o ${OBJECTDIR}/hardware/Pinguino/diskio.o hardware/Pinguino/diskio.c    -D_SUPPRESS_PLIB_WARNING
	
${OBJECTDIR}/hardware/serial_comm/serial_comm.o: hardware/serial_comm/serial_comm.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/hardware/serial_comm 
	@${RM} ${OBJECTDIR}/hardware/serial_comm/serial_comm.o.d 
	@${RM} ${OBJECTDIR}/hardware/serial_comm/serial_comm.o 
	@${FIXDEPS} "${OBJECTDIR}/hardware/serial_comm/serial_comm.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -D_DEBUG_ -MMD -MF "${OBJECTDIR}/hardware/serial_comm/serial_comm.o.d" -o ${OBJECTDIR}/hardware/serial_comm/serial_comm.o hardware/serial_comm/serial_comm.c    -D_SUPPRESS_PLIB_WARNING
	
${OBJECTDIR}/hardware/sound/sound.o: hardware/sound/sound.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/hardware/sound 
	@${RM} ${OBJECTDIR}/hardware/sound/sound.o.d 
	@${RM} ${OBJECTDIR}/hardware/sound/sound.o 
	@${FIXDEPS} "${OBJECTDIR}/hardware/sound/sound.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -D_DEBUG_ -MMD -MF "${OBJECTDIR}/hardware/sound/sound.o.d" -o ${OBJECTDIR}/hardware/sound/sound.o hardware/sound/sound.c    -D_SUPPRESS_PLIB_WARNING
	
${OBJECTDIR}/hardware/spiram/spiram.o: hardware/spiram/spiram.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/hardware/spiram 
	@${RM} ${OBJECTDIR}/hardware/spiram/spiram.o.d 
	@${RM} ${OBJECTDIR}/hardware/spiram/spiram.o 
	@${FIXDEPS} "${OBJECTDIR}/hardware/spiram/spiram.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -D_DEBUG_ -MMD -MF "${OBJECTDIR}/hardware/spiram/spiram.o.d" -o ${OBJECTDIR}/hardware/spiram/spiram.o hardware/spiram/spiram.c    -D_SUPPRESS_PLIB_WARNING
	
${OBJECTDIR}/hardware/store/store_spi.o: hardware/store/store_spi.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/hardware/store 
	@${RM} ${OBJECTDIR}/hardware/store/store_spi.o.d 
	@${RM} ${OBJECTDIR}/hardware/store/store_spi.o 
	@${FIXDEPS} "${OBJECTDIR}/hardware/store/store_spi.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -D_DEBUG_ -MMD -MF "${OBJECTDIR}/hardware/store/store_spi.o.d" -o ${OBJECTDIR}/hardware/store/store_spi.o hardware/store/store_spi.c    -D_SUPPRESS_PLIB_WARNING
	
${OBJECTDIR}/hardware/tvout/ntsc.o: hardware/tvout/ntsc.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/hardware/tvout 
	@${RM} ${OBJECTDIR}/hardware/tvout/ntsc.o.d 
	@${RM} ${OBJECTDIR}/hardware/tvout/ntsc.o 
	@${FIXDEPS} "${OBJECTDIR}/hardware/tvout/ntsc.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -D_DEBUG_ -MMD -MF "${OBJECTDIR}/hardware/tvout/ntsc.o.d" -o ${OBJECTDIR}/hardware/tvout/ntsc.o hardware/tvout/ntsc.c    -D_SUPPRESS_PLIB_WARNING
	
${OBJECTDIR}/hardware/HardwareProfile.o: hardware/HardwareProfile.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/hardware 
	@${RM} ${OBJECTDIR}/hardware/HardwareProfile.o.d 
	@${RM} ${OBJECTDIR}/hardware/HardwareProfile.o 
	@${FIXDEPS} "${OBJECTDIR}/hardware/HardwareProfile.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -D_DEBUG_ -MMD -MF "${OBJECTDIR}/hardware/HardwareProfile.o.d" -o ${OBJECTDIR}/hardware/HardwareProfile.o hardware/HardwareProfile.c    -D_SUPPRESS_PLIB_WARNING
	
${OBJECTDIR}/console.o: console.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/console.o.d 
	@${RM} ${OBJECTDIR}/console.o 
	@${FIXDEPS} "${OBJECTDIR}/console.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -D_DEBUG_ -MMD -MF "${OBJECTDIR}/console.o.d" -o ${OBJECTDIR}/console.o console.c    -D_SUPPRESS_PLIB_WARNING
	
${OBJECTDIR}/font.o: font.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/font.o.d 
	@${RM} ${OBJECTDIR}/font.o 
	@${FIXDEPS} "${OBJECTDIR}/font.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -D_DEBUG_ -MMD -MF "${OBJECTDIR}/font.o.d" -o ${OBJECTDIR}/font.o font.c    -D_SUPPRESS_PLIB_WARNING
	
${OBJECTDIR}/hardware/Pinguino/fileio.o: hardware/Pinguino/fileio.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/hardware/Pinguino 
	@${RM} ${OBJECTDIR}/hardware/Pinguino/fileio.o.d 
	@${RM} ${OBJECTDIR}/hardware/Pinguino/fileio.o 
	@${FIXDEPS} "${OBJECTDIR}/hardware/Pinguino/fileio.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -D_DEBUG_ -MMD -MF "${OBJECTDIR}/hardware/Pinguino/fileio.o.d" -o ${OBJECTDIR}/hardware/Pinguino/fileio.o hardware/Pinguino/fileio.c    -D_SUPPRESS_PLIB_WARNING
	
${OBJECTDIR}/hardware/Pinguino/ff.o: hardware/Pinguino/ff.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/hardware/Pinguino 
	@${RM} ${OBJECTDIR}/hardware/Pinguino/ff.o.d 
	@${RM} ${OBJECTDIR}/hardware/Pinguino/ff.o 
	@${FIXDEPS} "${OBJECTDIR}/hardware/Pinguino/ff.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -D_DEBUG_ -MMD -MF "${OBJECTDIR}/hardware/Pinguino/ff.o.d" -o ${OBJECTDIR}/hardware/Pinguino/ff.o hardware/Pinguino/ff.c    -D_SUPPRESS_PLIB_WARNING
	
${OBJECTDIR}/editor.o: editor.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/editor.o.d 
	@${RM} ${OBJECTDIR}/editor.o 
	@${FIXDEPS} "${OBJECTDIR}/editor.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -D_DEBUG_ -MMD -MF "${OBJECTDIR}/editor.o.d" -o ${OBJECTDIR}/editor.o editor.c    -D_SUPPRESS_PLIB_WARNING
	
${OBJECTDIR}/shell.o: shell.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/shell.o.d 
	@${RM} ${OBJECTDIR}/shell.o 
	@${FIXDEPS} "${OBJECTDIR}/shell.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -D_DEBUG_ -MMD -MF "${OBJECTDIR}/shell.o.d" -o ${OBJECTDIR}/shell.o shell.c    -D_SUPPRESS_PLIB_WARNING
	
${OBJECTDIR}/vpc-32.o: vpc-32.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/vpc-32.o.d 
	@${RM} ${OBJECTDIR}/vpc-32.o 
	@${FIXDEPS} "${OBJECTDIR}/vpc-32.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -D_DEBUG_ -MMD -MF "${OBJECTDIR}/vpc-32.o.d" -o ${OBJECTDIR}/vpc-32.o vpc-32.c    -D_SUPPRESS_PLIB_WARNING
	
${OBJECTDIR}/graphics.o: graphics.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/graphics.o.d 
	@${RM} ${OBJECTDIR}/graphics.o 
	@${FIXDEPS} "${OBJECTDIR}/graphics.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -D_DEBUG_ -MMD -MF "${OBJECTDIR}/graphics.o.d" -o ${OBJECTDIR}/graphics.o graphics.c    -D_SUPPRESS_PLIB_WARNING
	
${OBJECTDIR}/vpcForth/TinyForth.o: vpcForth/TinyForth.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/vpcForth 
	@${RM} ${OBJECTDIR}/vpcForth/TinyForth.o.d 
	@${RM} ${OBJECTDIR}/vpcForth/TinyForth.o 
	@${FIXDEPS} "${OBJECTDIR}/vpcForth/TinyForth.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -D_DEBUG_ -MMD -MF "${OBJECTDIR}/vpcForth/TinyForth.o.d" -o ${OBJECTDIR}/vpcForth/TinyForth.o vpcForth/TinyForth.c    -D_SUPPRESS_PLIB_WARNING
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compileCPP
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
dist/${CND_CONF}/${IMAGE_TYPE}/vpc-32.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -mdebugger -D__MPLAB_DEBUGGER_PK3=1 -mprocessor=$(MP_PROCESSOR_OPTION)  -o dist/${CND_CONF}/${IMAGE_TYPE}/vpc-32.${IMAGE_TYPE}.${OUTPUT_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}           -mreserve=data@0x0:0x1FC -mreserve=boot@0x1FC00490:0x1FC00BEF  -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,--defsym=__MPLAB_DEBUGGER_PK3=1,--defsym=_min_heap_size=48000,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--report-mem
	
else
dist/${CND_CONF}/${IMAGE_TYPE}/vpc-32.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -mprocessor=$(MP_PROCESSOR_OPTION)  -o dist/${CND_CONF}/${IMAGE_TYPE}/vpc-32.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}          -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--defsym=_min_heap_size=48000,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--report-mem
	${MP_CC_DIR}/xc32-bin2hex dist/${CND_CONF}/${IMAGE_TYPE}/vpc-32.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} 
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

DEPFILES=$(shell "${PATH_TO_IDE_BIN}"mplabwildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif

#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
GREP=grep
NM=nm
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc
CCC=g++
CXX=g++
FC=gfortran
AS=as

# Macros
CND_PLATFORM=GNU-MacOSX
CND_DLIB_EXT=dylib
CND_CONF=Debug
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/Controls.o \
	${OBJECTDIR}/HowPlay.o \
	${OBJECTDIR}/HowPlay2.o \
	${OBJECTDIR}/Sprites.o \
	${OBJECTDIR}/background1.o \
	${OBJECTDIR}/bath.o \
	${OBJECTDIR}/biteSoundFX.o \
	${OBJECTDIR}/collisionfull.o \
	${OBJECTDIR}/collisionwater.o \
	${OBJECTDIR}/duckieSong.o \
	${OBJECTDIR}/gameHelp.o \
	${OBJECTDIR}/main.o \
	${OBJECTDIR}/myLib.o \
	${OBJECTDIR}/sound4.o \
	${OBJECTDIR}/whiteTiles.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=
CXXFLAGS=

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk cs2261

cs2261: ${OBJECTFILES}
	${LINK.c} -o cs2261 ${OBJECTFILES} ${LDLIBSOPTIONS}

${OBJECTDIR}/Controls.o: Controls.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Controls.o Controls.c

${OBJECTDIR}/HowPlay.o: HowPlay.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/HowPlay.o HowPlay.c

${OBJECTDIR}/HowPlay2.o: HowPlay2.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/HowPlay2.o HowPlay2.c

${OBJECTDIR}/Sprites.o: Sprites.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Sprites.o Sprites.c

${OBJECTDIR}/background1.o: background1.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/background1.o background1.c

${OBJECTDIR}/bath.o: bath.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/bath.o bath.c

${OBJECTDIR}/biteSoundFX.o: biteSoundFX.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/biteSoundFX.o biteSoundFX.c

${OBJECTDIR}/collisionfull.o: collisionfull.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/collisionfull.o collisionfull.c

${OBJECTDIR}/collisionwater.o: collisionwater.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/collisionwater.o collisionwater.c

${OBJECTDIR}/duckieSong.o: duckieSong.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/duckieSong.o duckieSong.c

${OBJECTDIR}/gameHelp.o: gameHelp.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/gameHelp.o gameHelp.c

${OBJECTDIR}/main.o: main.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/main.o main.c

${OBJECTDIR}/myLib.o: myLib.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/myLib.o myLib.c

${OBJECTDIR}/sound4.o: sound4.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/sound4.o sound4.c

${OBJECTDIR}/whiteTiles.o: whiteTiles.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/whiteTiles.o whiteTiles.c

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} cs2261

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc

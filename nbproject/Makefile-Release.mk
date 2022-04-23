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
CND_PLATFORM=GNU-Linux
CND_DLIB_EXT=so
CND_CONF=Release
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/dbase/DBASE__info.o \
	${OBJECTDIR}/dbase/DBASE__recipe.o \
	${OBJECTDIR}/dbase/DBASE__source.o \
	${OBJECTDIR}/dbase/DBASE__title.o \
	${OBJECTDIR}/dbase/dbase_api.o \
	${OBJECTDIR}/dbase/dbase_lib.o \
	${OBJECTDIR}/decode/bof/decode_bof_api.o \
	${OBJECTDIR}/decode/bof/decode_bof_lib.o \
	${OBJECTDIR}/decode/decode_api.o \
	${OBJECTDIR}/decode/decode_lib.o \
	${OBJECTDIR}/decode/mmf/decode_mmf_api.o \
	${OBJECTDIR}/decode/mmf/decode_mmf_lib.o \
	${OBJECTDIR}/decode/mxp/decode_mxp_api.o \
	${OBJECTDIR}/decode/mxp/decode_mxp_lib.o \
	${OBJECTDIR}/decode/post/decode_post_api.o \
	${OBJECTDIR}/decode/post/decode_post_lib.o \
	${OBJECTDIR}/decode/rxf/decode_rxf_api.o \
	${OBJECTDIR}/decode/rxf/decode_rxf_lib.o \
	${OBJECTDIR}/decode/test/DECODE_TEST.o \
	${OBJECTDIR}/email/email_api.o \
	${OBJECTDIR}/email/email_lib.o \
	${OBJECTDIR}/encode/RXF/encode_rxf_api.o \
	${OBJECTDIR}/encode/RXF/encode_rxf_lib.o \
	${OBJECTDIR}/encode/XML/encode_xml_api.o \
	${OBJECTDIR}/encode/XML/encode_xml_lib.o \
	${OBJECTDIR}/encode/encode_api.o \
	${OBJECTDIR}/encode/encode_lib.o \
	${OBJECTDIR}/export/export_api.o \
	${OBJECTDIR}/export/export_lib.o \
	${OBJECTDIR}/import/import_api.o \
	${OBJECTDIR}/import/import_lib.o \
	${OBJECTDIR}/main.o \
	${OBJECTDIR}/monitor/monitor_api.o \
	${OBJECTDIR}/monitor/monitor_lib.o \
	${OBJECTDIR}/rcb/rcb_api.o \
	${OBJECTDIR}/rcb/rcb_lib.o \
	${OBJECTDIR}/recipe/recipe_api.o \
	${OBJECTDIR}/recipe/recipe_lib.o \
	${OBJECTDIR}/tcb/tcb_api.o \
	${OBJECTDIR}/tcb/tcb_lib.o \
	${OBJECTDIR}/xlate/xlate_api.o \
	${OBJECTDIR}/xlate/xlate_lib.o


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
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/ria_v6

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/ria_v6: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.c} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/ria_v6 ${OBJECTFILES} ${LDLIBSOPTIONS}

${OBJECTDIR}/dbase/DBASE__info.o: dbase/DBASE__info.c
	${MKDIR} -p ${OBJECTDIR}/dbase
	${RM} "$@.d"
	$(COMPILE.c) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/dbase/DBASE__info.o dbase/DBASE__info.c

${OBJECTDIR}/dbase/DBASE__recipe.o: dbase/DBASE__recipe.c
	${MKDIR} -p ${OBJECTDIR}/dbase
	${RM} "$@.d"
	$(COMPILE.c) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/dbase/DBASE__recipe.o dbase/DBASE__recipe.c

${OBJECTDIR}/dbase/DBASE__source.o: dbase/DBASE__source.c
	${MKDIR} -p ${OBJECTDIR}/dbase
	${RM} "$@.d"
	$(COMPILE.c) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/dbase/DBASE__source.o dbase/DBASE__source.c

${OBJECTDIR}/dbase/DBASE__title.o: dbase/DBASE__title.c
	${MKDIR} -p ${OBJECTDIR}/dbase
	${RM} "$@.d"
	$(COMPILE.c) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/dbase/DBASE__title.o dbase/DBASE__title.c

${OBJECTDIR}/dbase/dbase_api.o: dbase/dbase_api.c
	${MKDIR} -p ${OBJECTDIR}/dbase
	${RM} "$@.d"
	$(COMPILE.c) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/dbase/dbase_api.o dbase/dbase_api.c

${OBJECTDIR}/dbase/dbase_lib.o: dbase/dbase_lib.c
	${MKDIR} -p ${OBJECTDIR}/dbase
	${RM} "$@.d"
	$(COMPILE.c) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/dbase/dbase_lib.o dbase/dbase_lib.c

${OBJECTDIR}/decode/bof/decode_bof_api.o: decode/bof/decode_bof_api.c
	${MKDIR} -p ${OBJECTDIR}/decode/bof
	${RM} "$@.d"
	$(COMPILE.c) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/decode/bof/decode_bof_api.o decode/bof/decode_bof_api.c

${OBJECTDIR}/decode/bof/decode_bof_lib.o: decode/bof/decode_bof_lib.c
	${MKDIR} -p ${OBJECTDIR}/decode/bof
	${RM} "$@.d"
	$(COMPILE.c) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/decode/bof/decode_bof_lib.o decode/bof/decode_bof_lib.c

${OBJECTDIR}/decode/decode_api.o: decode/decode_api.c
	${MKDIR} -p ${OBJECTDIR}/decode
	${RM} "$@.d"
	$(COMPILE.c) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/decode/decode_api.o decode/decode_api.c

${OBJECTDIR}/decode/decode_lib.o: decode/decode_lib.c
	${MKDIR} -p ${OBJECTDIR}/decode
	${RM} "$@.d"
	$(COMPILE.c) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/decode/decode_lib.o decode/decode_lib.c

${OBJECTDIR}/decode/mmf/decode_mmf_api.o: decode/mmf/decode_mmf_api.c
	${MKDIR} -p ${OBJECTDIR}/decode/mmf
	${RM} "$@.d"
	$(COMPILE.c) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/decode/mmf/decode_mmf_api.o decode/mmf/decode_mmf_api.c

${OBJECTDIR}/decode/mmf/decode_mmf_lib.o: decode/mmf/decode_mmf_lib.c
	${MKDIR} -p ${OBJECTDIR}/decode/mmf
	${RM} "$@.d"
	$(COMPILE.c) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/decode/mmf/decode_mmf_lib.o decode/mmf/decode_mmf_lib.c

${OBJECTDIR}/decode/mxp/decode_mxp_api.o: decode/mxp/decode_mxp_api.c
	${MKDIR} -p ${OBJECTDIR}/decode/mxp
	${RM} "$@.d"
	$(COMPILE.c) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/decode/mxp/decode_mxp_api.o decode/mxp/decode_mxp_api.c

${OBJECTDIR}/decode/mxp/decode_mxp_lib.o: decode/mxp/decode_mxp_lib.c
	${MKDIR} -p ${OBJECTDIR}/decode/mxp
	${RM} "$@.d"
	$(COMPILE.c) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/decode/mxp/decode_mxp_lib.o decode/mxp/decode_mxp_lib.c

${OBJECTDIR}/decode/post/decode_post_api.o: decode/post/decode_post_api.c
	${MKDIR} -p ${OBJECTDIR}/decode/post
	${RM} "$@.d"
	$(COMPILE.c) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/decode/post/decode_post_api.o decode/post/decode_post_api.c

${OBJECTDIR}/decode/post/decode_post_lib.o: decode/post/decode_post_lib.c
	${MKDIR} -p ${OBJECTDIR}/decode/post
	${RM} "$@.d"
	$(COMPILE.c) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/decode/post/decode_post_lib.o decode/post/decode_post_lib.c

${OBJECTDIR}/decode/rxf/decode_rxf_api.o: decode/rxf/decode_rxf_api.c
	${MKDIR} -p ${OBJECTDIR}/decode/rxf
	${RM} "$@.d"
	$(COMPILE.c) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/decode/rxf/decode_rxf_api.o decode/rxf/decode_rxf_api.c

${OBJECTDIR}/decode/rxf/decode_rxf_lib.o: decode/rxf/decode_rxf_lib.c
	${MKDIR} -p ${OBJECTDIR}/decode/rxf
	${RM} "$@.d"
	$(COMPILE.c) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/decode/rxf/decode_rxf_lib.o decode/rxf/decode_rxf_lib.c

${OBJECTDIR}/decode/test/DECODE_TEST.o: decode/test/DECODE_TEST.c
	${MKDIR} -p ${OBJECTDIR}/decode/test
	${RM} "$@.d"
	$(COMPILE.c) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/decode/test/DECODE_TEST.o decode/test/DECODE_TEST.c

${OBJECTDIR}/email/email_api.o: email/email_api.c
	${MKDIR} -p ${OBJECTDIR}/email
	${RM} "$@.d"
	$(COMPILE.c) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/email/email_api.o email/email_api.c

${OBJECTDIR}/email/email_lib.o: email/email_lib.c
	${MKDIR} -p ${OBJECTDIR}/email
	${RM} "$@.d"
	$(COMPILE.c) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/email/email_lib.o email/email_lib.c

${OBJECTDIR}/encode/RXF/encode_rxf_api.o: encode/RXF/encode_rxf_api.c
	${MKDIR} -p ${OBJECTDIR}/encode/RXF
	${RM} "$@.d"
	$(COMPILE.c) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/encode/RXF/encode_rxf_api.o encode/RXF/encode_rxf_api.c

${OBJECTDIR}/encode/RXF/encode_rxf_lib.o: encode/RXF/encode_rxf_lib.c
	${MKDIR} -p ${OBJECTDIR}/encode/RXF
	${RM} "$@.d"
	$(COMPILE.c) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/encode/RXF/encode_rxf_lib.o encode/RXF/encode_rxf_lib.c

${OBJECTDIR}/encode/XML/encode_xml_api.o: encode/XML/encode_xml_api.c
	${MKDIR} -p ${OBJECTDIR}/encode/XML
	${RM} "$@.d"
	$(COMPILE.c) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/encode/XML/encode_xml_api.o encode/XML/encode_xml_api.c

${OBJECTDIR}/encode/XML/encode_xml_lib.o: encode/XML/encode_xml_lib.c
	${MKDIR} -p ${OBJECTDIR}/encode/XML
	${RM} "$@.d"
	$(COMPILE.c) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/encode/XML/encode_xml_lib.o encode/XML/encode_xml_lib.c

${OBJECTDIR}/encode/encode_api.o: encode/encode_api.c
	${MKDIR} -p ${OBJECTDIR}/encode
	${RM} "$@.d"
	$(COMPILE.c) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/encode/encode_api.o encode/encode_api.c

${OBJECTDIR}/encode/encode_lib.o: encode/encode_lib.c
	${MKDIR} -p ${OBJECTDIR}/encode
	${RM} "$@.d"
	$(COMPILE.c) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/encode/encode_lib.o encode/encode_lib.c

${OBJECTDIR}/export/export_api.o: export/export_api.c
	${MKDIR} -p ${OBJECTDIR}/export
	${RM} "$@.d"
	$(COMPILE.c) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/export/export_api.o export/export_api.c

${OBJECTDIR}/export/export_lib.o: export/export_lib.c
	${MKDIR} -p ${OBJECTDIR}/export
	${RM} "$@.d"
	$(COMPILE.c) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/export/export_lib.o export/export_lib.c

${OBJECTDIR}/import/import_api.o: import/import_api.c
	${MKDIR} -p ${OBJECTDIR}/import
	${RM} "$@.d"
	$(COMPILE.c) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/import/import_api.o import/import_api.c

${OBJECTDIR}/import/import_lib.o: import/import_lib.c
	${MKDIR} -p ${OBJECTDIR}/import
	${RM} "$@.d"
	$(COMPILE.c) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/import/import_lib.o import/import_lib.c

${OBJECTDIR}/main.o: main.c
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/main.o main.c

${OBJECTDIR}/monitor/monitor_api.o: monitor/monitor_api.c
	${MKDIR} -p ${OBJECTDIR}/monitor
	${RM} "$@.d"
	$(COMPILE.c) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/monitor/monitor_api.o monitor/monitor_api.c

${OBJECTDIR}/monitor/monitor_lib.o: monitor/monitor_lib.c
	${MKDIR} -p ${OBJECTDIR}/monitor
	${RM} "$@.d"
	$(COMPILE.c) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/monitor/monitor_lib.o monitor/monitor_lib.c

${OBJECTDIR}/rcb/rcb_api.o: rcb/rcb_api.c
	${MKDIR} -p ${OBJECTDIR}/rcb
	${RM} "$@.d"
	$(COMPILE.c) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/rcb/rcb_api.o rcb/rcb_api.c

${OBJECTDIR}/rcb/rcb_lib.o: rcb/rcb_lib.c
	${MKDIR} -p ${OBJECTDIR}/rcb
	${RM} "$@.d"
	$(COMPILE.c) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/rcb/rcb_lib.o rcb/rcb_lib.c

${OBJECTDIR}/recipe/recipe_api.o: recipe/recipe_api.c
	${MKDIR} -p ${OBJECTDIR}/recipe
	${RM} "$@.d"
	$(COMPILE.c) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/recipe/recipe_api.o recipe/recipe_api.c

${OBJECTDIR}/recipe/recipe_lib.o: recipe/recipe_lib.c
	${MKDIR} -p ${OBJECTDIR}/recipe
	${RM} "$@.d"
	$(COMPILE.c) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/recipe/recipe_lib.o recipe/recipe_lib.c

${OBJECTDIR}/tcb/tcb_api.o: tcb/tcb_api.c
	${MKDIR} -p ${OBJECTDIR}/tcb
	${RM} "$@.d"
	$(COMPILE.c) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/tcb/tcb_api.o tcb/tcb_api.c

${OBJECTDIR}/tcb/tcb_lib.o: tcb/tcb_lib.c
	${MKDIR} -p ${OBJECTDIR}/tcb
	${RM} "$@.d"
	$(COMPILE.c) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/tcb/tcb_lib.o tcb/tcb_lib.c

${OBJECTDIR}/xlate/xlate_api.o: xlate/xlate_api.c
	${MKDIR} -p ${OBJECTDIR}/xlate
	${RM} "$@.d"
	$(COMPILE.c) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/xlate/xlate_api.o xlate/xlate_api.c

${OBJECTDIR}/xlate/xlate_lib.o: xlate/xlate_lib.c
	${MKDIR} -p ${OBJECTDIR}/xlate
	${RM} "$@.d"
	$(COMPILE.c) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/xlate/xlate_lib.o xlate/xlate_lib.c

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc

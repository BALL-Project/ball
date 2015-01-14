###################################################
# Please list all header source files here        #
# (necessary to distinguish between binaries      #
# and header-sources)                             #
#                                                 #
# by pbrach                                       #
###################################################

### the directory name ###
SET(DIRECTORY source/APPLICATIONS/LigStruct/sources)

### list all filenames of the directory here ###
SET(SOURCES_LIST
	ligandStructureBase.C
	starAligner.C
	moleculeConnector.C
	rmsdBinner.C
	ioModule.C
	fragmenter.C
	clashResolver.C
	fragmentBuilder.C
	assembler.C # TODO: remove
	structureAssembler.C
	combiAssembler.C
)	
#FILE(GLOB SOURCES_LIST "source/APPLICATIONS/LigStruct/*.C")

ADD_BALL_SOURCES("APPLICATIONS/LigStruct/sources" "${SOURCES_LIST}")

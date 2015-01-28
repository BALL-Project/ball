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
	base.C
	canonicalizer.C
	combiAssembler.C
	clashResolver.C
	linkerBuilder.C
	fragmenter.C
	ioModule.C
	moleculeConnector.C
	rmsdBinner.C
	structureAssembler.C
	starAligner.C
)	
#FILE(GLOB SOURCES_LIST "source/APPLICATIONS/LigStruct/*.C")

ADD_BALL_SOURCES("APPLICATIONS/LigStruct/sources" "${SOURCES_LIST}")

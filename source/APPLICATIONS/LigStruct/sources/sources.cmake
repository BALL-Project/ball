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
	assembler.C
	structureAssembler.C
)	
#FILE(GLOB SOURCES_LIST "source/APPLICATIONS/LigStruct/*.C")

ADD_BALL_SOURCES("APPLICATIONS/LigStruct/sources" "${SOURCES_LIST}")

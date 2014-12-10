###################################################
# Please list all header source files here        #
# (necessary to distinguish between binaries      #
# and header-sources)                             #
#                                                 #
# by pbrach                                       #
###################################################

### the directory name ###
SET(DIRECTORY source/APPLICATIONS/LigStruct)

### list all filenames of the directory here ###
SET(SOURCES_LIST
	splitMolecule.C
	structureAssembler.C
)	
#FILE(GLOB SOURCES_LIST "source/APPLICATIONS/LigStruct/*.C")

ADD_BALL_SOURCES("APPLICATIONS/LigStruct" "${SOURCES_LIST}")

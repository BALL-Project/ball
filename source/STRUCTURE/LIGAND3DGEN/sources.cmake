
### the directory name ###
#SET(LIG_SOURCE_DIR source/STRUCTURE/LIGAND3DGEN)

### list all filenames of the directory here ###
SET(SOURCES_LIST
	ligand3DBase.C
	combiAssembler.C
	linkerBuilder.C
	ioModule.C
	moleculeConnector.C
	structureAssembler.C
	starAligner.C
)

ADD_BALL_SOURCES("STRUCTURE/LIGAND3DGEN" "${SOURCES_LIST}")

#####################################
## Automatically find all sources 
## from *.C files within the 
## directory specified (first argument)
##
## 01-2015, by Philipp Brachvogel
##
#####################################

#### the directory name
#SET(LIG_SOURCE_PATH APPLICATIONS/LigStruct/sources)

#### list all full filenames of the directory here ###
#FILE(GLOB LIG_SOURCE_PATHES_LIST "source/${LIG_SOURCE_PATH}/*.C")

#### remove absolute paths but KEEP the extension:
#SET(LIG_SOURCES_LIST)

#FOREACH(i ${LIG_SOURCE_PATHES_LIST})
#    GET_FILENAME_COMPONENT(i ${i} NAME)
#    LIST(APPEND LIG_SOURCES_LIST ${i})
#ENDFOREACH()

## add to global sources list
#ADD_BALL_SOURCES("${LIG_SOURCE_PATH}" "${LIG_SOURCES_LIST}")

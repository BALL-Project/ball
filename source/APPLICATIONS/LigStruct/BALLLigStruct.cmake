####################################
# Set here which BINARIES to compile
# by pbrach
####################################

### the directory name
SET(DIRECTORY source/APPLICATIONS/LigStruct)

### list all filenames of the directory here ###
SET(EXECUTABLES_LIST
	playTool
	testTool
	prototype_libFragmenter
	prototype_queryFragmenter
	connectionFragmenter
	connectionRMSDFilter
)


SET(LIGSTRUCT_EXECUTABLES ${LIGSTRUCT_EXECUTABLES} ${EXECUTABLES_LIST})

### add filenames to Visual Studio solution
SET(LIGSTRUCT_SOURCES)
FOREACH(i ${EXECUTABLES_LIST})
	LIST(APPEND LIGSTRUCT_SOURCES "${i}")
ENDFOREACH(i)
SOURCE_GROUP("" FILES ${LIGSTRUCT_SOURCES})

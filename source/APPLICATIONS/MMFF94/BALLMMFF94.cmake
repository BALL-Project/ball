### the directory name
SET(DIRECTORY source/APPLICATIONS/MMFF94)

### list all filenames of the directory here ###
SET(MMFF94_EXECUTABLES
	compareForces
	readMMFF94TestFile
	splitMMFFTestSuiteMolFiles
	splitMMFFTestSuiteOptiFiles
	testKekulizer
)

### add filenames to Visual Studio solution
SET(MMFF94_SOURCES)
FOREACH(i ${EXECUTABLES_LIST})
	LIST(APPEND MMFF94_SOURCES "${i}")
ENDFOREACH(i)
SOURCE_GROUP("" FILES ${MMFF94_SOURCES})

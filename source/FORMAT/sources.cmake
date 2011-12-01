### list all filenames of the directory here ###
SET(SOURCES_LIST
	amiraMeshFile.C
	antechamberFile.C
	bruker1DFile.C
	bruker2DFile.C
	commandlineParser.C
	dockResultFile.C
	CCP4File.C
	CIFFile.C
	DCDFile.C
	DSN6File.C
	GAMESSDatFile.C
#	GAMESSLogFile.C
	genericMolFile.C
	HINFile.C
	INIFile.C
	JCAMPFile.C
	KCFFile.C
	lineBasedFile.C
	molDB.C
	MOLFile.C
	MOPACInputFile.C
	MOPACOutputFile.C
	SDFile.C
	MOL2File.C
	NMRStarFile.C
	paramFile.C
	parameters.C
	parameterSection.C
	paramFile.C
	PDBdefs.C
	PDBFileGeneral.C
	PDBFileDetails.C
	PDBInfo.C
	PDBRecords.C
	pubchemDownloader.C
	resourceFile.C
	trajectoryFile.C
	TRRFile.C
	XYZFile.C
	SCWRLRotamerFile.C
	molFileFactory.C
)

IF (BALL_HAS_QTSQL)
	SET(SOURCES_LIST ${SOURCES_LIST} DBInterface.C)
ENDIF()

SET(MOC_SOURCES_LIST "pubchemDownloader.C")

ADD_BALL_SOURCES("FORMAT" "${SOURCES_LIST}")

ADD_BALL_PARSER_LEXER("FORMAT" "CIFParser" "CIFParser")
ADD_BALL_PARSER_LEXER("FORMAT" "GAMESSDatParser" "GAMESSDatParser")
#ADD_BALL_PARSER_LEXER("FORMAT" "GAMESSLogParser" "GAMESSLogParser")

ADD_BALL_CORE_MOCFILES("FORMAT" "include/BALL/FORMAT" "${MOC_SOURCES_LIST}")

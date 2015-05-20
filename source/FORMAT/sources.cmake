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
	HMOFile.C
	INIFile.C
	JCAMPFile.C
	KCFFile.C
	lineBasedFile.C
	MOLFile.C
	molFileFactory.C
	MOPACInputFile.C
	MOPACOutputFile.C
	SDFile.C
	MOL2File.C
	NMRStarFile.C
	paramFile.C
	parameters.C
	parameterSection.C
	PDBdefs.C
	PDBFileGeneral.C
	PDBFileDetails.C
	PDBInfo.C
	PDBRecords.C
	pubchemDownloader.C
	resourceFile.C
	SCWRLRotamerFile.C
	trajectoryFile.C
	trajectoryFileFactory.C
	TRRFile.C
	XYZFile.C
)

ADD_BALL_SOURCES("FORMAT" "${SOURCES_LIST}")

ADD_BALL_PARSER_LEXER("FORMAT" "CIFParser" "CIFParser")
ADD_BALL_PARSER_LEXER("FORMAT" "GAMESSDatParser" "GAMESSDatParser")
#ADD_BALL_PARSER_LEXER("FORMAT" "GAMESSLogParser" "GAMESSLogParser")

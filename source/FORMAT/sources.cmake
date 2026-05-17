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
	# GAMESSLogFile.C — removed v1.7 (Phase 999.14 / issue #662). Class
	# was forward-referenced against a never-implemented QMBasisSet header
	# since commit c482d49fd5 (2005-10-05); .C had been disabled in the
	# build for ~20 years. No callers existed. Resurrect from scratch
	# against modern OpenBabel if GAMESS log parsing is ever needed again.
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
# ADD_BALL_PARSER_LEXER("FORMAT" "GAMESSLogParser" "GAMESSLogParser") — removed
# v1.7 (Phase 999.14 / issue #662) alongside GAMESSLogFile.{h,C}. Lexer + parser
# sources GAMESSLogParserLexer.l + GAMESSLogParserParser.y both depended on the
# never-implemented QMBasisSet header chain through GAMESSLogFile.h; macro had
# been commented out since GAMESSLogFile.C was disabled (~2005). Whole GAMESS-
# log subsystem deleted as obsolete dead code.

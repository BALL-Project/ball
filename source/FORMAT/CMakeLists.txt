set(path FORMAT)


# SET(YACC_SOURCES "")
# SET(YACC_SOURCES 
# 	GAMESSDatParserParser.y
# 	GAMESSLogParserParser.y
# )
# 
# foreach(i ${YACC_SOURCES})
# 	wrap_YACC(BALL ${path} ${i})
# endforeach(i)
# 
# 
# SET(FLEX_SOURCES "")
# SET(FLEX_SOURCES 
# 	GAMESSDatParserLexer.l
# 	GAMESSLogParserLexer.l
# )
# 
# foreach(i ${FLEX_SOURCES})
# 	wrap_FLEX(BALL ${path} ${i})
# endforeach(i)


list(APPEND BALL_SOURCES 
	FORMAT/bruker1DFile.C
	FORMAT/bruker2DFile.C
	FORMAT/DBInterface.C
	FORMAT/DCDFile.C
	FORMAT/DSN6File.C
#	FORMAT/GAMESSDatFile.C
#	FORMAT/GAMESSLogFile.C   # seems to contains errors!
	FORMAT/genericMolFile.C
	FORMAT/HINFile.C
	FORMAT/INIFile.C
	FORMAT/JCAMPFile.C
	FORMAT/KCFFile.C
	FORMAT/lineBasedFile.C
	FORMAT/MOL2File.C
	FORMAT/MOLFile.C
	FORMAT/molFileFactory.C
	FORMAT/NMRStarFile.C
	FORMAT/parameters.C
	FORMAT/parameterSection.C
	FORMAT/PDBdefs.C
	FORMAT/PDBFileDetails.C
	FORMAT/PDBFileGeneral.C
	FORMAT/PDBInfo.C
	FORMAT/PDBRecords.C
	FORMAT/resourceFile.C
	FORMAT/SCWRLRotamerFile.C
	FORMAT/SDFile.C
	FORMAT/trajectoryFile.C
	FORMAT/TRRFile.C
	FORMAT/XYZFile.C
)


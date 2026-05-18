### the directory name ###
SET(DIRECTORY source/FORMAT)

### list all filenames of the directory here ###
# B1.1 (Track B Wave 1a, 2026-05-18): minimum-viable FORMAT enable.
# Goal is to unblock the K0.5 selector test surface (Expression_test,
# Selector_test, StandardPredicates_test1..4) which only need PDB +
# HIN parsing. Most other FORMAT files (DCD/TRR trajectories, MOL2
# with GAFF typing, SCWRLRotamer, dockResultFile, NMRStar) require
# downstream-module symbols (MOLMEC, STRUCTURE, DOCKING, QSAR) that
# Track B Waves 2-6 enable later. Those files come back as their
# respective module re-enables happen.
#
# Kept: base infrastructure + PDB + HIN + generic-mol + INI + simple
# small-molecule formats (XYZ, KCF, MOLFile, JCAMP, MOPAC input/output,
# HMOFile, antechamber, bruker NMR, GAMESS, SDFile, CCP4 / DSN6 /
# CIFFile electron density / pubchem downloader).
SET(SOURCES_LIST
	amiraMeshFile.C
	antechamberFile.C
	bruker1DFile.C
	bruker2DFile.C
	commandlineParser.C
	CCP4File.C
	CIFFile.C
	DSN6File.C
	GAMESSDatFile.C
	genericMolFile.C
	HINFile.C
	HMOFile.C
	INIFile.C
	JCAMPFile.C
	KCFFile.C
	lineBasedFile.C
	MOLFile.C
	MOPACInputFile.C
	MOPACOutputFile.C
	SDFile.C
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
	XYZFile.C
)

# B1.1: trimmed out for Wave-by-Wave re-enable:
# - DCDFile.C / TRRFile.C / trajectoryFile{,Factory}.C  : need SnapShot[Manager] (MOLMEC, Wave 3)
# - dockResultFile.C                                    : needs Result::ResultData (DOCKING, Wave 6)
# - NMRStarFile.C                                       : needs Peptides::NameConverter (STRUCTURE, Wave 1b)
# - MOL2File.C                                          : needs GAFFTypeProcessor (MOLMEC + QSAR, Wave 3-4)
# - SCWRLRotamerFile.C                                  : needs Rotamer/RotamerLibrary (STRUCTURE, Wave 1b)
# - molFileFactory.C                                    : references DockResultFile typeinfo; comes back with dockResultFile.C
ADD_BALL_SOURCES("FORMAT" "${SOURCES_LIST}")

ADD_BALL_PARSER_LEXER("FORMAT" "CIFParser" "CIFParser")
ADD_BALL_PARSER_LEXER("FORMAT" "GAMESSDatParser" "GAMESSDatParser")

# B1.1 auxiliary: MOLMEC/COMMON/snapShot.C and XRAY/crystalInfo.C are
# pulled in so the FORMAT files that reference SnapShot / CrystalInfo
# (PDBFileDetails.C for CRYST1 record interpretation) link cleanly.
# snapShot.C is header-only-dep self-contained; crystalInfo.C only needs
# FORMAT (which is now in). Both auxiliaries get pulled OUT in their
# proper Wave 3 (MOLMEC) / Wave 2 (XRAY) re-enable commits.
ADD_BALL_SOURCES("MOLMEC/COMMON" "snapShot.C")
ADD_BALL_SOURCES("XRAY"          "crystalInfo.C")

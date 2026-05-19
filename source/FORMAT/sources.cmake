### the directory name ###
SET(DIRECTORY source/FORMAT)

### list all filenames of the directory here ###
# B1.1 (Track B Wave 1a, 2026-05-18): minimum-viable FORMAT enable.
# Goal is to unblock the K0.5 selector test surface (Expression_test,
# Selector_test, StandardPredicates_test1..4) which only need PDB +
# HIN parsing. Most other FORMAT files (DCD/TRR trajectories, MOL2
# with GAFF typing, SCWRLRotamer, dockResultFile, NMRStar) require
# downstream-module symbols (MOLMEC, STRUCTURE, DOCKING, QSAR) that
# Track B Waves 2-6 enable later.
#
# B1.3 (Codex R10 fix, 2026-05-18): the trim list applies ONLY in
# BALL_CORE_ONLY mode. In full builds, MOLMEC/STRUCTURE/DOCKING/QSAR
# are all compiled in and their callers (MMFF94::specificSetup calling
# Kekuliser, Pair6_12InteractionEnergyProcessor calling RDFParameter,
# DockingAlgorithm needing DockResultFile typeinfo) require the full
# FORMAT surface to link. CORE_ONLY trims the subset that cascades
# back into the disabled extension modules.
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
	# Cluster A (2026-05-19): trajectory files now linkable since
	# MOLMEC/COMMON/snapShot.C + snapShotManager.C are in.
	DCDFile.C
	TRRFile.C
	trajectoryFile.C
	trajectoryFileFactory.C
)

# Cluster B-blocked FORMAT files (still need QSAR/DOCKING/STRUCTURE
# rotamer):
# - dockResultFile.C    : needs Result::ResultData (DOCKING)
# - NMRStarFile.C       : needs Peptides::NameConverter (STRUCTURE)
# - MOL2File.C          : needs GAFFTypeProcessor (MOLMEC AMBER + QSAR)
# - SCWRLRotamerFile.C  : needs Rotamer/RotamerLibrary (STRUCTURE — rotamerLibrary trimmed)
# - molFileFactory.C    : references DockResultFile typeinfo
IF(NOT BALL_CORE_ONLY)
	LIST(APPEND SOURCES_LIST
		dockResultFile.C
		NMRStarFile.C
		MOL2File.C
		SCWRLRotamerFile.C
		molFileFactory.C
	)
ENDIF()

ADD_BALL_SOURCES("FORMAT" "${SOURCES_LIST}")

ADD_BALL_PARSER_LEXER("FORMAT" "CIFParser" "CIFParser")
ADD_BALL_PARSER_LEXER("FORMAT" "GAMESSDatParser" "GAMESSDatParser")

# Cluster A (2026-05-19): snapShot.C aux pull REMOVED. MOLMEC's
# COMMON subdir is now re-enabled unconditionally (see
# cmake/BALLIncludes.cmake) so snapShot.C + snapShotManager.C come in
# via MOLMEC/COMMON/sources.cmake. Both crystalInfo.C (B2.1) and
# snapShot.C (Cluster A) aux pulls are now retired; FORMAT depends
# only on its own sources + the already-in MOLMEC/XRAY modules.

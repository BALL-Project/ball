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
)

# B1.3 (Codex R10 fix #6, 2026-05-18): in full builds, restore the
# CORE_ONLY-trimmed FORMAT sources. These are the files that need
# MOLMEC / STRUCTURE / DOCKING / QSAR symbols which are only available
# when BALL_CORE_ONLY=OFF. Per-file dependency:
# - DCDFile.C / TRRFile.C / trajectoryFile{,Factory}.C  : need SnapShot[Manager] (MOLMEC)
# - dockResultFile.C                                    : needs Result::ResultData (DOCKING)
# - NMRStarFile.C                                       : needs Peptides::NameConverter (STRUCTURE)
# - MOL2File.C                                          : needs GAFFTypeProcessor (MOLMEC + QSAR)
# - SCWRLRotamerFile.C                                  : needs Rotamer/RotamerLibrary (STRUCTURE)
# - molFileFactory.C                                    : references DockResultFile typeinfo
IF(NOT BALL_CORE_ONLY)
	LIST(APPEND SOURCES_LIST
		DCDFile.C
		TRRFile.C
		trajectoryFile.C
		trajectoryFileFactory.C
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

# B1.1 auxiliary: MOLMEC/COMMON/snapShot.C is pulled in so the FORMAT
# files that reference SnapShot (PDBFileDetails.C for HELIX-like
# record interpretation) link cleanly. snapShot.C is header-only-dep
# self-contained.
#
# B1.3 (Codex R10 fix #6, 2026-05-18): gate the aux pulls on
# BALL_CORE_ONLY. In full builds the MOLMEC and XRAY sources.cmake
# files are INCLUDE()d via cmake/BALLIncludes.cmake and already add
# snapShot.C / crystalInfo.C. ADD_BALL_SOURCES doesn't dedupe
# (cmake/BALLMacros.cmake `ADD_BALL_SOURCES` appends to BALL_sources
# unconditionally) so unguarded inclusion here would double-add the
# .C files into the link line — duplicate-symbol errors on strict
# linkers.
#
# B2.1 (Track B Wave 2a, 2026-05-18): crystalInfo.C aux pull REMOVED;
# XRAY module is now re-enabled unconditionally (see
# cmake/BALLIncludes.cmake) so crystalInfo.C comes in via XRAY's own
# sources.cmake. snapShot.C aux pull stays until MOLMEC Wave 3.
IF(BALL_CORE_ONLY)
	ADD_BALL_SOURCES("MOLMEC/COMMON" "snapShot.C")
ENDIF()

SET(BALL_sources CACHE INTERNAL "This variable should hold all BALL sources at the end of the config step")
SET(BALL_headers CACHE INTERNAL "This variable should hold all BALL headers at the end of the config step")

# Always-on: core foundation. KERNEL replacement (BALL 2.0) targets these
# modules first; everything else can be conditionally disabled via
# BALL_CORE_ONLY (see top-level CMakeLists.txt).
INCLUDE(include/BALL/CONFIG/sources.cmake)

INCLUDE(source/CONCEPT/sources.cmake)
INCLUDE(include/BALL/CONCEPT/sources.cmake)

INCLUDE(source/COMMON/sources.cmake)
INCLUDE(include/BALL/COMMON/sources.cmake)

INCLUDE(source/DATATYPE/sources.cmake)
INCLUDE(include/BALL/DATATYPE/sources.cmake)

INCLUDE(source/KERNEL/sources.cmake)
INCLUDE(include/BALL/KERNEL/sources.cmake)

INCLUDE(source/MATHS/sources.cmake)
INCLUDE(include/BALL/MATHS/sources.cmake)

INCLUDE(source/SYSTEM/sources.cmake)
INCLUDE(include/BALL/SYSTEM/sources.cmake)

# B0.1 (Track B Wave 0, 2026-05-18): PLUGIN re-enabled unconditionally.
# Has zero deps on other disabled modules (per MODULE-REENABLE-PLAN.md
# audit) so the toggle is safe even with BALL_CORE_ONLY=ON. Smoke-tests
# the BALL_CORE_ONLY-removal path before the FORMAT/STRUCTURE waves.
INCLUDE(source/PLUGIN/sources.cmake)
INCLUDE(include/BALL/PLUGIN/sources.cmake)

# B1.1 (Track B Wave 1, 2026-05-18): FORMAT re-enabled unconditionally.
# Depends only on core (no other disabled-module deps per
# MODULE-REENABLE-PLAN.md §1). Unblocks 9 downstream modules and the
# ~11 deferred Expression/Selector/StandardPredicates/Residue tests
# that #include <BALL/FORMAT/PDBFile.h>.
INCLUDE(source/FORMAT/sources.cmake)
INCLUDE(include/BALL/FORMAT/sources.cmake)

# B1.2 (Track B Wave 1b, 2026-05-18): STRUCTURE re-enabled — SUBSET.
# In CORE_ONLY mode ~47 of 60+ STRUCTURE sources are compiled. Provides:
# FragmentDB, ResidueChecker, DefaultProcessors, Peptides{,Builder,
# CapProcessor}, NormalizeNamesProcessor, SecondaryStructureProcessor,
# connectedComponents, disulfid, numericalSAS, analyticalSES,
# reducedSurface, SES*/SAS*/RS* surface stack, triangulated* mesh,
# structureMapper, RMSDMinimizer, UCK, binaryFingerprintMethods,
# bindingPocketProcessor, atomBijection.
#
# B1.3 (Codex R10 fix, 2026-05-18): TRIMMED in CORE_ONLY (see
# source/STRUCTURE/sources.cmake for the full list + reasons):
# rotamerLibrary, sideChainPlacementProcessor, ringAnalyser, sdGenerator,
# hybridisationProcessor, buildBondsProcessor, assignBondOrderProcessor
# (and the BONDORDERS/ sub-tree that depends on it), kekulizer,
# smartsMatcher, atomTyper, molecularSimilarity, addHydrogenProcessor,
# RDFParameter, DNAMutator. These come back as MOLMEC (Wave 3), QSAR
# (Wave 4), and FORMAT-SCWRLRotamerFile (Wave 1b cycle close) re-enable.
# Until then, STRUCTURE headers compile but calls into trimmed
# implementations are link errors — public v2.0 users should consult
# RELEASE-NOTES-v2.0.md §"Track B partial-module surface".
INCLUDE(source/STRUCTURE/sources.cmake)
INCLUDE(include/BALL/STRUCTURE/sources.cmake)

# B2.1 (Track B Wave 2a, 2026-05-18): XRAY re-enabled unconditionally.
# Tiny module (3 sources: crystalGenerator, crystalInfo,
# spaceGroupParameters). Depends on FORMAT (Wave 1a, enabled) and
# STRUCTURE/geometricProperties + geometricTransformations (Wave 1b,
# kept in subset). Provides X-ray crystallography support — unlocks
# CRYST1 record interpretation in PDBFileDetails and the bpti.pdb
# round-trip path in TextPersistenceManager_test.
INCLUDE(source/XRAY/sources.cmake)
INCLUDE(include/BALL/XRAY/sources.cmake)

# B2.2 (Track B Wave 2b, 2026-05-18): NMR re-enabled unconditionally.
# 15 sources (anisotropy/EF/empiricalHS/HBond/haighMallion/johnsonBovey/
# randomCoil shift processors + spectrum/peak/peakList +
# createSpectrumProcessor + clear + experiment + shiftModel + shiftModule).
# Deps: CONCEPT, DATATYPE, FORMAT (Wave 1a, in), KERNEL, MATHS,
# STRUCTURE (Wave 1b subset, in — uses geometricProperties only),
# SYSTEM. No MOLMEC/QSAR/DOCKING/ENERGY dep, so safe to add now.
INCLUDE(source/NMR/sources.cmake)
INCLUDE(include/BALL/NMR/sources.cmake)

# Conditional: extension modules. Disabled by BALL_CORE_ONLY=ON for the
# v2.0 KERNEL-replacement work (D10 / KERNEL-V2-DECISIONS.md). Re-enabled
# module-by-module as we broaden scope.
IF(NOT BALL_CORE_ONLY)
	INCLUDE(source/ENERGY/sources.cmake)
	INCLUDE(include/BALL/ENERGY/sources.cmake)

	INCLUDE(source/MOLMEC/sources.cmake)
	INCLUDE(include/BALL/MOLMEC/sources.cmake)

	INCLUDE(source/PYTHON/sources.cmake)
	INCLUDE(include/BALL/PYTHON/sources.cmake)

	INCLUDE(source/QSAR/sources.cmake)
	INCLUDE(include/BALL/QSAR/sources.cmake)

	INCLUDE(source/SCORING/sources.cmake)
	INCLUDE(include/BALL/SCORING/sources.cmake)

	INCLUDE(source/SOLVATION/sources.cmake)
	INCLUDE(include/BALL/SOLVATION/sources.cmake)

	INCLUDE(source/DOCKING/sources.cmake)
	INCLUDE(include/BALL/DOCKING/sources.cmake)
ENDIF()

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

# ===== v2.2 HCP-1 task 0 (D-HC7): KERNEL-only build partition for the =====
# hierarchy-collapse KERNEL-first work. Everything below (PLUGIN + the
# extension modules + PYTHON) is gated behind the DEDICATED flag
# BALL_COLLAPSE_KERNEL_ONLY. With BALL_COLLAPSE_KERNEL_ONLY=ON the build is
# core foundation + KERNEL only; the cluster re-open (HCP-3) removes this guard
# module-by-module in dependency order. NOTE: a dedicated flag (NOT
# BALL_CORE_ONLY) is used deliberately — Track-B turned BALL_CORE_ONLY=ON into
# the de-facto "build everything" config, so reusing it would break the full
# build/CI; this leaves BALL_CORE_ONLY untouched (see V22-H1bPRIME-DESIGN.md
# §10, D-HC7 dedicated-flag resolution).
IF(NOT BALL_COLLAPSE_KERNEL_ONLY)

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

# B2.3 (Track B Wave 2c, 2026-05-18): ENERGY re-enabled unconditionally.
# 5 sources (atomicContactEnergy, composedEnergyProcessor, coulomb,
# distanceCoulomb, energyProcessor). Deps: CONCEPT, DATATYPE, KERNEL,
# MATHS, STRUCTURE (Wave 1b subset, in — uses geometricProperties,
# geometricTransformations only), SYSTEM. No MOLMEC/QSAR/FORMAT dep —
# the energy processors are pure functional algorithms over atom
# positions/charges. Provides ENERGY tests: AtomicContactEnergy_test,
# ComposedEnergyProcessor_test, EnergyProcessor_test, OoiEnergy_test.
INCLUDE(source/ENERGY/sources.cmake)
INCLUDE(include/BALL/ENERGY/sources.cmake)

# Cluster A (2026-05-19): SCORING + MOLMEC re-enabled as a single
# pair-landing. Discovery: MOLMEC/COMMON/forceFieldComponent inherits
# SCORING/COMMON/ScoringComponent; SCORING/COMMON/scoringComponent.C
# is clean (no DOCKING/QSAR). The dirty SCORING/COMMON files
# (scoringFunction.C, scoringOptimizer.C, diffScoringFunction.C,
# diffGridBasedScoring.C, rescorer.C) pull DOCKING + QSAR — gated in
# their own sources.cmake. MOLMEC/AMBER + MMFF94 pull QSAR
# transitively — gated in MOLMEC/sources.cmake. The remaining MOLMEC
# subdirs (CHARMM + COMMON + MINIMIZATION + MDSIMULATION + PARAMETER)
# now link cleanly. Brings SnapShot[Manager] in so FORMAT trajectory
# files (DCDFile, TRRFile, trajectoryFile{,Factory}) can also lift.
INCLUDE(source/SCORING/sources.cmake)
INCLUDE(include/BALL/SCORING/sources.cmake)
INCLUDE(source/MOLMEC/sources.cmake)
INCLUDE(include/BALL/MOLMEC/sources.cmake)

# Conditional: extension modules. Disabled by BALL_CORE_ONLY=ON for the
# v2.0 KERNEL-replacement work (D10 / KERNEL-V2-DECISIONS.md). Re-enabled
# module-by-module as we broaden scope.
#
# Cluster B (planned post-Cluster A): QSAR + DOCKING + SCORING
# COMPONENTS/FUNCTIONS + AMBER + MMFF94. Together they unblock the
# remaining SCORING/COMMON trim, the STRUCTURE bits still gated
# (addHydrogenProcessor, smartsMatcher, etc.), full FORMAT, and the
# remaining v2.1 architectural items.
# Cluster B step 1 (2026-05-19): QSAR re-enabled unconditionally.
# Depends only on KERNEL + STRUCTURE + SYSTEM + CONCEPT + MATHS +
# DATATYPE + FORMAT — all already in CORE_ONLY. Unblocks STRUCTURE's
# smartsMatcher/atomTyper/kekulizer/ringAnalyser/sdGenerator (CB-2),
# AMBER's GAFFTypeProcessor (CB-5), MMFF94 (CB-5), and SCORING/COMPONENTS
# bits that reference RingPerception (CB-3). Also lifts the
# Expression_test SMARTSPredicate WILL_FAIL since SmartsMatcher
# becomes linkable after CB-2.
INCLUDE(source/QSAR/sources.cmake)
INCLUDE(include/BALL/QSAR/sources.cmake)

# Cluster B step 3 (2026-05-19): SOLVATION re-enabled. Provides FDPB
# (Poisson-Boltzmann) for SCORING/COMPONENTS/PB.C and related.
# Depends on MOLMEC + QSAR — both in.
INCLUDE(source/SOLVATION/sources.cmake)
INCLUDE(include/BALL/SOLVATION/sources.cmake)

# Cluster B step 3 (2026-05-19): DOCKING re-enabled. Provides
# Constraint/PharmacophoreConstraint/SideChainOptimizer for
# SCORING/COMMON/scoringFunction + SCORING/COMPONENTS. Depends on
# MOLMEC + QSAR + STRUCTURE — all in.
INCLUDE(source/DOCKING/sources.cmake)
INCLUDE(include/BALL/DOCKING/sources.cmake)

	# PYTHON (always core-only-gated; now inside the outer guard).
	INCLUDE(source/PYTHON/sources.cmake)
	INCLUDE(include/BALL/PYTHON/sources.cmake)

ELSE()
	# BALL_COLLAPSE_KERNEL_ONLY=ON: assert exactly which modules are excluded
	# so the narrowed build is auditable (H1bP-7).
	MESSAGE(STATUS "BALL_COLLAPSE_KERNEL_ONLY=ON: KERNEL-only build. EXCLUDED "
		"modules: PLUGIN FORMAT STRUCTURE XRAY NMR ENERGY SCORING MOLMEC QSAR "
		"SOLVATION DOCKING PYTHON. (v2.2 hierarchy-collapse HCP-1; re-opened "
		"cluster-by-cluster at HCP-3.)")
ENDIF()

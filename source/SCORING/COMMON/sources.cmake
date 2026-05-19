### the directory name ###
SET(DIRECTORY source/SCORING/COMMON)

### list all filenames of the directory here ###
# Cluster A (2026-05-19): SCORING/COMMON enable so MOLMEC's
# COMMON/forceFieldComponent (which extends ScoringComponent) can link.
#
# Kept (clean in CORE_ONLY):
#   - baseFunction, linearBaseFunction, fermiBaseFunction : pure base
#     classes for scoring weight functions
#   - scoringComponent              : ForceFieldComponent's base
#   - scoringFunction               : 2 methods (createStaticLigandFragments,
#                                     setupFlexibleResidues_'s auto branch)
#                                     are #ifdef BALL_CORE_ONLY-trimmed
#                                     internally — they pull QSAR's
#                                     RingPerceptionProcessor and
#                                     DOCKING's SideChainOptimizer. All
#                                     other ScoringFunction methods are
#                                     available, satisfying scoringComponent's
#                                     `scoring_function_->getExpEnergyStddev()`
#                                     link requirement.
#   - diffScoringFunction           : only includes MOLMEC, no DOCKING/QSAR
#   - scoreGridSet                  : PharmacophoreConstraint is forward-
#                                     declared + stored as pointer; no
#                                     method calls
#
# Trimmed in CORE_ONLY (need DOCKING at link time):
#   - gridBasedScoring.C            : uses Constraint hierarchy
#   - diffGridBasedScoring.C        : DOCKING/structurePreparer
#   - rescorer.C                    : DOCKING/dockingAlgorithm
#   - scoringOptimizer.C            : DOCKING/structurePreparer
# Revised set (build attempt 3): scoringFunction.C transitively pulls
# DOCKING's Constraint/PharmacophoreConstraint via many method bodies
# (printResult, hasPharmacophoreConstraints_, updateScore,
# clearStoredInteractions_, enableStoreInteractionsOnlyForPhContraints).
# scoreGridSet.C calls GridBasedScoring::getAtomTypesMap.
# Both too entangled to ifdef inline — trim them in CORE_ONLY.
# ScoringComponent's one ScoringFunction->getExpEnergyStddev() call
# is the only blocker; wrapped in #ifndef BALL_CORE_ONLY in
# source/SCORING/COMMON/scoringComponent.C.
# Cluster B step 3 (2026-05-19): scoringFunction restored. Its
# two QSAR/DOCKING-dependent code paths (createStaticLigandFragments
# uses RingPerception, setupFlexibleResidues_'s automatic branch
# uses SideChainOptimizer) need ifdef gating since DOCKING is still
# in CB-7. We restore the .C file but expect DOCKING-pulling
# methods to be guarded — see source/SCORING/COMMON/scoringFunction.C
# for the gates. scoreGridSet/gridBasedScoring/diffScoringFunction
# depend on scoringFunction → now linkable.
# Cluster B step 3 (2026-05-19): full SCORING/COMMON restored
# now that DOCKING is in. All files compile + link.
SET(SOURCES_LIST
	baseFunction.C
	diffGridBasedScoring.C
	diffScoringFunction.C
	fermiBaseFunction.C
	gridBasedScoring.C
	linearBaseFunction.C
	rescorer.C
	scoreGridSet.C
	scoringComponent.C
	scoringFunction.C
	scoringOptimizer.C
)

ADD_BALL_SOURCES("SCORING/COMMON" "${SOURCES_LIST}")

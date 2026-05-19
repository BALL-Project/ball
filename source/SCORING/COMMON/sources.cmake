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
SET(SOURCES_LIST
	baseFunction.C
	fermiBaseFunction.C
	linearBaseFunction.C
	scoringComponent.C
)

# Cluster A trim list — restored when DOCKING re-enables (Cluster B).
# diffScoringFunction.C extends ScoringFunction; can't link without it.
IF(NOT BALL_CORE_ONLY)
	LIST(APPEND SOURCES_LIST
		diffGridBasedScoring.C
		diffScoringFunction.C
		gridBasedScoring.C
		rescorer.C
		scoreGridSet.C
		scoringFunction.C
		scoringOptimizer.C
	)
ENDIF()

ADD_BALL_SOURCES("SCORING/COMMON" "${SOURCES_LIST}")

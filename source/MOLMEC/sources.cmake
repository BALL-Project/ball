### the directory name ###
SET(DIRECTORY source/MOLMEC)

# Cluster A (2026-05-19): MOLMEC subset. AMBER + MMFF94 need QSAR
# (RingPerception, Aromaticity) which is Cluster B. Without those,
# CHARMM (clean), MINIMIZATION, MDSIMULATION, COMMON (full now that
# SCORING/COMMON subset lands ScoringComponent), PARAMETER all link.
INCLUDE(source/MOLMEC/CHARMM/sources.cmake)
INCLUDE(source/MOLMEC/COMMON/sources.cmake)
INCLUDE(source/MOLMEC/MINIMIZATION/sources.cmake)
INCLUDE(source/MOLMEC/MDSIMULATION/sources.cmake)
INCLUDE(source/MOLMEC/PARAMETER/sources.cmake)
# Cluster B step 3 (2026-05-19): AMBER re-enabled. Its GAFFTypeProcessor
# needs QSAR (CB-1 in). Its amberNonBonded.C has a circular dep with
# SCORING/COMPONENTS/advElectrostatic.h — both restore in the same
# Cluster B step.
INCLUDE(source/MOLMEC/AMBER/sources.cmake)
# Cluster B step 3 (2026-05-19): MMFF94 re-enabled. Needs QSAR
# (CB-1 in) for ring + aromaticity + atom typing.
INCLUDE(source/MOLMEC/MMFF94/sources.cmake)

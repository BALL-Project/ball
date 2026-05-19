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
IF(NOT BALL_CORE_ONLY)
	INCLUDE(source/MOLMEC/AMBER/sources.cmake)
	INCLUDE(source/MOLMEC/MMFF94/sources.cmake)
ENDIF()

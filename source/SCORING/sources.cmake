### the directory name ###
SET(DIRECTORY source/SCORING)

# Cluster A (2026-05-19): SCORING/COMMON has a clean subset (see its
# sources.cmake) that doesn't need DOCKING/QSAR — needed by MOLMEC's
# forceFieldComponent. COMPONENTS + FUNCTIONS pull MOLMEC's MMFF94 /
# AMBER force fields (which need QSAR's RingPerception / Aromaticity)
# so they stay gated on !BALL_CORE_ONLY for Wave 3 cluster B.
INCLUDE(source/SCORING/COMMON/sources.cmake)
# Cluster B step 3 (2026-05-19): SCORING/COMPONENTS + FUNCTIONS
# re-enabled. COMPONENTS depends on MOLMEC + ENERGY + STRUCTURE;
# FUNCTIONS depends on COMPONENTS + MOLMEC AMBER/MMFF94 + QSAR.
# All deps now in.
INCLUDE(source/SCORING/COMPONENTS/sources.cmake)
INCLUDE(source/SCORING/FUNCTIONS/sources.cmake)

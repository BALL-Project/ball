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

# Conditional: extension modules. Disabled by BALL_CORE_ONLY=ON for the
# v2.0 KERNEL-replacement work (D10 / KERNEL-V2-DECISIONS.md). Re-enabled
# module-by-module as we broaden scope.
IF(NOT BALL_CORE_ONLY)
	INCLUDE(source/ENERGY/sources.cmake)
	INCLUDE(include/BALL/ENERGY/sources.cmake)

	INCLUDE(source/FORMAT/sources.cmake)
	INCLUDE(include/BALL/FORMAT/sources.cmake)

	INCLUDE(source/MOLMEC/sources.cmake)
	INCLUDE(include/BALL/MOLMEC/sources.cmake)

	INCLUDE(source/NMR/sources.cmake)
	INCLUDE(include/BALL/NMR/sources.cmake)

	INCLUDE(source/PLUGIN/sources.cmake)
	INCLUDE(include/BALL/PLUGIN/sources.cmake)

	INCLUDE(source/PYTHON/sources.cmake)
	INCLUDE(include/BALL/PYTHON/sources.cmake)

	INCLUDE(source/QSAR/sources.cmake)
	INCLUDE(include/BALL/QSAR/sources.cmake)

	INCLUDE(source/SCORING/sources.cmake)
	INCLUDE(include/BALL/SCORING/sources.cmake)

	INCLUDE(source/SOLVATION/sources.cmake)
	INCLUDE(include/BALL/SOLVATION/sources.cmake)

	INCLUDE(source/STRUCTURE/sources.cmake)
	INCLUDE(include/BALL/STRUCTURE/sources.cmake)

	INCLUDE(source/DOCKING/sources.cmake)
	INCLUDE(include/BALL/DOCKING/sources.cmake)

	INCLUDE(source/XRAY/sources.cmake)
	INCLUDE(include/BALL/XRAY/sources.cmake)
ENDIF()

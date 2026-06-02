# v2.2 H3a.5 (D-H3.8): handle-key-leak check gate.
#
# Enforces that no file which migrates to the v2.2 handle API (i.e., includes
# BALL/KERNEL/atomHandle.h or BALL/KERNEL/bondHandle.h) ALSO stores raw
# `Atom*` / `Bond*` pointers as container keys. The migration target is the
# StableId / handle key (D-H3.8); a stored getAtom() / getBond() bridge value
# used as a hash/map key would silently re-introduce pointer-identity into the
# migrated path and force a re-churn at H4.
#
# Pre-H3b (now) this gate is a no-op (no consumer includes the handle headers
# yet beyond the handle headers themselves + the handle tests, which the gate
# excludes). It activates per-cluster as consumers migrate in H3b/H3c/...
#
# Invoked via add_test() with -DSOURCE_DIR=<repo root>.

if(NOT DEFINED SOURCE_DIR)
	message(FATAL_ERROR "HandleKeyLeakGate: SOURCE_DIR not set")
endif()

# Scan the kernel-build source + headers (skip the handle headers themselves,
# the handle tests, and the .planning/ artifacts).
file(GLOB_RECURSE KERNEL_SOURCES
	"${SOURCE_DIR}/include/BALL/*.h"
	"${SOURCE_DIR}/include/BALL/*.iC"
	"${SOURCE_DIR}/source/*.C")

set(LEAKS "")
foreach(F IN LISTS KERNEL_SOURCES)
	get_filename_component(BASENAME "${F}" NAME)

	# Skip the handle headers themselves + the focused handle/key tests.
	if(BASENAME MATCHES "^(atomHandle|bondHandle|containerHandle)\\.h$")
		continue()
	endif()
	if(BASENAME MATCHES "^(AtomHandle_test|BondHandle_test|ContainerHandle_test)\\.C$")
		continue()
	endif()

	file(READ "${F}" CONTENT)
	# Trigger only on files that opted into the v2.2 handle API.
	if(NOT CONTENT MATCHES "BALL/KERNEL/(atomHandle|bondHandle)\\.h")
		continue()
	endif()

	# Anti-pattern: a raw `Atom*` / `Bond*` (incl. `const Atom*`) used as the
	# KEY position of a hash/map/set/vector container — i.e., the stored
	# pointer-identity key the D-H3.8 stable-id policy forbids. The first
	# template parameter of these containers is the key (or the element for
	# set/vector); we conservatively flag any of them when the type is a
	# bare pointer to Atom/Bond.
	if(CONTENT MATCHES
		"(HashSet|HashMap|StringHashMap|std::set|std::map|std::unordered_set|std::unordered_map|std::vector)[ \t]*<[ \t]*(const[ \t]+)?(Atom|Bond)[ \t]*\\*")
		list(APPEND LEAKS "${F}")
	endif()
endforeach()

if(LEAKS)
	string(REPLACE ";" "\n  " LEAK_LIST "${LEAKS}")
	message(FATAL_ERROR
		"H3a.5 check gate FAILED -- the following file(s) include "
		"BALL/KERNEL/atomHandle.h or BALL/KERNEL/bondHandle.h AND still store "
		"raw Atom*/Bond* as a container key. Per D-H3.8, migrated consumers "
		"must key on StableId / AtomHandle / BondHandle (std::hash + "
		"operator< are provided), NOT on stored getAtom()/getBond() bridge "
		"pointers (which re-churn at H4):\n  ${LEAK_LIST}")
endif()

message(STATUS "HandleKeyLeakGate PASS: 0 leaks across ${SOURCE_DIR}")

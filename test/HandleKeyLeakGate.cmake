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
# v2.2 H4 Codex H4-DR R8 F3 fix (D-H4.16 GATE-TIGHTENING): per-pair
# scan. The original gate only checked a file against itself -- if a
# .C file opts into atomHandle.h but stores raw Atom*/Bond* in a
# SIBLING .h (same basename), the original gate missed it. The
# tightened gate also scans the sibling .h when a .C file opts in.
# Documented allow-list captures known-pre-existing violations that
# need H3d-style migration commits to remove; the allow-list is the
# audit ledger so the gate stays honest about coverage.
set(GATE_ALLOWLIST
	# Codex H4-DR R8 F3 known-pending sites surfaced by gate tightening.
	# Each entry will retire under its own bounded migration commit.
	# The audit ledger PDB-CONSUMER-AUDIT.md / ATOMCONTAINER-AUDIT.md
	# captures these as part of the STRUCTURE H4 cluster.
	"include/BALL/STRUCTURE/assignBondOrderProcessor.h"     # H4 7b.9 — HashMap<Atom*,int> + vector<Atom*>
	"source/STRUCTURE/assignBondOrderProcessor.C"           # H4 7b.9 — sibling .C
	"source/STRUCTURE/kekulizer.C"                          # H4 7b.9 — vector<Atom*> via using namespace std
	"include/BALL/STRUCTURE/kekulizer.h"                    # H4 7b.9 — sibling .h
	"include/BALL/STRUCTURE/BONDORDERS/partialBondOrderAssignment.h"  # H4 7b.9 — BONDORDERS cluster
	"source/STRUCTURE/BONDORDERS/partialBondOrderAssignment.C"        # H4 7b.9 — BONDORDERS cluster
	"include/BALL/STRUCTURE/addHydrogenProcessor.h"         # H4 7b.10 — addHydrogen cluster
	"source/STRUCTURE/addHydrogenProcessor.C"               # H4 7b.10 — addHydrogen cluster
	"include/BALL/STRUCTURE/atomTyper.h"                    # H4 7b.11 — atomTyper cluster
	"source/STRUCTURE/buildBondsProcessor.C"                # H4 7b.12 — buildBonds cluster
	"source/STRUCTURE/geometricProperties.C"                # H4 7b.13 — geometricProperties cluster
	"source/STRUCTURE/hybridisationProcessor.C"             # H4 7b.14 — hybridisation cluster
	# v2.2 H4 R3.3: sites surfaced by the deque/list widening (Codex R2-N2).
	# DNAMutator MIGRATED at 7b.15 (deque<AtomHandle>) -- off the allowlist.
	"source/STRUCTURE/reconstructFragmentProcessor.C"      # H4 7b.16 — reconstructFragment list<Atom*>
	"include/BALL/STRUCTURE/reconstructFragmentProcessor.h" # H4 7b.16 — reconstructFragment list<Atom*>
)
foreach(F IN LISTS KERNEL_SOURCES)
	get_filename_component(BASENAME "${F}" NAME)

	# Skip the handle headers themselves + the focused handle/key tests.
	# v2.2 H4 R3.3: also skip extractorsHandle.h -- it is a handle header
	# (the D-H3.8 migration TARGET, returns std::vector<AtomHandle>); its
	# only match is a DOC COMMENT mentioning the v0 "std::list<Atom*>"
	# return type it replaces. The gate is a text scanner and can't tell
	# comment from code, so the handle headers are excluded by name.
	if(BASENAME MATCHES "^(atomHandle|bondHandle|containerHandle|extractorsHandle)\\.h$")
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
	# v2.2 H4 R3.3 (Codex R2-N2): container alternation now covers the full
	# STL spelling set including deque/list/forward_list/multiset/multimap
	# -- the original missed std::deque<Atom*> / std::list<Atom*> (e.g.
	# DNAMutator.h). Pointer-identity in ANY of these forbidden by D-H3.8.
	if(CONTENT MATCHES
		"(HashSet|HashMap|StringHashMap|std::(set|map|unordered_set|unordered_map|multiset|multimap|vector|deque|list|forward_list))[ \t]*<[ \t]*(const[ \t]+)?(Atom|Bond)[ \t]*\\*")
		# v2.2 H4 R3.3: consult the allowlist here too. Widening the
		# alternation to std::list/std::deque means qualified-form leaks in
		# known-pending files (geometricProperties.C etc.) now reach THIS
		# branch, not just the unqualified one below; without the allowlist
		# check they would hard-fail the gate.
		set(ALLOWED FALSE)
		foreach(ALLOW IN LISTS GATE_ALLOWLIST)
			if(F MATCHES "${ALLOW}$")
				set(ALLOWED TRUE)
				break()
			endif()
		endforeach()
		if(NOT ALLOWED)
			list(APPEND LEAKS "${F}")
		endif()
	endif()

	# v2.2 H4 Codex F3 fix: also scan the sibling .h file (same
	# basename) so a .C opting in via atomHandle.h can't hide its
	# stored pointer-identity keys in a public header. The sibling
	# scan uses a WIDENED regex that ALSO catches unqualified
	# `vector<Atom*>` / `HashMap<Atom*,...>` -- the H3c CR finding 3
	# pattern -- because public headers are the most damaging place
	# for a stored pointer key to hide.
	if(BASENAME MATCHES "\\.C$")
		string(REGEX REPLACE "\\.C$" ".h" SIB_BASENAME "${BASENAME}")
		string(REGEX REPLACE "/source/" "/include/BALL/" SIB_PATH "${F}")
		string(REGEX REPLACE "\\.C$" ".h" SIB_PATH "${SIB_PATH}")
		if(EXISTS "${SIB_PATH}")
			file(READ "${SIB_PATH}" SIB_CONTENT)
			if(SIB_CONTENT MATCHES
				"(HashSet|HashMap|StringHashMap|std::(set|map|unordered_set|unordered_map|multiset|multimap|vector|deque|list|forward_list)|[^A-Za-z_](set|map|unordered_set|unordered_map|multiset|multimap|vector|deque|list|forward_list))[ \t]*<[ \t]*(const[ \t]+)?(Atom|Bond)[ \t]*\\*")
				# Check allow-list before flagging.
				set(ALLOWED FALSE)
				foreach(ALLOW IN LISTS GATE_ALLOWLIST)
					if(SIB_PATH MATCHES "${ALLOW}$")
						set(ALLOWED TRUE)
						break()
					endif()
				endforeach()
				if(NOT ALLOWED)
					list(APPEND LEAKS "${SIB_PATH} (sibling of ${F})")
				endif()
			endif()
		endif()
	endif()

	# v2.2 H4 Codex F3 fix: also catch the H3c CR finding 3 pattern
	# (unqualified container<Atom*>) when the file itself opts in.
	# The narrower scope of this check (own-file only, not transitive)
	# matches the original gate's contract; the new sibling-.h scan
	# above is the widened part.
	if(CONTENT MATCHES
		"[^A-Za-z_](set|map|unordered_set|unordered_map|multiset|multimap|vector|deque|list|forward_list)[ \t]*<[ \t]*(const[ \t]+)?(Atom|Bond)[ \t]*\\*")
		# Check allow-list before flagging.
		set(ALLOWED FALSE)
		foreach(ALLOW IN LISTS GATE_ALLOWLIST)
			if(F MATCHES "${ALLOW}$")
				set(ALLOWED TRUE)
				break()
			endif()
		endforeach()
		if(NOT ALLOWED)
			list(APPEND LEAKS "${F} (unqualified container<Atom*/Bond*>)")
		endif()
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

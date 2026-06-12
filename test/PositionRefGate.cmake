# v2.2 Option-A P0.2 (V22-ATOM-HANDLE-SOA-DESIGN-RECONCILE §8): the
# mutable-Vector3& position/velocity/force API growth gate.
#
# Option A keeps AoS Vector3 storage, so the reference-returning column
# accessors (Vector3& getPosition() / position(i) / ...) SURVIVE -- that
# is the whole point (it dodges the 783-site break Option B would force).
# But the API surface that an eventual Option-B SoA migration would have
# to retire must NOT GROW while H4 lands: every new mutable-Vector3&
# position/velocity/force accessor adds to that future blast radius.
#
# This gate freezes the count of such DECLARATIONS in include/BALL/ at
# its baseline. Removals are fine (H4 commit 8 deletes the 3 Atom facade
# accessors -> count drops); ADDING a new one fails CI. New code should
# use the P0.1 span/range API or value setters instead.
#
# Invoked via add_test() with -DSOURCE_DIR=<repo root>.
#
# KNOWN LIMITATIONS (accepted, like HandleKeyLeakGate): this is a text
# matcher, not a C++ parser. It reliably catches the realistic threat --
# ACCIDENTAL growth, i.e. someone adding a normal
# `Vector3& getFoo()` accessor. It does NOT catch deliberately-obfuscated
# forms: a declaration split across two physical lines, a `typedef Vector3
# Vec3;` alias, a `#define`d type, or a `template<class T> T& foo()`. Those
# are a code-review concern, not an automated-gate one; making the gate
# bullet-proof would require a real parser and is out of scope. The gate's
# value is freezing the *ordinary* API surface while H4 lands.

if(NOT DEFINED SOURCE_DIR)
	message(FATAL_ERROR "PositionRefGate: SOURCE_DIR not set")
endif()

# Baseline at lock time (HEAD f84d4127c + P0.1):
#   include/BALL/KERNEL/moleculeStore.h : position/velocity/force(Index)  = 3 (durable)
#   include/BALL/KERNEL/atom.h          : getPosition/getVelocity/getForce = 3 (facade, deleted H4 c8)
set(BASELINE 6)

file(GLOB_RECURSE HEADERS
	"${SOURCE_DIR}/include/BALL/*.h"
	"${SOURCE_DIR}/include/BALL/*.iC")

set(HITS "")
set(COUNT 0)
foreach(F IN LISTS HEADERS)
	file(READ "${F}" CONTENT)
	# Match a real DECLARATION: a newline, optional indentation, optional
	# `virtual`, then a bare `Vector3&` (the pattern anchors on the
	# preceding char NOT being a word/`>` so `const Vector3&` and
	# `... (Vector3&` inside a comment are excluded), a position/velocity/
	# force accessor name, and an opening paren. REGEX MATCHALL over the
	# whole file content avoids line-splitting fragility.
	# Codex P0-review: broaden the common-accidental coverage --
	#   * allow leading qualifiers (inline / virtual / static / BALL_INLINE / ...)
	#     via a (word + space)* prefix, and
	#   * allow `Vector3 &name` (space before &) as well as `Vector3& name`.
	# `const` matches the qualifier prefix too, so const-ref accessors are
	# filtered out below (they are read-only -- not the Option-B blast
	# radius). Deliberately-obfuscated forms (split-line / typedef alias /
	# macro / .C-only) remain a code-review concern per the header note.
	string(REGEX MATCHALL
		"\n[ \t]*([A-Za-z_][A-Za-z0-9_]*[ \t]+)*Vector3[ \t]*&[ \t]*(getPosition|getVelocity|getForce|position|velocity|force)[ \t]*\\("
		MATCHES "${CONTENT}")
	foreach(M IN LISTS MATCHES)
		# Exclude const-ref accessors (read-only; they survive Option B).
		if(M MATCHES "(^|[^A-Za-z0-9_])const[ \t]")
			continue()
		endif()
		math(EXPR COUNT "${COUNT} + 1")
		string(STRIP "${M}" MSTRIP)
		list(APPEND HITS "${F}: ${MSTRIP}")
	endforeach()
endforeach()

if(COUNT GREATER BASELINE)
	string(REPLACE ";" "\n  " HIT_LIST "${HITS}")
	message(FATAL_ERROR
		"PositionRefGate FAILED: ${COUNT} mutable-Vector3& position/velocity/"
		"force accessor declarations in include/BALL/ (baseline ${BASELINE}). "
		"A NEW reference-returning position/velocity/force accessor was added; "
		"this grows the Option-B (SoA) retirement blast radius. Use the P0.1 "
		"span/range API (MoleculeStore::positions()/forces()/AtomRange) or a "
		"value setter (setPosition(idx, v)) instead. Matches:\n  ${HIT_LIST}")
endif()

message(STATUS "PositionRefGate PASS: ${COUNT} mutable-Vector3& position/velocity/force accessors (baseline ${BASELINE})")

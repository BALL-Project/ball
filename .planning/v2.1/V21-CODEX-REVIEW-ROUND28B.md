# Track A v2.1 Cluster R28b — Codex CLI Round 28b (2026-05-20)
**Status:** Complete
**Verdict:** NEEDS-FIXES
**Reviewer:** Codex CLI 0.128.0
**Subject:** v2.1.0-rc1 pre-tag re-check (R28 fix verification)

## Probe answers

**P28b-1. P4.2 deferred references:** Mostly corrected. `BACKLOG.md`
marks `V21-BOND-PROPERTY-JSON` CLOSED in v2.1 P4.2 and says it is no
longer outstanding. `V21-ROADMAP.md` P4.2 marks it IMPLEMENTED
(D42→D42-REV). `V21-DECISIONS.md` has a D42-REV amendment and D43
CLOSED note. The remaining "deferred to v2.2" language for bond
properties is inside explicitly historical/superseded D42/D43 text.

**P28b-2. RTTI count:** The corrected release-note count is internally
consistent: 24 dynamic_cast/isKindOf + 5 castTo = 29; 26 via helper + 3
dropped = 29. Code grep in CORE_ONLY modules minus
`source/CONCEPT/composite.C` found zero remaining exact Atom RTTI
matches for `dynamic_cast<Atom...>`, `isKindOf<Atom>`, or
`castTo<Atom>`. The only exact Atom casts are the centralized helper in
`composite.C`; remaining CORE_ONLY hits are PDBAtom/AtomContainer or
comments, not the claimed Atom RTTI surface.

**P28b-3. Forward-compat claim:** The corrected release-note claim is
accurate against `source/KERNEL/moleculeStoreJson.C`: the loader gates on
MAJOR `format_version` only and treats `format_minor` as optional, with
higher-minor docs loadable and unknown top-level keys ignored. A v2.0
reader therefore loads v2.1 JSON and silently drops v2.1-only per-bond
properties. However, this correction is not complete across the required
docs: `V21-DECISIONS.md` D30 still says v2.0 readers fail cleanly with a
schema MINOR ParseError and that v2.1 JSON does not load in v2.0;
`V21-ROADMAP.md` P4.4 repeats "v2.0 readers fail cleanly on v2.1 JSON
(per D30)."

**P28b-4. Perf headline:** Release notes and milestone context are
consistent at 7-14x, with milestone context explicitly noting the ~16x
single-run no-props figure. The remaining `16x load` text in
`V21-DECISIONS.md` is inside the superseded historical D42 impact block,
not current release framing.

**P28b-5. New contradiction:** Yes. After the release-note fix, the
release notes now correctly say v2.0 readers load v2.1 JSON lossily, but
`V21-DECISIONS.md` D30 and `V21-ROADMAP.md` P4.4 still say v2.0 readers
reject/fail cleanly. That leaves a doc-integrity contradiction in the
required pre-tag set.

**P28b-6. Overall:** NEEDS-FIXES before tagging.

## Overall verdict

Do not tag `v2.1.0-rc1` yet. The P4.2 implemented/closed fix and RTTI
count fix check out. The release-note forward-compat text is now correct
against code, but the same false forward-compat claim still exists in
`V21-DECISIONS.md` D30 and `V21-ROADMAP.md` P4.4. Fix those two doc
references to match the major-only gate / lossy-load behavior, then this
focused re-check should be GO.

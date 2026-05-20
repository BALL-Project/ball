# Track A v2.1 Cluster R28c — Codex CLI Round 28c (2026-05-20)
**Status:** Complete
**Verdict:** NEEDS-FIXES
**Reviewer:** Codex CLI 0.128.0
**Subject:** v2.1.0 final-tag confirmation

## Checks

1. **Forward-compat claim:** PASS. The current release-facing/planning docs no longer claim that v2.0 readers reject or fail cleanly on v2.1 JSON. `RELEASE-NOTES-v2.1.md`, `V21-DECISIONS.md` D30 correction, and `V21-ROADMAP.md` P4.4 consistently say v2.0 readers load v2.1 JSON and silently drop v2.1-only per-bond `properties` (lossy/graceful, not a hard reject).

2. **P4.2 / V21-BOND-PROPERTY-JSON status:** PASS. `RELEASE-NOTES-v2.1.md`, `MILESTONE-CONTEXT-v2.1.md`, `V21-ROADMAP.md` P4.2, and `BACKLOG.md` consistently mark bond-property JSON + Bond* graph reconstruction implemented/closed in v2.1 P4.2. `V21-DECISIONS.md` keeps the original D42 deferral text only as explicitly superseded historical rationale under D42-REV.

3. **RTTI count:** PASS. The release-facing count is internally consistent: 24 `dynamic_cast`/`isKindOf` sites plus 5 `RTTI::castTo<Atom>` sites = 29 Atom-RTTI uses; 26 route through `detail::compositeAsAtom_`, and 3 redundant casts were dropped. The CORE_ONLY grep finds only the documented helper body/comments in `source/CONCEPT/composite.C`.

4. **Perf headline:** NEEDS-FIX. The release notes and milestone context correctly use the 7-14x headline and qualify the ~16x figure as a single-run no-props result. However, `V21-DECISIONS.md` D44 still says, unqualified, `v2.1 still ships honestly per D40: JSON load 16× + bond round-trip + RTTI cleanup...`. That violates the final consistency rule that 16x only appears when explicitly qualified as single-run.

5. **No new contradictions / final sanity scan:** PASS except for the D44 perf wording above. `.github/workflows/ci-v2.yml`'s D41.1 grep gate catches `dynamic_cast<Atom*>`, `dynamic_cast<const Atom*>`, `isKindOf<Atom>`, and `castTo<Atom>` in the CORE_ONLY source module set; the only exception is `source/CONCEPT/composite.C`, which is the documented helper body. The visible cross-references in `RELEASE-NOTES-v2.1.md` and `MILESTONE-CONTEXT-v2.1.md` resolve to existing files.

## Overall verdict

Do not tag `v2.1.0` final yet. This is a narrow documentation consistency fix, not a code blocker: update `V21-DECISIONS.md` D44 to use the 7-14x headline or explicitly qualify the 16x wording as the single-run no-props figure. After that one-line correction, the final tag should be GO.

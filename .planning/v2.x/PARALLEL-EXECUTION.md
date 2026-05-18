# BALL 2.0 Parallel Execution Plan

**Status:** Authored 2026-05-18 as the concurrency model for the
remaining v2.0 work.

**Companion docs:**
- `V2.0-ROADMAP.md` — full phase graph + timeline
- `MODULE-REENABLE-PLAN.md` — module dependency matrix
- `K0-CODEX-REVIEW-ROUND2.md` — finding-driven K0.3c insertion

---

## 1. Parallelism principles

1. **Per-track file ownership.** Each parallel track owns a disjoint
   set of files. No two concurrent commits touch the same `.h` /
   `.iC` / `.C`.
2. **Atomic commits per checkpoint.** Each track lands as one commit
   when its gate (test green + scope contract) is met.
3. **Merge cadence: rebase-then-push.** Workers rebase their feature
   branch on `v2.0` HEAD before pushing the atomic commit. Conflicts
   are rare by construction (file ownership) but the rebase enforces a
   linear history.
4. **Green-bar invariant.** After every merge to `v2.0`, the 36-test
   K0 sweep + `MoleculeStore_test` must pass. Tracks include their own
   sweep before pushing.
5. **Roll back, don't fix forward, on red bar.** If a merge breaks the
   sweep, revert immediately and re-investigate.

---

## 2. K0.3c — first parallel window (1-2 weeks)

K0.3c.1 + K0.3c.2 are prerequisites; they touch `MoleculeStore`'s
mutation surface and serialise. After both land, the remaining four
subphases run in parallel:

```
Sequence:  K0.3c.1 ──► K0.3c.2 ──┬──► K0.3c.3 ──┐
                                  ├──► K0.3c.4 ──┤
                                  ├──► K0.3c.5 ──┼──► merge gate
                                  └──► K0.3c.6 ──┘
```

| Track | Subphase | Files owned | Tests added |
|---|---|---|---|
| T-K0.3c.1 (lead) | Orphan reclaim | `moleculeStore.{h,C}`, `atom.{h,C}` (destructor) | `OrphanStoreReclaim_test` |
| T-K0.3c.2 (lead) | Bond removal + CSR tombstones | `moleculeStore.{h,C}`, `bond.C` (destructors), `atom.C` (destroyBond*) | `BondShadowConsistency_test` |
| T-K0.3c.3 | Bond store-awareness | `bond.{h,C}`, `bond.iC` | `BondMetadataDrift_test` |
| T-K0.3c.4 | Atom::swap bond mirror | `atom.C` (swap only) | extend `Atom_test1` swap CHECK |
| T-K0.3c.5 | Persistence bond mirror | `atom.C` (persistentRead only), `bond.C` (persistentRead only) | extend `PersistenceManager_test` round-trip |
| T-K0.3c.6 | Mutable-ref gap test | `atom.iC` (proxy spike — risk: drops to lock-only if proxy doesn't work) | `MutableRefDriftGap_test` |

**Coordination notes:**
- T-K0.3c.1 + T-K0.3c.2 both touch `moleculeStore.h`. Do .1 first; .2
  rebases on .1.
- T-K0.3c.3 touches `bond.h` for the new `bond_record_idx_` field —
  pin it BEFORE K0.3c.5 starts mirroring through `Bond`, otherwise
  .5's mirror call sites collide with .3's API.
- T-K0.3c.6 is the only track that may need to spike before committing
  (the proxy approach might not work). Fallback: gap acceptance test
  + release-note line is always safe.

---

## 3. K0.4 + K0.5 + K0.6a parallel window (3-4 weeks)

After K0.3c lands. Three tracks run in parallel:

| Track | Scope | Files owned |
|---|---|---|
| T-K0.4 | `System::adopt(Atom&)` + per-System store | `system.{h,C}`, `atom.C` (adopt callback wire-up), `atomContainer.C` (insert callbacks) |
| T-K0.5-design | Compiled selection spec | New file `.planning/v2.x/COMPILED-SELECTION-SPEC.md` |
| T-K0.6a | Persistence format spec | New file `.planning/v2.x/STORE-FORMAT-SPEC.md` |

T-K0.5-design and T-K0.6a are pure-doc tracks — no source changes. They
exist as "design preludes" to their respective implementation
subphases. Can be authored entirely in parallel with T-K0.4.

---

## 4. K0.3b.LATER per-field flip — assignable parallel

**Bottleneck:** all flips touch `atom.h`'s class declaration (private
section). Naive parallel work would conflict.

**Workaround:**
1. **Pre-allocate the header.** Single commit that adds visual
   "MIGRATION CHECKPOINT" comments delimiting each field's position in
   the private section. Workers patch the comment range only.
2. **Per-field flip = atomic ~10-line commit.** Pattern:
   - Delete the v1.x field (e.g. `Vector3 position_;`)
   - Delete from all 3 ctor init lists
   - Flip getter to read from store: `return store_->position(store_idx_);`
   - Delete dual-write call sites for that field
   - Build + test
   - Commit

| Field | Owner | Estimate |
|---|---|---|
| position | T-flip-1 | 1 day |
| charge | T-flip-2 | 1 day |
| velocity | T-flip-3 | 1 day |
| force | T-flip-4 | 1 day |
| name (string pool) | T-flip-5 | 1.5 day |
| type_name (string pool) | T-flip-6 | 1.5 day |
| element (PTE roundtrip) | T-flip-7 | 2 day |
| radius | T-flip-8 | 1 day |
| type (atom_type) | T-flip-9 | 1 day |
| formal_charge | T-flip-10 | 1 day |

**With 3-4 workers**, all 10 flips land in ~3 days serial-equivalent
time (workers rebase on each other's flips; collision rate ≤10%).

---

## 5. K0.5 + K0.6b implementation parallel window (4-5 weeks)

After K0.4 + K0.3b.LATER + K0.5-design + K0.6a land.

| Track | Scope | Files |
|---|---|---|
| T-K0.5-impl | Compiled selection rewrite | `compiledExpression.{h,C}` (new); `expression.{h,C}`, `selector.{h,C}`, `expressionTree.{h,C}`, `expressionParser.{h,C}` (rewrites) |
| T-K0.6b | Persistence v2 + converter | `storeFormat.{h,C}` (new); `ball-convert-v1-v2.C` (new in APPLICATIONS/TOOLS) |

Disjoint file sets. Truly parallel.

---

## 6. Track B (module re-enable) parallel windows

After KERNEL v2.0 ships.

### B0 — PLUGIN smoke test
Single commit. No parallelism needed.

### B1 — FORMAT + STRUCTURE parallel
Both touch `cmake/BALLIncludes.cmake` (the `IF(NOT BALL_CORE_ONLY)`
block). One worker reshapes the CMake file to expose both modules;
the other works on source-side fixes.

| Track | Module | Estimate |
|---|---|---|
| T-B1a | FORMAT (40 sources, 9 downstream unblocks) | ~5 days |
| T-B1b | STRUCTURE (77 sources, 10 downstream unblocks) | ~10 days |

### B2 — XRAY + NMR + ENERGY 3-way parallel
After B1. Each track owns one module. Independent files. No CMake
conflict if all three workers share a single `cmake/BALLIncludes.cmake`
edit (then divide source-side work).

### B3 — MOLMEC alone
Hub module. Single track, but multiple workers can split sub-modules
(MMFF94, AMBER, CHARMM, MINIMIZATION).

### B4 — QSAR + SOLVATION + SCORING (partial parallel)
QSAR + SOLVATION in parallel; SCORING serialises behind both.

### B5 — DOCKING + PYTHON
DOCKING first (~1 week); PYTHON last (~3-4 weeks).

---

## 7. Merge cadence + integration testing

**Every merge to `v2.0`** triggers:
1. Full 36-test sweep (~1 minute)
2. `MoleculeStore_test` (~5 seconds)
3. Bench regression check (`KernelCreation_bench` median of 3 runs)
4. Memory check (peak RSS of full sweep < 200 MB)

**Every phase completion** triggers:
1. Tri-OS CI matrix (macOS-arm64 + Linux-x64 + Windows-x64)
2. ABI diff against previous phase's libBALL (D6 enforcement)
3. Memory benchmark snapshot recorded in `baseline/post-<phase>.txt`

**Phase sign-off:** annotate the merge commit with which Codex round
was last run + whether it surfaced new findings.

---

## 8. Suggested kick-off (right now, this session)

Start three tracks immediately:

**Track 1: K0.3c.1 — orphan store reclaim** (sequential lead)
- File ownership: `moleculeStore.{h,C}` (add `release_atom`), `atom.C` (~Atom destructor wire-up), new `OrphanStoreReclaim_test`
- ~3 days at 1 FTE
- Gate: existing 36-test sweep still green + new test green + orphan
  store size < 5 after full sweep

**Track 2: STORE-FORMAT-SPEC.md (K0.6a preface)** (doc-only, can run NOW)
- File ownership: `.planning/v2.x/STORE-FORMAT-SPEC.md` (new)
- Independent of K0.3c
- 2-3 days
- Lands as a planning commit

**Track 3: Compiled selection design (K0.5 preface)** (doc-only, can
run NOW)
- File ownership: `.planning/v2.x/COMPILED-SELECTION-SPEC.md` (new)
- Independent of everything
- 2-3 days
- Lands as a planning commit

After Track 1 finishes (gating prerequisite for K0.3c.3-6), spin up
**Tracks 4-7** for K0.3c.2/3/4/5/6 in parallel.

---

*End of PARALLEL-EXECUTION.md.*

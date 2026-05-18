# K0.6 Adversarial Review — Codex CLI Round 8 (2026-05-18) — FINAL pre-close

**Status:** Complete. Verdict: **fix-first before K0.6 close.** 2 OPEN
issues + 4 FYI items. R7 OPEN-2/3/4/5 all verified closed.

**Reviewer:** Codex CLI 0.128.0 (`codex exec`).
**Subject:** K0.6.1 through K0.6.5b (commits `4433a350b..93f8c8d33`).

---

## Findings

| # | Sev | Probe | Verdict | Rationale |
|---|---|---|---|---|
| 1a | CLOSED + FYI | R7 OPEN-2 stable_id hooks privatised | `restore_stable_ids_for_load_` private + `detail::json_obj_to_store` friended. Stale `friend void loadStoreJSON(...)` still present — not a hole, but narrower would be cleaner. |
| 1b | CLOSED | R7 OPEN-3 document_type discriminator | Store writes "MoleculeStore", System writes "System", both readers reject mismatch. |
| 1c | CLOSED | R7 OPEN-4 parse-error boundary | Both loaders catch json::exception + invalid_argument + out_of_range, rethrow as ParseError. |
| 1d | CLOSED | R7 OPEN-5 is_freed + live_count cross-check | Strict 0/1 + `live_atom_count == count(is_freed==0)` cross-check in store loader. |
| 2 | FYI | K0.6.5b property layer | Non-virtual `pm.PropertyManager::clear()` correct; OBJECT placeholder semantics intentional but sharp (callers checking `getType()==OBJECT` post-load see NONE); BitVector emit is O(size) not O(set-bits), fine for K0.6. |
| 3a | OPEN | System loader: duplicate save-slot rejection happens AFTER state mutation | Molecule + atoms inserted at systemJson.C:223-238 before the duplicate-check rejection at line 253. Worse: duplicate save_idx within ONE molecule's atom_indices is never rejected (saw_in_molecule set AFTER insertion, never checked inside the molecule loop). |
| 3b | OPEN | Loader memory leak on mid-load throw | Pre-created Atoms (line 203-204) leak if a later validation throws. No RAII or two-phase validation. |
| 4 | FYI | per-atom property restore redundant clear | Safe to keep; load-bearing if construction defaults ever change. |
| 5a | OPEN | System loader doesn't restore stable_ids | Store loader does (R7 OPEN-2 fix); System loader validates `stable_ids` column length but skips the restore. Breaks cross-session identity. |
| 5b | FYI | Orphan atom test only checks counts | Add content assertions (names of orphan atoms post-load). |
| 5c | FYI | No combined integration test | "2 molecules + orphan + cross-mol bonds + props at every level" covered only by sum of feature slices. |
| 6 | CLOSED | Schema versioning under K0.6.5b | "properties" is additive-optional; no minor bump required per stated policy. |
| 7 | FYI | Performance | 250k property emits for 50k atoms × 5 props acceptable for K0.6; defer profiling to K0.7. |
| 8 | OPEN | Test coverage gaps | (a) Indexed BitVector properties not write/read tested. (b) Freed-slot property skip not regression-tested. |

---

## Remediation Plan (K0.6.5c — final K0.6 close)

| Item | Fix | Effort |
|---|---|---|
| 3a duplicate-rejection-before-mutation | Pre-validate ALL atom_indices (molecules + system_atom_indices) for uniqueness + ranges in a single pass BEFORE any `new Atom` or `m->insert`. Throws on duplicate / out-of-range without touching state. | 30 min |
| 3b mid-load leak | Add RAII guard: collect pre-created Atom* into a std::vector and delete-all on throw. Or move validation up so no atoms are allocated until validation passes. | 20 min |
| 5a System loader stable_id restore | Add `restore_stable_ids_for_load_` call with the doc's stable_ids vector after the load-loop, using the slot_map to translate save_idx -> fresh_store_idx. (Or write stable_ids per-slot inline with the column-data loop.) | 20 min |
| 8 test coverage | Add: (a) indexed BitVector property round-trip test, (b) freed-slot property-skip test, (c) combined integration test (the one Codex called out). | 30 min |

Total: ~1.5 hours. After K0.6.5c green sweep, K0.6 closes.

FYI items (1a stale friend, 2 OBJECT semantics doc, 4 clear redundancy,
5b/5c test additions, 7 perf) — bundled into K0.6.5c or deferred to
RELEASE-NOTES-v2.0.md.

---

## Verdict (Codex, verbatim)

> Fix first before closing K0.6. R7's original blockers are mostly
> closed, but Round 8 found one real System identity bug and a loader
> robustness issue.
>
> Fix before K0.6 close: restore System stable IDs/reseed next ID,
> reject duplicate live slot placement before insertion, add RAII
> cleanup or two-phase validation before allocating/mutating, and add
> tests for orphan content, indexed BitVector properties, and the
> combined integration case.

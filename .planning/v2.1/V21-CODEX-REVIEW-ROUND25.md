# Track A v2.1 Cluster R25 — Codex CLI Round 25 (2026-05-20)

**Status:** Complete
**Verdict:** NEEDS-FIXES
**Reviewer:** Codex CLI 0.128.0
**Subject:** v2.1 P4 close review (V21-BOND-PROPERTY-JSON + Bond* graph reconstruction)

## Probe answers

**P25-1 RollbackGuard safety:** mostly safe, with one exception. Once molecule/system insertion has started, the guard stays armed until the end (`systemJson.C:417`, `systemJson.C:613-616`) and rollback calls `sys.destroy()` plus `store.clear()` (`systemJson.C:379-390`). Atom destruction calls `destroyBonds()` (`atom.C:325-330`, `atom.C:968`), `destroyBonds()` deletes each auto-deletable `Bond*` (`atom.C:713-727`), and `Bond::~Bond()` removes the store record (`bond.C:137-139`, `bond.C:295-308`). A throw during bond property restore is therefore cleaned up.

There is still a leak path before a new `Bond*` becomes reachable from an atom: `Atom::createBond` allocates `new Bond` inline (`atom.C:648-659`), then `Bond::createBond` can throw `TooManyBonds` before wiring the bond into either atom (`bond.C:78-87`). That allocated `Bond` is not in `atom_by_save_idx`, not reachable from `sys.destroy()`, and not deleted by `Atom::createBond`. This can be hit by JSON whose reconstructed Atom-side degree exceeds `Atom::MAX_NUMBER_OF_BONDS` (12).

**P25-2 Double-wiring / store record duplication:** for unique atom pairs, the new path creates exactly one store record. The loader no longer calls `store.add_bond`; it calls `atom_a->createBond(*atom_b)` (`systemJson.C:597-600`). `Bond::createBond` mirrors to the shared store through `add_bond` and then sets `bond_back_ptr` (`bond.C:123-131`). However, duplicate JSON records for the same atom pair do not create one record per JSON bond; the second call returns the existing bond (`atom.C:648-659`, `bond.C:71-76`), so the second store record is silently dropped.

**P25-3 Order/type consistency:** correct after `setOrder` / `setType`. `createBond` initially mirrors the default `Bond` order/type into the store (`bond.C:126-130`), then the loader applies the JSON values (`systemJson.C:602-603`). The setters dual-write to the pinned store record (`bond.iC:116-120`, `bond.iC:136-140`, `bond.C:239-252`). There is a short internal window where the store has default order/type, but no external observer is exposed in normal control flow; if a later restore step throws, rollback tears the whole partial load down.

**P25-4 `createBond` returns existing bond:** yes, store-level duplicate pairs are legitimate: `MoleculeStore::add_bond` supports duplicate records, and tests exercise duplicate/reverse duplicate removal (`MoleculeStore_test.C` uses duplicate `add_bond(a,b)` / `add_bond(b,a)`). In `loadSystemJSON`, duplicates collapse to the first `Bond*`; the later record overwrites that first bond's order/type/properties and does not increment `live_bond_count`. The code comment documents the collapse (`systemJson.C:591-596`), but documentation does not make it fidelity-safe. This is a must-fix: either reject duplicate pairs in System JSON before mutation, or preserve extra store records separately while making the Atom-side limitation explicit.

**P25-5 Save-side correctness:** correct. The writer emits bond `properties` only when `bond_back_ptr` is non-null and the serialized bag is non-empty (`moleculeStoreJson.C:195-209`). Store-only `saveStoreJSON` and System instances with bonds added only via `store.add_bond` have null `bond_back_ptr`, so they emit no bond properties. That is acceptable because there is no `Bond*` property bag to serialize in those cases.

**P25-6 Backward read (v2.0 JSON in v2.1):** clean. The loader gates bond properties with `if (br.contains("properties"))` (`systemJson.C:604-605`), so v2.0-style bonds without the key load. Because the loader now always calls `createBond`, v2.0 System JSON also gains reconstructed Atom-side `Bond*` graph for unique/degree-valid pairs.

**P25-7 Forward read (v2.1 JSON in v2.0):** not cleanly rejected by the current version policy. Both current readers gate on major `format_version` only (`systemJson.C:199-202`, `moleculeStoreJson.C:287-293`); the store header explicitly says higher minors are accepted and unknown keys ignored (`moleculeStoreJson.h:32-39`). A v2.0 reader with the same minor policy would silently load the rest and drop bond properties, not reject the higher minor. That is consistent with the documented additive-minor policy, but it is data-lossy for forward reads.

**P25-8 `createBond` store-binding precondition:** satisfied for the normal System loader path. The bond phase happens after all molecule atoms and `system_atom_indices` orphans are inserted (`systemJson.C:416-445`), and the slot map is built from each atom's fresh store index (`systemJson.C:456-465`). Therefore both endpoints are live atoms in `sys.getStore()` by the time `createBond` runs. Orphans inserted via `sys.AtomContainer::insert` still adopt into the System store before the bond phase.

**P25-9 Test adequacy:** the new test covers the main happy path: a `createBond` source, non-empty bond properties, post-load `countBonds()`, order, and property values (`SystemJson_test.C:186-245`). Existing tests cover store-direct bonds preserving store-side order/count (`SystemJson_test.C:155-184`) and multiple store-direct bonds across molecules (`SystemJson_test.C:526-573`), but they do not assert Atom-side reconstruction for no-property bonds or multiple bonds per atom. Missing must-cover tests: no-property `Bond*` round-trip with `countBonds()`, duplicate store bond pair behavior, and over-12-degree input rollback/exception behavior. A v2.0-style no-`properties` forged bond test would also lock P25-6.

**P25-10 Overall:** not a clean close yet. The unique-pair happy path is correct, but P4.2 introduces silent store-fidelity loss for duplicate pairs and exposes a leak path on high-degree input.

## New findings

**R25-1 — NEEDS-FIX: System JSON silently collapses duplicate store bond records.**

`MoleculeStore` is a store-level multigraph; duplicate records can exist and can be emitted by `saveSystemJSON` when callers used `store.add_bond` directly. The new System loader routes every bond through `Atom::createBond`, whose contract is one `Bond*` per atom pair. On the second JSON record for the same pair, it returns the first bond and creates no store record. The loader then applies the second record's order/type/properties to the first bond, so `live_bond_count` and per-record data no longer match the JSON.

Required fix before P5: choose an explicit behavior and test it. Best conservative options are either:
- prevalidate System JSON and reject duplicate unordered endpoint pairs with a `ParseError`, preserving the Atom API invariant without silent data loss; or
- preserve duplicate store records separately and only attach `bond_back_ptr` / properties to the one representable `Bond*`, with documented property semantics for duplicate records.

**R25-2 — NEEDS-FIX: high-degree bond reconstruction can leak the freshly allocated `Bond`.**

`Atom::createBond` calls `Bond::createBond(*new Bond, ...)`. If either endpoint already has `MAX_NUMBER_OF_BONDS` bonds, `Bond::createBond` throws `TooManyBonds` before the new `Bond` is wired into an atom. Rollback cannot see that allocation, so it leaks. This is now reachable from `loadSystemJSON` for malformed or store-valid-but-Atom-invalid JSON with degree > 12.

Required fix before P5: avoid the inline raw allocation in the loader path. For example, allocate into a local owning guard, call `Atom::createBond(Bond&, Atom&)`, and release ownership only after successful wiring; or prevalidate per-atom unique-pair degree <= `Atom::MAX_NUMBER_OF_BONDS` before mutation. The safer loader should still test the throw path and assert rollback leaves the destination empty.

## Overall verdict

**NEEDS-FIXES.** The P4.2 happy path is sound: unique bonds create exactly one store record, wire `bond_back_ptr`, restore order/type into the store, and round-trip non-empty bond properties. `ctest -R '^SystemJson_test$' --output-on-failure` passes after rebuilding `SystemJson_test`.

Do not close P4 until the loader handles duplicate pairs and over-degree input deliberately. Both are boundary cases created by choosing Option A, and both touch the exact lifetime/store-consistency invariants P4.2 was supposed to settle.

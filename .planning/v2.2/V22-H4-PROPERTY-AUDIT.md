# V22-H4-PROPERTY-AUDIT — Property persistence migration ledger

**Status:** Audit ledger for H4 commit 6 (per V22-H4-DESIGN.md R7,
D-H4.6 R2).
**Authored:** 2026-06-04.
**Scope:** non-VIEW, non-PYTHON production + test code.

D-H4.6 R2 splits property migration into (a) consumer-surface
(already DONE in H3a) + (b) persistence API break (NEW in commit 6)
+ (c) Selectable base deletion (commits 8-11 onwards). This ledger
covers the (b) work: the JSON + bag serialization API break.

## Audit predicates at HEAD `5d4ae1fe8`

### (A) Mutable-reference surface (PropertyManager&, NamedProperty&, BitVector&)

```
rg -tcpp 'PropertyManager\s*&|NamedProperty\s*&|BitVector\s*&\s' source test include
| exclude source/VIEW, source/PYTHON
```

**Hits: 165 / ~40 files.**

These are the breaking sites at commit 6. Each consumer that took
a mutable reference to NamedProperty / BitVector must migrate to
the visitor-style pattern:

```cpp
// Before H4 (mutable-reference surface):
NamedProperty& p = container.getNamedProperty("foo");
p.setBitVector(...);

// After H4 commit 6 (visitor pattern):
container.eachProperty([](const String& name, NamedPropertyView& view) {
  if (name == "foo") {
    view.update(...);   // value-semantics write through the column
  }
});
```

### (B) Property bag iteration (beginNamedProperty / endNamedProperty / getNamedProperty)

```
rg -tcpp 'beginNamedProperty|endNamedProperty|getNamedProperty' source test include
```

**Hits: 53 / ~25 files.**

These rely on iterator access to the v0 PropertyManager bag.
Replacement contract per D-H4.6 R2:

```cpp
// Before H4:
for (auto it = container.beginNamedProperty();
     it != container.endNamedProperty(); ++it) {
  do_thing(*it);
}

// After H4 commit 6:
container.eachProperty([](const String& name, const NamedPropertyView& view) {
  do_thing(name, view);
});

// or, for keys-only:
auto names = container.propertyNames();
for (const String& n : names) ...;
```

### (C) JSON persistence sites

```
rg -tcpp 'propertyJson|writeProperty|readProperty|PropertyJson' source/KERNEL source/FORMAT
```

**Hits: 9 / ~5 files** (concentrated in `propertyJson.C`,
`moleculeStoreJson.C`, `systemJson.C`).

These are the canonical persistence entry points. Commit 6's
substance is rewriting these 9 sites to:

  - Read/write the store's `property_columns_` + sparse bag
    directly (not via the PropertyManager& v0 bag iterator).
  - Use the per-column visitor pattern for typed property
    columns; use `propertyNames()` for the bag.
  - The JSON WIRE FORMAT is UNCHANGED (H6b is the schema freeze;
    H4 keeps the schema). The JSON round-trip test added in
    commit 1 catches any inadvertent format drift.

## Commit 6 plan

D-H4.6 R2's commit 6 has 3 sub-phases:

  6.a Visitor introduction
       Add `container.eachProperty(visitor)` + `propertyNames()`
       to the handle headers, backed by the existing store
       property_columns_ + sparse bag. Source-compatible with
       a v0 fallback for the dual-existence window.
  6.b Persistence rewrite
       Rewrite the 9 (C) sites in `propertyJson.C` /
       `moleculeStoreJson.C` / `systemJson.C` to use the visitor
       + propertyNames() path. JSON round-trip test (commit 1)
       gates against wire-format drift.
  6.c Consumer migration
       Per-cluster rewrite of the 165 (A) sites + 53 (B) sites.
       Sub-divides by directory:
         6.c.1 KERNEL + STRUCTURE
         6.c.2 MOLMEC + QSAR + SCORING + DOCKING + SOLVATION
         6.c.3 FORMAT + tests

Each sub-commit follows the D-H4.9 per-commit gate.

## Selectable base deletion (out of scope for commit 6)

D-H4.6 R2 also notes the `: public Selectable` base disappears
in the per-class deletion commits (8-11). Selection storage
already lives in `selected_bits_` (v2.1 P1). Consumers already
use `select()` / `deselect()` / `isSelected()` on the handle.
Commit 6 does NOT touch this.

## Drift baseline

Today's counts (HEAD `5d4ae1fe8`):
- (A) mutable-reference: 165 hits
- (B) bag iteration: 53 hits
- (C) JSON persistence: 9 hits
- Total: 227 hits / ~70 files

If commit-6-prep counts drift by more than ±30%, re-run the
classification.

## Reproduction

Run the three predicates above at HEAD; the ledger refreshes
against that snapshot.

---

*Authored 2026-06-04 as the H4 commit 6 audit deliverable per D-H4.6 R2.*

# Track A v2.1 Cluster R17 — Codex CLI Round 17 (2026-05-19)

**Status:** NEEDS-FIXES
**Verdict:** NO-GO
**Reviewer:** Codex CLI 0.128.0
**Subject:** v2.1 P0 design lock (D22-D30 + MILESTONE-v2.1-KICKOFF.md)

## Probe answers

| # | Probe | Verdict | Notes |
|---|---|---|---|
| P17-1 | Composite side-table memory math | BLOCKER | D22's node shape has only `parent_handle`, `first_child`, `next_sibling`, `kind` ([V21-DECISIONS.md:27](V21-DECISIONS.md:27)). Current Composite stores `parent_`, `previous_`, `next_`, `first_child_`, `last_child_` ([include/BALL/CONCEPT/composite.h](/Users/kohlbach/Claude/BALL2/include/BALL/CONCEPT/composite.h:1585)). `last_child_` and `previous_` are API-visible through reverse/bidirectional iterators and sibling queries. The 24 B math is not sufficient. |
| P17-2 | Property column registry growth | BUG | D23 registers by arbitrary `String` name with no cap or admission policy ([V21-DECISIONS.md:73](V21-DECISIONS.md:73)). 1M distinct names means 1M columns, each pre-allocated to store capacity. That is an unbounded memory bomb. |
| P17-3 | Column-per-property sizing | BUG | The float-column math is true only for dense numeric force-field props ([V21-DECISIONS.md:95](V21-DECISIONS.md:95)). Named properties support STRING/OBJECT/SMART_OBJECT/NONE ([include/BALL/CONCEPT/property.h](/Users/kohlbach/Claude/BALL2/include/BALL/CONCEPT/property.h:51)), and v2.0 JSON round-trips STRING scalars ([source/KERNEL/propertyJson.C](/Users/kohlbach/Claude/BALL2/source/KERNEL/propertyJson.C:58)). String columns need explicit sizing and likely interning/dedup policy. |
| P17-4 | Selectable BitVector concurrency | BLOCKER | D24 relies on single-thread mutation ([V21-DECISIONS.md:146](V21-DECISIONS.md:146)), but kickoff itself calls out renderer reads plus GUI writes ([MILESTONE-v2.1-KICKOFF.md:102](MILESTONE-v2.1-KICKOFF.md:102)). Packed BitVector word read/write across threads is a C++ data race unless the design adds snapshot publication, locks, or atomic words. |
| P17-5 | EBO three-empty-base collapse | BUG | Clang/GCC-style ABI is fine for distinct empty bases, but MSVC default multiple-empty-base layout is not. Microsoft documents that default Visual Studio layout does not fully use EBCO in multiple inheritance and requires `__declspec(empty_bases)`. D26's unconditional "EBO collapses them" claim ([V21-DECISIONS.md:231](V21-DECISIONS.md:231)) is not portable. |
| P17-6 | Bond table bond_idx column | BUG | v2.0 Bond is still heap-owned and mirrored into `MoleculeStore` with `bond_record_idx_`/`bond_store_` ([include/BALL/KERNEL/bond.h](/Users/kohlbach/Claude/BALL2/include/BALL/KERNEL/bond.h:461)); JSON serializes live bond records only as `(a,b,order,type,flags)` ([source/KERNEL/moleculeStoreJson.C](/Users/kohlbach/Claude/BALL2/source/KERNEL/moleculeStoreJson.C:183)). D23's "bond properties for free" has no stable identity mapping for bond property columns. |
| P17-7 | Composite insertBefore/After iterator semantics | DEBT | O(1) side-table relinking can preserve current semantics only if iterators keep `Composite*` positions and resolve node links lazily. Current iterators directly rely on `last_child_`, `previous_`, `next_`, `parent_`, `first_child_` ([include/BALL/CONCEPT/composite.h](/Users/kohlbach/Claude/BALL2/include/BALL/CONCEPT/composite.h:1127), [include/BALL/CONCEPT/composite.h](/Users/kohlbach/Claude/BALL2/include/BALL/CONCEPT/composite.h:1362)). D22 must explicitly forbid iterators from caching `CompositeNode*`. |
| P17-8 | D27 sequencing risk | DEBT | D27 says Wave 4-7 closures retarget to v2.1.x and v2.0.x gets only critical fixes ([V21-DECISIONS.md:242](V21-DECISIONS.md:242)), but it does not define the policy for bugs introduced only by v2.1 thin handles. |
| P17-9 | D28 review burden | DEBT | D28 schedules planning+close reviews through P5 plus release close review ([V21-DECISIONS.md:276](V21-DECISIONS.md:276)). P5/P6 planning gates are mostly execution/release verification after P0 design lock; downgrade is reasonable. |
| P17-10 | D30 backward read of v2.0 JSON | BUG | v2.0 stores per-atom embedded `atoms.properties[]` ([source/KERNEL/moleculeStoreJson.C](/Users/kohlbach/Claude/BALL2/source/KERNEL/moleculeStoreJson.C:109)) and System load restores each bag into the Atom handle ([source/KERNEL/systemJson.C](/Users/kohlbach/Claude/BALL2/source/KERNEL/systemJson.C:430)). D30 says v2.1 reads v2.0 JSON ([V21-DECISIONS.md:324](V21-DECISIONS.md:324)) but does not specify demux or mixed-type-name handling. |

## Findings detail

### P17-1: CompositeNode is underspecified and undersized — BLOCKER

D22's proposed `CompositeNode { parent_handle, first_child, next_sibling, kind: u8 }` has three handles plus a discriminator ([V21-DECISIONS.md:27](V21-DECISIONS.md:27), [V21-DECISIONS.md:44](V21-DECISIONS.md:44)). That does not model current `Composite`.

Current `Composite` carries five topology pointers plus child count and selection counters: `parent_`, `previous_`, `next_`, `first_child_`, `last_child_` ([include/BALL/CONCEPT/composite.h](/Users/kohlbach/Claude/BALL2/include/BALL/CONCEPT/composite.h:1585)). `getLastChild()` is public ([include/BALL/CONCEPT/composite.iC](/Users/kohlbach/Claude/BALL2/include/BALL/CONCEPT/composite.iC:79)), `hasPreviousSibling()` is public ([include/BALL/CONCEPT/composite.iC](/Users/kohlbach/Claude/BALL2/include/BALL/CONCEPT/composite.iC:193)), and `insertAfter()` specifically uses `parent_->last_child_` as the O(1) append boundary ([source/CONCEPT/composite.C](/Users/kohlbach/Claude/BALL2/source/CONCEPT/composite.C:835)).

This is not just an optimization detail. `ChildCompositeIteratorTraits::backward()` decrements past-the-end by using `bound_->last_child_`, then walks `previous_` ([include/BALL/CONCEPT/composite.h](/Users/kohlbach/Claude/BALL2/include/BALL/CONCEPT/composite.h:1135)). `CompositeIteratorTraits::findPreviousPosition()` uses both `last_child_` and `previous_` for reverse DFS ([include/BALL/CONCEPT/composite.h](/Users/kohlbach/Claude/BALL2/include/BALL/CONCEPT/composite.h:1362)). Without `last_child` and `prev_sibling`, either reverse traversal regresses to O(children) or behavior changes.

Suggested fix: change D22 to a five-link node:

```cpp
struct CompositeNode {
  CompositeHandle parent;
  CompositeHandle first_child;
  CompositeHandle last_child;
  CompositeHandle next_sibling;
  CompositeHandle prev_sibling;
  uint32_t child_count;
  uint8_t kind;
};
```

Then redo sizing. If handles are 64-bit, topology alone is 40 B per node before `child_count`, `kind`, and padding. If handle is `(kind,u32 idx)` packed into 32 or 40 bits, specify that. P1 must not implement the 24 B / 3-handle node.

### P17-2: Property registry has unbounded adversarial growth — BUG

D23's registry maps arbitrary names to columns and registers at first use ([V21-DECISIONS.md:73](V21-DECISIONS.md:73)). Present `PropertyManager` allows arbitrary unique named properties; `setProperty` linearly replaces by name and otherwise pushes a new `NamedProperty` ([source/CONCEPT/property.C](/Users/kohlbach/Claude/BALL2/source/CONCEPT/property.C:193)). That means the public API already admits untrusted or accidental high-cardinality names.

In the D23 design, the first occurrence of each distinct name allocates a full-capacity column. On a 100k-atom store, 1M distinct float property names is roughly 400 GB of `data_` alone, before BitVectors, hash keys, column objects, allocator overhead, and strings. Even 10k debug-marker names is already a large accidental allocation.

Suggested fix: add an admission policy before P1:

- well-known force-field columns are predeclared/promoted;
- dynamic property names start in a sparse fallback bag keyed by atom/bond idx;
- promotion to dense column requires fill-rate threshold or explicit registration;
- registry has a configurable cap and rejects or keeps sparse beyond it;
- intern property names once per store and reclaim unused dynamic columns on `compact()`/GC.

### P17-3: D23's storage math ignores string columns and mixed property types — BUG

D23's 2 MB + 62.5 KB example is correct for five dense 4-byte numeric columns ([V21-DECISIONS.md:95](V21-DECISIONS.md:95)). It is not a general `PropertyManager` replacement budget.

`NamedProperty` supports BOOL, INT, UNSIGNED_INT, FLOAT, DOUBLE, STRING, OBJECT, NONE, and SMART_OBJECT ([include/BALL/CONCEPT/property.h](/Users/kohlbach/Claude/BALL2/include/BALL/CONCEPT/property.h:51)). v2.0 JSON serializes STRING values directly ([source/KERNEL/propertyJson.C](/Users/kohlbach/Claude/BALL2/source/KERNEL/propertyJson.C:58)) and restores them with `setProperty(name, String(...))` ([source/KERNEL/propertyJson.C](/Users/kohlbach/Claude/BALL2/source/KERNEL/propertyJson.C:133)). A dense `std::vector<String>` column at 100k rows is not close to 4 B/row; it carries per-object SSO/storage overhead even for short strings.

Suggested fix: split D23 into numeric dense columns vs string/object columns. String columns should store interned `uint32_t` offsets into a property string pool, not `String` objects per row, unless benchmarks prove otherwise. OBJECT/SMART_OBJECT should remain sparse or unsupported with the same placeholder semantics as v2.0 JSON.

### P17-4: Packed selection BitVector is racy for VIEW/rendering — BLOCKER

D24 says no atomics because D16 gives single-thread per-System mutation ([V21-DECISIONS.md:146](V21-DECISIONS.md:146)). That does not cover concurrent read/write. The kickoff explicitly describes the selection use case as read in tight rendering loops and written by GUI events ([MILESTONE-v2.1-KICKOFF.md:102](MILESTONE-v2.1-KICKOFF.md:102)). BALLView already has multithreaded paths and composite locking rules ([include/BALL/VIEW/KERNEL/threads.h](/Users/kohlbach/Claude/BALL2/include/BALL/VIEW/KERNEL/threads.h:54)); `GLRenderWindow` documents fresh buffers arriving from a separate renderer thread ([include/BALL/VIEW/RENDERING/glRenderWindow.h](/Users/kohlbach/Claude/BALL2/include/BALL/VIEW/RENDERING/glRenderWindow.h:123)).

A packed `BitVector` makes the race worse than a byte vector: GUI toggling one bit performs a read-modify-write on the containing word while the renderer reads the same word. That is undefined behavior under the C++ memory model unless synchronized. Current v2.0 uses a `std::vector<uint8_t>` selection column in `MoleculeStore` ([include/BALL/KERNEL/moleculeStore.h](/Users/kohlbach/Claude/BALL2/include/BALL/KERNEL/moleculeStore.h:146)); D24 is changing that storage shape and must carry the concurrency contract.

Suggested fix: choose one before P1:

- publish immutable per-frame selection snapshots for renderer reads;
- store selection as `std::atomic<uint64_t>` words with relaxed reads/writes and explicit API around range updates;
- route all GUI/render concurrent access through existing composite locks and state that renderer never reads live `selected_` bits without the lock.

### P17-5: D26's EBO assumption is false on default MSVC multiple inheritance — BUG

D26 says the three shim bases are empty and EBO contributes 0 B ([V21-DECISIONS.md:227](V21-DECISIONS.md:227)). On Apple Clang 21, a local sanity compile of three distinct empty bases plus pointer/int payload produced the expected compact size. GCC on this machine resolves to Apple Clang, so I did not verify GNU libstdc++ locally.

For MSVC, this is not a guess. Microsoft documents that default Visual Studio layout does not fully apply EBCO in multiple inheritance, that it adds padding between consecutive empty bases, and that `__declspec(empty_bases)` is the per-class opt-in for full EBCO. Source: Microsoft Learn, "`empty_bases`" (`learn.microsoft.com/en-us/cpp/cpp/empty-bases?view=msvc-170`).

Suggested fix: D26 must require an MSVC-specific annotation on the concrete handle classes that directly inherit the empty shims, e.g. `class BALL_EXPORT BALL_EMPTY_BASES Atom : public D17Composite, ...`, where `BALL_EMPTY_BASES` expands to `__declspec(empty_bases)` on MSVC and empty elsewhere. Add compile-time layout tests for Clang, GCC, and MSVC in `Sizeof_test`, with the v2.1 target values.

### P17-6: Bond property columns need a stable bond identity — BUG

D23 says bond properties use the same column infra and close V21-BOND-PROPERTY-JSON "for free" ([V21-DECISIONS.md:122](V21-DECISIONS.md:122)). That hides a coupling.

Today `Bond` is a heap object inheriting `Composite` and `PropertyManager` ([include/BALL/KERNEL/bond.h](/Users/kohlbach/Claude/BALL2/include/BALL/KERNEL/bond.h:54)). v2.0 mirrors it into `MoleculeStore::bonds_` and stores `bond_record_idx_` plus `bond_store_` on the Bond handle ([include/BALL/KERNEL/bond.h](/Users/kohlbach/Claude/BALL2/include/BALL/KERNEL/bond.h:461)); `Bond::createBond` sets that index from `add_bond()` ([source/KERNEL/bond.C](/Users/kohlbach/Claude/BALL2/source/KERNEL/bond.C:123)). JSON writes only live records as `(a,b,order,type,flags)` and strips tombstoned store indices ([source/KERNEL/moleculeStoreJson.C](/Users/kohlbach/Claude/BALL2/source/KERNEL/moleculeStoreJson.C:183)). On load, `add_bond()` creates fresh row indices in input order ([source/KERNEL/moleculeStoreJson.C](/Users/kohlbach/Claude/BALL2/source/KERNEL/moleculeStoreJson.C:375)).

If v2.1 writes `bond_property_columns` indexed by current `bond_idx`, those columns only line up if the `bonds[]` array and property arrays are serialized in the exact same live-row order and the reader assigns fresh bond indices in that order. That can work, but it must be the contract. It does not cohere with a reader that treats `(a,b)` as identity, because duplicate bonds, order/type changes, and tombstone compaction all make `(a,b)` non-unique or insufficient.

Suggested fix: D23/D30/P4 need an explicit bond identity design:

- `bonds[]` order is canonical and `bond_property_columns.values[k]` refers to `bonds[k]`; or
- add a stable `bond_id`/`bond_idx` field to each serialized bond record; or
- store bond properties inline per bond record for v2.1 JSON.

Do not start P3/P4 with "for free" as the design.

### P17-7: Iterator stability can be preserved, but the constraints are not written down — DEBT

Current Composite iterators store `Composite*` positions, not separate iterator nodes ([include/BALL/CONCEPT/composite.h](/Users/kohlbach/Claude/BALL2/include/BALL/CONCEPT/composite.h:1115), [include/BALL/CONCEPT/composite.h](/Users/kohlbach/Claude/BALL2/include/BALL/CONCEPT/composite.h:1275)). That is favorable: side-table relinking can preserve iterator identity across insertions as long as the handle object remains alive and the iterator resolves topology through the store on each step.

The edge case is implementation temptation. If P1/P2 rewrites iterators to cache `CompositeNode*` or vector indices into `composite_nodes_`, `std::vector` growth and node reuse can invalidate iterators in ways current code does not. Insertions before/after an iterator's current node should not invalidate the current `Composite*`; removal of the current node remains invalid as it is today.

Suggested fix: add a D22/D26 invariant and tests: Composite iterators may cache `Composite*` only; they must not cache `CompositeNode*`; side-table reserve/reallocation must not affect active iterators; insertion before/after current child preserves traversal. Reuse current tests such as `CompositeChildcompositeIteratorTraits_test` and add insertion-during-iteration cases.

### P17-8: Thin-handle-only Wave 4-7 bugs need a backport policy — DEBT

D27 intentionally ships v2.1 before the remaining Wave 4-7 closures and keeps v2.0.x for critical fixes only ([V21-DECISIONS.md:242](V21-DECISIONS.md:242)). That is reasonable, but the document only describes kernel bugs that can land as v2.1 patches. It does not state what happens when a Wave 4-7 module exposes a bug caused by the v2.1 thin-handle rewrite, while v2.0 is unaffected because it still has fat handles.

Suggested fix: add policy text:

- v2.1-only thin-handle bugs are fixed on v2.1.x and are not backported to v2.0.x;
- if the same user-visible correctness bug exists in v2.0, create a separate v2.0.x patch without thin-handle dependencies;
- Wave 4-7 closure branches must declare whether failures are v2.0-fat-compatible, v2.1-thin-only, or shared.

### P17-9: P5/P6 planning reviews are probably over-scoped — DEBT

D28 schedules planning and close reviews for P1-P5 and a close review for P6 ([V21-DECISIONS.md:276](V21-DECISIONS.md:276)). The strict cadence is justified for P1-P4: side-table infrastructure, Atom flip, Bond flip, and JSON schema are design-sensitive and easy to get subtly wrong.

P5 is performance gates, benchmark methodology, store iterator API, and generation guard finalization ([MILESTONE-v2.1-KICKOFF.md:72](MILESTONE-v2.1-KICKOFF.md:72)). P6 is release notes and tag ([MILESTONE-v2.1-KICKOFF.md:74](MILESTONE-v2.1-KICKOFF.md:74)). These need close/pre-tag review, but a planning review before each likely adds process cost without much design risk.

Suggested fix: downgrade P5/P6 planning reviews to maintainer checklist unless the phase plan changes public API or schema. Keep P5 close and P6 pre-tag reviews.

### P17-10: v2.0 JSON demux into v2.1 property columns is not specified — BUG

D30 promises v2.1 reads v2.0 JSON ([V21-DECISIONS.md:324](V21-DECISIONS.md:324)). v2.0 JSON writes `atoms.properties` as a per-atom array of `PropertyManager` bags ([source/KERNEL/moleculeStoreJson.C](/Users/kohlbach/Claude/BALL2/source/KERNEL/moleculeStoreJson.C:109)) and restores each bag into the live Atom handle ([source/KERNEL/systemJson.C](/Users/kohlbach/Claude/BALL2/source/KERNEL/systemJson.C:430)). v2.1 column storage has to demux that shape into `(property name, type) -> column`.

The hard case is legal in v2.0: atom 0 can have `foo` as INT and atom 1 can have `foo` as STRING because each atom owns an independent `PropertyManager`. D23 says mismatched type for a registered column throws `InvalidArgument` ([V21-DECISIONS.md:113](V21-DECISIONS.md:113)). If the v2.1 loader does that while reading a v2.0 file, backward read is not actually supported for mixed-type files.

Suggested fix: D30 must define loader behavior:

- strict mode: reject mixed-type same-name properties with `ParseError` and document this as the only unsupported v2.0 edge case; or
- compatibility mode: key columns by `(name,type)` internally and expose `getProperty(name)` using v2.0 precedence rules; or
- fallback: properties with name/type conflicts stay in a sparse per-object compatibility bag.

The fallback is the least disruptive and pairs well with the D23 sparse/dense promotion policy.

## Overall verdict

**NO-GO for P1 as written.** The v2.1 direction is sound, but P1 would currently implement under-specified side tables. Blocking fixes before P1 starts:

1. Fix D22 node shape and memory math to include `last_child` and `prev_sibling`.
2. Add D23 dynamic-property bounds: sparse fallback, promotion policy, registry cap, and string interning.
3. Resolve D24 selection concurrency with snapshot, atomics, or locking.
4. Fix D26's MSVC EBO assumption with `__declspec(empty_bases)` and CI size pins.
5. Specify D30 v2.0 property demux and mixed-type handling.

P17-6 can be finalized before P3/P4, but the decisions doc should stop claiming Bond property JSON comes "for free." P17-8/P17-9/P17-7 are process/design debts, not P1 blockers.

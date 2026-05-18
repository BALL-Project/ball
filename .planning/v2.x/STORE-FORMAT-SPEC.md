# STORE-FORMAT-SPEC.md — BALL 2.0 Persistence v2 Wire Format

**Status:** DRAFT (K0.6a deliverable). **Authored:** 2026-05-18.
**Normative for:** K0.6b implementation (`include/BALL/KERNEL/storeFormat.h`,
`source/KERNEL/storeFormat.C`, `bin/ball-convert-v1-v2`).
**Companion docs:**
- [`KERNEL-V2-DECISIONS.md`](KERNEL-V2-DECISIONS.md) — D12 amendment (split K0.6 into design + impl).
- [`V2.0-ROADMAP.md`](V2.0-ROADMAP.md) §3 K0.6a/K0.6b — phase contract this doc fills.
- [`K0-CODEX-REVIEW-ROUND2.md`](K0-CODEX-REVIEW-ROUND2.md) MEDIUM-8 — bond mirror from persistence; the round-trip invariant in §3 below directly addresses it.
- [`CORE-MOLECULESTORE-RESEARCH.md`](CORE-MOLECULESTORE-RESEARCH.md) §3 column layout — the wire format is the on-disk projection of these columns.
- `include/BALL/KERNEL/moleculeStore.h` — the in-memory shape this format must serialise.

This spec defines the **on-wire binary format** for persisting a
MoleculeStore plus the container hierarchy (System → Molecule →
Residue → … → Atom) layered on top of it. It does **not** specify the
reader/writer API or the converter executable — those are K0.6b
implementation. The spec is the contract that the implementation must
match byte-for-byte.

When this document and the implementation disagree on the wire bits,
this document wins; either the implementation is fixed or this doc is
explicitly amended with a version bump.

---

## 1. Design goals

These goals are listed in **descending priority**. When two goals
collide, the higher one wins.

1. **Byte-stable.** Same in-memory MoleculeStore + container tree →
   same bytes on disk, every time, every platform (little-endian
   normalised on writers from big-endian hosts). Reproducible builds,
   reproducible test artefacts, `diff`-able output for CI.
2. **Version-tagged.** Every file carries `(magic, format_major,
   format_minor)`. The 2.0 reader rejects any file whose `format_major
   != 2`. Forward compatibility within `format_major == 2` is
   *defined* (readers ignore unknown chunk kinds) but **not
   exercised** until v2.1 ships a second writer (see §8).
3. **Round-trip bit-identical.** Write → read → write produces the
   same bytes. This is the K0.6b invariant test (§3 below). The format
   is **deterministic**: insertion order of named properties is
   sorted; bond table is sorted by `(min(a,b), max(a,b))`; CSR
   adjacency is rebuilt from scratch on load, not stored.
4. **Streamable on write.** Writer produces output in a single
   forward pass: header → column blobs → bond table → string pool →
   hierarchy → property bag → index footer. No seeks, no two-pass
   buffering. Suitable for piping to compression or to a network
   socket.
5. **Reader uses index footer.** The footer (last 64 B + table) gives
   absolute byte offsets to every section. Readers seek directly to
   needed sections; they do not parse the whole file linearly. A
   future v2.1 "load atoms only, skip bonds" mode (out of scope here)
   becomes trivial.
6. **mmap-friendly (nice-to-have, not required for v2.0).** Column
   blobs are 16-byte-aligned within the file. A v2.1+ reader on POSIX
   could mmap the file and hand the column pointer directly to the
   store. v2.0 readers must still `read(2)` into store-owned
   `std::vector` storage; alignment exists so future code can flip
   without a format change.
7. **Compressible externally.** No internal compression. v2.0 ships
   `.bf2` (uncompressed); users may `gzip` to produce `.bf2.gz` and
   the converter recognises the suffix. Internal per-column
   compression (e.g. zstd-per-blob) is v2.1+.
8. **Self-describing within a major version.** Every chunk carries
   its `(kind, length)`. Unknown chunks are skipped by readers
   ≥ writer's `format_minor`. This enables additive evolution
   without a major bump.
9. **No object-graph fixup.** v1.x's `writeObjectPointer` /
   `readObjectPointer` machinery (which fixed up raw `Atom*` /
   `Bond*` pointers across the read) is **deleted**. References
   between objects are stable IDs or slot indices, both of which are
   pure integers in the file.
10. **No vtable / RTTI dependency.** v1.x relied on
    `RTTI::getStreamName<Composite>()` and `dynamic_cast` to route
    `readObject()` calls. v2 dispatch is by `chunk_kind` enum; no
    RTTI hooks survive into the format reader.

---

## 2. Wire format

### 2.1 File-level structure

```
+----------------------------------------+
| FileHeader              (64 B fixed)   |
+----------------------------------------+
| StoreSection                (1 or N)   |
|   StoreHeader  (32 B)                  |
|   ColumnChunk×K                        |
|   StringPoolChunk                      |
|   BondTableChunk                       |
|   StableIdMapChunk                     |
|   PropertyBagChunk                     |
|   SelectionChunk                       |
+----------------------------------------+
| HierarchyChunk          (CSR encoded)  |
+----------------------------------------+
| CrossRefIndexChunk      (stable→slot)  |
+----------------------------------------+
| Footer                  (table+magic)  |
+----------------------------------------+
```

Each chunk is `(kind: u32, length: u64, payload[length])`, 16-byte
aligned. Padding bytes are zero. The reader uses the footer offset
table to seek; the writer streams strictly forward.

### 2.2 FileHeader (offset 0, exactly 64 bytes)

| Offset | Size | Field          | Value / Notes                              |
|--------|------|----------------|--------------------------------------------|
| 0      | 8    | `magic`        | ASCII `BALL2\0\0\0` — `0x42 41 4C 4C 32 00 00 00` |
| 8      | 4    | `format_major` | `u32 LE` = `2`                             |
| 12     | 4    | `format_minor` | `u32 LE` = `0` (v2.0 ships `2.0`)          |
| 16     | 1    | `endian_byte`  | `0x01` (little-endian). All scalars are LE on disk. |
| 17     | 1    | `pointer_size_hint` | `0x08` (info only — file is identical on 32-bit / 64-bit hosts) |
| 18     | 2    | `flags`        | bit 0 = "has hierarchy"; bit 1 = "has property bag"; bit 2 = "has selection"; bits 3-15 reserved (zero) |
| 20     | 4    | `n_stores`     | `u32 LE` — number of MoleculeStore records in this file |
| 24     | 8    | `created_unix_ns` | `u64 LE` — `clock_gettime(CLOCK_REALTIME)` at write start. Determinism note: tests must override to `0` for byte-stable fixtures. |
| 32     | 16   | `writer_id`    | UTF-8 identifier, NUL-padded. v2.0 writer emits `"BALL2.0-K0.6b\0\0"`. |
| 48     | 8    | `footer_offset`| `u64 LE` — absolute byte offset of the Footer (filled in by writer at end). |
| 56     | 4    | `header_crc32` | CRC-32 over bytes [0,52). Catches truncation / corruption of the header itself. |
| 60     | 4    | reserved       | `0x00000000`                                |

Total: **64 bytes**, naturally aligned.

### 2.3 StoreHeader (32 bytes, one per MoleculeStore)

| Offset | Size | Field          | Notes                                      |
|--------|------|----------------|--------------------------------------------|
| 0      | 4    | `chunk_kind`   | `0x53_54_4F_52` = `"STOR"`                 |
| 4      | 8    | `payload_len`  | total bytes of all sub-chunks in this store |
| 12     | 4    | `n_atoms`      | `u32 LE`                                   |
| 16     | 4    | `n_bonds`      | `u32 LE`                                   |
| 20     | 8    | `next_stable_id` | `u64 LE` — the store's high-water `next_stable_id_` after write |
| 28     | 4    | `store_index`  | 0-based index in this file (matches FileHeader.n_stores) |

Followed immediately by the store's sub-chunks (§2.4–§2.10) until
`payload_len` bytes are consumed.

### 2.4 ColumnChunk (one per typed column)

The MoleculeStore today (see `moleculeStore.h`) owns these atom-level
columns. Each ships as one ColumnChunk:

| `column_kind` | Element type | Bytes/atom | Source field         |
|---------------|--------------|-----------:|----------------------|
| `0x01 POS`    | `Vector3` (3×f32) | 12   | `positions_`         |
| `0x02 VEL`    | `Vector3`         | 12   | `velocities_`        |
| `0x03 FRC`    | `Vector3`         | 12   | `forces_`            |
| `0x04 CHG`    | `f32`             | 4    | `charges_`           |
| `0x05 RAD`    | `f32`             | 4    | `radii_`             |
| `0x06 ATY`    | `i16`             | 2    | `atom_types_`        |
| `0x07 FCH`    | `i16`             | 2    | `formal_charges_`    |
| `0x08 ELI`    | `u8`              | 1    | `element_indices_`   |
| `0x09 NMO`    | `u32`             | 4    | `name_offsets_`      |
| `0x0A TNO`    | `u32`             | 4    | `type_name_offsets_` |
| `0x0B STI`    | `u64`             | 8    | `stable_ids_`        |

Chunk layout:

| Offset | Size | Field        | Notes                                  |
|--------|------|--------------|----------------------------------------|
| 0      | 4    | `chunk_kind` | `0x434F_4C00` `| column_kind` (`"COL\0" | k`) |
| 4      | 8    | `payload_len`| = `n_atoms × bytes_per_atom`           |
| 12     | 4    | `column_kind`| see table above                        |
| 16     | 4    | `element_size`| bytes_per_atom (redundant; sanity check) |
| 20     | `payload_len` | column blob | aligned to 16 B at chunk start    |

`back_ptr_` (the live `Atom*` column) is **NOT** persisted. It is a
runtime-only handle linkage; on load, the reader walks the new
container hierarchy (§2.8) and populates `back_ptr_` from freshly
constructed Atom handles.

`selection_` is persisted but in a packed bitmap (§2.7).

### 2.5 StringPoolChunk

| Offset | Size | Field        | Notes                                     |
|--------|------|--------------|-------------------------------------------|
| 0      | 4    | `chunk_kind` | `0x53_50_4F_4C` = `"SPOL"`                |
| 4      | 8    | `payload_len`| total bytes (including NULs)              |
| 12     | 4    | `n_bytes`    | `u32 LE` — same as payload_len-padding    |
| 16     | …    | UTF-8 bytes  | NUL-terminated entries, exactly as the in-memory `string_pool_` |

Offsets stored in `name_offsets_` / `type_name_offsets_` index into
this blob and must point at an entry start (verified on load).

### 2.6 BondTableChunk

| Offset | Size | Field        | Notes                                     |
|--------|------|--------------|-------------------------------------------|
| 0      | 4    | `chunk_kind` | `0x42_4F_4E_44` = `"BOND"`                |
| 4      | 8    | `payload_len`| = `12 × n_bonds`                          |
| 12     | …    | `BondRecord[n_bonds]` | 12 B each: `u32 a, u32 b, u8 order, u8 type, u16 flags` (matches in-memory layout exactly) |

**Determinism rule:** bonds are sorted by
`(min(a,b), max(a,b), order, type, flags)` before write. CSR
adjacency (`bond_csr_off_`, `bond_csr_idx_`) is **not** in the file;
the reader calls `MoleculeStore::ensure_csr_()` after loading bonds.

`bond_back_ptr_` is **NOT** persisted (handle linkage, like
`back_ptr_`).

### 2.7 SelectionChunk

| Offset | Size | Field        | Notes                                     |
|--------|------|--------------|-------------------------------------------|
| 0      | 4    | `chunk_kind` | `0x53_45_4C_42` = `"SELB"`                |
| 4      | 8    | `payload_len`| = `ceil(n_atoms / 8)` rounded to 16 B     |
| 12     | …    | packed bitmap | LSB-first within each byte; bit i = `selection_[i]` |

Omitted entirely if every atom is unselected; reader treats absence
as "all bits 0".

### 2.8 HierarchyChunk (CSR-encoded container tree)

The container tree (System → Molecule → Residue → … → Atom) is
serialised as a **node table + CSR child-offset array**. No pointer
fixup. Each non-Atom node is described by:

| Field            | Type | Notes                                        |
|------------------|------|----------------------------------------------|
| `node_id`        | u32  | sequential, 0-based; node_id 0 is the root System |
| `node_kind`      | u8   | 0=System, 1=Molecule, 2=Protein, 3=NucleicAcid, 4=Chain, 5=Residue, 6=Nucleotide, 7=SecondaryStructure, 8=Fragment, 9=AtomContainer (generic), 10=PDBAtomContainer, 0xFF=Atom-leaf-ref |
| `parent_id`      | u32  | `0xFFFFFFFF` for root                        |
| `first_child`    | u32  | index into `children[]` array (or `0xFFFFFFFF`) |
| `n_children`     | u32  | child count                                  |
| `properties_off` | u32  | offset into PropertyBagChunk (or `0xFFFFFFFF`) |
| `name_offset`    | u32  | offset into StringPoolChunk                  |
| `atom_first_slot`| u32  | for Atom-leaf-ref nodes: store atom index; else `0xFFFFFFFF` |

Chunk layout:

| Offset | Size | Field         | Notes                                    |
|--------|------|---------------|------------------------------------------|
| 0      | 4    | `chunk_kind`  | `0x48_49_45_52` = `"HIER"`               |
| 4      | 8    | `payload_len` |                                          |
| 12     | 4    | `n_nodes`     | `u32 LE`                                 |
| 16     | 4    | `n_children_total` | sum of all `n_children` (size of `children[]` array) |
| 20     | `n_nodes × 32` | `NodeRecord[n_nodes]` | each 32 B as above |
| …      | `n_children_total × 4` | `children[]` — `u32` node IDs in parent-then-child order |

**Determinism rule:** sibling order is preserved as the in-memory
tree had it. Children are written in insertion order; CSR offsets
follow.

### 2.9 PropertyBagChunk (sparse named properties on atoms and containers)

PropertyManager (per D3, reduced to thin proxy) stores
`(stable_id, name) → typed_value`. Written as a packed table sorted
by `(stable_id_or_node_id, name)`:

| Offset | Size | Field         | Notes                                    |
|--------|------|---------------|------------------------------------------|
| 0      | 4    | `chunk_kind`  | `0x50_42_41_47` = `"PBAG"`               |
| 4      | 8    | `payload_len` |                                          |
| 12     | 4    | `n_entries`   | `u32 LE`                                 |
| 16     | `n_entries × Entry` | see Entry layout below             |

Entry layout (variable size per entry; aligned to 8 B):

| Size | Field      | Notes                                              |
|------|------------|----------------------------------------------------|
| 8    | `owner_id` | atom: `stable_id`; container: `node_id | (1<<63)`  |
| 4    | `name_off` | offset into StringPoolChunk                        |
| 1    | `type_tag` | `0`=bool, `1`=i32, `2`=i64, `3`=f32, `4`=f64, `5`=string(name_off), `6`=Vector3 |
| 7    | padding    | zero                                               |
| 8    | `value`    | bool→u8 in lo byte; i32/f32→4 B + 4 B pad; i64/f64→8 B; string→u32 string pool offset + 4 B pad; Vector3→12 B (overflows into next 8 B slot; entry consumes 32 B total instead of 24 B in that case) |

The Vector3 case is the only variable-width entry. Encoders pad to
the next 8 B boundary after the value.

### 2.10 StableIdMapChunk (cross-reference index)

For cross-document references (e.g. a saved selection blob that
points at atoms in *this* file), readers need a fast `stable_id →
slot_index` lookup. Persisted as a sorted array:

| Offset | Size | Field         | Notes                                    |
|--------|------|---------------|------------------------------------------|
| 0      | 4    | `chunk_kind`  | `0x53_49_44_58` = `"SIDX"`               |
| 4      | 8    | `payload_len` | = `12 × n_atoms`                         |
| 12     | …    | `Entry[n_atoms]` — `u64 stable_id, u32 slot_index`, sorted by `stable_id` ascending |

Reader can binary-search this without loading the entire StableIdMap
into a hash table.

### 2.11 CrossRefIndexChunk (multi-store linkage)

Only emitted when `n_stores > 1`. Maps `(store_index, stable_id) →
nothing` (a presence set) so readers can validate cross-store stable
ID uniqueness. Omitted in v2.0 single-store files.

### 2.12 Footer

Last `64 + n_chunks * 16` bytes of the file:

| Field            | Size | Notes                                  |
|------------------|------|----------------------------------------|
| `n_chunks_total` | u32  | including header + footer self         |
| `chunk_offsets[]`| `n_chunks_total × 16` | each entry: `u32 kind, u64 offset, u32 length` |
| `footer_crc32`   | u32  | CRC-32 over the offset table           |
| `footer_magic`   | u64  | `0x42 41 4C 4C 32 45 4E 44` = `"BALL2END"` |

The FileHeader's `footer_offset` field points at `n_chunks_total`.
Truncation detection: reader reads the last 8 B; if it's not
`"BALL2END"`, the file is incomplete and rejected.

---

## 3. Invariants (K0.6b round-trip tests must verify)

For any in-memory MoleculeStore `S`, the K0.6b test suite verifies
the following invariants. Each is a separate `TEST_EQUAL` so a
failure pin-points which one regressed.

| # | Invariant | How to test                                              |
|---|-----------|----------------------------------------------------------|
| I1 | **Atom ordering preserved.** The `i`th atom written is the `i`th atom read back; `store->position(i)`, `store->stable_id(i)`, etc., all match before and after `write→read`. | Loop `for i in 0..n_atoms: TEST_EQUAL` over every typed column. |
| I2 | **Bond endpoints by stable_id**, not raw `Atom*`. After reload, every BondRecord's `a`/`b` resolve (via the StableIdMap) to the *same* atom slots as on write. | For each bond pre- and post-roundtrip: `TEST_EQUAL(pre.a_stable_id, post.a_stable_id)`. Closes K0-CODEX-REVIEW-ROUND2 MEDIUM-8. |
| I3 | **CSR adjacency reconstructible.** After load + `ensure_csr_()`, `bonds_of(i)` returns the same bond-index multiset as pre-write. | Sort both vectors and compare. |
| I4 | **Hierarchy reproduction.** Every node in the post-load tree has the same `(kind, name, parent_id, children IDs in order)` as pre-write. | DFS walk both trees in parallel; compare at each node. |
| I5 | **Named property round-trip.** For every `(owner, name)` → `value` in the pre-write property bag, the post-load bag has the same key with type-preserving equality. | Iterate property bag; `TEST_EQUAL(typeid + bytewise)`. |
| I6 | **Selection bitmap preserved.** `selected(i)` matches for all `i`. | Bitmap XOR == 0. |
| I7 | **Byte-stable serialisation.** `write(S) → bytes_A; read(bytes_A) → S'; write(S') → bytes_B; TEST_EQUAL(bytes_A, bytes_B)`. | One memcmp. The strongest invariant — catches non-determinism in any encoder step. |
| I8 | **Footer CRC valid.** Computed footer CRC matches stored. | Trivial. |
| I9 | **Header CRC valid.** | Trivial. |
| I10 | **String pool offsets in-range.** Every `name_offset` / `type_name_offset` / property `name_off` points at the start of a NUL-terminated entry inside the pool. | Per-offset bounds check + NUL preceded-by check. |
| I11 | **String pool determinism.** Identical strings share a single offset (writer interns); pool is built in the same order on every write. | `TEST_EQUAL(offset_of("foo", first_write), offset_of("foo", second_write))`. |
| I12 | **Stable-ID monotonicity.** `next_stable_id` after load ≥ any persisted `stable_id` + 1. | Allocate a new atom post-load; verify ID > all loaded IDs. |
| I13 | **`back_ptr_` reconstructed.** After load, every `back_ptr_[i]` points at a valid `Atom*` whose `store_idx_ == i` and whose `store_ == &S`. | Loop; assert. |

Invariants I1, I2, I4, I6, I7 together imply: *the post-load
in-memory state is observationally indistinguishable from the
pre-write state for all locked columns.*

---

## 4. Edge cases

### 4.1 Malformed-but-tolerated v1.x files

The v1→v2 converter accepts v1.x files that v1.x's own readers
already tolerated. Specifically:

- **HIN files with degenerate bonds** (atom bonded to itself; same
  bond declared twice with different orders) — converter merges
  duplicates (keeps highest order); logs at WARN.
- **PDB files with non-standard residue names** — kept as the
  residue name string; no normalisation. The store's PropertyBag
  carries a `"residue_name_was_unrecognized"` flag.
- **HIN with no `forcefield` directive** — converter defaults to
  `AMBER` (matches v1.x `HINFile` behaviour); WARNs.
- **PDB with negative occupancies** — converter clamps to 0; logs.

### 4.2 Intermolecular bonds (cross-Molecule, intra-System)

Permitted. A System owns one MoleculeStore; bonds live in the
store's `bonds_` table regardless of which Molecule node owns the
endpoint atoms. BondRecord knows nothing about Molecule
boundaries. The container hierarchy (§2.8) is independent of bond
topology.

### 4.3 Cross-System bonds (forbidden)

Per K0.4 design + K0-CODEX-REVIEW-ROUND2 finding 7, bonds connecting
atoms in different Systems are rejected at the API level. The
writer asserts (debug) and skips (release, logging at ERROR) any
such bond. The format has no encoding for them.

### 4.4 Selections spanning multiple Systems

Selections live in per-store `selection_` bitmaps. A "multi-System
selection" is the *union* of per-store bitmaps; the format carries
each store's bitmap in its own SelectionChunk. A future v2.1
"saved selection blob" file format (out of scope here) would
reference atoms by `(file_path, stable_id)` pairs and use the
StableIdMap to resolve.

### 4.5 Very large stores (> 2 GB single column)

`payload_len` is `u64` so any single chunk can be up to 16 EiB.
The 32-bit `n_atoms` field caps a single store at 4.29 billion
atoms. At 12 B/atom for positions alone that's ~52 GiB — well within
the format's headroom. **K0.6b ships with no testing above 10 M
atoms;** the > 2 GB column path is exercised by a single
`StoreFormat_LargeColumn_test` that fabricates a 200 M-atom store
with positions only.

### 4.6 Empty stores

`n_atoms == 0` is legal. The writer omits all ColumnChunks (no
payload to write) and the BondTableChunk (`n_bonds == 0` →
`payload_len == 0`). The StringPoolChunk is present but empty
(`n_bytes == 0`). The HierarchyChunk has one root System node with
zero children. Smallest legal file: ~256 B (header + empty store
header + footer).

### 4.7 Stores with only orphan atoms

Per D16, default-constructed atoms live in the process-global orphan
store. The orphan store is **never** persisted (it has no
associated System and no meaningful identity across processes).
Writers reject attempts to serialise the orphan store with an
error code `BF2_ORPHAN_STORE_NOT_SERIALISABLE`.

### 4.8 Bond table that exceeds `u32` index

`BondRecord.a` and `.b` are `u32`. Combined with the
`n_atoms ≤ 2³²` cap from §4.5, no overflow can occur. Writer
asserts the invariant in debug builds.

### 4.9 Unicode / non-ASCII atom names

The string pool is UTF-8. v1.x's atom/residue names were
nominally ASCII but in practice some PDB files carry ISO-8859
bytes in residue name fields. The converter re-encodes to UTF-8
on the way in (assuming Latin-1 if non-ASCII bytes appear), logs
a WARN, and writes valid UTF-8.

---

## 5. Curated v1.x test corpus

The K0.6b converter must round-trip every file in this list:
v1.x reader → freshly constructed MoleculeStore → v2 writer →
v2 reader → MoleculeStore → re-parse to v1.x in-memory shape →
*semantic* equality with the original (positions, bonds,
hierarchy, named properties). All files already live in
`test/data/` or `data/structures/`; no new fixtures need
manufacture.

| # | File | Path | Edge case covered |
|---|------|------|-------------------|
| C1 | `methane.hin` | `test/data/methane.hin` | Smallest molecule; trivial single-Molecule single-Residue case; sanity floor. |
| C2 | `benzene.hin` | `test/data/benzene.hin` | Aromatic ring; bond-order encoding (single + 1.5 / aromatic); tests I2. |
| C3 | `AA.hin` | `test/data/AA.hin` | Single amino acid (Ala); minimal protein hierarchy (System → Protein → Chain → Residue → Atom). |
| C4 | `AlaGlySer.hin` | `test/data/AlaGlySer.hin` | Tripeptide; chain residue ordering invariant (I4); inter-residue peptide bonds (§4.2). |
| C5 | `AlaAla.hin` | `data/structures/AlaAla.hin` | Dipeptide from the canonical structures dir; the v1.7 baseline test corpus member. |
| C6 | `aspirin.hin` | `data/structures/aspirin.hin` | Small drug-like molecule; mixed bond orders; tests bond table determinism. |
| C7 | `1BNA.pdb` / `1BNA.hin` | `test/data/1BNA.{pdb,hin}` | DNA dodecamer; NucleicAcid + Nucleotide hierarchy nodes; non-protein chain types (I4). |
| C8 | `bpti.pdb` | `data/structures/bpti.pdb` | BPTI (58 residues, disulfide bridges); cross-Residue intra-Chain bonds; multiple secondary structure annotations → SecondaryStructure nodes. |
| C9 | `2ptc_H.pdb` | `test/data/2ptc_H.pdb` | Trypsin-BPTI complex; **two chains, intermolecular** in the sense of "different chains" — exercises System with multiple Protein children, multiple peptide chains. |
| C10 | `1A1F.pdb` | `data/structures/1A1F.pdb` | Zinc-finger protein with metal coordination; tests PDBAtom container kind + element_index for non-CHONPS elements; stress test for hierarchy chunk size. |
| C11 | `AAG.mol2` | `test/data/AAG.mol2` | mol2 input path (different v1.x reader); ensures converter handles all FORMAT-side readers (not just HIN/PDB) into the same v2 output. |
| C12 | `all_amino.hin` | `test/data/all_amino.hin` | All 20 canonical amino acids in one System; string-pool deduplication stress (I11) — same atom name `"CA"` appears 20 times, must intern to one offset. |

**Edge-case categorisation summary:**

- Hierarchy depth: C3, C4, C7, C8 (4 levels), C10 (5 levels with PDBAtom).
- Bond order diversity: C2 (aromatic), C6 (mixed single/double), C8 (disulfide).
- Multi-chain/multi-molecule: C9 (two chains), C10 (zinc cofactor).
- String-pool stress: C12 (intern dedup).
- Cross-reader path: C11 (mol2 entry).
- Format-size stress: C8, C10 (largest in corpus).
- Minimal: C1 (smallest).

If any corpus file is later found *not* to exercise a unique edge
case, it stays in the list anyway — corpus inflation is cheap, but
losing coverage is expensive.

### 5.1 Corpus storage

The K0.6b implementation copies these files (or symlinks them) into
`test/data/v1_corpus/` and the round-trip test iterates the
directory. New entries land via PR; never deleted without a
deprecation note in `RELEASE-NOTES-v2.0.md`.

---

## 6. v1→v2 converter design

The converter ships as a CLI in `source/APPLICATIONS/TOOLS/ball-convert-v1-v2.C`,
built to `bin/ball-convert-v1-v2`. It is the **only** path from v1.x
data to v2.0; v2.0 itself cannot read v1.x format.

### 6.1 Read v1.x

For each input file, the converter probes the suffix and dispatches
to the appropriate v1.x reader: `HINFile`, `PDBFile`, `MOL2File`,
`SDFile`, etc. — all of which already exist under
`source/FORMAT/` and use the v1.x `TextPersistenceManager` /
`XDRPersistenceManager` only for the legacy `.txt` / `.xdr` paths
(rare in the corpus). The output of each reader is a fully
constructed `System` with v1.x Atom/Bond/Composite objects. No
changes to v1.x readers.

### 6.2 Build a fresh MoleculeStore

For each loaded `System`, the converter walks its Composite tree
and constructs a fresh per-System `MoleculeStore`. For each Atom
visited, the converter calls `store->allocate_atom()` and copies
every v1.x Atom payload field into the corresponding store
column. For each Bond, it calls `store->add_bond(stable_id_of_a,
stable_id_of_b, order, type)`. Container nodes are recorded in a
parallel `std::vector<NodeRecord>` that becomes the
HierarchyChunk. Named properties from each Composite's
`PropertyManager` go into a parallel `vector<PropEntry>` that
becomes the PropertyBagChunk. Selection bits go straight into
`store->set_selected(i, ...)`.

### 6.3 Write v2 format

Once the in-memory MoleculeStore + node table + property bag are
built, the converter invokes the v2 writer (a single function
`storeFormat::write(const std::vector<const MoleculeStore*>&,
const HierarchySpec&, std::ostream&)`). The writer is the same
code used by the runtime persistence path, so the converter and
the runtime share a single implementation of every wire-format
invariant.

### 6.4 Verify via reload + diff

After writing, the converter immediately reloads the freshly
written v2 file and runs the **same** I1–I13 invariant checks
(§3) against the in-memory pre-write state. On any failure, the
converter exits non-zero with the failing invariant ID; the file
is treated as invalid and not promoted to its final path (the
converter writes to `.tmp` and renames on success).

This makes the converter **self-checking**: a converter run that
exits 0 has already proven round-trip identity for the file it
produced.

### 6.5 Batch mode + parallelism

`ball-convert-v1-v2 --in dir/ --out dir/ -j N` walks a tree and
converts every recognised file in parallel. Each conversion is
independent (no shared state beyond the orphan store, which is
flushed between files); writer output is deterministic, so
parallel runs produce identical bytes.

---

## 7. Backwards compatibility statement

**v1.x files cannot be loaded by v2.0+.** The
`TextPersistenceManager` / `XDRPersistenceManager` / `PersistentObject`
machinery is deleted in K0 (see KERNEL-V2-DECISIONS.md §3). v2.0
ships with **no** runtime path that accepts a v1.x BALL binary or
text persistence file.

The supported migration path is exactly one tool:

```
bin/ball-convert-v1-v2 input.{hin,pdb,mol2,...} -o output.bf2
```

This is a **one-time** migration. Once a user has converted their
v1.x corpus to `.bf2`, the v1.x originals can be archived; v2.0
will never read them again.

This statement must appear, verbatim, in `RELEASE-NOTES-v2.0.md`
in the "Breaking Changes" section. Reproduce it in the user-facing
docs (Read-the-Docs portal, v2.2+). A FAQ entry "How do I open my
old `.hin` file?" must point at `ball-convert-v1-v2`.

The converter is itself **frozen at v2.0**: once v2.1 ships with
new column kinds (compression, additional sparse columns, etc.),
the converter still emits valid `format_minor == 0` files. v2.1+
readers must accept `format_minor == 0` files without
modification. The converter's job is one-shot; we never expand its
output target.

---

## 8. Out of scope (defer to v2.1+)

The following are deliberately **excluded** from the v2.0 format and
its K0.6b implementation. Each gets its own design subphase later.

| # | Item | Why deferred | Earliest |
|---|------|--------------|----------|
| O1 | Forward compatibility (v2.0 reader handling v2.1 chunk kinds) | Requires a robust "skip unknown chunk" path that we cannot exercise until v2.1 actually ships new chunk kinds. The format reserves chunk_kind ranges for future use; the reader logs WARN on unknown chunks but does not error. | v2.1 |
| O2 | Per-column compression (zstd or lz4 per ColumnChunk) | Adds CPU cost on hot path; needs benchmarking against typical workloads. v2.0 users may `gzip` the whole file for now. | v2.1 |
| O3 | Streaming/partial loads (load only positions, defer bond table) | Requires reader-API split between "metadata" and "payload" + lazy column population. Premature optimisation; full MoleculeStore must currently fit in memory. | v2.2 |
| O4 | Cross-language bindings (Python `.bf2` reader, JS web viewer) | Format is documented and stable enough for third parties to write readers, but bundled bindings wait for PyBALL re-enablement (post-K0). | v2.1+ |
| O5 | mmap-direct-access (reader returns pointers straight into the mmap'd region) | The format is *prepared* for this (16 B alignment, no internal compression, no per-record fixup) but the reader code does not implement it. Requires platform-specific paths; v2.0 reader copies into store-owned `std::vector` storage. | v2.2 |
| O6 | Multi-trajectory storage (N frames × atoms positions) | Trajectory storage is a different use case with different access patterns (e.g. random frame seeks). Better served by a separate `.bf2-traj` format that piggybacks on this spec for the static parts. | v2.x or v3.0 |
| O7 | Encrypted / signed files | No demand; can be layered externally with PGP/age. | TBD |
| O8 | Schema evolution for the property bag (e.g. typed property declarations at file level) | Property bag is sparse and untyped today; if a v2.1+ user wants typed-column promotion of hot properties (D3 reference), that's an in-memory optimisation, not a format change. | v2.1+ |
| O9 | Cross-document references that span multiple `.bf2` files | StableIdMap is per-file. A future "project file" format could chain multiple `.bf2` files with a cross-file index. | v2.x |
| O10 | Backwards roundtrip (v2 → v1.x) | No demand; v1.x is being retired. | Never. |

---

## 9. Open questions (must close before K0.6b plan locks)

These are **NOT** design decisions yet; K0.6b implementation must
resolve each. Most can be answered in <1 day of prototype work.

- **Q1.** CRC-32 polynomial: `0xEDB88320` (zlib) or `0x82F63B78`
  (Castagnoli, faster on x86 CRC32 instruction)? Recommend zlib for
  portability; revisit in v2.1.
- **Q2.** String pool dedup strategy: hash-on-write vs. one-pass
  sort+unique at end? Determinism (I11) requires sorted output;
  recommend "insert into `std::unordered_map<string, u32>` on demand,
  then re-emit sorted at end".
- **Q3.** Endianness on big-endian writers (e.g. PowerPC): is anyone
  actually using BALL on BE hosts? If not, writer asserts LE host
  and the BE swap path is dead code. Recommend: BE writer is a
  follow-on; v2.0 writer assumes LE host.
- **Q4.** Should the converter emit a `.bf2.log` companion file
  listing every WARN/ERROR? Useful for auditing large corpora.
  Recommend: yes, optional via `--log out.log`.
- **Q5.** PropertyBag value 0x7 (Vector3) currently expands the
  entry to 32 B instead of 24 B (§2.9). Alternative: separate
  "wide value" chunk. Simpler now, slower seek later. Recommend
  current 32 B-inline encoding; revisit if profiling shows pain.

---

## 10. Provenance & sign-off

- 2026-05-18 — Initial draft (this document). Authored against
  `v2.0` branch HEAD `fe1737ef2` (K0.3b.8); aware of
  K0-CODEX-REVIEW-ROUND2 amendments + K0.3c work-in-flight.
- TBD — K0.6a review sign-off (target: end of K0.6a 2-week window).
  Required signatories: KERNEL maintainer, FORMAT maintainer,
  release manager.
- TBD — Format frozen at K0.6b commit. Any post-freeze change
  requires `format_minor` bump and explicit amendment to this doc.

---

*End of STORE-FORMAT-SPEC.md. This is the K0.6a wire-format
contract; K0.6b implementation must match byte-for-byte.*

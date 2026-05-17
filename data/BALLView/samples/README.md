# BALLView sample structures

Phase 999.47 (Handover §7.2) — six curated structures surfaced by the
BALLView WelcomeScreen "Sample structures" grid.

| File             | Subject                       | Status |
|------------------|-------------------------------|--------|
| `1bna.pdb`       | B-form DNA (Drew-Dickerson)   | excerpt (4 bp); full structure via File > Open from PDB |
| `1ubq.pdb`       | Ubiquitin                     | **placeholder** (BPTI geometry; replace via 999.48 asset pipeline) |
| `1stp.pdb`       | Streptavidin + biotin         | **placeholder** (BPTI geometry; replace via 999.48 asset pipeline) |
| `1ymg.pdb`       | Myoglobin                     | **placeholder** (BPTI geometry; replace via 999.48 asset pipeline) |
| `caffeine.mol2`  | Caffeine (Tripos mol2)        | real geometry (24 atoms) |
| `dialanine.pdb`  | Alanine–alanine dipeptide     | real geometry (10 atoms) |

Thumbnails are 320×200 PNG. The @2× DPR pipeline (640×400 physical) is built
in Phase 999.48; for v1.7 RC1 we ship single-density placeholders that the
WelcomeScreen scales via Qt's `QPixmap` smooth-transform.

## Licensing

PDB entries cited above (`1BNA`, `1UBQ`, `1STP`, `1YMG`) are public-domain
deposits in the RCSB Protein Data Bank. The placeholder PDB content (BPTI
chain reused under each protein slot) is derived from the BALL repository's
existing `data/structures/bpti.pdb` test fixture.

Caffeine and dialanine coordinates are conventional small-molecule
geometries widely used as teaching examples; no third-party copyright
attaches.

Thumbnails are generated locally and original to this repository.

## Replacement workflow (deferred to 999.48)

The placeholder protein slots can be replaced by running a fetch script
(not yet authored) that downloads the canonical PDB entries from
`https://files.rcsb.org/download/<id>.pdb`, validates the result, and
re-renders the thumbnails via the 999.48 offline-renderer pipeline.

---
title: Loading molecules
order: 2
prev: 01-welcome.md
next: 03-models.md
prereq: 01-welcome.md
expected-runtime: 3 minutes
---

# Loading molecules

BALLView reads the common molecular file formats out of the box:
PDB, mol2, HIN, MOL, SDF, XYZ, CIF, and BALL's own native format
for whole-project snapshots.

## From a file

Use **File → Open…** (or the Welcome screen's *Open file…* button).
The dialog will auto-detect the format from the extension. After
loading, the new system appears under the **Structures** tab of
the Project dock.

## From the PDB / PubChem

**File → Open from PDB / PubChem…** opens the downloader. Type a
PDB ID (e.g. `1ubq`) or a compound name, and the file is fetched,
parsed, and displayed.

## From a sample

The Welcome screen ships six curated samples (DNA, ubiquitin,
streptavidin, myoglobin, caffeine, dialanine). Click any card to
load it directly — useful for exploring BALLView features without
hunting down a file.

[Try this → Open caffeine](ballview://open?file=BALLView/samples/caffeine.mol2)

## Recent files

The Welcome screen lists your five most recently opened files;
toggle "Skip this screen on startup" if you'd rather jump straight
to the empty 3D view.

Click **Next →** to pick a render style.

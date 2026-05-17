---
title: Saving your work
order: 6
prev: 05-coloring.md
next: null
prereq: 05-coloring.md
expected-runtime: 2 minutes
---

# Saving your work

BALLView distinguishes between **molecular files** (atoms +
coordinates), **project files** (the entire scene including
representations, colorings, camera, and lights), and **images**.

## Molecular files

**File → Save…** writes the active System back to disk in the
original or a chosen format (PDB, mol2, HIN, SDF, …). Useful when
you've performed edits — added hydrogens, removed solvent, run a
minimization — and want to hand off the result.

## Project files (BALL .bvp)

**File → Save Project…** captures everything: the loaded systems,
each Representation with its model and coloring, the camera, the
lights, the workspace layout. Reload with **File → Open Project…**
and the scene reconstructs exactly.

This is the preferred path for collaboration ("here's the figure
I was working on") and for resuming long sessions.

## Images

**File → Export → Image…** renders the current view to PNG / JPEG
at a configurable resolution. For publication-quality figures use
**File → Export → POV-Ray…** which writes a POV-Ray scene file —
trace it externally for ray-traced reflection, refraction, and
ambient occlusion.

## Where to go next

- **Tools → Tutorials** for deeper-dive lessons (energy minimization,
  trajectory playback, docking, surface analysis).
- The user guide at **Help → User guide** covers the full menu.

[Try this → Save project](ballview://command/file.saveProject)

You've finished the BALLView quick-start. Happy modelling.

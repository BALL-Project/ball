---
title: Representations
order: 3
prev: 02-loading.md
next: 04-selection.md
prereq: 02-loading.md
expected-runtime: 3 minutes
---

# Representations (rendering styles)

A **Representation** in BALLView is a recipe for turning the
loaded molecule (or any subset of it) into something visible:
spheres, sticks, surface, ribbon, cartoon, and so on. A single
system can carry several Representations at once — for example,
a cartoon backbone with sticks for a single ligand.

## The Representations tab

The **Representations** tab in the Project dock lists every active
Representation. Each one has a checkbox (visibility), a name, a
selection scope ("all atoms" by default), and an underlying model
type.

## Common models

| Model | Best for |
|-------|----------|
| **Lines** | Quick chemistry sketch — small molecules |
| **Stick** | Detailed small molecules and ligands |
| **Ball & Stick** | Pedagogy; atoms visible without occluding bonds |
| **VDW spheres** | Packing studies |
| **Ribbon / Cartoon** | Protein backbone traces |
| **SES / SAS** | Surface analysis (Solvent Excluded / Accessible) |

Select a Representation and use the **Inspector → Model** section
on the right to swap models or tune precision.

## Adding a Representation

Right-click any Composite (atom, residue, chain, system) in the
Structures tab → **Create Representation…**. Or use **Build →
Create Representation** from the menubar.

[Try this → Open the Command palette](ballview://command/view.commandPalette)

Click **Next →** to make a selection.

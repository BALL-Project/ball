---
title: Coloring
order: 5
prev: 04-selection.md
next: 06-saving.md
prereq: 04-selection.md
expected-runtime: 3 minutes
---

# Coloring

Color encodes information directly into the render. BALLView
ships several built-in coloring methods that you can apply to a
Representation, a selection, or the whole system.

## Built-in coloring methods

| Method | What it shows |
|--------|---------------|
| **Element** | CPK colors (C grey, N blue, O red, S yellow, …) |
| **Residue type** | One color per amino-acid type |
| **Residue index** | Gradient along the chain |
| **Chain** | One color per chain |
| **Secondary structure** | α-helix vs β-sheet vs loop |
| **B-factor** | Heat map from crystallographic B-factors |
| **Charge** | Heat map (red ↔ blue) for partial charges |
| **Custom** | Set per-atom colors from a CSV |

## Apply a color

1. Make a selection (Tutorial 4).
2. **Inspector → Coloring** section, or **Build → Coloring**.
3. Pick a method and a palette.
4. Click **Apply** — the active Representation updates.

## Coloring a surface by electrostatics

For SES / SAS surfaces you can map an electrostatic potential
(or any per-atom scalar) onto the surface. Generate the field via
**Tools → Electrostatics → Solve Poisson-Boltzmann**, then
**Coloring → By scalar field**.

[Try this → Coloring section](ballview://command/inspector.coloring)

Click **Next →** to save your work.

---
title: Selecting atoms
order: 4
prev: 03-models.md
next: 05-coloring.md
prereq: 03-models.md
expected-runtime: 3 minutes
---

# Selecting atoms

Selections are the basis for almost every BALLView operation:
coloring, hiding, deleting, creating new Representations, and
exporting subsystems.

## In the 3D view

- **Shift + left-click** an atom or bond to add it to the selection.
- **Shift + right-click** to remove from the selection.
- **Cmd/Ctrl + A** selects everything.
- **Esc** clears the selection.

## In the Structures tab

Click any Composite to focus it. Shift-click a range; Cmd/Ctrl
click to toggle individual entries. Selections in the tree mirror
selections in the 3D view.

## Selection by expression

**Select → By Expression…** (or the Command palette → "select by
expression") opens a query field. Examples:

- `element(C)` — all carbon atoms
- `residue(LYS)` — every lysine residue
- `chain(A) and element(N)` — nitrogens on chain A
- `not solvent` — drop waters and ions

The Inspector's **Selection** tab summarizes the current selection
(counts by element, residue type, chain).

[Try this → Select all](ballview://command/select.all)

Click **Next →** to color the selection.

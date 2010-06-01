# select all residues in proximity of less than maxdistance from selected atoms
maxdistance = 10

system = getSystems()[0]

cl = []

for atom1 in atoms(system):
  if not atom1.isSelected(): 
    continue
  for residue in residues(system):
    close=False
    for atom2 in atoms(residue):
      dist = (atom1.getPosition() - atom2.getPosition()).getLength()
      if not atom2.isSelected() and dist < maxdistance:
        close=True
        break
    if close:
      cl.append(residue)

system.deselect()

for residue in cl:
  for atom in atoms(residue):
    if not atom.isSelected():
      atom.select()


getMainControl().update(system)


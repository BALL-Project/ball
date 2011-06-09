# select all residues in proximity of less than maxdistance from selected atoms
maxdistance = 10

cl = []

for system1 in getSystems():
  for atom1 in atoms(system1):
    if not atom1.isSelected(): 
      continue
    for system in getSystems():
      for residue in residues(system):
        close=False
        for atom2 in atoms(residue):
          dist = (atom1.getPosition() - atom2.getPosition()).getLength()
          if not atom2.isSelected() and dist < maxdistance:
            close=True
            break
        if close:
          cl.append(residue)
  
for system in getSystems():
  system.deselect()

print "selected", len(cl), "residues"

for residue in cl:
  for atom in atoms(residue):
    if not atom.isSelected():
      atom.select()

for sysem in getSystems():
  getMainControl().update(system)


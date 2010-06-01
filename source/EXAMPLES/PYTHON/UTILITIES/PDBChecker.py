# Lara Schneider 2010-04-22

import sys
from BALL import *

#print a usage message, if called with the wrong number
#of arguments
if (len(sys.argv) != 2):
  print sys.argv[0], " <pdbfile>"
  print "  checks the PDB file for consistency."
  exit()

#create a System and read the contents of the PDB file
S = System()
pdb_file = PDBFile(sys.argv[1])
if (not pdb_file):
  print "cannot open file ", argv[1]
  exit()
pdb_file.read(S)
pdb_file.close()

#create a fragment database
#The fragment database contains information on
#all amino acids: structure, names, bond distances, and so on.
db = FragmentDB("")

#normalize the names
#This processor converts the atom names from several naming
#conventions to the standard PDB names
S.apply(db.normalize_names)

#build bonds
#Since PDB files normally do not contain bond information
#we have to reconstruct the bonds from the templates in
# the fragment database
S.apply(db.build_bonds)
	
#now we create a ResidueChecker object and apply it
#This object checks all residues it is given and 
#prints warnings to Log if 
#  - bond distances deviate from their usual values
#  - non integral charges of a residues (does not apply here)
#  - missing or superfluous atoms in a residue
check = ResidueChecker(db)
S.apply(check)

#done.
print "done."

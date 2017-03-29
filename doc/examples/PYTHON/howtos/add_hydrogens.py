#
# Anna Dehof 2010-03-22
#   get a system and add hydrogens
#

import sys
from  BALL import *

#### for use in BALLView
#system = getSystems()[0]

#### for command line use	
# issue a usage hint if called without parameters
if (len(sys.argv) != 3 ):
  print sys.argv[0] , " <PDB infile> <PDB outfile>" 
  exit()

# open a PDB file with the name of the first argument
file = PDBFile(sys.argv[1])
if (not file):
  # if file does not exist: complain and abort
  print "error opening ", sys.argv[1], " for input."
  exit ()

# create a system and read the contents of the PDB file
system = System()
file.read(system)
file.close()
# print the number of atoms read from the file
print "read ", system.countAtoms(), " atoms."

# now we open a fragment database
print "reading fragment DB..." 
fragment_db = FragmentDB("")

# and normalize the atom names, i.e. we convert different
# naming standards to the PDB naming scheme - just in case!
print "normalizing names..." 
system.apply(fragment_db.normalize_names)

# now we add any missing hydrogens to the residues
# the data on the hydrogen positions stems from the
# fragment database. However the hydrogen positions 
# created in this way are only good estimates
print "creating missing atoms..." 
system.apply(fragment_db.add_hydrogens)
print "added ", fragment_db.add_hydrogens.getNumberOfInsertedAtoms(), " atoms" 

# now we create the bonds between the atoms (PDB files hardly
# ever contain a complete set of CONECT records)							
print "building bonds..."
system.apply(fragment_db.build_bonds)

# now we check whether the model we built is consistent
# The ResidueChecker checks for charges, bond lengths,
# and missing atoms
print "checking the built model..." 
checker = ResidueChecker(fragment_db)
system.apply(checker)

# now we create an AMBER force field 
print "setting up force field..." 
FF= AmberFF()

# we then select all hydrogens (element(H))
# using a specialized processor (Selector)
system.deselect()
FF.setup(system)
selector = Selector("element(H)")
system.apply(selector)

#just for curiosity: check how many atoms we are going
# to optimize
print "optimizing ", FF.getNumberOfMovableAtoms(), " out of ", system.countAtoms(), " atoms"	

# now we create a minimizer object that uses a conjugate 
# gradient algorithm to optimize the atom positions
minimizer = ConjugateGradientMinimizer()
initial_energy = FF.updateEnergy()
print "initial energy: ", initial_energy , " kJ/mol" 

# initialize the minimizer and perform (up to)
# 50 optimization steps
minimizer.setup(FF)
minimizer.setEnergyOutputFrequency(1)
minimizer.minimize(50)

# calculate the terminal energy and print it
terminal_energy = FF.getEnergy()
print "energy before/after minimization: ", initial_energy , "/", terminal_energy, " kJ/mol" 


#### for command line use 
# write the optimized structure to a file whose
# name is given as the second command line argument
print "writing PBD file ", sys.argv[2] 
outfile = PDBFile(sys.argv[2],  File.MODE_OUT)
outfile.write(system)
outfile.close()

# done



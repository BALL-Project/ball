#
# Lara Schneider 2010-05-20
#
# Computing the binding energy of two molecules
#

import sys
from BALL import *

# issue a usage hint if called without parameters
if (len(sys.argv) != 3):
  print sys.argv[0], " <pdb file A> <pdb file B>"
  exit()

# open the first PDB file with MolFileFactory
file1 = MolFileFactory.open(argv[1])
if (not file1):
    # if file does not exist: complain and abort
    print "error opening ", sys.argv[1], " for input."
    exit()

# create a system and read the contents of the first PDB file
system1 = System()
file1.read(system1)
file1.close()

# open the second PDB file with MolFileFactory
file2 = MolFileFactory.open(argv[2])
if (not file2):
    # if file does not exist: complain and abort
    print "error opening ", sys.argv[2], " for input."
    exit()

# create a system and read the contents of the second PDB file
system2 = System()
file2.read(system2)
file2.close()

# normalize the names and build all bonds
print "normalizing names and building all bonds..."
db = FragmentDB("")
system1.apply(db.normalize_names)
system1.apply(db.build_bonds)
system2.apply(db.normalize_names)
system2.apply(db.build_bonds)
 
# calculate the atomic contact energies of file1 and file2
  float ACE_system1 = calculateACE(system1)
  float ACE_system2 = calculateACE(system2)

# calculate the electrostatic energies of file1 and file2
amber = AmberFF()
amber.options[AmberFF.Option.ASSIGN_CHARGES] = "true"
amber.options[AmberFF.Option.OVERWRITE_CHARGES] = "true"
amber.setup(system1)
amber.updateEnergy()
float ES_system1 = amber.getESEnergy()
float C_system1 = calculateCoulomb(system1)
amber.setup(system2)
amber.updateEnergy()
float ES_system2 = amber.getESEnergy()
float C_system2 = calculateCoulomb(system2)

# finally, join the to systems into a single system
print "atoms in system1:  ", system1.countAtoms()
print "atoms in system2:  ", system2.countAtoms()
system1.splice(system2)
print "final atoms: ", system1.countAtoms()
float ACE_system1&system2 = calculateACE(system1)
amber.setup(system1)
amber.updateEnergy()
float ES_system1&system2 = amber.getESEnergy()
float C_system1&system2 = calculateCoulomb(system1)

# print the resulting energies
print "ES energy of system1: ", ES_system1
print "ES energy of system2: ", ES_system2
print "ES energy of system1&system2: ", ES_system1&system2
print "C energy of system1: ", C_system1
print "C energy of system2: ", C_system2
print "C energy of system1&system2: ", C_system1&system2
print "change in atomic contact energy on binding:   ", (ACE_system1&system2 - ACE_system1 - ACE_system2), " kJ/mol"
print "change in electrostatic energy on binding:    ", (ES_system1&system2 - ES_system1 - ES_system2), " kJ/mol"
print "total binding free energy:                    ", (ACE_system1&system2 - ACE_system1 - ACE_system2) + (ES_system1&system2 - ES_system1 - ES_system2), " kJ/mol"

# done






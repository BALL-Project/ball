#
# Anna Dehof 2011-10-20
#   Compute and assign bond orders for a given 
#   ligand based on atomic valence penalty rules.
#

import sys
from BALL import *

# for use in BALLView
#system = getSystems()[0]

#### for command line use	
# issue a usage hint if called without parameters
if (len(sys.argv) != 3 ):
  print"Usage: ", sys.argv[0] , " <Ligand infile> <Ligand outfile>" 
  exit()

# open the file
file = MOL2File(sys.argv[1], File.MODE_IN)

if (not file):
  # if file does not exist: complain and abort
  print "error opening ", sys.argv[1], " for input."
  exit ()

# create a system and read the contents of the PDB file
system = System()
file.read(system)
file.close()

# print the number of atoms read from the file
print "Read", system.countAtoms(), "atoms and", system.countBonds(), "bonds from file"
print  "   ", sys.argv[1]

# create an BondOrderAssignmentProcessor
abop = AssignBondOrderProcessor()
abop.options.setBool(AssignBondOrderProcessor.Option.KEKULIZE_RINGS, True) 
abop.options.setBool(AssignBondOrderProcessor.Option.OVERWRITE_SINGLE_BOND_ORDERS, True)
abop.options.setBool(AssignBondOrderProcessor.Option.OVERWRITE_DOUBLE_BOND_ORDERS, True)
abop.options.setBool(AssignBondOrderProcessor.Option.OVERWRITE_TRIPLE_BOND_ORDERS, True)
abop.options.set(AssignBondOrderProcessor.Option.ALGORITHM, AssignBondOrderProcessor.Algorithm.A_STAR)
abop.options.setReal(AssignBondOrderProcessor.Option.BOND_LENGTH_WEIGHTING,0)
abop.options.setInteger(AssignBondOrderProcessor.Option.MAX_NUMBER_OF_SOLUTIONS,10)
abop.options.setBool(AssignBondOrderProcessor.Option.COMPUTE_ALSO_NON_OPTIMAL_SOLUTIONS, True)
abop.options.setBool(AssignBondOrderProcessor.Option.ADD_HYDROGENS, False)

print "Current penalty: " , abop.evaluatePenalty(system)

# apply the processor
system.apply(abop)

print "Computed", abop.getNumberOfComputedSolutions(), "bond order assignments:"

# print all solutions penalty
for i in range(abop.getNumberOfComputedSolutions()):
  print "   solution", str(i) , ": penalty", str(abop.getTotalPenalty(i)) #, " , ", abop.getNumberOfAddedHydrogens(i) , " added hydrogens."	
  # apply the i-th solution
  if (abop.apply(i)):
    # and write the solution into an MOL2file	
    outfile_name = String(sys.argv[2]) + "_" + str(i) + ".mol2";
    print "   Writing solution", i, "as", outfile_name
    outfile = MOL2File(outfile_name, File.MODE_OUT)
    outfile.write(system)
    outfile.close()

# done
print "Done."

# for use in BALLView
#getMainControl().update(system)



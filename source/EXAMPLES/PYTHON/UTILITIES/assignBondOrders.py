#
# Anna Dehof 2009-02-02
#   get the system and assign bond orders 
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
print "read ", system.countAtoms(), " atoms."
print "read ", system.countBonds(), " bonds."

# create an BondOrderAssignmentProcessor
abop = BALL.AssignBondOrderProcessor()
abop.options.setBool(BALL.AssignBondOrderProcessor.Option.KEKULIZE_RINGS, True) 
abop.options.setBool(BALL.AssignBondOrderProcessor.Option.OVERWRITE_SINGLE_BOND_ORDERS, True)
abop.options.setBool(BALL.AssignBondOrderProcessor.Option.OVERWRITE_DOUBLE_BOND_ORDERS, True)
abop.options.setBool(BALL.AssignBondOrderProcessor.Option.OVERWRITE_TRIPLE_BOND_ORDERS, True)
abop.options.set(BALL.AssignBondOrderProcessor.Option.ALGORITHM, BALL.AssignBondOrderProcessor.Algorithm.A_STAR)
abop.options.setReal(BALL.AssignBondOrderProcessor.Option.BOND_LENGTH_WEIGHTING,0)
abop.options.setInteger(BALL.AssignBondOrderProcessor.Option.MAX_NUMBER_OF_SOLUTIONS,10)
abop.options.setBool(BALL.AssignBondOrderProcessor.Option.COMPUTE_ALSO_NON_OPTIMAL_SOLUTIONS, True)
abop.options.setBool(BALL.AssignBondOrderProcessor.Option.ADD_HYDROGENS, False)

print "Current penalty: " , abop.evaluatePenalty(system)

# apply the processor
system.apply(abop)

# print all solutions penalty
for i in range(abop.getNumberOfComputedSolutions()):
  print "solution ", str(i) , ": penalty ", str(abop.getTotalPenalty(i)) #, " , ", abop.getNumberOfAddedHydrogens(i) , " added hydrogens."

# apply the last solution
abop.apply(abop.getNumberOfComputedSolutions()-1)


# for use in BALLView
#getMainControl().update(system)



#
# Anna Dehof 2009-02-02
#   get the system and assign bond orders 
#

import BALL

# for use in BALLView
#system = getSystems()[0]

# for use in BALL
system = BALL.System()
file = BALL.MOL2File("../../TEST/data/AssignBondOrderProcessor_test_CITSED10_sol_2.mol2")
file.read(system)

abop = BALL.AssignBondOrderProcessor()
abop.options.setBool(BALL.AssignBondOrderProcessor.Option.KEKULIZE_RINGS, True) 
abop.options.setBool(BALL.AssignBondOrderProcessor.Option.OVERWRITE_SINGLE_BOND_ORDERS, True)
abop.options.setBool(BALL.AssignBondOrderProcessor.Option.OVERWRITE_DOUBLE_BOND_ORDERS, True)
abop.options.setBool(BALL.AssignBondOrderProcessor.Option.OVERWRITE_TRIPLE_BOND_ORDERS, True)
abop.options.set(BALL.AssignBondOrderProcessor.Option.ALGORITHM, BALL.AssignBondOrderProcessor.Algorithm.A_STAR)
abop.options.setReal(BALL.AssignBondOrderProcessor.Option.BOND_LENGTH_WEIGHTING,0)
abop.options.setInteger(BALL.AssignBondOrderProcessor.Option.MAX_NUMBER_OF_SOLUTIONS,10)
abop.options.setBool(BALL.AssignBondOrderProcessor.Option.COMPUTE_ALSO_NON_OPTIMAL_SOLUTIONS, True)
abop.options.setBool(BALL.AssignBondOrderProcessor.Option.ADD_HYDROGENS, True)

print "Current penalty: " , abop.evaluatePenalty(system)

# apply the processor
system.apply(abop)

# print all solutions penalty
for i in range(abop.getNumberOfComputedSolutions()):
  print "solution ", str(i) , ": penalty ", str(abop.getTotalPenalty(i)), " , ", abop.getNumberOfAddedHydrogens(i) , " added hydrogens."

# apply the last solution
abop.apply(abop.getNumberOfComputedSolutions()-1)


# for use in BALLView
#getMainControl().update(system)



#
# Anna Dehof 2008-09-02
#   get the system and assign bond orders 
#

import BALL

system = getSystems()[0]
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

print abop.evaluatePenalty(system)

system.apply(abop)
for i in range(abop.getNumberOfComputedSolutions()):
  print "solution ", str(i) , ": penalty ", str(abop.getTotalPenalty(i)), ", charge ", abop.getTotalCharge(i), " , ", abop.getNumberOfAddedHydrogens(i) , " added hydrogens."


abop.apply(abop.getNumberOfComputedSolutions()-1)

getMainControl().update(system)



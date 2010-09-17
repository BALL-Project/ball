#
# Anna Dehof 2010-09-15
#   read a NMR and assign the shifts to a proteins' atoms
#
import sys
from BALL import *
import BALL

#### for use in BALLView
#system = getSystems()[0]

#### for command line use	
# issue a usage hint if called without parameters
#if (len(sys.argv) != 3 ):
#  print"Usage: ", sys.argv[0] , "<NMR file> <PDB file>" 
#  exit()

# read the NMRStar file
f = BALL.NMRStarFile("../source/TEST/data/NMRStarFile_test_1z0r.bmr", BALL.File.MODE_IN)
#f = BALL.NMRStarFile(sys.argv[1], BALL.File.MODE_IN)
f.read()

print f.getNumberOfAtoms()

# read the corresponding PDB file
p = BALL.PDBFile("../source/TEST/data/NMRStarFile_test_1z0r.pdb", BALL.File.MODE_IN)
#p = BALL.PDBFile(sys.argv[2], BALL.File.MODE_IN)
S = BALL.System()
p.read(S)

# get the sequences to compute an alignment
print BALL.Peptides.GetSequence(chains(S)[0])
print f.getResidueSequence(0)

# create a mapping based on an alignment
mapper = BALL.NMRStarFile.BALLToBMRBMapper(chains(S)[0], f)
mapper.createMapping("MKSTGIVRKVDELGR-VVIPIELRRTLGIAEKDALEIYVDDEKIILKKYKPNMT", "MKSTGIVRKVDELGRVV-IPIELRRTLGIAEKDALEIYVDDEKIILKKYKPNMT")
#mapper.createTrivialMapping()
print mapper.getNumberOfMismatches()
print mapper.getNumberOfGabs()

# assign the shifts
f.assignShifts(mapper)

# get some print outs
for a in BALL.atoms(S):
  if a.hasProperty("ExperimentalChemicalShift"):
#    print a.getProperty("ExperimentalChemicalShift").getFloat()
    print a.getProperty(ShiftModule.PROPERTY__EXPERIMENTAL__SHIFT).getFloat()


p.close()
# done

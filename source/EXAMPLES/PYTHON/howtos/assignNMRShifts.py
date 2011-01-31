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
nmr_file = BALL.NMRStarFile("../../../TEST/data/NMRStarFile_test_1z0r.bmr", BALL.File.MODE_IN)
#f = BALL.NMRStarFile(sys.argv[1], BALL.File.MODE_IN)
if not nmr_file:
  print "Could not open given file, aborting"
  exit(-1)

#nmr_file.read()

print nmr_file.getNumberOfAtoms()

# read the corresponding PDB file
p = BALL.PDBFile("../../../TEST/data/NMRStarFile_test_1z0r.pdb", BALL.File.MODE_IN)
#p = BALL.PDBFile(sys.argv[2], BALL.File.MODE_IN)
if not p:
  print "Could not open given file, aborting"
  exit(-1)

S = BALL.System()
p.read(S)

# get the sequences to compute an alignment
print BALL.Peptides.GetSequence(chains(S)[0])
print nmr_file.getResidueSequence(0)

# get a chemical unit of this file
chem_unit = nmr_file.getMolecularInformation().getChemicalUnit(0).label 
print nmr_file.getMolecularInformation().system_name

# create a mapping based on an alignment
mapper = BALL.NMRStarFile.BALLToBMRBMapper(chains(S)[0], nmr_file, chem_unit)
mapper.createMapping("MKSTGIVRKVDELGR-VVIPIELRRTLGIAEKDALEIYVDDEKIILKKYKPNMT", "MKSTGIVRKVDELGRVV-IPIELRRTLGIAEKDALEIYVDDEKIILKKYKPNMT")
#mapper.createTrivialMapping()
print mapper.getNumberOfMismatches()
print mapper.getNumberOfGabs()

# assign the shifts
nmr_file.assignShifts(mapper)

# get some print outs
for a in BALL.atoms(S):
  if a.hasProperty("ExperimentalChemicalShift"):
#    print a.getProperty("ExperimentalChemicalShift").getFloat()
    print a.getProperty(ShiftModule.PROPERTY__EXPERIMENTAL__SHIFT).getFloat()


p.close()
# done

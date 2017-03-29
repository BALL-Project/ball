#
# Anna Dehof 2010-09-15
#   read a NMR and assign the shifts to a proteins' atoms
#
import sys
from BALL import *

#### for use in BALLView
#system = getSystems()[0]

#### for command line use	
# issue a usage hint if called without parameters
#if (len(sys.argv) != 3 ):
#  print"Usage: ", sys.argv[0] , "<NMR file> <PDB file>" 
#  exit()

# read the NMRStar file
nmr_file = NMRStarFile("../../../TEST/data/NMRStarFile_test_1z0r.bmr", File.MODE_IN)
#nmr_file = NMRStarFile(sys.argv[1], File.MODE_IN)
if not nmr_file:
  print "Could not open given file, aborting"
  exit(-1)

print "File contains ", nmr_file.getNumberOfAtoms(), " chemcial shifts"

# read the corresponding PDB file
p = PDBFile("../../../TEST/data/NMRStarFile_test_1z0r.pdb", File.MODE_IN)
#p = PDBFile(sys.argv[2], File.MODE_IN)
if not p:
  print "Could not open given file, aborting"
  exit(-1)

S = System()
p.read(S)

# get the sequences to compute an alignment
print Peptides.GetSequence(chains(S)[0])
print nmr_file.getResidueSequence(0)

# get a chemical unit of this file
chem_unit = nmr_file.getMolecularInformation().getChemicalUnit(0).label 
print "Label of chemical unit: ", chem_unit
print "Molecular systems name: ", nmr_file.getMolecularInformation().system_name

# create a mapping based on an alignment
mapper = NMRStarFile.BALLToBMRBMapper(chains(S)[0], nmr_file, chem_unit)
mapper.createMapping("MKSTGIVRKVDELGR-VVIPIELRRTLGIAEKDALEIYVDDEKIILKKYKPNMT", "MKSTGIVRKVDELGRVV-IPIELRRTLGIAEKDALEIYVDDEKIILKKYKPNMT")

print "Number of mismatches: ", mapper.getNumberOfMismatches()
print "Number of gaps: ", mapper.getNumberOfGaps()

# assign the shifts
nmr_file.assignShifts(mapper)

# get some print outs
for a in atoms(S):
  if a.hasProperty("ExperimentalChemicalShift"):
#    print a.getProperty("ExperimentalChemicalShift").getFloat()
    print a.getProperty(ShiftModule.PROPERTY__EXPERIMENTAL__SHIFT).getFloat()

p.close()

# we are done

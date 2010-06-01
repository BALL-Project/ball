# -*- coding: utf-8 -*-
# 
# Lara Schneider
# 2010-05-20
#
# TODO: description
# for use in BALLView
#

import sys
from BALL import *

system = getSystems()[0]

#open a fragment database
print "reading fragment DB..."
db = FragmentDB("")

# and normalize the atom names, i.e. we convert different
# naming standards to the PDB naming scheme - just in case!
print "normalizing names..."
system.apply(db.normalize_names)

# now build the bonds
print "building bonds..."
system.apply(db.build_bonds)

# adding missing hydrogens, however the H-positions created
# in this way are only good estimates
print "creating missing hydrogens..."
system.apply(db.add_hydrogens)

# now we check whether the model we built is consistent
# ResidueChecker checks charges, bond lengths and missing atoms
print "checking the build model..."
checker = ResidueChecker(db)
system.apply(checker)

# creating an AMBER force field and set its options from a file
amber = AmberFF()
if (len(sys.argv)>2):
  amber.options.readOptionFile(sys.argv[2])

# set up the force field
print "setting up force field..."
amber.setup(system)

# calculate the total energy and print it
print "total energy: ", amber.updateEnergy(), " kJ/mol"
print "stretch: ", amber.getStretchEnergy(), " kJ/mol"
print "bend: ", amber.getBendEnergy(), " kJ/mol"
print "torsion: ", amber.getTorsionEnergy(), " kJ/mol"
print "ES: ", amber.getESEnergy(), " kJ/mol"
print "vdW: ", amber.getVdWEnergy(), " kJ/mol"

# alternatively
# amber.getResults()

# done











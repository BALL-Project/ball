# -*- coding: utf-8 -*-
#
# Lara Schneider 2010-05-20
#
# description
#

import sys
from BALL import *

# for use in BALLView
system1 = getSystems()[0]
system2 = getSystems()[1]

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
print "atoms in file1:  ", system1.countAtoms()
print "atoms in file2:  ", system2.countAtoms()
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






#============================================================================
# BALL -  Example for a energy evaluations as it was used in Althaus 
# et al. "A branch and cut algorithm for the optimal solution of the 
# side-chain placement problem", 2000
#
# This example reads a PDB file and calculates a bonded energy using a force
# field and a non bonded energy (electrostatics only) by solving the Poisson-
# Boltzmann equation.
#============================================================================
#
# Lara Schneider 2010-04-22
#

import sys
from BALL import *

s = getSystems()[0]
	
#normalize the names and build all bonds
print "normalizing names and building bonds..."
db = FragmentDB("")
s.apply(db.normalize_names)
s.apply(db.build_bonds)

#create an AMBER force field without non-bonded interactions
FF = AmberFF(s);
	
#calculate the total energy
total_energy = FF.updateEnergy();
print FF.getResults()
print "   total energy using force field evaluation: ", total_energy, " kJ/mol"
print "removing non bonded energy terms ..."
FF.removeComponent("Amber NonBonded")
	
#calculate the internal energy (neglecting internal VdW interactions)
internal_energy = FF.updateEnergy()
print FF.getResults()
print "   internal energy: ", internal_energy, " kJ/mol"

	
#assign atom radii
radius_processor = AssignRadiusProcessor("radii/PARSE.siz")
s.apply(radius_processor)

#calculate the electrostatic part of the solvation energy	
print "calculating electrostatic energy terms with FD-Poisson-Boltzmann ..."

fdpb = FDPB(s)
fdpb.solve();
print "... using dielectric constant in medium: ", fdpb.options[FDPB.Option.SOLVENT_DC].toFloat()
solvent_energy = fdpb.getEnergy()
	
fdpb.options[FDPB.Option.SOLVENT_DC] = 1.0
print "... using dielectric constant in vacuum: ", fdpb.options[FDPB.Option.SOLVENT_DC].toFloat()
fdpb.setup(s)
fdpb.solve()
vacuum_energy = fdpb.getEnergy()
print "\n   electrostatic solvation free energy: ", solvent_energy - vacuum_energy
	
print "   total energy using a combination of force field and FDPB evaluation: ", internal_energy - vacuum_energy + solvent_energy, " kJ/mol"

#that's it
print "done."

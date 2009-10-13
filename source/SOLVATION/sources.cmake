### list all filenames of the directory here ###
SET(SOURCES_LIST
	claverieParameter.C
	electrostaticPotentialCalculator.C
	generalizedBornCase.C
	molecularSurfaceGrid.C
	ooiEnergy.C
	PCMCavFreeEnergyProcessor.C
	pierottiCavFreeEnergyProcessor.C
	pair6_12InteractionEnergyProcessor.C
	pair6_12RDFIntegrator.C
	pairExpInteractionEnergyProcessor.C
	pairExpRDFIntegrator.C
	poissonBoltzmann.C
	reissCavFreeEnergyProcessor.C
	solventDescriptor.C
	solventParameter.C
	uhligCavFreeEnergyProcessor.C
)

ADD_BALL_SOURCES("SOLVATION" "${SOURCES_LIST}")

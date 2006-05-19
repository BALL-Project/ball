DIRECTORY=STRUCTURE/DOCKING

THISLIB=$(LIBNAME)

FFT_DEPENDENT_SOURCES=$(BALL_HAS_FFTW:true=geometricFit.C)

CPP_SOURCES= \
	dockResult.C\
	dockingAlgorithm.C\
	conformationSet.C\
	energeticEvaluation.C\
	forceFieldEvaluation.C\
	amberEvaluation.C\
	randomEvaluation.C\
	$(FFT_DEPENDENT_SOURCES)\
	geneticMating.C\
  genericMapping.C\
  genericEvolutionaryAlgorithm.C\
  genericPoolUpdater.C\
  quaternionGene.C\
  genericGene.C\
  doubleGene.C\
  geneticPool.C\
  selectionStrategy.C\
  rankWeighting.C\
  energyGrid.C\
  rotateBonds.C\
  dockMapping.C\
  geneticMutation.C\
  standardPoolUpdater.C\
  geneticIndividual.C\
  geneticAlgorithm.C\
  evolutionaryDocking.C\
  rotateBond.C\
  assignGridTypes.C\
  convert.C


include ../../common.mak
include .Dependencies

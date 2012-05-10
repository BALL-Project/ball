### the directory name ###
SET(DIRECTORY source/DOCKING/GENETICDOCK)

### list all filenames of the directory here ###
SET(SOURCES_LIST
  dockProblem.C
  evolutionaryDocking.C
  genes.C
  geneticAlgorithm.C
  geneticIndividual.C
  parameter.C
  rotateBond.C
  rotateBonds.C
)

ADD_BALL_SOURCES("DOCKING/GENETICDOCK" "${SOURCES_LIST}")

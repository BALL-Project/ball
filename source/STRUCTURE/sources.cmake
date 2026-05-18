INCLUDE(source/STRUCTURE/BONDORDERS/sources.cmake)

### list all filenames of the directory here ###
# B1.2 (Track B Wave 1b, 2026-05-18): minimum-viable STRUCTURE enable.
# Goal: unblock FragmentDB_test / DefaultProcessors_test / Peptides_test
# / PeptideBuilder_test / PeptideCapProcessor_test / ResidueChecker_test
# / NormalizeNamesProcessor_test / surface-tests / RMSD-related tests
# without flipping MOLMEC/QSAR on (those are Waves 3-4).
#
# Trimmed (require MOLMEC, QSAR, or trimmed FORMAT bits — see also
# the trim list in source/FORMAT/sources.cmake):
# - DNAMutator.C              : AmberFF, EnergyMinimizer (MOLMEC, Wave 3)
# - RDFParameter.C            : ForceFieldParameters, AtomTypes (MOLMEC, Wave 3)
# - addHydrogenProcessor.C    : MMFF94StretchParameters (MOLMEC, Wave 3)
# - assignBondOrderProcessor.C: AromaticityProcessor, RingPerceptionProcessor (QSAR, Wave 4)
# - buildBondsProcessor.C     : RingPerceptionProcessor (QSAR, Wave 4)
# - hybridisationProcessor.C  : RingPerceptionProcessor (QSAR, Wave 4)
# - ringAnalyser.C            : RingPerceptionProcessor (QSAR, Wave 4)
# - smartsMatcher.C           : RingPerceptionProcessor (QSAR, Wave 4)
# - atomTyper.C               : transitive SmartsMatcher
# - kekulizer.C               : transitive SmartsMatcher
# - molecularSimilarity.C     : transitive SmartsMatcher
# - rotamerLibrary.C          : SCWRLRotamerFile (FORMAT-trimmed; Wave 1b cycle)
# - sideChainPlacementProcessor.C : transitive RotamerLibrary
# - sdGenerator.C             : transitive RingAnalyser (uses Ring/CFS APIs)
#
# Note: SMARTSPredicate (KERNEL/standardPredicates.h) #includes
# <BALL/STRUCTURE/smartsMatcher.h>. Header is present and compiles
# because it doesn't pull RingPerceptionProcessor at parse time, but the
# SMARTS expression-predicate count fails by 1 in Expression_test ("27
# expected, got 26") until QSAR comes back in Wave 4. Documented gap.
SET(SOURCES_LIST
	analyticalSES.C
	atomBijection.C
	binaryFingerprintMethods.C
	bindingPocketProcessor.C
	connectedComponentsProcessor.C
	connolly.C
	defaultProcessors.C
	disulfidBondProcessor.C
	fragmentDB.C
	geometricProperties.C
	geometricTransformations.C
	graphEdge.C
	graphFace.C
	graphVertex.C
	HBondProcessor.C
	mutator.C
	nucleotideMapping.C
	numericalSAS.C
	peptides.C
	peptideBuilder.C
	peptideCapProcessor.C
	radialDistributionFunction.C
	reconstructFragmentProcessor.C
	RDFIntegrator.C
	RDFSection.C
	reducedSurface.C
	residueChecker.C
	rGroupAssembler.C
	RSEdge.C
	RSFace.C
	RSVertex.C
	SASEdge.C
	SASFace.C
	SASVertex.C
	SESEdge.C
	SESFace.C
	SESVertex.C
	secondaryStructureProcessor.C
	smilesParser.C
	smartsParser.C
	solventAccessibleSurface.C
	solventExcludedSurface.C
	structureMapper.C
	surfaceProcessor.C
	triangle.C
	triangleEdge.C
	trianglePoint.C
	triangulatedSAS.C
	triangulatedSES.C
	triangulatedSurface.C
	UCK.C
	residueRotamerSet.C
	RMSDMinimizer.C
)

IF(BALL_HAS_OPENBABEL)
	LIST(APPEND SOURCES_LIST logP.C)
ENDIF()

ADD_BALL_SOURCES("STRUCTURE" "${SOURCES_LIST}")

ADD_BALL_PARSER_LEXER("STRUCTURE" "smilesParser" "SmilesParser")
ADD_BALL_PARSER_LEXER("STRUCTURE" "smartsParser" "SmartsParser")

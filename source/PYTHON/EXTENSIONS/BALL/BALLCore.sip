/////////////////////////
// BALL PYTHON SUPPORT //
/////////////////////////

%Module(name=BALLCore, all_raise_py_exception=True)

%ModuleHeaderCode
	#include <BALLPyMacros.h>
%End

%UnitCode
	namespace BALL{}
	using namespace BALL;
%End

%Copying
	BALL is made available under the Lesser GNU Public License (LGPL 2.1).
	See LICENSE for details.
%End

// STL
%Include deque.sip
%Include list.sip
%Include vector.sip
%Include pair.sip
%Include iterators.sip

// COMMON
%Include constants.sip
%Include exception.sip
%Include global.sip
%Include logStream.sip
%Include version.sip

// CONCEPT
%Include composite.sip
%Include embeddable.sip
%Include object.sip
%Include molecularInformation.sip
%Include persistentObject.sip
%Include predicateConcept.sip
%Include processor.sip
%Include property.sip
%Include selectable.sip
%Include timeStamp.sip

// DATATYPE
%Include bitVector.sip
%Include options.sip
%Include regularData1D.sip
%Include regularData2D.sip
%Include regularData3D.sip
%Include regularExpression.sip
%Include string.sip
%Include stringHashMap.sip

// ENERGY
%Include atomicContactEnergy.sip
%Include coulomb.sip
%Include distanceCoulomb.sip

// FORMAT
%Include DCDFile.sip
%Include HINFile.sip
%Include HMOFile.sip
%Include INIFile.sip
%Include KCFFile.sip
%Include MOL2File.sip
%Include MOLFile.sip
%Include NMRStarFile.sip
%Include PDBFile.sip
%Include PDBInfo.sip
%Include PDBRecords.sip
%Include PDBdefs.sip
%Include SDFile.sip
%Include TRRFile.sip
%Include XYZFile.sip
%Include genericMolFile.sip
%Include lineBasedFile.sip
%Include molFileFactory.sip
%Include parameterSection.sip
%Include parameters.sip
%Include resourceFile.sip
%Include trajectoryFile.sip

// KERNEL
%Include atom.sip
%Include atomContainer.sip
%Include PDBAtom.sip
%Include PTE_.sip
%Include bond.sip
%Include chain.sip
%Include expression.sip
%Include fragment.sip
%Include molecule.sip
%Include nucleicAcid.sip
%Include nucleotide.sip
%Include protein.sip
%Include residue.sip
%Include secondaryStructure.sip
%Include selector.sip
%Include system.sip

// MATHS
%Include analyticalGeometry.sip
%Include angle.sip
%Include box3.sip
%Include simpleBox3.sip
%Include circle3.sip
%Include line3.sip
%Include matrix44.sip
%Include plane3.sip
%Include quaternion.sip
%Include sphere3.sip
%Include surface.sip
%Include vector2.sip
%Include vector3.sip
%Include vector4.sip

// MOLMEC/SUPPORT
%Include molmec-support.sip

// MOLMEC/PARAMETER
%Include atomTypes.sip
%Include forceFieldParameters.sip
%Include cosineTorsion.sip

// MOLMEC/MMFF94
%Include MMFF94.sip

// MOLMEC/AMBER
%Include amber.sip
%Include amberBend.sip
%Include amberNonBonded.sip
%Include amberStretch.sip
%Include amberTorsion.sip

// MOLMEC/CHARMM
%Include charmm.sip
%Include charmmBend.sip
%Include charmmImproperTorsion.sip
%Include charmmNonBonded.sip
%Include charmmStretch.sip
%Include charmmTorsion.sip

// MOLMEC/COMMON
%Include assignTypes.sip
%Include atomVector.sip
%Include chargeRuleProcessor.sip
%Include forceField.sip
%Include forceFieldComponent.sip
%Include gradient.sip
%Include periodicBoundary.sip
%Include radiusRuleProcessor.sip
%Include ruleEvaluator.sip
%Include ruleProcessor.sip
%Include snapShot.sip
%Include snapShotManager.sip
%Include typeRuleProcessor.sip

// MOLMEC/MDSIMULATION
%Include canonicalMD.sip
%Include microCanonicalMD.sip
%Include molecularDynamics.sip

// MOLMEC/MINIMIZATION
%Include conjugateGradient.sip
%Include energyMinimizer.sip
%Include lineSearch.sip
%Include steepestDescent.sip
%Include strangLBFGS.sip
%Include shiftedLVMM.sip

// NMR
%Include shiftModule.sip

// PYTHON
%Include extractors.sip
%Include pyIndexList.sip // a list of numbers (indices etc.)
%Include pyAtomDict.sip  // a dictionary mapping to atoms
%Include pyUnaryProcessors.sip

// QSAR
%Include aromaticityProcessor.sip

// SOLVATION
%Include ooiEnergy.sip
%Include poissonBoltzmann.sip

// STRUCTURE
%Include addHydrogenProcessor.sip
%Include assignBondOrderProcessor.sip
%Include atomBijection.sip
%Include connectedComponentsProcessor.sip
%Include defaultProcessors.sip
%Include DNAMutator.sip
%Include fragmentDB.sip
%Include HBondProcessor.sip
%Include reconstructFragmentProcessor.sip
%Include geometricProperties.sip
%Include geometricTransformations.sip
%Include pyAtomHashGrid.sip
%Include peptides.sip
%Include numericalSAS.sip
%Include nucleotideMapping.sip
%Include residueChecker.sip
%Include RMSDMinimizer.sip
%Include structureMapper.sip
%Include peptideBuilder.sip
%Include peptideCapProcessor.sip
%Include secondaryStructureProcessor.sip
%Include surfaceProcessor.sip
%Include smilesParser.sip
%Include sdGenerator.sip

// SYSTEM
%Include file.sip
%Include path.sip
%Include sysinfo.sip
%Include timer.sip

// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

/////////////////////////
// BALL PYTHON SUPPORT
/////////////////////////
//
// module definition file
//

%Module(name=BALLCore, all_raise_py_exception=True)

%ModuleHeaderCode
#include <BALLPyMacros.h>
%End

%Feature BALL_HAS_ASIO

%Include copying.sip

%Include deque.sip
%Include list.sip
%Include vector.sip
%Include pair.sip
%Include iterators.sip

// COMMON
%Include constants.sip
// %Include create.sip
// debug.sip // not needed
%Include exception.sip
%Include global.sip
// %Include hash.sip // ??
// %Include limits.sip // ??
%Include logStream.sip
// %Include rtti.sip // ??
%Include version.sip 

// CONCEPT
// XDRPersistenceManager.h
// autoDeletable.h	// not needed
// baseFunctor.h		// not supported
// baseIterator.h   // not supported
// bidirectionalIterator.h // not supported
// classTest.h      // not needed
// comparator.h     // ??
%Include composite.sip
%Include embeddable.sip
// enumerator.h     // ??
// forwardIterator.h // not supported
// notification.h    // ??
%Include object.sip 
// %Include client.sip // we need to find a way to have this depend on BALL_HAS_ASIO
%Include molecularInformation.sip
// persitenceManager.h // ??
%Include persistentObject.sip
%Include predicateConcept.sip
%Include processor.sip
%Include property.sip
// randomAccessIterator.h // not supported
// reverseBidirectionalIterator.h // not supported
%Include selectable.sip
// singleton.h // not supported
// textPersistenceManager.h // ??
%Include timeStamp.sip
// visitor.h // not supported

// DATATYPE
// binarySearchTree.h // ??
%Include bitVector.sip 
// forEach.h // not supported -> extractors!
// %Include hashGrid.h // ?? too generic... Possibly create an Atom* grid from it?
// hashMap.h // -> dictionary
// hashSet.h // -> dictionary
// list.h // -> Python lists
// listStack.h // ?? 
%Include options.sip
%Include regularData1D.sip
%Include regularData2D.sip
%Include regularData3D.sip
// triple.h -- generic, no way...
// quadruple.h -- generic, no way...
%Include regularExpression.sip
%Include string.sip 
%Include stringHashMap.sip
// stringHashSet.h // -> dictionaries


// ENERGY
%Include atomicContactEnergy.sip
%Include coulomb.sip
%Include distanceCoulomb.sip


// FORMAT
%Include DCDFile.sip
%Include HINFile.sip
%Include HMOFile.sip
%Include INIFile.sip
// %Include JCAMPFile.sip
%Include KCFFile.sip
%Include MOL2File.sip
%Include MOLFile.sip
%Include NMRStarFile.sip
%Include PDBFile.sip
%Include PDBInfo.sip
%Include PDBRecords.sip
%Include PDBdefs.sip // only the most relevant enums, we do not expose the internals
%Include SDFile.sip
%Include TRRFile.sip
%Include XYZFile.sip
// %Include bruker1DFile.sip
// %Include bruker2DFile.sip
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
// atomIterator.h // not needed
%Include PDBAtom.sip
// PDBAtomIterator.h // not needed
%Include PTE_.sip
%Include bond.sip 
// bondIterator.h // not needed
%Include chain.sip
// chainIterator.h // not needed
%Include expression.sip 
// forEach.h // not needed
%Include fragment.sip
// fragmentIterator.h // not needed
// global.h // not needed
// iterator.h // not needed
%Include molecule.sip
// moleculeIterator.h // not needed
%Include nucleicAcid.sip
// nucleicAcidIterator.h // not needed
%Include nucleotide.sip
// nucleotideIterator.h // not needed
// %Include predicate.sip
%Include protein.sip
// proteinIterator.h // not needed
%Include residue.sip
// residueIterator.h // not needed
%Include secondaryStructure.sip
// secondaryStructureIterator.h // not needed
%Include selector.sip
// %Include standardPredicates.sip // not needed - use Selector class!
%Include system.sip

// MATHS
%Include analyticalGeometry.sip
%Include angle.sip
%Include box3.sip
%Include simpleBox3.sip
%Include circle3.sip
// %Include common.sip - trouble! namespaces...
// %Include function.sip - template stuff only...
%Include line3.sip
%Include matrix44.sip
// %Include numericalIntegrator.sip
// %Include piecewiseFunction.sip
// %Include piecewisePolynomial.sip
// %Include rombergIntegrator.sip
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
// assignTypes.h // ??
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
// support.h
%Include typeRuleProcessor.sip
%Include typeNameRuleProcessor.sip

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

// MOLMEC/PARAMETER
// %Include atomTypes.sip
// %Include charmmEEF1.sip
// %Include cosineTorsion.sip
// %Include forceFieldParameters.sip
// %Include lennardJones.sip
// %Include potential1210.sip
// %Include quadraticAngleBend.sip
// %Include quadraticBondStretch.sip
// %Include quadraticImproperTorsion.sip
// %Include residueTorsions.sip
// %Include templates.sip


// NMR
%Include shiftModule.sip
// %Include johnsonBovey.sip // ??

// PYTHON
%Include extractors.sip
%Include pyIndexList.sip // a list of numbers (indices etc.)
%Include pyAtomDict.sip // a dictionary mapping to atoms
%Include pyAtomProcessor.sip 
%Include pyBondProcessor.sip
%Include pyFragmentProcessor.sip
%Include pyResidueProcessor.sip
%Include pyCompositeProcessor.sip
%Include pyChainProcessor.sip
//%Include pyLists.sip

// QSAR
//%Include descriptor.sip
//%Include simpleDescriptors.sip

// SOLVATION
// %Include molecularSurfaceGrid.sip // ??
%Include ooiEnergy.sip
%Include poissonBoltzmann.sip

// STRUCTURE
// %Include analyticalSES.sip
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
// %Include rotamerLibrary.sip
// %Include rules.sip
%Include structureMapper.sip
%Include peptideBuilder.sip
%Include peptideCapProcessor.sip
%Include secondaryStructureProcessor.sip
%Include surfaceProcessor.sip
%Include smilesParser.sip
%Include sdGenerator.sip

// SYSTEM
// %Include directory.sip
%Include file.sip
// %Include fileSystem.sip
%Include path.sip
// %Include socket.sip
%Include sysinfo.sip
%Include timer.sip


// $Id: BALL.sip,v 1.13 2000/07/18 08:29:59 oliver Exp $
/////////////////////////
// BALL PYTHON SUPPORT
/////////////////////////
//
// module definition file
//

%Module BALL
// COMMON
%Include constants.sip
// %Include create.sip
// debug.sip // not needed
// %Include exception.sip
%Include global.sip
// %Include hash.sip // ??
// %Include limits.sip // ??
// %Include logStream.sip // ??
// %Include rtti.sip // ??
%Include version.sip 

// CONCEPT
// XDRPersistenceManager.h
// autoDeletable.h	// not needed
// baseFunctor.h		// not supported
// baseIterator.h   // not supported
// bidirectionalIterator.h // not supported
// callback.h       // not supported
// classTest.h      // not needed
// comparator.h     // ??
%Include composite.sip
%Include embeddable.sip
// enumerator.h     // ??
// forwardIterator.h // not supported
// notification.h    // ??
%Include object.sip 
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
// visitor.h // not supported

// DATATYPE
// binarySearchTree.h // ??
%Include bitVector.sip 
// forEach.h // not supported -> extractors!
// hashGrid.h // ??
// hashMap.h // ?? ->pyAtomDict
// hashSet.h // ??
// list.h // ??
// listStack.h // ?? 
%Include options.sip
%Include pointGrid.sip //  float only!
// quadruple.h // ??
// redBlackTree.h // ??
%Include regExp.sip
// splayTree.h
%Include string.sip
// stringHashMap.h // ??
// stringHashSet.h // ??


// ENERGY
%Include atomicContactEnergy.sip
%Include coulomb.sip
%Include distanceCoulomb.sip


// FORMAT
%Include HINFile.sip
%Include INIFile.sip
%Include MOL2File.sip
%Include PDBFile.sip
%Include XYZFile.sip
// genericPDBFile.h // not supported
// %Include parameterSection.sip
// %Include parameters.sip
%Include resourceFile.sip


// KERNEL
%Include PDBAtom.sip
// PDBAtomIterator.h // not needed
%Include PTE.sip
%Include atom.sip
// atomIterator.h // not needed
%Include bond.sip 
// bondIterator.h // not needed
%Include baseFragment.sip
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
// universe.h  // not supported

// MATHS
%Include analyticalGeometry.sip
%Include angle.sip
%Include box3.sip
%Include circle3.sip
// %Include common.sip - trouble! namespaces...
%Include line3.sip
%Include matrix44.sip
%Include plane3.sip
%Include quaternion.sip
%Include sphere3.sip
%Include surface.sip
%Include vector3.sip
%Include vector4.sip

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
// chargeRuleProcessor.h // ??
%Include forceField.sip
%Include forceFieldComponent.sip
%Include gradient.sip
%Include periodicBoundary.sip
// radiusRuleProcessor.h // ??
// ruleEvaluator.h // ??
// ruleProcessor.h // ??
%Include snapShot.sip
// support.h
// typeRuleProcessor.h
// typenameRuleProcessor.h

// MOLMEC/MDSIMULATION
%Include canonicalMD.sip
%Include microCanonicalMD.sip
%Include molecularDynamics.sip

// MOLMEC/MINIMIZATION
%Include conjugateGradient.sip
%Include energyMinimizer.sip
// %Include lineSearch.sip
// %Include steepestDescent.sip

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


// MOLVIEW ??


// NMR
// %Include johnsonBovey.sip // ??




// PYTHON
%Include extractors.sip
%Include pyAtomDict.sip
%Include pyAtomList.sip
%Include pyAtomProcessor.sip
%Include pyCompositeDescriptorList.sip
%Include pyCompositeProcessor.sip
%Include pyBondList.sip
%Include pyBaseFragmentList.sip
%Include pyFragmentList.sip
%Include pyMoleculeList.sip
%Include pyPDBAtomList.sip
%Include pyResidueList.sip
%Include pySecondaryStructureList.sip
%Include pyChainList.sip
%Include pyProteinList.sip
%Include pyNucleotideList.sip
%Include pyNucleicAcidList.sip


// SOLVATION
// %Include molecularSurfaceGrid.sip // ??
// %Include ooiEnergy.sip
// %Include poissonBoltzmann.sip

// STRUCTURE
// %Include analyticalSES.sip
%Include defaultProcessors.sip
// %Include fragmentDB.sip
%Include geometricProperties.sip
%Include geometricTransformations.sip
%Include numericalSAS.sip 
// %Include residueChecker.sip // ??
// %Include rotamerLibrary.sip
// %Include rules.sip
// %Include structureMapper.sip


// SYSTEM
// %Include directory.sip
%Include file.sip
// %Include fileSystem.sip
// %Include path.sip
// %Include socket.sip
// %Include timer.sip

// VIEW/OPENGL/KERNEL
%Include mainControl.sip
%Include compositeDescriptor.sip
// VIEW/OPENGL/WIDGETS
%Include pyWidget.sip

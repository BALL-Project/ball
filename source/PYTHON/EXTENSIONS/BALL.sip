// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: BALL.sip,v 1.40 2005/02/11 15:27:13 oliver Exp $
//
//
//

/////////////////////////
// BALL PYTHON SUPPORT
/////////////////////////
//
// module definition file
//

%Module BALL

%Include versions.sip
%Include copying.sip

// COMMON
%Include constants.sip
// %Include create.sip
// debug.sip // not needed
%Include exception.sip
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
%Include timeStamp.sip
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
%Include regularData1D.sip
%Include regularData2D.sip
%Include regularData3D.sip
// triple.h -- generic, no way...
// quadruple.h -- generic, no way...
%Include regularExpression.sip
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
%Include MOLFile.sip
%Include SDFile.sip
%Include lineBasedFile.sip
%Include genericMolFile.sip
%Include PDBFile.sip
%Include XYZFile.sip
// genericPDBFile.h // not supported
// %Include parameterSection.sip
// %Include parameters.sip
%Include resourceFile.sip
%Include trajectoryFile.sip
%Include DCDFile.sip


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


// NMR
// %Include johnsonBovey.sip // ??




// PYTHON
%Include extractors.sip
%Include pyAtomDict.sip
%Include pyAtomList.sip
%Include pyAtomProcessor.sip
%Include pyFragmentProcessor.sip
%Include pyResidueProcessor.sip
%Include pyCompositeProcessor.sip
%Include pyBondList.sip
%Include pyCompositeList.sip
%Include pyAtomContainerList.sip
%Include pyFragmentList.sip
%Include pyMoleculeList.sip
%Include pyPDBAtomList.sip
%Include pyResidueList.sip
%Include pySecondaryStructureList.sip
%Include pyChainList.sip
%Include pyProteinList.sip
%Include pyNucleotideList.sip
%Include pyNucleicAcidList.sip

// QSAR
//%Include descriptor.sip
//%Include simpleDescriptors.sip

// SOLVATION
// %Include molecularSurfaceGrid.sip // ??
// %Include ooiEnergy.sip
%Include poissonBoltzmann.sip

// STRUCTURE
// %Include analyticalSES.sip
%Include defaultProcessors.sip
%Include fragmentDB.sip
%Include reconstructFragmentProcessor.sip
%Include geometricProperties.sip
%Include geometricTransformations.sip
%Include numericalSAS.sip 
%Include residueChecker.sip
// %Include rotamerLibrary.sip
// %Include rules.sip
// %Include structureMapper.sip
//%Include smilesParser.sip
%Include secondaryStructureProcessor.sip
%Include surfaceProcessor.sip

// SYSTEM
// %Include directory.sip
%Include file.sip
// %Include fileSystem.sip
// %Include path.sip
// %Include socket.sip
%Include timer.sip

// VIEW/KERNEL
%Include message.sip
%Include stage.sip
%Include mainControl.sip
%Include compositeManager.sip
%Include primitiveManager.sip
%Include representationList.sip
%Include client.sip
%Include representation.sip
%Include geometricObject.sip
%Include geometricObjectList.sip
%Include view-common.sip

// VIEW/DATATYPE
%Include colorUnit.sip
%Include colorRGBA.sip
%Include vertex1.sip
%Include vertex2.sip
%Include colorExtension2.sip

// VIEW/MODELS
%Include colorProcessor.sip
%Include standardColorProcessor.sip
%Include modelProcessor.sip
%Include atomBondModelBaseProcessor.sip
%Include lineModel.sip
%Include vanDerWaalsModel.sip

%Include backboneModel.sip
%Include ballAndStickModel.sip
%Include cartoonModel.sip
%Include HBondModel.sip
%Include surfaceModel.sip

// VIEW/PRIMITIVES
%Include box.sip
%Include disc.sip
%Include label.sip
%Include line.sip
%Include mesh.sip
%Include point.sip
%Include simpleBox.sip
%Include sphere.sip
%Include twoColoredLine.sip
%Include twoColoredTube.sip
%Include tube.sip

// VIEW/DIALOGS
%Include preferences.sip
%Include displayProperties.sip
%Include molecularFileDialog.sip

// VIEW/RENDERER
%Include renderer.sip
%Include POVRenderer.sip

// VIEW/WIDGETS
%Include modularWidget.sip
%Include pyWidget.sip
%Include scene.sip
%Include molecularStructure.sip
%Include molecularControl.sip

// Makefile
//%Include Makefile.sip

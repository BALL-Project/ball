#include "sipBALLDeclBALL.h"

#include "sipBALLConstant.h"
#include "sipBALLIndex.h"
#include "sipBALLPosition.h"
#include "sipBALLVersionInfo.h"
#include "sipBALLComposite.h"
#include "sipBALLEmbeddable.h"
#include "sipBALLObject.h"
#include "sipBALLPersistentObject.h"
#include "sipBALLUnaryCompositePredicate.h"
#include "sipBALLUnaryAtomPredicate.h"
#include "sipBALLProcessor.h"
#include "sipBALLNamedProperty.h"
#include "sipBALLPropertyManager.h"
#include "sipBALLSelectable.h"
#include "sipBALLBit.h"
#include "sipBALLBitVector.h"
#include "sipBALLOptions.h"
#include "sipBALLGridIndex.h"
#include "sipBALLFloatPointGrid.h"
#include "sipBALLRegularExpression.h"
#include "sipBALLSubstring.h"
#include "sipBALLString.h"
#include "sipBALLHINFile.h"
#include "sipBALLINIFile.h"
#include "sipBALLMOL2File.h"
#include "sipBALLPDBFile.h"
#include "sipBALLXYZFile.h"
#include "sipBALLResourceEntry.h"
#include "sipBALLResourceFile.h"
#include "sipBALLPDBAtom.h"
#include "sipBALLElement.h"
#include "sipBALLPTE_.h"
#include "sipBALLAtom.h"
#include "sipBALLAtomType.h"
#include "sipBALLBond.h"
#include "sipBALLBaseFragment.h"
#include "sipBALLChain.h"
#include "sipBALLExpressionPredicate.h"
#include "sipBALLExpression.h"
#include "sipBALLFragment.h"
#include "sipBALLMolecule.h"
#include "sipBALLNucleicAcid.h"
#include "sipBALLNucleotide.h"
#include "sipBALLProtein.h"
#include "sipBALLResidue.h"
#include "sipBALLSecondaryStructure.h"
#include "sipBALLSelector.h"
#include "sipBALLSystem.h"
#include "sipBALLAngle.h"
#include "sipBALLBox3.h"
#include "sipBALLCircle3.h"
#include "sipBALLLine3.h"
#include "sipBALLMatrix4x4.h"
#include "sipBALLPlane3.h"
#include "sipBALLQuaternion.h"
#include "sipBALLSphere3.h"
#include "sipBALLSurface.h"
#include "sipBALLVector3.h"
#include "sipBALLVector4.h"
#include "sipBALLAmberFF.h"
#include "sipBALLAmberBend.h"
#include "sipBALLAmberNonBonded.h"
#include "sipBALLAmberStretch.h"
#include "sipBALLAmberTorsion.h"
#include "sipBALLCharmmFF.h"
#include "sipBALLCharmmBend.h"
#include "sipBALLCharmmImproperTorsion.h"
#include "sipBALLCharmmNonBonded.h"
#include "sipBALLCharmmStretch.h"
#include "sipBALLCharmmTorsion.h"
#include "sipBALLAtomVector.h"
#include "sipBALLForceField.h"
#include "sipBALLForceFieldComponent.h"
#include "sipBALLGradient.h"
#include "sipBALLPeriodicBoundary.h"
#include "sipBALLSnapShotManager.h"
#include "sipBALLCanonicalMD.h"
#include "sipBALLMicroCanonicalMD.h"
#include "sipBALLMolecularDynamics.h"
#include "sipBALLConjugateGradientMinimizer.h"
#include "sipBALLEnergyMinimizer.h"
#include "sipBALLPyAtomDict.h"
#include "sipBALLPyAtomList.h"
#include "sipBALLAtomProcessor.h"
#include "sipBALLPyCompositeDescriptorList.h"
#include "sipBALLCompositeProcessor.h"
#include "sipBALLPyBondList.h"
#include "sipBALLPyBaseFragmentList.h"
#include "sipBALLPyFragmentList.h"
#include "sipBALLPyMoleculeList.h"
#include "sipBALLPyPDBAtomList.h"
#include "sipBALLPyResidueList.h"
#include "sipBALLPySecondaryStructureList.h"
#include "sipBALLPyChainList.h"
#include "sipBALLPyProteinList.h"
#include "sipBALLPyNucleotideList.h"
#include "sipBALLPyNucleicAcidList.h"
#include "sipBALLClearChargeProcessor.h"
#include "sipBALLClearRadiusProcessor.h"
#include "sipBALLAssignRadiusProcessor.h"
#include "sipBALLAssignChargeProcessor.h"
#include "sipBALLBoundingBoxProcessor.h"
#include "sipBALLGeometricCenterProcessor.h"
#include "sipBALLFragmentDistanceCollector.h"
#include "sipBALLTranslationProcessor.h"
#include "sipBALLTransformationProcessor.h"
#include "sipBALLFile.h"
#include "sipBALLOpenMode.h"
#include "sipBALLMainControl.h"
#include "sipBALLCompositeDescriptor.h"
#include "sipBALLModularWidget.h"
#include "sipBALLPyWidget.h"

char sipName_BALL_cursorRight[] = "cursorRight";
char sipName_BALL_backspace[] = "backspace";
char sipName_BALL_cursorLeft[] = "cursorLeft";
char sipName_BALL_newLine[] = "newLine";
char sipName_BALL_cursorDown[] = "cursorDown";
char sipName_BALL_cursorUp[] = "cursorUp";
char sipName_BALL_stopInterpreter[] = "stopInterpreter";
char sipName_BALL_startInterpreter[] = "startInterpreter";
char sipName_BALL_PyWidget[] = "PyWidget";
char sipName_BALL_checkMenu[] = "checkMenu";
char sipName_BALL_finalizeWidget[] = "finalizeWidget";
char sipName_BALL_initializeWidget[] = "initializeWidget";
char sipName_BALL_registerWidget[] = "registerWidget";
char sipName_BALL_getIdentifier[] = "getIdentifier";
char sipName_BALL_setIdentifier[] = "setIdentifier";
char sipName_BALL_ModularWidget[] = "ModularWidget";
char sipName_BALL_isDeepCopy[] = "isDeepCopy";
char sipName_BALL_isShallowCopy[] = "isShallowCopy";
char sipName_BALL_drawDirect[] = "drawDirect";
char sipName_BALL_drawEntity[] = "drawEntity";
char sipName_BALL_getQuaternion[] = "getQuaternion";
char sipName_BALL_setQuaternion[] = "setQuaternion";
char sipName_BALL_CompositeDescriptor[] = "CompositeDescriptor";
char sipName_BALL_getInstance[] = "getInstance";
char sipName_BALL_countInstances[] = "countInstances";
char sipName_BALL_isInserted[] = "isInserted";
char sipName_BALL_getOptions[] = "getOptions";
char sipName_BALL_updateAll[] = "updateAll";
char sipName_BALL_getBoundingBox[] = "getBoundingBox";
char sipName_BALL_setBoundingBox[] = "setBoundingBox";
char sipName_BALL_setCenter[] = "setCenter";
char sipName_BALL_getDescriptorList[] = "getDescriptorList";
char sipName_BALL_getDescriptor[] = "getDescriptor";
char sipName_BALL_unkown[] = "unkown";
char sipName_BALL_MainControl[] = "MainControl";
char sipName_BALL_OpenMode[] = "OpenMode";
char sipName_BALL_isExecutable[] = "isExecutable";
char sipName_BALL_isWritable[] = "isWritable";
char sipName_BALL_isReadable[] = "isReadable";
char sipName_BALL_isCanonized[] = "isCanonized";
char sipName_BALL_isAccessible[] = "isAccessible";
char sipName_BALL_isClosed[] = "isClosed";
char sipName_BALL_isOpen[] = "isOpen";
char sipName_BALL_renameTo[] = "renameTo";
char sipName_BALL_copyTo[] = "copyTo";
char sipName_BALL_getOpenMode[] = "getOpenMode";
char sipName_BALL_reopen[] = "reopen";
char sipName_BALL_TRUNC[] = "TRUNC";
char sipName_BALL_ATE[] = "ATE";
char sipName_BALL_BINARY[] = "BINARY";
char sipName_BALL_APP[] = "APP";
char sipName_BALL_OUT[] = "OUT";
char sipName_BALL_IN[] = "IN";
char sipName_BALL_calculateSASPoints[] = "calculateSASPoints";
char sipName_BALL_calculateSASAtomAreas[] = "calculateSASAtomAreas";
char sipName_BALL_calculateSASVolume[] = "calculateSASVolume";
char sipName_BALL_calculateSASArea[] = "calculateSASArea";
char sipName_BALL_getTransformation[] = "getTransformation";
char sipName_BALL_setTransformation[] = "setTransformation";
char sipName_BALL_TransformationProcessor[] = "TransformationProcessor";
char sipName_BALL_getTranslation[] = "getTranslation";
char sipName_BALL_TranslationProcessor[] = "TranslationProcessor";
char sipName_BALL_calculateBondAngle[] = "calculateBondAngle";
char sipName_BALL_calculateTorsionAngle[] = "calculateTorsionAngle";
char sipName_BALL_setDistance[] = "setDistance";
char sipName_BALL_getComposite[] = "getComposite";
char sipName_BALL_setComposite[] = "setComposite";
char sipName_BALL_getNumberOfFragments[] = "getNumberOfFragments";
char sipName_BALL_FragmentDistanceCollector[] = "FragmentDistanceCollector";
char sipName_BALL_getCenter[] = "getCenter";
char sipName_BALL_GeometricCenterProcessor[] = "GeometricCenterProcessor";
char sipName_BALL_getUpper[] = "getUpper";
char sipName_BALL_getLower[] = "getLower";
char sipName_BALL_BoundingBoxProcessor[] = "BoundingBoxProcessor";
char sipName_BALL_getTotalCharge[] = "getTotalCharge";
char sipName_BALL_AssignChargeProcessor[] = "AssignChargeProcessor";
char sipName_BALL_getNumberOfErrors[] = "getNumberOfErrors";
char sipName_BALL_getNumberOfAssignments[] = "getNumberOfAssignments";
char sipName_BALL_AssignRadiusProcessor[] = "AssignRadiusProcessor";
char sipName_BALL_ClearRadiusProcessor[] = "ClearRadiusProcessor";
char sipName_BALL_ClearChargeProcessor[] = "ClearChargeProcessor";
char sipName_BALL_PyNucleicAcidList[] = "PyNucleicAcidList";
char sipName_BALL_PyNucleotideList[] = "PyNucleotideList";
char sipName_BALL_PyProteinList[] = "PyProteinList";
char sipName_BALL_PyChainList[] = "PyChainList";
char sipName_BALL_PySecondaryStructureList[] = "PySecondaryStructureList";
char sipName_BALL_PyResidueList[] = "PyResidueList";
char sipName_BALL_PyPDBAtomList[] = "PyPDBAtomList";
char sipName_BALL_PyMoleculeList[] = "PyMoleculeList";
char sipName_BALL_PyFragmentList[] = "PyFragmentList";
char sipName_BALL_PyBaseFragmentList[] = "PyBaseFragmentList";
char sipName_BALL_PyBondList[] = "PyBondList";
char sipName_BALL_PyCompositeDescriptorList[] = "PyCompositeDescriptorList";
char sipName_BALL_finish[] = "finish";
char sipName_BALL_AtomProcessor[] = "AtomProcessor";
char sipName_BALL_PyAtomList[] = "PyAtomList";
char sipName_BALL_PyAtomDict[] = "PyAtomDict";
char sipName_BALL_nucleicAcids[] = "nucleicAcids";
char sipName_BALL_nucleotides[] = "nucleotides";
char sipName_BALL_proteins[] = "proteins";
char sipName_BALL_chains[] = "chains";
char sipName_BALL_secondaryStructures[] = "secondaryStructures";
char sipName_BALL_residues[] = "residues";
char sipName_BALL_molecules[] = "molecules";
char sipName_BALL_fragments[] = "fragments";
char sipName_BALL_baseFragments[] = "baseFragments";
char sipName_BALL_bonds[] = "bonds";
char sipName_BALL_PDBAtoms[] = "PDBAtoms";
char sipName_BALL_atoms[] = "atoms";
char sipName_BALL_getMaximalShift[] = "getMaximalShift";
char sipName_BALL_setMaximalShift[] = "setMaximalShift";
char sipName_BALL_getMaxGradient[] = "getMaxGradient";
char sipName_BALL_setMaxGradient[] = "setMaxGradient";
char sipName_BALL_getEnergyDifferenceBound[] = "getEnergyDifferenceBound";
char sipName_BALL_setEnergyDifferenceBound[] = "setEnergyDifferenceBound";
char sipName_BALL_getMaxSameEnergy[] = "getMaxSameEnergy";
char sipName_BALL_setMaxSameEnergy[] = "setMaxSameEnergy";
char sipName_BALL_getInitialEnergy[] = "getInitialEnergy";
char sipName_BALL_getInitialGradient[] = "getInitialGradient";
char sipName_BALL_getGradient[] = "getGradient";
char sipName_BALL_getDirection[] = "getDirection";
char sipName_BALL_finishIteration[] = "finishIteration";
char sipName_BALL_printEnergy[] = "printEnergy";
char sipName_BALL_isConverged[] = "isConverged";
char sipName_BALL_minimize[] = "minimize";
char sipName_BALL_updateDirection[] = "updateDirection";
char sipName_BALL_findStep[] = "findStep";
char sipName_BALL_getStepLength[] = "getStepLength";
char sipName_BALL_setStepLength[] = "setStepLength";
char sipName_BALL_EnergyMinimizer[] = "EnergyMinimizer";
char sipName_BALL_ConjugateGradientMinimizer[] = "ConjugateGradientMinimizer";
char sipName_BALL_getKineticEnergy[] = "getKineticEnergy";
char sipName_BALL_getPotentialEnergy[] = "getPotentialEnergy";
char sipName_BALL_getTotalEnergy[] = "getTotalEnergy";
char sipName_BALL_getTime[] = "getTime";
char sipName_BALL_getSnapShotFrequency[] = "getSnapShotFrequency";
char sipName_BALL_getTimeStep[] = "getTimeStep";
char sipName_BALL_getMaximalSimulationTime[] = "getMaximalSimulationTime";
char sipName_BALL_getMaximalNumberOfIterations[] = "getMaximalNumberOfIterations";
char sipName_BALL_getNumberOfIteration[] = "getNumberOfIteration";
char sipName_BALL_getEnergyOutputFrequency[] = "getEnergyOutputFrequency";
char sipName_BALL_setSnapShotFrequency[] = "setSnapShotFrequency";
char sipName_BALL_setEnergyOutputFrequency[] = "setEnergyOutputFrequency";
char sipName_BALL_setCurrentTime[] = "setCurrentTime";
char sipName_BALL_setReferenceTemperature[] = "setReferenceTemperature";
char sipName_BALL_setMaximalSimulationTime[] = "setMaximalSimulationTime";
char sipName_BALL_setMaximalNumberOfIterations[] = "setMaximalNumberOfIterations";
char sipName_BALL_setNumberOfIteration[] = "setNumberOfIteration";
char sipName_BALL_MicroCanonicalMD[] = "MicroCanonicalMD";
char sipName_BALL_simulateTime[] = "simulateTime";
char sipName_BALL_simulateIterations[] = "simulateIterations";
char sipName_BALL_simulate[] = "simulate";
char sipName_BALL_setTimeStep[] = "setTimeStep";
char sipName_BALL_getBathRelaxationTime[] = "getBathRelaxationTime";
char sipName_BALL_setBathRelaxationTime[] = "setBathRelaxationTime";
char sipName_BALL_MolecularDynamics[] = "MolecularDynamics";
char sipName_BALL_CanonicalMD[] = "CanonicalMD";
char sipName_BALL_getSnapShotAsSystem[] = "getSnapShotAsSystem";
char sipName_BALL_getNumberOfSnapShots[] = "getNumberOfSnapShots";
char sipName_BALL_flushToDisk[] = "flushToDisk";
char sipName_BALL_takeSnapShot[] = "takeSnapShot";
char sipName_BALL_getFlushToDiskFrequency[] = "getFlushToDiskFrequency";
char sipName_BALL_setFlushToDiskFrequency[] = "setFlushToDiskFrequency";
char sipName_BALL_SnapShotManager[] = "SnapShotManager";
char sipName_BALL_updateMolecules[] = "updateMolecules";
char sipName_BALL_isEnabled[] = "isEnabled";
char sipName_BALL_removeSolvent[] = "removeSolvent";
char sipName_BALL_addSolvent[] = "addSolvent";
char sipName_BALL_setBox[] = "setBox";
char sipName_BALL_getBox[] = "getBox";
char sipName_BALL_disable[] = "disable";
char sipName_BALL_enable[] = "enable";
char sipName_BALL_generateMoleculesVector[] = "generateMoleculesVector";
char sipName_BALL_PeriodicBoundary[] = "PeriodicBoundary";
char sipName_BALL_rms[] = "rms";
char sipName_BALL_inv_norm[] = "inv_norm";
char sipName_BALL_norm[] = "norm";
char sipName_BALL_invalidate[] = "invalidate";
char sipName_BALL_Gradient[] = "Gradient";
char sipName_BALL_setForceField[] = "setForceField";
char sipName_BALL_getForceField[] = "getForceField";
char sipName_BALL_periodic_boundary[] = "periodic_boundary";
char sipName_BALL_options[] = "options";
char sipName_BALL_update[] = "update";
char sipName_BALL_getUpdateFrequency[] = "getUpdateFrequency";
char sipName_BALL_getRMSGradient[] = "getRMSGradient";
char sipName_BALL_getEnergy[] = "getEnergy";
char sipName_BALL_getComponent[] = "getComponent";
char sipName_BALL_removeComponent[] = "removeComponent";
char sipName_BALL_insertComponent[] = "insertComponent";
char sipName_BALL_countComponents[] = "countComponents";
char sipName_BALL_setUseSelection[] = "setUseSelection";
char sipName_BALL_getUseSelection[] = "getUseSelection";
char sipName_BALL_getAtoms[] = "getAtoms";
char sipName_BALL_getNumberOfMovableAtoms[] = "getNumberOfMovableAtoms";
char sipName_BALL_getNumberOfAtoms[] = "getNumberOfAtoms";
char sipName_BALL_resize[] = "resize";
char sipName_BALL_push_back[] = "push_back";
char sipName_BALL_moveTo[] = "moveTo";
char sipName_BALL_resetPositions[] = "resetPositions";
char sipName_BALL_savePositions[] = "savePositions";
char sipName_BALL_AtomVector[] = "AtomVector";
char sipName_BALL_CharmmTorsion[] = "CharmmTorsion";
char sipName_BALL_CharmmStretch[] = "CharmmStretch";
char sipName_BALL_CharmmNonBonded[] = "CharmmNonBonded";
char sipName_BALL_CharmmImproperTorsion[] = "CharmmImproperTorsion";
char sipName_BALL_CharmmBend[] = "CharmmBend";
char sipName_BALL_getSolvationEnergy[] = "getSolvationEnergy";
char sipName_BALL_getProperTorsionEnergy[] = "getProperTorsionEnergy";
char sipName_BALL_getImproperTorsionEnergy[] = "getImproperTorsionEnergy";
char sipName_BALL_CharmmFF[] = "CharmmFF";
char sipName_BALL_AmberTorsion[] = "AmberTorsion";
char sipName_BALL_AmberStretch[] = "AmberStretch";
char sipName_BALL_getVdwEnergy[] = "getVdwEnergy";
char sipName_BALL_getElectrostaticEnergy[] = "getElectrostaticEnergy";
char sipName_BALL_AmberNonBonded[] = "AmberNonBonded";
char sipName_BALL_updateForces[] = "updateForces";
char sipName_BALL_updateEnergy[] = "updateEnergy";
char sipName_BALL_setup[] = "setup";
char sipName_BALL_ForceFieldComponent[] = "ForceFieldComponent";
char sipName_BALL_AmberBend[] = "AmberBend";
char sipName_BALL_hasInitializedParameters[] = "hasInitializedParameters";
char sipName_BALL_getVdWEnergy[] = "getVdWEnergy";
char sipName_BALL_getESEnergy[] = "getESEnergy";
char sipName_BALL_getNonbondedEnergy[] = "getNonbondedEnergy";
char sipName_BALL_getTorsionEnergy[] = "getTorsionEnergy";
char sipName_BALL_getBendEnergy[] = "getBendEnergy";
char sipName_BALL_getStretchEnergy[] = "getStretchEnergy";
char sipName_BALL_specificSetup[] = "specificSetup";
char sipName_BALL_ForceField[] = "ForceField";
char sipName_BALL_AmberFF[] = "AmberFF";
char sipName_BALL_Vector4[] = "Vector4";
char sipName_BALL_isOrthogonalTo[] = "isOrthogonalTo";
char sipName_BALL_isZero[] = "isZero";
char sipName_BALL_getTripleProduct[] = "getTripleProduct";
char sipName_BALL_getPerpendicularNormalization[] = "getPerpendicularNormalization";
char sipName_BALL_getOrthogonalProjection[] = "getOrthogonalProjection";
char sipName_BALL_getSquareDistance[] = "getSquareDistance";
char sipName_BALL_getDistance[] = "getDistance";
char sipName_BALL_Operator__mod__[] = "Operator__mod__";
char sipName_BALL_getUnit[] = "getUnit";
char sipName_BALL_getSquareLength[] = "getSquareLength";
char sipName_BALL_Vector3[] = "Vector3";
char sipName_BALL_getArea[] = "getArea";
char sipName_BALL_readMSMSFile[] = "readMSMSFile";
char sipName_BALL_Surface[] = "Surface";
char sipName_BALL_radius[] = "radius";
char sipName_BALL_Sphere3[] = "Sphere3";
char sipName_BALL_angle[] = "angle";
char sipName_BALL_j[] = "j";
char sipName_BALL_i[] = "i";
char sipName_BALL_getConjugate[] = "getConjugate";
char sipName_BALL_getInverse[] = "getInverse";
char sipName_BALL_getRotationMatrix[] = "getRotationMatrix";
char sipName_BALL_getAxis[] = "getAxis";
char sipName_BALL_getAngle[] = "getAngle";
char sipName_BALL_Quaternion[] = "Quaternion";
char sipName_BALL_hessify[] = "hessify";
char sipName_BALL_Plane3[] = "Plane3";
char sipName_BALL_m44[] = "m44";
char sipName_BALL_m43[] = "m43";
char sipName_BALL_m42[] = "m42";
char sipName_BALL_m41[] = "m41";
char sipName_BALL_m34[] = "m34";
char sipName_BALL_m33[] = "m33";
char sipName_BALL_m32[] = "m32";
char sipName_BALL_m31[] = "m31";
char sipName_BALL_m24[] = "m24";
char sipName_BALL_m23[] = "m23";
char sipName_BALL_m22[] = "m22";
char sipName_BALL_m21[] = "m21";
char sipName_BALL_m14[] = "m14";
char sipName_BALL_m13[] = "m13";
char sipName_BALL_m12[] = "m12";
char sipName_BALL_m11[] = "m11";
char sipName_BALL_isDiagonal[] = "isDiagonal";
char sipName_BALL_isUpperTriangular[] = "isUpperTriangular";
char sipName_BALL_isLowerTriangular[] = "isLowerTriangular";
char sipName_BALL_isSymmetric[] = "isSymmetric";
char sipName_BALL_isSingular[] = "isSingular";
char sipName_BALL_isRegular[] = "isRegular";
char sipName_BALL_isIdentity[] = "isIdentity";
char sipName_BALL_setRotation[] = "setRotation";
char sipName_BALL_rotate[] = "rotate";
char sipName_BALL_setRotationZ[] = "setRotationZ";
char sipName_BALL_rotateZ[] = "rotateZ";
char sipName_BALL_setRotationY[] = "setRotationY";
char sipName_BALL_rotateY[] = "rotateY";
char sipName_BALL_setRotationX[] = "setRotationX";
char sipName_BALL_rotateX[] = "rotateX";
char sipName_BALL_setScale[] = "setScale";
char sipName_BALL_scale[] = "scale";
char sipName_BALL_setTranslation[] = "setTranslation";
char sipName_BALL_translate[] = "translate";
char sipName_BALL_getDeterminant[] = "getDeterminant";
char sipName_BALL_invert[] = "invert";
char sipName_BALL_Operator__div__[] = "Operator__div__";
char sipName_BALL_Operator__mul__[] = "Operator__mul__";
char sipName_BALL_Operator__sub__[] = "Operator__sub__";
char sipName_BALL_Operator__neg__[] = "Operator__neg__";
char sipName_BALL_Operator__pos__[] = "Operator__pos__";
char sipName_BALL_getDiagonal[] = "getDiagonal";
char sipName_BALL_isEqual[] = "isEqual";
char sipName_BALL_setColumn[] = "setColumn";
char sipName_BALL_setRow[] = "setRow";
char sipName_BALL_getColumn[] = "getColumn";
char sipName_BALL_getRow[] = "getRow";
char sipName_BALL_transpose[] = "transpose";
char sipName_BALL_setIdentity[] = "setIdentity";
char sipName_BALL_getIdentity[] = "getIdentity";
char sipName_BALL_getZero[] = "getZero";
char sipName_BALL_getTrace[] = "getTrace";
char sipName_BALL_Matrix4x4[] = "Matrix4x4";
char sipName_BALL_d[] = "d";
char sipName_BALL_p[] = "p";
char sipName_BALL_normalize[] = "normalize";
char sipName_BALL_Line3[] = "Line3";
char sipName_BALL_Circle3[] = "Circle3";
char sipName_BALL_b[] = "b";
char sipName_BALL_a[] = "a";
char sipName_BALL_join[] = "join";
char sipName_BALL_getWidth[] = "getWidth";
char sipName_BALL_getVolume[] = "getVolume";
char sipName_BALL_getSurface[] = "getSurface";
char sipName_BALL_Box3[] = "Box3";
char sipName_BALL_isEquivalent[] = "isEquivalent";
char sipName_BALL_negate[] = "negate";
char sipName_BALL_toDegree[] = "toDegree";
char sipName_BALL_toRadian[] = "toRadian";
char sipName_BALL_Angle[] = "Angle";
char sipName_BALL_getTorsionAngle[] = "getTorsionAngle";
char sipName_BALL_isParallel[] = "isParallel";
char sipName_BALL_isIntersecting[] = "isIntersecting";
char sipName_BALL_isOrthogonal[] = "isOrthogonal";
char sipName_BALL_isComplanar[] = "isComplanar";
char sipName_BALL_isCollinear[] = "isCollinear";
char sipName_BALL_GetIntersection[] = "GetIntersection";
char sipName_BALL_GetAngle[] = "GetAngle";
char sipName_BALL_GetDistance[] = "GetDistance";
char sipName_BALL_countFragments[] = "countFragments";
char sipName_BALL_countMolecules[] = "countMolecules";
char sipName_BALL_System[] = "System";
char sipName_BALL_getNumberOfSelectedAtoms[] = "getNumberOfSelectedAtoms";
char sipName_BALL_start[] = "start";
char sipName_BALL_CompositeProcessor[] = "CompositeProcessor";
char sipName_BALL_Selector[] = "Selector";
char sipName_BALL_SecondaryStructure[] = "SecondaryStructure";
char sipName_BALL_isCTerminal[] = "isCTerminal";
char sipName_BALL_isNTerminal[] = "isNTerminal";
char sipName_BALL_isAminoAcid[] = "isAminoAcid";
char sipName_BALL_Residue[] = "Residue";
char sipName_BALL_countChains[] = "countChains";
char sipName_BALL_Protein[] = "Protein";
char sipName_BALL_is5Prime[] = "is5Prime";
char sipName_BALL_is3Prime[] = "is3Prime";
char sipName_BALL_isTerminal[] = "isTerminal";
char sipName_BALL_getInsertionCode[] = "getInsertionCode";
char sipName_BALL_setInsertionCode[] = "setInsertionCode";
char sipName_BALL_getNucleicAcid[] = "getNucleicAcid";
char sipName_BALL_Nucleotide[] = "Nucleotide";
char sipName_BALL_countNucleotides[] = "countNucleotides";
char sipName_BALL_getID[] = "getID";
char sipName_BALL_setID[] = "setID";
char sipName_BALL_get5Prime[] = "get5Prime";
char sipName_BALL_get3Prime[] = "get3Prime";
char sipName_BALL_getNucleotide[] = "getNucleotide";
char sipName_BALL_NucleicAcid[] = "NucleicAcid";
char sipName_BALL_getSystem[] = "getSystem";
char sipName_BALL_Molecule[] = "Molecule";
char sipName_BALL_Fragment[] = "Fragment";
char sipName_BALL_getExpression[] = "getExpression";
char sipName_BALL_setExpression[] = "setExpression";
char sipName_BALL_getPredicate[] = "getPredicate";
char sipName_BALL_[] = "";
char sipName_BALL_hasPredicate[] = "hasPredicate";
char sipName_BALL_Expression[] = "Expression";
char sipName_BALL_setArgument[] = "setArgument";
char sipName_BALL_ExpressionPredicate[] = "ExpressionPredicate";
char sipName_BALL_countPDBAtoms[] = "countPDBAtoms";
char sipName_BALL_countResidues[] = "countResidues";
char sipName_BALL_countSecondaryStructures[] = "countSecondaryStructures";
char sipName_BALL_getPDBAtom[] = "getPDBAtom";
char sipName_BALL_getCTerminal[] = "getCTerminal";
char sipName_BALL_getNTerminal[] = "getNTerminal";
char sipName_BALL_getSecondaryStructure[] = "getSecondaryStructure";
char sipName_BALL_Chain[] = "Chain";
char sipName_BALL_apply[] = "apply";
char sipName_BALL_isSuperBaseFragmentOf[] = "isSuperBaseFragmentOf";
char sipName_BALL_isSubBaseFragmentOf[] = "isSubBaseFragmentOf";
char sipName_BALL_remove[] = "remove";
char sipName_BALL_append[] = "append";
char sipName_BALL_prepend[] = "prepend";
char sipName_BALL_countIntraBonds[] = "countIntraBonds";
char sipName_BALL_countInterBonds[] = "countInterBonds";
char sipName_BALL_countAtoms[] = "countAtoms";
char sipName_BALL_countBaseFragments[] = "countBaseFragments";
char sipName_BALL_getAtom[] = "getAtom";
char sipName_BALL_getBaseFragment[] = "getBaseFragment";
char sipName_BALL_getSuperBaseFragment[] = "getSuperBaseFragment";
char sipName_BALL_BaseFragment[] = "BaseFragment";
char sipName_BALL_isIntraBondOf[] = "isIntraBondOf";
char sipName_BALL_isIntraBond[] = "isIntraBond";
char sipName_BALL_isInterBondOf[] = "isInterBondOf";
char sipName_BALL_isInterBond[] = "isInterBond";
char sipName_BALL_isBondOf[] = "isBondOf";
char sipName_BALL_getBoundAtom[] = "getBoundAtom";
char sipName_BALL_getLength[] = "getLength";
char sipName_BALL_getOrder[] = "getOrder";
char sipName_BALL_setOrder[] = "setOrder";
char sipName_BALL_getSecondAtom[] = "getSecondAtom";
char sipName_BALL_setSecondAtom[] = "setSecondAtom";
char sipName_BALL_getPartner[] = "getPartner";
char sipName_BALL_getFirstAtom[] = "getFirstAtom";
char sipName_BALL_setFirstAtom[] = "setFirstAtom";
char sipName_BALL_finalize[] = "finalize";
char sipName_BALL_Bond[] = "Bond";
char sipName_BALL_AtomType[] = "AtomType";
char sipName_BALL_isVicinal[] = "isVicinal";
char sipName_BALL_isGeminal[] = "isGeminal";
char sipName_BALL_isBoundTo[] = "isBoundTo";
char sipName_BALL_hasBond[] = "hasBond";
char sipName_BALL_destroyBonds[] = "destroyBonds";
char sipName_BALL_destroyBond[] = "destroyBond";
char sipName_BALL_cloneBond[] = "cloneBond";
char sipName_BALL_createBond[] = "createBond";
char sipName_BALL_getBond[] = "getBond";
char sipName_BALL_countBonds[] = "countBonds";
char sipName_BALL_getForce[] = "getForce";
char sipName_BALL_setForce[] = "setForce";
char sipName_BALL_getVelocity[] = "getVelocity";
char sipName_BALL_setVelocity[] = "setVelocity";
char sipName_BALL_setTypeName[] = "setTypeName";
char sipName_BALL_getTypeName[] = "getTypeName";
char sipName_BALL_setType[] = "setType";
char sipName_BALL_getRadius[] = "getRadius";
char sipName_BALL_setRadius[] = "setRadius";
char sipName_BALL_getPosition[] = "getPosition";
char sipName_BALL_setPosition[] = "setPosition";
char sipName_BALL_getFullName[] = "getFullName";
char sipName_BALL_getFragment[] = "getFragment";
char sipName_BALL_getMolecule[] = "getMolecule";
char sipName_BALL_getCharge[] = "getCharge";
char sipName_BALL_setCharge[] = "setCharge";
char sipName_BALL_setElement[] = "setElement";
char sipName_BALL_UNK[] = "UNK";
char sipName_BALL_getElement[] = "getElement";
char sipName_BALL_PTE_[] = "PTE_";
char sipName_BALL_isUnknown[] = "isUnknown";
char sipName_BALL_getElectronegativity[] = "getElectronegativity";
char sipName_BALL_setElectronegativity[] = "setElectronegativity";
char sipName_BALL_getVanDerWaalsRadius[] = "getVanDerWaalsRadius";
char sipName_BALL_setVanDerWaalsRadius[] = "setVanDerWaalsRadius";
char sipName_BALL_getCovalentRadius[] = "getCovalentRadius";
char sipName_BALL_setCovalentRadius[] = "setCovalentRadius";
char sipName_BALL_getAtomicRadius[] = "getAtomicRadius";
char sipName_BALL_setAtomicRadius[] = "setAtomicRadius";
char sipName_BALL_getAtomicWeight[] = "getAtomicWeight";
char sipName_BALL_setAtomicWeight[] = "setAtomicWeight";
char sipName_BALL_getAtomicNumber[] = "getAtomicNumber";
char sipName_BALL_setAtomicNumber[] = "setAtomicNumber";
char sipName_BALL_getPeriod[] = "getPeriod";
char sipName_BALL_setPeriod[] = "setPeriod";
char sipName_BALL_getGroup[] = "getGroup";
char sipName_BALL_setGroup[] = "setGroup";
char sipName_BALL_getSymbol[] = "getSymbol";
char sipName_BALL_setSymbol[] = "setSymbol";
char sipName_BALL_Element[] = "Element";
char sipName_BALL_getTemperatureFactor[] = "getTemperatureFactor";
char sipName_BALL_setTemperatureFactor[] = "setTemperatureFactor";
char sipName_BALL_getOccupancy[] = "getOccupancy";
char sipName_BALL_setOccupancy[] = "setOccupancy";
char sipName_BALL_getAlternateLocationIndicator[] = "getAlternateLocationIndicator";
char sipName_BALL_setAlternateLocationIndicator[] = "setAlternateLocationIndicator";
char sipName_BALL_getRemotenessIndicator[] = "getRemotenessIndicator";
char sipName_BALL_setRemotenessIndicator[] = "setRemotenessIndicator";
char sipName_BALL_getBranchDesignator[] = "getBranchDesignator";
char sipName_BALL_setBranchDesignator[] = "setBranchDesignator";
char sipName_BALL_getResidue[] = "getResidue";
char sipName_BALL_getChain[] = "getChain";
char sipName_BALL_getProtein[] = "getProtein";
char sipName_BALL_Atom[] = "Atom";
char sipName_BALL_PDBAtom[] = "PDBAtom";
char sipName_BALL_hasKey[] = "hasKey";
char sipName_BALL_save[] = "save";
char sipName_BALL_saveAs[] = "saveAs";
char sipName_BALL_close[] = "close";
char sipName_BALL_open[] = "open";
char sipName_BALL_SEPARATOR[] = "SEPARATOR";
char sipName_BALL_ENTRY_END[] = "ENTRY_END";
char sipName_BALL_ENTRY_BEGIN[] = "ENTRY_BEGIN";
char sipName_BALL_ResourceFile[] = "ResourceFile";
char sipName_BALL_findEntry[] = "findEntry";
char sipName_BALL_findDescendant[] = "findDescendant";
char sipName_BALL_findChild[] = "findChild";
char sipName_BALL_removeKey[] = "removeKey";
char sipName_BALL_mergeChildrenOf[] = "mergeChildrenOf";
char sipName_BALL_insert[] = "insert";
char sipName_BALL_insertSibling[] = "insertSibling";
char sipName_BALL_insertChild[] = "insertChild";
char sipName_BALL_countChildren[] = "countChildren";
char sipName_BALL_getPath[] = "getPath";
char sipName_BALL_getKey[] = "getKey";
char sipName_BALL_getEntry[] = "getEntry";
char sipName_BALL_ResourceEntry[] = "ResourceEntry";
char sipName_BALL_XYZFile[] = "XYZFile";
char sipName_BALL_PDBFile[] = "PDBFile";
char sipName_BALL_MOL2File[] = "MOL2File";
char sipName_BALL_setValue[] = "setValue";
char sipName_BALL_getValue[] = "getValue";
char sipName_BALL_hasEntry[] = "hasEntry";
char sipName_BALL_getSectionLength[] = "getSectionLength";
char sipName_BALL_getSectionLastLine[] = "getSectionLastLine";
char sipName_BALL_getSectionFirstLine[] = "getSectionFirstLine";
char sipName_BALL_hasSection[] = "hasSection";
char sipName_BALL_getNumberOfLines[] = "getNumberOfLines";
char sipName_BALL_setLine[] = "setLine";
char sipName_BALL_getLine[] = "getLine";
char sipName_BALL_setFilename[] = "setFilename";
char sipName_BALL_getFilename[] = "getFilename";
char sipName_BALL_INIFile[] = "INIFile";
char sipName_BALL_getTemperature[] = "getTemperature";
char sipName_BALL_getPeriodicBoundary[] = "getPeriodicBoundary";
char sipName_BALL_hasPeriodicBoundary[] = "hasPeriodicBoundary";
char sipName_BALL_read[] = "read";
char sipName_BALL_write[] = "write";
char sipName_BALL_File[] = "File";
char sipName_BALL_HINFile[] = "HINFile";
char sipName_BALL_calculateDistanceCoulomb[] = "calculateDistanceCoulomb";
char sipName_BALL_calculateCoulomb[] = "calculateCoulomb";
char sipName_BALL_calculateACE[] = "calculateACE";
char sipName_BALL_isWhitespace[] = "isWhitespace";
char sipName_BALL_isSpace[] = "isSpace";
char sipName_BALL_isDigit[] = "isDigit";
char sipName_BALL_isAlnum[] = "isAlnum";
char sipName_BALL_isAlpha[] = "isAlpha";
char sipName_BALL_hasSuffix[] = "hasSuffix";
char sipName_BALL_hasPrefix[] = "hasPrefix";
char sipName_BALL_hasSubstring[] = "hasSubstring";
char sipName_BALL_substitute[] = "substitute";
char sipName_BALL_reverse[] = "reverse";
char sipName_BALL_Operator__add__[] = "Operator__add__";
char sipName_BALL_instr[] = "instr";
char sipName_BALL_right[] = "right";
char sipName_BALL_left[] = "left";
char sipName_BALL_truncate[] = "truncate";
char sipName_BALL_trim[] = "trim";
char sipName_BALL_trimRight[] = "trimRight";
char sipName_BALL_trimLeft[] = "trimLeft";
char sipName_BALL_getField[] = "getField";
char sipName_BALL_countFields[] = "countFields";
char sipName_BALL_after[] = "after";
char sipName_BALL_from[] = "from";
char sipName_BALL_through[] = "through";
char sipName_BALL_before[] = "before";
char sipName_BALL_getSubstring[] = "getSubstring";
char sipName_BALL_toDouble[] = "toDouble";
char sipName_BALL_toFloat[] = "toFloat";
char sipName_BALL_toLong[] = "toLong";
char sipName_BALL_toInt[] = "toInt";
char sipName_BALL_toShort[] = "toShort";
char sipName_BALL_toChar[] = "toChar";
char sipName_BALL_toBool[] = "toBool";
char sipName_BALL_getCompareMode[] = "getCompareMode";
char sipName_BALL_setCompareMode[] = "setCompareMode";
char sipName_BALL_String[] = "String";
char sipName_BALL_isBound[] = "isBound";
char sipName_BALL_toUpper[] = "toUpper";
char sipName_BALL_toLower[] = "toLower";
char sipName_BALL_size[] = "size";
char sipName_BALL_getLastIndex[] = "getLastIndex";
char sipName_BALL_getFirstIndex[] = "getFirstIndex";
char sipName_BALL_c_str[] = "c_str";
char sipName_BALL_getBoundString[] = "getBoundString";
char sipName_BALL_unbind[] = "unbind";
char sipName_BALL_bind[] = "bind";
char sipName_BALL_toString[] = "toString";
char sipName_BALL_Substring[] = "Substring";
char sipName_BALL_find[] = "find";
char sipName_BALL_match[] = "match";
char sipName_BALL_countSubexpressions[] = "countSubexpressions";
char sipName_BALL_getPattern[] = "getPattern";
char sipName_BALL_WHITESPACE[] = "WHITESPACE";
char sipName_BALL_UPPERCASE[] = "UPPERCASE";
char sipName_BALL_NON_WHITESPACE[] = "NON_WHITESPACE";
char sipName_BALL_NON_NUMERIC[] = "NON_NUMERIC";
char sipName_BALL_NON_ALPHANUMERIC[] = "NON_ALPHANUMERIC";
char sipName_BALL_NON_ALPHA[] = "NON_ALPHA";
char sipName_BALL_LOWERCASE[] = "LOWERCASE";
char sipName_BALL_HEXADECIMAL_INTEGER[] = "HEXADECIMAL_INTEGER";
char sipName_BALL_INTEGER[] = "INTEGER";
char sipName_BALL_IDENTIFIER[] = "IDENTIFIER";
char sipName_BALL_REAL[] = "REAL";
char sipName_BALL_ALPHANUMERIC[] = "ALPHANUMERIC";
char sipName_BALL_ALPHA[] = "ALPHA";
char sipName_BALL_RegularExpression[] = "RegularExpression";
char sipName_BALL_getInterpolatedValue[] = "getInterpolatedValue";
char sipName_BALL_has[] = "has";
char sipName_BALL_getDimension[] = "getDimension";
char sipName_BALL_setOrigin[] = "setOrigin";
char sipName_BALL_getOrigin[] = "getOrigin";
char sipName_BALL_getBoxIndices[] = "getBoxIndices";
char sipName_BALL_getGridCoordinates[] = "getGridCoordinates";
char sipName_BALL_getIndex[] = "getIndex";
char sipName_BALL_getZSpacing[] = "getZSpacing";
char sipName_BALL_getYSpacing[] = "getYSpacing";
char sipName_BALL_getXSpacing[] = "getXSpacing";
char sipName_BALL_getMaxZIndex[] = "getMaxZIndex";
char sipName_BALL_getMaxYIndex[] = "getMaxYIndex";
char sipName_BALL_getMaxXIndex[] = "getMaxXIndex";
char sipName_BALL_getMinZ[] = "getMinZ";
char sipName_BALL_getMinY[] = "getMinY";
char sipName_BALL_getMinX[] = "getMinX";
char sipName_BALL_getMaxZ[] = "getMaxZ";
char sipName_BALL_getMaxY[] = "getMaxY";
char sipName_BALL_getMaxX[] = "getMaxX";
char sipName_BALL_FloatPointGrid[] = "FloatPointGrid";
char sipName_BALL_z[] = "z";
char sipName_BALL_y[] = "y";
char sipName_BALL_x[] = "x";
char sipName_BALL_GridIndex[] = "GridIndex";
char sipName_BALL_readOptionFile[] = "readOptionFile";
char sipName_BALL_setDefaultBool[] = "setDefaultBool";
char sipName_BALL_setDefaultReal[] = "setDefaultReal";
char sipName_BALL_setDefaultInteger[] = "setDefaultInteger";
char sipName_BALL_setDefault[] = "setDefault";
char sipName_BALL_setBool[] = "setBool";
char sipName_BALL_setInteger[] = "setInteger";
char sipName_BALL_setVector[] = "setVector";
char sipName_BALL_setReal[] = "setReal";
char sipName_BALL_getInteger[] = "getInteger";
char sipName_BALL_getVector[] = "getVector";
char sipName_BALL_getReal[] = "getReal";
char sipName_BALL_setName[] = "setName";
char sipName_BALL_isSet[] = "isSet";
char sipName_BALL_isVector[] = "isVector";
char sipName_BALL_isReal[] = "isReal";
char sipName_BALL_isBool[] = "isBool";
char sipName_BALL_isInteger[] = "isInteger";
char sipName_BALL_MAX_ENTRY_LENGTH[] = "MAX_ENTRY_LENGTH";
char sipName_BALL_Options[] = "Options";
char sipName_BALL_isEveryBit[] = "isEveryBit";
char sipName_BALL_isAnyBit[] = "isAnyBit";
char sipName_BALL_bitwiseAnd[] = "bitwiseAnd";
char sipName_BALL_bitwiseXor[] = "bitwiseXor";
char sipName_BALL_bitwiseOr[] = "bitwiseOr";
char sipName_BALL_toggle[] = "toggle";
char sipName_BALL_fill[] = "fill";
char sipName_BALL_toggleBit[] = "toggleBit";
char sipName_BALL_getBit[] = "getBit";
char sipName_BALL_setBit[] = "setBit";
char sipName_BALL_Operator__getitem__[] = "Operator__getitem__";
char sipName_BALL_countValue[] = "countValue";
char sipName_BALL_getSize[] = "getSize";
char sipName_BALL_setSize[] = "setSize";
char sipName_BALL_BlockSize[] = "BlockSize";
char sipName_BALL_BitVector[] = "BitVector";
char sipName_BALL_Bit[] = "Bit";
char sipName_BALL_isSelected[] = "isSelected";
char sipName_BALL_hasProperty[] = "hasProperty";
char sipName_BALL_countNamedProperties[] = "countNamedProperties";
char sipName_BALL_getProperty[] = "getProperty";
char sipName_BALL_countProperties[] = "countProperties";
char sipName_BALL_toggleProperty[] = "toggleProperty";
char sipName_BALL_clearProperty[] = "clearProperty";
char sipName_BALL_setProperty[] = "setProperty";
char sipName_BALL_PropertyManager[] = "PropertyManager";
char sipName_BALL_getString[] = "getString";
char sipName_BALL_getObject[] = "getObject";
char sipName_BALL_getUnsignedInt[] = "getUnsignedInt";
char sipName_BALL_getFloat[] = "getFloat";
char sipName_BALL_getInt[] = "getInt";
char sipName_BALL_getBool[] = "getBool";
char sipName_BALL_getName[] = "getName";
char sipName_BALL_NamedProperty[] = "NamedProperty";
char sipName_BALL_Processor[] = "Processor";
char sipName_BALL_UnaryAtomPredicate[] = "UnaryAtomPredicate";
char sipName_BALL_Operator__call__[] = "Operator__call__";
char sipName_BALL_UnaryCompositePredicate[] = "UnaryCompositePredicate";
char sipName_BALL_compare[] = "compare";
char sipName_BALL_getNewHandle[] = "getNewHandle";
char sipName_BALL_getNextHandle[] = "getNextHandle";
char sipName_BALL_getHandle[] = "getHandle";
char sipName_BALL_registerThis[] = "registerThis";
char sipName_BALL_unregisterThis[] = "unregisterThis";
char sipName_BALL_Embeddable[] = "Embeddable";
char sipName_BALL_isValid[] = "isValid";
char sipName_BALL_containsSelection[] = "containsSelection";
char sipName_BALL_isHomomorph[] = "isHomomorph";
char sipName_BALL_isRelatedWith[] = "isRelatedWith";
char sipName_BALL_isAncestorOf[] = "isAncestorOf";
char sipName_BALL_isAncestor[] = "isAncestor";
char sipName_BALL_hasAnyAncestor[] = "hasAnyAncestor";
char sipName_BALL_isDescendantOf[] = "isDescendantOf";
char sipName_BALL_isDescendant[] = "isDescendant";
char sipName_BALL_hasDescendant[] = "hasDescendant";
char sipName_BALL_isNextSiblingOf[] = "isNextSiblingOf";
char sipName_BALL_hasNextSibling[] = "hasNextSibling";
char sipName_BALL_isPreviousSiblingOf[] = "isPreviousSiblingOf";
char sipName_BALL_hasPreviousSibling[] = "hasPreviousSibling";
char sipName_BALL_isSiblingOf[] = "isSiblingOf";
char sipName_BALL_hasSibling[] = "hasSibling";
char sipName_BALL_isParentOf[] = "isParentOf";
char sipName_BALL_isParent[] = "isParent";
char sipName_BALL_hasParent[] = "hasParent";
char sipName_BALL_isLastChildOf[] = "isLastChildOf";
char sipName_BALL_isLastChild[] = "isLastChild";
char sipName_BALL_isFirstChildOf[] = "isFirstChildOf";
char sipName_BALL_isFirstChild[] = "isFirstChild";
char sipName_BALL_isChildOf[] = "isChildOf";
char sipName_BALL_isChild[] = "isChild";
char sipName_BALL_hasChild[] = "hasChild";
char sipName_BALL_isInterior[] = "isInterior";
char sipName_BALL_isRootOf[] = "isRootOf";
char sipName_BALL_isRoot[] = "isRoot";
char sipName_BALL_isEmpty[] = "isEmpty";
char sipName_BALL_isCollapsed[] = "isCollapsed";
char sipName_BALL_isExpanded[] = "isExpanded";
char sipName_BALL_deselect[] = "deselect";
char sipName_BALL_select[] = "select";
char sipName_BALL_swap[] = "swap";
char sipName_BALL_replace[] = "replace";
char sipName_BALL_removeChild[] = "removeChild";
char sipName_BALL_splice[] = "splice";
char sipName_BALL_spliceAfter[] = "spliceAfter";
char sipName_BALL_spliceBefore[] = "spliceBefore";
char sipName_BALL_insertAfter[] = "insertAfter";
char sipName_BALL_insertBefore[] = "insertBefore";
char sipName_BALL_insertParent[] = "insertParent";
char sipName_BALL_appendChild[] = "appendChild";
char sipName_BALL_prependChild[] = "prependChild";
char sipName_BALL_collapse[] = "collapse";
char sipName_BALL_expand[] = "expand";
char sipName_BALL_getLastChild[] = "getLastChild";
char sipName_BALL_getFirstChild[] = "getFirstChild";
char sipName_BALL_getSibling[] = "getSibling";
char sipName_BALL_getChild[] = "getChild";
char sipName_BALL_getParent[] = "getParent";
char sipName_BALL_getLowestCommonAncestor[] = "getLowestCommonAncestor";
char sipName_BALL_getRoot[] = "getRoot";
char sipName_BALL_getHeight[] = "getHeight";
char sipName_BALL_getDepth[] = "getDepth";
char sipName_BALL_getPathLength[] = "getPathLength";
char sipName_BALL_countDescendants[] = "countDescendants";
char sipName_BALL_getDegree[] = "getDegree";
char sipName_BALL_get[] = "get";
char sipName_BALL_set[] = "set";
char sipName_BALL_destroy[] = "destroy";
char sipName_BALL_clear[] = "clear";
char sipName_BALL_Selectable[] = "Selectable";
char sipName_BALL_Object[] = "Object";
char sipName_BALL_PersistentObject[] = "PersistentObject";
char sipName_BALL_Composite[] = "Composite";
char sipName_BALL_getType[] = "getType";
char sipName_BALL_getMinorRevision[] = "getMinorRevision";
char sipName_BALL_getMajorRevision[] = "getMajorRevision";
char sipName_BALL_getVersion[] = "getVersion";
char sipName_BALL_VersionInfo[] = "VersionInfo";
char sipName_BALL_Position[] = "Position";
char sipName_BALL_Index[] = "Index";
char sipName_BALL_CAL_PER_JOULE[] = "CAL_PER_JOULE";
char sipName_BALL_JOULE_PER_CAL[] = "JOULE_PER_CAL";
char sipName_BALL_M_PER_FOOT[] = "M_PER_FOOT";
char sipName_BALL_MM_PER_INCH[] = "MM_PER_INCH";
char sipName_BALL_RAD_PER_DEG[] = "RAD_PER_DEG";
char sipName_BALL_DEG_PER_RAD[] = "DEG_PER_RAD";
char sipName_BALL_FINE_STRUCTURE_CONSTANT[] = "FINE_STRUCTURE_CONSTANT";
char sipName_BALL_GRAVITATIONAL_CONSTANT[] = "GRAVITATIONAL_CONSTANT";
char sipName_BALL_c[] = "c";
char sipName_BALL_SPEED_OF_LIGHT[] = "SPEED_OF_LIGHT";
char sipName_BALL_VACUUM_PERMEABILITY[] = "VACUUM_PERMEABILITY";
char sipName_BALL_VACUUM_PERMITTIVITY[] = "VACUUM_PERMITTIVITY";
char sipName_BALL_a0[] = "a0";
char sipName_BALL_BOHR_RADIUS[] = "BOHR_RADIUS";
char sipName_BALL_F[] = "F";
char sipName_BALL_FARADAY[] = "FARADAY";
char sipName_BALL_R[] = "R";
char sipName_BALL_GAS_CONSTANT[] = "GAS_CONSTANT";
char sipName_BALL_h[] = "h";
char sipName_BALL_PLANCK[] = "PLANCK";
char sipName_BALL_k[] = "k";
char sipName_BALL_BOLTZMANN[] = "BOLTZMANN";
char sipName_BALL_MOL[] = "MOL";
char sipName_BALL_NA[] = "NA";
char sipName_BALL_AVOGADRO[] = "AVOGADRO";
char sipName_BALL_NEUTRON_MASS[] = "NEUTRON_MASS";
char sipName_BALL_PROTON_MASS[] = "PROTON_MASS";
char sipName_BALL_ELECTRON_MASS[] = "ELECTRON_MASS";
char sipName_BALL_e0[] = "e0";
char sipName_BALL_ELEMENTARY_CHARGE[] = "ELEMENTARY_CHARGE";
char sipName_BALL_EPSILON[] = "EPSILON";
char sipName_BALL_E[] = "E";
char sipName_BALL_PI[] = "PI";
char sipName_BALL_Constant[] = "Constant";
char sipName_BALL_BALL[] = "BALL";

static PyObject *sipDo_calculateSASPoints(PyObject *,PyObject *sipArgs)
{

	{
#line 44 "numericalSAS.sip"
	const BaseFragment *a0;
	PyObject *a0obj;
	float a1 = 1.5;
	Size a2 = 0;

	if (sipParseArgs(sipArgs,"-I|fI",sipCanConvertTo_BaseFragment,&a0obj,&a1,&a2))
	{
		Surface *res;

		int iserr = 0;

		sipConvertTo_BaseFragment(a0obj,(BaseFragment **)&a0,1,&iserr);
		if (iserr)
			return NULL;

		res = new Surface;
		calculateSASPoints(* a0, *res, a1,a2);

		return sipMapCppToSelf(res,sipClass_Surface);
	}
#line 941 "../CPP/BALLcmodule.cpp"
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoFunction(sipName_BALL_calculateSASPoints);

	return NULL;
}

static PyObject *sipDo_calculateSASAtomAreas(PyObject *,PyObject *sipArgs)
{

	{
#line 15 "numericalSAS.sip"
	const BaseFragment *a0;
	PyObject *a0obj;
	float a1 = 1.5;
	Size a2 = 0;

	if (sipParseArgs(sipArgs,"-I|fI",sipCanConvertTo_BaseFragment,&a0obj,&a1,&a2))
	{
		PyAtomDict *res;

		int iserr = 0;

		sipConvertTo_BaseFragment(a0obj,(BaseFragment **)&a0,1,&iserr);

		if (iserr)
			return NULL;

		res = new PyAtomDict;
		calculateSASAtomAreas(* a0, *res, a1, a2);
		PyObject *resobj = sipConvertFrom_PyAtomDict(res);

		delete res;

		return resobj;
	}
#line 980 "../CPP/BALLcmodule.cpp"
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoFunction(sipName_BALL_calculateSASAtomAreas);

	return NULL;
}

static PyObject *sipDo_calculateSASVolume(PyObject *,PyObject *sipArgs)
{

	{
		const BaseFragment *a0;
		PyObject *a0obj;
		float a1 = 1.5;
		int a2 = 400;

		if (sipParseArgs(sipArgs,"-I|fi",sipCanConvertTo_BaseFragment,&a0obj,&a1,&a2))
		{
			float res;

			int iserr = 0;

			sipConvertTo_BaseFragment(a0obj,(BaseFragment **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			res = calculateSASVolume(* a0, a1, a2);

			return PyFloat_FromDouble((double)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoFunction(sipName_BALL_calculateSASVolume);

	return NULL;
}

static PyObject *sipDo_calculateSASArea(PyObject *,PyObject *sipArgs)
{

	{
		const BaseFragment *a0;
		PyObject *a0obj;
		float a1 = 1.5;
		int a2 = 400;

		if (sipParseArgs(sipArgs,"-I|fi",sipCanConvertTo_BaseFragment,&a0obj,&a1,&a2))
		{
			float res;

			int iserr = 0;

			sipConvertTo_BaseFragment(a0obj,(BaseFragment **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			res = calculateSASArea(* a0, a1, a2);

			return PyFloat_FromDouble((double)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoFunction(sipName_BALL_calculateSASArea);

	return NULL;
}

static PyObject *sipDo_calculateBondAngle(PyObject *,PyObject *sipArgs)
{

	{
		const Atom *a0;
		PyObject *a0obj;
		const Atom *a1;
		PyObject *a1obj;
		const Atom *a2;
		PyObject *a2obj;

		if (sipParseArgs(sipArgs,"-III",sipCanConvertTo_Atom,&a0obj,sipCanConvertTo_Atom,&a1obj,sipCanConvertTo_Atom,&a2obj))
		{
			Angle *res;

			int iserr = 0;

			sipConvertTo_Atom(a0obj,(Atom **)&a0,1,&iserr);
			sipConvertTo_Atom(a1obj,(Atom **)&a1,1,&iserr);
			sipConvertTo_Atom(a2obj,(Atom **)&a2,1,&iserr);

			if (iserr)
				return NULL;

			res = new Angle(calculateBondAngle(* a0,* a1,* a2));

			return sipNewCppToSelf(res,sipClass_Angle,SIP_SIMPLE | SIP_PY_OWNED);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoFunction(sipName_BALL_calculateBondAngle);

	return NULL;
}

static PyObject *sipDo_calculateTorsionAngle(PyObject *,PyObject *sipArgs)
{

	{
		const Atom *a0;
		PyObject *a0obj;
		const Atom *a1;
		PyObject *a1obj;
		const Atom *a2;
		PyObject *a2obj;
		const Atom *a3;
		PyObject *a3obj;

		if (sipParseArgs(sipArgs,"-IIII",sipCanConvertTo_Atom,&a0obj,sipCanConvertTo_Atom,&a1obj,sipCanConvertTo_Atom,&a2obj,sipCanConvertTo_Atom,&a3obj))
		{
			Angle *res;

			int iserr = 0;

			sipConvertTo_Atom(a0obj,(Atom **)&a0,1,&iserr);
			sipConvertTo_Atom(a1obj,(Atom **)&a1,1,&iserr);
			sipConvertTo_Atom(a2obj,(Atom **)&a2,1,&iserr);
			sipConvertTo_Atom(a3obj,(Atom **)&a3,1,&iserr);

			if (iserr)
				return NULL;

			res = new Angle(calculateTorsionAngle(* a0,* a1,* a2,* a3));

			return sipNewCppToSelf(res,sipClass_Angle,SIP_SIMPLE | SIP_PY_OWNED);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoFunction(sipName_BALL_calculateTorsionAngle);

	return NULL;
}

static PyObject *sipDo_nucleicAcids(PyObject *,PyObject *sipArgs)
{

	{
		const BaseFragment *a0;
		PyObject *a0obj;
		long a1 = false;

		if (sipParseArgs(sipArgs,"-I|l",sipCanConvertTo_BaseFragment,&a0obj,&a1))
		{
			PyNucleicAcidList *res;

			int iserr = 0;

			sipConvertTo_BaseFragment(a0obj,(BaseFragment **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			res = nucleicAcids(* a0, (bool)a1);

			PyObject *resobj = sipConvertFrom_PyNucleicAcidList(res);

			return resobj;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoFunction(sipName_BALL_nucleicAcids);

	return NULL;
}

static PyObject *sipDo_nucleotides(PyObject *,PyObject *sipArgs)
{

	{
		const BaseFragment *a0;
		PyObject *a0obj;
		long a1 = false;

		if (sipParseArgs(sipArgs,"-I|l",sipCanConvertTo_BaseFragment,&a0obj,&a1))
		{
			PyNucleotideList *res;

			int iserr = 0;

			sipConvertTo_BaseFragment(a0obj,(BaseFragment **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			res = nucleotides(* a0, (bool)a1);

			PyObject *resobj = sipConvertFrom_PyNucleotideList(res);

			return resobj;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoFunction(sipName_BALL_nucleotides);

	return NULL;
}

static PyObject *sipDo_proteins(PyObject *,PyObject *sipArgs)
{

	{
		const BaseFragment *a0;
		PyObject *a0obj;
		long a1 = false;

		if (sipParseArgs(sipArgs,"-I|l",sipCanConvertTo_BaseFragment,&a0obj,&a1))
		{
			PyProteinList *res;

			int iserr = 0;

			sipConvertTo_BaseFragment(a0obj,(BaseFragment **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			res = proteins(* a0, (bool)a1);

			PyObject *resobj = sipConvertFrom_PyProteinList(res);

			return resobj;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoFunction(sipName_BALL_proteins);

	return NULL;
}

static PyObject *sipDo_chains(PyObject *,PyObject *sipArgs)
{

	{
		const BaseFragment *a0;
		PyObject *a0obj;
		long a1 = false;

		if (sipParseArgs(sipArgs,"-I|l",sipCanConvertTo_BaseFragment,&a0obj,&a1))
		{
			PyChainList *res;

			int iserr = 0;

			sipConvertTo_BaseFragment(a0obj,(BaseFragment **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			res = chains(* a0, (bool)a1);

			PyObject *resobj = sipConvertFrom_PyChainList(res);

			return resobj;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoFunction(sipName_BALL_chains);

	return NULL;
}

static PyObject *sipDo_secondaryStructures(PyObject *,PyObject *sipArgs)
{

	{
		const BaseFragment *a0;
		PyObject *a0obj;
		long a1 = false;

		if (sipParseArgs(sipArgs,"-I|l",sipCanConvertTo_BaseFragment,&a0obj,&a1))
		{
			PySecondaryStructureList *res;

			int iserr = 0;

			sipConvertTo_BaseFragment(a0obj,(BaseFragment **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			res = secondaryStructures(* a0, (bool)a1);

			PyObject *resobj = sipConvertFrom_PySecondaryStructureList(res);

			return resobj;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoFunction(sipName_BALL_secondaryStructures);

	return NULL;
}

static PyObject *sipDo_residues(PyObject *,PyObject *sipArgs)
{

	{
		const BaseFragment *a0;
		PyObject *a0obj;
		long a1 = false;

		if (sipParseArgs(sipArgs,"-I|l",sipCanConvertTo_BaseFragment,&a0obj,&a1))
		{
			PyResidueList *res;

			int iserr = 0;

			sipConvertTo_BaseFragment(a0obj,(BaseFragment **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			res = residues(* a0, (bool)a1);

			PyObject *resobj = sipConvertFrom_PyResidueList(res);

			return resobj;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoFunction(sipName_BALL_residues);

	return NULL;
}

static PyObject *sipDo_molecules(PyObject *,PyObject *sipArgs)
{

	{
		const BaseFragment *a0;
		PyObject *a0obj;
		long a1 = false;

		if (sipParseArgs(sipArgs,"-I|l",sipCanConvertTo_BaseFragment,&a0obj,&a1))
		{
			PyMoleculeList *res;

			int iserr = 0;

			sipConvertTo_BaseFragment(a0obj,(BaseFragment **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			res = molecules(* a0, (bool)a1);

			PyObject *resobj = sipConvertFrom_PyMoleculeList(res);

			return resobj;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoFunction(sipName_BALL_molecules);

	return NULL;
}

static PyObject *sipDo_fragments(PyObject *,PyObject *sipArgs)
{

	{
		const BaseFragment *a0;
		PyObject *a0obj;
		long a1 = false;

		if (sipParseArgs(sipArgs,"-I|l",sipCanConvertTo_BaseFragment,&a0obj,&a1))
		{
			PyFragmentList *res;

			int iserr = 0;

			sipConvertTo_BaseFragment(a0obj,(BaseFragment **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			res = fragments(* a0, (bool)a1);

			PyObject *resobj = sipConvertFrom_PyFragmentList(res);

			return resobj;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoFunction(sipName_BALL_fragments);

	return NULL;
}

static PyObject *sipDo_baseFragments(PyObject *,PyObject *sipArgs)
{

	{
		const BaseFragment *a0;
		PyObject *a0obj;
		long a1 = false;

		if (sipParseArgs(sipArgs,"-I|l",sipCanConvertTo_BaseFragment,&a0obj,&a1))
		{
			PyBaseFragmentList *res;

			int iserr = 0;

			sipConvertTo_BaseFragment(a0obj,(BaseFragment **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			res = baseFragments(* a0, (bool)a1);

			PyObject *resobj = sipConvertFrom_PyBaseFragmentList(res);

			return resobj;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoFunction(sipName_BALL_baseFragments);

	return NULL;
}

static PyObject *sipDo_bonds(PyObject *,PyObject *sipArgs)
{

	{
		const BaseFragment *a0;
		PyObject *a0obj;
		long a1 = false;

		if (sipParseArgs(sipArgs,"-I|l",sipCanConvertTo_BaseFragment,&a0obj,&a1))
		{
			PyBondList *res;

			int iserr = 0;

			sipConvertTo_BaseFragment(a0obj,(BaseFragment **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			res = bonds(* a0, (bool)a1);

			PyObject *resobj = sipConvertFrom_PyBondList(res);

			return resobj;
		}
	}

	{
		const Atom *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"-I",sipCanConvertTo_Atom,&a0obj))
		{
			PyBondList *res;

			int iserr = 0;

			sipConvertTo_Atom(a0obj,(Atom **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			res = bonds(* a0);

			PyObject *resobj = sipConvertFrom_PyBondList(res);

			return resobj;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoFunction(sipName_BALL_bonds);

	return NULL;
}

static PyObject *sipDo_PDBAtoms(PyObject *,PyObject *sipArgs)
{

	{
		const BaseFragment *a0;
		PyObject *a0obj;
		const String *a1;
		PyObject *a1obj;

		if (sipParseArgs(sipArgs,"-II",sipCanConvertTo_BaseFragment,&a0obj,sipCanConvertTo_String,&a1obj))
		{
			PyPDBAtomList *res;

			int iserr = 0;

			sipConvertTo_BaseFragment(a0obj,(BaseFragment **)&a0,1,&iserr);
			int istemp1 = sipConvertTo_String(a1obj,(String **)&a1,1,&iserr);

			if (iserr)
				return NULL;

			res = PDBAtoms(* a0,* a1);

			if (istemp1)
				delete a1;

			PyObject *resobj = sipConvertFrom_PyPDBAtomList(res);

			return resobj;
		}
	}

	{
		const BaseFragment *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"-I",sipCanConvertTo_BaseFragment,&a0obj))
		{
			PyPDBAtomList *res;

			int iserr = 0;

			sipConvertTo_BaseFragment(a0obj,(BaseFragment **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			res = PDBAtoms(* a0);

			PyObject *resobj = sipConvertFrom_PyPDBAtomList(res);

			return resobj;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoFunction(sipName_BALL_PDBAtoms);

	return NULL;
}

static PyObject *sipDo_atoms(PyObject *,PyObject *sipArgs)
{

	{
		const BaseFragment *a0;
		PyObject *a0obj;
		const String *a1;
		PyObject *a1obj;

		if (sipParseArgs(sipArgs,"-II",sipCanConvertTo_BaseFragment,&a0obj,sipCanConvertTo_String,&a1obj))
		{
			PyAtomList *res;

			int iserr = 0;

			sipConvertTo_BaseFragment(a0obj,(BaseFragment **)&a0,1,&iserr);
			int istemp1 = sipConvertTo_String(a1obj,(String **)&a1,1,&iserr);

			if (iserr)
				return NULL;

			res = atoms(* a0,* a1);

			if (istemp1)
				delete a1;

			PyObject *resobj = sipConvertFrom_PyAtomList(res);

			return resobj;
		}
	}

	{
		const BaseFragment *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"-I",sipCanConvertTo_BaseFragment,&a0obj))
		{
			PyAtomList *res;

			int iserr = 0;

			sipConvertTo_BaseFragment(a0obj,(BaseFragment **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			res = atoms(* a0);

			PyObject *resobj = sipConvertFrom_PyAtomList(res);

			return resobj;
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoFunction(sipName_BALL_atoms);

	return NULL;
}

static PyObject *sipDo_getTorsionAngle(PyObject *,PyObject *sipArgs)
{

	{
		float a0;
		float a1;
		float a2;
		float a3;
		float a4;
		float a5;
		float a6;
		float a7;
		float a8;
		float a9;
		float a10;
		float a11;

		if (sipParseArgs(sipArgs,"-ffffffffffff",&a0,&a1,&a2,&a3,&a4,&a5,&a6,&a7,&a8,&a9,&a10,&a11))
		{
			Angle *res;

			res = new Angle(getTorsionAngle( a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11));

			return sipNewCppToSelf(res,sipClass_Angle,SIP_SIMPLE | SIP_PY_OWNED);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoFunction(sipName_BALL_getTorsionAngle);

	return NULL;
}

static PyObject *sipDo_isParallel(PyObject *,PyObject *sipArgs)
{

	{
		const Line3 *a0;
		PyObject *a0obj;
		const Plane3 *a1;
		PyObject *a1obj;

		if (sipParseArgs(sipArgs,"-II",sipCanConvertTo_Line3,&a0obj,sipCanConvertTo_Plane3,&a1obj))
		{
			bool res;

			int iserr = 0;

			sipConvertTo_Line3(a0obj,(Line3 **)&a0,1,&iserr);
			sipConvertTo_Plane3(a1obj,(Plane3 **)&a1,1,&iserr);

			if (iserr)
				return NULL;

			res = isParallel(* a0,* a1);

			return sipConvertFromBool((int)res);
		}
	}

	{
		const Plane3 *a0;
		PyObject *a0obj;
		const Line3 *a1;
		PyObject *a1obj;

		if (sipParseArgs(sipArgs,"-II",sipCanConvertTo_Plane3,&a0obj,sipCanConvertTo_Line3,&a1obj))
		{
			bool res;

			int iserr = 0;

			sipConvertTo_Plane3(a0obj,(Plane3 **)&a0,1,&iserr);
			sipConvertTo_Line3(a1obj,(Line3 **)&a1,1,&iserr);

			if (iserr)
				return NULL;

			res = isParallel(* a0,* a1);

			return sipConvertFromBool((int)res);
		}
	}

	{
		const Plane3 *a0;
		PyObject *a0obj;
		const Plane3 *a1;
		PyObject *a1obj;

		if (sipParseArgs(sipArgs,"-II",sipCanConvertTo_Plane3,&a0obj,sipCanConvertTo_Plane3,&a1obj))
		{
			bool res;

			int iserr = 0;

			sipConvertTo_Plane3(a0obj,(Plane3 **)&a0,1,&iserr);
			sipConvertTo_Plane3(a1obj,(Plane3 **)&a1,1,&iserr);

			if (iserr)
				return NULL;

			res = isParallel(* a0,* a1);

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoFunction(sipName_BALL_isParallel);

	return NULL;
}

static PyObject *sipDo_isIntersecting(PyObject *,PyObject *sipArgs)
{

	{
		const Vector3 *a0;
		PyObject *a0obj;
		const Line3 *a1;
		PyObject *a1obj;

		if (sipParseArgs(sipArgs,"-II",sipCanConvertTo_Vector3,&a0obj,sipCanConvertTo_Line3,&a1obj))
		{
			bool res;

			int iserr = 0;

			sipConvertTo_Vector3(a0obj,(Vector3 **)&a0,1,&iserr);
			sipConvertTo_Line3(a1obj,(Line3 **)&a1,1,&iserr);

			if (iserr)
				return NULL;

			res = isIntersecting(* a0,* a1);

			return sipConvertFromBool((int)res);
		}
	}

	{
		const Line3 *a0;
		PyObject *a0obj;
		const Vector3 *a1;
		PyObject *a1obj;

		if (sipParseArgs(sipArgs,"-II",sipCanConvertTo_Line3,&a0obj,sipCanConvertTo_Vector3,&a1obj))
		{
			bool res;

			int iserr = 0;

			sipConvertTo_Line3(a0obj,(Line3 **)&a0,1,&iserr);
			sipConvertTo_Vector3(a1obj,(Vector3 **)&a1,1,&iserr);

			if (iserr)
				return NULL;

			res = isIntersecting(* a0,* a1);

			return sipConvertFromBool((int)res);
		}
	}

	{
		const Line3 *a0;
		PyObject *a0obj;
		const Line3 *a1;
		PyObject *a1obj;

		if (sipParseArgs(sipArgs,"-II",sipCanConvertTo_Line3,&a0obj,sipCanConvertTo_Line3,&a1obj))
		{
			bool res;

			int iserr = 0;

			sipConvertTo_Line3(a0obj,(Line3 **)&a0,1,&iserr);
			sipConvertTo_Line3(a1obj,(Line3 **)&a1,1,&iserr);

			if (iserr)
				return NULL;

			res = isIntersecting(* a0,* a1);

			return sipConvertFromBool((int)res);
		}
	}

	{
		const Vector3 *a0;
		PyObject *a0obj;
		const Plane3 *a1;
		PyObject *a1obj;

		if (sipParseArgs(sipArgs,"-II",sipCanConvertTo_Vector3,&a0obj,sipCanConvertTo_Plane3,&a1obj))
		{
			bool res;

			int iserr = 0;

			sipConvertTo_Vector3(a0obj,(Vector3 **)&a0,1,&iserr);
			sipConvertTo_Plane3(a1obj,(Plane3 **)&a1,1,&iserr);

			if (iserr)
				return NULL;

			res = isIntersecting(* a0,* a1);

			return sipConvertFromBool((int)res);
		}
	}

	{
		const Plane3 *a0;
		PyObject *a0obj;
		const Vector3 *a1;
		PyObject *a1obj;

		if (sipParseArgs(sipArgs,"-II",sipCanConvertTo_Plane3,&a0obj,sipCanConvertTo_Vector3,&a1obj))
		{
			bool res;

			int iserr = 0;

			sipConvertTo_Plane3(a0obj,(Plane3 **)&a0,1,&iserr);
			sipConvertTo_Vector3(a1obj,(Vector3 **)&a1,1,&iserr);

			if (iserr)
				return NULL;

			res = isIntersecting(* a0,* a1);

			return sipConvertFromBool((int)res);
		}
	}

	{
		const Line3 *a0;
		PyObject *a0obj;
		const Plane3 *a1;
		PyObject *a1obj;

		if (sipParseArgs(sipArgs,"-II",sipCanConvertTo_Line3,&a0obj,sipCanConvertTo_Plane3,&a1obj))
		{
			bool res;

			int iserr = 0;

			sipConvertTo_Line3(a0obj,(Line3 **)&a0,1,&iserr);
			sipConvertTo_Plane3(a1obj,(Plane3 **)&a1,1,&iserr);

			if (iserr)
				return NULL;

			res = isIntersecting(* a0,* a1);

			return sipConvertFromBool((int)res);
		}
	}

	{
		const Plane3 *a0;
		PyObject *a0obj;
		const Line3 *a1;
		PyObject *a1obj;

		if (sipParseArgs(sipArgs,"-II",sipCanConvertTo_Plane3,&a0obj,sipCanConvertTo_Line3,&a1obj))
		{
			bool res;

			int iserr = 0;

			sipConvertTo_Plane3(a0obj,(Plane3 **)&a0,1,&iserr);
			sipConvertTo_Line3(a1obj,(Line3 **)&a1,1,&iserr);

			if (iserr)
				return NULL;

			res = isIntersecting(* a0,* a1);

			return sipConvertFromBool((int)res);
		}
	}

	{
		const Plane3 *a0;
		PyObject *a0obj;
		const Plane3 *a1;
		PyObject *a1obj;

		if (sipParseArgs(sipArgs,"-II",sipCanConvertTo_Plane3,&a0obj,sipCanConvertTo_Plane3,&a1obj))
		{
			bool res;

			int iserr = 0;

			sipConvertTo_Plane3(a0obj,(Plane3 **)&a0,1,&iserr);
			sipConvertTo_Plane3(a1obj,(Plane3 **)&a1,1,&iserr);

			if (iserr)
				return NULL;

			res = isIntersecting(* a0,* a1);

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoFunction(sipName_BALL_isIntersecting);

	return NULL;
}

static PyObject *sipDo_isOrthogonal(PyObject *,PyObject *sipArgs)
{

	{
		const Vector3 *a0;
		PyObject *a0obj;
		const Vector3 *a1;
		PyObject *a1obj;

		if (sipParseArgs(sipArgs,"-II",sipCanConvertTo_Vector3,&a0obj,sipCanConvertTo_Vector3,&a1obj))
		{
			bool res;

			int iserr = 0;

			sipConvertTo_Vector3(a0obj,(Vector3 **)&a0,1,&iserr);
			sipConvertTo_Vector3(a1obj,(Vector3 **)&a1,1,&iserr);

			if (iserr)
				return NULL;

			res = isOrthogonal(* a0,* a1);

			return sipConvertFromBool((int)res);
		}
	}

	{
		const Vector3 *a0;
		PyObject *a0obj;
		const Line3 *a1;
		PyObject *a1obj;

		if (sipParseArgs(sipArgs,"-II",sipCanConvertTo_Vector3,&a0obj,sipCanConvertTo_Line3,&a1obj))
		{
			bool res;

			int iserr = 0;

			sipConvertTo_Vector3(a0obj,(Vector3 **)&a0,1,&iserr);
			sipConvertTo_Line3(a1obj,(Line3 **)&a1,1,&iserr);

			if (iserr)
				return NULL;

			res = isOrthogonal(* a0,* a1);

			return sipConvertFromBool((int)res);
		}
	}

	{
		const Line3 *a0;
		PyObject *a0obj;
		const Vector3 *a1;
		PyObject *a1obj;

		if (sipParseArgs(sipArgs,"-II",sipCanConvertTo_Line3,&a0obj,sipCanConvertTo_Vector3,&a1obj))
		{
			bool res;

			int iserr = 0;

			sipConvertTo_Line3(a0obj,(Line3 **)&a0,1,&iserr);
			sipConvertTo_Vector3(a1obj,(Vector3 **)&a1,1,&iserr);

			if (iserr)
				return NULL;

			res = isOrthogonal(* a0,* a1);

			return sipConvertFromBool((int)res);
		}
	}

	{
		const Line3 *a0;
		PyObject *a0obj;
		const Line3 *a1;
		PyObject *a1obj;

		if (sipParseArgs(sipArgs,"-II",sipCanConvertTo_Line3,&a0obj,sipCanConvertTo_Line3,&a1obj))
		{
			bool res;

			int iserr = 0;

			sipConvertTo_Line3(a0obj,(Line3 **)&a0,1,&iserr);
			sipConvertTo_Line3(a1obj,(Line3 **)&a1,1,&iserr);

			if (iserr)
				return NULL;

			res = isOrthogonal(* a0,* a1);

			return sipConvertFromBool((int)res);
		}
	}

	{
		const Vector3 *a0;
		PyObject *a0obj;
		const Plane3 *a1;
		PyObject *a1obj;

		if (sipParseArgs(sipArgs,"-II",sipCanConvertTo_Vector3,&a0obj,sipCanConvertTo_Plane3,&a1obj))
		{
			bool res;

			int iserr = 0;

			sipConvertTo_Vector3(a0obj,(Vector3 **)&a0,1,&iserr);
			sipConvertTo_Plane3(a1obj,(Plane3 **)&a1,1,&iserr);

			if (iserr)
				return NULL;

			res = isOrthogonal(* a0,* a1);

			return sipConvertFromBool((int)res);
		}
	}

	{
		const Plane3 *a0;
		PyObject *a0obj;
		const Vector3 *a1;
		PyObject *a1obj;

		if (sipParseArgs(sipArgs,"-II",sipCanConvertTo_Plane3,&a0obj,sipCanConvertTo_Vector3,&a1obj))
		{
			bool res;

			int iserr = 0;

			sipConvertTo_Plane3(a0obj,(Plane3 **)&a0,1,&iserr);
			sipConvertTo_Vector3(a1obj,(Vector3 **)&a1,1,&iserr);

			if (iserr)
				return NULL;

			res = isOrthogonal(* a0,* a1);

			return sipConvertFromBool((int)res);
		}
	}

	{
		const Plane3 *a0;
		PyObject *a0obj;
		const Plane3 *a1;
		PyObject *a1obj;

		if (sipParseArgs(sipArgs,"-II",sipCanConvertTo_Plane3,&a0obj,sipCanConvertTo_Plane3,&a1obj))
		{
			bool res;

			int iserr = 0;

			sipConvertTo_Plane3(a0obj,(Plane3 **)&a0,1,&iserr);
			sipConvertTo_Plane3(a1obj,(Plane3 **)&a1,1,&iserr);

			if (iserr)
				return NULL;

			res = isOrthogonal(* a0,* a1);

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoFunction(sipName_BALL_isOrthogonal);

	return NULL;
}

static PyObject *sipDo_isComplanar(PyObject *,PyObject *sipArgs)
{

	{
		const Vector3 *a0;
		PyObject *a0obj;
		const Vector3 *a1;
		PyObject *a1obj;
		const Vector3 *a2;
		PyObject *a2obj;

		if (sipParseArgs(sipArgs,"-III",sipCanConvertTo_Vector3,&a0obj,sipCanConvertTo_Vector3,&a1obj,sipCanConvertTo_Vector3,&a2obj))
		{
			bool res;

			int iserr = 0;

			sipConvertTo_Vector3(a0obj,(Vector3 **)&a0,1,&iserr);
			sipConvertTo_Vector3(a1obj,(Vector3 **)&a1,1,&iserr);
			sipConvertTo_Vector3(a2obj,(Vector3 **)&a2,1,&iserr);

			if (iserr)
				return NULL;

			res = isComplanar(* a0,* a1,* a2);

			return sipConvertFromBool((int)res);
		}
	}

	{
		const Vector3 *a0;
		PyObject *a0obj;
		const Vector3 *a1;
		PyObject *a1obj;
		const Vector3 *a2;
		PyObject *a2obj;
		const Vector3 *a3;
		PyObject *a3obj;

		if (sipParseArgs(sipArgs,"-IIII",sipCanConvertTo_Vector3,&a0obj,sipCanConvertTo_Vector3,&a1obj,sipCanConvertTo_Vector3,&a2obj,sipCanConvertTo_Vector3,&a3obj))
		{
			bool res;

			int iserr = 0;

			sipConvertTo_Vector3(a0obj,(Vector3 **)&a0,1,&iserr);
			sipConvertTo_Vector3(a1obj,(Vector3 **)&a1,1,&iserr);
			sipConvertTo_Vector3(a2obj,(Vector3 **)&a2,1,&iserr);
			sipConvertTo_Vector3(a3obj,(Vector3 **)&a3,1,&iserr);

			if (iserr)
				return NULL;

			res = isComplanar(* a0,* a1,* a2,* a3);

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoFunction(sipName_BALL_isComplanar);

	return NULL;
}

static PyObject *sipDo_isCollinear(PyObject *,PyObject *sipArgs)
{

	{
		const Vector3 *a0;
		PyObject *a0obj;
		const Vector3 *a1;
		PyObject *a1obj;

		if (sipParseArgs(sipArgs,"-II",sipCanConvertTo_Vector3,&a0obj,sipCanConvertTo_Vector3,&a1obj))
		{
			bool res;

			int iserr = 0;

			sipConvertTo_Vector3(a0obj,(Vector3 **)&a0,1,&iserr);
			sipConvertTo_Vector3(a1obj,(Vector3 **)&a1,1,&iserr);

			if (iserr)
				return NULL;

			res = isCollinear(* a0,* a1);

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoFunction(sipName_BALL_isCollinear);

	return NULL;
}

static PyObject *sipDo_GetIntersection(PyObject *,PyObject *sipArgs)
{

	{
		const Line3 *a0;
		PyObject *a0obj;
		const Line3 *a1;
		PyObject *a1obj;
		Vector3 *a2;
		PyObject *a2obj;

		if (sipParseArgs(sipArgs,"-III",sipCanConvertTo_Line3,&a0obj,sipCanConvertTo_Line3,&a1obj,sipCanConvertTo_Vector3,&a2obj))
		{
			bool res;

			int iserr = 0;

			sipConvertTo_Line3(a0obj,(Line3 **)&a0,1,&iserr);
			sipConvertTo_Line3(a1obj,(Line3 **)&a1,1,&iserr);
			sipConvertTo_Vector3(a2obj,&a2,1,&iserr);

			if (iserr)
				return NULL;

			res = GetIntersection(* a0,* a1,* a2);

			return sipConvertFromBool((int)res);
		}
	}

	{
		const Plane3 *a0;
		PyObject *a0obj;
		const Line3 *a1;
		PyObject *a1obj;
		Vector3 *a2;
		PyObject *a2obj;

		if (sipParseArgs(sipArgs,"-III",sipCanConvertTo_Plane3,&a0obj,sipCanConvertTo_Line3,&a1obj,sipCanConvertTo_Vector3,&a2obj))
		{
			bool res;

			int iserr = 0;

			sipConvertTo_Plane3(a0obj,(Plane3 **)&a0,1,&iserr);
			sipConvertTo_Line3(a1obj,(Line3 **)&a1,1,&iserr);
			sipConvertTo_Vector3(a2obj,&a2,1,&iserr);

			if (iserr)
				return NULL;

			res = GetIntersection(* a0,* a1,* a2);

			return sipConvertFromBool((int)res);
		}
	}

	{
		const Line3 *a0;
		PyObject *a0obj;
		const Plane3 *a1;
		PyObject *a1obj;
		Vector3 *a2;
		PyObject *a2obj;

		if (sipParseArgs(sipArgs,"-III",sipCanConvertTo_Line3,&a0obj,sipCanConvertTo_Plane3,&a1obj,sipCanConvertTo_Vector3,&a2obj))
		{
			bool res;

			int iserr = 0;

			sipConvertTo_Line3(a0obj,(Line3 **)&a0,1,&iserr);
			sipConvertTo_Plane3(a1obj,(Plane3 **)&a1,1,&iserr);
			sipConvertTo_Vector3(a2obj,&a2,1,&iserr);

			if (iserr)
				return NULL;

			res = GetIntersection(* a0,* a1,* a2);

			return sipConvertFromBool((int)res);
		}
	}

	{
		const Plane3 *a0;
		PyObject *a0obj;
		const Plane3 *a1;
		PyObject *a1obj;
		Line3 *a2;
		PyObject *a2obj;

		if (sipParseArgs(sipArgs,"-III",sipCanConvertTo_Plane3,&a0obj,sipCanConvertTo_Plane3,&a1obj,sipCanConvertTo_Line3,&a2obj))
		{
			bool res;

			int iserr = 0;

			sipConvertTo_Plane3(a0obj,(Plane3 **)&a0,1,&iserr);
			sipConvertTo_Plane3(a1obj,(Plane3 **)&a1,1,&iserr);
			sipConvertTo_Line3(a2obj,&a2,1,&iserr);

			if (iserr)
				return NULL;

			res = GetIntersection(* a0,* a1,* a2);

			return sipConvertFromBool((int)res);
		}
	}

	{
		const Sphere3 *a0;
		PyObject *a0obj;
		const Line3 *a1;
		PyObject *a1obj;
		Vector3 *a2;
		PyObject *a2obj;
		Vector3 *a3;
		PyObject *a3obj;

		if (sipParseArgs(sipArgs,"-IIII",sipCanConvertTo_Sphere3,&a0obj,sipCanConvertTo_Line3,&a1obj,sipCanConvertTo_Vector3,&a2obj,sipCanConvertTo_Vector3,&a3obj))
		{
			bool res;

			int iserr = 0;

			sipConvertTo_Sphere3(a0obj,(Sphere3 **)&a0,1,&iserr);
			sipConvertTo_Line3(a1obj,(Line3 **)&a1,1,&iserr);
			sipConvertTo_Vector3(a2obj,&a2,1,&iserr);
			sipConvertTo_Vector3(a3obj,&a3,1,&iserr);

			if (iserr)
				return NULL;

			res = GetIntersection(* a0,* a1,* a2,* a3);

			return sipConvertFromBool((int)res);
		}
	}

	{
		const Line3 *a0;
		PyObject *a0obj;
		const Sphere3 *a1;
		PyObject *a1obj;
		Vector3 *a2;
		PyObject *a2obj;
		Vector3 *a3;
		PyObject *a3obj;

		if (sipParseArgs(sipArgs,"-IIII",sipCanConvertTo_Line3,&a0obj,sipCanConvertTo_Sphere3,&a1obj,sipCanConvertTo_Vector3,&a2obj,sipCanConvertTo_Vector3,&a3obj))
		{
			bool res;

			int iserr = 0;

			sipConvertTo_Line3(a0obj,(Line3 **)&a0,1,&iserr);
			sipConvertTo_Sphere3(a1obj,(Sphere3 **)&a1,1,&iserr);
			sipConvertTo_Vector3(a2obj,&a2,1,&iserr);
			sipConvertTo_Vector3(a3obj,&a3,1,&iserr);

			if (iserr)
				return NULL;

			res = GetIntersection(* a0,* a1,* a2,* a3);

			return sipConvertFromBool((int)res);
		}
	}

	{
		const Sphere3 *a0;
		PyObject *a0obj;
		const Plane3 *a1;
		PyObject *a1obj;
		Circle3 *a2;
		PyObject *a2obj;

		if (sipParseArgs(sipArgs,"-III",sipCanConvertTo_Sphere3,&a0obj,sipCanConvertTo_Plane3,&a1obj,sipCanConvertTo_Circle3,&a2obj))
		{
			bool res;

			int iserr = 0;

			sipConvertTo_Sphere3(a0obj,(Sphere3 **)&a0,1,&iserr);
			sipConvertTo_Plane3(a1obj,(Plane3 **)&a1,1,&iserr);
			sipConvertTo_Circle3(a2obj,&a2,1,&iserr);

			if (iserr)
				return NULL;

			res = GetIntersection(* a0,* a1,* a2);

			return sipConvertFromBool((int)res);
		}
	}

	{
		const Plane3 *a0;
		PyObject *a0obj;
		const Sphere3 *a1;
		PyObject *a1obj;
		Circle3 *a2;
		PyObject *a2obj;

		if (sipParseArgs(sipArgs,"-III",sipCanConvertTo_Plane3,&a0obj,sipCanConvertTo_Sphere3,&a1obj,sipCanConvertTo_Circle3,&a2obj))
		{
			bool res;

			int iserr = 0;

			sipConvertTo_Plane3(a0obj,(Plane3 **)&a0,1,&iserr);
			sipConvertTo_Sphere3(a1obj,(Sphere3 **)&a1,1,&iserr);
			sipConvertTo_Circle3(a2obj,&a2,1,&iserr);

			if (iserr)
				return NULL;

			res = GetIntersection(* a0,* a1,* a2);

			return sipConvertFromBool((int)res);
		}
	}

	{
		const Sphere3 *a0;
		PyObject *a0obj;
		const Sphere3 *a1;
		PyObject *a1obj;
		Circle3 *a2;
		PyObject *a2obj;

		if (sipParseArgs(sipArgs,"-III",sipCanConvertTo_Sphere3,&a0obj,sipCanConvertTo_Sphere3,&a1obj,sipCanConvertTo_Circle3,&a2obj))
		{
			bool res;

			int iserr = 0;

			sipConvertTo_Sphere3(a0obj,(Sphere3 **)&a0,1,&iserr);
			sipConvertTo_Sphere3(a1obj,(Sphere3 **)&a1,1,&iserr);
			sipConvertTo_Circle3(a2obj,&a2,1,&iserr);

			if (iserr)
				return NULL;

			res = GetIntersection(* a0,* a1,* a2);

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoFunction(sipName_BALL_GetIntersection);

	return NULL;
}

static PyObject *sipDo_GetAngle(PyObject *,PyObject *sipArgs)
{

	{
		const Vector3 *a0;
		PyObject *a0obj;
		const Vector3 *a1;
		PyObject *a1obj;
		Angle *a2;
		PyObject *a2obj;

		if (sipParseArgs(sipArgs,"-III",sipCanConvertTo_Vector3,&a0obj,sipCanConvertTo_Vector3,&a1obj,sipCanConvertTo_Angle,&a2obj))
		{
			bool res;

			int iserr = 0;

			sipConvertTo_Vector3(a0obj,(Vector3 **)&a0,1,&iserr);
			sipConvertTo_Vector3(a1obj,(Vector3 **)&a1,1,&iserr);
			sipConvertTo_Angle(a2obj,&a2,1,&iserr);

			if (iserr)
				return NULL;

			res = GetAngle(* a0,* a1,* a2);

			return sipConvertFromBool((int)res);
		}
	}

	{
		const Line3 *a0;
		PyObject *a0obj;
		const Line3 *a1;
		PyObject *a1obj;
		Angle *a2;
		PyObject *a2obj;

		if (sipParseArgs(sipArgs,"-III",sipCanConvertTo_Line3,&a0obj,sipCanConvertTo_Line3,&a1obj,sipCanConvertTo_Angle,&a2obj))
		{
			bool res;

			int iserr = 0;

			sipConvertTo_Line3(a0obj,(Line3 **)&a0,1,&iserr);
			sipConvertTo_Line3(a1obj,(Line3 **)&a1,1,&iserr);
			sipConvertTo_Angle(a2obj,&a2,1,&iserr);

			if (iserr)
				return NULL;

			res = GetAngle(* a0,* a1,* a2);

			return sipConvertFromBool((int)res);
		}
	}

	{
		const Plane3 *a0;
		PyObject *a0obj;
		const Vector3 *a1;
		PyObject *a1obj;
		Angle *a2;
		PyObject *a2obj;

		if (sipParseArgs(sipArgs,"-III",sipCanConvertTo_Plane3,&a0obj,sipCanConvertTo_Vector3,&a1obj,sipCanConvertTo_Angle,&a2obj))
		{
			bool res;

			int iserr = 0;

			sipConvertTo_Plane3(a0obj,(Plane3 **)&a0,1,&iserr);
			sipConvertTo_Vector3(a1obj,(Vector3 **)&a1,1,&iserr);
			sipConvertTo_Angle(a2obj,&a2,1,&iserr);

			if (iserr)
				return NULL;

			res = GetAngle(* a0,* a1,* a2);

			return sipConvertFromBool((int)res);
		}
	}

	{
		const Vector3 *a0;
		PyObject *a0obj;
		const Plane3 *a1;
		PyObject *a1obj;
		Angle *a2;
		PyObject *a2obj;

		if (sipParseArgs(sipArgs,"-III",sipCanConvertTo_Vector3,&a0obj,sipCanConvertTo_Plane3,&a1obj,sipCanConvertTo_Angle,&a2obj))
		{
			bool res;

			int iserr = 0;

			sipConvertTo_Vector3(a0obj,(Vector3 **)&a0,1,&iserr);
			sipConvertTo_Plane3(a1obj,(Plane3 **)&a1,1,&iserr);
			sipConvertTo_Angle(a2obj,&a2,1,&iserr);

			if (iserr)
				return NULL;

			res = GetAngle(* a0,* a1,* a2);

			return sipConvertFromBool((int)res);
		}
	}

	{
		const Plane3 *a0;
		PyObject *a0obj;
		const Line3 *a1;
		PyObject *a1obj;
		Angle *a2;
		PyObject *a2obj;

		if (sipParseArgs(sipArgs,"-III",sipCanConvertTo_Plane3,&a0obj,sipCanConvertTo_Line3,&a1obj,sipCanConvertTo_Angle,&a2obj))
		{
			bool res;

			int iserr = 0;

			sipConvertTo_Plane3(a0obj,(Plane3 **)&a0,1,&iserr);
			sipConvertTo_Line3(a1obj,(Line3 **)&a1,1,&iserr);
			sipConvertTo_Angle(a2obj,&a2,1,&iserr);

			if (iserr)
				return NULL;

			res = GetAngle(* a0,* a1,* a2);

			return sipConvertFromBool((int)res);
		}
	}

	{
		const Line3 *a0;
		PyObject *a0obj;
		const Plane3 *a1;
		PyObject *a1obj;
		Angle *a2;
		PyObject *a2obj;

		if (sipParseArgs(sipArgs,"-III",sipCanConvertTo_Line3,&a0obj,sipCanConvertTo_Plane3,&a1obj,sipCanConvertTo_Angle,&a2obj))
		{
			bool res;

			int iserr = 0;

			sipConvertTo_Line3(a0obj,(Line3 **)&a0,1,&iserr);
			sipConvertTo_Plane3(a1obj,(Plane3 **)&a1,1,&iserr);
			sipConvertTo_Angle(a2obj,&a2,1,&iserr);

			if (iserr)
				return NULL;

			res = GetAngle(* a0,* a1,* a2);

			return sipConvertFromBool((int)res);
		}
	}

	{
		const Plane3 *a0;
		PyObject *a0obj;
		const Plane3 *a1;
		PyObject *a1obj;
		Angle *a2;
		PyObject *a2obj;

		if (sipParseArgs(sipArgs,"-III",sipCanConvertTo_Plane3,&a0obj,sipCanConvertTo_Plane3,&a1obj,sipCanConvertTo_Angle,&a2obj))
		{
			bool res;

			int iserr = 0;

			sipConvertTo_Plane3(a0obj,(Plane3 **)&a0,1,&iserr);
			sipConvertTo_Plane3(a1obj,(Plane3 **)&a1,1,&iserr);
			sipConvertTo_Angle(a2obj,&a2,1,&iserr);

			if (iserr)
				return NULL;

			res = GetAngle(* a0,* a1,* a2);

			return sipConvertFromBool((int)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoFunction(sipName_BALL_GetAngle);

	return NULL;
}

static PyObject *sipDo_GetDistance(PyObject *,PyObject *sipArgs)
{

	{
		const Vector3 *a0;
		PyObject *a0obj;
		const Vector3 *a1;
		PyObject *a1obj;

		if (sipParseArgs(sipArgs,"-II",sipCanConvertTo_Vector3,&a0obj,sipCanConvertTo_Vector3,&a1obj))
		{
			float res;

			int iserr = 0;

			sipConvertTo_Vector3(a0obj,(Vector3 **)&a0,1,&iserr);
			sipConvertTo_Vector3(a1obj,(Vector3 **)&a1,1,&iserr);

			if (iserr)
				return NULL;

			res = GetDistance(* a0,* a1);

			return PyFloat_FromDouble((double)res);
		}
	}

	{
		const Line3 *a0;
		PyObject *a0obj;
		const Vector3 *a1;
		PyObject *a1obj;

		if (sipParseArgs(sipArgs,"-II",sipCanConvertTo_Line3,&a0obj,sipCanConvertTo_Vector3,&a1obj))
		{
			float res;

			int iserr = 0;

			sipConvertTo_Line3(a0obj,(Line3 **)&a0,1,&iserr);
			sipConvertTo_Vector3(a1obj,(Vector3 **)&a1,1,&iserr);

			if (iserr)
				return NULL;

			res = GetDistance(* a0,* a1);

			return PyFloat_FromDouble((double)res);
		}
	}

	{
		const Vector3 *a0;
		PyObject *a0obj;
		const Line3 *a1;
		PyObject *a1obj;

		if (sipParseArgs(sipArgs,"-II",sipCanConvertTo_Vector3,&a0obj,sipCanConvertTo_Line3,&a1obj))
		{
			float res;

			int iserr = 0;

			sipConvertTo_Vector3(a0obj,(Vector3 **)&a0,1,&iserr);
			sipConvertTo_Line3(a1obj,(Line3 **)&a1,1,&iserr);

			if (iserr)
				return NULL;

			res = GetDistance(* a0,* a1);

			return PyFloat_FromDouble((double)res);
		}
	}

	{
		const Line3 *a0;
		PyObject *a0obj;
		const Line3 *a1;
		PyObject *a1obj;

		if (sipParseArgs(sipArgs,"-II",sipCanConvertTo_Line3,&a0obj,sipCanConvertTo_Line3,&a1obj))
		{
			float res;

			int iserr = 0;

			sipConvertTo_Line3(a0obj,(Line3 **)&a0,1,&iserr);
			sipConvertTo_Line3(a1obj,(Line3 **)&a1,1,&iserr);

			if (iserr)
				return NULL;

			res = GetDistance(* a0,* a1);

			return PyFloat_FromDouble((double)res);
		}
	}

	{
		const Vector3 *a0;
		PyObject *a0obj;
		const Plane3 *a1;
		PyObject *a1obj;

		if (sipParseArgs(sipArgs,"-II",sipCanConvertTo_Vector3,&a0obj,sipCanConvertTo_Plane3,&a1obj))
		{
			float res;

			int iserr = 0;

			sipConvertTo_Vector3(a0obj,(Vector3 **)&a0,1,&iserr);
			sipConvertTo_Plane3(a1obj,(Plane3 **)&a1,1,&iserr);

			if (iserr)
				return NULL;

			res = GetDistance(* a0,* a1);

			return PyFloat_FromDouble((double)res);
		}
	}

	{
		const Plane3 *a0;
		PyObject *a0obj;
		const Vector3 *a1;
		PyObject *a1obj;

		if (sipParseArgs(sipArgs,"-II",sipCanConvertTo_Plane3,&a0obj,sipCanConvertTo_Vector3,&a1obj))
		{
			float res;

			int iserr = 0;

			sipConvertTo_Plane3(a0obj,(Plane3 **)&a0,1,&iserr);
			sipConvertTo_Vector3(a1obj,(Vector3 **)&a1,1,&iserr);

			if (iserr)
				return NULL;

			res = GetDistance(* a0,* a1);

			return PyFloat_FromDouble((double)res);
		}
	}

	{
		const Line3 *a0;
		PyObject *a0obj;
		const Plane3 *a1;
		PyObject *a1obj;

		if (sipParseArgs(sipArgs,"-II",sipCanConvertTo_Line3,&a0obj,sipCanConvertTo_Plane3,&a1obj))
		{
			float res;

			int iserr = 0;

			sipConvertTo_Line3(a0obj,(Line3 **)&a0,1,&iserr);
			sipConvertTo_Plane3(a1obj,(Plane3 **)&a1,1,&iserr);

			if (iserr)
				return NULL;

			res = GetDistance(* a0,* a1);

			return PyFloat_FromDouble((double)res);
		}
	}

	{
		const Plane3 *a0;
		PyObject *a0obj;
		const Line3 *a1;
		PyObject *a1obj;

		if (sipParseArgs(sipArgs,"-II",sipCanConvertTo_Plane3,&a0obj,sipCanConvertTo_Line3,&a1obj))
		{
			float res;

			int iserr = 0;

			sipConvertTo_Plane3(a0obj,(Plane3 **)&a0,1,&iserr);
			sipConvertTo_Line3(a1obj,(Line3 **)&a1,1,&iserr);

			if (iserr)
				return NULL;

			res = GetDistance(* a0,* a1);

			return PyFloat_FromDouble((double)res);
		}
	}

	{
		const Plane3 *a0;
		PyObject *a0obj;
		const Plane3 *a1;
		PyObject *a1obj;

		if (sipParseArgs(sipArgs,"-II",sipCanConvertTo_Plane3,&a0obj,sipCanConvertTo_Plane3,&a1obj))
		{
			float res;

			int iserr = 0;

			sipConvertTo_Plane3(a0obj,(Plane3 **)&a0,1,&iserr);
			sipConvertTo_Plane3(a1obj,(Plane3 **)&a1,1,&iserr);

			if (iserr)
				return NULL;

			res = GetDistance(* a0,* a1);

			return PyFloat_FromDouble((double)res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoFunction(sipName_BALL_GetDistance);

	return NULL;
}

static PyObject *sipDo_calculateDistanceCoulomb(PyObject *,PyObject *sipArgs)
{

	{
		const BaseFragment *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"-I",sipCanConvertTo_BaseFragment,&a0obj))
		{
			double res;

			int iserr = 0;

			sipConvertTo_BaseFragment(a0obj,(BaseFragment **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			res = calculateDistanceCoulomb(* a0);

			return PyFloat_FromDouble(res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoFunction(sipName_BALL_calculateDistanceCoulomb);

	return NULL;
}

static PyObject *sipDo_calculateCoulomb(PyObject *,PyObject *sipArgs)
{

	{
		const BaseFragment *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"-I",sipCanConvertTo_BaseFragment,&a0obj))
		{
			double res;

			int iserr = 0;

			sipConvertTo_BaseFragment(a0obj,(BaseFragment **)&a0,1,&iserr);

			if (iserr)
				return NULL;

			res = calculateCoulomb(* a0);

			return PyFloat_FromDouble(res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoFunction(sipName_BALL_calculateCoulomb);

	return NULL;
}

static PyObject *sipDo_calculateACE(PyObject *,PyObject *sipArgs)
{

	{
		BaseFragment *a0;
		PyObject *a0obj;

		if (sipParseArgs(sipArgs,"-I",sipCanConvertTo_BaseFragment,&a0obj))
		{
			double res;

			int iserr = 0;

			sipConvertTo_BaseFragment(a0obj,&a0,1,&iserr);

			if (iserr)
				return NULL;

			res = calculateACE(* a0);

			return PyFloat_FromDouble(res);
		}
	}

	// Report an error if the arguments couldn't be parsed.

	sipNoFunction(sipName_BALL_calculateACE);

	return NULL;
}

static sipClassDef classesTable[] = {
	{sipName_BALL_PyWidget, sipNew_PyWidget, &sipClass_PyWidget, sipClassAttrTab_PyWidget, NULL},
	{sipName_BALL_ModularWidget, sipNew_ModularWidget, &sipClass_ModularWidget, sipClassAttrTab_ModularWidget, NULL},
	{sipName_BALL_CompositeDescriptor, sipNew_CompositeDescriptor, &sipClass_CompositeDescriptor, sipClassAttrTab_CompositeDescriptor, NULL},
	{sipName_BALL_MainControl, sipNew_MainControl, &sipClass_MainControl, sipClassAttrTab_MainControl, NULL},
	{sipName_BALL_OpenMode, sipNew_OpenMode, &sipClass_OpenMode, sipClassAttrTab_OpenMode, NULL},
	{sipName_BALL_TransformationProcessor, sipNew_TransformationProcessor, &sipClass_TransformationProcessor, sipClassAttrTab_TransformationProcessor, NULL},
	{sipName_BALL_TranslationProcessor, sipNew_TranslationProcessor, &sipClass_TranslationProcessor, sipClassAttrTab_TranslationProcessor, NULL},
	{sipName_BALL_FragmentDistanceCollector, sipNew_FragmentDistanceCollector, &sipClass_FragmentDistanceCollector, sipClassAttrTab_FragmentDistanceCollector, NULL},
	{sipName_BALL_GeometricCenterProcessor, sipNew_GeometricCenterProcessor, &sipClass_GeometricCenterProcessor, sipClassAttrTab_GeometricCenterProcessor, NULL},
	{sipName_BALL_BoundingBoxProcessor, sipNew_BoundingBoxProcessor, &sipClass_BoundingBoxProcessor, sipClassAttrTab_BoundingBoxProcessor, NULL},
	{sipName_BALL_AssignChargeProcessor, sipNew_AssignChargeProcessor, &sipClass_AssignChargeProcessor, sipClassAttrTab_AssignChargeProcessor, NULL},
	{sipName_BALL_AssignRadiusProcessor, sipNew_AssignRadiusProcessor, &sipClass_AssignRadiusProcessor, sipClassAttrTab_AssignRadiusProcessor, NULL},
	{sipName_BALL_ClearRadiusProcessor, sipNew_ClearRadiusProcessor, &sipClass_ClearRadiusProcessor, sipClassAttrTab_ClearRadiusProcessor, NULL},
	{sipName_BALL_ClearChargeProcessor, sipNew_ClearChargeProcessor, &sipClass_ClearChargeProcessor, sipClassAttrTab_ClearChargeProcessor, NULL},
	{NULL, NULL, NULL, NULL, NULL},
	{NULL, NULL, NULL, NULL, NULL},
	{NULL, NULL, NULL, NULL, NULL},
	{NULL, NULL, NULL, NULL, NULL},
	{NULL, NULL, NULL, NULL, NULL},
	{NULL, NULL, NULL, NULL, NULL},
	{NULL, NULL, NULL, NULL, NULL},
	{NULL, NULL, NULL, NULL, NULL},
	{NULL, NULL, NULL, NULL, NULL},
	{NULL, NULL, NULL, NULL, NULL},
	{NULL, NULL, NULL, NULL, NULL},
	{NULL, NULL, NULL, NULL, NULL},
	{sipName_BALL_AtomProcessor, sipNew_AtomProcessor, &sipClass_AtomProcessor, sipClassAttrTab_AtomProcessor, NULL},
	{NULL, NULL, NULL, NULL, NULL},
	{NULL, NULL, NULL, NULL, NULL},
	{sipName_BALL_EnergyMinimizer, sipNew_EnergyMinimizer, &sipClass_EnergyMinimizer, sipClassAttrTab_EnergyMinimizer, sipClassVarHierTab_EnergyMinimizer},
	{sipName_BALL_ConjugateGradientMinimizer, sipNew_ConjugateGradientMinimizer, &sipClass_ConjugateGradientMinimizer, sipClassAttrTab_ConjugateGradientMinimizer, sipClassVarHierTab_ConjugateGradientMinimizer},
	{sipName_BALL_MicroCanonicalMD, sipNew_MicroCanonicalMD, &sipClass_MicroCanonicalMD, sipClassAttrTab_MicroCanonicalMD, NULL},
	{sipName_BALL_MolecularDynamics, sipNew_MolecularDynamics, &sipClass_MolecularDynamics, sipClassAttrTab_MolecularDynamics, sipClassVarHierTab_MolecularDynamics},
	{sipName_BALL_CanonicalMD, sipNew_CanonicalMD, &sipClass_CanonicalMD, sipClassAttrTab_CanonicalMD, NULL},
	{sipName_BALL_SnapShotManager, sipNew_SnapShotManager, &sipClass_SnapShotManager, sipClassAttrTab_SnapShotManager, sipClassVarHierTab_SnapShotManager},
	{sipName_BALL_PeriodicBoundary, sipNew_PeriodicBoundary, &sipClass_PeriodicBoundary, sipClassAttrTab_PeriodicBoundary, sipClassVarHierTab_PeriodicBoundary},
	{sipName_BALL_Gradient, sipNew_Gradient, &sipClass_Gradient, sipClassAttrTab_Gradient, sipClassVarHierTab_Gradient},
	{sipName_BALL_AtomVector, sipNew_AtomVector, &sipClass_AtomVector, sipClassAttrTab_AtomVector, NULL},
	{sipName_BALL_CharmmTorsion, sipNew_CharmmTorsion, &sipClass_CharmmTorsion, sipClassAttrTab_CharmmTorsion, NULL},
	{sipName_BALL_CharmmStretch, sipNew_CharmmStretch, &sipClass_CharmmStretch, sipClassAttrTab_CharmmStretch, NULL},
	{sipName_BALL_CharmmNonBonded, sipNew_CharmmNonBonded, &sipClass_CharmmNonBonded, sipClassAttrTab_CharmmNonBonded, NULL},
	{sipName_BALL_CharmmImproperTorsion, sipNew_CharmmImproperTorsion, &sipClass_CharmmImproperTorsion, sipClassAttrTab_CharmmImproperTorsion, NULL},
	{sipName_BALL_CharmmBend, sipNew_CharmmBend, &sipClass_CharmmBend, sipClassAttrTab_CharmmBend, NULL},
	{sipName_BALL_CharmmFF, sipNew_CharmmFF, &sipClass_CharmmFF, sipClassAttrTab_CharmmFF, NULL},
	{sipName_BALL_AmberTorsion, sipNew_AmberTorsion, &sipClass_AmberTorsion, sipClassAttrTab_AmberTorsion, NULL},
	{sipName_BALL_AmberStretch, sipNew_AmberStretch, &sipClass_AmberStretch, sipClassAttrTab_AmberStretch, NULL},
	{sipName_BALL_AmberNonBonded, sipNew_AmberNonBonded, &sipClass_AmberNonBonded, sipClassAttrTab_AmberNonBonded, NULL},
	{sipName_BALL_ForceFieldComponent, sipNew_ForceFieldComponent, &sipClass_ForceFieldComponent, sipClassAttrTab_ForceFieldComponent, NULL},
	{sipName_BALL_AmberBend, sipNew_AmberBend, &sipClass_AmberBend, sipClassAttrTab_AmberBend, NULL},
	{sipName_BALL_ForceField, sipNew_ForceField, &sipClass_ForceField, sipClassAttrTab_ForceField, sipClassVarHierTab_ForceField},
	{sipName_BALL_AmberFF, sipNew_AmberFF, &sipClass_AmberFF, sipClassAttrTab_AmberFF, NULL},
	{sipName_BALL_Vector4, sipNew_Vector4, &sipClass_Vector4, sipClassAttrTab_Vector4, sipClassVarHierTab_Vector4},
	{sipName_BALL_Vector3, sipNew_Vector3, &sipClass_Vector3, sipClassAttrTab_Vector3, sipClassVarHierTab_Vector3},
	{sipName_BALL_Surface, sipNew_Surface, &sipClass_Surface, sipClassAttrTab_Surface, NULL},
	{sipName_BALL_Sphere3, sipNew_Sphere3, &sipClass_Sphere3, sipClassAttrTab_Sphere3, sipClassVarHierTab_Sphere3},
	{sipName_BALL_Quaternion, sipNew_Quaternion, &sipClass_Quaternion, sipClassAttrTab_Quaternion, sipClassVarHierTab_Quaternion},
	{sipName_BALL_Plane3, sipNew_Plane3, &sipClass_Plane3, sipClassAttrTab_Plane3, NULL},
	{sipName_BALL_Matrix4x4, sipNew_Matrix4x4, &sipClass_Matrix4x4, sipClassAttrTab_Matrix4x4, sipClassVarHierTab_Matrix4x4},
	{sipName_BALL_Line3, sipNew_Line3, &sipClass_Line3, sipClassAttrTab_Line3, sipClassVarHierTab_Line3},
	{sipName_BALL_Circle3, sipNew_Circle3, &sipClass_Circle3, sipClassAttrTab_Circle3, NULL},
	{sipName_BALL_Box3, sipNew_Box3, &sipClass_Box3, sipClassAttrTab_Box3, sipClassVarHierTab_Box3},
	{sipName_BALL_Angle, sipNew_Angle, &sipClass_Angle, sipClassAttrTab_Angle, NULL},
	{sipName_BALL_System, sipNew_System, &sipClass_System, sipClassAttrTab_System, NULL},
	{sipName_BALL_CompositeProcessor, sipNew_CompositeProcessor, &sipClass_CompositeProcessor, sipClassAttrTab_CompositeProcessor, NULL},
	{sipName_BALL_Selector, sipNew_Selector, &sipClass_Selector, sipClassAttrTab_Selector, NULL},
	{sipName_BALL_SecondaryStructure, sipNew_SecondaryStructure, &sipClass_SecondaryStructure, sipClassAttrTab_SecondaryStructure, NULL},
	{sipName_BALL_Residue, sipNew_Residue, &sipClass_Residue, sipClassAttrTab_Residue, NULL},
	{sipName_BALL_Protein, sipNew_Protein, &sipClass_Protein, sipClassAttrTab_Protein, NULL},
	{sipName_BALL_Nucleotide, sipNew_Nucleotide, &sipClass_Nucleotide, sipClassAttrTab_Nucleotide, NULL},
	{sipName_BALL_NucleicAcid, sipNew_NucleicAcid, &sipClass_NucleicAcid, sipClassAttrTab_NucleicAcid, NULL},
	{sipName_BALL_Molecule, sipNew_Molecule, &sipClass_Molecule, sipClassAttrTab_Molecule, NULL},
	{sipName_BALL_Fragment, sipNew_Fragment, &sipClass_Fragment, sipClassAttrTab_Fragment, NULL},
	{sipName_BALL_Expression, sipNew_Expression, &sipClass_Expression, sipClassAttrTab_Expression, NULL},
	{sipName_BALL_ExpressionPredicate, sipNew_ExpressionPredicate, &sipClass_ExpressionPredicate, sipClassAttrTab_ExpressionPredicate, NULL},
	{sipName_BALL_Chain, sipNew_Chain, &sipClass_Chain, sipClassAttrTab_Chain, NULL},
	{sipName_BALL_BaseFragment, sipNew_BaseFragment, &sipClass_BaseFragment, sipClassAttrTab_BaseFragment, NULL},
	{sipName_BALL_Bond, sipNew_Bond, &sipClass_Bond, sipClassAttrTab_Bond, NULL},
	{sipName_BALL_AtomType, sipNew_AtomType, &sipClass_AtomType, sipClassAttrTab_AtomType, NULL},
	{sipName_BALL_PTE_, sipNew_PTE_, &sipClass_PTE_, sipClassAttrTab_PTE_, NULL},
	{sipName_BALL_Element, sipNew_Element, &sipClass_Element, sipClassAttrTab_Element, NULL},
	{sipName_BALL_Atom, sipNew_Atom, &sipClass_Atom, sipClassAttrTab_Atom, NULL},
	{sipName_BALL_PDBAtom, sipNew_PDBAtom, &sipClass_PDBAtom, sipClassAttrTab_PDBAtom, NULL},
	{sipName_BALL_ResourceFile, sipNew_ResourceFile, &sipClass_ResourceFile, sipClassAttrTab_ResourceFile, NULL},
	{sipName_BALL_ResourceEntry, sipNew_ResourceEntry, &sipClass_ResourceEntry, sipClassAttrTab_ResourceEntry, NULL},
	{sipName_BALL_XYZFile, sipNew_XYZFile, &sipClass_XYZFile, sipClassAttrTab_XYZFile, NULL},
	{sipName_BALL_PDBFile, sipNew_PDBFile, &sipClass_PDBFile, sipClassAttrTab_PDBFile, NULL},
	{sipName_BALL_MOL2File, sipNew_MOL2File, &sipClass_MOL2File, sipClassAttrTab_MOL2File, NULL},
	{sipName_BALL_INIFile, sipNew_INIFile, &sipClass_INIFile, sipClassAttrTab_INIFile, NULL},
	{sipName_BALL_File, sipNew_File, &sipClass_File, sipClassAttrTab_File, NULL},
	{sipName_BALL_HINFile, sipNew_HINFile, &sipClass_HINFile, sipClassAttrTab_HINFile, NULL},
	{sipName_BALL_String, sipNew_String, &sipClass_String, sipClassAttrTab_String, NULL},
	{sipName_BALL_Substring, sipNew_Substring, &sipClass_Substring, sipClassAttrTab_Substring, NULL},
	{sipName_BALL_RegularExpression, sipNew_RegularExpression, &sipClass_RegularExpression, sipClassAttrTab_RegularExpression, NULL},
	{sipName_BALL_FloatPointGrid, sipNew_FloatPointGrid, &sipClass_FloatPointGrid, sipClassAttrTab_FloatPointGrid, NULL},
	{sipName_BALL_GridIndex, sipNew_GridIndex, &sipClass_GridIndex, sipClassAttrTab_GridIndex, sipClassVarHierTab_GridIndex},
	{sipName_BALL_Options, sipNew_Options, &sipClass_Options, sipClassAttrTab_Options, NULL},
	{sipName_BALL_BitVector, sipNew_BitVector, &sipClass_BitVector, sipClassAttrTab_BitVector, NULL},
	{sipName_BALL_Bit, sipNew_Bit, &sipClass_Bit, sipClassAttrTab_Bit, NULL},
	{sipName_BALL_PropertyManager, sipNew_PropertyManager, &sipClass_PropertyManager, sipClassAttrTab_PropertyManager, NULL},
	{sipName_BALL_NamedProperty, sipNew_NamedProperty, &sipClass_NamedProperty, sipClassAttrTab_NamedProperty, NULL},
	{sipName_BALL_Processor, sipNew_Processor, &sipClass_Processor, sipClassAttrTab_Processor, NULL},
	{sipName_BALL_UnaryAtomPredicate, sipNew_UnaryAtomPredicate, &sipClass_UnaryAtomPredicate, sipClassAttrTab_UnaryAtomPredicate, NULL},
	{sipName_BALL_UnaryCompositePredicate, sipNew_UnaryCompositePredicate, &sipClass_UnaryCompositePredicate, sipClassAttrTab_UnaryCompositePredicate, NULL},
	{sipName_BALL_Embeddable, sipNew_Embeddable, &sipClass_Embeddable, sipClassAttrTab_Embeddable, NULL},
	{sipName_BALL_Selectable, sipNew_Selectable, &sipClass_Selectable, sipClassAttrTab_Selectable, NULL},
	{sipName_BALL_Object, sipNew_Object, &sipClass_Object, sipClassAttrTab_Object, NULL},
	{sipName_BALL_PersistentObject, sipNew_PersistentObject, &sipClass_PersistentObject, sipClassAttrTab_PersistentObject, NULL},
	{sipName_BALL_Composite, sipNew_Composite, &sipClass_Composite, sipClassAttrTab_Composite, NULL},
	{sipName_BALL_VersionInfo, sipNew_VersionInfo, &sipClass_VersionInfo, sipClassAttrTab_VersionInfo, NULL},
	{sipName_BALL_Position, sipNew_Position, &sipClass_Position, sipClassAttrTab_Position, NULL},
	{sipName_BALL_Index, sipNew_Index, &sipClass_Index, sipClassAttrTab_Index, NULL},
	{sipName_BALL_Constant, sipNew_Constant, &sipClass_Constant, sipClassAttrTab_Constant, NULL},
};

static sipModuleDef sipModule = {
	sipName_BALL_BALL,
	112,
	classesTable
};

// The entry point into the bindings for constructors.

static PyObject *callCtor(PyObject *,PyObject *args)
{
	return sipCallCtor(&sipModule,args);
}

// The entry point into the bindings for getting class variables.

static PyObject *callGetVar(PyObject *,PyObject *args)
{
	return sipGetVar(&sipModule,args);
}

// The entry point into the bindings for setting class variables.

static PyObject *callSetVar(PyObject *,PyObject *args)
{
	return sipSetVar(&sipModule,args);
}

// Initialise the module.

static PyObject *initModule(PyObject *,PyObject *)
{
	if (sipRegisterModule(&sipModule) < 0)
		return NULL;

	// Add the global functions to the dictionary.

	static PyMethodDef globfuncs[] = {
		{sipName_BALL_calculateSASPoints, sipDo_calculateSASPoints, METH_VARARGS, NULL},
		{sipName_BALL_calculateSASAtomAreas, sipDo_calculateSASAtomAreas, METH_VARARGS, NULL},
		{sipName_BALL_calculateSASVolume, sipDo_calculateSASVolume, METH_VARARGS, NULL},
		{sipName_BALL_calculateSASArea, sipDo_calculateSASArea, METH_VARARGS, NULL},
		{sipName_BALL_calculateBondAngle, sipDo_calculateBondAngle, METH_VARARGS, NULL},
		{sipName_BALL_calculateTorsionAngle, sipDo_calculateTorsionAngle, METH_VARARGS, NULL},
		{sipName_BALL_nucleicAcids, sipDo_nucleicAcids, METH_VARARGS, NULL},
		{sipName_BALL_nucleotides, sipDo_nucleotides, METH_VARARGS, NULL},
		{sipName_BALL_proteins, sipDo_proteins, METH_VARARGS, NULL},
		{sipName_BALL_chains, sipDo_chains, METH_VARARGS, NULL},
		{sipName_BALL_secondaryStructures, sipDo_secondaryStructures, METH_VARARGS, NULL},
		{sipName_BALL_residues, sipDo_residues, METH_VARARGS, NULL},
		{sipName_BALL_molecules, sipDo_molecules, METH_VARARGS, NULL},
		{sipName_BALL_fragments, sipDo_fragments, METH_VARARGS, NULL},
		{sipName_BALL_baseFragments, sipDo_baseFragments, METH_VARARGS, NULL},
		{sipName_BALL_bonds, sipDo_bonds, METH_VARARGS, NULL},
		{sipName_BALL_PDBAtoms, sipDo_PDBAtoms, METH_VARARGS, NULL},
		{sipName_BALL_atoms, sipDo_atoms, METH_VARARGS, NULL},
		{sipName_BALL_getTorsionAngle, sipDo_getTorsionAngle, METH_VARARGS, NULL},
		{sipName_BALL_isParallel, sipDo_isParallel, METH_VARARGS, NULL},
		{sipName_BALL_isIntersecting, sipDo_isIntersecting, METH_VARARGS, NULL},
		{sipName_BALL_isOrthogonal, sipDo_isOrthogonal, METH_VARARGS, NULL},
		{sipName_BALL_isComplanar, sipDo_isComplanar, METH_VARARGS, NULL},
		{sipName_BALL_isCollinear, sipDo_isCollinear, METH_VARARGS, NULL},
		{sipName_BALL_GetIntersection, sipDo_GetIntersection, METH_VARARGS, NULL},
		{sipName_BALL_GetAngle, sipDo_GetAngle, METH_VARARGS, NULL},
		{sipName_BALL_GetDistance, sipDo_GetDistance, METH_VARARGS, NULL},
		{sipName_BALL_calculateDistanceCoulomb, sipDo_calculateDistanceCoulomb, METH_VARARGS, NULL},
		{sipName_BALL_calculateCoulomb, sipDo_calculateCoulomb, METH_VARARGS, NULL},
		{sipName_BALL_calculateACE, sipDo_calculateACE, METH_VARARGS, NULL},
		{NULL}
	};

	if (sipAddFunctions(sipModule.md_dict,globfuncs) < 0)
		return NULL;

	Py_INCREF(Py_None);
	return Py_None;
}

// Final stage after the Python classes have been created.

static PyObject *registerClasses(PyObject *,PyObject *)
{
	if (sipRegisterClasses(&sipModule,-1) < 0)
		return NULL;

	// Add the chars to the dictionary.

	static sipCharInstanceDef ResourceFilecharInstances[] = {
		{sipName_BALL_ENTRY_BEGIN, ResourceFile::ENTRY_BEGIN},
		{sipName_BALL_ENTRY_END, ResourceFile::ENTRY_END},
		{sipName_BALL_SEPARATOR, ResourceFile::SEPARATOR},
		{NULL}
	};

	if (sipAddCharInstances(((PyClassObject *)sipClass_ResourceFile) -> cl_dict,ResourceFilecharInstances) < 0)
		return NULL;

	// Add the class instances to the dictionary.

	static sipClassInstanceDef FileclassInstances[] = {
		{sipName_BALL_IN, &File::IN, sipClass_OpenMode, SIP_SIMPLE},
		{sipName_BALL_OUT, &File::OUT, sipClass_OpenMode, SIP_SIMPLE},
		{sipName_BALL_APP, &File::APP, sipClass_OpenMode, SIP_SIMPLE},
		{sipName_BALL_BINARY, &File::BINARY, sipClass_OpenMode, SIP_SIMPLE},
		{sipName_BALL_ATE, &File::ATE, sipClass_OpenMode, SIP_SIMPLE},
		{sipName_BALL_TRUNC, &File::TRUNC, sipClass_OpenMode, SIP_SIMPLE},
		NULL
	};

	if (sipAddClassInstances(((PyClassObject *)sipClass_File) -> cl_dict,FileclassInstances) < 0)
		return NULL;

	// Add the class instances to the dictionary.

	static sipClassInstanceDef RegularExpressionclassInstances[] = {
		{sipName_BALL_ALPHA, &RegularExpression::ALPHA, sipClass_String, SIP_SIMPLE},
		{sipName_BALL_ALPHANUMERIC, &RegularExpression::ALPHANUMERIC, sipClass_String, SIP_SIMPLE},
		{sipName_BALL_REAL, &RegularExpression::REAL, sipClass_String, SIP_SIMPLE},
		{sipName_BALL_IDENTIFIER, &RegularExpression::IDENTIFIER, sipClass_String, SIP_SIMPLE},
		{sipName_BALL_INTEGER, &RegularExpression::INTEGER, sipClass_String, SIP_SIMPLE},
		{sipName_BALL_HEXADECIMAL_INTEGER, &RegularExpression::HEXADECIMAL_INTEGER, sipClass_String, SIP_SIMPLE},
		{sipName_BALL_LOWERCASE, &RegularExpression::LOWERCASE, sipClass_String, SIP_SIMPLE},
		{sipName_BALL_NON_ALPHA, &RegularExpression::NON_ALPHA, sipClass_String, SIP_SIMPLE},
		{sipName_BALL_NON_ALPHANUMERIC, &RegularExpression::NON_ALPHANUMERIC, sipClass_String, SIP_SIMPLE},
		{sipName_BALL_NON_NUMERIC, &RegularExpression::NON_NUMERIC, sipClass_String, SIP_SIMPLE},
		{sipName_BALL_NON_WHITESPACE, &RegularExpression::NON_WHITESPACE, sipClass_String, SIP_SIMPLE},
		{sipName_BALL_UPPERCASE, &RegularExpression::UPPERCASE, sipClass_String, SIP_SIMPLE},
		{sipName_BALL_WHITESPACE, &RegularExpression::WHITESPACE, sipClass_String, SIP_SIMPLE},
		NULL
	};

	if (sipAddClassInstances(((PyClassObject *)sipClass_RegularExpression) -> cl_dict,RegularExpressionclassInstances) < 0)
		return NULL;

	// Add the longs to the dictionary.

	static sipLongInstanceDef OptionslongInstances[] = {
		{sipName_BALL_MAX_ENTRY_LENGTH, Options::MAX_ENTRY_LENGTH},
		{NULL}
	};

	if (sipAddLongInstances(((PyClassObject *)sipClass_Options) -> cl_dict,OptionslongInstances) < 0)
		return NULL;

	// Add the longs to the dictionary.

	static sipLongInstanceDef BitVectorlongInstances[] = {
		{sipName_BALL_BlockSize, BitVector::BlockSize},
		{NULL}
	};

	if (sipAddLongInstances(((PyClassObject *)sipClass_BitVector) -> cl_dict,BitVectorlongInstances) < 0)
		return NULL;

	// Add the doubles to the dictionary.

	static sipDoubleInstanceDef ConstantdoubleInstances[] = {
		{sipName_BALL_PI, Constant::PI},
		{sipName_BALL_E, Constant::E},
		{sipName_BALL_EPSILON, Constant::EPSILON},
		{sipName_BALL_ELEMENTARY_CHARGE, Constant::ELEMENTARY_CHARGE},
		{sipName_BALL_e0, Constant::e0},
		{sipName_BALL_ELECTRON_MASS, Constant::ELECTRON_MASS},
		{sipName_BALL_PROTON_MASS, Constant::PROTON_MASS},
		{sipName_BALL_NEUTRON_MASS, Constant::NEUTRON_MASS},
		{sipName_BALL_AVOGADRO, Constant::AVOGADRO},
		{sipName_BALL_NA, Constant::NA},
		{sipName_BALL_MOL, Constant::MOL},
		{sipName_BALL_BOLTZMANN, Constant::BOLTZMANN},
		{sipName_BALL_k, Constant::k},
		{sipName_BALL_PLANCK, Constant::PLANCK},
		{sipName_BALL_h, Constant::h},
		{sipName_BALL_GAS_CONSTANT, Constant::GAS_CONSTANT},
		{sipName_BALL_R, Constant::R},
		{sipName_BALL_FARADAY, Constant::FARADAY},
		{sipName_BALL_F, Constant::F},
		{sipName_BALL_BOHR_RADIUS, Constant::BOHR_RADIUS},
		{sipName_BALL_a0, Constant::a0},
		{sipName_BALL_VACUUM_PERMITTIVITY, Constant::VACUUM_PERMITTIVITY},
		{sipName_BALL_VACUUM_PERMEABILITY, Constant::VACUUM_PERMEABILITY},
		{sipName_BALL_SPEED_OF_LIGHT, Constant::SPEED_OF_LIGHT},
		{sipName_BALL_c, Constant::c},
		{sipName_BALL_GRAVITATIONAL_CONSTANT, Constant::GRAVITATIONAL_CONSTANT},
		{sipName_BALL_FINE_STRUCTURE_CONSTANT, Constant::FINE_STRUCTURE_CONSTANT},
		{sipName_BALL_DEG_PER_RAD, Constant::DEG_PER_RAD},
		{sipName_BALL_RAD_PER_DEG, Constant::RAD_PER_DEG},
		{sipName_BALL_MM_PER_INCH, Constant::MM_PER_INCH},
		{sipName_BALL_M_PER_FOOT, Constant::M_PER_FOOT},
		{sipName_BALL_JOULE_PER_CAL, Constant::JOULE_PER_CAL},
		{sipName_BALL_CAL_PER_JOULE, Constant::CAL_PER_JOULE},
		{NULL}
	};

	if (sipAddDoubleInstances(((PyClassObject *)sipClass_Constant) -> cl_dict,ConstantdoubleInstances) < 0)
		return NULL;

	Py_INCREF(Py_None);
	return Py_None;
}

// The Python module initialisation function.

extern "C" SIP_EXPORT void initlibBALLc()
{
	static PyMethodDef methods[] = {
		{"sipCallCtor", callCtor, METH_VARARGS, NULL},
		{"sipCallGetVar", callGetVar, METH_VARARGS, NULL},
		{"sipCallSetVar", callSetVar, METH_VARARGS, NULL},
		{"sipInitModule", initModule, METH_VARARGS, NULL},
		{"sipRegisterClasses", registerClasses, METH_VARARGS, NULL},
		{NULL}
	};

	// Initialise the module.

	Py_InitModule("libBALLc",methods);
}

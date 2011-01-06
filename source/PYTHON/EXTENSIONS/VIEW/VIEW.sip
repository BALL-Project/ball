/////////////////////////
// VIEW PYTHON SUPPORT
/////////////////////////
//
// module definition file
//

%Module VIEW

%ModuleHeaderCode
#include <BALLPyMacros.h>
%End

%Import BALL/BALLCore.sip

%Include pyDatasetVector.sip
%Include pyCompositeList.sip

// VIEW/KERNEL
%Include message.sip
%Include stage.sip
%Include mainControl.sip
%Include compositeManager.sip
%Include representationManager.sip
%Include representation.sip
%Include geometricObject.sip
%Include view-common.sip
%Include clippingPlane.sip

%If (BALL_HAS_ASIO)
%Include serverWidget.sip
%End

// VIEW/DATATYPE
%Include colorUnit.sip
%Include colorRGBA.sip
%Include colorRGBAVector.sip
%Include vertex1.sip
%Include vertex2.sip
%Include colorExtensions.sip
%Include colorMap.sip
%Include dataset.sip
%Include standardDatasets.sip

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
%Include forceModel.sip
%Include labelModel.sip

// VIEW/PRIMITIVES
%Include box.sip
%Include disc.sip
%Include gridVisualisation.sip
%Include label.sip
%Include line.sip
%Include mesh.sip
// %Include point.sip
%Include simpleBox.sip
%Include sphere.sip
%Include twoColoredLine.sip
%Include twoColoredTube.sip
%Include tube.sip

// VIEW/DIALOGS
%Include preferences.sip
%Include displayProperties.sip
%Include lightSettings.sip
%Include molecularFileDialog.sip
%Include modifyRepresentationDialog.sip

// VIEW/RENDERER
%Include renderer.sip
%Include POVRenderer.sip
%Include VRMLRenderer.sip

// VIEW/WIDGETS
%Include modularWidget.sip
%Include dockWidget.sip
%Include pyWidget.sip
%Include scene.sip
%Include molecularStructure.sip
%Include molecularControl.sip
%Include geometricControl.sip
%Include logView.sip
%Include datasetControl.sip
//%Include helpViewer.sip


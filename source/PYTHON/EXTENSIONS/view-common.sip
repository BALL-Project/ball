// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: view-common.sip,v 1.5.4.1 2005/11/11 17:21:27 leonhardt Exp $
//

%ModuleHeaderCode 
	#include <BALL/VIEW/KERNEL/common.h>
	using namespace BALL;
	using namespace BALL::VIEW;
%End

//extern bool BALL_VIEW_DOCKWINDOWS_SHOW_LABELS;

enum Command
{
	COMMAND__UNKOWN              = 0,
	COMMAND__SEND_OBJECT         = 1,
	NUMBER_OF_COMMANDS
};

enum EventsIDs
{
	SCENE_EXPORTPNG_EVENT = 60000,
	SCENE_EXPORTPOV_EVENT,
	SCENE_SETCAMERA_EVENT,
	SIMULATION_THREAD_FINISHED_EVENT,
	LOG_EVENT,
	UPDATE_COMPOSITE_EVENT
	FINISHED_REPRESENTATION_UPDATE_EVENT
};

enum DrawingMode
{
	DRAWING_MODE_DOTS = 0,
	DRAWING_MODE_WIREFRAME,
	DRAWING_MODE_SOLID
};

enum DrawingPrecision
{
	DRAWING_PRECISION_LOW = 0,
	DRAWING_PRECISION_MEDIUM,
	DRAWING_PRECISION_HIGH,
	DRAWING_PRECISION_ULTRA
};

enum ModelType
{
	MODEL_UNKNOWN = -1,
	MODEL_LINES = 0,
	MODEL_STICK,
	MODEL_BALL_AND_STICK,
	MODEL_VDW,
	MODEL_SE_SURFACE,
	MODEL_SA_SURFACE,
	MODEL_BACKBONE,
	MODEL_CARTOON,
	MODEL_HBONDS,
  MODEL_FORCES,
	MODEL_LABEL,
	MODEL_CONTOUR_SURFACE
};

enum ColoringMethod
{
	COLORING_ELEMENT          = 0,
	COLORING_RESIDUE_INDEX,
	COLORING_RESIDUE_NAME,
	COLORING_SECONDARY_STRUCTURE,
	COLORING_ATOM_CHARGE,
	COLORING_DISTANCE,
	COLORING_TEMPERATURE_FACTOR,
	COLORING_OCCUPANCY,
	COLORING_FORCES,
	COLORING_RESIDUE_TYPE,
	COLORING_CHAIN,
	COLORING_MOLECULE,
	COLORING_CUSTOM,
	COLORING_UNKNOWN
};

	enum GeometricObjectType
	{
		TYPE__UNKNOWN              = -1,
		TYPE__LINE                 = 0,
		TYPE__SPHERE               = 1,
		TYPE__TUBE                 = 2,
		TYPE__POINT                = 3,
		TYPE__MESH                 = 4,
		TYPE__BOX           			 = 5
	};

String getModelName(ModelType) throw();
String getColoringName(ColoringMethod)  throw();
bool isSurfaceModel(ModelType)	throw();
bool modelMuteableByDisplayProperties(ModelType)	throw();
bool modelMustBeRebuild(ModelType)	throw();
String getTypeName(GeometricObjectType);
GeometricObjectType getGeometricObjectType(const GeometricObject&);
String vector3ToString(const Vector3& ) throw();
bool stringToVector3(const String&, Vector3&) throw();
String createFloatString(float, Size)	throw();
MainControl* getMainControl()	throw();
String createTemporaryFilename() throw();
Vector3 getNormal(const Vector3&)	throw();

//void logString(const String& data);
//QColor chooseColor(QLabel* label);
//void processDropEvent(QDropEvent* e);
//void focusCamera(const List<Vector3>& points);
String getDataPath();



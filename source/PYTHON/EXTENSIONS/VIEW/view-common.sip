// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

%ModuleHeaderCode
	#include <QtCore/QtGlobal>
	#include <BALL/VIEW/KERNEL/common.h>
	using namespace BALL;
	using namespace BALL::VIEW;
%End


enum EventsIDs
{
	MESSAGE_EVENT = 60000,
	LOG_EVENT
};

enum DrawingMode
{
	DRAWING_MODE_DOTS = 0,
	DRAWING_MODE_WIREFRAME,
	DRAWING_MODE_SOLID,
	DRAWING_MODE_TOON
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
	MODEL_CONTOUR_SURFACE,
	MODEL_GRID_SLICE,
	MODEL_GRID_VOLUME,
	MODEL_FIELD_LINES
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

String vector3ToString(const Vector3& ) throw();
bool stringToVector3(const String&, Vector3&) throw();
String createFloatString(float, Size)	throw();
MainControl* getMainControl()	throw();
String createTemporaryFilename() throw();
Vector3 getNormal(const Vector3&)	throw();

void logString(const String&);
//QColor chooseColor(QLabel* label);
//void processDropEvent(QDropEvent* e);
//Camera focusCamera(const Vector3List)
Camera focusCamera(Composite*);



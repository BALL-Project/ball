#include <BALL/VIEW/KERNEL/common.h>

#include <BALL/VIEW/PRIMITIVES/line.h>
#include <BALL/VIEW/PRIMITIVES/sphere.h>
#include <BALL/VIEW/PRIMITIVES/tube.h>
#include <BALL/VIEW/PRIMITIVES/point.h>
#include <BALL/VIEW/PRIMITIVES/mesh.h>
#include <BALL/VIEW/PRIMITIVES/box.h>

namespace BALL
{
	namespace VIEW
	{

bool BALL_VIEW_DOCKWINDOWS_SHOW_LABELS = true;

String getModelName(ModelType type) 
	throw()
{
	switch (type)
	{
		case MODEL_LINES:
			return "Line";
		case MODEL_STICK:
			return "Stick";
		case MODEL_BALL_AND_STICK:
			return "Ball and Stick";
		case MODEL_VDW:
			return "VDW";
		case MODEL_SE_SURFACE:
			return "SES";
		case MODEL_SA_SURFACE:
			return "SAS";
		case MODEL_CONTOUR_SURFACE:
			return "Contour Surface";
		case MODEL_BACKBONE:
			return "Backbone";
		case MODEL_CARTOON:
			return "Cartoon";
		case MODEL_HBONDS:
			return "H-Bonds";
		case MODEL_FORCES:
			return "Forces";
		case MODEL_LABEL:
			return "Label";
		case MODEL_CLIPPING_PLANE:
			return "Clipping Plane";

		case MODEL_UNKNOWN:
			return "Unknown";
	}
	return "Unknown";
}

String getColoringName(ColoringMethod type) 
	throw()
{
	switch (type)
	{
		case COLORING_ELEMENT:
			return "by element";
		case COLORING_RESIDUE_INDEX:
			return "by residue index";
		case COLORING_RESIDUE_NAME:
			return "by residue name";
		case COLORING_SECONDARY_STRUCTURE:
			return "by secondary structure";
		case COLORING_ATOM_CHARGE:
			return "by atom charge";
		case COLORING_DISTANCE:
			return "by atom distance";
		case COLORING_TEMPERATURE_FACTOR:
			return "by temperature factor";
		case COLORING_OCCUPANCY:
			return "by occupancy";
		case COLORING_FORCES:
			return "by forces";
		case COLORING_CUSTOM:
			return "custom";
		case COLORING_UNKNOWN:
			return "Unknown";
	}
	return "Unknown";
}


bool isSurfaceModel(ModelType type)
	throw()
{
	if (type == MODEL_SE_SURFACE || 
			type == MODEL_SA_SURFACE ||
			type == MODEL_CONTOUR_SURFACE)
	{
		return true;
	}

	return false;
}


bool modelMustBeRebuild(ModelType type)
	throw()
{
	return (type == MODEL_SE_SURFACE ||
					type == MODEL_SA_SURFACE ||
					type == MODEL_BACKBONE 	||
					type == MODEL_FORCES    ||
					type == MODEL_BALL_AND_STICK||
					type == MODEL_CARTOON);
}


bool modelMuteableByDisplayProperties(ModelType type)
	throw()
{ 
	return type < MODEL_LABEL && type > MODEL_UNKNOWN;
}

// ===============================================
String getTypeName(GeometricObjectType type)
{
	switch(type)
	{
		case TYPE__LINE:
			return "Line";

		case TYPE__SPHERE:
			return "Sphere";
			
		case TYPE__TUBE:
			return "Tube";
			
		case TYPE__POINT:
			return "Point";
			
		case TYPE__MESH:
			return "Mesh";
			
		case TYPE__BOX:
			return "Box";

		default:
			return "unknown GeometricObject";
	}
}


GeometricObjectType getGeometricObjectType(const GeometricObject& object)
{
	if (RTTI::isKindOf<Line>(object))
	{
		return TYPE__LINE;
	}	
	if (RTTI::isKindOf<Sphere>(object))
	{
		return TYPE__SPHERE;
	}
	if (RTTI::isKindOf<Tube>(object))
	{
		return TYPE__TUBE;
	}
	if (RTTI::isKindOf<Point>(object))
	{
		return TYPE__POINT;
	}	
	if (RTTI::isKindOf<Mesh>(object))
	{
		return TYPE__MESH;
	}	
	if (RTTI::isKindOf<Box>(object))
	{
		return TYPE__BOX;
	}	

	return TYPE__UNKNOWN;
}


} } //namespaces

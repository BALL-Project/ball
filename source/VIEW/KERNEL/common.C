#include <BALL/VIEW/KERNEL/common.h>

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
					type == MODEL_CARTOON);
}


bool modelMuteableByDisplayProperties(ModelType type)
	throw()
{ 
	return type < MODEL_LABEL && type > MODEL_UNKNOWN;
}


} } //namespaces

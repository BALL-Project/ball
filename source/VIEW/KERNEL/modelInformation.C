// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/VIEW/KERNEL/modelInformation.h>
#include <BALL/VIEW/KERNEL/common.h>

using namespace std;

namespace BALL
{
	namespace VIEW
	{

		ModelInformation::ModelInformation()
		{
		}

		ModelInformation::~ModelInformation()
		{
			#ifdef BALL_VIEW_DEBUG
				Log.info() << "Destructing object " << (void *)this 
				 					 << " of class " << RTTI::getName<ModelInformation>() << std::endl;
			#endif 
		}


		String ModelInformation::getModelName(ModelType type) const
		{
			switch (type)
			{
				case MODEL_LINES:
					return (String)qApp->translate("BALL::VIEW::ModelInformation", "Line");
				case MODEL_STICK:
					return (String)qApp->translate("BALL::VIEW::ModelInformation", "Stick");
				case MODEL_BALL_AND_STICK:
					return (String)qApp->translate("BALL::VIEW::ModelInformation", "Ball and Stick");
				case MODEL_VDW:
					return (String)qApp->translate("BALL::VIEW::ModelInformation", "VDW");
				case MODEL_SE_SURFACE:
					return (String)qApp->translate("BALL::VIEW::ModelInformation", "SES");
				case MODEL_SA_SURFACE:
					return (String)qApp->translate("BALL::VIEW::ModelInformation", "SAS");
				case MODEL_CONTOUR_SURFACE:
					return (String)qApp->translate("BALL::VIEW::ModelInformation", "Contour Surface");
				case MODEL_GRID_SLICE:
					return (String)qApp->translate("BALL::VIEW::ModelInformation", "Grid Slice");
				case MODEL_GRID_VOLUME:
					return (String)qApp->translate("BALL::VIEW::ModelInformation", "Grid Volume");
				case MODEL_FIELD_LINES:
					return (String)qApp->translate("BALL::VIEW::ModelInformation", "Field Lines");
				case MODEL_BACKBONE:
					return (String)qApp->translate("BALL::VIEW::ModelInformation", "Backbone");
				case MODEL_CARTOON:
					return (String)qApp->translate("BALL::VIEW::ModelInformation", "Cartoon");
				case MODEL_RIBBON:
					return (String)qApp->translate("BALL::VIEW::ModelInformation", "Ribbon");
				case MODEL_HBONDS:
					return (String)qApp->translate("BALL::VIEW::ModelInformation", "H-Bonds");
				case MODEL_FORCES:
					return (String)qApp->translate("BALL::VIEW::ModelInformation", "Forces");
				case MODEL_LABEL:
					return (String)qApp->translate("BALL::VIEW::ModelInformation", "Label");
				case MODEL_INFORMATIONS:
					return (String)qApp->translate("BALL::VIEW::ModelInformation", "Informations");
				case MODEL_COORDINATE_SYSTEM:
					return (String)qApp->translate("BALL::VIEW::ModelInformation", "Coordinate System");
				case MODEL_PROXIMITY_LIGHT:
					return (String)qApp->translate("BALL::VIEW::ModelInformation", "Proximity Light");
				case MODEL_PLANE:
					return (String)qApp->translate("BALL::VIEW::ModelInformation", "Plane");
				case MODEL_UNKNOWN:
					return (String)qApp->translate("BALL::VIEW::ModelInformation", "Unknown");
			}
			return (String)qApp->translate("BALL::VIEW::ModelInformation", "Unknown");
		}

		String ModelInformation::getColoringName(ColoringMethod type) const
		{
			switch (type)
			{
				case COLORING_ELEMENT:
					return (String)qApp->translate("BALL::VIEW::ModelInformation", "by element");
				case COLORING_RESIDUE_INDEX:
					return (String)qApp->translate("BALL::VIEW::ModelInformation", "by residue index");
				case COLORING_RESIDUE_NAME:
					return (String)qApp->translate("BALL::VIEW::ModelInformation", "by residue name");
				case COLORING_SECONDARY_STRUCTURE:
					return (String)qApp->translate("BALL::VIEW::ModelInformation", "by secondary structure");
				case COLORING_ATOM_CHARGE:
					return (String)qApp->translate("BALL::VIEW::ModelInformation", "by atom charge");
				case COLORING_DISTANCE:
					return (String)qApp->translate("BALL::VIEW::ModelInformation", "by atom distance");
				case COLORING_TEMPERATURE_FACTOR:
					return (String)qApp->translate("BALL::VIEW::ModelInformation", "by temperature factor");
				case COLORING_OCCUPANCY:
					return (String)qApp->translate("BALL::VIEW::ModelInformation", "by occupancy");
				case COLORING_FORCES:
					return (String)qApp->translate("BALL::VIEW::ModelInformation", "by forces");
				case COLORING_RESIDUE_TYPE:
					return (String)qApp->translate("BALL::VIEW::ModelInformation", "by residue type");
				case COLORING_CHAIN:
					return (String)qApp->translate("BALL::VIEW::ModelInformation", "by chain");
				case COLORING_MOLECULE:
					return (String)qApp->translate("BALL::VIEW::ModelInformation", "by molecule");
				case COLORING_CUSTOM:
					return (String)qApp->translate("BALL::VIEW::ModelInformation", "custom");
				case COLORING_UNKNOWN:
					return (String)qApp->translate("BALL::VIEW::ModelInformation", "unknown");
			}
			return (String)qApp->translate("BALL::VIEW::ModelInformation", "unknown");
		}


		bool ModelInformation::isSurfaceModel(ModelType type) const
		{
			return type == MODEL_SE_SURFACE
			    || type == MODEL_SA_SURFACE
			    || type == MODEL_CONTOUR_SURFACE;
		}


		bool ModelInformation::modelMustBeRebuild(ModelType type) const
		{
			return (type == MODEL_SE_SURFACE ||
							type == MODEL_SA_SURFACE ||
							type == MODEL_BACKBONE 	||
							type == MODEL_RIBBON 	  ||
							type == MODEL_FORCES    ||
							type == MODEL_BALL_AND_STICK||
							type == MODEL_CARTOON   ||
							type == MODEL_LABEL);
		}

		bool ModelInformation::modelMuteableByDisplayProperties(ModelType type) const
		{ 
			return type <= MODEL_FORCES;
		}

		bool ModelInformation::coloringMuteableByDisplayProperties(ColoringMethod type) const
		{ 
			return type <= COLORING_CUSTOM;
		}

	} // namespace VIEW
} // namespace BALL

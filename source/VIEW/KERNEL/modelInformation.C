/* TRANSLATOR BALL::VIEW::ModelInformation

		Necessary for lupdate.
*/

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
					return (String)qApp->tr("Line");
				case MODEL_STICK:
					return (String)qApp->tr("Stick");
				case MODEL_BALL_AND_STICK:
					return (String)qApp->tr("Ball and Stick");
				case MODEL_VDW:
					return (String)qApp->tr("VDW");
				case MODEL_SE_SURFACE:
					return (String)qApp->tr("SES");
				case MODEL_SA_SURFACE:
					return (String)qApp->tr("SAS");
				case MODEL_CONTOUR_SURFACE:
					return (String)qApp->tr("Contour Surface");
				case MODEL_GRID_SLICE:
					return (String)qApp->tr("Grid Slice");
				case MODEL_GRID_VOLUME:
					return (String)qApp->tr("Grid Volume");
				case MODEL_FIELD_LINES:
					return (String)qApp->tr("Field Lines");
				case MODEL_BACKBONE:
					return (String)qApp->tr("Backbone");
				case MODEL_CARTOON:
					return (String)qApp->tr("Cartoon");
				case MODEL_RIBBON:
					return (String)qApp->tr("Ribbon");
				case MODEL_HBONDS:
					return (String)qApp->tr("H-Bonds");
				case MODEL_FORCES:
					return (String)qApp->tr("Forces");
				case MODEL_LABEL:
					return (String)qApp->tr("Label");
				case MODEL_INFORMATIONS:
					return (String)qApp->tr("Informations");
				case MODEL_COORDINATE_SYSTEM:
					return (String)qApp->tr("Coordinate System");
				case MODEL_UNKNOWN:
					return (String)qApp->tr("Unknown");
			}
			return (String)qApp->tr("Unknown");
		}

		String ModelInformation::getColoringName(ColoringMethod type) const
		{
			switch (type)
			{
				case COLORING_ELEMENT:
					return (String)qApp->tr("by element");
				case COLORING_RESIDUE_INDEX:
					return (String)qApp->tr("by residue index");
				case COLORING_RESIDUE_NAME:
					return (String)qApp->tr("by residue name");
				case COLORING_SECONDARY_STRUCTURE:
					return (String)qApp->tr("by secondary structure");
				case COLORING_ATOM_CHARGE:
					return (String)qApp->tr("by atom charge");
				case COLORING_DISTANCE:
					return (String)qApp->tr("by atom distance");
				case COLORING_TEMPERATURE_FACTOR:
					return (String)qApp->tr("by temperature factor");
				case COLORING_OCCUPANCY:
					return (String)qApp->tr("by occupancy");
				case COLORING_FORCES:
					return (String)qApp->tr("by forces");
				case COLORING_RESIDUE_TYPE:
					return (String)qApp->tr("by residue type");
				case COLORING_CHAIN:
					return (String)qApp->tr("by chain");
				case COLORING_MOLECULE:
					return (String)qApp->tr("by molecule");
				case COLORING_CUSTOM:
					return (String)qApp->tr("custom");
				case COLORING_UNKNOWN:
					return (String)qApp->tr("unknown");
			}
			return (String)qApp->tr("unknown");
		}


		bool ModelInformation::isSurfaceModel(ModelType type) const
		{
			if (type == MODEL_SE_SURFACE || 
					type == MODEL_SA_SURFACE ||
					type == MODEL_CONTOUR_SURFACE)
			{
				return true;
			}

			return false;
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

// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: modelInformation.C,v 1.1.4.2 2007/04/04 15:02:48 amoll Exp $
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
				case MODEL_GRID_SLICE:
					return "Grid Slice";
				case MODEL_GRID_VOLUME:
					return "Grid Volume";
				case MODEL_FIELD_LINES:
					return "Field Lines";
				case MODEL_BACKBONE:
					return "Backbone";
				case MODEL_CARTOON:
					return "Cartoon";
				case MODEL_RIBBON:
					return "Ribbon";
				case MODEL_HBONDS:
					return "H-Bonds";
				case MODEL_FORCES:
					return "Forces";
				case MODEL_LABEL:
					return "Label";
				case MODEL_INFORMATIONS:
					return "Informations";
				case MODEL_COORDINATE_SYSTEM:
					return "Coordinate System";
				case MODEL_PROXIMITY_LIGHT:
					return "Proximity Light";
				case MODEL_PLANE:
					return "Plane";
				case MODEL_UNKNOWN:
					return "Unknown";
			}
			return "Unknown";
		}

		String ModelInformation::getColoringName(ColoringMethod type) const
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
				case COLORING_RESIDUE_TYPE:
					return "by residue type";
				case COLORING_CHAIN:
					return "by chain";
				case COLORING_MOLECULE:
					return "by molecule";
				case COLORING_CUSTOM:
					return "custom";
				case COLORING_UNKNOWN:
					return "unknown";
			}
			return "unknown";
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

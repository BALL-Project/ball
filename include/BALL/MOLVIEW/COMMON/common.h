// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: common.h,v 1.21 2003/08/26 08:04:29 oliver Exp $
//

#ifndef BALL_MOLVIEW_COMMON_COMMON_H
#define BALL_MOLVIEW_COMMON_COMMON_H

#define BALL_MOLVIEW_PRINT_PROPERTY(property) \
	(hasProperty(property) ? "yes" : "no")

namespace BALL
{
	namespace MOLVIEW
	{
			/** Predefined properties.
					Enumeration of all properties that are used by the MOLVIEW kernel.
					This properties define the models available in MOLVIEW.
			*/
			enum ModelTypes
			{
				/// unkown property
				MODEL_UNKNOWN = -1,
				
				/// defines the property for the model: Lines
				MODEL_LINES = 0,

				/// defines the property for the model: Stick
				MODEL_STICK,

				/// defines the property for the model: Ball and Stick
				MODEL_BALL_AND_STICK,
				
				/// defines the property for the model: Van der Waals
				MODEL_VDW,

				/// defines the property for the model: Surface
				MODEL_SURFACE,

				/// defines the property for the model: Backbone
				MODEL_BACKBONE,

				/// defines the property for the model: Cartoon
				MODEL_CARTOON,
				
				/// defines the property for the model: Label
				MODEL_LABEL

			};


			/// Coloring Methods
			enum ColoringMethod
			{
				///
				COLORING_ELEMENT          = 0,

				///
				COLORING_RESIDUE_TYPE,

				///
				COLORING_RESIDUE_NAME,

				///
				COLORING_ATOM_CHARGE,

				///
				COLORING_DISTANCE,

				///
				COLORING_CUSTOM
			};

	} // namespace MOLVIEW

} // namespace BALL

#endif // BALL_MOLVIEW_COMMON_COMMON_H

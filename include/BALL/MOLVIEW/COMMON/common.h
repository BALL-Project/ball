// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: common.h,v 1.20 2003/07/21 07:40:22 amoll Exp $

#ifndef BALL_MOLVIEW_COMMON_COMMON_H
#define BALL_MOLVIEW_COMMON_COMMON_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_VIEW_KERNEL_GEOMETRICOBJECT_H
#	include <BALL/VIEW/KERNEL/geometricObject.h>
#endif

#define BALL_MOLVIEW_PRINT_PROPERTY(property) \
	(hasProperty(property) ? "yes" : "no")

using BALL::VIEW::GeometricObject;

namespace BALL
{
	namespace MOLVIEW
	{
		//		struct GeometricObject
		//		{
			/** Predefined properties.
					Enumeration of all properties that are used by the MOLVIEW kernel.
					This properties define the models available in MOLVIEW.
					This enumeration is an extension of the property in the  \link GeometricObject GeometricObject \endlink 
					class.
    
					\ingroup  MolviewCommon
			*/
			enum Properties
			{
				/// unkown property
				PROPERTY__UNKNOWN = -1,
				
				/// defines the number of the first model available (the next free number).
				PROPERTY__FIRST_MODEL            = GeometricObject::NUMBER_OF_PROPERTIES,

				/// defines the property for the model: Van der Waals
				PROPERTY__MODEL_VDW              = PROPERTY__FIRST_MODEL,

				/// defines the property for the model: Dots
				PROPERTY__MODEL_DOTS,

				/// defines the property for the model: Ball and Stick
				PROPERTY__MODEL_BALL_AND_STICK,

				/// defines the property for the model: Lines
				PROPERTY__MODEL_LINES,

				/// defines the number of the last model available.
				PROPERTY__LAST_MODEL             = PROPERTY__MODEL_LINES,

				/// defines the start of the next free properties.
				NUMBER_OF_PROPERTIES
			};
			//		};
	} // namespace MOLVIEW

} // namespace BALL

#endif // BALL_MOLVIEW_COMMON_COMMON_H

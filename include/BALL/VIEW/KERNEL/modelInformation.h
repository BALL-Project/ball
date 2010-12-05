// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_VIEW_KERNEL_MODELINFORMATION_H
#define BALL_VIEW_KERNEL_MODELINFORMATION_H

#ifndef BALL_VIEW_KERNEL_COMMON_H
# include <BALL/VIEW/KERNEL/common.h>
#endif

#include <QtCore/QCoreApplication>

namespace BALL
{
	namespace VIEW
	{
		/** ModelInformation class.
				This class is used to retrieve names, types and different properties 
				for the different models and coloring methods.
				It can be used to inform the VIEW libraries classes of additional model types,
				that are declared outside of the library itself.
				To do so, call MainControl::setModelInformation().
				\ingroup ViewKernelOther
		*/
		class BALL_VIEW_EXPORT ModelInformation
		{

			public:
			

			//@{

			/** Default Constructor.
					Calls clear.
			*/
			ModelInformation();

			/** Destructor.
			*/
			virtual ~ModelInformation();

			//@} 
			/**	@name	Accessors: inspectors and mutators */ 
			//@{
		
			/** Get a name for a ModelType
			*/
			virtual String getModelName(ModelType type) const;

			/** Get a name for a ColoringMethod
			*/
			virtual String getColoringName(ColoringMethod type) const;

			/** Define, which Models are Surfaces.
					Add new kinds of Surfaces to this function!
			*/
			virtual bool isSurfaceModel(ModelType type) const;

			/** Model can be modified with DisplayProperitesDialog
			*/
			virtual bool modelMuteableByDisplayProperties(ModelType type) const;

			/** Model can be modified with DisplayProperitesDialog
			*/
			virtual bool coloringMuteableByDisplayProperties(ColoringMethod type) const;

			/** Model must be rebuild, if Composite changes
					but hierarchy is unchanged.
			*/
			virtual bool modelMustBeRebuild(ModelType type) const;
		};

	} // namespace VIEW
} // namespace BALL

#endif // BALL_VIEW_KERNEL_MODELINFORMATION_H

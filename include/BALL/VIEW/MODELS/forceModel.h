// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: forceModel.h,v 1.1 2004/07/08 16:49:36 amoll Exp $
//

#ifndef BALL_VIEW_MODELS_FORCEMODEL_H
#define BALL_VIEW_MODELS_FORCEMODEL_H

#ifndef BALL_VIEW_MODELS_MODELPROCESSOR_H
# include <BALL/VIEW/MODELS/modelProcessor.h>
#endif

namespace BALL
{
	namespace VIEW
	{

		/** ForceModel class.
				The class ForceModel is a model processor that is responsible
				for creating a model for forces on atoms.
				For information about the processor concept see Processor.
				\ingroup  ViewModels
		*/
		class BALL_EXPORT ForceModel: public ModelProcessor
		{
			public:

			/**	@name	Constructors and Destructors
			*/	
			//@{

			/** Default Constructor.
			*/
			ForceModel()
				throw();

			/** Copy constructor.
			*/
			ForceModel(const ForceModel& add_line_model)
				throw();

			/** Destructor.
			*/
			virtual ~ForceModel()
				throw();

			//@} 
			/**	@name Processor specific methods 
			*/ 
			//@{

			/**	Operator method.
					This method iterates over each Composite object reachable in the tree. 
					If the composite is of kind Atom than a Point
					is created for that atom, and it inserted with the method insertAtom_().
					The color for that Point object is calculated with the ColorCalculator
					object retrieved with the method getColorCalculator().
					All atoms inserted with the method insertAtom_() will later used for creating
					the model of the reachable Bond objects. Those models will be created
					with the method buildBondModels_().
					\param  composite the Composite object that will be processed
					\return Processor::Result the result 
					@exception OutOfMemory thrown if the memory allocation for a Point object failed
			*/
			virtual Processor::Result operator() (Composite& composite);
						
			//@}

			protected:
		};

	} // namespace VIEW
} // namspace BALL

#endif // BALL_VIEW_MODELS_FORCEMODEL_H

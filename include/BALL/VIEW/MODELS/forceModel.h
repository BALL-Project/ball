// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: forceModel.h,v 1.5.18.1 2007/03/25 21:26:06 oliver Exp $
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
		class BALL_VIEW_EXPORT ForceModel: public ModelProcessor
		{
			public:

			BALL_CREATE(ForceModel)

			/**	@name	Constructors and Destructors
			*/	
			//@{

			/** Default Constructor.
			*/
			ForceModel();

			/** Copy constructor.
			*/
			ForceModel(const ForceModel& add_line_model);

			/** Destructor.
			*/
			virtual ~ForceModel();

			///
			void setScaling(float value)
				{ scaling_ = value;}

			///
			void setMaxLength(float value)
				{ max_length_ = value;}

			///
			float getScaling() const
				{ return scaling_;}

			///
			float getMaxLength() const
				{ return max_length_;}

			///
			void setBaseSize(float size) { base_size_ = size;}

			///
			float getBaseSize() const { return base_size_;}

			///
			void setOffset(float off) { offset_ = off;}

			///
			float getOffset() const { return offset_;}

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

			float scaling_;
			float max_length_;
			float base_size_;
			float offset_;
		};

	} // namespace VIEW
} // namspace BALL

#endif // BALL_VIEW_MODELS_FORCEMODEL_H

// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: vanDerWaalsModel.h,v 1.12 2005/12/23 17:02:19 amoll Exp $
//

#ifndef BALL_VIEW_MODELS_VANDERWAALSMODEL_H
#define BALL_VIEW_MODELS_VANDERWAALSMODEL_H

#ifndef BALL_VIEW_MODELS_ATOMBONDMODELBASEPROCESSOR_H
# include <BALL/VIEW/MODELS/atomBondModelBaseProcessor.h>
#endif

namespace BALL
{
	namespace VIEW
	{
		/** AddVanDerWaalsModel class.
				AddVanDerWaalsModel is a model processor that is responsible
				for creating a <b>Van der Waals</b> model.
				For information about the processor concept see Processor.
				\ingroup  ViewModels
		*/
		class BALL_VIEW_EXPORT AddVanDerWaalsModel: public AtomBondModelBaseProcessor
		{
			public:

			BALL_CREATE(AddVanDerWaalsModel)

			/**	@name	Constructors and Destructor
			*/	
			//@{

			/** Default Constructor.
			*/
			AddVanDerWaalsModel();

			/** Copy constructor.
			*/
			AddVanDerWaalsModel(const AddVanDerWaalsModel& add_van_der_waals_model);

			/** Destructor.
			*/
			virtual ~AddVanDerWaalsModel();

			//@}
			/**	@name Processor specific methods 
			*/ 
			//@{

			/**	Operator method.
					This method iterates over each Composite object reachable in the 
					Composite tree. If the Composite is of kind Atom, than a Sphere
					with radius getVanDerWaalsRadius() is created for that atom, 
					and the atom is appended with the method insertAtom_().
					The color for that Sphere object is calculated with the ColorCalculator
					object retrieved with the method getColorCalculator().
					All atoms inserted with the method insertAtom_() will later be used for creating
					the model of the reachable Bond objects. 
					Those models will be created with the method buildBondModels_().
					\param  composite the Composite object that will be processed
					\return Processor::Result the result 
					@exception OutOfMemory thrown if the memory allocation for a Sphere failed
			*/
			virtual Processor::Result operator() (Composite& composite);

			//@} 
			/**	@name	debuggers and diagnostics 
			*/ 
			//@{

			/** Internal value dump.
					Dump the current state to the output ostream with a given dumping depth.
					Calls AtomBondModelBaseProcessor::dump.
					\param   s output stream where to output the state 
					\param   depth the dumping depth
			*/
			virtual void dump(std::ostream& s = std::cout, Size depth = 0) const;

			/// Default = 1
			void setVDWRadiusFactor(float value)
				{ radius_factor_ = value;}

			///
			float getVDWRadiusFactor() const
				{ return 	radius_factor_;}
							
			//@}
			
			protected:
			float radius_factor_;
		};

	} // namespace VIEW
} // namespace BALL

#endif // BALL_VIEW_MODELS_VANDERWAALSMODEL_H

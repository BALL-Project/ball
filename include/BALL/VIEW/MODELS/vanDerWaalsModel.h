// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: vanDerWaalsModel.h,v 1.2 2003/08/26 18:35:34 amoll Exp $
//

#ifndef BALL_VIEW_MODELS_VANDERWAALSMODEL_H
#define BALL_VIEW_MODELS_VANDERWAALSMODEL_H

#ifndef BALL_VIEW_PRIMITIVES_SPHERE_H
#	include <BALL/VIEW/PRIMITIVES/sphere.h>
#endif

#ifndef BALL_VIEW_MODELS_ATOMBONDMODELBASEPROCESSOR_H
# include <BALL/VIEW/MODELS/atomBondModelBaseProcessor.h>
#endif

namespace BALL
{
	namespace VIEW
	{
		using VIEW::Sphere;

		/** AddVanDerWaalsModel class.
				AddVanDerWaalsModel is a model processor that is responsible
				for creating a <b>Van der Waals</b> model.
				For information about the processor concept see Processor.
				\ingroup  MolviewModels
		*/
		class AddVanDerWaalsModel: public AtomBondModelBaseProcessor
		{
			public:

			/**	@name	Constructors and Destructor
			*/	
			//@{

			/** Default Constructor.
			*/
			AddVanDerWaalsModel()
				throw();

			/** Copy constructor.
			*/
			AddVanDerWaalsModel(const AddVanDerWaalsModel& add_van_der_waals_model)
				throw();

			/** Destructor.
			*/
			virtual ~AddVanDerWaalsModel()
				throw();

			/** Explicit default initialization.
					Calls AtomBondModelBaseProcessor::clear.
			*/
			virtual void clear()
				throw();

			//@}
			/**	@name Processor specific methods 
			*/ 
			//@{

			/** Start method.
					Calls AtomBondModelBaseProcessor::start.
					\return bool true if the start of was successful
			*/
			virtual bool start();
			
			/** Finish method.
					This method will be internally called from the processor mechanism if the processor
					has finished processing the Composite tree.
					All previously inserted Atom objects (inserted with the method insertAtom_())
					will be processed with the method buildBondModels_() to create the graphical 
					representation of the Bond objects.
					\return bool true if the finish was successful
			*/
			virtual bool finish();

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
					@exeception OutOfMemory thrown if the memory allocation for a Sphere failed
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
			virtual void dump(std::ostream& s = std::cout, Size depth = 0) const
				throw();
							
			//@}
		};

	} // namespace VIEW

} // namespace BALL

#endif // BALL_VIEW_MODELS_VANDERWAALSMODEL_H

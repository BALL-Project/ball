// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: lineModel.h,v 1.17 2003/08/26 08:04:35 oliver Exp $
//

#ifndef BALL_MOLVIEW_FUNCTOR_LINEMODEL_H
#define BALL_MOLVIEW_FUNCTOR_LINEMODEL_H

#ifndef BALL_MOLVIEW_FUNCTOR_ATOMBONDMODELBASEPROCESSOR_H
#	include <BALL/MOLVIEW/FUNCTOR/atomBondModelBaseProcessor.h>
#endif

namespace BALL
{
	namespace MOLVIEW
	{

		/** AddLineModel class.
				The class AddLineModel is a model processor that is responsible
				for creating a line model.
				For information about the processor concept see Processor.
				\ingroup  MolviewFunctorsModels
		*/
		class AddLineModel: public AtomBondModelBaseProcessor
		{
			public:

			/**	@name	Constructors and Destructors
			*/	
			//@{

			/** Default Constructor.
			*/
			AddLineModel()
				throw();

			/** Copy constructor.
			*/
			AddLineModel(const AddLineModel& add_line_model)
				throw();

			/** Destructor.
			*/
			virtual ~AddLineModel()
				throw();

			//@} 
			/**	@name Processor specific methods 
			*/ 
			//@{

			/** Start method.
					Initialize the properties of the BaseModelConnector to this AddLineModel.
					Calls AtomBondModelBaseProcessor::start.
					\return bool true if the start was successful
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
					@exeception OutOfMemory thrown if the memory allocation for a Point object failed
			*/
			virtual Processor::Result operator() (Composite& composite);
						
			//@} 
			/**	@name	debuggers and diagnostics 
			*/ 
			//@{

			/** Internal value dump.
					Dump the current state of this AddLineModel to 
					the output ostream <b>s</b> with dumping depth <b>depth</b>.
					Calls AtomBondModelBaseProcessor::dump.
					\param   s output stream where to output the state 
					\param   depth the dumping depth
					\see     AtomBondModelBaseProcessor
			*/
			virtual void dump(std::ostream& s = std::cout, Size depth = 0) const
				throw();
							
			//@}

		};

	} // namespace MOLVIEW

} // namspace BALL

#endif // BALL_MOLVIEW_FUNCTOR_LINEMODEL_H

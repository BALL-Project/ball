// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: HBondModel.h,v 1.4 2004/06/07 10:17:16 amoll Exp $
//

#ifndef BALL_VIEW_MODELS_HBONDMODELPROCESSOR_H
#define BALL_VIEW_MODELS_HBONDMODELPROCESSOR_H

#ifndef BALL_VIEW_MODELS_ATOMBONDMODELBASEPROCESSOR_H
#	include <BALL/VIEW/MODELS/atomBondModelBaseProcessor.h>
#endif

namespace BALL
{
	class Atom;

	namespace VIEW
	{
		/** HBondModelProcessor class.
				\ingroup  ViewModels
		*/
		class BALL_EXPORT HBondModelProcessor: public AtomBondModelBaseProcessor
		{
			public:

			/**	@name	Constructors and Destructors
			*/	
			//@{

			/** Default Constructor.
			*/
			HBondModelProcessor()
				throw();

			/** Copy constructor.
			*/
			HBondModelProcessor(const HBondModelProcessor& model)
				throw();

			/** Destructor.
			*/
			virtual ~HBondModelProcessor()
				throw();

			/** Explicit default initialization.
					Calls AtomBondModelBaseProcessor::clear.
			*/
			virtual void clear()
				throw();

			//@} 
			/**	@name	Assignment 
			*/ 
			//@{

			/** Assignment.
					Calls AtomBondModelBaseProcessor::set.
			*/
			void set(const HBondModelProcessor& model)
				throw();

			/** Assignment operator.
					Calls set().
					\param       model the HBondModelProcessor to be copied
			*/
			const HBondModelProcessor& operator = (const HBondModelProcessor& model)
				throw();

			/** Swapping of HBondModelProcessor's.
					Calls AtomBondModelBaseProcessor::swap
			*/
			void swap(HBondModelProcessor& model)
				throw();

			//@}
			/**	@name Processor specific methods 
			*/ 
			//@{

			/**	Operator method.
					This method iterates over each Composite object reachable in the 
					Composite tree. If the composite is of kind Atom than a Sphere
					is created for that atom, and the atom is inserted with 
					the method insertAtom_().
			*/
			virtual Processor::Result operator() (Composite& composite);

			///
			void setRadius(float radius)
				throw() { radius_ = radius;}

			///
			float getRadius() const
				throw() { return radius_;}
			
			protected:

			float radius_;

			//@} 
		};

	} // namespace VIEW
} // namespace BALL

#endif // BALL_VIEW_MODELS_HBONDMODELPROCESSOR_H

// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: HBondModel.h,v 1.8 2005/12/23 17:02:16 amoll Exp $
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
		class BALL_VIEW_EXPORT HBondModelProcessor: public AtomBondModelBaseProcessor
		{
			public:

			BALL_CREATE(HBondModelProcessor)

			/**	@name	Constructors and Destructors
			*/	
			//@{

			/** Default Constructor.
			*/
			HBondModelProcessor();

			/** Copy constructor.
			*/
			HBondModelProcessor(const HBondModelProcessor& model);

			/** Destructor.
			*/
			virtual ~HBondModelProcessor();

			/** Explicit default initialization.
					Calls AtomBondModelBaseProcessor::clear.
			*/
			virtual void clear();

			//@} 
			/**	@name	Assignment 
			*/ 
			//@{

			/** Assignment.
					Calls AtomBondModelBaseProcessor::set.
			*/
			void set(const HBondModelProcessor& model);

			/** Assignment operator.
					Calls set().
					\param       model the HBondModelProcessor to be copied
			*/
			const HBondModelProcessor& operator = (const HBondModelProcessor& model);
			
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
				{ radius_ = radius;}

			///
			float getRadius() const
				{ return radius_;}
			
			protected:

			float radius_;

			//@} 
		};

	} // namespace VIEW
} // namespace BALL

#endif // BALL_VIEW_MODELS_HBONDMODELPROCESSOR_H

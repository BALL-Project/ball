// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: objectSelector.h,v 1.5 2002/12/12 09:48:48 oliver Exp $

#ifndef BALL_MOLVIEW_FUNCTOR_OBJECTSELECTOR_H
#define BALL_MOLVIEW_FUNCTOR_OBJECTSELECTOR_H

#ifndef BALL_MOLVIEW_FUNCTOR_ATOMBONDMODELBASEPROCESSOR_H
#	include <BALL/MOLVIEW/FUNCTOR/atomBondModelBaseProcessor.h>
#endif

namespace BALL
{
	namespace MOLVIEW
	{
		/** ObjectSelector class.
				The class ObjectSelector is responsible for selecting
				\Ref{GeometricObject} objects. The method \Ref{Selectable::select} or 
				\Ref{Selectable::deselect} will be called from each processed \Ref{Composite} object
				according to the state of {\em *this} objectSelector.
				See \Ref{Selectable} for information about selecting or deselecting objects.
				for information about the processor concept see \Ref{Processor} in tbe BALL 
				documentation. \\
				{\bf Definition:} \URL{BALL/MOLVIEW/FUNCTOR/objectSelector.h}
				@see  Selectable
		*/
		class ObjectSelector: public UnaryProcessor<Composite>
		{
			public:

			/**	@name	Constructors
			*/	
			//@{

			/** Default Constructor.
			*/
			ObjectSelector()
				throw();

			//@}
			/** @name Destructors 
			*/
			//@{

			/** Destructor.
			*/
			virtual ~ObjectSelector()
				throw();

			//@}
			/**	@name Processor specific methods
			*/
			//@{

			/**	Operator method.
					This method iterates over each \Ref{Composite} object reachable in the 
					\Ref{Composite} tree.
			*/
			virtual Processor::Result operator() (Composite& composite)
				throw();

			//@}
		};

		/** Deselector class
		 		This class behaves in the same way as its parent class ObjectSelector, but
				it deselects atoms and bonds.
		*/
		class ObjectDeselector : public ObjectSelector
		{
			protected:
			virtual Processor::Result operator() (Composite& composite)
				throw();
		};
		
	} // namespace MOLVIEW
} // namespace BALL

#endif // BALL_MOLVIEW_FUNCTOR_OBJECTSELECTOR_H

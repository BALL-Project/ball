// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: objectSelector.h,v 1.10 2003/03/26 13:56:48 anhi Exp $

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
				 \link GeometricObject GeometricObject \endlink  objects. The method  \link Selectable::select Selectable::select \endlink  or 
				 \link Selectable::deselect Selectable::deselect \endlink  will be called from each processed  \link Composite Composite \endlink  object
				according to the state of {\em *this} objectSelector.
				See  \link Selectable Selectable \endlink  for information about selecting or deselecting objects.
				for information about the processor concept see  \link Processor Processor \endlink  in tbe BALL 
				documentation.  \par
				
				@see  Selectable
    		
				\ingroup  MolviewFunctorsModels
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
					This method iterates over each  \link Composite Composite \endlink  object reachable in the 
					 \link Composite Composite \endlink  tree.
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

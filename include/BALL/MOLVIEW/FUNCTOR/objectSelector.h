// $Id: objectSelector.h,v 1.2 2001/07/15 18:49:18 oliver Exp $

#ifndef BALL_MOLVIEW_FUNCTOR_OBJECTSELECTOR_H
#define BALL_MOLVIEW_FUNCTOR_OBJECTSELECTOR_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_KERNEL_ATOM_H
#	include <BALL/KERNEL/atom.h>
#endif

#ifndef BALL_KERNEL_BOND_H
#	include <BALL/KERNEL/bond.h>
#endif

#ifndef BALL_KERNEL_FOREACH_H
#	include <BALL/KERNEL/forEach.h>
#endif

#ifndef BALL_VIEW_DATATYPE_COLOR_H
#	include <BALL/VIEW/DATATYPE/color.h>
#endif

#ifndef BALL_VIEW_KERNEL_GEOMETRICOBJECT_H
#	include <BALL/VIEW/KERNEL/geometricObject.h>
#endif

#ifndef BALL_MOLVIEW_COMMON_COMMON_H
#	include <BALL/MOLVIEW/COMMON/common.h>
#endif

#ifndef BALL_MOLVIEW_FUNCTOR_ATOMBONDMODELBASEPROCESSOR_H
#	include <BALL/MOLVIEW/FUNCTOR/atomBondModelBaseProcessor.h>
#endif


//using namespace BALL;
//using namespace BALL::VIEW;


namespace BALL
{
 	using namespace VIEW;

	namespace MOLVIEW
	{
		/** ObjectSelector class.
				{\bf Framework:} BALL/MOLVIEW/FUNCTOR\\
				{\bf Definition:} \URL{BALL/MOLVIEW/FUNCTOR/objectSelector.h}\\ \\
				The class ObjectSelector is responsible for selecting or deselecting
				\Ref{Composite} objects. The method \Ref{Selectable::select} or 
				\Ref{Selectable::deselect} will be called from each processed \Ref{Composite} object
				according to the state of {\em *this} objectSelector.
				If \Ref{GeometricObject} objects are found their color will be set to the 
				selected color if they are selected.
				See \Ref{Selectable} for information about selecting or deselecting objects.
				For information about the processor concept see \Ref{Processor} in tbe BALL
				documentation.
				@see  Selectable
				@memo    ObjectSelector class (BALL MOLVIEW functor framework)
				@author  $Author: oliver $
				@version $Revision: 1.2 $
				@date    $Date: 2001/07/15 18:49:18 $
		*/
		class ObjectSelector: public AtomBondModelBaseProcessor
		{
			public:

			/**	@name	Constructors
			*/	
			//@{

			/** Default Constructor.
					Construct new objectSelector. Set the state of {\em *this} 
					objectSelector to: select objects. See concept of
					\Ref{Selectable} for information concerning selection of objects.
					@return      ObjectSelector new constructed objectSelector
					@see         AtomBondModelBaseProcessor
					@see         Selectable
			*/
			ObjectSelector()
				throw();

			/** Copy constructor.
					Construct new objectSelector by copying the objectSelector
					{\em selector}.
					@param       selector the objectSelector to be copied
					@param       deep make a deep (={\tt true}) or shallow (={\tt false}) copy of {\em selector}
					@return      ObjectSelector new constructed objectSelector copied from {\em selector}
					@see         AtomBondModelBaseProcessor
			*/
			ObjectSelector
				(const ObjectSelector& selector, bool deep = true)
				throw();

			//@}

			/** @name Destructors 
			*/
			//@{

			/** Destructor.
					Default destruction of {\em *this} objectSelector.
					Calls \Ref{destroy}.
					@see         destroy
			*/
			virtual ~ObjectSelector()
				throw();

			/** Explicit default initialization.
					Set state of {\em *this} objectSelector to: select objects.
					See concept of \Ref{Selectable} for information concerning selection 
					of objects.
					Calls \Ref{AtomBondModelBaseProcessor::clear}.
					@see  AtomBondModelBaseProcessor
					@see  Selectable
			*/
			virtual void clear()
				throw();

			/** Explicit destructor.
					Empty for further purpose.
			*/
			virtual void destroy()
				throw();
			//@}


			/**	@name	Assignment
			*/
			//@{

			/** Assignment.
					Assign the objectSelector {\em selector} to {\em *this} 
					objectSelector.
					Initializes the \Ref{AtomBondModelBaseProcessor} of {\em *this}
					objectSelector to that of {\em selector}.
					Calls \Ref{AtomBondModelBaseProcessor::set}.
					@param       selector the objectSelector to be copied 
					@param       deep make a deep (={\tt true}) or shallow (={\tt false}) copy of {\em selector}
					@see         AtomBondModelBaseProcessor
			*/
			void set(const ObjectSelector& selector, bool deep = true)
				throw();

			/** Assignment operator.
					Assign the objectSelector {\em selector} to {\em *this} 
					objectSelector.
					Initializes the \Ref{AtomBondModelBaseProcessor} of {\em *this} 
					objectSelector	to that of {\em selector}.
					Calls \Ref{set}.
					The assignment will be deep.
					@param       selector the objectSelector to be copied
					@return      ObjectSelector& constant reference of {\em *this} objectSelector
					@see         set
			*/
			const ObjectSelector& operator = (const ObjectSelector& selector)
				throw();

			/** Copying.
					Copy {\em *this} objectSelector to the objectSelector
					{\em selector}.
					Initializes the \Ref{AtomBondModelBaseProcessor} of {\em *this} objectSelector
					to that of {\em selector}.
					Calls \Ref{set}.
					@param       selector the objectSelector to be assigned to
					@param       deep make a deep (={\tt true}) or shallow (={\tt false}) copy of {\em selector}
					@see         set
			*/
			void get(ObjectSelector& selector, bool deep = true) const
				throw();

			/** Swapping of objectSelector's.
					Swap the \Ref{AtomBondModelBaseProcessor} and the state of {\em *this}
					objectSelector	with that of {\em selector}.
					Calls \Ref{AtomBondModelBaseProcessor::swap}
					@param       selector the objectSelector being swapped with {\em *this} atomBondModelBaseProcessor 
					@see         AtomBondModelBaseProcessor
			*/
			void swap(ObjectSelector& selector)
				throw();
			//@}

			/**	@name	Accessors: inspectors and mutators 
					With these two methods {\em *this} objectSelector can be 
					switched between selection and deselection of processed objects.
					{\em *this} objectSelector is set to select objects initially.
					@see   Selectable
			*/
			//@{

			/** Select objects.
					Change the state of {\em *this} objectSelector to select processed
					objects. The method \Ref{Selectable::select} will be called for every
					processed objects.
					@see   deselect
					@see   Selectable
			*/
			void select()
				throw();

			/** Deselect objects.
					Change the state of {\em *this} objectSelector to deselect processed
					objects. The method \Ref{Selectable::deselect} will be called for every
					processed objects.
					@see   select
					@see   Selectable
			*/
			void deselect()
				throw();

			//@}

			/**	@name Processor specific methods
			*/
			//@{
			/** Start method.
					Calls \Ref{AtomBondModelBaseProcessor::start}.
					@return bool {\tt true} if the start of {\em *this} objectSelector was successful, {\tt false} otherwise
					@see    operator()
					@see    AtomBondModelBaseProcessor
			*/
			virtual bool start();
			
			/** Finish method.
					This method will be internally called from the processor mechanism if the processor
					has finished processing the \Ref{Composite} tree.
					All previously inserted \Ref{Atom} objects (inserted with the method \Ref{insertAtom_})
					will be processed. The connecting \Ref{Bond} objects will be selected or
					deselected according to the state of {\em *this} objectSelector.
					@return bool {\tt true} if the finish of {\em *this} objectSelector was successful, {\tt false} otherwise
					@see    select
					@see    deselect
					@see    Selectable
					@see    insertAtom_
					@see    operator()
					@see    Atom
					@see    Bond
					@see    Composite
			*/
			virtual bool finish();

			
			/**	Operator method.
					This method iterates over each \Ref{Composite} object reachable in the 
					\Ref{Composite} tree. If {\em composite} is of kind \Ref{Atom} this atom is
					selected or deselected according to the state of {\em *this} objectSelector
					and is inserted with the method \Ref{insertAtom_}.
					@param  composite the \Ref{Composite} object that will be processed
					@return Processor::Result the result of {\em *this} geometricObjectSelectot
					@see    select
					@see    deselect
					@see    Selectable
					@see    start
					@see    finish
					@see    insertAtom_
					@see    Composite
					@see    Atom
			*/
			virtual Processor::Result operator() (Composite& composite);
			//@}

			/**	@name	Predicates
					These methods allow inspection of the state of {\em *this} objectSelector.
			*/
			//@{

			/** Test if selection mode is enabled.
					Check if the state of {\em *this} objectSelector is set to
					select objects.
					@return  bool {\tt true} if the state of {\em *this} objectSelector is set to select objects, {\tt false} otherwise
					@see     select
					@see     deselect
					@see     isDeselectionMode
					@see     Selectable
			*/
			bool isSelectionMode()
				throw();

			/** Test if deselection mode is enabled.
					Check if the state of {\em *this} objectSelector is set to
					deselect objects.
					@return  bool {\tt true} if the state of {\em *this} objectSelector is set to deselect objects, {\tt false} otherwise
					@see     select
					@see     deselect
					@see     isSelectionMode
					@see     Selectable
			*/
			bool isDeselectionMode()
				throw();

			//@}


			/**	@name	debuggers and diagnostics
			*/
			//@{
			/** Internal value dump.
					Dump the current state of {\em *this} objectSelector to 
					the output ostream {\em s} with dumping depth {\em depth}.
					Calls \Ref{AtomBondModelBaseProcessor::dump}.
					@param   s output stream where to output the state of {\em *this} objectSelector
					@param   depth the dumping depth
					@see     AtomBondModelBaseProcessor
			*/
			virtual void dump
				(std::ostream& s = std::cout, Size depth = 0) const
				throw();
			//@}

			
			private:

			bool selection_;
		};

#			ifndef BALL_NO_INLINE_FUNCTIONS
#				include <BALL/MOLVIEW/FUNCTOR/objectSelector.iC>
#			endif

	} // namespace MOLVIEW

} // namespace BALL

#endif // BALL_MOLVIEW_FUNCTOR_OBJECTSELECTOR_H

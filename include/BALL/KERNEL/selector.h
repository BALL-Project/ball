// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: selector.h,v 1.28 2004/02/23 20:39:00 anker Exp $
//

#ifndef BALL_KERNEL_SELECTOR_H
#define BALL_KERNEL_SELECTOR_H

#ifndef BALL_KERNEL_EXPRESSION_H
#	include <BALL/KERNEL/expression.h>
#endif

#ifndef BALL_DATATYPE_LIST_H
#	include <BALL/DATATYPE/list.h>
#endif


namespace BALL 
{
	class Atom;

	/**	Atom Selection from Regular Expression.
    	\ingroup  Predicates
	*/
	class Selector
		:	public UnaryProcessor<Composite>
	{
		
		public:
		
		/**	@name	Constructors and Destructors
		*/
		//@{

		// BALL_CREATE(Selector)

		/**	Default constructor.
		*/
		Selector()
			throw();

		/**	Copy constructor.
		*/
		Selector(const Selector& selector)
			throw();

		/** Construct a Selector with a string.
				The selector's expression is set to <tt>expression_string</tt>.
		*/
		Selector(const String& expression_string)
			throw();

		/** Destructor.
		*/
		virtual ~Selector()
			throw();

		//@}
		/** @name Assignment
		*/
		//@{

		/** Clear method.
		*/
		virtual void clear()
			throw();

		/** Assignment operator.
		*/
		Selector& operator = (const Selector& selector)
			throw();

		//@}
		/** @name Predicates
		*/
		//@{

		/** Equality.
		*/
		bool operator == (const Selector& selector) const
			throw();

		//@}
		/**	@name	Processor related methods
		*/
		//@{

		/** Access operator.
				If the given Composite is an  \link Atom Atom \endlink  and the current
				 \link expression_ expression_ \endlink  is fulfilled for that atom, select it by
				calling  \link Composite::select Composite::select \endlink .
		*/
		virtual Processor::Result operator () (Composite& composite)
			throw();
		
		/**	Processor start method.
				This method is needed to reset the internal counter for the number of
				selected atoms.
		*/
		virtual bool start()
			throw();

		//@}
		/**	@name	Accessors
		*/
		//@{

		/**	Return the number of atoms selected during the last application.
		*/
		Size getNumberOfSelectedAtoms() const
			throw();

		/** Return a list containing pointers to all selected Atoms
		*/
		List<Atom*>& getSelectedAtoms() 
			throw();
		
		/** Define the expression.
		*/
		void setExpression(const Expression& expression)
			throw();

		/** Return the expression.
		*/
		const Expression& getExpression() const
			throw();
		
				
		//@}
			
		protected:

		List<Atom*>	selected_atoms_;
		Expression	expression_;
	};

} // namespace BALL

#endif // BALL_KERNEL_SELECTOR_H

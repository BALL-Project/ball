// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: selector.h,v 1.29 2005/10/23 12:02:20 oliver Exp $
//

#ifndef BALL_KERNEL_SELECTOR_H
#define BALL_KERNEL_SELECTOR_H

#ifndef BALL_KERNEL_EXPRESSION_H
#	include <BALL/KERNEL/expression.h>
#endif


namespace BALL 
{
	class Atom;

	/**
	 * Atom Selection from BALL \ref Expression.
	 * \par
	 * This class takes a string that represents an \ref Expression. It can then be applied to a \ref Composite
	 * and will select all atoms matching the \ref Expression. Refer to the documentation of \ref Expression for a
	 * list of available predicates.
	 *
	 * \see Expression
	 *
	 * \ingroup  Predicates
	 */
	class BALL_EXPORT Selector
		:	public UnaryProcessor<Composite>
	{
		
		public:
		
		/**	@name	Constructors and Destructors
		*/
		//@{

		// BALL_CREATE(Selector)

		/**	Default constructor.
		*/
		Selector();

		/**	Copy constructor.
		*/
		Selector(const Selector& selector);

		/** Construct a Selector with a string.
				The selector's expression is set to <tt>expression_string</tt>.
		*/
		Selector(const String& expression_string);

		/** Destructor.
		*/
		virtual ~Selector();

		//@}
		/** @name Assignment
		*/
		//@{

		/** Clear method.
		*/
		virtual void clear();

		/** Assignment operator.
		*/
		Selector& operator = (const Selector& selector);

		//@}
		/** @name Predicates
		*/
		//@{

		/** Equality.
		*/
		bool operator == (const Selector& selector) const;

		//@}
		/**	@name	Processor related methods
		*/
		//@{

		/** Access operator.
				If the given Composite is an  \link Atom Atom \endlink  and the current
				 \link expression_ expression_ \endlink  is fulfilled for that atom, select it by
				calling  \link Composite::select Composite::select \endlink .
		*/
		virtual Processor::Result operator () (Composite& composite);
		
		/**	Processor start method.
				This method is needed to reset the internal counter for the number of
				selected atoms.
		*/
		virtual bool start();

		//@}
		/**	@name	Accessors
		*/
		//@{

		/**	Return the number of atoms selected during the last application.
		*/
		Size getNumberOfSelectedAtoms() const;

		/** Return a list containing pointers to all selected Atoms
		*/
		std::list<Atom*>& getSelectedAtoms();
		
		/** Define the expression.
		*/
		void setExpression(const Expression& expression);

		/** Return the expression.
		*/
		const Expression& getExpression() const;
		
				
		//@}
			
		protected:

		std::list<Atom*> selected_atoms_;
		Expression	expression_;
	};

} // namespace BALL

#endif // BALL_KERNEL_SELECTOR_H

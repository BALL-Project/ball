// $Id: selector.h,v 1.15 2001/07/09 19:18:04 anker Exp $

#ifndef BALL_KERNEL_SELECTOR_H
#define BALL_KERNEL_SELECTOR_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_DATATYPE_STRINGHASHMAP_H
#	include <BALL/DATATYPE/stringHashMap.h>
#endif

#ifndef BALL_KERNEL_ATOM_H
#	include <BALL/KERNEL/atom.h>
#endif

#ifndef BALL_KERNEL_EXPRESSION_H
#	include <BALL/KERNEL/expression.h>
#endif


namespace BALL 
{

	/**	@name	Kernel Object Selection.
			These classes allow the convenient selection of kernel objects.\\
			{\bf Definition:}\URL{BALL/KERNEL/selector.h}
	*/
	//@{
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
				Note: Implementation of using the expression not yet implemented.
		*/
		// BAUSTELLE
		Selector(const String& expression_string)
			throw();

		/** Destructor.
		*/
		virtual ~Selector()
			throw();

		//@}
		/**	@name	Processor related methods
		*/
		//@{

		/** Access operator.
				All atom instances contained in the given composite instance are
				selected and counted.
				@see Composite::select
		*/
		// BAUSTELLE
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

		Size				number_of_selected_atoms_;
		Expression	expression_;
	};
	//@}

} // namespace BALL

#endif // BALL_KERNEL_SELECTOR_H

// $Id: selector.h,v 1.12 2000/08/30 19:58:11 oliver Exp $

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
		BALL_CREATE(Selector)
		/**	Default constructor.
		*/
		Selector();

		/**	Copy constructor.
		*/
		Selector(const Selector& selector);

		/** Construct a Selector with a string.
		*/
		Selector(const String& expression_string);

		/** Destructor.
		*/
		virtual ~Selector();
		//@}

		/**	@name	Processor related methods
		*/
		//@{
		/**
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
				
		//@}
			
		protected:

		Size				number_of_selected_atoms_;
		Expression	expression_;
	};
	//@}

} // namespace BALL

#endif // BALL_KERNEL_SELECTOR_H

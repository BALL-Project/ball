// $Id: shiftModule.h,v 1.5 2000/09/18 11:02:51 oliver Exp $

#ifndef BALL_NMR_SHIFTMODULE_H
#define BALL_NMR_SHIFTMODULE_H

#ifndef BALL_KERNEL_SYSTEM_H
#	include<BALL/KERNEL/system.h>
#endif

#ifndef BALL_DATATYPE_STRING_H
#	include<BALL/DATATYPE/string.h>
#endif

#ifndef BALL_DATATYPE_LIST_H
#	include<BALL/DATATYPE/list.h>
#endif

#ifndef BALL_CONCEPT_PROCESSOR_H
#	include<BALL/CONCEPT/processor.h>
#endif

#ifndef BALL_FORMAT_PARAMETERS_H
#	include<BALL/FORMAT/parameters.h>
#endif

namespace BALL 
{

	/**@name	 ShiftModule
	*/

	//@{		

	/**	A single contribution of a NMR shift model.
			NMR shift models typically consist of a number of different 
			contributions (e.g. ring current, ansisotopy, etc.). Each of these
			contributions is implemented in a ShiftModule. Several of these ShiftModules 
			can then be combined to a complete \Ref{ShiftModel}.
			Since ShiftModules are derived from \Ref{UnaryProcessor}, they can be applied
			to arbitrary kernel data structures. 
			\\
			{\bf Caveat:} The shifts are usually stored in a property of the corresponding atom.
			Applying the same processor multiply will give incorrect results, as the ShiftModules
			\emph{add} their shift contribution. Before applying a ShiftModule, these
			properties can be reset to zero by applying a \Ref{ClearShiftProcessor}.
			\\
			{\bf Definition}\URL{BALL/NMR/shiftModule.h}
	*/

	class ShiftModule 
		: public UnaryProcessor<Composite>
	{
		public:	

		BALL_CREATE(ShiftModule)

		/**@name	Constructors and Destructors
		*/
		//@{
		
		/** 	Default Constructor
		*/
		ShiftModule()
			throw();

		/**	Detailed constructor
		*/
		ShiftModule(Parameters& parameters, const String& name = "")
			throw();

		/**	Copy constructor
		*/
		ShiftModule(const ShiftModule& module)
			throw();

		/**	Destructor
		*/
		virtual ~ShiftModule()
			throw();

		/**	Clear method.
				Clear the name and the pointer to the parameters.
		*/
		virtual void clear()
			throw();
		//@}

		/**	@name Assignment
		*/
		//@{

		/**	Assignment operator
		*/
		const ShiftModule& operator = (const ShiftModule& module)
			throw();

		//@}

		/**	@name Accessors
		*/
		//@{
		/**	Set the modules name
		*/
		void setName(const String& name)
			throw();

		/**	Return the module name
		*/
		const String& getName() const
			throw();

		/**	Set the parameters
		*/
		void setParameters(Parameters& parameters)
			throw();

		/**	Return a pointer to the parameters
		*/
		const Parameters* getParameters() const
			throw();

		//@}
		protected:

		String			module_name_;		

		Parameters*	parameters_;
	};
	//@}

} // namespace BALL

#endif // BALL_NMR_SHIFTMODULE_H

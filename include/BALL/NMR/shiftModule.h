// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: shiftModule.h,v 1.18 2005/12/23 17:01:56 amoll Exp $
//

#ifndef BALL_NMR_SHIFTMODULE_H
#define BALL_NMR_SHIFTMODULE_H

#ifndef BALL_KERNEL_SYSTEM_H
#	include<BALL/KERNEL/system.h>
#endif

#ifndef BALL_DATATYPE_STRING_H
#	include<BALL/DATATYPE/string.h>
#endif

#ifndef BALL_CONCEPT_PROCESSOR_H
#	include<BALL/CONCEPT/processor.h>
#endif

#ifndef BALL_FORMAT_PARAMETERS_H
#	include<BALL/FORMAT/parameters.h>
#endif

namespace BALL 
{
	/**	A single contribution of a NMR shift model.
			NMR shift models typically consist of a number of different 
			contributions (e.g. ring current, ansisotopy, etc.). Each of these
			contributions is implemented in a ShiftModule. Several of these ShiftModules 
			can then be combined to a complete  \link ShiftModel ShiftModel \endlink .
			Since ShiftModules are derived from  \link UnaryProcessor UnaryProcessor \endlink , they can be applied
			to arbitrary kernel data structures. 	 \par
			<b>Caveat:</b> The shifts are usually stored in a property of the corresponding atom.
			Applying the same processor multiply will give incorrect results, as the ShiftModules
			\emph{add} their shift contribution. Before applying a ShiftModule, these
			properties can be reset to zero by applying a  \link ClearShiftProcessor ClearShiftProcessor \endlink .  \par
	\ingroup ShiftModel
	*/
	class BALL_EXPORT ShiftModule 
		: public UnaryProcessor<Composite>
	{
		public:	

		BALL_CREATE(ShiftModule)

		/**	@name	Enums and Constants
		*/
		//@{

		/**	Named property to store the shift values.
				Use this string constant to access the shift values stored in the single
				atoms. \par
				<b>Example:</b>
				<tt>atom.setProperty(ShiftModule::PROPERTY__SHIFT, 0.0);</tt>
		*/
		static const char* PROPERTY__SHIFT;
		static const char* PROPERTY__EXPERIMENTAL__SHIFT;
		//@}

		/** @name	Constructors and Destructors
		*/
		//@{
		
		/** Default Constructor
		*/
		ShiftModule();

		/**	Detailed constructor
		*/
		ShiftModule(Parameters& parameters, const String& name = "");

		/**	Copy constructor
		*/
		ShiftModule(const ShiftModule& module);

		/**	Destructor
		*/
		virtual ~ShiftModule();

		/**	Clear method.
				Clear the name and the pointer to the parameters.
		*/
		virtual void clear();

		//@}
		/**	@name Assignment
		*/
		//@{

		/**	Assignment operator
		*/
		const ShiftModule& operator = (const ShiftModule& module);

		//@}
		/**	@name Accessors
		*/
		//@{

		/**	Set the modules name
		*/
		void setName(const String& name);

		/**	Return the module name
		*/
		const String& getName() const;

		/**	Set the parameters.
				After the assignment, the state of the module is \emph{invalid},
				so it is required to run  \link init init \endlink .
				@param parameters the new parameters
				@see	isValid
		*/
		void setParameters(Parameters& parameters);

		/**	Return a pointer to the parameters
		*/
		const Parameters* getParameters() const;

		/**	Parameter initalization.
				Use this method to implement the extraction and initialization of
				the module's parameters.
				 \link init init \endlink  is called by  \link ShiftModel ShiftModel \endlink  as soon as the  \link ShiftModule ShiftModule \endlink 
				is constructed and parameters and name are assigned. \par
				All implementations in derived classes should set the  \link valid_ valid_ \endlink  flag
				to <b>true</b> if the initialization was successful and to <b>false</b> otherwise.
		*/
		virtual void init();

		//@}
		/**	@name Processor related methods
		*/
		//@{

		/**	Start method.
				This method aborts, if the module is not correctly initialized.
				@see isValid
		*/
		virtual bool start();

		/**	Finish method.
				This method aborts, if the module is not correctly initialized.
				@see isValid
		*/
		virtual bool finish();

		//@}
		/**	@name	Predicates
		*/
		//@{

		/**	Return the module state.
				The module is valid if  \link init init \endlink  was executed successfully.
				@return the module state
		*/
		bool isValid() const;

		//@}

		protected:

		/*_	The module name
		*/
		String	module_name_;		

		/*_	A pointer to the modules parameters
		*/
		Parameters*	parameters_;

		/*_	The module's validity flag.
				This flag should indicate that the module was correctly
				initialized (using \Ref{init}).
		*/
		bool valid_;
	};
  
} // namespace BALL

#endif // BALL_NMR_SHIFTMODULE_H

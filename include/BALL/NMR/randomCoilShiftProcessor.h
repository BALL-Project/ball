// $Id: randomCoilShiftProcessor.h,v 1.1 2000/09/19 13:34:59 oliver Exp $

#ifndef BALL_NMR_SHIFTMODULE_H
#	include<BALL/NMR/shiftModule.h>
#endif

#ifndef BALL_DATAYTPE_STRINGHASHMAP_H
#	include<BALL/NMR/shiftModule.h>
#endif

#ifndef BALL_KERNEL_EXPRESSION_H
#	include<BALL/KERNEL/expression.h>
#endif

#ifndef BALL_FORMAT_PARAMETERS_H
#	include<BALL/FORMAT/parameters.h>
#endif

#ifndef BALL_FORMAT_PARAMETERSECTION_H
#	include<BALL/FORMAT/parameterSection.h>
#endif

#include <list>

namespace BALL 
{
		

	/**	adding random coil shifts to Hydrogens
	*/
	class RandomCoilShiftProcessor
		:	public ShiftModule
	{
		public:
		
		/**@name	Constructors and Destructors
		*/
		//@{

		/**	Default constructor.
		*/
		RandomCoilShiftProcessor() throw();
		
		/**	Destructor
		*/
		virtual ~RandomCoilShiftProcessor() throw();
		
		//@}

		
		/**@name	Processor specific functions.
		*/
		//@{
		
		/**	Start method.
			A StringHashMap of floats is built and named {\tt rc\_table\_}
			A file called "random.dat" is opened ,it contains the randomcoilshift date for
			every residue´s atoms. The random coil shift is stored in {\tt rc\_table\_}
			under [residue_name:atom_name].

		*/
		virtual bool start() throw();
		
		/**	Finish method.
			nothing is done here
			
		*/
		virtual bool finish() throw();

		/**	Application method
			if current object is a PDBAtom of kind Hydrogen its random coil shift has to be looked up
			in {\tt rc\_table}.
			Get Hydrogens residue name append ":" and its name, the lookup entry is reconstruced.
			If this entry is found within the table the randomcoil shift is added to the Hydrogens
			chemical shift.
			If this entry is not found the random coil shift is set to 1000 and added as well to the
			Hydrogens chemical shift,to mark that Hydrogen not to have a random coil shift table entry.
	 
			
			
		*/
		virtual Processor::Result operator () (Composite& composite) throw();
		//@}
		

		protected:

		StringHashMap<float>		rc_table_;
		std::list<PDBAtom*>			proton_list_;	
		String									ini_filename_;
		Parameters							parameters_;
		ParameterSection				parameter_section_;
		std::vector<Expression>	expressions_;
	};

} // namespace BALL

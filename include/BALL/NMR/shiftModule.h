// $Id: shiftModule.h,v 1.4 2000/09/08 07:09:54 oliver Exp $

#ifndef BALL_NMR_SHIFTMODULE_H
#define BALL_NMR_SHIFTMODULE_H

#include<BALL/KERNEL/system.h>
#include<BALL/DATATYPE/string.h>
#include<BALL/CONCEPT/processor.h>
#include<BALL/CONCEPT/object.h>

namespace BALL 
{

	/**@name	 ShiftModule
	*/

	//@{		

	/**		is derived from UnaryProcessor.
			This way its secure just to have shift modules in the processor list of NMRSpectrum.
	*/

	class ShiftModule 
		: public UnaryProcessor<Composite>
	{
		public:	
		/**@name	Constructors and Destructors
		*/
		//@{
		
		/** 	Default Constructor
		*/
		ShiftModule();

		/**	Destructor
		*/
		~ShiftModule();

		//@}
		protected:
		String module_name_;		
	};
	//@}

} // namespace Ball

#endif // BALL_NMR_SHIFTMODULE_H

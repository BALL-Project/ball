// $Id: clearShiftProcessor.h,v 1.3 2000/09/20 13:51:08 amoll Exp $

#ifndef BALL_NMR_CLEARSHIFTPROCESSOR_H
#define BALL_NMR_CLEARSHIFTPROCESSOR_H

#ifndef BALL_NMR_SHIFTMODULE_H
#	include <BALL/NMR/shiftModule.h>
#endif

namespace BALL 
{
		
	/**	Clear assigned shift data of all atoms.
	*/
	class ClearShiftProcessor
		:	public ShiftModule
	{
		public:
		
		/**@name	Constructors and Destructors
		*/
		//@{

		/**	Default constructor.
		*/
		ClearShiftProcessor()	
			throw();
		
		/**	Destructor
		*/
		virtual ~ClearShiftProcessor()
			throw();
		
		//@}

		/**@name	Processor specific functions.
		*/
		//@{

		/**	Application method
		*/
		virtual Processor::Result operator () (Composite& composite)
			throw();
		//@}
		
	};

} // namespace BALL

#endif // BALL_NMR_CLEARSHIFTPROCESSOR_H

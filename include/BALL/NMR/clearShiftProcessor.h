// $Id: clearShiftProcessor.h,v 1.1 2000/09/16 07:43:16 oliver Exp $

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
		ClearShiftProcessor();
		
		/**	Destructor
		*/
		virtual ~ClearShiftProcessor();
		
		//@}

		
		/**@name	Processor specific functions.
		*/
		//@{

		/**	Application method
		*/
		virtual Processor::Result operator () (Composite&);
		//@}
		
	};

} // namespace Ball


#endif // BALL_NMR_CLEARSHIFTPROCESSOR_H

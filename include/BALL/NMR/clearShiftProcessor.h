// $Id: clearShiftProcessor.h,v 1.4 2000/09/22 12:07:12 oliver Exp $

#ifndef BALL_NMR_CLEARSHIFTPROCESSOR_H
#define BALL_NMR_CLEARSHIFTPROCESSOR_H

#ifndef BALL_NMR_SHIFTMODULE_H
#	include <BALL/NMR/shiftModule.h>
#endif

namespace BALL 
{
		
	/**	Clear assigned shift data of all atoms.
			This processors clears the named property defined by \Ref{ShiftModule::PROPERTY__SHIFT}
			in the atoms of all data structures it is applied to.
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
		
		/**	Copy constructor.
		*/
		ClearShiftProcessor(const ClearShiftProcessor& processor)	
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

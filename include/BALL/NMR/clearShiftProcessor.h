// $Id: clearShiftProcessor.h,v 1.5 2001/03/02 22:19:33 amoll Exp $

#ifndef BALL_NMR_CLEARSHIFTPROCESSOR_H
#define BALL_NMR_CLEARSHIFTPROCESSOR_H

#ifndef BALL_NMR_SHIFTMODULE_H
#	include <BALL/NMR/shiftModule.h>
#endif

namespace BALL 
{
		
	/**	Clear assigned shift data of all atoms.
			This processors clears the named property defined by \Ref{ShiftModule::PROPERTY__SHIFT}
			in the atoms of all data structures it is applied to. \\
			{\bf Definition}\URL{BALL/NMR/clearShiftProcessor.h}
	*/
	class ClearShiftProcessor
		:	public ShiftModule
	{
		public:
		
		/** @name	Constructors and Destructors
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

		/** @name	Processor specific functions.
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

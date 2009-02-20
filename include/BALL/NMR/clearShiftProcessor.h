// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: clearShiftProcessor.h,v 1.13 2005/12/23 17:01:55 amoll Exp $
//

#ifndef BALL_NMR_CLEARSHIFTPROCESSOR_H
#define BALL_NMR_CLEARSHIFTPROCESSOR_H

#ifndef BALL_NMR_SHIFTMODULE_H
#	include <BALL/NMR/shiftModule.h>
#endif

namespace BALL 
{
	/**	Clear assigned shift data of all atoms.
			This processors clears the named property defined by  \link ShiftModule::PROPERTY__SHIFT ShiftModule::PROPERTY__SHIFT \endlink 
			in the atoms of all data structures it is applied to.  \par
	\ingroup ShiftModulesNMR		
	*/
	class BALL_EXPORT ClearShiftProcessor
		:	public ShiftModule
	{
		public:
		
		/** @name	Constructors and Destructors
		*/
		//@{

		/**	Default constructor.
		*/
		ClearShiftProcessor();
		
		/**	Copy constructor.
		*/
		ClearShiftProcessor(const ClearShiftProcessor& processor);
		
		/**	Destructor
		*/
		virtual ~ClearShiftProcessor();
		
		//@}

		/** @name	Processor specific functions.
		*/
		//@{

		/**	Application method
		*/
		virtual Processor::Result operator () (Composite& composite);

		//@}
		
	};
  
} // namespace BALL

#endif // BALL_NMR_CLEARSHIFTPROCESSOR_H

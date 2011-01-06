// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: randomCoilShiftProcessor.h,v 1.16.10.2 2007-04-12 13:53:55 anne Exp $
//

#ifndef BALL_NMR_SHIFTMODULE_H
#	include<BALL/NMR/shiftModule.h>
#endif

#ifndef BALL_DATAYTPE_STRINGHASHMAP_H
#	include<BALL/DATATYPE/stringHashMap.h>
#endif

namespace BALL 
{	
	/**	Random coil shift assignment processor.
		\ingroup ShiftModulesNMR	
	*/
	class BALL_EXPORT RandomCoilShiftProcessor
		:	public ShiftModule
	{
		public:
		
    BALL_CREATE(RandomCoilShiftProcessor)

    /** @name Enums and Constants
    */
    //@{

    /** A symbolic name for the random coil contribution to the chemical shift
        @see ShiftModule::PROPERTY__SHIFT
    */
    static const char* PROPERTY__RANDOM_COIL_SHIFT;

    //@}
		/** @name	Constructors and Destructors
		*/
		//@{

		/**	Default constructor.
		*/
		RandomCoilShiftProcessor();
		
		/**	Copy constructor.
		*/
		RandomCoilShiftProcessor(const RandomCoilShiftProcessor& processor);
		
		/**	Destructor
		*/
		virtual ~RandomCoilShiftProcessor();
		
		//@}
		/**	@name Accessors
		*/
		//@{

		/**	Initialize the module.
				This method fails if no  \link Parameters Parameters \endlink  object was assigned
				or the parameter file does not contain the proper section (<tt>RandomCoilShifts</tt>).
				The random coil shift as read from the parameter file are stored in {\tt shift_map_}.
				If the method terminates correctly, the module is valid.
				@see isValid
		*/
		virtual void init();

		//@}
		/** @name	Processor specific functions.
		*/
		//@{
		
		/**	Application method.
		 		For all atom objects, the random coil shift contribution as read from the parameter files
				is determined, stored in the named property \link RandomCoilShiftProcessor::PROPERTY__RANDOM_COIL_SHIFT 
				RandomCoilShiftProcessor::PROPERTY__RANDOM_COIL_SHIFT\endlink,
				and added to the total chemical shift contribution, stored in the named property 
				\link ShiftModule::PROPERTY__SHIFT ShiftModule::PROPERTY__SHIFT \endlink.
		*/
		virtual Processor::Result operator () (Composite& composite);

		//@}

		protected:

		/*_	The hash map containing the atom names and the random coil shifts.
		*/
		StringHashMap<float>	shift_map_;
		private:
		void      postprocessing_();
		System*   system_;

	};
  
} // namespace BALL

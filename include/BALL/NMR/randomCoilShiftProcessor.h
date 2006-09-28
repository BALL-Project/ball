// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: randomCoilShiftProcessor.h,v 1.16.10.1 2006/10/04 13:49:41 anne Exp $
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
				If it terminates correctly, the module is valid.
				@see isValid
		*/
		virtual void init();

		//@}
		/** @name	Processor specific functions.
		*/
		//@{
		
		/**	Application method.
				If current object is a Atom of kind Hydrogen its random coil shift has to be looked up
				in <tt>rc_table</tt>.
				Get Hydrogens residue name append ":" and its name, the lookup entry is reconstruced.
				If this entry is found within the table the randomcoil shift is added to the Hydrogens
				chemical shift.
				If this entry is not found the random coil shift is set to 1000 and added as well to the
				Hydrogens chemical shift, to mark that Hydrogen not to have a random coil shift table entry.
		*/
		virtual Processor::Result operator () (Composite& composite);

		//@}

		protected:

		/*_	The hash map containing the atom names and the random coil shifts.
		*/
		StringHashMap<float>	shift_map_;
		private:
		void			postprocessing_() throw();
		System* system_;

	};
  
} // namespace BALL

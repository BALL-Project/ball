// $Id: randomCoilShiftProcessor.h,v 1.6 2000/09/26 19:27:47 oliver Exp $

#ifndef BALL_NMR_SHIFTMODULE_H
#	include<BALL/NMR/shiftModule.h>
#endif

#ifndef BALL_DATAYTPE_STRINGHASHMAP_H
#	include<BALL/DATATYPE/stringHashMap.h>
#endif

namespace BALL 
{
		

	/**	Random coil shift assignment processor
	*/
	class RandomCoilShiftProcessor
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
 
		/**@name	Constructors and Destructors
		*/
		//@{

		/**	Default constructor.
		*/
		RandomCoilShiftProcessor() 
			throw();
		
		/**	Copy constructor.
		*/
		RandomCoilShiftProcessor(const RandomCoilShiftProcessor& processor) 
			throw();
		
		/**	Destructor
		*/
		virtual ~RandomCoilShiftProcessor() 
			throw();
		
		//@}

		/**	@name Accessors
		*/
		//@{

		/**	Initialize the module.
				This method fails if not \Ref{Parameters} object was assigned
				or the parameter file does not contain the proper section ({\tt RandomCoilShifts}).
				If it terminates correctly, the modules is valid.
				@see isValid
		*/
		virtual void init()
			throw();
		//@}
		
		/**@name	Processor specific functions.
		*/
		//@{
		
		/**	Application method
				If current object is a Atom of kind Hydrogen its random coil shift has to be looked up
				in {\tt rc\_table}.
				Get Hydrogens residue name append ":" and its name, the lookup entry is reconstruced.
				If this entry is found within the table the randomcoil shift is added to the Hydrogens
				chemical shift.
				If this entry is not found the random coil shift is set to 1000 and added as well to the
				Hydrogens chemical shift, to mark that Hydrogen not to have a random coil shift table entry.
		*/
		virtual Processor::Result operator () (Composite& composite) throw();
		//@}

		protected:

		/**	The hash map containing the atom names and the random coil shifts.
		*/
		StringHashMap<float>		shift_map_;
	};

} // namespace BALL

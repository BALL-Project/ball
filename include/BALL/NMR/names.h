// $Id: names.h,v 1.2 2000/07/03 21:08:50 oliver Exp $

#include<BALL/DATATYPE/string.h>
#include<BALL/KERNEL/system.h>

#ifndef SHIFT_MODULE
#	include<BALL/NMR/shiftModule.h>
#endif
///////////////////////////////////////////////

namespace BALL {
/**@name	names
*/

//@{

/**	Here every shift module header has to be included (in fact they have to be included in the .C file)
	Every shift module is associated with a special string. Each NMRSpectrum has an instance
	of names. 
*/

class ShiftNames
	{
	public:
	/**@name Constructors and Destructors
	*/

	//@{

	/** Default Constructor
	*/	
	ShiftNames();
	
	/** Destructor
	*/
	~ShiftNames();

	//@}

	/**@name class special functions
	*/

	//@{
	
	/** String - ShiftModule association function.
	it gets a string naming the Shift Module and returns an instance of that Shift Module
	*/

	ShiftModule* get_module(String);
	
	//@}
	
	};

//@}
	
} // namespace Ball


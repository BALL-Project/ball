// $Id: shiftModule.h,v 1.2 2000/07/03 21:08:51 oliver Exp $

#define SHIFT_MODULE

#include<BALL/KERNEL/system.h>
#include<BALL/DATATYPE/string.h>
#include<BALL/CONCEPT/processor.h>
#include<BALL/CONCEPT/object.h>

namespace BALL {

/**@name	 ShiftModule
*/

//@{		

/**		is derived from UnaryProcessor.
		This way its secure just to have shift modules in the processor list of NMRSpectrum.
*/

class ShiftModule 
: public UnaryProcessor <Object>
	{
	private:
	String* module_name_;
	
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
	};
//@}

} // namespace Ball

// $Id: makeSpectrumProcessor.h,v 1.2 2000/07/03 21:08:50 oliver Exp $

#define MAKE_SPECTRUM_PROCESSOR

#include<BALL/KERNEL/system.h>
#include<BALL/KERNEL/PTE.h>

#ifndef BALL_CONCEPT_PROCESSOR_H
#	include <BALL/CONCEPT/processor.h>
#endif

#ifndef NMR_SPECTRUM
#	include<BALL/NMR/NMRSpectrum.h>
#endif

namespace BALL
{
/**@name 	MakeSpectrumProcessor
*/

//@{

/** 	Processor designed to built the well known processor user Interface of NMRSpectrum
*/

class MakeSpectrumProcessor:public UnaryProcessor<Object>
	{
	private:
	System *system_;
	NMRSpectrum *spectrum_;

	public:
	
	/**@name Constructors and Destructors
	*/
	
	//@{
	
	/**	Default Constructor
		a new NMRSpectrum is allocated for {\tt spectrum\_}
		and some standard shift modules are inserted : JB , HM , ANISO and RC
	*/
	MakeSpectrumProcessor();
	
	/**	Destructor
	*/
	virtual ~MakeSpectrumProcessor();
	
	//@}

	/**@name class special functions
	*/

	//@{

	/**	start function
		nothing is done here
	*/
	virtual bool start();
	
	/**	finish funcion
		NMRSpectrum function calculate_shifts() is called
	*/
	virtual bool finish();
	
	/**	operator function
		if object is kind of system 
		the {\tt system\_} variable of NMRSpectrum {\tt spectrum\_} is set to this system
	*/
	virtual Processor::Result operator()(Object&);
	
	//@}
	};

//@}

} // namespace BALL	 

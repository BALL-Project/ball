// $Id: makeSpectrumProcessor.h,v 1.3 2000/09/07 19:37:04 oliver Exp $

#ifndef BALL_NMR_MAKESPECTRUMPROCESSOR_H
#define BALL_NMR_MAKESPECTRUMPROCESSOR_H

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

class MakeSpectrumProcessor
	:	public UnaryProcessor<Atom>
{
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

	/**	finish funcion
			NMRSpectrum function calculate_shifts() is called
	*/
	virtual bool finish();
	
	/**	operator function
			if object is kind of system 
			the {\tt system\_} variable of NMRSpectrum {\tt spectrum\_} is set to this system
	*/
	virtual Processor::Result operator () (Atom& atom);
	
	//@}
	protected:
	System*			system_;
	NMRSpectrum spectrum_;
};

//@}

} // namespace BALL	 

#endif // BALL_NMR_MAKESPECTRUMPROCESSOR_H

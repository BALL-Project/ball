// $Id: makeSpectrumProcessor.h,v 1.4 2000/09/21 22:10:42 amoll Exp $

#ifndef BALL_NMR_MAKESPECTRUMPROCESSOR_H
#define BALL_NMR_MAKESPECTRUMPROCESSOR_H

#ifndef BALL_CONCEPT_PROCESSOR_H
#	include <BALL/CONCEPT/processor.h>
#endif

#ifndef NMR_SPECTRUM
#	include<BALL/NMR/NMRSpectrum.h>
#endif

class System;

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
			A new NMRSpectrum is allocated for {\tt spectrum\_}
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
	virtual Processor::Result operator () (const Atom& atom);
	
	//@}
	protected:
		NMRSpectrum spectrum_;
};

//@}

} // namespace BALL	 

#endif // BALL_NMR_MAKESPECTRUMPROCESSOR_H

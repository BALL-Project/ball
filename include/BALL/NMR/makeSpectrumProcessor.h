// $Id: makeSpectrumProcessor.h,v 1.6 2001/03/02 22:19:33 amoll Exp $

#ifndef BALL_NMR_MAKESPECTRUMPROCESSOR_H
#define BALL_NMR_MAKESPECTRUMPROCESSOR_H

#ifndef BALL_CONCEPT_PROCESSOR_H
#	include <BALL/CONCEPT/processor.h>
#endif

#ifndef NMR_SPECTRUM
#	include<BALL/NMR/NMRSpectrum.h>
#endif

//class System;

namespace BALL
{

/** MakeSpectrumProcessor.	
		Processor designed to built the well known processor user Interface of NMRSpectrum. \\
		{\bf Definition}\URL{BALL/NMR/makeSpectrumProcessor.h}
*/
class MakeSpectrumProcessor
	:	public UnaryProcessor<Atom>
{
	public:
	
	/** @name Constructors and Destructors
	*/
	//@{
	
	/**	Default Constructor.
			A new NMRSpectrum is allocated for {\tt spectrum_} and some standard 
			shift modules are inserted : JB , HM , ANISO and RC.
	*/
	MakeSpectrumProcessor();
	
	/**	Destructor
	*/
	virtual ~MakeSpectrumProcessor();
	
	//@}
	/** @name class special functions
	*/
	//@{

	/**	Finish funcion.
			NMRSpectrum function calculate_shifts() is called
	*/
	virtual bool finish();
	
	/**	Operator function.
			If object is kind of system the {\tt system_} variable of 
			NMRSpectrum {\tt spectrum_} is set to this system.
	*/
	virtual Processor::Result operator () (Atom& atom);
	
	//@}

	protected:
		NMRSpectrum spectrum_;
};

} // namespace BALL	 

#endif // BALL_NMR_MAKESPECTRUMPROCESSOR_H

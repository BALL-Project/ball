// $Id: createSpectrumProcessor.h,v 1.7 2000/09/26 19:29:46 oliver Exp $

#ifndef BALL_NMR_CREATESPECTRUMPROCESSOR_H
#define BALL_NMR_CREATESPECTRUMPROCESSOR_H

#ifndef BALL_NMR_PEAK_H
#	include <BALL/NMR/peak.h>
#endif

#ifndef BALL_KERNEL_ATOM_H
# include <BALL/KERNEL/atom.h>
#endif

#ifndef BALL_DATATYPE_STRINGHASHSET_H
# include <BALL/DATATYPE/stringHashSet.h>
#endif

#ifndef BALL_NMR_SHIFTMODULE_H
#	include <BALL/NMR/shiftModule.h>
#endif

#include <list>

namespace BALL 
{
	class Atom;

	/**	Processor creates {\tt peaklist\_}. 
			Each atom´s chemical shift will create a new peak.
			Thist peak´s {\tt atomlist\_} will contain the corresponding atom.
	*/
	class CreateSpectrumProcessor
		:	public ShiftModule
	{
		public:

		/**@name	Constructors and Destructors
		*/
		
		//@{

		/**	Default Constructor
		*/	
		CreateSpectrumProcessor();
		
		/**	Destructor
		*/
		virtual ~CreateSpectrumProcessor()
			throw();
		
		//@}

		/**@name	class special functions
		*/

		//@{

		/**	Application method.
		*/
		virtual Processor::Result operator () (Composite& atom)
			throw();
		
		/**	Returns a reference to {\tt peaklist\_}.
		*/
		const list<Peak1D>& getPeakList() const;
		
		//@}

		/**	@name Accessors
		*/
		//@{
		
		/**
		*/
		virtual void init()
			throw();
		//@}
		protected:

		std::list<Peak1D> peaklist_;

		StringHashSet			ignore_atoms_;

		vector<String>						equivalency_residues_;
		vector<vector<String> >		equivalency_atoms_;
	};

} //namespace BALL
	 
#endif // BALL_NMR_CREATESPECTRUMPROCESSOR_H

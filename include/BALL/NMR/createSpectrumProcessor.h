// $Id: createSpectrumProcessor.h,v 1.4 2000/09/07 21:04:28 oliver Exp $

#ifndef BALL_NMR_CREATESPECTRUMPROCESSOR_H
#define BALL_NMR_CREATESPECTRUMPROCESSOR_H

#include<BALL/COMMON/constants.h>
#include<BALL/KERNEL/system.h>
#include<BALL/KERNEL/atom.h>
#include<BALL/KERNEL/bond.h>
#include<BALL/KERNEL/residue.h>
#include<BALL/KERNEL/molecule.h>
#include<BALL/KERNEL/protein.h>
#include<BALL/KERNEL/residue.h>
#include<BALL/KERNEL/PDBAtom.h>
#include<BALL/DATATYPE/string.h>
#include<BALL/DATATYPE/stringHashMap.h>
#include<BALL/KERNEL/PTE.h>

#ifndef BALL_NMR_PEAK_H
#	include<BALL/NMR/peak.h>
#endif

#include <list>

namespace BALL 
{
	/**@name	CreateSpectrumProcessor
	*/

	//@{

	/**	Processor creates {\tt peaklist\_}. Each atom큦 chemical shift will create a new peak.
		thist peak큦 {\tt atomlist\_} will contain the corresponding atom.
	*/

	class CreateSpectrumProcessor
		:	public UnaryProcessor<Atom>
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
		virtual ~CreateSpectrumProcessor();
		
		//@}

		/**@name	class special functions
		*/

		//@{

		/**	start-function
			nothing is done here
		*/
		virtual bool start();
		
		/**	operator-function
			if object is kind of PDBAtom a new peak is created ,
			peak큦 parameters are set and the atom is added to the peak큦
			{\tt atomlist\_}
		*/
		virtual Processor::Result operator () (Atom& atom);
		

		/**	returns a pointer to {\tt peaklist\_}
		*/
		const list<Peak1D>& getPeakList() const;
		
		//@}
		protected:

		list<Peak1D> peaklist_;
		float raster_;
	};

	//@}

} //namespace Ball
	 
#endif // BALL_NMR_CREATESPECTRUMPROCESSOR_H

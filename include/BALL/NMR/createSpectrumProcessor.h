// $Id: createSpectrumProcessor.h,v 1.6 2000/09/20 18:31:35 amoll Exp $

#ifndef BALL_NMR_CREATESPECTRUMPROCESSOR_H
#define BALL_NMR_CREATESPECTRUMPROCESSOR_H

#ifndef BALL_NMR_PEAK_H
#	include<BALL/NMR/peak.h>
#endif

#ifndef BALL_KERNEL_ATOM_H
# include<BALL/KERNEL/atom.h>
#endif

#include <list>

namespace BALL 
{
	class Atom;

	/**@name	CreateSpectrumProcessor
	*/

	//@{

	/**	Processor creates {\tt peaklist\_}. 
			Each atom큦 chemical shift will create a new peak.
			Thist peak큦 {\tt atomlist\_} will contain the corresponding atom.
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
		*/
		virtual bool start();
		
		/**	operator-function.
				If object is kind of PDBAtom a new peak is created ,
				peak큦 parameters are set and the atom is added to the peak큦
				{\tt atomlist\_}
		*/
		virtual Processor::Result operator () (Atom& atom);
		
		/**	Returns a reference to {\tt peaklist\_}.
		*/
		const list<Peak1D>& getPeakList() const;
		
		//@}
		protected:

		std::list<Peak1D> peaklist_;
	};

	//@}

} //namespace Ball
	 
#endif // BALL_NMR_CREATESPECTRUMPROCESSOR_H

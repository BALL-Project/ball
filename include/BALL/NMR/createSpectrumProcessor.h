// $Id: createSpectrumProcessor.h,v 1.11 2001/07/14 22:55:24 oliver Exp $

#ifndef BALL_NMR_CREATESPECTRUMPROCESSOR_H
#define BALL_NMR_CREATESPECTRUMPROCESSOR_H

#ifndef BALL_NMR_PEAK_H
#	include <BALL/NMR/peak.h>
#endif

#ifndef BALL_NMR_PEAKLIST_H
#	include <BALL/NMR/peakList.h>
#endif

#ifndef BALL_KERNEL_ATOM_H
# include <BALL/KERNEL/atom.h>
#endif

#ifndef BALL_KERNEL_EXPRESSION_H
# include <BALL/KERNEL/expression.h>
#endif

#ifndef BALL_DATATYPE_STRINGHASHSET_H
# include <BALL/DATATYPE/stringHashSet.h>
#endif

#ifndef BALL_NMR_SHIFTMODULE_H
#	include <BALL/NMR/shiftModule.h>
#endif

namespace BALL 
{
	class Atom;

	/**	Processor creates {\tt peaklist_}. 
			Each atom´s chemical shift will create a new peak.
			Thist peak´s {\tt atomlist_} will contain the corresponding atom. \\
			{\bf Definition}\URL{BALL/NMR/createSpectrumProcessor.h}
	*/
	class CreateSpectrumProcessor
		:	public ShiftModule
	{
		public:

		/** @name	Constructors and Destructors
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

		/** @name	class special functions
		*/

		//@{

		/**	Start method.
		*/
		virtual bool start() 
			throw();

		/**	Application method.
		*/
		virtual Processor::Result operator () (Composite& atom)
			throw();
		
		/**	Returns a reference to {\tt peaklist_}.
		*/
		const PeakList1D& getPeakList() const;
		
		//@}
		/**	@name Accessors
		*/
		//@{
		
		/**
		*/
		virtual void init()
			throw();

		///
		void setWidth(float width)
			throw();

		///
		float getWidth() const
			throw();

		///
		void enableAveraging(bool flag = true)
			throw();
			
		///
		void enableIgnoreTable(bool flag = true)
			throw();

		///
		void setExpression(const String& expression)
			throw();

		//@}

		protected:

		PeakList1D							peaklist_;

		StringHashSet						ignore_atoms_;

		vector<String>						equivalency_residues_;
		vector<vector<String> >		equivalency_atoms_;
		float width_;
		float use_averaging_;
		float use_ignore_table_;
		Expression expression_;
	};

} //namespace BALL
	 
#endif // BALL_NMR_CREATESPECTRUMPROCESSOR_H

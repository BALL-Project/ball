// $Id: createSpectrumProcessor.h,v 1.13 2001/09/28 14:44:25 amoll Exp $

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

#ifndef BALL_DATATYPE_REGULARDATA1D_H
# include <BALL/DATATYPE/regularData1D.h>
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

	/**	Processor creates peaklist_. 
			Each atom´s chemical shift will create a new peak.
			Thist peak´s {\tt atomlist_} will contain the corresponding atom. \\
			{\bf Definition} \URL{BALL/NMR/createSpectrumProcessor.h}
	*/
	class CreateSpectrumProcessor
		:	public ShiftModule
	{
		public:
		
		/**	Constants
		*/
		//@{	
		
		/// Name of the section containing ignored atoms in the config file
		static const String IGNORE_SECTION_NAME;

		/// Name of the section containing averaged atoms in the config file
		static const String AVERAGE_SECTION_NAME;
		
		//@}
		/** @name	Constructors and Destructors
		*/
		//@{

		/**	Default Constructor.
				Create a default instance of CreateSpectrumProcessor.
				Calls \Ref{init} to read default values from a configuration
				file, see \Ref{init} for details.
				@exception FileNotFound if the INI file does not exist
				@exception ParseError if the contents of the file could not be parsed
		*/	
		CreateSpectrumProcessor()
			throw(Exception::FileNotFound, Exception::ParseError);
		
		/**	Destructor
		*/
		virtual ~CreateSpectrumProcessor()
			throw();
		
		//@}
		/** @name	Processor related methods
		*/
		//@{

		/**	Init method.
				Read the contents of a specified INI file containing
				information on ignored and averaged atoms.
				The default filename is {\tt NMR/StandardSpectrum.ini} 
				in the BALL data path.
				@exception FileNotFound if the INI file does not exist
				@exception ParseError if the contents of the file could not be parsed
		*/
		virtual void init(const String& filename = "NMR/StandardSpectrum.ini")
			throw();

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
		
		///
		void setWidth(float width)
			throw();

		///
		float getWidth() const
			throw();

		///
		void setAtomAveraging(bool flag = true)
			throw();
			
		///
		bool getAtomAveraging() const
			throw();
			
		///
		void setAtomIgnoring(bool flag = true)
			throw();

		///
		bool getAtomIgnoring() const
			throw();

		///
		void setExpression(const String& expression)
			throw();

		const String& getExpression() const
			throw();

		//@}

		protected:

		PeakList1D							peaklist_;
		StringHashSet						ignore_atoms_;
		vector<String>					equivalency_residues_;
		vector<vector<String> >	equivalency_atoms_;
		float width_;
		float use_averaging_;
		float use_ignore_table_;
		Expression expression_;
	};

	/**	Create a simulated spectrum from a peak list.
			Using this operator, a peak list is converted to a gridded
			representation of the spectrum. The spectrum synthesis is
			based upon the assumption of a Lorentzian line shape. Peak width,
			position, and height are taken from each individual peak of the 
			peak list. Each point in the \Ref{RegularData1D} array is assigned
			the sum of all Lorentzians centered at the peak positions:
			\begin{equation}
					S(\delta) = \sum_{i} \frac{h_i}{w_i (\delta_i-\delta)^2},
			\end{equation}
			where $\delta$ is the shift coordinate and each peak is defined
			by its position $\delta_i$, intensity $h_i$, and width $w_i$.
	*/
	const RegularData1D& operator << (RegularData1D& data, const PeakList1D& peak_list) throw();

} //namespace BALL
	 
#endif // BALL_NMR_CREATESPECTRUMPROCESSOR_H

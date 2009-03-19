// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: createSpectrumProcessor.h,v 1.26 2005/12/23 17:01:55 amoll Exp $
//

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
			Each atom's chemical shift will create a new peak.
			This peak's <tt>atomlist_</tt> will contain the corresponding atom.  \par
	\ingroup Spectra
	*/
	class BALL_EXPORT CreateSpectrumProcessor
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
				Calls  \link init init \endlink  to read default values from a configuration
				file, see  \link init init \endlink  for details.
				@exception FileNotFound if the INI file does not exist
				@exception ParseError if the contents of the file could not be parsed
		*/	
		CreateSpectrumProcessor()
			throw(Exception::FileNotFound, Exception::ParseError);
		
		/**	Destructor
		*/
		virtual ~CreateSpectrumProcessor();
		
		//@}
		/** @name	Processor related methods
		*/
		//@{

		/**	Init method.
				Read the contents of a specified INI file containing
				information on ignored and averaged atoms.
				The default filename is <tt>NMR/StandardSpectrum.ini</tt> 
				in the BALL data path.
				@exception FileNotFound if the INI file does not exist
				@exception ParseError if the contents of the file could not be parsed
		*/
		virtual void init();

		/**	Init method.
				Read the contents of a specified INI file containing
				information on ignored and averaged atoms.
				The default filename is <tt>NMR/StandardSpectrum.ini</tt> 
				in the BALL data path.
				@exception FileNotFound if the INI file does not exist
				@exception ParseError if the contents of the file could not be parsed
		*/
		virtual void init(const String& filename)
			throw(Exception::ParseError, Exception::FileNotFound);

		/**	Start method.
		*/
		virtual bool start() 
			;

		/**	Application method.
		*/
		virtual Processor::Result operator () (Composite& atom)
			;
		
		/**	Returns a reference to <tt>peaklist_</tt>.
		*/
		const PeakList1D& getPeakList() const;
		
		//@}
		/**	@name Accessors
		*/
		//@{
		
		///
		void setWidth(float width)
			;

		///
		float getWidth() const
			;

		///
		void setAtomAveraging(bool flag = true)
			;
			
		///
		bool getAtomAveraging() const
			;
			
		///
		void setAtomIgnoring(bool flag = true)
			;

		///
		bool getAtomIgnoring() const
			;

		///
		void setExpression(const String& expression)
			;

		const String& getExpression() const
			;

		//@}

		protected:

		PeakList1D							peaklist_;
		StringHashSet						ignore_atoms_;
		vector<String>					equivalency_residues_;
		vector<vector<String> >	equivalency_atoms_;
		float width_;
		bool	use_averaging_;
		bool	use_ignore_table_;
		Expression expression_;
	};

	/**	Create a simulated spectrum from a peak list.
			Using this operator, a peak list is converted to a gridded
			representation of the spectrum. The spectrum synthesis is
			based upon the assumption of a Lorentzian line shape. Peak width,
			position, and height are taken from each individual peak of the 
			peak list. Each point in the  \link TRegularData1D RegularData1D \endlink  array is assigned
			the sum of all Lorentzians centered at the peak positions:
			\f[
					S(\delta) = \sum_{i} \frac{h_i}{w_i (\delta_i-\delta)^2}
			\f]  
			where \f$\delta\f$ is the shift coordinate and each peak is defined
			by its position \f$\delta_i\f$, intensity \f$h_i\f$, and width \f$w_i\f$.
	\ingroup Spectra
	*/
	BALL_EXPORT const RegularData1D& operator << (RegularData1D& data, const PeakList1D& peak_list) ;
  
} //namespace BALL
	 
#endif // BALL_NMR_CREATESPECTRUMPROCESSOR_H

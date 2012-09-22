// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: spectrum.h,v 1.14.18.6 2007-04-12 13:53:57 anne Exp $
//

#ifndef BALL_NMR_SPECTRUM_H
#define BALL_NMR_SPECTRUM_H

#ifndef BALL_NMR_PEAKLIST_H
#	include<BALL/NMR/peakList.h>
#endif

#ifndef BALL_DATATYPE_REGULARDATA1D_H
#	include<BALL/DATATYPE/regularData1D.h>
#endif

#ifndef BALL_DATATYPE_REGULARDATA2D_H
#	include<BALL/DATATYPE/regularData2D.h>
#endif

#ifndef BALL_DATATYPE_REGULARDATA3D_H
#	include<BALL/DATATYPE/regularData3D.h>
#endif

#ifdef BALL_HAS_FFTW
# include <BALL/MATHS/complex.h>
#ifndef BALL_MATHS_FFT1D_H
# include <BALL/MATHS/FFT1D.h>
#endif
#ifndef BALL_MATHS_FFT2D_H
# include <BALL/MATHS/FFT2D.h>
#endif
#endif


namespace BALL 
{
	/**	A generic NMR spectrum class.
			 \par
			
			 \par
	\ingroup Spectra
	*/
	template <typename DataT, typename PeakT, typename PositionT = typename PeakT::Position>
	class Spectrum
	{
		public:
		
		/**	Typedefs
		*/
		//@{
		/// The datatype used to store the spectrum
		typedef DataT DataType;
		/// the data type used to store a position (for peaks and data)
		typedef	PositionT PositionType;
		///	The peak type
		typedef	PeakT PeakType;
		/// An iterator 
		typedef typename DataT::Iterator Iterator;
		/// A constant iterator 
		typedef typename DataT::ConstIterator ConstIterator;
		//@}

		/**	Constructors and Destructor
		*/	
		//@{
			// ?????
		Spectrum()
			: data_(),
				sticks_(),
				spacing_(),
				min_(),
				max_()
		{}

		Spectrum(const DataType& data)
			: data_(data),
				sticks_(),
				spacing_(),
				min_(),
				max_()
		{}

		Spectrum(const std::vector<PeakType>& peaks, const PositionType& origin, const PositionType& dimension, const PositionType& spacing)
		{}

		/** Destructor
		*/
		virtual ~Spectrum() {}
		//@}

		/**	@name	Accessors
		*/
		//@{
		///
		const DataType& getData() const;
		///
		DataType& getData();
		
		const std::vector<float>& getHuInvariants() const;
		
		std::vector<float>& getHuInvariants();

		//@}

		virtual void clear();
		virtual void clearSticks();
		virtual double difference(const Spectrum<DataT, PeakT, PositionT>& spectrum) const;
		virtual  Spectrum<DataT, PeakT, PositionT> differenceSpectrum(const Spectrum<DataT, PeakT, PositionT>& spectrum);
		virtual double earthMoversDistance(const Spectrum<DataT,PeakT, PositionT>& spectrum) const; 
		
		virtual void convertToGaussian(); 
		virtual void convertToLorentzian(); 
		virtual void computeAllMoments(int  moment_number);

		virtual void setSpacing(const PositionType& spacing);
		virtual PositionType getSpacing() const;

		virtual void setSticks(std::vector<PeakType> sticks) {sticks_ = sticks;};
		virtual std::vector<PeakType> getSticks() const {return sticks_;};
		
	
		// computes the integral over the fabs() of the spectrum
		virtual double getAbsIntegral() const;
		virtual void computeHuInvariants();
		virtual std::vector<float> computeHuInvariantsDifferences(std::vector<Spectrum<DataT, PeakT, PositionT> >& spectra);

		/** Computes the difference between the spectra in Fourier space. max_freq and min_freq can be used to
		 *  include only a certain range into the comparison. In this case, we only use the positive frequencies (the
		 *  negative ones are redundant anyhow) and assume that 0<=min_freq<max_freq<=1, where min_freq and max_freq
		 *  are interpreted as percentages, e.g. calling this function with min_freq=0 and max_freq=0.5 would include
		 *  the lower half of the positive frequencies.
		 */
		virtual double getFourierDifference(const Spectrum<DataT,PeakT, PositionT>& spectrum, float min_freq = 1e6, float max_freq = -1e6);

		/** Computes the difference according to the precomputed normal moments up to moment_number. If the values have
		 *  not been precomputed so far, we regenerate them.
		 */
		virtual double getNormalMomentsDifference(Spectrum<DataT,PeakT, PositionT>& spectrum, int moment_number);
		virtual double getCentralMomentsDifference(Spectrum<DataT,PeakT, PositionT>& spectrum, int moment_number);
		virtual double getStandardizedMomentsDifference(Spectrum<DataT,PeakT, PositionT>& spectrum, int moment_number);
		
		/// precomputed normal moments
		std::vector<float> normal_moments;
		/// precomputed central moments
		std::vector<float> central_moments;
		/// precomputed standardized moments
		std::vector<float> standardized_moments;
	
		/// This function requires that DataT has a binaryWrite method
		void binaryWrite(const String& filename);

		/// This function requires that DataT has a binaryRead method
		void binaryRead(const String& filename);

		protected:
			DataType							data_;
			std::vector<PeakType> sticks_;
			PositionType					spacing_; // rausschmeissen , wie auch immer 
			PositionType					min_;     // rausschmeissen , wie auch immer 
			PositionType					max_;     // rausschmeissen , wie auch immer 
			std::vector<float>	 	Hu_invariants_;
	};

	/**	Clear the spectrum.
			Sets all data to zero.  // TODO: what about the  spacing?
	*/
	template <typename DataT, typename PeakT, typename PositionT>
	void Spectrum<DataT, PeakT, PositionT>::clear()
	{
		data_.clear();
		sticks_.clear();
	}
	
	template <typename DataT, typename PeakT, typename PositionT>
	void Spectrum<DataT, PeakT, PositionT>::clearSticks()
	{
		sticks_.clear();
	}


	// TODO: muss die hier stehen??? 
	/**	Calculate the difference between two spectra.
	*/
	template <typename DataT, typename PeakT, typename PositionT>
	double Spectrum<DataT, PeakT, PositionT>::difference(const Spectrum<DataT, PeakT, PositionT>& /* spectrum */) const
	{
		// ?????
		return 0.0;
	}

	/**	Return the spacing.
	*/
	template <typename DataT, typename PeakT, typename PositionT>
	typename Spectrum<DataT, PeakT, PositionT>::PositionType Spectrum<DataT, PeakT, PositionT>::getSpacing() const
	{
		return spacing_;
	}

	/** Set the spacing.
	*/
	template <typename DataT, typename PeakT, typename PositionT>
	void Spectrum<DataT, PeakT, PositionT>::setSpacing(const typename Spectrum<DataT, PeakT, PositionT>::PositionType& spacing)
	{
		spacing_ = spacing;
	}

	/**	Difference operator.
			Calculate the sum of the unsigned differences of the two spectra
	*/
	template <typename DataT, typename PeakT, typename PositionT>
	double operator - (const Spectrum<DataT, PeakT, PositionT>& s1, const Spectrum<DataT, PeakT, PositionT>& s2)
	{
		return s1.difference(s2);
	}
	
	template <typename DataT, typename PeakT, typename PositionT>
	double Spectrum<DataT, PeakT, PositionT>::getNormalMomentsDifference(Spectrum<DataT, PeakT, PositionT>& spectrum, int moment_number)
	{
		if (normal_moments.size() != (Size)moment_number)
			computeAllMoments(moment_number);
		if (spectrum.normal_moments.size() != (Size)moment_number)
			spectrum.computeAllMoments(moment_number);

		double diff = 0.;
		for (int current_moment=0; current_moment<moment_number; current_moment++)
			diff += fabs(normal_moments[current_moment] - spectrum.normal_moments[current_moment]);

		return diff;
	}

	template <typename DataT, typename PeakT, typename PositionT>
	double Spectrum<DataT, PeakT, PositionT>::getCentralMomentsDifference(Spectrum<DataT, PeakT, PositionT>& spectrum, int moment_number)
	{
		if (central_moments.size() != (Size)moment_number)
			computeAllMoments(moment_number);
		if (spectrum.central_moments.size() != (Size)moment_number)
			spectrum.computeAllMoments(moment_number);

		double diff = 0.;
		for (int current_moment=0; current_moment<moment_number; current_moment++)
			diff += fabs(central_moments[current_moment] - spectrum.central_moments[current_moment]);

		return diff;
	}

	template <typename DataT, typename PeakT, typename PositionT>
	double Spectrum<DataT, PeakT, PositionT>::getStandardizedMomentsDifference(Spectrum<DataT, PeakT, PositionT>& spectrum, int moment_number)
	{
		if (standardized_moments.size() != (Size)moment_number)
			computeAllMoments(moment_number);
		if (spectrum.standardized_moments.size() != (Size)moment_number)
			spectrum.computeAllMoments(moment_number);

		double diff = 0.;
		for (int current_moment=0; current_moment<moment_number; current_moment++)
			diff += fabs(standardized_moments[current_moment] - spectrum.standardized_moments[current_moment]);

		return diff;
	}	
	
	template <typename DataT, typename PeakT, typename PositionT>
	const DataT& Spectrum<DataT, PeakT, PositionT>::getData() const
	{
		return data_;
	}
	
	template <typename DataT, typename PeakT, typename PositionT>
	DataT& Spectrum<DataT, PeakT, PositionT>::getData()
	{
		return data_;
	}
/*	
	const vector<float>& getHuInvariants();
*/

	template <typename DataT, typename PeakT, typename PositionT>
	const std::vector<float>& Spectrum<DataT, PeakT, PositionT>::getHuInvariants() const
	{
		return Hu_invariants_;
	}

	template <typename DataT, typename PeakT, typename PositionT>
	std::vector<float>& Spectrum<DataT, PeakT, PositionT>::getHuInvariants()
	{
		return Hu_invariants_;
	}


	
	template <typename DataT, typename PeakT, typename PositionT>
	void Spectrum<DataT, PeakT, PositionT>::computeHuInvariants()
	{
		Log.error()<< "computeHuInvariants() only implemented in 2D" << std::endl;
		return;
	}

	template <typename DataT, typename PeakT, typename PositionT>
	std::vector<float> Spectrum<DataT, PeakT, PositionT>::computeHuInvariantsDifferences(std::vector<Spectrum<DataT, PeakT, PositionT> >& /*spectra*/)
	{
		Log.error()<< "computeHuInvariantsDifferences() only implemented in 2D" << std::endl;
		std::vector<float> result;
		return result;
	}

	template <typename DataT, typename PeakT, typename PositionT>
	double Spectrum<DataT, PeakT, PositionT>::getFourierDifference(const Spectrum<DataT, PeakT, PositionT>& spectrum, float min_freq, float max_freq)
	{
		Log.error() << "getFourierDifference only implemented in 1D and 2D" << std::endl;
		return 0.;
	}

	/**	Convenience typedefs
	\ingroup Spectra
	*/
	//@{
	/// One-dimensional spectrum
	typedef Spectrum<RegularData1D, Peak1D> Spectrum1D;

	/// Two-dimensional spectrum
	typedef Spectrum<RegularData2D, Peak2D> Spectrum2D;

	/// Three-dimensional spectrum
	typedef Spectrum<RegularData3D, Peak3D> Spectrum3D;
	//@}
  
	template <typename DataT, typename PeakT, typename PositionT>
	std::ostream& operator << (std::ostream& os, const Spectrum<DataT, PeakT, PositionT>& spectrum);
	
	template <typename DataT, typename PeakT, typename PositionT>
	std::istream& operator >> (std::istream& is, Spectrum<DataT, PeakT, PositionT>& spectrum);

# ifndef BALL_NO_INLINE_FUNCTIONS
#   include <BALL/NMR/spectrum.iC>
# endif
} // namespace BALL


#endif // BALL_NMR_SPECTRUM_H

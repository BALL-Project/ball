// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: spectrum.h,v 1.14.18.1 2006/09/06 12:23:21 anne Exp $
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
		Spectrum() {}

		Spectrum(const DataType& data)
			: data_(data)
		{}

		Spectrum(const std::vector<PeakType>& peaks, const PositionType& origin, const PositionType& dimension, const PositionType& spacing)
		{}

			// ?????
		virtual ~Spectrum() {}
		//@}

		/**	@name	Accessors
		*/
		//@{
		///
		const DataType& getData() const;
		///
		DataType& getData();
		//@}

		virtual void clear();
		virtual double difference(const Spectrum<DataT, PeakT, PositionT>& spectrum) const;

		virtual void setSpacing(const PositionType& spacing);
		virtual PositionType getSpacing() const;
		
		protected:
		DataType	data_;
		std::vector<PeakType> sticks_;
		PositionType					spacing_;
		PositionType					min_;
		PositionType					max_;
	};

	/**	Clear the spectrum.
			Sets all data to zero.
	*/
	template <typename DataT, typename PeakT, typename PositionT>
	void Spectrum<DataT, PeakT, PositionT>::clear()
	{
		data_.clear();
		sticks_.clear();
	}

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
	const DataT& Spectrum<DataT, PeakT, PositionT>::getData() const
	throw()
	{
		return data_;
	}
	
	template <typename DataT, typename PeakT, typename PositionT>
	DataT& Spectrum<DataT, PeakT, PositionT>::getData()
	throw()
	{
		return data_;
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

# ifndef BALL_NO_INLINE_FUNCTIONS
#   include <BALL/NMR/spectrum.iC>
# endif
} // namespace BALL


#endif // BALL_NMR_SPECTRUM_H

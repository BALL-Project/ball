// $Id: spectrum.h,v 1.5 2001/08/16 00:45:33 oliver Exp $

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
			\\
			{\bf Definition}\URL{BALL/NMR/spectrum.h}
			\\
	*/
	template <typename DataT, typename PeakT, typename PositionT = typename PeakT::Position>
	class Spectrum
	{
		public:
		
		/**	Typedefs
		*/
		//@{
		/// The datatype used to store the spectrum
		typedef DataT			DataType;
		/// the data type used to store a position (for peaks and data)
		typedef	PositionT	PositionType;
		///	The peak type
		typedef	PeakT			PeakType;
		/// An iterator 
		typedef typename DataT::Iterator Iterator;
		/// A constant iterator 
		typedef typename DataT::ConstIterator ConstIterator;
		//@}

		/**	Constructors and Destructor
		*/	
		//@{
			// BAUSTELLE
		Spectrum() {}
			// BAUSTELLE
		virtual ~Spectrum() {}
		//@}

		/**	@name	Accessors
		*/
		//@{
		///
		const DataType& getData() const
			throw();
		///
		DataType& getData()
			throw();
		//@}

		virtual void clear();
		virtual double difference(const Spectrum<DataT, PeakT, PositionT>& spectrum) const;

		virtual void setSpacing(const PositionType& spacing);
		virtual PositionType getSpacing() const;
		
		protected:
		DataType			data_;
		PositionType	spacing_;
		PositionType	min_;
		PositionType	max_;
	};

	/**	Clear the spectrum.
			Sets all data to zero.
	*/
	template <typename DataT, typename PeakT, typename PositionT>
	void Spectrum<DataT, PeakT, PositionT>::clear()
	{
		// BAUSTELLE
	}

	/**	Calculate the difference between two spectra.
	*/
	template <typename DataT, typename PeakT, typename PositionT>
	double Spectrum<DataT, PeakT, PositionT>::difference(const Spectrum<DataT, PeakT, PositionT>& /* spectrum */) const
	{
		// BAUSTELLE
		return 0.0;
	}

	/**	Return the spacing.
	*/
	template <typename DataT, typename PeakT, typename PositionT>
	Spectrum<DataT, PeakT, PositionT>::PositionType Spectrum<DataT, PeakT, PositionT>::getSpacing() const
	{
		return spacing_;
	}

	/** Set the spacing.
	*/
	template <typename DataT, typename PeakT, typename PositionT>
	void Spectrum<DataT, PeakT, PositionT>::setSpacing(const Spectrum<DataT, PeakT, PositionT>::PositionType& spacing)
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

	/**	Convenience typedefs
	*/
	//@{
	/// One-dimensional spectrum
	typedef Spectrum<RegularData1D, Peak1D> Spectrum1D;

	/// Two-dimensional spectrum
	typedef Spectrum<RegularData1D, Peak2D> Spectrum2D;

	/// Three-dimensional spectrum
	typedef Spectrum<RegularData1D, Peak3D> Spectrum3D;
	//@}

} // namespace BALL

#endif // BALL_NMR_SPECTRUM_H

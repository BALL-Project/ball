// $Id: spectrum.h,v 1.1 2001/06/14 11:43:30 oliver Exp $

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
		//@}

		/**	@name	Accessors
		*/
		//@{
		const DataType& getData() const;
		DataType& getData();
		//@}

		virtual void clear();
		virtual double difference(const Spectrum<DataT, PositionT, PeakT>& spectrum);

		virtual void setSpacing(const PositionType& spacing);
		virtual PositionType getSpacing() const;
		
		protected:
		DataType			data_;
		PositionType	spacing_;
		PositionType	min_;
		PositionType	max_;
	};



	/**	Convenience typedefs
	*/
	//@{
	///
	typedef Spectrum<RegularData1D, Peak1D> Spectrum1D;
	///
	// BAUSTELLE
	// typedef Spectrum<RegularData1D, Peak2D> Spectrum2D;
	///
	typedef Spectrum<RegularData1D, Peak3D> Spectrum3D;
	//@}

} // namespace BALL

#endif // BALL_NMR_SPECTRUM_H

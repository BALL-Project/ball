 // $Id: peakList.h,v 1.6 2002/01/05 02:54:28 oliver Exp $

#ifndef BALL_NMR_PEAKLIST_H
#define BALL_NMR_PEAKLIST_H

#ifndef BALL_NMR_PEAK_H
#	include<BALL/NMR/peak.h>
#endif

#ifndef BALL_COMMON_LIMITS_H
#	include<BALL/COMMON/limits.h>
#endif

#ifndef BALL_DATATYPE_LIST_H
#	include<BALL/DATATYPE/list.h>
#endif

namespace BALL 
{

	/**	A generic peak list for spectra of arbitrary dimension.
			\\
			{\bf Definition}\URL{BALL/NMR/peakList.h}
	*/
	template <typename PT>
	class PeakList
		:	public List<PT>
	{
		public:
		
		BALL_CREATE(PeakList<PT>)

		/**	@name Typedefs
		*/
		//@{
			
		///	The peak position type
		typedef typename PT::Position Position;

		/// The peak type
		typedef	PT										PeakType;
	
		///
		typedef typename List<PT>::iterator					Iterator;

		///
		typedef typename List<PT>::ConstIterator		ConstIterator;		
		//@}

		/** @name	Constructors and Destructors
		*/
		//@{

		/**	Default Constructor
		*/
		PeakList();
		
		/**	Copy constructor
		*/
		PeakList(const PeakList& peak_list);
		
		/**	Destructor
		*/
		virtual ~PeakList()
			throw();
		
		//@}

		/**	@name	Accessors
		*/
		//@{
			
		/**	Scale all peak intensities with a factor.
		*/
		void scale(float x);

		/**	Determine the maximum intensity of all peaks.
				@return maximum intensity, Limits<float>::min() for an empty peak list
		*/
		float getMaxIntensity() const;

		/**	Determine the minimum intensity of all peaks.
				@return minimum intensity, Limits<float>::max() for an empty peak list
		*/
		float getMinIntensity() const;

		/**	Determine the minimum position of the spectrum (all dimensions)
		*/
		Position getMinPosition() const;

		/**	Determine the maximum position of the spectrum (all dimensions)
		*/
		Position getMaxPosition() const;
		//@}
	};

	/**	@name	Standard typedefs
	*/
	//@{
		
	/// 1D peak list
	typedef PeakList<Peak<float> >			PeakList1D;

	/// 2D peak list
	typedef PeakList<Peak<Vector2> >		PeakList2D;

	/// 3D peak list
	typedef PeakList<Peak<Vector3> >		PeakList3D;
	
	//@}

	template <typename PT>
	PeakList<PT>::PeakList()
		:	List<PT>()
	{
	}

	template <typename PT>
	PeakList<PT>::PeakList(const PeakList<PT>& peak_list)
		:	List<PT>(peak_list)
	{
	}

	template <typename PT>
	PeakList<PT>::~PeakList()
		throw()
	{
	}

	template <typename PT>
	void PeakList<PT>::scale(float x)
	{
		Iterator it = begin();
		for (; it != end(); ++it)
		{
			it->setIntensity(it->getIntensity() * x);
		}
	}

	template <typename PT>
	float PeakList<PT>::getMaxIntensity() const
	{
		ConstIterator it = begin();
		float max = -Limits<float>::max();
		for (; it != end(); ++it)
		{
			max = std::max(max, it->getIntensity());
		}
		
		return max;
	}

	template <typename PT>
	float PeakList<PT>::getMinIntensity() const
	{
		ConstIterator it = begin();
		float min = Limits<float>::max();
		for (; it != end(); ++it)
		{
			min = std::min(min, it->getIntensity());
		}
		
		return min;
	}

	template <typename PT>
	PeakList<PT>::Position PeakList<PT>::getMinPosition() const
	{
		ConstIterator it = begin();
		PeakList<PT>::Position min = Limits<PeakList<PT>::Position>::max();
		for (; it != end(); ++it)
		{
			min = std::min(min, it->getPosition());
		}
		return min;
	}

	template <typename PT>
	PeakList<PT>::Position PeakList<PT>::getMaxPosition() const
	{
		ConstIterator it = begin();
		PeakList<PT>::Position max = -Limits<PeakList<PT>::Position>::max();
		for (; it != end(); ++it)
		{
			max = std::max(max, it->getPosition());
		}

		return max;
	}

	template <>
	PeakList<Peak2D>::Position PeakList<Peak2D>::getMinPosition() const
	{
		ConstIterator it = begin();
		Peak2D::Position min(Limits<float>::max(), Limits<float>::max());
		for (; it != end(); ++it)
		{
			min.x = std::min(min.x, it->getPosition().x);
			min.y = std::min(min.y, it->getPosition().y);
		}

		return min;
	}

	template <>
	PeakList<Peak2D>::Position PeakList<Peak2D>::getMaxPosition() const
	{
		ConstIterator it = begin();
		Peak2D::Position max(-Limits<float>::max(), -Limits<float>::max());
		for (; it != end(); ++it)
		{
			max.x = std::max(max.x, it->getPosition().x);
			max.y = std::max(max.y, it->getPosition().y);
		}

		return max;
	}

	template <>
	PeakList<Peak3D>::Position PeakList<Peak3D>::getMinPosition() const
	{
		ConstIterator it = begin();
		Peak3D::Position min(Limits<float>::max(), Limits<float>::max(), Limits<float>::max());
		for (; it != end(); ++it)
		{
			min.x = std::min(min.x, it->getPosition().x);
			min.y = std::min(min.y, it->getPosition().y);
			min.z = std::min(min.z, it->getPosition().z);
		}

		return min;
	}

	template <>
	PeakList<Peak3D>::Position PeakList<Peak3D>::getMaxPosition() const
	{
		ConstIterator it = begin();
		Peak3D::Position max(-Limits<float>::max(), -Limits<float>::max(), -Limits<float>::max());
		for (; it != end(); ++it)
		{
			max.x = std::max(max.x, it->getPosition().x);
			max.y = std::max(max.y, it->getPosition().y);
			max.z = std::max(max.z, it->getPosition().z);
		}

		return max;
	}



} // namespace BALL

#endif // BALL_NMR_PEAKLIST_H

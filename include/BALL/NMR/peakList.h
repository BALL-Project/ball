// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: peakList.h,v 1.15 2003/05/08 09:02:46 sneumann Exp $

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
			 \par
	\ingroup Spectra		
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
		PeakList() 
		{
		}
		
		/**	Copy constructor
		*/
		PeakList(const PeakList& peak_list)
			:	List<PT>(peak_list)
		{
		}		

		/**	Destructor
		*/
		virtual ~PeakList() throw()
		{
		}
		
		//@}

		/**	@name	Accessors
		*/
		//@{
			
		/**	Scale all peak intensities with a factor.
		*/
		void scale(float x)
		{
			Iterator it = List<PT>::begin();
			for (; it != List<PT>::end(); ++it)
			{
				it->setIntensity(it->getIntensity() * x);
			}
		}

		/**	Determine the maximum intensity of all peaks.
				@return maximum intensity, Limits<float>::min() for an empty peak list
		*/
		float getMaxIntensity() const
		{
			ConstIterator it = List<PT>::begin();
			float max = -Limits<float>::max();
			for (; it != List<PT>::end(); ++it)
			{
				max = std::max(max, it->getIntensity());
			}
		
			return max;
		}

		/**	Determine the minimum intensity of all peaks.
				@return minimum intensity, Limits<float>::max() for an empty peak list
		*/
		float getMinIntensity() const
		{
			ConstIterator it = List<PT>::begin();
			float min = Limits<float>::max();
			for (; it != List<PT>::end(); ++it)
			{
				min = std::min(min, it->getIntensity());
			}
		
			return min;
		}

		/**	Determine the minimum position of the spectrum (all dimensions)
		*/
		Position getMinPosition() const
		{
			ConstIterator it = List<PT>::begin();
			PeakList<PT>::Position min = Limits<PeakList<PT>::Position>::max();
			for (; it != List<PT>::end(); ++it)
			{
				min = std::min(min, it->getPosition());
			}
			return min;
		}

		/**	Determine the maximum position of the spectrum (all dimensions)
		*/
		Position getMaxPosition() const
		{
			ConstIterator it = List<PT>::begin();
			PeakList<PT>::Position max = -Limits<PeakList<PT>::Position>::max();
			for (; it != List<PT>::end(); ++it)
			{
				max = std::max(max, it->getPosition());
			}

			return max;
		}
		//@}
	};

	/**	@name	Standard typedefs
		\ingroup Spectra	
	*/
	//@{
		
	/// 1D peak list
	typedef PeakList<Peak<float> >			PeakList1D;

	/// 2D peak list
	typedef PeakList<Peak<Vector2> >		PeakList2D;

	/// 3D peak list
	typedef PeakList<Peak<Vector3> >		PeakList3D;
	
	//@}
  
} // namespace BALL

#endif // BALL_NMR_PEAKLIST_H

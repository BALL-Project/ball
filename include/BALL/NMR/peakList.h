// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: peakList.h,v 1.17 2003/08/26 08:04:45 oliver Exp $
//

#ifndef BALL_NMR_PEAKLIST_H
#define BALL_NMR_PEAKLIST_H

#ifndef BALL_NMR_PEAK_H
#	include<BALL/NMR/peak.h>
#endif

namespace BALL 
{

	/**	A generic peak list for spectra of arbitrary dimension.
			 \par
	\ingroup Spectra		
	*/
	template <typename PT>
	class PeakList
		:	public std::list<PT>
	{
		public:
		
		BALL_CREATE(PeakList<PT>)

		/**	@name Typedefs
		*/
		//@{
			
		///	The peak position type
		typedef typename PT::Position Position;

		/// The peak type
		typedef	PT PeakType;
	
		///
		typedef typename std::list<PT>::iterator Iterator;

		///
		typedef typename std::list<PT>::const_iterator ConstIterator;
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
			:	std::list<PT>(peak_list)
		{
		}		

		/**	Destructor
		*/
		virtual ~PeakList()
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
			Iterator it = std::list<PT>::begin();
			for (; it != std::list<PT>::end(); ++it)
			{
				it->setIntensity(it->getIntensity() * x);
			}
		}

		/**	Determine the maximum intensity of all peaks.
				@return maximum intensity, std::numeric_limits<float>::min() for an empty peak list
		*/
		float getMaxIntensity() const
		{
			ConstIterator it = std::list<PT>::begin();
			float max = -std::numeric_limits<float>::max();
			for (; it != std::list<PT>::end(); ++it)
			{
				max = std::max(max, it->getIntensity());
			}
		
			return max;
		}

		/**	Determine the minimum intensity of all peaks.
				@return minimum intensity, std::numeric_limits<float>::max() for an empty peak list
		*/
		float getMinIntensity() const
		{
			ConstIterator it = std::list<PT>::begin();
			float min = std::numeric_limits<float>::max();
			for (; it != std::list<PT>::end(); ++it)
			{
				min = std::min(min, it->getIntensity());
			}
		
			return min;
		}

		//@}
	};

	/**	@name	Standard typedefs
		\ingroup Spectra	
	*/
	//@{
		
	/// 1D peak list
	typedef PeakList<Peak<float> > PeakList1D;

	/// 2D peak list
	typedef PeakList<Peak<Vector2> > PeakList2D;

	/// 3D peak list
	typedef PeakList<Peak<Vector3> > PeakList3D;
	
	//@}
  
} // namespace BALL

#endif // BALL_NMR_PEAKLIST_H

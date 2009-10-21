// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:

#ifndef BALL_XRAY_SPACEGROUPPARAMETERS_H
#define BALL_XRAY_SPACEGROUPPARAMETERS_H

#ifndef BALL_FORMAT_PARAMETERSECTION_H
#	include <BALL/FORMAT/parameterSection.h>
#endif

#ifndef BALL_MATHS_MATRIX44_H
#	include <BALL/MATHS/matrix44.h>
#endif

namespace BALL 
{
	/** SpaceGroupParameters.
			
    	\ingroup  XRay
	*/
	class BALL_EXPORT SpaceGroupParameters 
		:	public ParameterSection
	{
		public:

		/**	Default constructor.
		*/
		SpaceGroupParameters();

		virtual ~SpaceGroupParameters();
		virtual void clear();
		virtual bool extractSpaceGroup(Parameters& parameters, const String& space_group);
		virtual bool extractSpaceGroup(Parameters& parameters, const Position iucr_no);
		//std::vector<Matrix4x4> getSymMatrices();

		protected:
		
		std::vector<Matrix4x4> symops_;
	};
} // namespace BALL

#endif // BALL_XRAY_SPACEGROUPPARAMETERS_H

// $Id: vector.h,v 1.2 1999/12/28 18:49:46 oliver Exp $

#ifndef BALL_MATHS_VECTOR_H
#define BALL_MATHS_VECTOR_H

#ifndef BALL_CONFIG_CONFIG_H
#	include <BALL/CONFIG/config.h>
#endif

namespace BALL 
{

	/* Not documented - used in the implementation of Vector4 only!
	*/
	
	template <class T>
	class TVector_
	{
		public:
			TVector_(const T& vx, const T& vy, const T& vz)
				: x(x),
					y(y),
					z(z),
					h((T)0)
			{
			}
				 
			TVector_(const T& vx, const T& vy, const T& vz, const T& h)
				:	x(x),
					y(y),
					z(z),
					h(h)
			{
			}
				 
			T x;
			T y;
			T z;
			T h;

		protected:

		private:
	};

} // namespace BALL

#endif // BALL_MATHS_VECTOR_H

// $Id: common.h,v 1.3 2000/02/16 17:07:15 oliver Exp $

#ifndef BALL_MATHS_COMPARISON_H
#define BALL_MATHS_COMPARISON_H

#ifndef BALL_CONFIG_CONFIG_H
#	include <BALL/CONFIG/config.h>
#endif

namespace BALL 
{
	
	namespace Maths 
	{

		/**	@name	Common Mathematical Functions and Comparisons.
				{\bf Namespace:} {\tt BALL::Maths}\\
				{\bf Definition:} \URL{BALL/MATHS/common.h}
				\\
		*/
		//@{ 

		///
		template <class T>
		inline 
		T abs(const T& t)
		{
			return BALL_ABS(t);
		}

		///
		template <class T>
		inline 
		T frac(const T& t)
		{ 
			long tmp = (long)t;
			return (t - (T)tmp);
		}

		///
		template <class T>
		inline 
		T max(const T& a, const T& b)
		{ 
			return BALL_MAX(a, b);
		}

		///
		template <class T>
		inline 
		T max(const T& a, const T& b, const T &ct)
		{ 
			return BALL_MAX3(a, b, ct);
		}

		///
		template <class T>
		inline 
		T min(const T& a, const T& b)
		{ 
			return BALL_MIN(a, b);
		}

		///
		template <class T>
		inline 
		T min(const T& a, const T& b, const T &ct)
		{ 
			return BALL_MIN3(a, b, ct);
		}

		///
		template <class T>
		inline 
		T round(const T& t)               
		{ 
			return (T)(t > 0 ? long(t + 0.5) : long(t - 0.5)); 
		}

		///
		template <class T>
		inline 
		T sgn(const T& t)
		{
			return BALL_SGN(t);
		}

		///
		template <class T>
		inline 
		bool isFinite(const T& t)
		{
			return (bool)finite(t);
		}

		///
		template <class T>
		inline 
		bool isInfinite(const T& t)
		{
			return (bool)(!Maths::isFinite(t) && !Maths::isNan(t));
		}

		///
		template <class T>
		inline 
		bool isNan(const T& t)
		{
			return (bool)isnan(t);
		}

		///
		template <class T>
		inline 
		bool isZero(const T& t)
		{
			return (abs(t) < Constants::EPSILON);
		}

		///
		template <class T>
		inline 
		bool isNotZero(const T& t)
		{
			return (abs(t) >= Constants::EPSILON);
		}

		///
		template <class T1, class T2>
		inline 
		bool isEqual(const T1& a, const T2& b)
		{
			return (abs(a - b) < Constants::EPSILON);
		}

		///
		template <class T1, class T2>
		inline 
		bool isNotEqual(const T1& a, const T2& b)
		{
			return (abs(a - b) >= Constants::EPSILON);
		}

		///
		template <class T1, class T2>
		inline 
		bool isLess(const T1& a, const T2& b)
		{
			return (a - b <= -Constants::EPSILON);
		}

		///
		template <class T1, class T2>
		inline 
		bool isLessOrEqual(const T1& a, const T2& b)
		{
			return (a - b < Constants::EPSILON);
		}

		///
		template <class T1, class T2>
		inline 
		bool isGreaterOrEqual(const T1& a, const T2& b)
		{
			return (a - b > -Constants::EPSILON);
		}

		///
		template <class T1, class T2>
		inline 
		bool isGreater(const T1& a, const T2& b)
		{
			return (a - b >= Constants::EPSILON);
		}

		///
		template <class T>
		inline 
		long floor(const T& t)
		{
			return (long)(Maths::isGreater(t, 0) ? t: (Maths::isEqual(t, (T)(long)t) ? t : t - 1));
		}

		///
		template <class T>
		inline 
		long ceiling(const T& t)
		{
			return (long)(Maths::isLess(t, 0) ? t: (Maths::isEqual(t, (T)(long)t) ? t : t + 1));
		}

		///
		template <class T1, class T2>
		inline 
		Index compare(const T1& a, const T2& b)
		{
			return (Maths::isLess(a, b) ? -1 : Maths::isEqual(a, b) ? 0 : 1);
		}


	//@}
		
	} // namespace Maths

} // namespace BALL

#endif // BALL_MATHS_COMPARISON_H

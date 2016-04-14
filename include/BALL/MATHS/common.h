// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: common.h,v 1.29.16.1 2007/03/25 21:23:45 oliver Exp $
//

#ifndef BALL_MATHS_COMMON_H
#define BALL_MATHS_COMMON_H

#ifndef BALL_CONFIG_CONFIG_H
#	include <BALL/CONFIG/config.h>
#endif

#include <cmath>

#ifdef BALL_HAS_IEEEFP_H
# include <ieeefp.h>
#endif

#ifdef BALL_HAS_FLOAT_H
# include <float.h>
#endif

#ifndef BALL_COMMON_CONSTANTS_H
#	include <BALL/COMMON/constants.h>
#endif

#ifndef BALL_COMMON_GLOBAL_H
#	include <BALL/COMMON/global.h>
#endif

#ifndef BALL_COMMON_MACROS_H
#	include <BALL/COMMON/macros.h>
#endif

namespace BALL 
{
	
	namespace Maths 
	{

		/**	\defgroup MathsCommon Common Mathematical Functions
				<b>Namespace:</b> <tt>BALL::Maths</tt> \par
				
		*/
		//@{ 

		/**	Return the absolute value of a number.
				@param	t the number
				@return T the absolute value
		*/
		template <typename T>
		inline 
		T abs(const T& t)
		{
			return BALL_ABS(t);
		}

		/**	Return the fraction of a number.
				@param	t the number
				@return T the fraction
		*/
		template <typename T>
		inline 
		T frac(const T& t)
		{ 
			long tmp = (long)t;
			return (t - (T)tmp);
		}

#ifndef max 
		/**	Return the greater of two numbers.
				@param	a the first number
				@param	b the second number
				@return T the greatest number
		*/
		template <typename T>
		inline 
		T max(const T& a, const T& b)
		{ 
			return BALL_MAX(a, b);
		}

		/**	Return the greatest of three numbers.
				@param	a the first number
				@param	b the second number
				@param	ct the third number
				@return T the greatest number
		*/
		template <typename T>
		inline 
		T max(const T& a, const T& b, const T &ct)
		{ 
			return BALL_MAX3(a, b, ct);
		}
#endif

#ifndef min
		/**	Return the smallest of two numbers.
				@param	a the first number
				@param	b the second number
				@return T the smallest number
		*/
		template <typename T>
		inline 
		T min(const T& a, const T& b)
		{ 
			return BALL_MIN(a, b);
		}

		/**	Return the smallest of three numbers.
				@param	a the first number
				@param	b the second number
				@param	ct the third number
				@return T the smallest number
		*/
		template <typename T>
		inline 
		T min(const T& a, const T& b, const T &ct)
		{ 
			return BALL_MIN3(a, b, ct);
		}
#endif

		/**	Round a number and return the result.
				@param	t the number
				@return T the result
		*/
		template <typename T>
		inline 
		T round(const T& t)
		{ 
			return (T)(t > 0 ? long(t + 0.5) : long(t - 0.5)); 
		}

		/**	Return the sign of a number.
				@param	t the number
				@return Index <tt>-1</tt> t < 0;  <tt>0</tt> t = 0; <tt>1</tt> t > 0
		*/
		template <typename T>
		inline 
		T sgn(const T& t)
		{
			return BALL_SGN(t);
		}

		/**	Test whether a number is finite.
				@param	t the number
				@return bool, <b>true</b> if <tt>t</tt> is finite
		*/
		template <typename T>
		inline 
		bool isFinite(const T& t)
		{
			return std::isfinite(t);
		}

		/**	Test whether a value is not a number.
				@param	t the number
				@return bool, <b>true</b> if t equals <tt>nan</tt>
		*/
		template <typename T>
		inline 
		bool isNan(const T& t)
		{
			#ifdef BALL_COMPILER_MSVC
				return (_isnan(t) != 0);
			#elif  defined(BALL_OS_DARWIN)
				return ( __inline_isnand(t) != 0);
			#else
				using std::isnan;
				return (isnan(t) != 0);
			#endif
		}

		/**	Test whether a number is infinite.
				@param	t the number
				@return bool, <b>true</b> if <tt>t</tt> equals <tt>inf</tt> or <tt>-inf</tt>
		*/
		template <typename T>
		inline 
		bool isInfinite(const T& t)
		{
			return (!Maths::isFinite(t) && !Maths::isNan(t));
		}

		/**	Test whether a number is zero.
				@param	t the number
				@return bool, <b>true</b> if the absolute value of <tt>t</tt> is below  \link Constants::EPSILON Constants::EPSILON \endlink 
		*/
		template <typename T>
		inline 
		bool isZero(const T& t)
		{
			return (abs(t) < Constants::EPSILON);
		}

		/**	Test whether a number is not zero.
				@param	t the number
				@return bool, <b>true</b>, if the absolute value of <tt>t</tt> is at least  \link Constants::EPSILON Constants::EPSILON \endlink 
		*/
		template <typename T>
		inline 
		bool isNotZero(const T& t)
		{
			return (abs(t) >= Constants::EPSILON);
		}

		/**	Test whether a number is equal to another.
				@param	a the first number
				@param	b the second number
				@return bool, <b>true</b> if the absolute distance of <tt>a</tt> and <tt>b</tt> is below  \link Constants::EPSILON Constants::EPSILON \endlink 
		*/
		template <typename T1, typename T2>
		inline 
		bool isEqual(const T1& a, const T2& b)
		{
			return (abs(a - b) < Constants::EPSILON);
		}

		/**	Test whether a number is not equal to another.
				@param	a the first number
				@param	b the second number
				@return bool, <b>true</b> if the absolute distance of <tt>a</tt> and <tt>b</tt> is at least  \link Constants::EPSILON Constants::EPSILON \endlink 
		*/
		template <typename T1, typename T2>
		inline 
		bool isNotEqual(const T1& a, const T2& b)
		{
			return (abs(a - b) >= Constants::EPSILON);
		}

		/**	Test whether a number is less compared to another.
				@param	a the first number
				@param	b the second number
				@return bool, <b>true</b> if <tt>a</tt> is smaller than <tt>b</tt>
		*/
		template <typename T1, typename T2>
		inline 
		bool isLess(const T1& a, const T2& b)

		{
			return ((a - b) <= -Constants::EPSILON);
		}

		/**	Test whether a number is less or equal compared to another.
				@param	a the first number
				@param	b the second number
				@return bool, <b>true</b> if <tt>a</tt> is less or equal <tt>b</tt>
		*/
		template <typename T1, typename T2>
		inline 
		bool isLessOrEqual(const T1& a, const T2& b)
		{
			return ((a - b) < Constants::EPSILON);
		}

		/**	Test whether a number is greater or equal compared to another.
				@param	a the first number
				@param	b the second number
				@return bool, <b>true</b> if <tt>a</tt> is greater or equal than <tt>b</tt>
		*/
		template <typename T1, typename T2>
		inline 
		bool isGreaterOrEqual(const T1& a, const T2& b)
		{
			return ((a - b) > -Constants::EPSILON);
		}

		/**	Test whether a number is greater compared to another.
				@param	a the first number
				@param	b the second number
				@return bool, <b>true</b> if <tt>a</tt> is greater than <tt>b</tt>
		*/
		template <typename T1, typename T2>
		inline 
		bool isGreater(const T1& a, const T2& b)
		{
			return (a - b >= Constants::EPSILON);
		}

		/**	Return the floor of a number.
				@param	t the number
				@return T the floor
		*/
		template <typename T>
		inline 
		long floor(const T& t)
		{
			return (long)(Maths::isGreater(t, 0) ? t: (Maths::isEqual(t, (T)(long)t) ? t : t - 1));
		}

		/**	Return the ceiling of a number.
				@param	t the number
				@return T the ceiling
		*/
		template <typename T>
		inline 
		long ceiling(const T& t)
		{
			return (long)(Maths::isLess(t, 0) ? t: (Maths::isEqual(t, (T)(long)t) ? t : t + 1));
		}

		/**	Compare two numbers to each other.
				@param	a the first number
				@param	b the second number
				@return Index <tt>-1</tt> a < b; <tt>0</tt> a = b; <tt>1</tt> a > b
		*/
		template <typename T1, typename T2>
		inline 
		Index compare(const T1& a, const T2& b)
		{
			return (Maths::isLess(a, b) ? -1 : Maths::isEqual(a, b) ? 0 : 1);
		}

		/**	Test whether two numbers are close to each other.
				@param	a the first number
				@param	b the second number
				@param	max_diff the maximum allowed difference between the two numbers
				@return bool, <b>true</b> if the absolute distance between <tt>a</tt> and <tt>b</tt> is below <tt>max_diff</tt>
		*/
		template <typename T>
		inline 
		bool isNear(const T& a, const T& b, const T& max_diff)
		{
			return (abs((double)a - (double)b) < abs((double)max_diff));
		}


		/// round to integral value in floating-point format
		inline double rint(double x)
		{
			if (x < 0.0) return (double)(int)(x - 0.5);
			else return (double)(int)(x + 0.5);
		}

	//@}
		
	} // namespace Maths
} // namespace BALL

#endif // BALL_MATHS_COMMON_H

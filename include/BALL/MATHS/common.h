// $Id: common.h,v 1.7 2000/04/04 21:29:22 oliver Exp $

#ifndef BALL_MATHS_COMPARISON_H
#define BALL_MATHS_COMPARISON_H

#ifndef BALL_CONFIG_CONFIG_H
#	include <BALL/CONFIG/config.h>
#endif

#include <math.h>

#ifdef BALL_INCLUDE_IEEEFP
# include <ieeefp.h>
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

		/**	Return the absolute value of a number.
				@param	t the number
				@return T the absolute value
		*/
		template <class T>
		inline 
		T abs(const T& t)
		{
			return BALL_ABS(t);
		}

		/**	Return the fraction of a number.
				@param	t the number
				@return T the fraction
		*/
		template <class T>
		inline 
		T frac(const T& t)
		{ 
			long tmp = (long)t;
			return (t - (T)tmp);
		}

		/**	Return the greater of two numbers.
				@param	a the first number
				@param	b the second number
				@return T the greatest number
		*/
		template <class T>
		inline 
		T max(const T& a, const T& b)
		{ 
			return BALL_MAX(a, b);
		}

		/**	Return the greatest of three numbers.
				@param	a the first number
				@param	b the second number
				@param	c the third number
				@return T the greatest number
		*/
		template <class T>
		inline 
		T max(const T& a, const T& b, const T &ct)
		{ 
			return BALL_MAX3(a, b, ct);
		}

		/**	Return the smallest of two numbers.
				@param	a the first number
				@param	b the second number
				@return T the smallest number
		*/
		template <class T>
		inline 
		T min(const T& a, const T& b)
		{ 
			return BALL_MIN(a, b);
		}

		/**	Return the smallest of three numbers.
				@param	a the first number
				@param	b the second number
				@param	c the third number
				@return T the smallest number
		*/
		template <class T>
		inline 
		T min(const T& a, const T& b, const T &ct)
		{ 
			return BALL_MIN3(a, b, ct);
		}

		/**	Round a number and return the result.
				@param	t the number
				@return T the result
		*/
		template <class T>
		inline 
		T round(const T& t)               
		{ 
			return (T)(t > 0 ? long(t + 0.5) : long(t - 0.5)); 
		}

		/**	Return the sign of a number.
				@param	t the number
				@return Index {\tt -1} t < 0;  {\tt 0} t = 0; {\tt 1} t > 0
		*/
		template <class T>
		inline 
		T sgn(const T& t)
		{
			return BALL_SGN(t);
		}

		/**	Test if a number is finite.
				@param	t the number
				@return bool, {\bf true} if {\tt t} is finite
		*/
		template <class T>
		inline 
		bool isFinite(const T& t)
		{
			return (bool)finite(t);
		}

		/**	Test if a number is infinite.
				@param	t the number
				@return bool, {\bf true} if {\tt t} equals {\tt inf} or {\tt -inf}
		*/
		template <class T>
		inline 
		bool isInfinite(const T& t)
		{
			return (bool)(!Maths::isFinite(t) && !Maths::isNan(t));
		}

		/**	Test if a value is not a number.
				@param	t the number
				@return bool, {\bf true} if t equals {\tt nan}
		*/
		template <class T>
		inline 
		bool isNan(const T& t)
		{
			return (bool)isnan(t);
		}

		/**	Test if a number is zero.
				@param	t the number
				@return bool, {\bf true} if the absolute value of {\tt t} is below \Ref{Constants::EPSILON}
		*/
		template <class T>
		inline 
		bool isZero(const T& t)
		{
			return (abs(t) < Constants::EPSILON);
		}

		/**	Test if a number is not zero.
				@param	t the number
				@return bool, {\bf true}, if the absolute value of {\tt t} is at leas \Ref{Constants::EPSILON}
		*/
		template <class T>
		inline 
		bool isNotZero(const T& t)
		{
			return (abs(t) >= Constants::EPSILON);
		}

		/**	Test if a number is equal to an other.
				@param	a the first number
				@param	b the second number
				@return bool, {\bf true} if the absolute distance of {\tt a} and {\tt b} is below \Ref{Constants::EPSILON}
		*/
		template <class T1, class T2>
		inline 
		bool isEqual(const T1& a, const T2& b)
		{
			return (abs(a - b) < Constants::EPSILON);
		}

		/**	Test if a number is not equal to an other.
				@param	a the first number
				@param	b the second number
				@return bool, {\bf true} if the absolute distance of {\tt a} and {\tt b} is at least \Ref{Constants::EPSILON}
		*/
		template <class T1, class T2>
		inline 
		bool isNotEqual(const T1& a, const T2& b)
		{
			return (abs(a - b) >= Constants::EPSILON);
		}

		/**	Test if a number is less compared to an other.
				@param	a the first number
				@param	b the second number
				@return bool, {\bf true} if {\tt a} is smaller than {\tt b}
		*/
		template <class T1, class T2>
		inline 
		bool isLess(const T1& a, const T2& b)
		{
			return (a - b <= -Constants::EPSILON);
		}

		/**	Test if a number is less or equal compared to an other.
				@param	a the first number
				@param	b the second number
				@return bool, {\bf true} if {\tt a} is less or equal {\tt b}
		*/
		template <class T1, class T2>
		inline 
		bool isLessOrEqual(const T1& a, const T2& b)
		{
			return (a - b < Constants::EPSILON);
		}

		/**	Test if a number is greater or equal compared to another.
				@param	a the first number
				@param	b the second number
				@return bool, {\bf true} if {\tt a} is greater or equal than {\tt b}
		*/
		template <class T1, class T2>
		inline 
		bool isGreaterOrEqual(const T1& a, const T2& b)
		{
			return (a - b > -Constants::EPSILON);
		}

		/**	Test if a number is greater compared to an other.
				@param	a the first number
				@param	b the second number
				@return bool, {\bf true} if {\tt a} is greater than {\tt b}
		*/
		template <class T1, class T2>
		inline 
		bool isGreater(const T1& a, const T2& b)
		{
			return (a - b >= Constants::EPSILON);
		}

		/**	Return the floor of an number.
				@param	t the number
				@return T the floor
		*/
		template <class T>
		inline 
		long floor(const T& t)
		{
			return (long)(Maths::isGreater(t, 0) ? t: (Maths::isEqual(t, (T)(long)t) ? t : t - 1));
		}

		/**	Return the ceiling of an number.
				@param	t the number
				@return T the ceiling
		*/
		template <class T>
		inline 
		long ceiling(const T& t)
		{
			return (long)(Maths::isLess(t, 0) ? t: (Maths::isEqual(t, (T)(long)t) ? t : t + 1));
		}

		/**	Compare two numbers to eachother.
				@param	a the first number
				@param	b the second number
				@return Index {tt -1} a < b; {tt 0} a = b; {tt 1} a > b;
		*/
		template <class T1, class T2>
		inline 
		Index compare(const T1& a, const T2& b)
		{
			return (Maths::isLess(a, b) ? -1 : Maths::isEqual(a, b) ? 0 : 1);
		}

		/**	Test if two numbers are close each other.
				@param	a the first number
				@param	b the second number
				@param	max_diff the maximum allowed difference between the two numbers
				@return bool, {\bf true} if the absolute distance between {\tt a} and {\tt b} is below {\tt max_diff}
		*/
		template <class T>
		inline 
		bool isNear(const T& a, const T& b, const T& max_diff)
		{
			return (abs((double)a - (double)b) < abs((double)max_diff));
		}

	//@}
		
	} // namespace Maths

} // namespace BALL

#endif // BALL_MATHS_COMPARISON_H

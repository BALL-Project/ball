// $Id: limits.h,v 1.8 2001/07/18 08:24:51 sturm Exp $

#ifndef BALL_COMMON_LIMITS_H
#define BALL_COMMON_LIMITS_H

#ifndef BALL_CONFIG_CONFIG_H
#	include <BALL/CONFIG/config.h>
#endif

#ifdef BALL_HAVE_NUMERIC_LIMITS
#	include <limits>
#else
#	include <limits.h>
#	ifdef BALL_INCLUDE_FLOAT_H
#		include <float.h>
#	endif
#endif

namespace BALL 
{
	

	/**	Numeric limits class.
			This template class describes the minimum and maximum values for
			the differnet data types.
			It a substitute for the {\tt numeric_limits} class described in the
			ISO/ANSI standard (chapter 18.2.1.1), as most compiler still do not support
			this feature (or more precisely: the header file <limits> is not implemented).
			If <limits> exists, Limits is just a wrapper around the {\tt numeric_limits} class,
			otherwise it returns the value from the standard C header <limits.h>.\\
			{\bf Definition:} \URL{BALL/COMMON/limits.h}
	*/
	template <typename T>
	class Limits
	{
		public:

#	ifdef BALL_HAVE_NUMERIC_LIMITS


		/**	Return the minimum value.
				@return T the minimum value for this datatype
		*/
		static T min() throw() 
		{ 
			return std::numeric_limits<T>::min(); 
		}

		/**	Return the maximum value.
				@return T the maximum value for this datatype
		*/
		static T max() throw() 
		{ 
			return std::numeric_limits<T>::max(); 
		}
#	else
		static T min() throw() 
		{ 
			return (T)0; 
		}
		static T max() throw() 
		{ 
			return (T)0; 
		}
#	endif
	};

#ifndef BALL_HAVE_NUMERIC_LIMITS
	
	template <> 
	class Limits<float>
	{
		public:
			
		static float min() throw()
		{
			return FLT_MIN;
		}

		static float max() throw()
		{
			return FLT_MAX;
		}
	};

	template <> 
	class Limits<double>
	{
		public:
			
		static double min() throw()
		{
			return DBL_MIN;
		}

		static double max() throw()
		{
			return DBL_MAX;
		}
	};

	template <> 
	class Limits<bool>
	{
		public:
			
		static bool min() throw()
		{
			return false;
		}

		static bool max() throw()
		{
			return true;
		}
	};

	template <> 
	class Limits<char>
	{
		public:
			
		static char min() throw()
		{
			return CHAR_MIN;
		}

		static char max() throw()
		{
			return CHAR_MAX;
		}
	};

	template <> 
	class Limits<signed char>
	{
		public:
			
		static signed char min() throw()
		{
			return SCHAR_MIN;
		}

		static signed char max() throw()
		{
			return SCHAR_MAX;
		}
	};

	template <> 
	class Limits<unsigned char>
	{
		public:
			
		static unsigned char min() throw()
		{
			return 0;
		}

		static unsigned char max() throw()
		{
			return UCHAR_MAX;
		}
	};

	template <> 
	class Limits<short>
	{
		public:
			
		static short min() throw()
		{
			return SHRT_MIN;
		}

		static short max() throw()
		{
			return SHRT_MAX;
		}
	};

	template <> 
	class Limits<unsigned short>
	{
		public:
			
		static unsigned short min() throw()
		{
			return 0;
		}

		static unsigned short max() throw()
		{
			return USHRT_MAX;
		}
	};

	template <> 
	class Limits<int>
	{
		public:
			
		static int min() throw()
		{
			return INT_MIN;
		}

		static int max() throw()
		{
			return INT_MAX;
		}
	};

	template <> 
	class Limits<unsigned int>
	{
		public:
			
		static unsigned int min() throw()
		{
			return 0;
		}

		static unsigned int max() throw()
		{
			return UINT_MAX;
		}
	};

	template <> 
	class Limits<long>
	{
		public:
			
		static long min() throw()
		{
			return LONG_MIN;
		}

		static long max() throw()
		{
			return LONG_MAX;
		}
	};

	template <> 
	class Limits<unsigned long>
	{
		public:
			
		static unsigned long min() throw()
		{
			return 0;
		}

		static unsigned long max() throw()
		{
			return ULONG_MAX;
		}
	};

#endif // BALL_HAVE_NUMERIC_LIMITS

}	// namespace BALL

#endif // BALL_COMMON_LIMITS_H

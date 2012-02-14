// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_COMMON_LIMITS_H
#define BALL_COMMON_LIMITS_H

#ifndef BALL_CONFIG_CONFIG_H
#	include <BALL/CONFIG/config.h>
#endif

#ifndef BALL_COMMON_GLOBAL_H
# include <BALL/COMMON/global.h>
#endif

#ifdef BALL_HAS_NUMERIC_LIMITS
#	include <limits>
#else
#	ifdef BALL_HAS_LIMITS_H
#		include <limits.h>
#	endif
#	ifdef BALL_HAS_VALUES_H
#		include <limits.h>
#	endif
#endif

#	ifdef BALL_HAS_FLOAT_H
#		include <float.h>
#	endif

namespace BALL 
{
	

	/**	Numeric limits class.
			This template class describes the minimum and maximum values for
			the differnet data types.
			It a substitute for the <tt>numeric_limits</tt> class described in the
			ISO/ANSI standard (chapter 18.2.1.1), as most compiler still do not support
			this feature (or more precisely: the header file \<limits\> is not implemented).
			If \<limits\> exists, Limits is just a wrapper around the <tt>numeric_limits</tt> class,
			otherwise it returns the value from the standard C header \<limits.h\>. \par
			
	    \ingroup Common
	*/
	template <typename T>
	class BALL_EXPORT BALL_DEPRECATED Limits
	{
		public:

#ifdef BALL_HAS_NUMERIC_LIMITS


		/**	Return the minimum value.
				@return T the minimum value for this datatype
		*/
		static T min()  
		{ 
			return std::numeric_limits<T>::min(); 
		}

		/**	Return the maximum value.
				@return T the maximum value for this datatype
		*/
		static T max()  
		{ 
			return std::numeric_limits<T>::max(); 
		}
#else
		static T min()  
		{ 
			return (T)0; 
		}
		static T max()  
		{ 
			return (T)0; 
		}
#endif
	};

#ifndef BALL_HAS_NUMERIC_LIMITS
	
	template <> 
	class BALL_EXPORT Limits<float>
	{
		public:
			
		static float min() 
		{
			return FLT_MIN;
		}

		static float max() 
		{
			return FLT_MAX;
		}
	};

	template <> 
	class BALL_EXPORT Limits<double>
	{
		public:
			
		static double min() 
		{
			return DBL_MIN;
		}

		static double max() 
		{
			return DBL_MAX;
		}
	};

	template <> 
	class BALL_EXPORT Limits<bool>
	{
		public:
			
		static bool min() 
		{
			return false;
		}

		static bool max() 
		{
			return true;
		}
	};

	template <> 
	class BALL_EXPORT Limits<char>
	{
		public:
			
		static char min() 
		{
			return CHAR_MIN;
		}

		static char max() 
		{
			return CHAR_MAX;
		}
	};

	template <> 
	class BALL_EXPORT Limits<signed char>
	{
		public:
			
		static signed char min() 
		{
			return SCHAR_MIN;
		}

		static signed char max() 
		{
			return SCHAR_MAX;
		}
	};

	template <> 
	class BALL_EXPORT Limits<unsigned char>
	{
		public:
			
		static unsigned char min() 
		{
			return 0;
		}

		static unsigned char max() 
		{
			return UCHAR_MAX;
		}
	};

	template <> 
	class BALL_EXPORT Limits<short>
	{
		public:
			
		static short min() 
		{
			return SHRT_MIN;
		}

		static short max() 
		{
			return SHRT_MAX;
		}
	};

	template <> 
	class BALL_EXPORT Limits<unsigned short>
	{
		public:
			
		static unsigned short min() 
		{
			return 0;
		}

		static unsigned short max() 
		{
			return USHRT_MAX;
		}
	};

	template <> 
	class BALL_EXPORT Limits<int>
	{
		public:
			
		static int min() 
		{
			return INT_MIN;
		}

		static int max() 
		{
			return INT_MAX;
		}
	};

	template <> 
	class BALL_EXPORT Limits<unsigned int>
	{
		public:
			
		static unsigned int min() 
		{
			return 0;
		}

		static unsigned int max() 
		{
			return UINT_MAX;
		}
	};

	template <> 
	class BALL_EXPORT Limits<long>
	{
		public:
			
		static long min() 
		{
			return LONG_MIN;
		}

		static long max() 
		{
			return LONG_MAX;
		}
	};

	template <> 
	class BALL_EXPORT Limits<unsigned long>
	{
		public:
			
		static unsigned long min() 
		{
			return 0;
		}

		static unsigned long max() 
		{
			return ULONG_MAX;
		}
	};

#endif // BALL_HAS_NUMERIC_LIMITS

}	// namespace BALL

#endif // BALL_COMMON_LIMITS_H

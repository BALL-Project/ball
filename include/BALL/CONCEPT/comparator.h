// $Id: comparator.h,v 1.1 1999/08/26 07:53:16 oliver Exp $

#ifndef BALL_CONCEPT_COMPARATOR_H
#define BALL_CONCEPT_COMPARATOR_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

namespace BALL {

	/**	Generic Comparator Class.
			{\bf Definition:} \URL{BALL/CONCEPT/comparator.h}
	*/
	template <class T>
	class Comparator
	{
		public:

		/**	@name Constructors and Destructors
		*/
		//@{
	
		/**	Default constructor
		*/
		Comparator()
		{
		}

		/**	Destructor
		*/
		virtual ~Comparator()
		{
		}
		//@}

		/**	@name	Predicates
		*/
		//@{

		/**
		*/
		virtual bool isEqual(const T& a, const T& b) const;

		/**
		*/
		bool isNotEqual(const T& a, const T& b) const;

		/**
		*/
		virtual bool isLess(const T& a, const T& b) const;

		/**
		*/
		bool isLessOrEqual(const T& a, const T& b) const;

		/**
		*/
		bool isGreaterOrEqual(const T& a, const T& b) const;

		/**
		*/
		bool isGreater(const T& a, const T& b) const;

		/**
		*/
		int operator ()(const T& a, const T& b) const;
		//@}
	};

	template <class T>
	inline bool Comparator<T>::isEqual(const T& a, const T& b) const
	{
		return (bool)(a == b);
	}

	template <class T>
	inline bool Comparator<T>::isNotEqual(const T& a, const T& b) const
	{
		return (bool)!isEqual(a, b);
	}

	template <class T>
	inline bool Comparator<T>::isLess(const T& a, const T& b) const
	{
		return (bool)(a < b);
	}

	template <class T>
	inline bool Comparator<T>::isLessOrEqual(const T& a, const T& b) const
	{
		return (bool)!isLess(b, a);
	}

	template <class T>
	inline bool Comparator<T>::isGreaterOrEqual(const T& a, const T& b) const
	{
		return (bool)!isLess(a, b);
	}

	template <class T>
	inline bool Comparator<T>::isGreater(const T& a, const T& b) const
	{
		return isLess(b, a);
	}

	template <class T>
	inline int Comparator<T>::operator () (const T& a,const T& b) const
	{
		if (isEqual(a, b) == true)
		{ 
			return 0;
		} else if (isLess(a, b) == true)
		{ 
			return -1;
		} else { 
				return 1;
		}
	}

} // namespace BALL

#endif // BALL_CONCEPT_COMPARATOR_H

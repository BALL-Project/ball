// $Id: comparator.h,v 1.4 2000/12/09 20:52:56 amoll Exp $

#ifndef BALL_CONCEPT_COMPARATOR_H
#define BALL_CONCEPT_COMPARATOR_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

namespace BALL 
{

	/**	Generic Comparator Class.
			{\bf Definition:} \URL{BALL/CONCEPT/comparator.h}
	*/
	template <typename T>
	class Comparator
	{
		public:

		/**	@name Constructors and Destructors
		*/
		//@{
	
		/**	Default constructor
		*/
		Comparator()
			throw();

		/**	Destructor
		*/
		virtual ~Comparator()
			throw();
		//@}

		/**	@name	Predicates
		*/
		//@{

		/**
		*/
		virtual bool isEqual(const T& a, const T& b) const
			throw();

		/**
		*/
		bool isNotEqual(const T& a, const T& b) const
			throw();

		/**
		*/
		virtual bool isLess(const T& a, const T& b) const
			throw();

		/**
		*/
		bool isLessOrEqual(const T& a, const T& b) const
			throw();

		/**
		*/
		bool isGreaterOrEqual(const T& a, const T& b) const
			throw();

		/**
		*/
		bool isGreater(const T& a, const T& b) const
			throw();

		/**
		*/
		int operator () (const T& a, const T& b) const
			throw();
		//@}
	};

	template <typename T>
	inline Comparator<T>::Comparator()
		throw()
	{
	}

	template <typename T>
	inline Comparator<T>::~Comparator()
		throw()
	{
	}

	template <class T>
	inline bool Comparator<T>::isEqual(const T& a, const T& b) const
		throw()
	{
		return (a == b);
	}

	template <class T>
	inline bool Comparator<T>::isNotEqual(const T& a, const T& b) const
		throw()
	{
		return !isEqual(a, b);
	}

	template <class T>
	inline bool Comparator<T>::isLess(const T& a, const T& b) const
		throw()
	{
		return (a < b);
	}

	template <class T>
	inline bool Comparator<T>::isLessOrEqual(const T& a, const T& b) const
		throw()
	{
		return !isLess(b, a);
	}

	template <class T>
	inline bool Comparator<T>::isGreaterOrEqual(const T& a, const T& b) const
		throw()
	{
		return !isLess(a, b);
	}

	template <class T>
	inline bool Comparator<T>::isGreater(const T& a, const T& b) const
		throw()
	{
		return isLess(b, a);
	}

	template <class T>
	inline int Comparator<T>::operator () (const T& a,const T& b) const
		throw()
	{
		if (isEqual(a, b) == true)
		{ 
			return 0;
		} 

		if (isLess(a, b) == true)
		{ 
			return -1;
		} 
		else 
		{ 
			return 1;
		}
	}

} // namespace BALL

#endif // BALL_CONCEPT_COMPARATOR_H

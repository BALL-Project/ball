// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: predicate.h,v 1.5 2003/02/21 16:01:40 anhi Exp $

#ifndef BALL_CONCEPT_PREDICATE_H
#define BALL_CONCEPT_PREDICATE_H

#include <functional>

namespace BALL 
{
	
	using std::unary_function;
	using std::binary_function;
		
	/**	@name	Predicates
			<b>Definition:</b> BALL/CONCEPT/predicate.h
	*/
	//@{
	
	/**	Generic Unary Predicate Class
	*/
	template <typename T> 
	class UnaryPredicate 
		: public unary_function<T, bool> 
	{
		public:

		///
		virtual bool operator() (const T& /* x */) const
			throw()
		{
			return true;
		}
	};

	/**	Generic Binary Predicate Class
	*/
	template <typename T1, typename T2> 
	class BinaryPredicate 
		: public binary_function<T1, T2, bool> 
	{
		public:

		///
		virtual bool operator() (const T1& x, const T2& y) const
			throw()
		{
			return true;
		}
	};

	//@}

} // namespace BALL


#endif // BALL_CONCEPT_PREDICATE_H

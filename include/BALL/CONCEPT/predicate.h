// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: predicate.h,v 1.12.6.7 2005/08/11 23:26:18 amoll Exp $
//

#ifndef BALL_CONCEPT_PREDICATE_H
#define BALL_CONCEPT_PREDICATE_H

#ifndef BALL_COMMON_GLOBAL_H
# include <BALL/COMMON/global.h>
#endif

#include <functional>

namespace BALL 
{
	using std::unary_function;
	using std::binary_function;
		
	/**	@name	Predicates
			
			\ingroup ConceptsMiscellaneous
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
		virtual ~UnaryPredicate() {}

		///
		virtual bool operator() (const T& /* x */) const
			throw();
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
			throw();

		///
    virtual ~BinaryPredicate() {}
	};

	template <typename T> 
	bool UnaryPredicate<T>::operator() (const T& /* x */) const
		throw()
	{
		return true;
	}

	template <typename T1, typename T2> 
	bool BinaryPredicate<T1, T2>::operator() (const T1&, const T2&) const
		throw()
	{
		return true;
	}
	//@}
} // namespace BALL


#endif // BALL_CONCEPT_PREDICATE_H

// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
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
			;
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
			;

		///
    virtual ~BinaryPredicate() {}
	};

	template <typename T> 
	bool UnaryPredicate<T>::operator() (const T& /* x */) const
		
	{
		return true;
	}

	template <typename T1, typename T2> 
	bool BinaryPredicate<T1, T2>::operator() (const T1&, const T2&) const
		
	{
		return true;
	}
	//@}
} // namespace BALL


#endif // BALL_CONCEPT_PREDICATE_H

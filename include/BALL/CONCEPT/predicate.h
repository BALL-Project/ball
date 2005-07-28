// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: predicate.h,v 1.12.6.4 2005/07/28 13:50:51 amoll Exp $
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
	class BALL_EXPORT UnaryPredicate 
		: public unary_function<T, bool> 
	{
		public:
		///
		virtual ~UnaryPredicate() {}

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
	class BALL_EXPORT BinaryPredicate 
		: public binary_function<T1, T2, bool> 
	{
		public:

		///
		virtual bool operator() (const T1& x, const T2& y) const
			throw()
		{
			return true;
		}
		///
    virtual ~BinaryPredicate() {}
	};

	//@}
} // namespace BALL


#endif // BALL_CONCEPT_PREDICATE_H

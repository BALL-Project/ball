// $Id: predicate.h,v 1.1 1999/08/26 07:53:17 oliver Exp $

#ifndef BALL_CONCEPT_PREDICATE_H
#define BALL_CONCEPT_PREDICATE_H

#include <functional>

namespace BALL {
	
	using std::unary_function;
	using std::binary_function;
		
	/**	@name	Predicates
			{\bf Definition:} \URL{BALL/CONCEPT/predicate.h}
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
		{
			return true;
		}
	};

	//@}

} // namespace BALL


#endif // BALL_CONCEPT_PREDICATE_H

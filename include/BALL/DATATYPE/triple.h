// $Id: triple.h,v 1.1 2001/07/20 07:59:45 oliver Exp $

#ifndef BALL_DATATYPE_TRIPLE_H
#define BALL_DATATYPE_TRIPLE_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

namespace BALL 
{

	/**	Triple Class.
			This template class is used to represent triples of arbitrary types
			(like the STL type {\tt pair}).
			@see Quadruple
	*/
	template <typename T1, typename T2, typename T3>
	class Triple
	{
		public:

		/**	@name	Constructors and Destructors 
		*/
		//@{

		BALL_CREATE(Triple);

		/**	Default constructor.
				Creates a new Triple object.
		*/
		Triple() throw();

		/**	Copy constructor.
				Creates a new Triple object from another.
				@param	triple the Triple object to be copied
				@param	deep not used (needed for a consistent interface only)
		*/
		Triple(const Triple& triple, bool deep = true) 
			throw();

		/**	Detailed constructor.
				Creates a new Triple object from the triple's three
				members.
				@param	new_first	the first triple member
				@param	new_second	the second triple member
				@param	new_third	the third triple member
		*/
		Triple(const T1& new_first, const T2& new_second, const T3& new_third) 
			throw();

		/**	Destructor.
				Destructs the Triple object.
		*/
		virtual ~Triple() throw();
		//@}

		/** Clear method.
		*/
		virtual void clear() throw();

		/**	@name	Assignment
		*/
		//@{

		/**	Assignment operator.
				Assigns the contents of a triple to another.
				@param	triple the triple to be copied
		*/
		const Triple& operator = (const Triple& triple) throw();

		/**
		*/
		void set(const T1& t1, const T2& t2, const T3& t3) throw();

		/**
		*/
		void get(T1& first, T2& second, T3& third) const throw();

		//@}
		/**	@name	Predicates
		*/
		//@{

		/**	Equality operator 
				Two instances are equal if they have the same members.
		*/
		bool operator == (const Triple& triple) const 
			throw();

		/**	Inequality operator 
		*/
		bool operator != (const Triple& triple) const throw();

		/**	Lesser than operator.	
				One instance is lesser than an other if all members from first to
				third are equal or less than the members of the other instance and at least
				threeth is less.
		*/
		bool operator < (const Triple& triple) const throw();

		/**	Lesser or equal than operator.	
		*/
		bool operator <= (const Triple& triple) const throw();

		/**	Greater or equal than operator.	
		*/
		bool operator >= (const Triple& triple) const throw();

		/**	Greater than operator.	
		*/
		bool operator > (const Triple& triple) const throw();

		//@}
		
		/**	@name	Attributes
		*/
		//@{

		/**	The first triple member
		*/
		T1 first;

		/**	The second triple member
		*/
		T2 second;

		/**	The third triple member
		*/
		T3 third;
		//@}
	};

	template <typename T1, typename T2, typename T3>
	Triple<T1, T2, T3>::Triple()  
		throw()
	{
	}

	template <typename T1, typename T2, typename T3>
	Triple<T1, T2, T3>::Triple
		(const Triple<T1, T2, T3>& triple, bool /* deep */)
		 throw()
		:	first(triple.first),
			second(triple.second),
			third(triple.third)	{
	}

	template <typename T1, typename T2, typename T3>
	Triple<T1, T2, T3>::Triple
		(const T1& new_first, const T2& new_second, const T3& new_third)
		 throw()
		:	first(new_first),
			second(new_second),
			third(new_third)
	{
	}

	template <typename T1, typename T2, typename T3>
	Triple<T1, T2, T3>::~Triple()  
		throw()
	{
	}

	template <typename T1, typename T2, typename T3>
	BALL_INLINE 
	void Triple<T1, T2, T3>::set(const T1& new_first, const T2& new_second, const T3& new_third)
	 throw()
	{
		first		= new_first;
		second	= new_second;
		third		= new_third;
	}

	template <typename T1, typename T2, typename T3>
	BALL_INLINE 
	const Triple<T1, T2, T3>& Triple<T1, T2, T3>::operator = 
		(const Triple<T1, T2, T3>& triple) throw()
	{
		first = triple.first;
		second = triple.second;
		third = triple.third;

		return *this;
	}

	template <typename T1, typename T2, typename T3>
	BALL_INLINE 
	void Triple<T1, T2, T3>::get(T1& t1, T2& t2, T3& t3) 
		const throw()
	{
		t1 = first;
		t2 = second;
		t3 = third;
	}

	template <typename T1, typename T2, typename T3>
	BALL_INLINE 
	bool Triple<T1, T2, T3>::operator ==	(const Triple& triple) 
		const throw()
	{
		return (first  == triple.first
									&& second == triple.second
									&& third  == triple.third);
	}

	template <typename T1, typename T2, typename T3>
	BALL_INLINE 
	bool Triple<T1, T2, T3>::operator != (const Triple& triple)
		const throw()
	{
		return (first != triple.first
									|| second != triple.second
									|| third  != triple.third);
	}

	template <typename T1, typename T2, typename T3>
	BALL_INLINE 
	void Triple<T1, T2, T3>::clear() 
		throw()
	{
		first = T1();
		second = T2();
		third = T3();
	}

	template <typename T1, typename T2, typename T3>
	BALL_INLINE 
	bool Triple<T1, T2, T3>::operator <
		(const Triple<T1, T2, T3>& triple) const throw()
	{
		return ((first < triple.first)
						|| ((first == triple.first) && (second < triple.second))
						|| ((first  == triple.first) && (second == triple.second) && (third < triple.third)));
	}

	template <typename T1, typename T2, typename T3>
	BALL_INLINE 
	bool Triple<T1, T2, T3>::operator <=
		(const Triple<T1, T2, T3>& triple) const throw()
	{
		return ((first < triple.first) 
						|| ((first == triple.first)	&& (second < triple.second))
						|| ((first == triple.first)	&& (second == triple.second) && (third < triple.third)));
	}

	template <typename T1, typename T2, typename T3>
	BALL_INLINE 
	bool Triple<T1, T2, T3>::operator >=
		(const Triple<T1, T2, T3>& triple) const throw()
	{
		return ((first > triple.first)
						|| ((first == triple.first)	&& (second > triple.second)) 
						|| ((first == triple.first)	&& (second == triple.second) && (third > triple.third)));
	}

	template <typename T1, typename T2, typename T3>
	BALL_INLINE 
	bool Triple<T1, T2, T3>::operator >
		(const Triple<T1, T2, T3>& triple) const throw()
	{
		return ((first > triple.first)
						|| ((first == triple.first)	&& (second > triple.second))	
						|| ((first == triple.first)	&& (second == triple.second) && (third > triple.third)));
	}

} // namespace BALL

#endif // BALL_DATATYPE_TRIPLE_H

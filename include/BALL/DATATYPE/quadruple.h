// $Id: quadruple.h,v 1.4 2000/12/15 13:08:40 amoll Exp $

#ifndef BALL_DATATYPE_QUADRUPLE_H
#define BALL_DATATYPE_QUADRUPLE_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

namespace BALL 
{

	/**	Quadruple Class.
			This template class is used to represent quadruples of arbitrary types
			(like the STL type {\tt pair}).
	*/
	template <typename T1, typename T2, typename T3, typename T4>
	class Quadruple
	{
		public:

		/**	@name	Constructors and Destructors 
		*/
		//@{

		BALL_CREATE(Quadruple);

		/**	Default constructor.
				Creates a new Quadruple object.
		*/
		Quadruple() throw();

		/**	Copy constructor.
				Creates a new Quadruple object from another.
				@param	quadruple the Quadruple object to be copied
				@param	cone_deep not used (needed for a consistent interface only)
		*/
		Quadruple(const Quadruple& quadruple, bool deep = true) throw();

		/**	Detailled constructor.
				Creates a new Quadruple object from the quadruple's four
				members.
				@param	new_first	the first quadruple member
				@param	new_second	the second quadruple member
				@param	new_third	the third quadruple member
				@param	new_fourth	the fourth quadruple member
		*/
		Quadruple(const T1& new_first, const T2& new_second,
							const T3& new_third, const T4& new_fourth) throw();

		/**	Destructor.
				Destructs the Quadruple object.
		*/
		virtual ~Quadruple() throw();
		//@}

		/** Clear method.
		*/
		virtual void clear() throw();

		/**	@name	Assignment
		*/
		//@{

		/**	Assign a quadruple from another.
				This method copies the contents of a quadruple to another.
				@param	quadruple the quadruple to be copied
		*/
		void set(const Quadruple& quadruple) throw();

		/**	Assignment operator.
				Assigns the contents of a quadruple to another.
				@param	quadruple the quadruple to be copied
		*/
		const Quadruple& operator = (const Quadruple& quadruple) throw();

		/**	Assign the quadruple's content to another.
				@param quadruple the quadruple to be assigned
		*/
		void get(Quadruple& quadruple) const throw();
		//@}

		/**	@name	Accessors
		*/
		//@{

		/**
		*/
		void set(const T1& t1, const T2& t2, const T3& t3, const T4& t4) throw();

		/**
		*/
		void get(T1& first, T2& second, T3& third, T4& fourth) const throw();
		//@}

		/**	@name	Predicates
		*/
		//@{

		/**	Equality operator 
		*/
		bool operator == (const Quadruple& quadruple) const throw();

		/**	Inequality operator 
		*/
		bool operator != (const Quadruple& quadruple) const throw();

		/**	Lesser than operator.	
		*/
		bool operator < (const Quadruple& quadruple) const throw();

		/**	Lesser or equal than operator.	
		*/
		bool operator <= (const Quadruple& quadruple) const throw();

		/**	Greater or equal than operator.	
		*/
		bool operator >= (const Quadruple& quadruple) const throw();

		/**	Greater than operator.	
		*/
		bool operator > (const Quadruple& quadruple) const throw();

		//@}
		
		/**	@name	Attributes
		*/
		//@{

		/**	The first quadruple member
		*/
		T1 first;

		/**	The second quadruple member
		*/
		T2 second;

		/**	The third quadruple member
		*/
		T3 third;

		/**	The fourth quadruple member
		*/
		T4 fourth;
		//@}
	};

	template <typename T1, typename T2, typename T3, typename T4>
	Quadruple<T1, T2, T3, T4>::Quadruple()  throw()
	{
	}

	template <typename T1, typename T2, typename T3, typename T4>
	Quadruple<T1, T2, T3, T4>::Quadruple
		(const Quadruple<T1, T2, T3, T4>& quadruple, bool /* deep */)
		 throw()
		:	first(quadruple.first),
			second(quadruple.second),
			third(quadruple.third),
			fourth(quadruple.fourth)
	{
	}

	template <typename T1, typename T2, typename T3, typename T4>
	Quadruple<T1, T2, T3, T4>::Quadruple
		(const T1& first, const T2& second,
		 const T3& third, const T4& fourth)
		 throw()
		:	first(first),
			second(second),
			third(third),
			fourth(fourth)
	{
	}

	template <typename T1, typename T2, typename T3, typename T4>
	Quadruple<T1, T2, T3, T4>::~Quadruple()  throw()
	{
	}

	template <typename T1, typename T2, typename T3, typename T4>
	BALL_INLINE 
	void Quadruple<T1, T2, T3, T4>::set(const Quadruple<T1, T2, T3, T4>& quadruple)
		 throw()
	{
		first = quadruple.first;
		second = quadruple.second;
		third = quadruple.third;
		fourth = quadruple.fourth;
	}

	template <typename T1, typename T2, typename T3, typename T4>
	BALL_INLINE 
	void Quadruple<T1, T2, T3, T4>::set(const T1& new_first, const T2& new_second,
																		  const T3& new_third, const T4& new_fourth)
	 throw()
	{
		first		= new_first;
		second	= new_second;
		third		= new_third;
		fourth	= new_fourth;
	}

	template <typename T1, typename T2, typename T3, typename T4>
	BALL_INLINE 
	const Quadruple<T1, T2, T3, T4>& Quadruple<T1, T2, T3, T4>::operator = 
		(const Quadruple<T1, T2, T3, T4>& quadruple) throw()
	{
		first = quadruple.first;
		second = quadruple.second;
		third = quadruple.third;
		fourth = quadruple.fourth;

		return *this;
	}

	template <typename T1, typename T2, typename T3, typename T4>
	BALL_INLINE 
	void Quadruple<T1, T2, T3, T4>::get
		(Quadruple<T1, T2, T3, T4>& quadruple) const throw()
	{
		quadruple.first = first;
		quadruple.second = second;
		quadruple.third = third;
		quadruple.fourth = fourth;
	}

	template <typename T1, typename T2, typename T3, typename T4>
	BALL_INLINE 
	void Quadruple<T1, T2, T3, T4>::get(T1& t1, T2& t2,	T3& t3, T4& t4) 
		const throw()
	{
		t1 = first;
		t2 = second;
		t3 = third;
		t4 = fourth;
	}

	template <typename T1, typename T2, typename T3, typename T4>
	BALL_INLINE 
	bool Quadruple<T1, T2, T3, T4>::operator ==	(const Quadruple& quadruple) 
		const  throw()
	{
		return (first  == quadruple.first
									&& second == quadruple.second
									&& third  == quadruple.third
									&& fourth == quadruple.fourth);
	}

	template <typename T1, typename T2, typename T3, typename T4>
	BALL_INLINE 
	bool Quadruple<T1, T2, T3, T4>::operator != (const Quadruple& quadruple)
		const throw()
	{
		return (first != quadruple.first
									|| second != quadruple.second
									|| third  != quadruple.third
									|| fourth != quadruple.fourth);
	}

	template <typename T1, typename T2, typename T3, typename T4>
	BALL_INLINE 
	void Quadruple<T1, T2, T3, T4>::clear() throw()
	{
		first = T1();
		second = T2();
		third = T3();
		fourth = T4();
	}

	template <typename T1, typename T2, typename T3, typename T4>
	BALL_INLINE 
	bool Quadruple<T1, T2, T3, T4>::operator <
		(const Quadruple<T1, T2, T3, T4>& quadruple) const throw()
	{
		return (first  <  quadruple.first	|| 
						(first  == quadruple.first && second <  quadruple.second)															|| 
						(first  == quadruple.first && second == quadruple.second && third < quadruple.third)	||
						(first  == quadruple.first && second == quadruple.second 
							&& third  == quadruple.third && fourth < quadruple.fourth));
	}

	template <typename T1, typename T2, typename T3, typename T4>
	BALL_INLINE 
	bool Quadruple<T1, T2, T3, T4>::operator <=
		(const Quadruple<T1, T2, T3, T4>& quadruple) const throw()
	{
		return (first <= quadruple.first ||
						(first == quadruple.first	&& second <= quadruple.second) || 
						(first == quadruple.first	&& second == quadruple.second	&& third <= quadruple.third) ||
						(first == quadruple.first	&& second == quadruple.second	&& third == quadruple.third
											&& fourth <= quadruple.fourth));
	}

	template <typename T1, typename T2, typename T3, typename T4>
	BALL_INLINE 
	bool Quadruple<T1, T2, T3, T4>::operator >=
		(const Quadruple<T1, T2, T3, T4>& quadruple) const throw()
	{
		return (first >= quadruple.first || 
						(first == quadruple.first	&& second >= quadruple.second) || 
						(first == quadruple.first	&& second == quadruple.second && third >= quadruple.third) || 
						(first == quadruple.first	&& second == quadruple.second	&& third == quadruple.third
											&& fourth >= quadruple.fourth));
	}

	template <typename T1, typename T2, typename T3, typename T4>
	BALL_INLINE 
	bool Quadruple<T1, T2, T3, T4>::operator >
		(const Quadruple<T1, T2, T3, T4>& quadruple) const throw()
	{
		return (first > quadruple.first	|| 
						(first == quadruple.first	&& second > quadruple.second)	|| 
						(first == quadruple.first	&& second == quadruple.second	&& third > quadruple.third)	|| 
						(first == quadruple.first	&& second == quadruple.second	&& third == quadruple.third
											&& fourth > quadruple.fourth));
	}

} // namespace BALL

#endif // BALL_DATATYPE_QUADRUPLE_H

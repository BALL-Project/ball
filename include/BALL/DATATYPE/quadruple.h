// $Id: quadruple.h,v 1.2 2000/01/10 15:50:57 oliver Exp $

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

		/**	Default constructor.
				Creates a new Quadruple object.
		*/
		Quadruple();

		/**	Copy constructor.
				Creates a new Quadruple object from another.
				@param	quadruple the Quadruple object to be copied
				@param	cone_deep not used (needed for a consistent interface only)
		*/
		Quadruple(const Quadruple& quadruple, bool deep = true);

		/**	Detailled constructor.
				Creates a new Quadruple object from the quadruple's four
				members.
				@param	new_first	the first quadruple member
				@param	new_second	the second quadruple member
				@param	new_third	the third quadruple member
				@param	new_fourth	the fourth quadruple member
		*/
		Quadruple
			(const T1& new_first, const T2& new_second,
			 const T3& new_third, const T4& new_fourth);

		/**	Destructor.
				Destructs the Quadruple object.
		*/
		virtual ~Quadruple();
		//@}

		/**	@name	Assignment
		*/
		//@{

		/**	Assign a quadruple from another.
				This method copies the contents of a quadruple to another.
				@param	quadruple the quadruple to be copied
		*/
		void set(const Quadruple& quadruple);

		/**	Assignment operator.
				Assigns the contents of a quadruple to another.
				@param	quadruple the quadruple to be copied
		*/
		Quadruple& operator = (const Quadruple& quadruple);

		/**	Assign the quadruple's content to another.
				@param quadruple the quadruple to be assigned
		*/
		void get(Quadruple& quadruple) const;
		//@}

		/**	@name	Accessors
		*/
		//@{

		/**
		*/
		void set(const T1& first, const T2& second, const T3& third, const T4& fourth);

		/**
		*/
		void get(T1& first, T2& second, T3& third, T4& fourth) const;
		//@}

		/**	@name	Predicates
		*/
		//@{

		/**	Equality operator 
		*/
		bool operator == (const Quadruple& quadruple) const;

		/**	Inequality operator 
		*/
		bool operator != (const Quadruple& quadruple) const;
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
	Quadruple<T1, T2, T3, T4>::Quadruple()
	{
	}

	template <typename T1, typename T2, typename T3, typename T4>
	Quadruple<T1, T2, T3, T4>::Quadruple
		(const Quadruple<T1, T2, T3, T4>& quadruple, bool /* deep */)
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
		:	first(first),
			second(second),
			third(third),
			fourth(fourth)
	{
	}

	template <typename T1, typename T2, typename T3, typename T4>
	Quadruple<T1, T2, T3, T4>::~Quadruple()
	{
	}

	template <typename T1, typename T2, typename T3, typename T4>
	BALL_INLINE 
	void Quadruple<T1, T2, T3, T4>::set(const Quadruple<T1, T2, T3, T4>& quadruple)
	{
		first = quadruple.first;
		second = quadruple.second;
		third = quadruple.third;
		fourth = quadruple.fourth;
	}

	template <typename T1, typename T2, typename T3, typename T4>
	BALL_INLINE 
	void Quadruple<T1, T2, T3, T4>::set
		(const T1& new_first, const T2& new_second,
		 const T3& new_third, const T4& new_fourth)
	{
		first		= new_first;
		second	= new_second;
		third		= new_third;
		fourth	= new_fourth;
	}

	template <typename T1, typename T2, typename T3, typename T4>
	BALL_INLINE 
	Quadruple<T1, T2, T3, T4>& Quadruple<T1, T2, T3, T4>::operator = 
		(const Quadruple<T1, T2, T3, T4>& quadruple)
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
		(Quadruple<T1, T2, T3, T4>& quadruple) const
	{
		quadruple.first = first;
		quadruple.second = second;
		quadruple.third = third;
		quadruple.fourth = fourth;
	}

	template <typename T1, typename T2, typename T3, typename T4>
	BALL_INLINE 
	void Quadruple<T1, T2, T3, T4>::get
		(T1& first, T2& second,
		 T3& third, T4& fourth) const
	{
		first = first;
		second = second;
		third = third;
		fourth = fourth;
	}

	template <typename T1, typename T2, typename T3, typename T4>
	BALL_INLINE 
	bool Quadruple<T1, T2, T3, T4>::operator ==
		(const Quadruple& quadruple) const
	{
		return (bool)(first  == quadruple.first
									&& second == quadruple.second
									&& third  == quadruple.third
									&& fourth == quadruple.fourth);
	}

	template <typename T1, typename T2, typename T3, typename T4>
	BALL_INLINE 
	bool Quadruple<T1, T2, T3, T4>::operator != (const Quadruple& quadruple) const
	{
		return (bool)(first != quadruple.first
									|| second != quadruple.second
									|| third  != quadruple.third
									|| fourth != quadruple.fourth);
	}

	/**	Ordered Quadruple Class
	*/
	template <typename T1, typename T2, typename T3, typename T4>
	class OrderedQuadruple
		: public Quadruple<T1, T2, T3, T4>
	{
		public:

		/**	@name Constructors and Destructors 
		*/
		//@{
	
		/**	Default constructor
		*/
		OrderedQuadruple();

		/**	Copy constructor
		*/
		OrderedQuadruple(const OrderedQuadruple& quadruple, bool deep = true);

		/**	Detailled constructor
		*/
		OrderedQuadruple
			(const T1& first, const T2& second,
			 const T3& third, const T4& fourth);

		/**	Destructor.
		*/
		virtual ~OrderedQuadruple();
		//@}

		/**	@name	Assignment
		*/
		//@{

		/**	Assignment operator
		*/
		OrderedQuadruple& operator = (const OrderedQuadruple& quadruple);
		//@}

		/**	@name	Comparison
		*/
		//@{

		/**	Lesser than operator.	
		*/
		bool operator < (const OrderedQuadruple& quadruple) const;

		/**	Lesser or equal than operator.	
		*/
		bool operator <= (const OrderedQuadruple& quadruple) const;

		/**	Greater or equal than operator.	
		*/
		bool operator >= (const OrderedQuadruple& quadruple) const;

		/**	Greater than operator.	
		*/
		bool operator > (const OrderedQuadruple& quadruple) const;
		//@}
			
	};

	template <typename T1, typename T2, typename T3, typename T4>
	OrderedQuadruple<T1, T2, T3, T4>::OrderedQuadruple()
		:	Quadruple<T1, T2, T3, T4>()
	{
	}

	template <typename T1, typename T2, typename T3, typename T4>
	OrderedQuadruple<T1, T2, T3, T4>::OrderedQuadruple
		(const OrderedQuadruple<T1, T2, T3, T4>& quadruple, bool deep)
		:	Quadruple<T1, T2, T3, T4>(quadruple, deep)
	{
	}

	template <typename T1, typename T2, typename T3, typename T4>
	OrderedQuadruple<T1, T2, T3, T4>::OrderedQuadruple
		(const T1& first, const T2& second,
		 const T3& third, const T4& fourth)
		:	Quadruple<T1, T2, T3, T4>(first, second, third, fourth)
	{
	}

	template <typename T1, typename T2, typename T3, typename T4>
	OrderedQuadruple<T1, T2, T3, T4>::~OrderedQuadruple()
	{
	}

	template <typename T1, typename T2, typename T3, typename T4>
	BALL_INLINE 
	OrderedQuadruple<T1, T2, T3, T4>& OrderedQuadruple<T1, T2, T3, T4>::operator =
		(const OrderedQuadruple<T1, T2, T3, T4>& quadruple)
	{
		first = quadruple.first;
		second = quadruple.second;
		third = quadruple.third;
		fourth = quadruple.fourth;

		return *this;
	}

	template <typename T1, typename T2, typename T3, typename T4>
	BALL_INLINE 
	bool OrderedQuadruple<T1, T2, T3, T4>::operator <
		(const OrderedQuadruple<T1, T2, T3, T4>& quadruple) const
	{
		return (bool)(first  <  quadruple.first
									|| (first  == quadruple.first && second <  quadruple.second)
									|| (first  == quadruple.first && second == quadruple.second
											&& third < quadruple.third)
									|| (first  == quadruple.first && second == quadruple.second
											&& third  == quadruple.third && fourth < quadruple.fourth));
	}

	template <typename T1, typename T2, typename T3, typename T4>
	BALL_INLINE 
	bool OrderedQuadruple<T1, T2, T3, T4>::operator <=
		(const OrderedQuadruple<T1, T2, T3, T4>& quadruple) const
	{
		return (bool)(first <= quadruple.first
									|| (first == quadruple.first
											&& second <= quadruple.second)
									|| (first == quadruple.first
											&& second == quadruple.second
											&& third <= quadruple.third)
									|| (first == quadruple.first
											&& second == quadruple.second
											&& third == quadruple.third
											&& fourth <= quadruple.fourth));
	}

	template <typename T1, typename T2, typename T3, typename T4>
	BALL_INLINE 
	bool OrderedQuadruple<T1, T2, T3, T4>::operator >=
		(const OrderedQuadruple<T1, T2, T3, T4>& quadruple) const
	{
		return (bool)(first >= quadruple.first
									|| (first == quadruple.first
											&& second >= quadruple.second)
									|| (first == quadruple.first
											&& second == quadruple.second
											&& third >= quadruple.third)
									|| (first == quadruple.first
											&& second == quadruple.second
											&& third == quadruple.third
											&& fourth >= quadruple.fourth));
	}

	template <typename T1, typename T2, typename T3, typename T4>
	BALL_INLINE 
	bool OrderedQuadruple<T1, T2, T3, T4>::operator >
		(const OrderedQuadruple<T1, T2, T3, T4>& quadruple) const
	{
		return (bool)(first > quadruple.first
									|| (first == quadruple.first
											&& second > quadruple.second)
									|| (first == quadruple.first
											&& second == quadruple.second
											&& third > quadruple.third)
									|| (first == quadruple.first
											&& second == quadruple.second
											&& third == quadruple.third
											&& fourth > quadruple.fourth));
	}

} // namespace BALL

#endif // BALL_DATATYPE_QUADRUPLE_H

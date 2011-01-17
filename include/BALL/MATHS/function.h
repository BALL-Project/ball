// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_MATHS_FUNCTION_H
#define BALL_MATHS_FUNCTION_H

#ifndef BALL_COMMON_H
# include <BALL/common.h>
#endif

namespace BALL
{

	/** Constant function class. This function represents a constant that is
			already known at compile time. 
	\ingroup FunctionClasses
	*/
	template <int constant_template>
	class ConstantFunction
	{
		public:

		BALL_CREATE(ConstantFunction)

		/** @name Function realization
		*/
		//@{

		/** Evaluation of this subterm
				@param x the point where to evaluate
				@return the function value
		*/
		BALL_INLINE
		float operator () (float /* x */) const
		{
			return constant_template;
		}
		//@}

	};


	/** Constant function class. 
			This class represents constants that are not known at 
			compile time but must be set at running time.  \par
	*/
	template <typename DataType = float>
	class MutableConstant
	{
		public:

		BALL_CREATE(MutableConstant)

		/** @name Constructors and destructor
		*/
		//@{

		/** Default constructor
		*/
		MutableConstant();

		/** Copy constructor
		*/
		MutableConstant(const MutableConstant<DataType>& constant);

		/** Detailed constructor
		*/
		MutableConstant(DataType constant);

		/** Destructor
		*/
		virtual ~MutableConstant();

		//@}
		/** @name Assignment
		*/
		//@{

		/** Assignment operator
		*/
		MutableConstant<DataType>& operator = (const MutableConstant<DataType>& constant);

		//@}
		/** @name Predicates
		*/
		//@{

		/** Equality operator
		*/
		bool operator == (const MutableConstant<DataType>& constant) const;

		//@}
		/** @name Function realization
		*/
		//@{

		/** Evaluation of this subterm
				@param x the point where to evaluate
				@return the function value
		*/
		BALL_INLINE
		DataType operator () (const DataType& /* x */) const
		{
			return constant_;
		}

		//@}
		/** @name Accessors
		*/
		//@{

		/** Set the constant argument of the constant
		*/
		void setConstant(DataType constant)
		{
			constant_ = constant;
		}

		/** Get the constant argument of the constant
				@return a const reference to the constant argument
		*/
		const DataType& getConstant() const
		{
			return constant_;
		}
	
		//@}

		protected:

		/*_ the constant
		*/
		DataType constant_;

	};


	/** Addition class.
			
	*/
	template <typename First, typename Second, typename DataType = float>
	class Addition
	{
		public:

		BALL_CREATE(Addition)

		/** @name Constructors and destructor
		*/
		//@{

		/** Default constructor
		*/
		Addition();

		/** Copy constructor
		*/
		Addition(const Addition<First, Second, DataType>& addition);

		/** Destructor
		*/
		virtual ~Addition();

		//@}
		/** @name Assignment
		*/
		//@{

		/** Assignment operator
		*/
		Addition<First, Second, DataType>& operator = (const Addition<First, Second, DataType>& addition);

		//@}
		/** @name Predicates
		*/
		//@{

		/** Equality operator
		*/
		bool operator == (const Addition<First, Second, DataType>& addition) const;

		//@}
		/** @name Function realization
		*/
		//@{

		/** Evaluation of this subterm
				@param x the point where to evaluate
				@return the function value
		*/
		BALL_INLINE
		DataType operator () (const DataType& x) const 
		{
			return (first_(x) + second_(x));
		}

		//@}
		/** @name Accessors
		*/
		//@{

		/** set the first argument of the addition
		*/
		void setFirst(const First& first)
		{
			first_ = first;
		}

		/** get the first argument of the addition
				@return a const reference to the first argument
		*/
		First& getFirst()
		{
			return first_;
		}

		/** Get the first argument of the addition (const version).
		*/
		const First& getFirst() const
		{
			return first_;
		}

		/** set the second argument of the addition
		*/
		void setSecond(const Second& second)
		{
			second_ = second;
		}

		/** get the second argument of the addition
				@return a const reference to the second argument
		*/
		Second& getSecond()
		{
			return second_;
		}
	
		/** Get the second argument of the addition (const version).
		*/
		const Second& getSecond() const
		{
			return second_;
		}
	
		//@}

		protected:

		/*_ the first argument of the addition
		*/
		First first_;

		/*_ the second argument of the addition
		*/
		Second second_;

	};


	/** Subtraction class.
	*/
	template <typename First, typename Second, typename DataType = float>
	class Subtraction
	{
		public:

		BALL_CREATE(Subtraction)

		/** @name Constructors and destructor
		*/
		//@{

		/** Default constructor
		*/
		Subtraction();

		/** Copy constructor
		*/
		Subtraction(const Subtraction& subtraction);

		/** Destructor
		*/
		virtual ~Subtraction();

		//@}
		/** @name Assignment
		*/
		//@{

		/** Assignment operator
		*/
		Subtraction<First, Second, DataType>& operator = (const Subtraction<First, Second, DataType>& subtraction);

		//@}
		/** @name Predicates
		*/
		//@{

		/** Equality operator
		*/
		bool operator == (const Subtraction<First, Second, DataType>& subtraction) const;

		//@}
		/** @name Function realization
		*/
		//@{

		/** Evaluation of this subterm
				@param x the point where to evaluate
				@return the function value
		*/
		BALL_INLINE
		DataType operator () (const DataType& x) const
		{
			return (first_(x) - second_(x));
		}

		//@}
		/** @name Accessors
		*/
		//@{

		/** set the first argument of the subtraction
		*/
		void setFirst(const First& first)
		{
			first_ = first;
		}

		/** get the first argument of the subtraction
				@return a const reference to the first argument
		*/
		First& getFirst()
		{
			return first_;
		}

		/** set the second argument of the subtraction
		*/
		void setSecond(const Second& second)
		{
			second_ = second;
		}

		/** get the second argument of the subtraction
				@return a const reference to the second argument
		*/
		Second& getSecond()
		{
			return second_;
		}
	
		//@}

		protected:

		/*_ the first argument of the subtraction
		*/
		First first_;

		/*_ the second argument of the subtraction
		*/
		Second second_;
	};


	/** Multiplication class.
	*/
	template <typename First, typename Second, typename DataType = float>
	class Product
	{
		public:

		BALL_CREATE(Product)

		/** @name Constructors and destructor
		*/
		//@{

		/** Default constructor
		*/
		Product();

		/** Copy constructor
		*/
		Product(const Product& product);

		/** Destructor
		*/
		virtual ~Product();

		//@}
		/** @name Assignment
		*/
		//@{

		/** Assignment operator
		*/
		Product<First, Second, DataType>& operator = (const Product<First, Second, DataType>& product);

		//@}
		/** @name Predicates
		*/
		//@{

		/** Equality operator
		*/
		bool operator == (const Product<First, Second, DataType>& product) const;

		//@}
		/** @name Function realization
		*/
		//@{

		/** Evaluation of this subterm
				@param x the point where to evaluate
				@return the function value
		*/
		BALL_INLINE
		DataType operator () (const DataType& x) const 
		{
			return (first_(x) * second_(x));
		}

		//@}
		/** @name Accessors
		*/
		//@{

		/** set the first argument of the product
		*/
		void setFirst(const First& first)
		{
			first_ = first;
		}

		/** get the first argument of the product
				@return a const reference to the first argument
		*/
		First& getFirst()
		{
			return first_;
		}

		/** Get the first argument of the product (const version).
				@return a const reference to the first argument
		*/
		const First& getFirst() const
		{
			return first_;
		}

		/** set the second argument of the product
				@param second the second argument
		*/
		void setSecond(const Second& second)
		{
			second_ = second;
		}

		/** get the second argument of the product
				@return a const reference to the second argument
		*/
		Second& getSecond()
		{
			return second_;
		}
	
		//@}

		protected:

		/*_ the first argument of the product
		*/
		First first_;

		/*_ the second argument of the product
		*/
		Second second_;

	};


	/** Division class.
	*/
	template <typename First, typename Second, typename DataType = float>
	class Division
	{
		public:

		BALL_CREATE(Division)

		/** @name Constructors and destructor
		*/
		//@{

		/** Default constructor
		*/
		Division();

		/** Copy constructor
		*/
		Division(const Division& division);

		/** Destructor
		*/
		virtual ~Division();

		//@}
		/** @name Assignment
		*/
		//@{

		/** Assignment operator
		*/
		Division<First, Second, DataType>& operator = (const Division<First, Second, DataType>& division);

		//@}
		/** @name Predicates
		*/
		//@{

		/** Equality operator
		*/
		bool operator == (const Division<First, Second, DataType>& division) const;

		//@}
		/** @name Function realization
		*/
		//@{

		/** Evaluation of this subterm
		 *	@param x the point where to evaluate
		 *	@return the function value
		 *  @throw Exception::DivisonByZero if the divisor is zero
		 */
		BALL_INLINE
		DataType operator () (const DataType& x) const 
		{
			DataType val = second_(x);
			if (val != 0.0)
			{
				return (first_(x) / val);
			}
			else
			{
				throw Exception::DivisionByZero(__FILE__, __LINE__);
			}
		}

		//@}
		/** @name Accessors
		*/
		//@{

		/** set the first argument of the division
		*/
		void setFirst(const First& first)
		{
			first_ = first;
		}

		/** get the first argument of the division
				@return a const reference to the first argument
		*/
		First& getFirst()
		{
			return first_;
		}

		/** set the second argument of the division
		*/
		void setSecond(const Second& second)
		{
			second_ = second;
		}

		/** get the second argument of the division
				@return a const reference to the second argument
		*/
		Second& getSecond()
		{
			return second_;
		}
	
		//@}

		protected:

		/*_ the first argument of the division
		*/
		First first_;

		/*_ the second argument of the division
		*/
		Second second_;

	};


	/** Reciprocal class.
	*/
	template <typename Function, typename DataType = float>
	class Reciprocal
	{
		public:

		BALL_CREATE(Reciprocal)

		/** @name Constructors and destructor
		*/
		//@{

		/** Default constructor
		*/
		Reciprocal();

		/** Copy constructor
		*/
		Reciprocal(const Reciprocal& reciprocal);

		/** Destructor
		*/
		virtual ~Reciprocal();

		//@}
		/** @name Assignment
		*/
		//@{

		/** Assignment operator
		*/
		Reciprocal<Function, DataType>& operator = (const Reciprocal<Function, DataType>& reciprocal);

		//@}
		/** @name Predicates
		*/
		//@{

		/** Equality operator
		*/
		bool operator == (const Reciprocal<Function, DataType>& reciprocal) const;

		//@}
		/** @name Function realization
		*/
		//@{

		/** Evaluation of this subterm
		 *	@param x the point where to evaluate
		 *	@return the function value
		 *  @throw Exception::DivisionByZero if x is zero
		 */
		BALL_INLINE
		DataType operator () (const DataType& x) const 
		{
			DataType val = function_(x);
			if (val != 0)
			{
				return (1 / val);
			}
			else
			{
				throw Exception::DivisionByZero(__FILE__, __LINE__);
			}
		}

		//@}
		/** @name Accessors
		*/
		//@{

		/** set the function argument of the reciprocal
		*/
		void setFunction(const Function& function)
		{
			function_ = function;
		}

		/** get the function argument of the reciprocal
				@return a const reference to the function argument
		*/
		const Function& getFunction() const
		{
			return function_;
		}
	
		//@}

		protected:

		/*_ the argument of the reciprocal
		*/
		Function function_;

	};


	/** SquareFunction class.
	*/
	template <typename Function, typename DataType = float>
	class SquareFunction
	{
		public:

		BALL_CREATE(SquareFunction)

		/** @name Constructors and destructor
		*/
		//@{

		/** Default constructor
		*/
		SquareFunction();

		/** Copy constructor
		*/
		SquareFunction(const SquareFunction& square);

		/** Destructor
		*/
		virtual ~SquareFunction();

		//@}
		/** @name Assignment
		*/
		//@{

		/** Assignment operator
		*/
		SquareFunction<Function, DataType>& operator = (const SquareFunction<Function, DataType>& square);

		//@}
		/** @name Predicates
		*/
		//@{

		/** Equality operator
		*/
		bool operator == (const SquareFunction<Function, DataType>& square) const;

		//@}
		/** @name Function realization
		*/
		//@{

		/** Evaluation of this subterm
				@param x the point where to evaluate
				@return the function value
		*/
		BALL_INLINE
		DataType operator () (const DataType& x) const
		{
			DataType val = function_(x);
			return val * val;
		}

		//@}
		/** @name Accessors
		*/
		//@{

		/** set the function argument of the square
		*/
		void setFunction(const Function& function)
		{
			function_ = function;
		}

		/** get the function argument of the square
				@return a const reference to the function argument
		*/
		const Function& getFunction() const
		{
			return function_;
		}
	
		//@}

		protected:

		/*_ the argument of the square
		*/
		Function function_;

	};


	/** CubicFunction class.
	*/
	template <typename Function, typename DataType = float>
	class CubicFunction
	{
		public:

		BALL_CREATE(CubicFunction)

		/** @name Constructors and destructor
		*/
		//@{

		/** Default constructor
		*/
		CubicFunction();

		/** Copy constructor
		*/
		CubicFunction(const CubicFunction& cubic);

		/** Destructor
		*/
		virtual ~CubicFunction();

		//@}
		/** @name Assignment
		*/
		//@{

		/** Assignment operator
		*/
		CubicFunction<Function, DataType>& operator = (const CubicFunction<Function, DataType>& cubic);

		//@}
		/** @name Predicates
		*/
		//@{

		/** Equality operator
		*/
		bool operator == (const CubicFunction<Function, DataType>& cubic) const;

		//@}
		/** @name Function realization
		*/
		//@{

		/** Evaluation of this subterm
				@param x the point where to evaluate
				@return the function value
		*/
		BALL_INLINE
		DataType operator () (const DataType& x) const 
		{
			DataType val = function_(x);
			return val * val * val;
		}

		//@}
		/** @name Accessors
		*/
		//@{

		/** set the function argument of the cubic
		*/
		void setFunction(const Function& function)
		{
			function_ = function;
		}

		/** get the function argument of the cubic
				@return a const reference to the function argument
		*/
		const Function& getFunction() const
		{
			return function_;
		}
	
		//@}

		protected:

		/*_ the argument of the cubic
		*/
		Function function_;

	};


	/** MutablePower class.	
	*/
	template <typename Function, typename DataType = float>
	class MutablePower
	{
		public:

		BALL_CREATE(MutablePower)

		/** @name Constructors and destructor
		*/
		//@{

		/** Default constructor
		*/
		MutablePower();

		/** Copy constructor
		*/
		MutablePower(const MutablePower& power);

		/** Destructor
		*/
		virtual ~MutablePower();

		//@}
		/** @name Assignment
		*/
		//@{

		/** Assignment operator
		*/
		MutablePower<Function, DataType>& operator = (const MutablePower<Function, DataType>& power);

		//@}
		/** @name Predicates
		*/
		//@{

		/** Equality operator
		*/
		bool operator == (const MutablePower<Function, DataType>& power) const;

		//@}
		/** @name Function realization
		*/
		//@{

		/** Evaluation of this subterm
				@param x the point where to evaluate
				@return the function value
		*/
		BALL_INLINE
		DataType operator () (const DataType& x) const
		{
			return pow(function_(x), exponent_);
		}

		//@}
		/** @name Accessors
		*/
		//@{

		/** set the function argument of the power
		*/
		void setFunction(const Function& function)
		{
			function_ = function;
		}

		/** get the function argument of the power
				@return a const reference to the function argument
		*/
		const Function& getFunction() const
		{
			return function_;
		}
	
		/** set the exponent of the power function
				@param exp the exponent
		*/
		void setExponent(DataType exp);

		/** get the exponent of the power function
				@return the exponent of this
		*/
		DataType getExponent() const;

		//@}

		protected:

		/*_ the argument of the power
		*/
		Function function_;

		/*_ the exponent
		*/
		DataType exponent_;
	};


	template <typename DataType>
	BALL_INLINE
	MutableConstant<DataType>::MutableConstant()
		: constant_(0)
	{
	}

	template <typename DataType>
	BALL_INLINE
	MutableConstant<DataType>::MutableConstant
		(const MutableConstant<DataType>& constant)
		: constant_(constant.constant_)
	{
	}

	template <typename DataType>
	BALL_INLINE
	MutableConstant<DataType>::MutableConstant(DataType constant)
		: constant_(constant)
	{
	}

	template <typename DataType>
	BALL_INLINE
	MutableConstant<DataType>::~MutableConstant()
	{
	}

	template <typename DataType>
	BALL_INLINE
	MutableConstant<DataType>& MutableConstant<DataType>::operator = 
		(const MutableConstant<DataType>& constant)
	{
		constant_ = constant.constant_;
		return *this;
	}

	template <typename DataType>
	BALL_INLINE
	bool MutableConstant<DataType>::operator == 
		(const MutableConstant<DataType>& constant) const
	{
		return (constant_ == constant.constant_);
	}


	template <typename First, typename Second, typename DataType>
	BALL_INLINE
	Addition<First, Second, DataType>::Addition()
		:	first_(),
			second_()
	{
	}

	template <typename First, typename Second, typename DataType>
	BALL_INLINE
	Addition<First, Second, DataType>::Addition(const Addition& addition)
		:	first_(addition.first_),
			second_(addition.second_)
	{
	}

	template <typename First, typename Second, typename DataType>
	BALL_INLINE
	Addition<First, Second, DataType>::~Addition()
	{
	}

	template <typename First, typename Second, typename DataType>
	BALL_INLINE
	Addition<First, Second, DataType>& Addition<First, Second, DataType>::operator = (const Addition<First, Second, DataType>& addition)
	{
		first_ = addition.first_;
		second_ = addition.second_;
		return *this;
	}

	template <typename First, typename Second, typename DataType>
	BALL_INLINE
	bool Addition<First, Second, DataType>::operator == (const Addition<First, Second, DataType>& addition) const
	{
		return ((first_ == addition.first_) && (second_ == addition.second_));
	}


	template <typename First, typename Second, typename DataType>
	BALL_INLINE
	Subtraction<First, Second, DataType>::Subtraction()
		:	first_(),
			second_()
	{
	}

	template <typename First, typename Second, typename DataType>
	BALL_INLINE
	Subtraction<First, Second, DataType>::Subtraction(const Subtraction& subtraction)
		:	first_(subtraction.first_),
			second_(subtraction.second_)
	{
	}

	template <typename First, typename Second, typename DataType>
	BALL_INLINE
	Subtraction<First, Second, DataType>::~Subtraction()
	{
	}

	template <typename First, typename Second, typename DataType>
	BALL_INLINE
	Subtraction<First, Second, DataType>& Subtraction<First, Second, DataType>::operator = (const Subtraction<First, Second, DataType>& subtraction)
	{
		first_ = subtraction.first_;
		second_ = subtraction.second_;
		return *this;
	}

	template <typename First, typename Second, typename DataType>
	BALL_INLINE
	bool Subtraction<First, Second, DataType>::operator == (const Subtraction<First, Second, DataType>& subtraction) const
	{
		return ((first_ == subtraction.first_) && (second_ == subtraction.second_));
	}


	template <typename First, typename Second, typename DataType>
	BALL_INLINE
	Product<First, Second, DataType>::Product()
		:	first_(),
			second_()
	{
	}

	template <typename First, typename Second, typename DataType>
	BALL_INLINE
	Product<First, Second, DataType>::Product(const Product& product)
		:	first_(product.first_),
			second_(product.second_)
	{
	}

	template <typename First, typename Second, typename DataType>
	BALL_INLINE
	Product<First, Second, DataType>::~Product()
	{
	}

	template <typename First, typename Second, typename DataType>
	BALL_INLINE
	Product<First, Second, DataType>& Product<First, Second, DataType>::operator = (const Product<First, Second, DataType>& product)
	{
		first_ = product.first_;
		second_ = product.second_;
		return *this;
	}

	template <typename First, typename Second, typename DataType>
	BALL_INLINE
	bool Product<First, Second, DataType>::operator == (const Product<First, Second, DataType>& product) const
	{
		return ((first_ == product.first_) && (second_ == product.second_));
	}


	template <typename First, typename Second, typename DataType>
	BALL_INLINE
	Division<First, Second, DataType>::Division()
		:	first_(),
			second_()
	{
	}

	template <typename First, typename Second, typename DataType>
	BALL_INLINE
	Division<First, Second, DataType>::Division(const Division& division)
		:	first_(division.first_),
			second_(division.second_)
	{
	}

	template <typename First, typename Second, typename DataType>
	BALL_INLINE
	Division<First, Second, DataType>::~Division()
	{
	}

	template <typename First, typename Second, typename DataType>
	BALL_INLINE
	Division<First, Second, DataType>& Division<First, Second, DataType>::operator = (const Division<First, Second, DataType>& division)
	{
		first_ = division.first_;
		second_ = division.second_;
		return *this;
	}

	template <typename First, typename Second, typename DataType>
	BALL_INLINE
	bool Division<First, Second, DataType>::operator == (const Division<First, Second, DataType>& division) const
	{
		return ((first_ == division.first_) && (second_ == division.second_));
	}


	template <typename Function, typename DataType>
	BALL_INLINE
	SquareFunction<Function, DataType>::SquareFunction()
		: function_()
	{
	}

	template <typename Function, typename DataType>
	BALL_INLINE
	SquareFunction<Function, DataType>::SquareFunction(const SquareFunction& square)
		: function_(square.function_)
	{
	}

	template <typename Function, typename DataType>
	BALL_INLINE
	SquareFunction<Function, DataType>::~SquareFunction()
	{
	}

	template <typename Function, typename DataType>
	BALL_INLINE
	SquareFunction<Function, DataType>& SquareFunction<Function, DataType>::operator = (const SquareFunction<Function, DataType>& square)
	{
		function_ = square.function_;
		return *this;
	}

	template <typename Function, typename DataType>
	BALL_INLINE
	bool SquareFunction<Function, DataType>::operator == (const SquareFunction& square) const
	{
		return (function_ == square.function_);
	}


	template <typename Function, typename DataType>
	BALL_INLINE
	CubicFunction<Function, DataType>::CubicFunction()
		: function_(0)
	{
	}

	template <typename Function, typename DataType>
	BALL_INLINE
	CubicFunction<Function, DataType>::CubicFunction(const CubicFunction& cubic)
		: function_(cubic.function_)
	{
	}

	template <typename Function, typename DataType>
	BALL_INLINE
	CubicFunction<Function, DataType>::~CubicFunction()
	{
	}

	template <typename Function, typename DataType>
	BALL_INLINE
	CubicFunction<Function, DataType>& CubicFunction<Function, DataType>::operator = (const CubicFunction<Function, DataType>& cubic)
	{
		function_ = cubic.function_;
		return *this;
	}

	template <typename Function, typename DataType>
	BALL_INLINE
	bool CubicFunction<Function, DataType>::operator == (const CubicFunction& cubic) const
	{
		return (function_ == cubic.function_);
	}


	template <typename Function, typename DataType>
	BALL_INLINE
	Reciprocal<Function, DataType>::Reciprocal()
		: function_()
	{
	}

	template <typename Function, typename DataType>
	BALL_INLINE
	Reciprocal<Function, DataType>::Reciprocal(const Reciprocal& reciprocal)
		: function_(reciprocal.function_)
	{
	}

	template <typename Function, typename DataType>
	BALL_INLINE
	Reciprocal<Function, DataType>::~Reciprocal()
	{
	}

	template <typename Function, typename DataType>
	BALL_INLINE
	Reciprocal<Function, DataType>& Reciprocal<Function, DataType>::operator = (const Reciprocal<Function, DataType>& reciprocal)
	{
		function_ = reciprocal.function_;
		return *this;
	}

	template <typename Function, typename DataType>
	BALL_INLINE
	bool Reciprocal<Function, DataType>::operator == (const Reciprocal& reciprocal) const 
	{
		return (function_ == reciprocal.function_);
	}


	template <typename Function, typename DataType>
	BALL_INLINE
	MutablePower<Function, DataType>::MutablePower()
		: function_(0),
			exponent_(0)
	{
	}

	template <typename Function, typename DataType>
	BALL_INLINE
	MutablePower<Function, DataType>::MutablePower(const MutablePower& power)
		: function_(power.function_),
			exponent_(power.exponent_)
	{
	}

	template <typename Function, typename DataType>
	BALL_INLINE
	MutablePower<Function, DataType>::~MutablePower()
	{
	}

	template <typename Function, typename DataType>
	BALL_INLINE
	MutablePower<Function, DataType>& MutablePower<Function, DataType>::operator = (const MutablePower& power)
	{
		function_ = power.function_;
		exponent_ = power.exponent_;
		return *this;
	}

	template <typename Function, typename DataType>
	BALL_INLINE
	bool MutablePower<Function, DataType>::operator == (const MutablePower& power) const 
	{
		return ((exponent_ == power.exponent_)
			&& (function_ == power.function_));
	}

	template <typename Function, typename DataType>
	BALL_INLINE
	DataType MutablePower<Function, DataType>::getExponent() const
	{
		return exponent_;
	}

	template <typename Function, typename DataType>
	BALL_INLINE
	void MutablePower<Function, DataType>::setExponent(DataType exp)
	{
		exponent_ = exp;
	}
} // namespace BALL

#endif // BALL_MATHS_FUNCTION_H

// $Id: function.h,v 1.7 2001/03/06 14:02:18 balltest Exp $

#ifndef BALL_MATHS_FUNCTION_H
#define BALL_MATHS_FUNCTION_H

#include <BALL/common.h>
#include <BALL/COMMON/exception.h>

namespace BALL
{

	/** Constant function class. This function represents a constant that is
			already known at compile time. \\
			{\bf Definition:} \URL{BALL/MATHS/function.h}
	*/
	// BAUSTELLE float templates sind nicht ANSI.
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
		float operator () (float /* x */)
			throw()
		{
			return constant_template;
		}

		//@}

	};


	/** Constant function class. This class represents constants that are not
			known at compile time but must be set at running time. \\
			{\bf Definition:} \URL{BALL/MATHS/function.h}
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
		MutableConstant()
			throw();

		/** Copy constructor
		*/
		MutableConstant(const MutableConstant& constant)
			throw();

		/** Detailed constructor
		*/
		MutableConstant(DataType constant)
			throw();

		/** Destructor
		*/
		virtual ~MutableConstant()
			throw();

		//@}
		/** @name Assignment
		*/
		//@{

		/** Assignment operator
		*/
		const MutableConstant<DataType>& operator = (const MutableConstant<DataType>& constant)
			throw();

		//@}
		/** @name Predicates
		*/
		//@{

		/** Equality operator
		*/
		bool operator == (const MutableConstant<DataType>& constant) const
			throw();

		//@}
		/** @name Function realization
		*/
		//@{

		/** Evaluation of this subterm
				@param x the point where to evaluate
				@return the function value
		*/
		BALL_INLINE
		DataType operator () (DataType /* x */)
			throw()
		{
			return constant_;
		}

		//@}
		/** @name Accessors
		*/
		//@{

		/** set the constant argument of the constant
		*/
		void setConstant(DataType constant)
			throw()
		{
			constant_ = constant;
		}

		/** get the constant argument of the constant
				@return a const reference to the constant argument
		*/
		const DataType& getConstant()
			throw()
		{
			return constant_;
		}
	
		//@}

		protected:

		/*_ the constant
		*/
		DataType constant_;

	};


	/** Addition class.	\\
			{\bf Definition:} \URL{BALL/MATHS/function.h}
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
		Addition()
			throw();

		/** Copy constructor
		*/
		Addition(const Addition<First, Second, DataType>& addition)
			throw();

		/** Destructor
		*/
		virtual ~Addition()
			throw();

		//@}
		/** @name Assignment
		*/
		//@{

		/** Assignment operator
		*/
		const Addition<First, Second, DataType>& operator = (const Addition<First, Second, DataType>& addition)
			throw();

		//@}
		/** @name Predicates
		*/
		//@{

		/** Equality operator
		*/
		bool operator == (const Addition<First, Second, DataType>& addition) const
			throw();

		//@}
		/** @name Function realization
		*/
		//@{

		/** Evaluation of this subterm
				@param x the point where to evaluate
				@return the function value
		*/
		BALL_INLINE
		DataType operator () (DataType x)
			throw()
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
			throw()
		{
			first_ = first;
		}

		/** get the first argument of the addition
				@return a const reference to the first argument
		*/
		First& getFirst()
			throw()
		{
			return first_;
		}

		/** set the second argument of the addition
		*/
		void setSecond(const Second& second)
			throw()
		{
			second_ = second;
		}

		/** get the second argument of the addition
				@return a const reference to the second argument
		*/
		Second& getSecond()
			throw()
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


	/** Subtraction class. \\
			{\bf Definition:} \URL{BALL/MATHS/function.h}
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
		Subtraction()
			throw();

		/** Copy constructor
		*/
		Subtraction(const Subtraction& subtraction)
			throw();

		/** Destructor
		*/
		virtual ~Subtraction()
			throw();

		//@}
		/** @name Assignment
		*/
		//@{

		/** Assignment operator
		*/
		const Subtraction<First, Second, DataType>& operator = (const Subtraction<First, Second, DataType>& subtraction)
			throw();

		//@}
		/** @name Predicates
		*/
		//@{

		/** Equality operator
		*/
		bool operator == (const Subtraction<First, Second, DataType>& subtraction) const
			throw();

		//@}
		/** @name Function realization
		*/
		//@{

		/** Evaluation of this subterm
				@param x the point where to evaluate
				@return the function value
		*/
		BALL_INLINE
		DataType operator () (DataType x)
			throw()
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
			throw()
		{
			first_ = first;
		}

		/** get the first argument of the subtraction
				@return a const reference to the first argument
		*/
		First& getFirst()
			throw()
		{
			return first_;
		}

		/** set the second argument of the subtraction
		*/
		void setSecond(const Second& second)
			throw()
		{
			second_ = second;
		}

		/** get the second argument of the subtraction
				@return a const reference to the second argument
		*/
		Second& getSecond()
			throw()
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


	/** Multiplication class.	\\
			{\bf Definition:} \URL{BALL/MATHS/function.h}
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
		Product()
			throw();

		/** Copy constructor
		*/
		Product(const Product& product)
			throw();

		/** Destructor
		*/
		virtual ~Product()
			throw();

		//@}
		/** @name Assignment
		*/
		//@{

		/** Assignment operator
		*/
		const Product<First, Second, DataType>& operator = (const Product<First, Second, DataType>& product)
			throw();

		//@}
		/** @name Predicates
		*/
		//@{

		/** Equality operator
		*/
		bool operator == (const Product<First, Second, DataType>& product) const
			throw();

		//@}
		/** @name Function realization
		*/
		//@{

		/** Evaluation of this subterm
				@param x the point where to evaluate
				@return the function value
		*/
		BALL_INLINE
		DataType operator () (DataType x)
			throw()
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
			throw()
		{
			first_ = first;
		}

		/** get the first argument of the product
				@return a const reference to the first argument
		*/
		First& getFirst()
			throw()
		{
			return first_;
		}

		/** set the second argument of the product
				@param second the second argument
		*/
		void setSecond(const Second& second)
			throw()
		{
			second_ = second;
		}

		/** get the second argument of the product
				@return a const reference to the second argument
		*/
		Second& getSecond()
			throw()
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


	/** Division class.	\\
			{\bf Definition:} \URL{BALL/MATHS/function.h}
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
		Division()
			throw();

		/** Copy constructor
		*/
		Division(const Division& division)
			throw();

		/** Destructor
		*/
		virtual ~Division()
			throw();

		//@}
		/** @name Assignment
		*/
		//@{

		/** Assignment operator
		*/
		const Division<First, Second, DataType>& operator = (const Division<First, Second, DataType>& division)
			throw();

		//@}
		/** @name Predicates
		*/
		//@{

		/** Equality operator
		*/
		bool operator == (const Division<First, Second, DataType>& division) const
			throw();

		//@}
		/** @name Function realization
		*/
		//@{

		/** Evaluation of this subterm
				@param x the point where to evaluate
				@return the function value
		*/
		BALL_INLINE
		DataType operator () (DataType x)
			throw(Exception::DivisionByZero)
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
			throw()
		{
			first_ = first;
		}

		/** get the first argument of the division
				@return a const reference to the first argument
		*/
		First& getFirst()
			throw()
		{
			return first_;
		}

		/** set the second argument of the division
		*/
		void setSecond(const Second& second)
			throw()
		{
			second_ = second;
		}

		/** get the second argument of the division
				@return a const reference to the second argument
		*/
		Second& getSecond()
			throw()
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


	/** Reciprocal class.	\\
			{\bf Definition:} \URL{BALL/MATHS/function.h}
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
		Reciprocal()
			throw();

		/** Copy constructor
		*/
		Reciprocal(const Reciprocal& reciprocal)
			throw();

		/** Destructor
		*/
		virtual ~Reciprocal()
			throw();

		//@}
		/** @name Assignment
		*/
		//@{

		/** Assignment operator
		*/
		const Reciprocal<Function, DataType>& operator = (const Reciprocal<Function, DataType>& reciprocal)
			throw();

		//@}
		/** @name Predicates
		*/
		//@{

		/** Equality operator
		*/
		bool operator == (const Reciprocal<Function, DataType>& reciprocal) const
			throw();

		//@}
		/** @name Function realization
		*/
		//@{

		/** Evaluation of this subterm
				@param x the point where to evaluate
				@return the function value
		*/
		BALL_INLINE
		DataType operator () (DataType x)
			throw(Exception::DivisionByZero)
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
			throw()
		{
			function_ = function;
		}

		/** get the function argument of the reciprocal
				@return a const reference to the function argument
		*/
		const Function& getFunction() const
			throw()
		{
			return function_;
		}
	
		//@}

		protected:

		/*_ the argument of the reciprocal
		*/
		Function function_;

	};


	/** SquareFunction class.	\\
			{\bf Definition:} \URL{BALL/MATHS/function.h}
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
		SquareFunction()
			throw();

		/** Copy constructor
		*/
		SquareFunction(const SquareFunction& square)
			throw();

		/** Destructor
		*/
		virtual ~SquareFunction()
			throw();

		//@}
		/** @name Assignment
		*/
		//@{

		/** Assignment operator
		*/
		const SquareFunction<Function, DataType>& operator = (const SquareFunction<Function, DataType>& square)
			throw();

		//@}
		/** @name Predicates
		*/
		//@{

		/** Equality operator
		*/
		bool operator == (const SquareFunction<Function, DataType>& square) const
			throw();

		//@}
		/** @name Function realization
		*/
		//@{

		/** Evaluation of this subterm
				@param x the point where to evaluate
				@return the function value
		*/
		BALL_INLINE
		DataType operator () (DataType x)
			throw()
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
			throw()
		{
			function_ = function;
		}

		/** get the function argument of the square
				@return a const reference to the function argument
		*/
		const Function& getFunction() const
			throw()
		{
			return function_;
		}
	
		//@}


		protected:

		/*_ the argument of the square
		*/
		Function function_;

	};


	/** CubicFunction class. \\
			{\bf Definition:} \URL{BALL/MATHS/function.h}
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
		CubicFunction()
			throw();

		/** Copy constructor
		*/
		CubicFunction(const CubicFunction& cubic)
			throw();

		/** Destructor
		*/
		virtual ~CubicFunction()
			throw();

		//@}
		/** @name Assignment
		*/
		//@{

		/** Assignment operator
		*/
		const CubicFunction<Function, DataType>& operator = (const CubicFunction<Function, DataType>& cubic)
			throw();

		//@}
		/** @name Predicates
		*/
		//@{

		/** Equality operator
		*/
		bool operator == (const CubicFunction<Function, DataType>& cubic) const
			throw();

		//@}
		/** @name Function realization
		*/
		//@{

		/** Evaluation of this subterm
				@param x the point where to evaluate
				@return the function value
		*/
		BALL_INLINE
		DataType operator () (DataType x)
			throw()
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
			throw()
		{
			function_ = function;
		}

		/** get the function argument of the cubic
				@return a const reference to the function argument
		*/
		const Function& getFunction() const
			throw()
		{
			return function_;
		}
	
		//@}

		protected:

		/*_ the argument of the cubic
		*/
		Function function_;

	};


	/** MutablePower class.	\\
			{\bf Definition:} \URL{BALL/MATHS/function.h}
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
		MutablePower()
			throw();

		/** Copy constructor
		*/
		MutablePower(const MutablePower& power)
			throw();

		/** Destructor
		*/
		virtual ~MutablePower()
			throw();

		//@}
		/** @name Assignment
		*/
		//@{

		/** Assignment operator
		*/
		const MutablePower<Function, DataType>& operator = (const MutablePower<Function, DataType>& power)
			throw();

		//@}
		/** @name Predicates
		*/
		//@{

		/** Equality operator
		*/
		bool operator == (const MutablePower<Function, DataType>& power) const
			throw();

		//@}
		/** @name Function realization
		*/
		//@{

		/** Evaluation of this subterm
				@param x the point where to evaluate
				@return the function value
		*/
		BALL_INLINE
		DataType operator () (DataType x)
			throw()
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
			throw()
		{
			function_ = function;
		}

		/** get the function argument of the power
				@return a const reference to the function argument
		*/
		const Function& getFunction() const
			throw()
		{
			return function_;
		}
	
		/** set the exponent of the power function
				@param exp the exponent
		*/
		void setExponent(DataType exp)
			throw();

		/** get the exponent of the power function
				@return the exponent of this
		*/
		DataType getExponent() const
			throw();

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
		throw()
		: constant_(0)
	{
	}

	template <typename DataType>
	BALL_INLINE
	MutableConstant<DataType>::MutableConstant(const
			MutableConstant<DataType>& constant)
		throw()
		: constant_(constant.constant_)
	{
	}

	template <typename DataType>
	BALL_INLINE
	MutableConstant<DataType>::MutableConstant(DataType constant)
		throw()
		: constant_(constant)
	{
	}

	template <typename DataType>
	BALL_INLINE
	MutableConstant<DataType>::~MutableConstant()
		throw()
	{
	}

	template <typename DataType>
	BALL_INLINE
	const MutableConstant<DataType>& MutableConstant<DataType>::operator = 
		(const MutableConstant<DataType>& constant)
		throw()
	{
		constant_ = constant.constant_;
		return *this;
	}

	template <typename DataType>
	BALL_INLINE
	bool MutableConstant<DataType>::operator == 
		(const MutableConstant<DataType>& constant) const
		throw()
	{
		return (constant_ == constant.constant_);
	}


	template <typename First, typename Second, typename DataType>
	BALL_INLINE
	Addition<First, Second, DataType>::Addition()
		throw()
		:	first_(),
			second_()
	{
	}

	template <typename First, typename Second, typename DataType>
	BALL_INLINE
	Addition<First, Second, DataType>::Addition(const Addition& addition)
		throw()
		:	first_(addition.first_),
			second_(addition.second_)
	{
	}

	template <typename First, typename Second, typename DataType>
	BALL_INLINE
	Addition<First, Second, DataType>::~Addition()
		throw()
	{
	}

	template <typename First, typename Second, typename DataType>
	BALL_INLINE
	const Addition<First, Second, DataType>& Addition<First, Second, DataType>::operator = (const Addition<First, Second, DataType>& addition)
		throw()
	{
		first_ = addition.first_;
		second_ = addition.second_;
		return *this;
	}

	template <typename First, typename Second, typename DataType>
	BALL_INLINE
	bool Addition<First, Second, DataType>::operator == (const Addition<First, Second, DataType>& addition) const
		throw()
	{
		return ((first_ == addition.first_) && (second_ == addition.second_));
	}


	template <typename First, typename Second, typename DataType>
	BALL_INLINE
	Subtraction<First, Second, DataType>::Subtraction()
		throw()
		:	first_(),
			second_()
	{
	}

	template <typename First, typename Second, typename DataType>
	BALL_INLINE
	Subtraction<First, Second, DataType>::Subtraction(const Subtraction& subtraction)
		throw()
		:	first_(subtraction.first_),
			second_(subtraction.second_)
	{
	}

	template <typename First, typename Second, typename DataType>
	BALL_INLINE
	Subtraction<First, Second, DataType>::~Subtraction()
		throw()
	{
	}

	template <typename First, typename Second, typename DataType>
	BALL_INLINE
	const Subtraction<First, Second, DataType>& Subtraction<First, Second, DataType>::operator = (const Subtraction<First, Second, DataType>& subtraction)
		throw()
	{
		first_ = subtraction.first_;
		second_ = subtraction.second_;
		return *this;
	}

	template <typename First, typename Second, typename DataType>
	BALL_INLINE
	bool Subtraction<First, Second, DataType>::operator == (const Subtraction<First, Second, DataType>& subtraction) const
		throw()
	{
		return ((first_ == subtraction.first_) && (second_ == subtraction.second_));
	}


	template <typename First, typename Second, typename DataType>
	BALL_INLINE
	Product<First, Second, DataType>::Product()
		throw()
		:	first_(),
			second_()
	{
	}

	template <typename First, typename Second, typename DataType>
	BALL_INLINE
	Product<First, Second, DataType>::Product(const Product& product)
		throw()
		:	first_(product.first_),
			second_(product.second_)
	{
	}

	template <typename First, typename Second, typename DataType>
	BALL_INLINE
	Product<First, Second, DataType>::~Product()
		throw()
	{
	}

	template <typename First, typename Second, typename DataType>
	BALL_INLINE
	const Product<First, Second, DataType>& Product<First, Second, DataType>::operator = (const Product<First, Second, DataType>& product)
		throw()
	{
		first_ = product.first_;
		second_ = product.second_;
		return *this;
	}

	template <typename First, typename Second, typename DataType>
	BALL_INLINE
	bool Product<First, Second, DataType>::operator == (const Product<First, Second, DataType>& product) const
		throw()
	{
		return ((first_ == product.first_) && (second_ == product.second_));
	}


	template <typename First, typename Second, typename DataType>
	BALL_INLINE
	Division<First, Second, DataType>::Division()
		throw()
		:	first_(),
			second_()
	{
	}

	template <typename First, typename Second, typename DataType>
	BALL_INLINE
	Division<First, Second, DataType>::Division(const Division& division)
		throw()
		:	first_(division.first_),
			second_(division.second_)
	{
	}

	template <typename First, typename Second, typename DataType>
	BALL_INLINE
	Division<First, Second, DataType>::~Division()
		throw()
	{
	}

	template <typename First, typename Second, typename DataType>
	BALL_INLINE
	const Division<First, Second, DataType>& Division<First, Second, DataType>::operator = (const Division<First, Second, DataType>& division)
		throw()
	{
		first_ = division.first_;
		second_ = division.second_;
		return *this;
	}

	template <typename First, typename Second, typename DataType>
	BALL_INLINE
	bool Division<First, Second, DataType>::operator == (const Division<First, Second, DataType>& division) const
		throw()
	{
		return ((first_ == division.first_) && (second_ == division.second_));
	}


	template <typename Function, typename DataType>
	BALL_INLINE
	SquareFunction<Function, DataType>::SquareFunction()
		throw()
		: function_(0)
	{
	}

	template <typename Function, typename DataType>
	BALL_INLINE
	SquareFunction<Function, DataType>::SquareFunction(const SquareFunction& square)
		throw()
		: function_(square.function_)
	{
	}

	template <typename Function, typename DataType>
	BALL_INLINE
	SquareFunction<Function, DataType>::~SquareFunction()
		throw()
	{
	}

	template <typename Function, typename DataType>
	BALL_INLINE
	const SquareFunction<Function, DataType>& SquareFunction<Function, DataType>::operator = (const SquareFunction<Function, DataType>& square)
		throw()
	{
		function_ = square.function_;
		return *this;
	}

	template <typename Function, typename DataType>
	BALL_INLINE
	bool SquareFunction<Function, DataType>::operator == (const SquareFunction& square) const
		throw()
	{
		return (function_ == square.function_);
	}


	template <typename Function, typename DataType>
	BALL_INLINE
	CubicFunction<Function, DataType>::CubicFunction()
		throw()
		: function_(0)
	{
	}

	template <typename Function, typename DataType>
	BALL_INLINE
	CubicFunction<Function, DataType>::CubicFunction(const CubicFunction& cubic)
		throw()
		: function_(cubic.function_)
	{
	}

	template <typename Function, typename DataType>
	BALL_INLINE
	CubicFunction<Function, DataType>::~CubicFunction()
		throw()
	{
	}

	template <typename Function, typename DataType>
	BALL_INLINE
	const CubicFunction<Function, DataType>& CubicFunction<Function, DataType>::operator = (const CubicFunction<Function, DataType>& cubic)
		throw()
	{
		function_ = cubic.function_;
		return *this;
	}

	template <typename Function, typename DataType>
	BALL_INLINE
	bool CubicFunction<Function, DataType>::operator == (const CubicFunction& cubic) const
		throw()
	{
		return (function_ == cubic.function_);
	}


	template <typename Function, typename DataType>
	BALL_INLINE
	Reciprocal<Function, DataType>::Reciprocal()
		throw()
		: function_(0)
	{
	}

	template <typename Function, typename DataType>
	BALL_INLINE
	Reciprocal<Function, DataType>::Reciprocal(const Reciprocal& reciprocal)
		throw()
		: function_(reciprocal.function_)
	{
	}

	template <typename Function, typename DataType>
	BALL_INLINE
	Reciprocal<Function, DataType>::~Reciprocal()
		throw()
	{
	}

	template <typename Function, typename DataType>
	BALL_INLINE
	const Reciprocal<Function, DataType>& Reciprocal<Function, DataType>::operator = (const Reciprocal<Function, DataType>& reciprocal)
		throw()
	{
		function_ = reciprocal.function_;
		return *this;
	}

	template <typename Function, typename DataType>
	BALL_INLINE
	bool Reciprocal<Function, DataType>::operator == (const Reciprocal& reciprocal) const 
		throw()
	{
		return (function_ == reciprocal.function_);
	}


	template <typename Function, typename DataType>
	BALL_INLINE
	MutablePower<Function, DataType>::MutablePower()
		throw()
		: exponent_(0)
	{
	}

	template <typename Function, typename DataType>
	BALL_INLINE
	MutablePower<Function, DataType>::MutablePower(const MutablePower& power)
		throw()
		: exponent_(power.exponent_)
	{
	}

	template <typename Function, typename DataType>
	BALL_INLINE
	MutablePower<Function, DataType>::~MutablePower()
		throw()
	{
	}

	template <typename Function, typename DataType>
	BALL_INLINE
	const MutablePower<Function, DataType>& MutablePower<Function, DataType>::operator = (const MutablePower& power)
		throw()
	{
		exponent_ = power.exponent_;
		function_ = power.function_;
		return *this;
	}

	template <typename Function, typename DataType>
	BALL_INLINE
	bool MutablePower<Function, DataType>::operator == (const MutablePower& power) const 
		throw()
	{
		return ((exponent_ == power.exponent_)
			&& (function_ == power.function_));
	}

	template <typename Function, typename DataType>
	BALL_INLINE
	DataType MutablePower<Function, DataType>::getExponent() const
		throw()
	{
		return exponent_;
	}

	template <typename Function, typename DataType>
	BALL_INLINE
	void MutablePower<Function, DataType>::setExponent(DataType exp)
		throw()
	{
		exponent_ = exp;
	}

} // namespace BALL

#endif // BALL_MATHS_FUNCTION_H

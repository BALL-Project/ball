// $Id: angle.h,v 1.29 2001/01/21 21:10:13 amoll Exp $

#ifndef BALL_MATHS_ANGLE_H
#define BALL_MATHS_ANGLE_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_MATHS_COMMON_H
#	include <BALL/MATHS/common.h>
#endif

namespace BALL 
{
	template <typename T>
	class TAngle;

	/**	@name Angle
			@memo representation of angles: class \Ref{TAngle} and class \Ref{Angle}
	*/
	//@{
	

	template <typename T>
	BALL_INLINE
	TAngle<T> operator * (const T& val, const TAngle<T>& angle)
		throw();

	template <typename T>
	BALL_INLINE
	TAngle<T> operator + (const T& val, const TAngle<T>& angle)
		throw();

	template <typename T>
	BALL_INLINE
	TAngle<T> operator - (const T& val, const TAngle<T>& angle)
		throw();


	/**	Generic Angle Class.
			Use this class to describe angles. The TAngle class permits the conversion
			from degree to radians and is the return type of all functions used to calculate
			angles.\\
			{\bf Definition:} \URL{BALL/MATHS/angle.h}
			\\
	*/
	template <typename T>
	class TAngle
	{
		public:

		BALL_CREATE(TAngle<T>)

		/**	@name	Enums
		*/
		//@{

		/** form of the angle range:
				{\tt RANGE__UNLIMITED = 0} no limitations
				{\tt RANGE__UNSIGNED  = 1} 0 <= angle <= 360, 0 <= angle <= PI * 2
				{\tt RANGE__SIGNED    = 2} -180 <= angle <= 180, -PI <= angle <= PI
		*/
		enum Range
		{
			// no limitations
			RANGE__UNLIMITED = 0, 
			// 0 <= angle <= 360, 0 <= angle <= (Constants::PI * 2)
			RANGE__UNSIGNED  = 1, 
			// -180 <= angle <= 180, -Constants::PI <= angle <= Constants::PI
			RANGE__SIGNED    = 2 
		};
		//@}

		/**	@name	Constructors and Destructors
		*/
		//@{

		/**	Default constructor.
				Creates a new angle object. Its value is set to 0.
		*/
		TAngle()
			throw();

		/**	Copy constructor.
				Create a copy of a TAngle object. Copies are always
				shallow.
				@param	angle the object to be copied
		*/
		TAngle(const TAngle& angle)
			throw();

		/**	Detailed constructor.
				Create a new angle object and set its value to 
				{\tt new_value}. {\tt radian} determines whether {\tt new_value}
				is in radians or in degrees.
				@param	new_value the value of the angle object
				@param	radian {\bf true} if {\tt new_value} is in radians, {\tt false} otherwise 
		*/
		explicit TAngle(const T& new_value, bool radian = true)
			throw();

		/**	Destructor.
		*/
		virtual ~TAngle()
			throw()
		{
		}

		/** Clear method
				The value is set to 0.
		*/
		virtual void clear()
			throw()
		{
			value = (T)0;
		}
		//@}

		/**	@name	Assignment
		*/
		//@{

		/**	Swap the contents of two angles.
		*/
		void swap(TAngle& angle)
			throw();

		/**	Assign a new value to the angle.
				{\tt radian} determines whether {\tt new_value}
				is in radians or in degrees.
				@param	new_value the value of the angle object
				@param	radian {\bf true} if {\tt new_value} is in radians, {\tt false} otherwise 
		*/
		void set(const T& new_value, bool radian = true)
			throw();

		/**	Assign an Angle object from another.
				@param	angle the angle object to be assigned from
		*/
		void set(const TAngle& angle)
			throw();

		/**	Assignment operator
		*/
		const TAngle& operator = (const TAngle& angle)
			throw();

		/**	Assignment operator for floats.
				Assign a float value to the angle.
				The assigned value has to be in radians!
				@param	new_value the new value
		*/
		const TAngle& operator = (const T& new_value)
			throw();

		/**	Assign the value to another angle.
				@param	angle the angle to assign the value to
		*/
		void get(TAngle& angle) const
			throw();

		/**	Assign the value to a variable of type {\tt T}.
				@param	val the variable to assign the value to
				@param	radian, if set to {\tt true} assigns the value in radians (default).
		*/
		void get(T& val, bool radian = true) const
			throw();

		//@}

		/**	@name	Accessors
		*/
		//@{

		/** Cast operator
				@return value in radians
		*/
		operator T () const
			throw();

		/** Return the value of the angle
				@return value in radians
		*/
		T toRadian() const
			throw();

		/** Calculate radians from degrees
				@param degree the value in degrees
				@return T the value in radians
		*/
		static T toRadian(const T& degree)
			throw();

		/** Return the value of the angle
				@return value in degrees
		*/
		T toDegree() const
			throw();

		/** Calculate degrees from radians
				@param radian the value in radians
				@return T the value in degrees
		*/
		static T toDegree(const T& radian)
			throw();

		/**	Normalize the angle over a given range.
				{\tt RANGE__UNLIMITED = 0} no limitations.
				{\tt RANGE__UNSIGNED  = 1} $0 \le \mathtt{angle} \le 360, 0 \le angle \le 2 \pi$.
				{\tt RANGE__SIGNED    = 2} $-180 \le \mathtt{angle} \le 180, -\pi \le \mathtt{angle} \le \pi$.
				@param range the range of the angle
		*/		
		void normalize(Range range)
			throw();

		/**  Negate the angle
		*/
		void negate()
			throw();

		/**	Positive sign.
		*/
		TAngle operator + () const
			throw();

		/**	Negative sign.
		*/
		TAngle operator - () const
			throw();

		/**	Addition operator.
				@param angle the angle to add
				@return TAngle, {\em *this}
		*/
		TAngle& operator += (const TAngle& angle)
			throw();

		/**	Add a value to this angle.
				@param value the value to add
				@return TAngle, {\em *this}
		*/
		TAngle& operator += (const T& val)
			throw();

		/**	Addition operator.
				@param angle the angle to add
				@return TAngle, the new angle
		*/
		TAngle operator +	(const TAngle& angle)
			throw();

		/**	Substraction operator.
				@param angle the angle to substract
				@return TAngle, {\em *this}
		*/
		TAngle& operator -= (const TAngle& angle)
			throw();

		/**	Substract a value from this angle.
				@param val the value to substract
				@return TAngle, {\em *this}
		*/
		TAngle& operator -= (const T& val)
			throw();

		/**	Subtraction an angle from this angle.
				@param angle the angle to substract
				@return TAngle, the new angle
		*/
		TAngle operator - (const TAngle& angle)
			throw();

		/**	Multiply an angle with this angle.
				@param angle the angle to multiply by
				@return TAngle, {\em *this}
		*/
		TAngle& operator *= (const TAngle& angle)
			throw();

		/**	Multiply a value with this angle.
				@param val the value to multiply by
				@return TAngle, {\em *this}
		*/
		TAngle& operator *=	(const T& val)
			throw();

		/**	Division operator.
				@param angle the angle to divide by
				@return TAngle, {\em *this}
		*/
		TAngle& operator /= (const TAngle& angle)
			throw(Exception::DivisionByZero);

		/**	Divide this angle by a value.
				@param val the angle to divide by
				@return TAngle, {\em *this}
		*/
		TAngle& operator /=	(const T& val)
			throw(Exception::DivisionByZero);

		/**	Divide this angle by a value.
				@param val the angle to divide by
				@return TAngle, the new angle
		*/
		TAngle operator /	(const TAngle& val)
			throw(Exception::DivisionByZero);
		//@}

		/**	@name	Predicates
		*/
		//@{

		/**	Equality operator.
				This test uses Maths::isEqual instead of comparing the
				values directly.
				@param angle the angle to compare with
				@return bool, {\bf true} if the two angles are equal
		*/
		bool operator == (const TAngle& angle) const
			throw();

		/**	Inequality operator
				This test uses Maths::isNotEqual instead of comparing the
				values directly.
				@param angle the angle to compare with
				@return bool, {\bf true} if the two angles are not equal
		*/
		bool operator != (const TAngle& angle) const
			throw();

		/**	Is less operator.
				This test uses Maths::isLess instead of comparing the
				values directly.
				@param angle the angle to compare with
				@return bool, {\bf true} if {\em *this} angle is smaller than {\tt value}
		*/
		bool operator <	(const TAngle& angle) const
			throw();

		/**	Is less operator.
				This test uses Maths::isLess instead of comparing the
				values directly.
				@param val the value to compare with
				@return bool, {\bf true} if {\em *this} angle is smaller than {\tt value}
		*/
		bool operator <	(const T& val) const
			throw();

		/**	Is less or equal operator.
				This test uses Maths::isLessOrEqual instead of comparing the
				values directly.
				@param angle the angle to compare with
				@return bool, {\bf true} if {\em *this} angle is smaller or equal than {\tt value}
		*/
		bool operator <= (const TAngle& angle) const
			throw();

		/**	Is greater or equal operator.
				This test uses Maths::isGreaterOrEqual instead of comparing the
				values directly.
				@param angle the angle to compare with
				@return bool, {\bf true} if {\em *this} angle is greater or equal than {\tt value}
		*/
		bool operator >=	(const TAngle& angle) const
			throw();

		/**	Is greater operator.
				This test uses Maths::isGreater instead of comparing the
				values directly.
				@param angle the angle to compare with
				@return bool, {\bf true} if {\em *this} angle is greater than {\tt value}
		*/
		bool operator > (const TAngle& angle) const
			throw();

		/**	Test whether two angles are equivalent.
				Both angles are normalized and afterwards compared with Maths::isEqual
				instead of comparing the values directly.
				@param angle the angle to compare with
				@return bool, {\bf true} if {\em *this} angle is equal to {\tt value}
		*/
		bool isEquivalent(TAngle angle) const
			throw();
		//@}


		/**	@name	Debugging and Diagnostics
		*/
		//@{

		/**	Test whether instance is valid.
				Always returns true
				@return bool {\bf true}
		*/
		bool isValid () const
			throw();

		/** Internal state dump.
				Dump the current internal state of {\em *this} to 
				the output ostream {\em s} with dumping depth {\em depth}.
				@param   s - output stream where to output the internal state of {\em *this}
				@param   depth - the dumping depth
		*/
		void dump(std::ostream& s = std::cout, Size depth = 0) const
			throw();
		//@}

		/**	@name	Attributes
		*/
		//@{

		/**	The value
		*/
		T value;
		//@}
	};

	template <typename T>
	TAngle<T>::TAngle()
		throw()
		: value(0)
	{
	}

	template <typename T>
	TAngle<T>::TAngle(const TAngle& angle)
		throw()
		:	value(angle.value)
	{
	}

	template <typename T>
	TAngle<T>::TAngle(const T& new_value, bool radian)
		throw()
		:	value((radian == true)
			 ? (T)new_value 
			 : (T)BALL_ANGLE_DEGREE_TO_RADIAN((double)new_value))
	{
	}

	template <typename T>
	void TAngle<T>::swap(TAngle& angle)
		throw()
	{
		T temp = value;
		value = angle.value;
		angle.value = temp;
	}

	template <typename T>
	void TAngle<T>::set(const TAngle& angle)
		throw()
	{
		value = angle.value;
	}

	template <typename T>
	void TAngle<T>::set(const T& new_value, bool radian)
		throw()
	{
		value = (radian == true)
			 ? new_value 
			 : BALL_ANGLE_DEGREE_TO_RADIAN(new_value);
	}

	template <typename T>
	const TAngle<T>& TAngle<T>::operator = (const TAngle& angle)
		throw()
	{
		value = angle.value;
		return *this;
	}

	template <typename T>
	const TAngle<T>& TAngle<T>::operator = (const T& new_value)
		throw()
	{
		value = new_value;
		return *this;
	}

	template <typename T>
	void TAngle<T>::get(TAngle& angle) const
		throw()
	{
		angle.value = value;
	}

	template <typename T>
	void TAngle<T>::get(T& val, bool radian) const
		throw()
	{
		val = (radian == true)
					 ? value 
					 : BALL_ANGLE_RADIAN_TO_DEGREE(value);
	}

	template <typename T>
	TAngle<T>::operator T () const
		throw()
	{
		return value;
	}

	template <typename T>
	T TAngle<T>::toRadian() const
		throw()
	{
		return value;
	}

	template <typename T>
	T TAngle<T>::toRadian(const T& degree)
		throw()
	{
		return BALL_ANGLE_DEGREE_TO_RADIAN(degree);
	}

	template <typename T>
	T TAngle<T>::toDegree() const
		throw()
	{
		return BALL_ANGLE_RADIAN_TO_DEGREE(value);
	}

	template <typename T>
	T TAngle<T>::toDegree(const T& radian)
		throw()
	{
		return BALL_ANGLE_RADIAN_TO_DEGREE(radian);
	}

	template <typename T>
	void TAngle<T>::normalize(Range range)
		throw()
	{
		if (range == RANGE__UNLIMITED)
		{
			return;
		}

		long mod_factor = (long)(value / (2 * Constants::PI));
		value -= mod_factor * (Constants::PI * 2);

		while (Maths::isGreater(value, (Constants::PI * 2)))
		{
			value -= (Constants::PI * 2);
		}
		while (Maths::isLess(value, -(Constants::PI * 2)))
		{
			value += (Constants::PI * 2);
		}
		if (range == RANGE__SIGNED) // invariant: -180 to 180:
		{
			if (Maths::isGreater(value, Constants::PI)) 
			{
				value -= (Constants::PI * 2);
			}
		} 
		else 
		{ // invariant: 0 to 360:
			if (Maths::isLess(value, 0)) 
			{
				value += (Constants::PI * 2);
			}
		}
	}

	template <typename T>
	void TAngle<T>::negate() 
		throw()
	{
		value = -value;
	}

	template <typename T>
	TAngle<T> TAngle<T>::operator + () const 
		throw()
	{
		return *this;
	}

	template <typename T>
	TAngle<T> TAngle<T>::operator - () const 
		throw()
	{
		return TAngle(-value);
	}

	template <typename T>
	TAngle<T>& TAngle<T>::operator += (const TAngle& angle) 
		throw()
	{
		value += angle.value;
		return *this;
	}

	template <typename T>
	TAngle<T>& TAngle<T>::operator += (const T& val) 
		throw()
	{
		value += val;
		return *this;
	}

	template <typename T>
	TAngle<T> TAngle<T>::operator +	(const TAngle& angle) 
		throw()
	{
		return TAngle(value + angle.value);
	}

	template <typename T>
	TAngle<T>& TAngle<T>::operator -= (const TAngle& angle) 
		throw()
	{
		value -= angle.value;
		return *this;
	}

	template <typename T>
	TAngle<T>& TAngle<T>::operator -= (const T& val) 
		throw()
	{
		value -= val;
		return *this;
	}

	template <typename T>
	TAngle<T> TAngle<T>::operator - (const TAngle& angle) 
		throw()
	{
		return TAngle(value - angle.value);
	}

	template <typename T>
	TAngle<T>& TAngle<T>::operator *= (const TAngle& angle) 
		throw()
	{
		value *= angle.value;
		return *this;
	}

	template <typename T>
	TAngle<T>& TAngle<T>::operator *=	(const T& val) 
		throw()
	{
		value *= val;
		return *this;
	}

	template <typename T>
	TAngle<T>& TAngle<T>::operator /= (const TAngle& angle)
		throw(Exception::DivisionByZero)
	{
		if (angle.value == 0)
		{
			throw Exception::DivisionByZero(__FILE__, __LINE__);
		}
		value /= angle.value;
		return *this;
	}


	template <typename T>
	TAngle<T>& TAngle<T>::operator /=	(const T& val) 
		throw(Exception::DivisionByZero)
	{
		if (val == 0)
		{
			throw Exception::DivisionByZero(__FILE__, __LINE__);
		}

		value /= val;
		return *this;
	}


	template <typename T>
	TAngle<T> TAngle<T>::operator /	(const TAngle<T>& val) 
		throw(Exception::DivisionByZero)
	{
		if (val.value == 0)
		{
			throw Exception::DivisionByZero(__FILE__, __LINE__);
		}

		return TAngle(value / val.value);
	}

	template <typename T>
	bool TAngle<T>::operator == (const TAngle& angle) const 
		throw()
	{
		return Maths::isEqual(value, angle.value);
	}

	template <typename T>
	bool TAngle<T>::operator != (const TAngle& angle) const 
		throw()
	{
		return Maths::isNotEqual(value, angle.value);
	}

	template <typename T>
	bool TAngle<T>::operator <	(const TAngle& angle) const	
		throw()
	{
		return Maths::isLess(value, angle.value);
	}

	template <typename T>
	bool TAngle<T>::operator <	(const T& val) const 
		throw()
	{
		return Maths::isLess(value, val);
	}

	template <typename T>
	bool TAngle<T>::operator <= (const TAngle& angle) const
		throw()
	{
		return Maths::isLessOrEqual(value, angle.value);
	}

	template <typename T>
	bool TAngle<T>::operator >=	(const TAngle& angle) const 
		throw()
	{
		return Maths::isGreaterOrEqual(value, angle.value);
	}

	template <typename T>
	bool TAngle<T>::operator > (const TAngle& angle) const 
		throw()
	{
		return Maths::isGreater(value, angle.value);
	}

	template <typename T>
	bool TAngle<T>::isEquivalent(TAngle angle) const
		throw()
	{
		TAngle this_angle(*this);

		this_angle.normalize(RANGE__UNSIGNED);
		angle.normalize(RANGE__UNSIGNED);

		return (this_angle == angle);
	}

	template <typename T>
	bool TAngle<T>::isValid() const 
		throw()
	{
		return true;
	}

	template <typename T>
	void TAngle<T>::dump(std::ostream& s, Size depth) const
		throw()
	{
		BALL_DUMP_STREAM_PREFIX(s);

		BALL_DUMP_HEADER(s, this, this);

		BALL_DUMP_DEPTH(s, depth);
		s << "  value: " << value << std::endl;

		BALL_DUMP_STREAM_SUFFIX(s);
	}

	/**	The Default Angle Type.
			If double precision is not needed, {\tt TAngle<float>} should
			be used. It is predefined as {\tt Angle} for convenience.
	*/
	typedef TAngle<float> Angle;

	/**	Operators
	*/
	//@{

	/**	Multiplication operator.
			Multiplies a number with an angle.
	*/
	template <typename T>
	BALL_INLINE
	TAngle<T> operator * (const T& val, const TAngle<T>& angle)
		throw()
	{
		return TAngle<T>(val * angle.value);
	}

	/**	Plus operator.
			Adds a number with an angle (in rad!)
	*/
	template <typename T>
	BALL_INLINE
	TAngle<T> operator + (const T& val, const TAngle<T>& angle) 
		throw()
	{
		return TAngle<T>(val + angle.value);
	}

	/**	Minus operator.
			Subtracts the value of an angle (in rad!) from a number.
	*/
	template <typename T>
	BALL_INLINE
	TAngle<T> operator - (const T& val, const TAngle<T>& angle) 
		throw()
	{
		return TAngle<T>(val - angle.value);
	}

	/**	Input Operator.
			Reads the value (in radians) of an angle from an instream using T::operator >>
	*/
	template <typename T>
	std::istream& operator >> (std::istream& s, TAngle<T>& angle)
		throw()
	{
		char c;
		s >> c >> angle.value >> c;
		return s;
	}

	/**	Output Operator.
			Writes the value of the angle to an output stream.
			The stream operator {\tt operator <<} has to be defined
			for the template parameter {\tt T}.
	*/
	template <typename T>
	std::ostream& operator << (std::ostream& s, const TAngle<T>& angle)
		throw()
	{
		s << '(' << angle.value << ')';

		return s;
	}

	//@}
	//@}

} // namespace BALL

#endif // BALL_MATHS_ANGLE_H

// $Id: angle.h,v 1.22 2000/04/18 13:57:04 oliver Exp $

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
	TAngle<T> operator * (const T& val, const TAngle<T>& angle);

	template <typename T>
	BALL_INLINE
	TAngle<T> operator + (const T& val, const TAngle<T>& angle);

	template <typename T>
	BALL_INLINE
	TAngle<T> operator - (const T& val, const TAngle<T>& angle);


	/**	Generic Angle Class.
			Use this class to describe angles. The TAngle class permits the conversion
			from degree to radians and is a return type for all functions used to calculate
			angles.\\
			{\bf Definition:} \URL{BALL/MATHS/angle.h}
			\\
	*/
	template <typename T>
	class TAngle
	{
		public:

		BALL_CREATE_NODEEP(TAngle<T>)

		/**	@name	Enums
		*/
		//@{

		/** form of range of the angle:
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
		TAngle();

		/**	Copy constructor.
				Create a copy of a TAngle object. Copies are always
				shallow.
				@param	angle the object to be copied
		*/
		TAngle(const TAngle& angle);

		/**	Detailled constructor.
				Create a new angle object and set its value to 
				{\tt new_value}. {\tt radian} determines whether {\tt new_value}
				is in radians or in degrees.
				@param	new_value the value of the angle object
				@param	radian {\bf true} if {\tt new_value} is in radians, {\tt false} otherwise 
		*/
		explicit TAngle(const T& new_value, bool radian = true);

		/**	Destructor.
		*/
		virtual ~TAngle()
		{
		}
		//@}


		/**	@name	Assignment
		*/
		//@{

		/**	Swap the contents of two angles.
		*/
		void swap(TAngle& angle);

		/**	Assign a new value to the angle.
				{\tt radian} determines whether {\tt new_value}
				is in radians or in degrees.
				@param	new_value the value of the angle object
				@param	radian {\bf true} if {\tt new_value} is in radians, {\tt false} otherwise 
		*/
		void set(const T& new_value, bool radian = true);

		/**	Assign an Angle object from another.
				@param	angle the angle object to be assigned from
		*/
		void set(const TAngle& angle);


		/**	Assignment operator
		*/
		const TAngle& operator = (const TAngle& angle);


		/**	Assignment operator for floats.
				Assign a float value to the angle.
				The assigned value has to be in radians!
				@param	new_value the new value
		*/
		const TAngle& operator = (const T& new_value);

		/**	Assign the value to another angle.
				@param	angle the angle to assign the value to
		*/
		void get(TAngle& angle) const;

		/**	Assign the value to an {\tt T} variable.
				Variable can get the radian or degree value
				@param	val the variable to assign the value to
				@param	bool radian: default true
		*/
		void get(T& val, bool radian = true) const;

		//@}

		/**	@name	Accessors
		*/
		//@{

		/** Cast operator
				@return value in radians
		*/
		operator T () const;

		/** Return the value of the angle
				@return value in radians
		*/
		T toRadian() const;

		/** Calculate radians from degrees
				@param degree the value in degrees
				@return T the value in radians
		*/
		static T toRadian(const T& degree);

		/** Return the value of the angle
				@return value in degrees
		*/
		T toDegree() const;

		/** Calculate degrees from radians
				@param radian the value in radians
				@return T the value in degrees
		*/
		static T toDegree(const T& radian);

		/**	Normalize the angle.
				@param range :
				{\tt RANGE__UNLIMITED = 0} no limitations
				{\tt RANGE__UNSIGNED  = 1} $0 \le \mathtt{angle} \le 360, 0 \le angle \le 2 \pi$
				{\tt RANGE__SIGNED    = 2} $-180 \le \mathtt{angle} \le 180, -\pi \le \mathtt{angle} \le \pi$
		*/		
		void normalize(Range range);

		/**  Negate the angle
		*/
		void negate();

		/**	Positive sign.
		*/
		TAngle operator + () const;

		/**	Negative sign.
		*/
		TAngle operator - () const;

		/**	Add an angle to this angle.
				@param angle the angle to add
				@return TAngle, {\tt *this}
		*/
		TAngle& operator += (const TAngle& angle);

		/**	Add a value to this angle.
				@param value the value to add
				@return TAngle, {\tt *this}
		*/
		TAngle& operator += (const T& val);

		/**	Add this angle to an other and return the result.
				@param angle the angle to add
				@return TAngle, the new angle
		*/
		TAngle operator +	(const TAngle& angle);

		/**	Substract an angle from this angle.
				@param angle the angle to substract
				@return TAngle, {\tt *this}
		*/
		TAngle& operator -= (const TAngle& angle);

		/**	Substract a value from this angle.
				@param val the value to substract
				@return TAngle, {\tt *this}
		*/
		TAngle& operator -= (const T& val);

		/**	Subtraction an angle from this 
				angle and return the result.
				@param angle the angle to substract
				@return TAngle, the new angle
		*/
		TAngle operator - (const TAngle& angle);

		/**	Multiply an angle with this angle.
				@param angle the angle to multiply with
				@return TAngle, {\tt *this}
		*/
		TAngle& operator *= (const TAngle& angle);

		/**	Multiply a value with this 
				angle and return the result.
				@param val the value to multiply with
				@return TAngle, {\tt *this}
		*/
		TAngle& operator *=	(const T& val);


		/**	Calculate the division of this angle by an other.
				@param angle the angle to divide by
				@return TAngle, {\tt *this}
		*/
		TAngle& operator /= (const TAngle& angle);


		/**	Calculate the division of this angle by an value.
				@param val the angle to divide by
				@return TAngle, {\tt *this}
		*/
		TAngle& operator /=	(const T& val);


		/**	Calculate the division of this angle by an value
				and return the result.
				@param val the angle to divide by
				@return TAngle, the new angle
		*/
		TAngle operator /	(const TAngle& val);
		//@}

		/**	@name	Predicates
		*/
		//@{

		/**	Equality operator
				@param angle the angle to compare with
				@return bool, {\bf true} if the two angles are equal
		*/
		bool operator == (const TAngle& angle) const;


		/**	Inequality operator
				@param angle the angle to compare with
				@return bool, {\bf true} if the two angles are not equal
		*/
		bool operator != (const TAngle& angle) const;


		/**	Is less operator.
				@param angle the angle to compare with
				@return bool, {\bf true} if {\tt this} angle is smaller than {\tt value}
		*/
		bool operator <	(const TAngle& angle) const;

		/**	Is less operator.
				@param val the value to compare with
				@return bool, {\bf true} if {\tt this} angle is smaller than {\tt value}
		*/
		bool operator <	(const T& val) const;

		/**	Is less or equal operator.
				@param angle the angle to compare with
				@return bool, {\bf true} if {\tt this} angle is smaller or equal than {\tt value}
		*/
		bool operator <= (const TAngle& angle) const;

		/**	Is greater or equal operator.
				@param angle the angle to compare with
				@return bool, {\bf true} if {\tt this} angle is greater or equal than {\tt value}
		*/
		bool operator >=	(const TAngle& angle) const;

		/**	Is greater operator.
				@param angle the angle to compare with
				@return bool, {\bf true} if {\tt this} angle is greater than {\tt value}
		*/
		bool operator > (const TAngle& angle) const;

		/**	Test if an angle ist equivalent
				@param angle the angle to compare with
				@return bool, {\bf true} if {\tt this} angle is equal to {\tt value}
		*/
		bool isEquivalent(TAngle angle) const;
		//@}


		/**	@name	Debugging and Diagnostics
		*/
		//@{

		/**	Test if instance is valid.
				Always returns true
				@return bool {\bf true}
		*/
		bool isValid () const;

		/**
		*/
		void dump(std::ostream& s = std::cout, Size depth = 0) const;
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
		: value(0)
	{
	}

	template <typename T>
	TAngle<T>::TAngle(const TAngle& angle)
		:	value(angle.value)
	{
	}

	template <typename T>
	TAngle<T>::TAngle(const T& new_value, bool radian)
		:	value((radian == true)
			 ? (T)new_value 
			 : (T)BALL_ANGLE_DEGREE_TO_RADIAN((double)new_value))
	{
	}

	template <typename T>
	void TAngle<T>::swap(TAngle& angle)
	{
		T temp = value;
		value = angle.value;
		angle.value = temp;
	}

	template <typename T>
	void TAngle<T>::set(const TAngle& angle)
	{
		value = angle.value;
	}

	template <typename T>
	void TAngle<T>::set(const T& new_value, bool radian)
	{
		value = (radian == true)
			 ? new_value 
			 : BALL_ANGLE_DEGREE_TO_RADIAN(new_value);
	}

	template <typename T>
	const TAngle<T>& TAngle<T>::operator = (const TAngle& angle)
	{
		value = angle.value;

		return *this;
	}


	template <typename T>
	const TAngle<T>& TAngle<T>::operator = (const T& new_value)
	{
		value = new_value;

		return *this;
	}


	template <typename T>
	void TAngle<T>::get(TAngle& angle) const
	{
		angle.value = value;
	}

	template <typename T>
	void TAngle<T>::get(T& val, bool radian) const
	{
		val = (radian == true)
					 ? value 
					 : BALL_ANGLE_RADIAN_TO_DEGREE(value);
	}

	template <typename T>
	TAngle<T>::operator T () const
	{
		return value;
	}

	template <typename T>
	T TAngle<T>::toRadian() const
	{
		return value;
	}

	template <typename T>
	T TAngle<T>::toRadian(const T& degree)
	{
		return BALL_ANGLE_DEGREE_TO_RADIAN(degree);
	}

	template <typename T>
	T TAngle<T>::toDegree() const
	{
		return BALL_ANGLE_RADIAN_TO_DEGREE(value);
	}

	template <typename T>
	T TAngle<T>::toDegree(const T& radian)
	{
		return BALL_ANGLE_RADIAN_TO_DEGREE(radian);
	}

	template <typename T>
	void TAngle<T>::normalize(Range range)
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
		if (range == RANGE__SIGNED) { // invariant: -180 to 180:
			if (Maths::isGreater(value, Constants::PI)) {
				value -= (Constants::PI * 2);
			}
		} else { // invariant: 0 to 360:
			if (Maths::isLess(value, 0)) {
				value += (Constants::PI * 2);
			}
		}
	}

	template <typename T>
	void TAngle<T>::negate() 
	{
		value = -value;
	}

	template <typename T>
	TAngle<T> TAngle<T>::operator + () const 
	{
		return *this;
	}

	template <typename T>
	TAngle<T> TAngle<T>::operator - () const 
	{
		return TAngle(-value);
	}

	template <typename T>
	TAngle<T>& TAngle<T>::operator += (const TAngle& angle) 
	{
		value += angle.value;

		return *this;
	}

	template <typename T>
	TAngle<T>& TAngle<T>::operator += (const T& val) 
	{
		value += val;
		return *this;
	}

	template <typename T>
	TAngle<T> TAngle<T>::operator +	(const TAngle& angle) 
	{
		return TAngle(value + angle.value);
	}

	template <typename T>
	TAngle<T>& TAngle<T>::operator -= (const TAngle& angle) 
	{
		value -= angle.value;

		return *this;
	}

	template <typename T>
	TAngle<T>& TAngle<T>::operator -= (const T& val) 
	{
		value -= val;

		return *this;
	}

	template <typename T>
	TAngle<T> TAngle<T>::operator - (const TAngle& angle) 
	{
		return TAngle(value - angle.value);
	}

	template <typename T>
	TAngle<T>& TAngle<T>::operator *= (const TAngle& angle) 
	{
		value *= angle.value;

		return *this;
	}


	template <typename T>
	TAngle<T>& TAngle<T>::operator *=	(const T& val) 
	{
		value *= val;

		return *this;
	}

	template <typename T>
	TAngle<T>& TAngle<T>::operator /= (const TAngle& angle)
	{
		value /= angle.value;

		return *this;
	}


	template <typename T>
	TAngle<T>& TAngle<T>::operator /=	(const T& val) 
	{
		value /= val;

		return *this;
	}


	template <typename T>
	TAngle<T> TAngle<T>::operator /	(const TAngle<T>& val) 
	{
		return TAngle(value / val.value);
	}

	template <typename T>
	bool TAngle<T>::operator == (const TAngle& angle) const 
	{
		return Maths::isEqual(value, angle.value);
	}

	template <typename T>
	bool TAngle<T>::operator != (const TAngle& angle) const 
	{
		return Maths::isNotEqual(value, angle.value);
	}

	template <typename T>
	bool TAngle<T>::operator <	(const TAngle& angle) const	
	{
		return Maths::isLess(value, angle.value);
	}

	template <typename T>
	bool TAngle<T>::operator <	(const T& val) const 
	{
		return Maths::isLess(value, val);
	}

	template <typename T>
	bool TAngle<T>::operator <= (const TAngle& angle) const
	{
		return Maths::isLessOrEqual(value, angle.value);
	}

	template <typename T>
	bool TAngle<T>::operator >=	(const TAngle& angle) const 
	{
		return Maths::isGreaterOrEqual(value, angle.value);
	}

	template <typename T>
	bool TAngle<T>::operator > (const TAngle& angle) const 
	{
		return Maths::isGreater(value, angle.value);
	}

	template <typename T>
	bool TAngle<T>::isEquivalent(TAngle angle) const
	{
		TAngle this_angle(*this);

		this_angle.normalize(RANGE__UNSIGNED);
		angle.normalize(RANGE__UNSIGNED);

		return (bool)(this_angle == angle);
	}

	template <typename T>
	bool TAngle<T>::isValid() const 
	{
		return true;
	}

	template <typename T>
	void TAngle<T>::dump(std::ostream& s, Size depth) const
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
			Multiplies a number and an angle.
	*/
	template <typename T>
	BALL_INLINE
	TAngle<T> operator * (const T& val, const TAngle<T>& angle)
	{
		return TAngle<T>(val * angle.value);
	}

	/**	Plus operator.
			Adds an number and an angle (in rad!)
	*/
	template <typename T>
	BALL_INLINE
	TAngle<T> operator + (const T& val, const TAngle<T>& angle) 
	{
		return TAngle<T>(val + angle.value);
	}

	/**	Minus operator.
			Subtracts the value of an angle (in rad!) from a number.
	*/
	template <typename T>
	BALL_INLINE
	TAngle<T> operator - (const T& val, const TAngle<T>& angle) 
	{
		return TAngle<T>(val - angle.value);
	}

	/**	Input- Operator
			reads in one {\bf T} : val
	*/
	template <typename T>
	std::istream& operator >> (std::istream& s, TAngle<T>& angle)
	{
		char c;
		for (int i=0; i<6 ; i++)
		{
			s >> c;
		}
		s >> angle.value >> c;

		return s;
	}

	/**	Output- Operator
			Writes the value of the angle to an output stream.
			The stream operator {\tt operator <<} has to be defined
			for the template parameter {\tt T}.
	*/
	template <typename T>
	std::ostream& operator << (std::ostream& s, const TAngle<T>& angle)
	{
		s << angle.value;

		return s;
	}

	//@}
	//@}

} // namespace BALL

#endif // BALL_MATHS_ANGLE_H

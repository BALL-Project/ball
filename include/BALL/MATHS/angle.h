// $Id: angle.h,v 1.12 2000/02/29 11:03:33 oliver Exp $

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
			Angles.\\
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

		/**
		*/
		enum Range
		{
			/// no limitations
			RANGE__UNLIMITED = 0, 
			/// 0° <= angle <= 360°, 0 <= angle <= (Constants::PI * 2)
			RANGE__UNSIGNED  = 1, 
			/// -180° <= angle <= 180°, -Constants::PI <= angle <= Constants::PI
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
				Create a copy of an TAngle object. Copies are always
				shallow.
				@param	angle the object to be copied
				@param	deep ignored
		*/
		TAngle(const TAngle& angle, bool /* deep */ = true);

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
				@param	deep no effect
		*/
		void set(const TAngle& angle, bool deep = true);


		/**	Assignment operator
		*/
		const TAngle& operator = (const TAngle& angle);


		/**	Assignment operator for floats.
				Assign a float value to the angle.
				the assigned value has to be in radians!
				@param	new_value the new value
		*/
		const TAngle& operator = (const T& new_value);

		/**	Assign the value to another angle.
				@param	angle the angle to assign the value to
				@param	deep ignored
		*/
		void get(TAngle& angle, bool deep = true) const;

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

		/** /// BAUSTELLE
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

		/**	Return the torsion angle of four vectors to eachother.
				@param TVector3& ax 1. vector x component
				@param TVector3& ay 1. vector y component
				@param TVector3& az 1. vector z component
				@param TVector3& bx 2. vector x component
				@param TVector3& by 2. vector y component
				@param TVector3& bz 2. vector z component
				@param TVector3& cx 3. vector x component
				@param TVector3& cy 3. vector y component
				@param TVector3& cz 3. vector z component
				@param TVector3& dx 4. vector x component
				@param TVector3& dy 4. vector y component
				@param TVector3& dz 4. vector z component
				@return static TAngle the torsion angle
		*/
		static TAngle getTorsionAngle
			(const T& ax, const T& ay, const T& az,
			 const T& bx, const T& by, const T& bz,
			 const T& cx, const T& cy, const T& cz, 
			 const T& dx, const T& dy, const T& dz);

		/**  */
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
				@return TAngle&, {\tt *this}
		*/
		TAngle& operator += (const TAngle& angle);

		/**	Add a value to this angle.
				@param value the value to add
				@return TAngle&, {\tt *this}
		*/
		TAngle& operator += (const T& val);

		/**	Add this angle to another and return the result.
				@param angle the angle to add
				@return TAngle&, the new angle
		*/
		TAngle operator +	(const TAngle& angle);

		/**	Substract a angle from this angle.
				@param angle the angle to substract
				@return TAngle&, {\tt *this}
		*/
		TAngle& operator -= (const TAngle& angle);

		/**	Substract a value from this angle.
				@param val the value to substract
				@return TAngle&, {\tt *this}
		*/
		TAngle &operator -= (const T& val);

		/**	Subtraction an angle from this 
				angle and return the result.
				@param angle the angle to substract
				@return TAngle&, the new angle
		*/
		TAngle operator - (const TAngle& angle);

		/**	Multiply an angle with this angle.
				@param angle the angle to multiply with
				@return TAngle&, {\tt *this}
		*/
		TAngle& operator *= (const TAngle& angle);

		/**	Multiply a value with this 
				angle and return the result.
				@param val the value to multiply with
				@return TAngle&, {\tt *this}
		*/
		TAngle& operator *=	(const T& val);


		/**	Calculate the division of this angle by an other.
				@param angle the angle to divide by
				@return TAngle&, {\tt *this}
		*/
		TAngle& operator /= (const TAngle& angle);


		/**	Calculate the division of this angle by an value.
				@param val the angle to divide by
				@return TAngle&, {\tt *this}
		*/
		TAngle& operator /=	(const T& val);


		/**	Calculate the division of this angle by an value
				and return the result.
				@param val the angle to divide by
				@return TAngle&, the new angle
		*/
		TAngle operator /	(const TAngle& val);
		//@}

		/**	@name	Predicates
		*/
		//@{

		/**	Equality operator.
				@param angle the angle to compare with
				@return bool
		*/
		bool operator == (const TAngle& angle) const;


		/**	Inequality operator.
				@param angle the angle to compare with
				@return bool
		*/
		bool operator != (const TAngle& angle) const;


		/**	Is less operator.
				@param angle the angle to compare with
				@return bool
		*/
		bool operator <	(const TAngle& angle) const;

		/**	Is less operator.
				@param val the value to compare with
				@return bool
		*/
		bool operator <	(const T& val) const;

		/**	Is less or equal operator.
				@param angle the angle to compare with
				@return bool
		*/
		bool operator <= (const TAngle& angle) const;

		/**	Is greater or equal operator.
				@param angle the angle to compare with
				@return bool
		*/
		bool operator >=	(const TAngle& angle) const;

		/**	Is greater operator.
				@param angle the angle to compare with
				@return bool
		*/
		bool operator > (const TAngle& angle) const;

		/**	Test if an angle ist equivalent
				@param angle the angle to compare with
				@return bool
		*/
		bool isEquivalent(TAngle angle) const;
		//@}


		/**	@name	Debugging and Diagnostics
		*/
		//@{

		/**	Test if instance is valid.
				always retruns true
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
	TAngle<T>::TAngle(const TAngle& angle, bool /* deep */)
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
	void TAngle<T>::set(const TAngle& angle, bool/* deep */)
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
	void TAngle<T>::get(TAngle& angle, bool /* deep */) const
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
	TAngle<T> TAngle<T>::getTorsionAngle
		(const T &ax, const T &ay, const T &az,
		 const T &bx, const T &by, const T &bz,
		 const T &cx, const T &cy, const T &cz, 
		 const T &dx, const T &dy, const T &dz)
	{
		T abx = ax - bx;
		T aby = ay - by;
		T abz = az - bz;

		T cbx = cx - bx;
		T cby = cy - by;
		T cbz = cz - bz;

		T cdx = cx - dx;
		T cdy = cy - dy;
		T cdz = cz - dz;

		// Calculate the normals to the two planes n1 and n2
		// this is given as the cross products:
		//		 AB x BC
		//		--------- = n1
		//		|AB x BC|
		//
		//		 BC x CD
		// 	  --------- = n2
		// 	  |BC x CD|

		// Normal to plane 1 
		T ndax = aby * cbz - abz * cby; 
		T nday = abz * cbx - abx * cbz;
		T ndaz = abx * cby - aby * cbx;

		// Normal to plane 2 
		T neax = cbz * cdy - cby * cdz; 
		T neay = cbx * cdz - cbz * cdx;
		T neaz = cby * cdx - cbx * cdy;

		// Calculate the length of the two normals 
		T bl = sqrt((double)ndax * ndax + nday * nday + ndaz * ndaz);
		T el = sqrt((double)neax * neax + neay * neay + neaz * neaz);
		T bel = ndax * neax + nday * neay + ndaz * neaz;
		
		bel /= (bl * el);
		if (Maths::isGreater(bel, 1)) {
			bel = 1;
		} else if (Maths::isLess(bel, -1)) {
			bel = -1;
		}

		T acosbel = acos(bel);

		if (Maths::isLess(cbx * (ndaz * neay - nday * neaz) 
							 + cby * (ndax * neaz - ndaz * neax) 
							 + cbz * (nday * neax - ndax * neay), 
							 0))
		{
			acosbel = -acosbel;
		}
		
		acosbel = (Maths::isGreater(acosbel, 0)) ? Constants::PI - acosbel : -(Constants::PI + acosbel);
		
		return TAngle(acosbel);
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


	//@}
	//@}

} // namespace BALL

#endif // BALL_MATHS_ANGLE_H

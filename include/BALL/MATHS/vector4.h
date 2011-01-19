// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_MATHS_VECTOR4_H
#define BALL_MATHS_VECTOR4_H

#ifndef BALL_COMMON_EXCEPTION_H
#	include <BALL/COMMON/exception.h>
#endif

#ifndef BALL_MATHS_ANGLE_H
#	include <BALL/MATHS/angle.h>
#endif

#ifdef BALL_HAS_IEEEFP_H
#	include <ieeefp.h>
#endif 

namespace BALL 
{
	/**	\defgroup	Vector4 Four-dimensional vector.
	 		\ingroup Primitives
	*/
	//@{

	template <typename T>
	class TVector4;
	
	/**	@name	Global binary operators for three-dimensional vectors.
	*/
	//@{

	template <typename T>
	BALL_INLINE
	TVector4<T> operator + (const TVector4<T>& a, const TVector4<T>& b);

	template <typename T>
	BALL_INLINE
	TVector4<T> operator - (const TVector4<T>& a, const TVector4<T>& b);
	
	template <typename T>
	std::istream& operator >> (std::istream& s, TVector4<T>& vector);

	template <typename T>
	std::ostream& operator << (std::ostream& s, const TVector4<T>& vector);
		
	/**	Generic Four-Dimensional Vector.
			Representation of points and vectors in four-dimensional space.
	*/
	template <typename T>
	class TVector4
	{
		public:

		BALL_CREATE(TVector4<T>)

		/**	@name	Constructors and Destructors
		*/
		//@{

		/**	Default constructor.
				This method creates a new TVector4 object. The four components
				are initialized to <tt>(T)0</tt>.
		*/
		TVector4();

		/**	Array constructor.
				This constructor creates a TVector4 object from the first
				four elements pointed by <tt>ptr</tt>.
				@param ptr the array to construct from
				@exception NullPointer if <tt>ptr == 0</tt>
		*/
		TVector4(const T* ptr);

   /** Scalar constructor.
        Create a new vector with all components set
        to the same <tt>value</tt>.
        @param  value the value of all components
    */
    	explicit TVector4(const T& value);

		/**	Detailed constructor.
				Create a new TVector4 object from three or four values of type <tt>T</tt>.
				@param	x assigned to <tt>x</tt>
				@param	y assigned to <tt>y</tt>
				@param	z assigned to <tt>z</tt>
				@param	h assigned to <tt>h</tt>, default: 1 ; 
		*/
		TVector4(const T& x, const T& y, const T& z, const T& h = (T)1);

		/**	Copy constructor.
				Create a new TVector4 object from another.
				@param vector the TVector4 object to be copied
		*/	
		TVector4(const TVector4& vector);

		/**	Destructor.	
				Destructs the TVector4 object. As there are no dynamic
				data structures, nothing happens.
		*/	
		virtual ~TVector4();

		/** Clear method
				The values are set to 0.
		*/
		virtual void clear()
		{
			x = y = z = h = (T)0;
		}

		//@}
		/**	@name	Assignment
		*/
		//@{
	
		/**	Assign from an array.
				Assign the four components <tt>x</tt>, <tt>y</tt>, <tt>z</tt> and <tt>h</tt> from
				the first four elements of the array pointed to by <tt>ptr</tt>.
				@param ptr an array
				@exception Nullpointer if <tt>ptr == 0</tt>
		*/
		void set(const T* ptr);

		/**	Assign the vector components.
				@param rx the new x component
				@param ry the new y component
				@param rz the new z component
				@param rh the new h component, default: 1
		*/
		void set(const T& rx, const T& ry, const T& rz, const T& rh = (T)1);

		/**	Assign from another TVector4.
				@param vector	the TVector4 object to assign from
		*/
		void set(const TVector4& vector);

		/**	Array assignment operator.
				Assigns the first four elements of an array to the vector components x, y, z and h.
				@param	ptr the array
				@exception	NullPointer if <tt>ptr == 0</tt>
		*/
		TVector4& operator = (const T* ptr);

		/**	Assignment operator.
				Assign the vector components from another vector.
				@param vector the vector to assign from
		**/
		TVector4& operator = (const TVector4& vector);

		/** Assignment operator.
		    Assign a constant value to all four vector components.
			  @param value the constant to assign to x, y, z, h
		*/
		TVector4& operator = (T value);

		/**	Assign to an array.
				Sets the first four array elements pointed of array <tt>ptr</tt> 
				to the values of the four vector components.
				@param ptr the array
				@exception	NullPointer if <tt>ptr == 0</tt>
		*/
		void get(T* ptr) const;

		/**	Assign to four variables of type <tt>T</tt>.
				@param	rx the x component
				@param	ry the y component
				@param	rz the z component
				@param	rh the h component
		*/
		void get(T& rx, T& ry, T& rz, T& rh) const;

		/**	Assign to another Vector4.
				Assigns the vector components to another vector.
				@param vector	the vector to be assigned to
		*/
		void get(TVector4& vector) const;

		/**	Swap the contents of two vectors.
				@param	vector the vector to swap contents with
		*/
		void swap(TVector4& vector);

		//@}
		/**	@name	Accessors
		*/
		//@{

		/**	Return the length of the vector.
				The length of the vector is calculated as
				\f$\sqrt{x^2 + y^2 + z^2 + h^2}\f$.
				@return T, the vector length
		*/	
		T getLength() const;

		/**	Return the squared length of the vector.
				This method avoids the square root needed in getLength,
				so this method should be preferred if possible.
				@return T, \f$x^2 + y^2 + z^2 + h^2\f$
		*/
		T getSquareLength() const;

		/**	Normalize the vector.
				The vector is scaled with its length:
				\f$\{x|y|z|h\} *= \sqrt{x^2 + y^2 + z^2 + h^2}\f$.
				@return T, a reference to {\em *this} vector
				@exception DivisionByZero if the length of the vector is 0
		*/
		TVector4& normalize();

		/**	Return a vector with all components 0.
		*/
		static const TVector4& getZero();

		/**	Return a vector with all components 1.
		*/
		static const TVector4& getUnit();

		/** Assign one value to all vector components.
				@param value	the value to be assigned to, default: 1
		*/
		void set(const T& value = (T)1);

		/**	Mutable array-like access to the components.
				@exception Exception::IndexOverflow if <tt>index > 3</tt>
		*/
		T& operator [] (Position position);

		/**	Constant array-like access to the components.
				@exception Exception::IndexOverflow if <tt>index > 3</tt>
		*/
		const T& operator [] (Position position) const;

		//@}
		/**	@name	Arithmetic operators
		*/
		//@{

		/**	Positive sign.
		*/
		TVector4 operator + () const;

		/**	Negative sign.
		*/
		TVector4 operator - () const;

		/**	Add a vector to this vector.
				@param vector the vector to add
				@return TVector4, {\em *this}
		*/
		TVector4& operator += (const TVector4& vector);

		/**	Subtract a vector from this vector.
				@param vector the vector to subtract
				@return TVector4 {\em *this}
		*/
		TVector4& operator -= (const TVector4& vector);

		/**	Scalar product.
				Return <tt>TVector4(x * scalar, y * scalar, z * scalar, h * scalar)</tt>.
				@param scalar the scalar to multiply by
				@return TVector4 the scalar product of this vector and <tt>scalar</tt>
		*/
		TVector4 operator * (const T& scalar);

		/**	Multiply by a scalar.
				Multiply all components of the vector with a <tt>scalar</tt>.
				@param scalar the scalar to multiply by
				@return TVector4& {\em *this}
		*/
		TVector4& operator *= (const T& scalar);

		/**	Fraction of a vector.
				Return <tt>TVector4(x / scalar, y / scalar, z / scalar, h / scalar)</tt>.
				@param scalar the scalar to divide by
				@return TVector4 
				@exception Exception::DivisionByZero if <tt>scalar == (T)0</tt>
		*/
		TVector4 operator / (const T& scalar);

		/**	Divide a vector by a scalar.
				@param scalar the scalar to divide by
				@return TVector4&, {\em *this}
				@exception Exception::DivisionByZero if <tt>scalar == (T)0</tt>
		*/
		TVector4& operator /= (const T& scalar);

		/** Dot product.
				@param vector the vector to multiply by
				@return T the dot product of this vector with <tt>vector</tt>.
		*/
		T operator * (const TVector4& vector) const;

		/**	Return the distance to another vector
				@param vector the reference vector
				@return T, the distance
		*/
		T getDistance(const TVector4& vector) const;

		/**	Return the squared distance to another vector.
				This method avoids the square root needed in getDistance,
				so this method should be preferred if possible.
				@param vector the reference vector
				@return T, the squared distance
		*/
		T getSquareDistance(const TVector4& vector) const;

		//@}
		/**	@name	Predicates
		*/
		//@{
			
		/**	Equality operator.
				The function Maths::isEqual is used to compare the values. 
				 \link isEqual Maths::isEqual \endlink 
				@return bool, <b>true</b> if all four vector components are equal, <b>false</b> otherwise
		*/
		bool operator == (const TVector4& vector) const;

		/**	Inequality operator.
				The function Maths::isEqual is used to compare the values. 
				 \link Maths::isEqual Maths::isEqual \endlink 
				@return bool, <b>true</b> if the two vectors differ in at least one component, <b>false</b> otherwise
		*/
		bool operator != (const TVector4& vector) const;

		/**	Orthogonality predicate.
		*/
		bool isOrthogonalTo(const TVector4& vector) const;

		//@}
		/**	@name	Debugging and Diagnostics
		*/
		//@{

		/**	Test whether instance is valid.
				Always returns true
				@return bool <b>true</b>
		*/
		bool isValid() const;

		/** Internal state dump.
				Dump the current internal state of {\em *this} to 
				the output ostream <b>  s </b> with dumping depth <b>  depth </b>.
				@param   s - output stream where to output the internal state of {\em *this}
				@param   depth - the dumping depth
		*/
		void dump(std::ostream& s = std::cout, Size depth = 0) const;

		//@}
		/**	@name	Attributes
				All vector components are public members for convenience.
		*/
		//@{

		/**	x component of the vector
		*/
		T x;

		/**	y component of the vector
		*/
		T y;

		/**	z component of the vector
		*/
		T z;

		/**	height component of the vector
		*/
		T h;

		//@}
	};
	//@}
	
	template <typename T>
	TVector4<T>::TVector4()
		:	x(0),
			y(0),
			z(0),
			h(0)
	{
	}

	template <typename T>
	TVector4<T>::TVector4(const T* ptr)
	{
		if (ptr == 0)
		{
			throw Exception::NullPointer(__FILE__, __LINE__);
		}
		
		x = *ptr++;
		y = *ptr++;
		z = *ptr++;
		h = *ptr;
	}


  template <typename T>
  TVector4<T>::TVector4(const T& value)
    : x(value),
      y(value),
      z(value),
			h(value)
  {
	}                                                                                                                                                                                

	template <typename T>
	TVector4<T>::TVector4(const T& x, const T& y, const T& z, const T& h)
		: x(x),
			y(y),
			z(z),
			h(h)
	{
	}

	template <typename T>
	TVector4<T>::TVector4(const TVector4<T>& v)
		:	x(v.x),
			y(v.y),
			z(v.z),
			h(v.h)
	{
	}

	template <typename T>
	TVector4<T>::~TVector4()
	{
	}

	template <typename T>
	BALL_INLINE 
	void TVector4<T>::set(const T* ptr)
	{
		if (ptr == 0)
		{
			throw Exception::NullPointer(__FILE__, __LINE__);
		}
		x = *ptr++;
		y = *ptr++;
		z = *ptr++;
		h = *ptr;
	}

	template <typename T>
	BALL_INLINE 
	void TVector4<T>::set(const T& rx, const T& ry, const T& rz, const T& rh)
	{
		x = rx;
		y = ry;
		z = rz;
		h = rh;
	}

	template <typename T>
	BALL_INLINE 
	void TVector4<T>::set(const TVector4<T>& v)
	{
		x = v.x;
		y = v.y;
		z = v.z;
		h = v.h;
	}

	template <typename T>
	BALL_INLINE 
	TVector4<T>& TVector4<T>::operator = (const T* ptr)
	{
		if (ptr == 0)
		{
			throw Exception::NullPointer(__FILE__, __LINE__);
		}
		x = *ptr++;
		y = *ptr++;
		z = *ptr++;
		h = *ptr;

		return *this;
	}

	template <typename T>
	BALL_INLINE 
	TVector4<T>& TVector4<T>::operator = (const TVector4<T>& v)
	{
		x = v.x;
		y = v.y;
		z = v.z;
		h = v.h;

		return *this;
	}

	template <typename T>
	BALL_INLINE 
	TVector4<T>& TVector4<T>::operator = (T value)
	{
		x = value;
		y = value;
		z = value;
		h = value;

		return *this;
	}

	template <typename T>
	BALL_INLINE 
	void TVector4<T>::get(T* ptr) const
	{
		if (ptr == 0)
		{
			throw Exception::NullPointer(__FILE__, __LINE__);
		}
		*ptr++ = x;
		*ptr++ = y;
		*ptr++ = z;
		*ptr   = h;
	}

	template <typename T>
	BALL_INLINE 
	void TVector4<T>::get(T& rx, T& ry, T& rz, T& rh) const
	{
		rx = x;
		ry = y;
		rz = z;
		rh = h;
	}

	template <typename T>
	BALL_INLINE 
	void TVector4<T>::get(TVector4<T>& v) const
	{
		v.x = x;
		v.y = y;
		v.z = z;
		v.h = h;
	}

	template <typename T>
	void TVector4<T>::swap(TVector4<T>& v)
	{
		T temp = x;
		x = v.x;
		v.x = temp;

		temp = y;
		y = v.y;
		v.y = temp;

		temp = z;
		z = v.z;
		v.z = temp;

		temp = h;
		h = v.h;
		v.h = temp;
	}

	template <typename T>
	BALL_INLINE 
	T TVector4<T>::getLength() const
	{
		return (T)sqrt(x * x + y * y + z * z + h * h);
	}

	template <typename T>
	BALL_INLINE 
	T TVector4<T>::getSquareLength() const
	{
		return (T)(x * x + y * y + z * z + h * h);
	}

	template <typename T>
	BALL_INLINE
	TVector4<T>& TVector4<T>::normalize()
	{
		T len = (T)sqrt(x * x + y * y + z * z + h * h);

		if (Maths::isZero(len))
		{
			throw Exception::DivisionByZero(__FILE__, __LINE__);		
		}
		
		x /= len;
		y /= len;
		z /= len;
		h /= len;

		return *this;
	}

	template <typename T>
	BALL_INLINE 
	const TVector4<T>& TVector4<T>::getZero()
	{
		static const TVector4<T> null4(0, 0, 0, 0);
		return null4;
	}

	template <typename T>
	BALL_INLINE 
	const TVector4<T>& TVector4<T>::getUnit()
	{
		static const TVector4<T> unit_vector(1, 1, 1, 1);
		return unit_vector;
	}

	template <typename T>
	BALL_INLINE 
	void TVector4<T>::set(const T& value)
	{
		x = y = z = h = value;
	}

	template <typename T>
	BALL_INLINE 
	T& TVector4<T>::operator [] (Position pos)
	{
		if (pos > 3)
		{
			throw Exception::IndexOverflow(__FILE__, __LINE__);
		}
		switch (pos) 
		{
			case 0: return x;
			case 1: return y;
			case 2: return z;
			case 3:
			default:
				return h;
		}
	}

	template <typename T>
	BALL_INLINE 
	const T& TVector4<T>::operator [] (Position pos) const
	{
		if (pos > 3)
		{
			throw Exception::IndexOverflow(__FILE__, __LINE__);
		}
		switch (pos) 
		{
			case 0: return x;
			case 1: return y;
			case 2: return z;
			case 3:
			default:
				return h;
		}
	}

	template <typename T>
	BALL_INLINE 
	TVector4<T> TVector4<T>::operator + () const
	{
		return *this;
	}

	template <typename T>
	BALL_INLINE 
	TVector4<T> TVector4<T>::operator - () const
	{
		return TVector4<T>(-x, -y, -z, -h);
	}

	template <typename T>
	BALL_INLINE 
	TVector4<T>& TVector4<T>::operator += (const TVector4<T>& v)
	{
		x += v.x;
		y += v.y;
		z += v.z;
		h += v.h;

		return *this;
	}

	template <typename T>
	BALL_INLINE 
	TVector4<T>& TVector4<T>::operator -= (const TVector4<T> &v)
	{
		x -= v.x;
		y -= v.y;
		z -= v.z;
		h -= v.h;

		return *this;
	}

	template <typename T>
	BALL_INLINE 
	TVector4<T> TVector4<T>::operator * (const T& scalar)
	{
		return TVector4<T>(x * scalar, y * scalar, z * scalar, h * scalar);
	}

	template <typename T>
	BALL_INLINE 
	TVector4<T>& TVector4<T>::operator *= (const T &scalar)
	{
		x *= scalar;
		y *= scalar;
		z *= scalar;
		h *= scalar;

		return *this;
	}

	template <typename T>
	TVector4<T>TVector4<T>::operator / (const T &scalar)
	{
		if (Maths::isZero(scalar))
		{
			throw Exception::DivisionByZero(__FILE__, __LINE__);
		}
		return TVector4<T>(x / scalar, y / scalar, z / scalar, h / scalar); 
	}

	template <typename T>
	TVector4<T>& TVector4<T>::operator /= (const T& scalar)
	{
		if (Maths::isZero(scalar))
		{
			throw Exception::DivisionByZero(__FILE__, __LINE__);
		}
		x /= scalar;
		y /= scalar;
		z /= scalar;
		h /= scalar;

		return *this;
	}

	template <typename T>
	BALL_INLINE 
	T TVector4<T>::operator * (const TVector4<T>& v) const
	{
		return (x * v.x + y * v.y + z * v.z + h * v.h);
	}

	template <typename T>
	BALL_INLINE 
	T TVector4<T>::getDistance(const TVector4<T> &v) const
	{
		T da = x - v.x;
		T db = y - v.y;
		T dc = z - v.z;
		T dd = h - v.h;
		
		return (T)sqrt(da * da + db * db + dc * dc + dd * dd); 
	}

	template <typename T>
	BALL_INLINE 
	T TVector4<T>::getSquareDistance(const TVector4<T> &v) const
	{
		T da = x - v.x;
		T db = y - v.y;
		T dc = z - v.z;
		T dd = h - v.h;
		
		return (da * da + db * db + dc * dc + dd * dd); 
	}

	template <typename T>
	BALL_INLINE 
	bool TVector4<T>::operator == (const TVector4<T>& v) const
	{
		return (Maths::isEqual(x, v.x) && Maths::isEqual(y, v.y) 
									&& Maths::isEqual(z, v.z) && Maths::isEqual(h, v.h));
	}

	template <typename T>
	BALL_INLINE 
	bool TVector4<T>::operator != (const TVector4<T>& v) const
	{
		return (Maths::isNotEqual(x, v.x) || Maths::isNotEqual(y, v.y)
									|| Maths::isNotEqual(z, v.z) || Maths::isNotEqual(h, v.h));
	}

	template <typename T>
	BALL_INLINE 
	bool TVector4<T>::isOrthogonalTo(const TVector4<T>& v) const
	{
		return Maths::isZero(*this * v);
	}

	template <typename T>
	BALL_INLINE 
	bool TVector4<T>::isValid() const
	{
		return true;
	}

	template <typename T>
	void TVector4<T>::dump(std::ostream& s, Size depth) const
	{
		BALL_DUMP_STREAM_PREFIX(s);

		BALL_DUMP_HEADER(s, this, this);

		BALL_DUMP_DEPTH(s, depth);
		s << "x= " << x 
			<< ", y = " << y
			<< ", z = " << z
			<< ", h = " << h << std::endl;

		BALL_DUMP_STREAM_SUFFIX(s);
	}

	/**	Default four-dimensional vector type
	*/
	typedef TVector4<float> Vector4;

	/**	Addition operator for two vectors
	*/
	template <typename T>
	BALL_INLINE 
	TVector4<T> operator + (const TVector4<T>& a, const TVector4<T>& b)
	{
		return TVector4<T>(a.x + b.x, a.y + b.y, a.z + b.z, a.h + b.h);
	}

	/** Subtraction operator for two vectors
  		@return TVector4 the new vector
	*/
	template <typename T>
	BALL_INLINE 
	TVector4<T> operator - (const TVector4<T>& a, const TVector4<T>& b)
	{
		return TVector4<T>(a.x - b.x, a.y - b.y, a.z - b.z, a.h - b.h);
	}

	/**	Multiplication operator for a scalar and a vector
  		@return TVector4 the new vector
	*/
	template <typename T>
	BALL_INLINE 
	TVector4<T> operator * (const T& scalar, const TVector4<T>& v)
	{
		return TVector4<T>(scalar * v.x, scalar * v.y, scalar * v.z, scalar * v.h);
	}

	/**	Multiplication operator for a vector and a scalar
  		@return TVector4 the new vector
	*/
	template <typename T>
	BALL_INLINE 
	TVector4<T> operator * (const TVector4<T>& v, const T& scalar)
	{
		return TVector4<T>(scalar * v.x, scalar * v.y, scalar * v.z, scalar * v.h);
	}

	/**	Input operator
			Reads four values of type <b>T</b> from an input stream and assigns them to 
			the components x, y, z and h of the vector.
	*/
	template <typename T>
	std::istream& operator >> (std::istream& s, TVector4<T>& v)
	{
		char c;
    s >> c >> v.x >> v.y >> v.z >> v.h >>c;  
		return s;
	}

	/**	Output Operator.
			Write the four components of the vector to an output stream.
			The values are enclosed by brackets. \par
			<b>Example:</b> \par
			<tt>(0.1 2.0 0 1)</tt>
	*/
	template <typename T>
	std::ostream& operator << (std::ostream& s, const TVector4<T>& v)
	{
		s << '(' <<v.x << ' ' << v.y << ' ' << v.z << ' ' << v.h << ')';
		
		return s;
	}
} // namespace BALL

#endif // BALL_MATHS_VECTOR4_H

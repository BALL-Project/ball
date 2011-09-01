// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_MATHS_VECTOR2_H
#define BALL_MATHS_VECTOR2_H

#ifndef BALL_CONCEPT_PERSISTENCEMANAGER_H
#	include <BALL/CONCEPT/persistenceManager.h>
#endif

#ifndef BALL_COMMON_EXCEPTION_H
#	include <BALL/COMMON/exception.h>
#endif

#ifndef BALL_MATHS_COMMON_H
#	include <BALL/MATHS/common.h>
#endif


namespace BALL 
{

	/**	\defgroup Vector2 Two-dimensional vectors.
			Representation of points and vectors in two-dimensional space:
			class  \link TVector2 TVector2 \endlink  and class  \link Vector2 Vector2 \endlink .

	\ingroup Primitives
	*/
	//@{

	template <typename T>
	class TVector2;

	/**	@name	Global binary operator functions for two dimensional vectors.
	*/
	//@{

	/** Multiply a vector with a scalar. The symmetric case is a member of the
			vector class.
	*/
	template <typename T>
	BALL_INLINE 
	TVector2<T> operator * (const T& scalar, const TVector2<T>& vector);

	/** Input stream.
	*/
	template <typename T>
	std::istream& operator >> (std::istream& s, TVector2<T>& vector);

	/* Output stream.
	*/
	template <typename T>
	std::ostream& operator << (std::ostream& s, const TVector2<T>& vector);
	
	//@}

	/** Generic Two-Dimensional Vector.
	*/
	template <typename T>
	class TVector2
		: public PersistentObject
	{
		public:

		BALL_CREATE(TVector2<T>)

		/**	@name	Constructors and Destructors
		*/
		//@{

		/**	Default constructor.
				This method creates a new TVector2 object. The two components
				are initialized to <tt>(T)0</tt>.
		*/
		TVector2();

		/**	Scalar constructor.
				Create a new vector with all components set
				to the same <tt>value</tt>.
				@param	value the value of all components
		*/
		explicit TVector2(const T& value);

		/**	Detailed constructor.
				Create a new TVector2 object from two variables of type <tt>T</tt>.
				@param	vx assigned to <tt>x</tt>
				@param	vy assigned to <tt>y</tt>
		*/
		TVector2(const T& vx, const T& vy);

		/**	Copy constructor.
				Create a new TVector2 object from another.
				@param vector the TVector2 object to be copied
		*/	
		TVector2(const TVector2& vector);

		/**	Array constructor.
				This constructor creates a TVector3 object from the first
				two elements pointed to by <tt>ptr</tt>.
				@param ptr the array to construct from
				@exception NullPointer if <tt>ptr == 0</tt>
		*/
		TVector2(const T* ptr);


		/**	Destructor.	
				Destructs the TVector2 object. As there are no dynamic
				data structures, nothing happens.
		*/	
		virtual ~TVector2();

		/** Clear method
				The values are set to 0.
		*/
		virtual void clear();

		//@}

		/**	@name Persistence 
		*/
		//@{

		/**	Persistent writing.
				Writes a TVector2 object to a persistent stream.
				@param pm the persistence manager
		*/
		virtual void persistentWrite(PersistenceManager& pm,
				const char* name = 0) const;

		/**	Persistent reading.
				Reads a TVector2 object from a persistent stream.
				@param pm the persistence manager
				@exception Exception::GeneralException
		*/
		virtual void persistentRead(PersistenceManager& pm);

		//@}

		/**	@name	Assignment 
		*/
		//@{

		/**	Assign from a scalar.
				Assign <tt>value</tt> to the two vector components.
				@param	value the new value of the components
		*/
		void set(const T& value);

		/**	Assign the vector components.
				@param vx the new x component
				@param vy the new y component
		*/
		void set(const T& vx, const T& vy);

		/**	Assign from another TVector2.
				@param vector	the TVector2 object to assign from
		*/
		void set(const TVector2& vector);

		/**	Assignment operator.
				Assign the vector components from another vector.
				@param v the vector to assign from
		**/
		TVector2& operator = (const TVector2& v);

		/** Assignment operator.
				Assign a constant value to the two vector components.
				@param value the constant to assign to x, y
		**/
		TVector2& operator = (const T& value);

		/**	Array assignment operator.
				Assigns the first two elements of an array to the vector components.
				@param	ptr the array
				@exception	NullPointer if <tt>ptr == 0</tt>
		*/
		TVector2& operator = (const T* ptr);

		/**	Return the length of the vector.
				The length of the vector is calculated as
				\f$\sqrt{x^2 + y^2}\f$.
				@return T, the vector length
		*/	
		T getLength() const;

		/**	Return the squared length of the vector.
				This method avoids the square root needed in getLength,
				so this method is preferred if possible.
				@return T, \f$x^2 + y^2\f$
		*/
		T getSquareLength() const;

		/**	Normalize the vector.
				The vector is scaled with its length:
				\f$\{x|y|z\} *= \sqrt{x^2 + y^2}\f$.
				@return T, a reference to the normalized vector
				@exception DivisionByZero if the length of the vector is 0
		*/
		TVector2& normalize();

		/**	Negate the vector.
				Negate the two components of the vector
				@return T, a reference to {\em *this} vector
		*/
		TVector2& negate();

		/**	Return a vector with all components 0.
		*/
		static const TVector2& getZero();

		/**	Return a vector with all components 1.
				@return: TVector4(1, 1, 1, 1)
		*/
		static const TVector2& getUnit();

		/**	Mutable array-like access to the components.
				@exception Exception::IndexOverflow if <tt>index > 1</tt>
		*/
		T& operator [] (Position position);

		/**	Constant array-like access to the components.
				@exception Exception::IndexOverflow if <tt>index > 1</tt>
		*/
		const T& operator [] (Position position) const;

		//@}
		/**	@name	Arithmetic operators
		*/
		//@{

		/**	Positive sign.
		*/
		const TVector2& operator + () const;

		/**	Negative sign.
		*/
		TVector2 operator - () const;

		/** Addition.
		*/
		TVector2 operator + (const TVector2& b) const;

		/** Subtraction.
		*/
		TVector2 operator - (const TVector2& b) const;

		/**	Add a vector to this vector.
				Add the components of <tt>vector</tt> to this vector.
				@param vector the vector to add
				@return TVector2&, {\em *this}
		*/
		TVector2& operator += (const TVector2& vector);

		/**	Subtract a vector from this vector.
				@param vector the vector to subtract
				@return TVector2&, {\em *this}
		*/
		TVector2& operator -= (const TVector2& vector);

		/**	Scalar product.
				Return <tt>TVector2(x * scalar, y * scalar)</tt>.
				The symmetric case is a global function.
				@param scalar, the scalar to multiply by
				@return TVector2, the scalar product of this vector and <tt>scalar</tt>
		*/
		TVector2 operator * (const T& scalar) const;

		/**	Multiply by a scalar.
				Multiply all components of the vector by a <tt>scalar</tt> value.
				@param scalar the to multiply by
				@return TVector2&, {\em *this}
		*/
		TVector2& operator *= (const T& scalar);

		/**	Fraction of a vector.
				Return <tt>TVector2(x / lambda, y / lambda)</tt>.
				@param lambda the scalar value to divide by
				@return TVector2& 
				@exception Exception::DivisionByZero if <tt>lambda == (T)0</tt>
		*/
		TVector2 operator / (const T& lambda) const;

		/**	Divide a vector by a scalar.
				@param lambda the scalar value to divide by
				@return TVector2&, {\em *this}
				@exception Exception::DivisionByZero if <tt>lambda == (T)0</tt>
		*/
		TVector2& operator /= (const T& lambda);

		/** Dot product.
				Return the dot product of this vector and <tt>vector</tt>.
		*/
		T operator * (const TVector2& vector) const;

		//@}

		/**	@name	Geometric properties
		*/
		//@{

		/**	Return the distance to another vector.
		*/
		T getDistance(const TVector2& vector) const;

		/**	Return the squared distance to another vector.
		*/
		T getSquareDistance(const TVector2& vector) const;

		//@}
	
		/**	@name	Predicates
		*/
		//@{

		/**	Equality operator.
				The function Maths::isEqual is used to compare the values. 
				 \link Maths::isEqual Maths::isEqual \endlink 
				@return bool, <b>true</b> if all two vector components are equal, <b>false</b> otherwise
		*/
		bool operator == (const TVector2& vector) const;
	
		/**	Inequality operator.
				The function Maths::isEqual is used to compare the values. 
				 \link Maths::isEqual Maths::isEqual \endlink 
				@return bool, <b>true</b> if the two vectors differ in at least one component, <b>false</b> otherwise
		*/
		bool operator != (const TVector2& vector) const;

		/**	Zero predicate.
				The function Maths::isZero is used to compare the values with zero. 
				 \link Maths::isZero Maths::isZero \endlink 
		*/
		bool isZero() const;

		/**	Orthogonality predicate.
		*/
		bool isOrthogonalTo(TVector2& vector) const;

		//@}


		/**	@name	Debugging and Diagnostics
		*/
		//@{

		/** Internal state dump.
				Dump the current internal state of {\em *this} to 
				the output ostream <b>  s </b> with dumping depth <b>  depth </b>.
				@param   s - output stream where to output the internal state of {\em *this}
				@param   depth - the dumping depth
		*/
		void dump(std::ostream& s = std::cout, Size depth = 0) const;

		/**	Test if instance is valid.
				Always returns true.
				@return bool <b>true</b>
		*/
		bool isValid() const;

		//@}


		/**	@name	Vector components
				For easier access, the two components of the vector
				are public members.
		*/
		//@{
	
		/**	x component of the vector
		*/
		T x;

		/**	y component of the vector
		*/
		T y;

		//@}

		private:

	};
	//@}
	
	template <typename T>
	TVector2<T>::TVector2()
		:	PersistentObject(),
			x(0),
			y(0)
	{
	}

	template <typename T>
	TVector2<T>::TVector2(const T& value)
		:	PersistentObject(),	
			x(value),
			y(value)
	{
	}

	template <typename T>
	TVector2<T>::TVector2(const T& vx, const T& vy)
		:	PersistentObject(),
			x(vx),
			y(vy)
	{
	}

	template <typename T>
	TVector2<T>::TVector2(const TVector2& vector)
		:	PersistentObject(),
			x(vector.x),
			y(vector.y)
	{
	}

	template <typename T>
	TVector2<T>::~TVector2()
	{
	}

	template <typename T>
	BALL_INLINE
	TVector2<T>::TVector2(const T* ptr)
	{
		if (ptr == 0) 
		{
			throw Exception::NullPointer(__FILE__, __LINE__);
		}

		x = *ptr++;
		y = *ptr;
	}

	template <typename T>
	void TVector2<T>::clear()
	{
		x = y = (T)0;
	}

	template <typename T>
  void TVector2<T>::persistentWrite(PersistenceManager& pm, const char* name) const
	{
		pm.writeObjectHeader(this, name);
			pm.writePrimitive(x, "x");
			pm.writePrimitive(y, "y");
		pm.writeObjectTrailer(name);
	}

	template <typename T>
	void TVector2<T>::persistentRead(PersistenceManager& pm)
	{
		pm.readPrimitive(x, "x");
		pm.readPrimitive(y, "y");
	}
 
	template <typename T>
	BALL_INLINE 
	void TVector2<T>::set(const T& value)
	{
		x = value;
		y = value;
	}

	template <typename T>
	BALL_INLINE 
	void TVector2<T>::set(const T& vx, const T& vy)
	{
		x = vx;
		y = vy;
	}

	template <typename T>
	BALL_INLINE 
	void TVector2<T>::set(const TVector2<T>& vector)
	{
		x = vector.x;
		y = vector.y;
	}

	template <typename T>
	BALL_INLINE 
	TVector2<T>& TVector2<T>::operator = (const TVector2<T>& vector)
	{
		x = vector.x;
		y = vector.y;

		return *this;
	}

	template <typename T>
	BALL_INLINE 
	TVector2<T>& TVector2<T>::operator = (const T* ptr)
	{
		if (ptr == 0)
		{
			throw Exception::NullPointer(__FILE__, __LINE__);
		}
		x = *ptr++;;
		y = *ptr;;

		return *this;
	}

	template <typename T>
	BALL_INLINE
	TVector2<T>& TVector2<T>::operator = (const T& value)
	{
		x = value;
		y = value;

		return *this;
	}

	template <typename T>
	BALL_INLINE 
	T TVector2<T>::getLength() const
	{
		return (T)sqrt(x * x + y * y);
	}

	template <typename T>
	BALL_INLINE 
	T TVector2<T>::getSquareLength() const
	{
		return (T)(x * x + y * y);
	}

	template <typename T>
	TVector2<T>& TVector2<T>::normalize()
	{
		T len = (T)sqrt(x * x + y * y);

		if (Maths::isZero(len)) 
		{
			throw Exception::DivisionByZero(__FILE__, __LINE__);
		}
		
		x /= len;
		y /= len;

		return *this;
	}

	template <typename T>
	TVector2<T>& TVector2<T>::negate()
	{
		x *= -1;
		y *= -1;
		return *this;
	}

	template <typename T>
	BALL_INLINE 
	const TVector2<T>& TVector2<T>::getZero()
	{
		static TVector2<T> null_vector(0, 0);
		return null_vector;
	}

	template <typename T>
	BALL_INLINE 
	const TVector2<T>& TVector2<T>::getUnit()
	{
		static TVector2<T> unit_vector(1, 1);
		return unit_vector;
	}

	template <typename T>
	BALL_INLINE 
	T& TVector2<T>::operator [] (Position position)
	{
		if (position > 1)
		{
			throw Exception::IndexOverflow(__FILE__, __LINE__);
		}
		switch (position) 
		{
			case 0: return x;
			case 1:
			default:
				return y;
		}
	}

	template <typename T>
	BALL_INLINE 
	const T& TVector2<T>::operator [] (Position position) const
	{
		if (position > 1)
		{
			throw Exception::IndexOverflow(__FILE__, __LINE__);
		}
		switch (position) 
		{
			case 0: return x;
			case 1:
			default:
				return y;
		}
	}

	template <typename T>
	BALL_INLINE
	const TVector2<T>& TVector2<T>::operator + () const	
	{
		return *this;
	}

	template <typename T>
	BALL_INLINE
	TVector2<T> TVector2<T>::operator - () const	
	{
		return TVector2<T>(-x, -y);
	}

	template <typename T>
	BALL_INLINE 
	TVector2<T> TVector2<T>::operator + (const TVector2<T>& b) const
	{
		return TVector2<T>(x + b.x, y + b.y);
	}
	
	template <typename T>
	BALL_INLINE
	TVector2<T> TVector2<T>::operator - (const TVector2<T>& b) const
	{
		return TVector2<T>(x - b.x, y - b.y);
	}

	template <typename T>
	BALL_INLINE 
	TVector2<T>& TVector2<T>::operator += (const TVector2<T>& vector)
	{
		x += vector.x;
		y += vector.y;

		return *this;
	}

	template <typename T>
	BALL_INLINE 
	TVector2<T>& TVector2<T>::operator -= (const TVector2<T>& vector)
	{
		x -= vector.x;
		y -= vector.y;

		return *this;
	}

	template <typename T>
	BALL_INLINE 
	TVector2<T> TVector2<T>::operator * (const T& scalar) const 
	{
		return TVector2<T>(x * scalar, y * scalar);
	}

	template <typename T>
	BALL_INLINE 
	TVector2<T>& TVector2<T>::operator *= (const T &scalar)
	{
		x *= scalar;
		y *= scalar;

		return *this;
	}

	template <typename T>
	TVector2<T> TVector2<T>::operator / (const T& lambda) const
	{
		if (lambda == (T)0)
		{
			throw Exception::DivisionByZero(__FILE__, __LINE__);
		}
		return TVector2<T>(x / lambda, y / lambda);
	}

	template <typename T>
	TVector2<T>& TVector2<T>::operator /= (const T& lambda)
	{
		if (lambda == (T)0)
		{
			throw Exception::DivisionByZero(__FILE__, __LINE__);		
		}
		x /= lambda;
		y /= lambda;

		return *this;
	}

	template <typename T>
	BALL_INLINE 
	T TVector2<T>::operator * (const TVector2<T>& vector) const
	{
		return (x * vector.x + y * vector.y);
	}

	template <typename T>
	BALL_INLINE 
	T TVector2<T>::getDistance(const TVector2<T>& v) const
	{
		T dx = x - v.x;
		T dy = y - v.y;
		
		return (T)sqrt(dx * dx + dy * dy); 
	}

	template <typename T>
	BALL_INLINE 
	T TVector2<T>::getSquareDistance(const TVector2<T>& v) const
	{
		T dx = x - v.x;
		T dy = y - v.y;
		
		return (dx * dx + dy * dy); 
	}

	template <typename T>
	BALL_INLINE 
	bool TVector2<T>::operator == (const TVector2<T>& v) const
	{
		return (Maths::isEqual(x, v.x) && Maths::isEqual(y, v.y));
	}

	template <typename T>
	BALL_INLINE 
	bool TVector2<T>::operator != (const TVector2<T>& v) const
	{
		return (Maths::isNotEqual(x, v.x) || Maths::isNotEqual(y, v.y));
	}

	template <typename T>
	BALL_INLINE 
	bool TVector2<T>::isOrthogonalTo(TVector2<T>& v) const
	{
		return Maths::isZero((*this) * v);
	}

	template <typename T>
	BALL_INLINE 
	bool TVector2<T>::isValid() const
	{
		return true;
	}

	template <typename T>
	BALL_INLINE 
	bool TVector2<T>::isZero() const
	{
		return (Maths::isZero(x) && Maths::isZero(y));
	}

	template <typename T>
	void TVector2<T>::dump(std::ostream& s, Size depth) const
	{
		BALL_DUMP_STREAM_PREFIX(s);

		BALL_DUMP_HEADER(s, this, this);

		BALL_DUMP_DEPTH(s, depth);
		s << "  (x =  " << x << ", y = " << y << ")" << std::endl;

		BALL_DUMP_STREAM_SUFFIX(s);
	}

	/**	Default two-dimensional vector class.
			This is the class used in BALL kernel to represent points, coordinates.
	 
			\ingroup Vector2
	*/
	typedef TVector2<float> Vector2;

	template <typename T>
	BALL_INLINE 
	TVector2<T> operator * (const T& scalar, const TVector2<T>& vector)
	{
		return TVector2<T>(scalar * vector.x, scalar * vector.y);
	}

	template <typename T>
	std::istream& operator >> (std::istream& s, TVector2<T>& v)
	{
		char c;
		s >> c >> v.x >> v.y >> c;

		return s;
	}

	template <typename T>
	std::ostream& operator << (std::ostream& s, const TVector2<T>& v)
	{
		s << "(" << v.x << ' ' << v.y << ')';

		return s;
	}

}// namespace BALL

#endif // BALL_MATHS_VECTOR2_H

// $Id: vector2.h,v 1.1 2001/05/31 21:58:00 amoll Exp $

#ifndef BALL_MATHS_Vector2_H
#define BALL_MATHS_Vector2_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#include <math.h>

#ifndef BALL_CONCEPT_PERSISTENCEMANAGER_H
#	include <BALL/CONCEPT/persistenceManager.h>
#endif

#ifndef BALL_MATHS_COMMON_H
#	include <BALL/MATHS/common.h>
#endif


namespace BALL 
{
	/**	@name	two-dimensional vector.
			@memo representation of points and vectors in two-dimensional space: class \Ref{TVector2} and class \Ref{Vector2}
	*/
	//@{

	template <typename T>
	class TVector2;

	template <typename T>
	BALL_INLINE 
	TVector2<T> operator + (const TVector2<T>& a, const TVector2<T>& b)
		throw();

	template <typename T>
	BALL_INLINE 
	TVector2<T> operator - (const TVector2<T>& a, const TVector2<T>& b)
		throw();

	template <typename T>
	std::istream& operator >> (std::istream& s, TVector2<T>& vector)
		throw();

	template <typename T>
	std::ostream& operator << (std::ostream& s, const TVector2<T>& vector)
		throw();

	/** Generic Two-Dimensional Vector.
      {\bf Definition:} \URL{BALL/MATHS/Vector2.h}
      \\
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
				are initialized to {\tt (T)0}.
		*/
		TVector2()
			throw()
			:	PersistentObject(),
				x(0),
				y(0)
		{
		}

		/**	Scalar constructor.
				Create a new vector with all components set
				to the same {\tt value}.
				@param	value the value of all components
		*/
		TVector2(const T& value)
			throw()
			:	PersistentObject(),	
				x(value),
				y(value)
		{
		}

		/**	Detailed constructor.
				Create a new TVector2 object from two variables of type {\tt T}.
				@param	vx assigned to {\tt x}
				@param	vy assigned to {\tt y}
		*/
		TVector2(const T& vx, const T& vy)
			throw()
			:	PersistentObject(),
				x(vx),
				y(vy)
		{
		}

		/**	Copy constructor.
				Create a new TVector2 object from another.
				@param vector the TVector2 object to be copied
		*/	
		TVector2(const TVector2& vector)
			throw()
			:	PersistentObject(),
				x(vector.x),
				y(vector.y)
		{
		}

		/**	Destructor.	
				Destructs the TVector2 object. As there are no dynamic
				data structures, nothing happens.
		*/	
		virtual ~TVector2()
			throw()
		{
		}

		/** Clear method
				The values are set to 0.
		*/
		virtual void clear()
			throw()
		{
			x = y = (T)0;
		}

		//@}

		/**	@name Persistence 
		*/
		//@{

		/**	Persistent writing.
				Writes a TVector2 object to a persistent stream.
				@param pm the persistence manager
		*/
		virtual void persistentWrite(PersistenceManager& pm, const char* name = 0) const
			throw();

		/**	Persistent reading.
				Reads a TVector2 object from a persistent stream.
				@param pm the persistence manager
		*/
		virtual void persistentRead(PersistenceManager& pm)
			throw();

		//@}

		/**	@name	Assignment 
		*/
		//@{

		/**	Assign from a scalar.
				Assign {\tt value} to the two vector components.
				@param	value the new value of the components
		*/
		void set(const T& value)
			throw();

		/**	Assign the vector components.
				@param vx the new x component
				@param vy the new y component
		*/
		void set(const T& vx, const T& vy)
			throw();

		/**	Assign from another TVector2.
				@param vector	the TVector2 object to assign from
		*/
		void set(const TVector2& vector)
			throw();

		/**	Assignment operator.
				Assign the vector components from another vector.
				@param v the vector to assign from
		**/
		const TVector2& operator = (const TVector2& v)
			throw();

		/**	Array assignment operator.
				Assigns the first two elements of an array to the vector components.
				@param	ptr the array
				@exception	NullPointer if {\tt ptr == 0}
		*/
		const TVector2& operator = (const T* ptr)
			throw(Exception::NullPointer);

		/**	Assign to two variables of type {\tt T}.
				@param	x the x component
				@param	y the y component
		*/
		void get(T& x, T& y) const
			throw();

		/**	Assign to another Vector2.
				Assigns the vector components to another vector.
				@param vector	the vector to be assigned to
		*/
		void get(TVector2& vector) const
			throw();

		/**	Swap the contents of two vectors.
				@param	vector the vector to swap contents with
		*/
		void swap(TVector2& vector)
			throw();

		/**	Return the length of the vector.
				The length of the vector is calculated as
				$\sqrt{x^2 + y^2}$.
				@return T, the vector length
		*/	
		T getLength() const
			throw();

		/**	Return the squared length of the vector.
				This method avoids the square root needed in getLength,
				so this method is preferred if possible.
				@return T, $x^2 + y^2$
		*/
		T getSquareLength() const
			throw();

		/**	Normalize the vector.
				The vector is scaled with its length:
				$\{x|y|z\} *= \sqrt{x^2 + y^2}$.
				@return T, a reference to the normalized vector
				@exception DivisionByZero if the length of the vector is 0
		*/
		TVector2& normalize()
			throw(Exception::DivisionByZero);

		/**	Negate the vector.
				Negate the two components of the vector
				@return T, a reference to {\em *this} vector
		*/
		TVector2& negate()
			throw();

		/**	Return a vector with all components 0.
		*/
		static const TVector2& getZero()
			throw();

		/**	Return a vector with all components 1.
				@return: TVector4(1, 1, 1, 1)
		*/
		static const TVector2& getUnit()
			throw();

		/**	Mutable array-like access to the components.
				@exception Exception::IndexOverflow if {\tt index > 1}
		*/
		T& operator [] (Position position)
			throw(Exception::IndexOverflow);

		/**	Constant array-like access to the components.
				@exception Exception::IndexOverflow if {\tt index > 1}
		*/
		const T& operator [] (Position position) const
			throw(Exception::IndexOverflow);
		//@}
		
		/**	@name	Arithmetic operators
		*/
		//@{

		/**	Positive sign.
		*/
		const TVector2& operator + () const
			throw();

		/**	Negative sign.
		*/
		TVector2 operator - () const
			throw();

		/**	Add a vector to this vector.
				Add the components of {\tt vector} to this vector.
				@param vector the vector to add
				@return TVector2&, {\em *this}
		*/
		TVector2& operator += (const TVector2& vector)
			throw();

		/**	Subtract a vector from this vector.
				@param vector the vector to subtract
				@return TVector2&, {\em *this}
		*/
		TVector2& operator -= (const TVector2& vector)
			throw();

		/**	Scalar product.
				Return {\tt TVector2(x * scalar, y * scalar)}.
				@param scalar, the scalar to multiply by
				@return TVector2, the scalar product of this vector and {\tt scalar}
		*/
		TVector2 operator * (const T& scalar) const
			throw();

		/**	Multiply by a scalar.
				Multiply all components of the vector by a {\tt scalar} value.
				@param scalar the to multiply by
				@return TVector2&, {\em *this}
		*/
		TVector2& operator *= (const T& scalar)
			throw();

		/**	Fraction of a vector.
				Return {\tt TVector2(x / lambda, y / lambda)}.
				@param lambda the scalar value to divide by
				@return TVector2& 
				@exception Exception::DivisionByZero if {\tt lambda == (T)0}
		*/
		TVector2 operator / (const T& lambda) const
			throw(Exception::DivisionByZero);

		/**	Divide a vector by a scalar.
				@param lambda the scalar value to divide by
				@return TVector2&, {\em *this}
				@exception Exception::DivisionByZero if {\tt lambda == (T)0}
		*/
		TVector2& operator /= (const T& lambda)
			throw(Exception::DivisionByZero);

		/** Dot product.
				Return the dot product of this vector and {\tt vector}.
		*/
		T operator * (const TVector2& vector) const
			throw();

		/** Cross product.
				Return the cross product of this vector and {\tt vector}.
		*/
		TVector2 operator % (const TVector2& vector) const
			throw();

		/**	Assign to the cross product.
				Assign the vector to its cross product with another vector.
		*/
		TVector2& operator %= (const TVector2& vector)
			throw();

		//@}

		/**	@name	Geometric properties
		*/
		//@{

		/**	Return the distance to another vector.
		*/
		T getDistance(const TVector2& Vector2) const
			throw();

		/**	Return the squared distance to another vector.
		*/
		T getSquareDistance(const TVector2& vector) const
			throw();

		//@}
	
		/**	@name	Predicates
		*/
		//@{

		/**	Equality operator.
				The function Maths::isEqual is used to compare the values. 
				\Ref{Maths::isEqual}
				@return bool, {\bf true} if all two vector components are equal, {\bf false} otherwise
		*/
		bool operator == (const TVector2& vector) const
			throw();
	
		/**	Inequality operator.
				The function Maths::isEqual is used to compare the values. 
				\Ref{Maths::isEqual}
				@return bool, {\bf true} if the two vectors differ in at least one component, {\bf false} otherwise
		*/
		bool operator != (const TVector2& vector) const
			throw();

		/**	Zero predicate.
				The function Maths::isZero is used to compare the values with zero. 
				\Ref{Maths::isZero}
		*/
		bool isZero() const
			throw();

		/**	Orthogonality predicate.
		*/
		bool isOrthogonalTo(TVector2& vector) const
			throw();

		//@}


		/**	@name	Debugging and Diagnostics
		*/
		//@{

		/** Internal state dump.
				Dump the current internal state of {\em *this} to 
				the output ostream {\em s} with dumping depth {\em depth}.
				@param   s - output stream where to output the internal state of {\em *this}
				@param   depth - the dumping depth
		*/
		void dump(std::ostream& s = std::cout, Size depth = 0) const
			throw();

		/**	Test if instance is valid.
				Always returns true.
				@return bool {\bf true}
		*/
		bool isValid() const
			throw();

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

	template <typename T>
  void TVector2<T>::persistentWrite(PersistenceManager& pm, const char* name) const
		throw()
	{
		pm.writeObjectHeader(this, name);
			pm.writePrimitive(x, "x");
			pm.writePrimitive(y, "y");
		pm.writeObjectTrailer(name);
	}

	template <typename T>
	void TVector2<T>::persistentRead(PersistenceManager& pm)
		throw()
	{
		pm.readPrimitive(x, "x");
		pm.readPrimitive(y, "y");
	}
 
	template <typename T>
	BALL_INLINE 
	void TVector2<T>::set(const T& value)
		throw()
	{
		x = value;
		y = value;
	}

	template <typename T>
	BALL_INLINE 
	void TVector2<T>::set(const T& vx, const T& vy)
		throw()
	{
		x = vx;
		y = vy;
	}

	template <typename T>
	BALL_INLINE 
	void TVector2<T>::set(const TVector2<T>& vector)
		throw()
	{
		x = vector.x;
		y = vector.y;
	}

	template <typename T>
	BALL_INLINE 
	const TVector2<T>& TVector2<T>::operator = (const TVector2<T>& vector)
		throw()
	{
		x = vector.x;
		y = vector.y;

		return *this;
	}

	template <typename T>
	BALL_INLINE 
	void TVector2<T>::get(T& new_x, T& new_y) const
		throw()
	{
		new_x = x;
		new_y = y;
	}

	template <typename T>
	BALL_INLINE 
	void TVector2<T>::get(TVector2<T>& vector) const
		throw()
	{
		vector.x = x;
		vector.y = y;
	}

	template <typename T>
	void TVector2<T>::swap(TVector2<T>& vector)
		throw()
	{
		T temp = x;
		x = vector.x;
		vector.x = temp;

		temp = y;
		y = vector.y;
		vector.y = temp;
	}

	template <typename T>
	BALL_INLINE 
	T TVector2<T>::getLength() const
		throw()
	{
		return (T)sqrt(x * x + y * y);
	}

	template <typename T>
	BALL_INLINE 
	T TVector2<T>::getSquareLength() const
		throw()
	{
		return (x * x + y * y);
	}

	template <typename T>
	TVector2<T>& TVector2<T>::normalize()
		throw(Exception::DivisionByZero)
	{
		T len = sqrt(x * x + y * y);

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
		throw()
	{
		x *= -1;
		y *= -1;
		return *this;
	}

	template <typename T>
	BALL_INLINE 
	const TVector2<T>& TVector2<T>::getZero()
		throw()
	{
		static TVector2<T> null_vector(0, 0);
		return null_vector;
	}

	template <typename T>
	BALL_INLINE 
	const TVector2<T>& TVector2<T>::getUnit()
		throw()
	{
		static TVector2<T> unit_vector(1, 1);
		return unit_vector;
	}

	template <typename T>
	BALL_INLINE 
	T& TVector2<T>::operator [] (Position position)
		throw(Exception::IndexOverflow)
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
		throw(Exception::IndexOverflow)
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
		throw()
	{
		return *this;
	}

	template <typename T>
	BALL_INLINE
	TVector2<T> TVector2<T>::operator - () const	
		throw()
	{
		return TVector2<T>(-x, -y);
	}

	template <typename T>
	BALL_INLINE 
	TVector2<T>& TVector2<T>::operator += (const TVector2<T>& vector)
		throw()
	{
		x += vector.x;
		y += vector.y;

		return *this;
	}

	template <typename T>
	BALL_INLINE 
	TVector2<T>& TVector2<T>::operator -= (const TVector2<T>& vector)
		throw()
	{
		x -= vector.x;
		y -= vector.y;

		return *this;
	}

	template <typename T>
	BALL_INLINE 
	TVector2<T> TVector2<T>::operator * (const T& scalar) const 
		throw()
	{
		return TVector2<T>(x * scalar, y * scalar);
	}

	template <typename T>
	BALL_INLINE 
	TVector2<T>& TVector2<T>::operator *= (const T &scalar)
		throw()
	{
		x *= scalar;
		y *= scalar;

		return *this;
	}

	template <typename T>
	TVector2<T> TVector2<T>::operator / (const T& lambda) const
		throw(Exception::DivisionByZero)
	{
		if (lambda == (T)0)
		{
			throw Exception::DivisionByZero(__FILE__, __LINE__);
		}
		return TVector2<T>(x / lambda, y / lambda);
	}

	template <typename T>
	TVector2<T>& TVector2<T>::operator /= (const T& lambda)
		throw(Exception::DivisionByZero)
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
		throw()
	{
		return (x * vector.x + y * vector.y);
	}

	template <typename T>
	TVector2<T> TVector2<T>::operator % (const TVector2<T>& v) const
		throw()
	{//BAUSTELLE: formula
		return TVector2(v.y * x - y * v.x, y * v.x - x * v.y);
	}

	template <typename T>
	BALL_INLINE 
	TVector2<T>& TVector2<T>::operator %= (const TVector2<T>& v)
		throw()
	{//BAUSTELLE: formula
		set(v.y * x - y * v.x, y * v.x - x * v.y);
		return *this;
	}

	template <typename T>
	BALL_INLINE 
	T TVector2<T>::getDistance(const TVector2<T>& v) const
		throw()
	{
		T dx = x - v.x;
		T dy = y - v.y;
		
		return (T)sqrt(dx * dx + dy * dy); 
	}

	template <typename T>
	BALL_INLINE T
	TVector2<T>::getSquareDistance(const TVector2<T>& v) const
		throw()
	{
		T dx = x - v.x;
		T dy = y - v.y;
		
		return (dx * dx + dy * dy); 
	}

	template <typename T>
	BALL_INLINE 
	bool TVector2<T>::operator == (const TVector2<T>& v) const
		throw()
	{
		return (Maths::isEqual(x, v.x) && Maths::isEqual(y, v.y));
	}

	template <typename T>
	BALL_INLINE 
	bool TVector2<T>::operator != (const TVector2<T>& v) const
		throw()
	{
		return (Maths::isNotEqual(x, v.x) || Maths::isNotEqual(y, v.y));
	}

	template <typename T>
	BALL_INLINE 
	bool TVector2<T>::isOrthogonalTo(TVector2<T>& v) const
		throw()
	{
		return Maths::isZero((*this) * v);
	}

	template <typename T>
	BALL_INLINE 
	bool TVector2<T>::isValid() const
		throw()
	{
		return true;
	}

	template <typename T>
	BALL_INLINE 
	bool TVector2<T>::isZero() const
		throw()
	{
		return (Maths::isZero(x) && Maths::isZero(y));
	}

	template <typename T>
	void TVector2<T>::dump(std::ostream& s, Size depth) const
		throw()
	{
		BALL_DUMP_STREAM_PREFIX(s);

		BALL_DUMP_HEADER(s, this, this);

		BALL_DUMP_DEPTH(s, depth);
		s << "  (x =  " << x << ", y = " << y << ")" << std::endl;

		BALL_DUMP_STREAM_SUFFIX(s);
	}

	/**	Default two-dimensional vector class.
			This is the class used in BALL kernel to represent points, coordinates.
	*/
	typedef TVector2<float> Vector2;

	/**	Operators
	*/
	//@{
	/** Addition operator for two vectors
  		@return TVector2 - the new vector
	*/
	template <typename T>
	BALL_INLINE 
	TVector2<T> operator + (const TVector2<T>& a, const TVector2<T>& b)
		throw()
	{
		return TVector2<T>(a.x + b.x, a.y + b.y);
	}
	
	/** Subtraction operator of two vectors
  		@return TVector2 the new vector
	*/
	template <typename T>
	BALL_INLINE
	TVector2<T> operator - (const TVector2<T>& a, const TVector2<T>& b)
		throw()
	{
		return TVector2<T>(a.x - b.x, a.y - b.y);
	}

	/**	Multiplication operator for a scalar and a vector
  		@return TVector2 - the new vector
	*/
	template <typename T>
	BALL_INLINE 
	TVector2<T> operator * (const T& scalar, const TVector2<T>& vector)
		throw()
	{
		return TVector2<T>(scalar * vector.x, scalar * vector.y);
	}

	/**	Input operator.
			Reads the values of {\tt two} vector components of type {\em T}
			from an istream. The components are read in the order of x, y.
	*/
	template <typename T>
	std::istream& operator >> (std::istream& s, TVector2<T>& v)
		throw()
	{
		char c;
		s >> c >> v.x >> v.y >> c;

		return s;
	}

	/**	Output operator.
			Writes the values of {\tt two} vector components of type {\em T}
			to an ostream. The components are writen in the order of x, y.
	*/
	template <typename T>
	std::ostream& operator << (std::ostream& s, const TVector2<T>& v)
		throw()
	{
		s << "(" << v.x << ' ' << v.y << ')';

		return s;
	}

	//@}
	//@}
		
}// namespace BALL

#endif // BALL_MATHS_Vector2_H

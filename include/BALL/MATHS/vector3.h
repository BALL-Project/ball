// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_MATHS_VECTOR3_H
#define BALL_MATHS_VECTOR3_H

#ifndef BALL_COMMON_EXCEPTION_H
#	include <BALL/COMMON/exception.h>
#endif

#ifndef BALL_CONCEPT_PERSISTENCEMANAGER_H
#	include <BALL/CONCEPT/persistenceManager.h>
#endif

#ifndef BALL_MATHS_ANGLE_H
#	include <BALL/MATHS/angle.h>
#endif

#ifndef BALL_MATHS_COMMON_H
#	include <BALL/MATHS/common.h>
#endif

#ifdef BALL_HAS_IEEEFP_H
#	include <ieeefp.h>
#endif 


namespace BALL 
{
	/**	\defgroup Vector3 Three-dimensional vector.
			Representation of points and vectors in three-dimensional space: class  \link TVector3 TVector3 \endlink  and class  \link Vector3 Vector3 \endlink 
	\ingroup Primitives
	*/
	//@{

	template <typename T>
	class TVector3;

	/**	@name	Global binary operators for three-dimensional vectors.
	*/
	//@{

	/** Multiply a vector with a scalar. The symmetric case is a member of the
			vector class.
	*/
	template <typename T>
	BALL_INLINE 
	TVector3<T> operator * (const T& a, const TVector3<T>& b);

	/**	Input operator.
			Reads the values of <tt>three</tt> vector components of type <b>  T </b>
			from an istream. The components are read in the order of x, y, z.
	*/
	template <typename T>
	std::istream& operator >> (std::istream& s, TVector3<T>& vector);

	/**	Output operator.
			Writes the values of <tt>three</tt> vector components of type <b>  T </b>
			to an ostream. The components are writen in the order of x, y, z.
	*/
	template <typename T>
	std::ostream& operator << (std::ostream& s, const TVector3<T>& vector);

	//@}

	/** Generic Three-Dimensional Vector.
	*/
	template <typename T>
	class TVector3
	{
		public:

		/**	@name	Constructors and Destructors
		*/
		//@{

		/**	Default constructor.
				This method creates a new TVector3 object. The three components
				are initialized to <tt>(T)0</tt>.
		*/
		TVector3();

		/**	Array constructor.
				This constructor creates a TVector3 object from the first
				three elements pointed to by <tt>ptr</tt>.
				@param ptr the array to construct from
				@exception NullPointer if <tt>ptr == 0</tt>
		*/
		explicit TVector3(const T* ptr);

		/**	Scalar constructor.
				Create a new vector with all components set
				to the same <tt>value</tt>.
				@param	value the value of all components
		*/
		explicit TVector3(const T& value);

		/**
		 * Explicit type conversion constructor between a vector of type T and T2
		 */
		template<typename T2>
		explicit TVector3(const TVector3<T2>& vec);

		/**	Detailed constructor.
				Create a new TVector3 object from three variables of type <tt>T</tt>.
				@param	vx assigned to <tt>x</tt>
				@param	vy assigned to <tt>y</tt>
				@param	vz assigned to <tt>z</tt>
		*/
		TVector3(const T& vx, const T& vy, const T& vz);

		/**	Copy constructor.
				Create a new TVector3 object from another.
				@param vector the TVector3 object to be copied
		*/	
		TVector3(const TVector3& vector);
		
		/**	Spherical polar coordinate constructor.
				Create a TVector3 object and set its coordinates to 
				the point described by the three spherical polar coordinates
				<tt>r</tt> (radius), <tt>phi</tt> (azimuth), and <tt>theta</tt> (co-latitude).
				@see set(const T& r, const TAngle<T>& phi, const TAngle<T>& theta)
				@param r the radius
				@param phi the azimuth 
				@param theta the co-latitude
		*/
		TVector3(const T& r, const TAngle<T>& phi, const TAngle<T>& theta);

		/**	Destructor.	
				Destructs the TVector3 object. As there are no dynamic
				data structures, nothing happens.
		*/	
		~TVector3();

		/** Clear method
				The values are set to 0.
		*/
		void clear();

		//@}

		/**	@name	Assignment 
		*/
		//@{

		/**	Assign from an array.
				Assign the three components <tt>x</tt>, <tt>y</tt>, and <tt>z</tt> from
				the first three elements of the array pointed to by <tt>ptr</tt>.
				@param ptr an array
				@exception Nullpointer if <tt>ptr == 0</tt>
		*/
		void set(const T* ptr);

		/**	Assign from a scalar.
				Assign <tt>value</tt> to the three vector components.
				@param	value the new value of the components
		*/
		void set(const T& value);

		/**	Assign the vector components.
				@param vx the new x component
				@param vy the new y component
				@param vz the new z component
		*/
		void set(const T& vx, const T& vy, const T& vz);

		/**	Assign from another TVector3.
				@param vector	the TVector3 object to assign from
		*/
		void set(const TVector3& vector);

		/**	Assign from spherical polar coordinates.
				The radius describes the distance of the point from the origin. \par
				<tt>phi</tt>	ranges from 0 to \f$2 \pi\f$, <tt>theta</tt> ranges from 0 (north pole, positive z-axis)
				to \f$\pi\f$ (south pole, negative z-axis). \par
				Coordinates are calculated according to the following formulae: \par
				\f[ x  =  r \sin \theta \cos \phi \f] \par
				\f[ y  =  r \sin \theta \sin \phi \f] \par
				\f[ z  =  r \cos \theta \f] 
				@param	r the radius
				@param	phi the azimuth
				@param	theta	the co-latitude
		*/
		void set(const T& r, const TAngle<T>& phi, const TAngle<T>& theta);

		/**	Assignment operator.
				Assign the vector components from another vector.
				@param v the vector to assign from
		**/
		TVector3& operator = (const TVector3& v);

		/**	Assignment operator.
				Assign a constant value to all three vector components.
				@param value the constant to assign to x, y, z
		**/
		TVector3& operator = (T value);

		/**	Array assignment operator.
				Assigns the first three elements of an array to the vector components.
				@param	ptr the array
				@exception	NullPointer if <tt>ptr == 0</tt>
		*/
		TVector3& operator = (const T* ptr);

		/**	Assign to an array.
				Sets the first three array elements pointed to by <tt>ptr</tt> 
				to the values of the three vector components.
				@param ptr the array
				@exception	NullPointer if <tt>ptr == 0</tt>
		*/
		void get(T* ptr) const;

		/**	Assign to three variables of type <tt>T</tt>.
				@param	x the x component
				@param	y the y component
				@param	z the z component
		*/
		void get(T& x, T& y, T& z) const;

		/**	Assign to another Vector3.
				Assigns the vector components to another vector.
				@param vector	the vector to be assigned to
		*/
		void get(TVector3& vector) const;

		/**	Assign to polar coordinates.
				Sets <tt>r</tt>, <tt>phi</tt>, and <tt>theta</tt> to the
				coordinates of the vector in spherical polar coordinates.
				@param	r the radius (returned)
				@param  phi the azimuth (returned)
				@param	theta the co-latitude (returned)
		*/
		void get(T& r, TAngle<T>& phi, TAngle<T>& theta) const;

		/**	Swap the contents of two vectors.
				@param	vector the vector to swap contents with
		*/
		void swap(TVector3& vector);

		/**	Return the length of the vector.
				The length of the vector is calculated as
				\f$\sqrt{x^2 + y^2 + z^2}\f$.
				@return T, the vector length
		*/	
		T getLength() const;

		/**	Return the squared length of the vector.
				This method avoids the square root needed in getLength,
				so this method is preferred if possible.
				@return T, \f$x^2 + y^2 + z^2\f$
		*/
		T getSquareLength() const;

		/**	Normalize the vector.
				The vector is scaled with its length:
				\f$\{x|y|z\} *= \sqrt{x^2 + y^2 + z^2}\f$.
				@return T, a reference to the normalized vector
				@exception DivisionByZero if the length of the vector is 0
		*/
		TVector3& normalize();

		/**	Negate the vector.
				Negate the three components of the vector
				@return T, a reference to {\em *this} vector
		*/
		TVector3& negate();

		/**	Return a vector with all components 0.
		*/
		static const TVector3& getZero();

		/**	Return a vector with all components 1.
				@return: TVector4(1, 1, 1, 1)
		*/
		static const TVector3& getUnit();

		/**	Mutable array-like access to the components.
				@exception Exception::IndexOverflow if <tt>index > 2</tt>
		*/
		T& operator [] (Position position);

		/**	Constant array-like access to the components.
				@exception Exception::IndexOverflow if <tt>index > 2</tt>
		*/
		const T& operator [] (Position position) const;
		//@}
		
		/**	@name	Arithmetic operators
		*/
		//@{

		/**	Positive sign.
		*/
		const TVector3& operator + () const;

		/**	Negative sign.
		*/
		TVector3 operator - () const;

		/** Addition.
		*/
		TVector3 operator + (const TVector3& b) const;

		/** Subtraction.
		*/
		TVector3 operator - (const TVector3& b) const;

		/**	Add a vector to this vector.
				Add the components of <tt>vector</tt> to this vector.
				@param vector the vector to add
				@return TVector3&, {\em *this}
		*/
		TVector3& operator += (const TVector3& vector);

		/**	Subtract a vector from this vector.
				@param vector the vector to subtract
				@return TVector3&, {\em *this}
		*/
		TVector3& operator -= (const TVector3& vector);

		/**	Scalar product.
				Return <tt>TVector3(x * scalar, y * scalar, z * scalar)</tt>.
				@param scalar, the scalar to multiply by
				@return TVector3 the scalar product of this vector and <tt>scalar</tt>
		*/
		TVector3 operator * (const T& scalar) const;

		/**	Multiply by a scalar.
				Multiply all components of the vector by a <tt>scalar</tt> value.
				@param scalar the to multiply by
				@return TVector3&, {\em *this}
		*/
		TVector3& operator *= (const T& scalar);

		/**	Fraction of a vector.
				Return <tt>TVector3(x / lambda, y / lambda, z / lambda)</tt>.
				@param lambda the scalar value to divide by
				@return TVector3& 
				@exception Exception::DivisionByZero if <tt>lambda == (T)0</tt>
		*/
		TVector3 operator / (const T& lambda) const;

		/**	Divide a vector by a scalar.
				@param lambda the scalar value to divide by
				@return TVector3&, {\em *this}
				@exception Exception::DivisionByZero if <tt>lambda == (T)0</tt>
		*/
		TVector3& operator /= (const T& lambda);

		/** Dot product.
				Return the dot product of this vector and <tt>vector</tt>.
		*/
		T operator * (const TVector3& vector) const;

		/** Cross product.
				Return the cross product of this vector and <tt>vector</tt>.
		*/
		TVector3 operator % (const TVector3& vector) const;

		/**	Assign to the cross product.
				Assign the vector to its cross product with another vector.
		*/
		TVector3& operator %= (const TVector3& vector);

		//@}

		/**	@name	Geometric properties
		*/
		//@{

		/**	Return the distance to another vector.
		*/
		T getDistance(const TVector3& vector) const;

		/**	Return the squared distance to another vector.
		*/
		T getSquareDistance(const TVector3& vector) const;

		/**	Return the enclosed angle of two vectors.
				@exception Exception::DivisionByZero if the product of the squared
								lengths of the two vectors equals <tt>(T)0</tt>
		*/
		TAngle<T> getAngle(const TVector3& vector) const;

		/**	Return the orthogonal projection of this vector onto another.
				@param direction the vector to project onto
		*/
		TVector3 getOrthogonalProjection(const TVector3& direction) const;

		/**	Return the perpendicular normalization of the vector
				@param a 1st vector
				@param b 2nd vector
				@param c 3rd vector
				@return TVector3 the perpendicular normalization
		*/
		static TVector3 getPerpendicularNormalization
			(const TVector3& a, const TVector3& b, const TVector3& c);

		/**	Triple product of three vectors.
				Calculate the parallelepipedal product of three vectors.
				@param a first vector
				@param b second vector
				@param v third vector
				@return T the triple product
		*/
		static T getTripleProduct (const TVector3<T>& a, const TVector3<T>& b, const TVector3<T>& c);

		//@}
	
		/**	@name	Predicates
		*/
		//@{

		/**	Equality operator.
				The function Maths::isEqual is used to compare the values. 
				 \link Maths::isEqual Maths::isEqual \endlink 
				@return bool, <b>true</b> if all three vector components are equal, <b>false</b> otherwise
		*/
		bool operator == (const TVector3& vector) const;
	
		/**	Inequality operator.
				The function Maths::isEqual is used to compare the values. 
				 \link Maths::isEqual Maths::isEqual \endlink 
				@return bool, <b>true</b> if the two vectors differ in at least one component, <b>false</b> otherwise
		*/
		bool operator != (const TVector3& vector) const;

		/// Needed for MSVC
		bool operator < (const TVector3& vector) const;


		/**	Zero predicate.
				The function Maths::isZero is used to compare the values with zero. 
				 \link Maths::isZero Maths::isZero \endlink 
		*/
		bool isZero() const;

		/**	Orthogonality predicate.
		*/
		bool isOrthogonalTo(const TVector3& vector) const;

		//@}


    /** @name Storable Interface
    */
    //@{

    /** Persistent stream writing.
    */
    void write(PersistenceManager& pm) const;

    /** Persistent stream reading.
    */
		bool read(PersistenceManager& pm);

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
				For easier access, the three components of the vector
				are public members.
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
		//@}

		private:

		TAngle<T> getAngle_(const T& a, const T& b) const
		{
			TAngle<T> angle;
	
			if (Maths::isNotZero(a))
			{
				angle = atan(b / a);
			} 
			else	
			{
				angle = BALL_SGN(b) * Constants::PI / 2;
			}

			if (Maths::isLess(a, 0)) 
			{
				angle += Constants::PI;
			}

			if (Maths::isLess(angle.value, 0)) 
			{
				return (Angle)(angle.value += 2.0 * Constants::PI);
			} 
			else 
			{
				return angle;
			}
		}
	};
	//@}

	template <typename T>
	BALL_INLINE
	TVector3<T>::TVector3()
		:	x(0),
			y(0),
			z(0)
	{
	}

	template <typename T>
	BALL_INLINE
	TVector3<T>::TVector3(const T* ptr)
	{
		if (ptr == 0) 
		{
			throw Exception::NullPointer(__FILE__, __LINE__);
		}

		x = *ptr++;
		y = *ptr++;
		z = *ptr;
	}

	template <typename T>
	BALL_INLINE
	TVector3<T>::TVector3(const T& value)
		:	x(value),
			y(value),
			z(value)
	{
	}

	template <typename T> template <typename T2>
	BALL_INLINE
	TVector3<T>::TVector3(const TVector3<T2>& vec)
		: x((T)vec.x),
		  y((T)vec.y),
			z((T)vec.z)
	{
	}

	template <typename T>
	BALL_INLINE
	TVector3<T>::TVector3(const T& vx, const T& vy, const T& vz)
		:	x(vx),
			y(vy),
			z(vz)
	{
	}

	template <typename T>
	BALL_INLINE
	TVector3<T>::TVector3(const TVector3& vector)
		:	x(vector.x),
			y(vector.y),
			z(vector.z)
	{
	}

	template <typename T>
	BALL_INLINE
	TVector3<T>::TVector3(const T& r, const TAngle<T>& phi, const TAngle<T>& theta)
		:	x(r * cos(phi) * sin(theta)),
			y(r * sin(phi) * sin(theta)),
			z(r * cos(theta))
	{
	}

	template <typename T>
	BALL_INLINE
	TVector3<T>::~TVector3()
	{
	}

	template <typename T>
	BALL_INLINE
	void TVector3<T>::clear()
	{
		x = y = z = (T)0;
	}

	template <typename T>
	BALL_INLINE 
	void TVector3<T>::set(const T* ptr)
	{
		if (ptr == 0)	
			throw Exception::NullPointer(__FILE__, __LINE__);
		
		x = *ptr++;
		y = *ptr++;
		z = *ptr;
	}

	template <typename T>
	BALL_INLINE 
	void TVector3<T>::set(const T& value)
	{
		x = value;
		y = value;
		z = value;
	}

	template <typename T>
	BALL_INLINE 
	void TVector3<T>::set(const T& vx, const T& vy, const T& vz)
	{
		x = vx;
		y = vy;
		z = vz;
	}

	template <typename T>
	BALL_INLINE 
	void TVector3<T>::set(const TVector3<T>& vector)
	{
		x = vector.x;
		y = vector.y;
		z = vector.z;
	}

	template <typename T>
	BALL_INLINE
	void TVector3<T>::set(const T& r, const TAngle<T> &phi, const TAngle<T> &theta)
	{
		x = r * cos(phi) * sin(theta);
		y = r * sin(phi) * sin(theta);
		z = r * cos(theta);
	}

	template <typename T>
	BALL_INLINE
	TVector3<T>& TVector3<T>::operator = (const T* ptr)
	{
		if (ptr == 0)
		{
			throw Exception::NullPointer(__FILE__, __LINE__);
		}

		x = *ptr++;
		y = *ptr++;
		z = *ptr;

		return *this;
	}

	template <typename T>
	BALL_INLINE 
	TVector3<T>& TVector3<T>::operator = (const TVector3<T>& vector)
	{
		x = vector.x;
		y = vector.y;
		z = vector.z;

		return *this;
	}

	template <typename T>
	BALL_INLINE 
	TVector3<T>& TVector3<T>::operator = (T value)
	{
		x = y = z = value;

		return *this;
	}

	template <typename T>
	BALL_INLINE 
	void TVector3<T>::get(T* ptr) const
	{
		if (ptr == 0)
		{
			throw Exception::NullPointer(__FILE__, __LINE__);
		}

		*ptr++ = x;
		*ptr++ = y;
		*ptr   = z;
	}

	template <typename T>
	BALL_INLINE 
	void TVector3<T>::get(T& new_x, T& new_y, T& new_z) const
	{
		new_x = x;
		new_y = y;
		new_z = z;
	}

	template <typename T>
	BALL_INLINE 
	void TVector3<T>::get(TVector3<T>& vector) const
	{
		vector.x = x;
		vector.y = y;
		vector.z = z;
	}

	template <typename T>
	BALL_INLINE 
	void TVector3<T>::get(T& r, TAngle<T>& phi, TAngle<T>& theta) const
	{
		r 		= sqrt(x * x + y * y + z * z);
		phi 	= (Angle)getAngle_(x, y);
		theta = getAngle_(z, sqrt(x * x + y * y));
	}

	template <typename T>
	BALL_INLINE
	void TVector3<T>::swap(TVector3<T>& vector)
	{
		T temp = x;
		x = vector.x;
		vector.x = temp;

		temp = y;
		y = vector.y;
		vector.y = temp;

		temp = z;
		z = vector.z;
		vector.z = temp;
	}

	template <typename T>
	BALL_INLINE 
	T TVector3<T>::getLength() const
	{
		return (T)sqrt(x * x + y * y + z * z);
	}

	template <typename T>
	BALL_INLINE 
	T TVector3<T>::getSquareLength() const
	{
		return (x * x + y * y + z * z);
	}

	template <typename T>
	TVector3<T>& TVector3<T>::normalize()
	{
		T len = sqrt(x * x + y * y + z * z);

		if (Maths::isZero(len)) 
		{
			throw Exception::DivisionByZero(__FILE__, __LINE__);
		}
		
		x /= len;
		y /= len;
		z /= len;

		return *this;
	}

	template <typename T>
	BALL_INLINE
	TVector3<T>& TVector3<T>::negate()
	{
		x *= -1;
		y *= -1;
		z *= -1;
		return *this;
	}

	template <typename T>
	BALL_INLINE 
	const TVector3<T>& TVector3<T>::getZero()
	{
		static TVector3<T> null_vector(0, 0, 0);
		return null_vector;
	}

	template <typename T>
	BALL_INLINE 
	const TVector3<T>& TVector3<T>::getUnit()
	{
		static TVector3<T> unit_vector(1, 1, 1);
		return unit_vector;
	}

	template <typename T>
	BALL_INLINE 
	T& TVector3<T>::operator [] (Position position)
	{
		if (position > 2)
		{
			throw Exception::IndexOverflow(__FILE__, __LINE__, position);
		}
		switch (position) 
		{
			case 0: return x;
			case 1: return y;
			case 2:
			default:
				return z;
		}
	}

	template <typename T>
	BALL_INLINE 
	const T& TVector3<T>::operator [] (Position position) const
	{
		if (position > 2)
		{
			throw Exception::IndexOverflow(__FILE__, __LINE__);
		}
		switch (position) 
		{
			case 0: return x;
			case 1: return y;
			case 2:
			default:
				return z;
		}
	}

	template <typename T>
	BALL_INLINE
	const TVector3<T>& TVector3<T>::operator + () const	
	{
		return *this;
	}

	template <typename T>
	BALL_INLINE
	TVector3<T> TVector3<T>::operator - () const	
	{
		return TVector3<T>(-x, -y, -z);
	}

	template <typename T>
	BALL_INLINE 
	TVector3<T>& TVector3<T>::operator += (const TVector3<T>& vector)
	{
		x += vector.x;
		y += vector.y;
		z += vector.z;

		return *this;
	}

	template <typename T>
	BALL_INLINE 
	TVector3<T>& TVector3<T>::operator -= (const TVector3<T>& vector)
	{
		x -= vector.x;
		y -= vector.y;
		z -= vector.z;

		return *this;
	}

	template <typename T>
	BALL_INLINE 
	TVector3<T> TVector3<T>::operator * (const T& scalar) const 
	{
		return TVector3<T>(x * scalar, y * scalar, z * scalar);
	}

	template <typename T>
	BALL_INLINE 
	TVector3<T>& TVector3<T>::operator *= (const T &scalar)
	{
		x *= scalar;
		y *= scalar;
		z *= scalar;

		return *this;
	}

	template <typename T>
	TVector3<T> TVector3<T>::operator / (const T& lambda) const
	{
		if (lambda == (T)0)
		{
			throw Exception::DivisionByZero(__FILE__, __LINE__);
		}
		return TVector3<T>(x / lambda, y / lambda, z / lambda);
	}

	template <typename T>
	TVector3<T>& TVector3<T>::operator /= (const T& lambda)
	{
		if (lambda == (T)0)
		{
			throw Exception::DivisionByZero(__FILE__, __LINE__);		
		}
		x /= lambda;
		y /= lambda;
		z /= lambda;

		return *this;
	}

	template <typename T>
	BALL_INLINE 
	T TVector3<T>::operator * (const TVector3<T>& vector) const
	{
		return (x * vector.x + y * vector.y + z * vector.z);
	}

	template <typename T>
	TVector3<T> TVector3<T>::operator % (const TVector3<T>& v) const
	{
		return TVector3(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
	}

	template <typename T>
	BALL_INLINE 
	TVector3<T>& TVector3<T>::operator %= (const TVector3<T>& v)
	{
		set(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
		return *this;
	}

	template <typename T>
	BALL_INLINE 
	T TVector3<T>::getDistance(const TVector3<T>& v) const
	{
		T dx = x - v.x;
		T dy = y - v.y;
		T dz = z - v.z;
		
		return (T)sqrt(dx * dx + dy * dy + dz * dz); 
	}

	template <typename T>
	BALL_INLINE T
	TVector3<T>::getSquareDistance(const TVector3<T>& v) const
	{
		T dx = x - v.x;
		T dy = y - v.y;
		T dz = z - v.z;
		
		return (dx * dx + dy * dy + dz * dz); 
	}

	template <typename T>
	BALL_INLINE 
	TAngle<T> TVector3<T>::getAngle(const TVector3<T>& vector) const
	{
		T length_product = getSquareLength() * vector.getSquareLength();

		if (length_product == (T)0)	
		{
			throw Exception::DivisionByZero(__FILE__, __LINE__);
		}
		
		T acos_arg = ((*this) * vector) / sqrt(length_product);

		// ensure that the argument of acos is in the correct range
		// (might happen if the angle between the two vectors is
		// very close to zero)
		if (fabs(acos_arg) > 1.0)
		{	
			return (TAngle<T>)0.0;
		}
		
		return (TAngle<T>)acos(acos_arg);
	}

	template <typename T>
	BALL_INLINE 
	TVector3<T> TVector3<T>::getOrthogonalProjection(const TVector3<T>& direction) const
	{
		return ((direction * (*this)) / (direction * direction) * direction);
	}

	template <typename T>
	TVector3<T> TVector3<T>::getPerpendicularNormalization
		(const TVector3<T> &a, const TVector3<T> &b, const TVector3<T> &c)
	{
		TVector3 diff1(b.x - a.x, b.y - a.y, b.z - a.z);
		TVector3 diff2(b.x - c.x, b.y - c.y, b.z - c.z);

		return TVector3
			(diff1.y * diff2.z - diff1.z * diff2.y,
			 diff1.z * diff2.x - diff1.x * diff2.z,
			 diff1.x * diff2.y - diff1.y * diff2.x);
	}

	template <typename T>
	BALL_INLINE
	T TVector3<T>::getTripleProduct
		(const TVector3<T>& a,
		 const TVector3<T>& b,
		 const TVector3<T>& c)
	{
		return (  a.x * (b.y * c.z - b.z * c.y)
						+ a.y * (b.z * c.x - b.x * c.z)
						+ a.z * (b.x * c.y - b.y * c.x));
	}

	template <typename T>
	BALL_INLINE 
	bool TVector3<T>::operator == (const TVector3<T>& v) const
	{
		return (Maths::isEqual(x, v.x) && Maths::isEqual(y, v.y) && Maths::isEqual(z, v.z));
	}

	template <typename T>
	BALL_INLINE 
	bool TVector3<T>::operator < (const TVector3<T>& v) const
	{
		return (x < v.x || y < v.y || z < v.z);
	}


	template <typename T>
	BALL_INLINE 
	bool TVector3<T>::operator != (const TVector3<T>& v) const
	{
		return (Maths::isNotEqual(x, v.x) || Maths::isNotEqual(y, v.y) || Maths::isNotEqual(z, v.z));
	}

	template <typename T>
	BALL_INLINE 
	bool TVector3<T>::isOrthogonalTo(const TVector3<T>& v) const
	{
		return Maths::isZero((*this) * v);
	}

	template <typename T>
	BALL_INLINE 
	bool TVector3<T>::isValid() const
	{
		return true;
	}

	template <typename T>
	BALL_INLINE 
	bool TVector3<T>::isZero() const
	{
		return (Maths::isZero(x) && Maths::isZero(y) && Maths::isZero(z));
	}

	template <typename T>
	void TVector3<T>::dump(std::ostream& s, Size depth) const
	{
		BALL_DUMP_STREAM_PREFIX(s);

		BALL_DUMP_HEADER(s, this, this);

		BALL_DUMP_DEPTH(s, depth);
		s << "  (x =  " << x << ", y = " << y << ", z = " << z << ")" << std::endl;

		BALL_DUMP_STREAM_SUFFIX(s);
	}

	/**	Default three-dimensional vector class.
			This is the class used in BALL kernel to represent points, coordinates.
	*/
	typedef TVector3<float> Vector3;

	template <typename T>
	BALL_INLINE 
	TVector3<T> TVector3<T>::operator + (const TVector3<T>& b) const
	{
		return TVector3<T>(x + b.x, y + b.y, z + b.z);
	}
	
	template <typename T>
	BALL_INLINE
	TVector3<T> TVector3<T>::operator - (const TVector3<T>& b) const
	{
		return TVector3<T>(x - b.x, y - b.y, z - b.z);
	}

	template <typename T>
  void TVector3<T>::write(PersistenceManager& pm) const
  {
    pm.writePrimitive(x, "x");
    pm.writePrimitive(y, "y");
    pm.writePrimitive(z, "z");
	}

	template <typename T>			
  bool TVector3<T>::read(PersistenceManager& pm)
  {
    pm.readPrimitive(x, "x");
    pm.readPrimitive(y, "y");
    pm.readPrimitive(z, "z");

    return true;
	}


	template <typename T>
	BALL_INLINE 
	TVector3<T> operator * (const T& scalar, const TVector3<T>& vector)
	{
		return TVector3<T>(scalar * vector.x, scalar * vector.y, scalar * vector.z);
	}

	template <typename T>
	std::istream& operator >> (std::istream& s, TVector3<T>& v)
	{
		char c;
		s >> c >> v.x >> v.y >> v.z >> c;

		return s;
	}

	template <typename T>
	std::ostream& operator << (std::ostream& s, const TVector3<T>& v)
	{
		s << "(" << v.x << ' ' << v.y << ' ' << v.z << ')';

		return s;
	}
// required for visual studio
#ifdef BALL_COMPILER_MSVC
#include <vector>
#ifdef BALL_HAS_EXTERN_TEMPLATES
extern template class BALL_EXPORT std::vector<Vector3>;
#elif
template class BALL_EXPORT std::vector<Vector3>;
#endif
#endif

#ifdef BALL_HAS_EXTERN_TEMPLATES
extern template class BALL_EXPORT TVector3<float>;
#endif

}// namespace BALL

#endif // BALL_MATHS_VECTOR3_H

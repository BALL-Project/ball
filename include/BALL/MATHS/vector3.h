// $Id: vector3.h,v 1.19 2000/02/21 18:09:03 amoll Exp $


#ifndef BALL_MATHS_VECTOR3_H
#define BALL_MATHS_VECTOR3_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#include <math.h>

#ifndef BALL_CONCEPT_PROCESSOR_H
#	include <BALL/CONCEPT/processor.h>
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


namespace BALL 
{
	/**	@name	Three-dimensional vector.
			@memo representation of points and vectors in three-dimensional space: class \Ref{TVector3} and class \Ref{Vector3}
	*/
	//@{

	template <typename T>
	class TVector3;

	template <typename T>
	BALL_INLINE 
	TVector3<T> operator + (const TVector3<T>& a, const TVector3<T>& b);

	template <typename T>
	BALL_INLINE 
	TVector3<T> operator - (const TVector3<T>& a, const TVector3<T>& b);

	template <typename T>
	std::istream& operator >> (std::istream& s, TVector3<T>& vector);

	template <typename T>
	std::ostream& operator << (std::ostream& s, const TVector3<T>& vector);

	/** Generic Three-Dimensional Vector.
      {\bf Definition:} \URL{BALL/MATHS/vector3.h}
      \\
	*/
	template <typename T>
	class TVector3
		: public PersistentObject
	{
		public:

		BALL_CREATE(TVector3<T>)

		/**	@name	Constructors and Destructors
		*/
		//@{

		/**	Default constructor.
				This method creates a new TVector3 object. The three components
				are initialized to {\tt (T)0}.
		*/
		TVector3()
			:	PersistentObject(),
				x(0),
				y(0),
				z(0)
		{
		}

		/**	Array constructor.
				This constructor creates a TVector3 object from the first
				three elements pointed to by {\tt ptr}.
				@param ptr the array to construct from
				@exception NullPointer if {\tt ptr == 0}
		*/
		TVector3(const T* ptr)
			:	PersistentObject()
		{
			if (ptr == 0) 
				throw Exception::NullPointer(__FILE__, __LINE__);
			
			x = *ptr++;
			y = *ptr++;
			z = *ptr;
		}

		/**	Scalar constructor.
				Create a new vector with all components set
				to the same {\tt value}.
				@param	value the value of all components
		*/
		TVector3(const T& value)
			:	PersistentObject(),	
				x(value),
				y(value),
				z(value)
		{
		}

		/**	Detailled constructor.
				Create a new TVector3 object from three {\tt T} variables.
				@param	vx assigned to {\tt x}
				@param	vy assigned to {\tt y}
				@param	vz assigned to {\tt z}
		*/
		TVector3(const T& vx, const T& vy, const T& vz)
			:	PersistentObject(),
				x(vx),
				y(vy),
				z(vz)
		{
		}

		/**	Copy constructor.
				Create a new TVector3 object from another.
				@param vector the TVector3 object to be copied
				@param bool ignored - just for interface consistency
		*/	
		TVector3(const TVector3& vector, bool deep = true)
			:	PersistentObject(),
				x(vector.x),
				y(vector.y),
				z(vector.z)
		{
		}

		/**	Spherical polar coordinate constructor.
				Create a TVector3 object and set its coordinates to 
				the point described by the three spherical polar coordinates
				{\tt r} (radius), {\tt phi} (azimuth), and {\tt theta} (co-latitude).
				@see set
				@param r the radius
				@param phi the azimuth 
				@param theta the co-latitude
		*/
		TVector3(const T& r, const TAngle<T>& phi, const TAngle<T>& theta)
			:	PersistentObject(),
				x(r * cos(phi) * sin(theta)),
				y(r * sin(phi) * sin(theta)),
				z(r * cos(theta))
		{
		}

		/**	Destructor.	
				Destructs the TVector3 object. As there are no dynamic
				data structures, nothing happens.
		*/	
		virtual ~TVector3()
		{
		}

		//@}

		/**	@name Persistence 
		*/
		//@{

		/**	Persistent writing.
				Writes a TVector3 object to a persistent stream.
				@param pm the persistence manager
		*/
		virtual void persistentWrite(PersistenceManager& pm, const char* name = 0) const;

		/**	Persistent reading.
				Reads a TVector3 object from a persistent stream.
				@param pm the persistence manager
		*/
		virtual void persistentRead(PersistenceManager& pm);

		//@}

		/**	@name	Assignment 
		*/
		//@{

		/**	Assign from an array.
				Assign the three components {\tt x}, {\tt y}, and {\tt z} from
				the first three elements of the array pointed to by {\tt ptr}.
				@param ptr an array
				@exception Nullpointer if {\tt ptr == 0}
		*/
		void set(const T* ptr);

		/**	Assign from a scalar.
				Assign {\tt value} to the three vector components.
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
				@param deep ignored
		*/
		void set(const TVector3& vector, bool deep = true);

		/**	Set from spherical polar coordinates.
				The radius describes the distance of the point from the origin, {\tt phi}
				ranges from 0 to $2 \pi$, {\tt theta} ranges from 0 (north pole, positive z-axis)
				to $\pi$ (south pole, negative z-axis).\\
				Coordinates are calculated according to the following formulas:
				\begin{eqnarray*}
					x & = & r \sin \theta \cos \phi\\
					y & = & r \sin \theta \sin \phi\\
					z & = & r \cos \theta
				\end{eqnarray*}
					
				@param	r the radius
				@param	phi the azimuth
				@param	theta	the co-latitude
		*/
		void set(const T& r, const TAngle<T>& phi, const TAngle<T> &theta);

		/**	Assignment operator.
				Assign the vector components from another vector.
				@param v the vector to assign from
		**/
		TVector3& operator = (const TVector3& v);

		/**	Array assignment operator.
				Assigns the first three elements of an array to the vector components.
				@param	ptr the array
				@exception	NullPointer if {\tt ptr == 0}
		*/
		TVector3& operator = (const T* ptr);

		/**	Assign to an array.
				Sets the first three array elements pointed to by {\tt ptr} 
				to the values of the three vector components.
				@param ptr the array
				@exception	NullPointer if {\tt ptr == 0}
		*/
		void get(T* ptr) const;

		/**	Assign to three variables of type {\tt T}.
				@param	x the x component
				@param	y the y component
				@param	z the z component
		*/
		void get(T& x, T& y, T& z) const;

		/**	Assign to another Vector3.
				Assigns the vector components to another vector.
				@param vector	the vector to be asigned to
				@param deep ignored
		*/
		void get(TVector3& vector, bool deep = true) const;

		/**	Assign polar coordinates.
				Sets {\tt r}, {\tt phi}, and {\tt theta} to the
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
				$\sqrt{x^2 + y^2 + z^2}$.
				@return T, the vector length
		*/	
		T getLength() const;

		/**	Return the squared length of the vector.
				This method avoids the square root needed in getLength,
				so this method is preferred if possible.
				@return T, $x^2 + y^2 + z^2$
		*/
		T getSquareLength() const;

		/**	Normalize the vector.
				The vector is scaled with its length:
				$\{x|y|z\} *= \sqrt{x^2 + y^2 + z^2}$.
				@return T, a reference to {\tt this} vector
				@exception DivisionByZero if the length of the vector is 0
		*/
		TVector3& normalize();

		/**	Return a vector with all components 0.
		*/
		static const TVector3& getZero();

		/**	Return a vector with all components 1.
		*/
		static const TVector3& getUnit();

		/**	Mutable array-like access to the components.
				@exception Exception::IndexUnderflow if {\tt index < 0}
				@exception Exception::IndexOverflow if {\tt index > 2}
		*/
		T& operator [] (Index index);

		/**	Constant array-like access to the components.
				@exception Exception::IndexUnderflow if {\tt index < 0}
				@exception Exception::IndexOverflow if {\tt index > 2}
		*/
		const T& operator [] (Index index) const;
		//@}
		
		/**	@name	Arithmetic operators
		*/
		//@{

		/**	Positive sign.
		*/
		TVector3 operator + () const;

		/**	Negative sign.
		*/
		TVector3 operator - () const;

		/**	Add a vector to this vector.
				Add the components of {\tt vector} to this vector.
				@param vector the vector to add
				@return TVector3&, {\tt *this}
		*/
		TVector3& operator += (const TVector3& vector);

		/**	Subtract a vector from this vector.
				Subtract {\tt vector} from this vector componentwise.
				@param vector the vector to subtract
				@return TVector3&, {\tt *this}
		*/
		TVector3& operator -= (const TVector3& vector);

		/**	Scalar product.
				Return {\tt TVector3(x * scalar, y * scalar, z * scalar)}.
				@param scalar, the scalar to multiply with
				@return TVector3, the scalar product of this vector and {\tt scalar}
		*/
		TVector3 operator * (const T& scalar);

		/**	Multiply with a scalar.
				Multiply all components of the vector with a {\tt scalar}.
				@param scalar the scalar to multiply with
				@return TVector3&, {\tt *this}
		*/
		TVector3& operator *= (const T& scalar);

		/**	Fraction of a vector.
				Return {\tt TVector3(x / scalar, y / scalar, z / scalar)}.
				@param lambda the scalar to divide by
				@return TVector3& 
				@exception Exception::DivisionByZero if {\tt lambda == (T)0}
		*/
		TVector3 operator / (const T& lambda);

		/**	Divide a vector by a scalar.
				@param lambda the scalar to divide by
				@return TVector3&, {\tt *this}
				@exception Exception::DivisionByZero if {\tt lambda == (T)0}
		*/
		TVector3& operator /= (const T& lambda);

		/** Dot product.
				Return the dot product of this vector with {\tt vector}.
		*/
		T operator * (const TVector3& vector) const;

		/** Cross product.
				Return the cross product of this vector with {\tt vector}.
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
		T getDistance(const TVector3& vector3) const;

		/**	Return the squared distance to another vector.
		*/
		T getSquareDistance(const TVector3& vector) const;

		/**	Return the enclose angle between two vectors.
				@exception Exception::DivisionByZero if the product of the squared
								lengths of the two vectors equals {\tt (T)0}
		*/
		TAngle<T> getAngle(const TVector3& vector) const;

		/**	Return the orthogonal projection of this vector onto another.
				@param direction the vector to project onto
		*/
		TVector3 getOrthogonalProjection(const TVector3& direction) const;

		/**	Return the perpendicular normalization of the vector
				@param TVector3& a 1. vector
				@param TVector3& b 2. vector
				@param TVector3& c 3. vector
				@return static TVector3 the perpendicular normalization
		*/
		static TVector3 getPerpendicularNormalization
			(const TVector3& a, const TVector3& b, const TVector3& c);

		/**	Return the torsion angle of four vectors to eachother.
				@param TVector3& a 1. vector
				@param TVector3& b 2. vector
				@param TVector3& c 3. vector
				@param TVector3& d 4. vector
				@return static TAngle the torsion angle
		*/
		static TAngle<T> getTorsionTAngle
			(const TVector3& a, const TVector3& b,
			 const TVector3& c, const TVector3& d);

		//@}
	
		/**	@name	Predicates
		*/
		//@{

		/**	Equality operator.
				@return bool, {\bf true} if all three vector components are equal, {\bf false} otherwise
		*/
		bool operator == (const TVector3& vector) const;
	
		/**	Inequality operator.
				@return bool, {\bf true} if the two vectors differ in at least on component, {\bf false} otherwise
		*/
		bool operator != (const TVector3& vector) const;

		/**	Zero predicate.
		*/
		bool isZero() const;

		/**	Orthogonality predicate.
		*/
		bool isOrthogonalTo(TVector3& vector) const;

		//@}


		/**	@name	Debugging and Diagnostics
		*/
		//@{
		///
		void dump(std::ostream& s = std::cout, Size depth = 0) const;

		/**	Test if instance is valid.
				always retruns true
				@return bool {\bf true}
		*/
		bool isValid() const;

		//@}


		/**	@name	Vector components
				For easier access the three components of the vector
				are accesible to the public.
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

		TAngle<T> getAngle_(const T& x, const T& y) const
		{
			TAngle<T> angle;
	
			if (Maths::isNotZero(x)){
				angle = atan(y / x);
			} else {
				angle = BALL_SGN(y) * Constants::PI / 2;
			}

			if (Maths::isLess(x, 0)) {
				angle += Constants::PI;
			}

			if (Maths::isLess(angle.value, 0)) {
				return (angle.value += 2 * Constants::PI);
			} else {
				return angle;
			}
		}
	};


	template <typename T>
  void TVector3<T>::persistentWrite(PersistenceManager& pm, const char* name) const
	{
		pm.writeObjectHeader(this, name);
			pm.writePrimitive(x, "x");
			pm.writePrimitive(y, "y");
			pm.writePrimitive(z, "z");
		pm.writeObjectTrailer(name);
	}

	template <typename T>
	void TVector3<T>::persistentRead(PersistenceManager& pm)
	{
		pm.readPrimitive(x, "x");
		pm.readPrimitive(y, "y");
		pm.readPrimitive(z, "z");
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
	void TVector3<T>::set(const TVector3<T>& vector, bool /* deep */)
	{
		x = vector.x;
		y = vector.y;
		z = vector.z;
	}

	template <typename T>
	void TVector3<T>::set(const T& r, const TAngle<T> &phi, const TAngle<T> &theta)
	{
		x = r * cos(phi) * sin(theta);
		y = r * sin(phi) * sin(theta);
		z = r * cos(theta);
	}

	template <typename T>
	TVector3<T>& TVector3<T>::operator = (const T* ptr)
	{
		if (ptr == 0)
			throw Exception::NullPointer(__FILE__, __LINE__);
		
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
	void TVector3<T>::get(T* ptr) const
	{
		if (ptr == 0)
			throw Exception::NullPointer(__FILE__, __LINE__);

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
	void TVector3<T>::get(TVector3<T>& vector, bool /* deep */) const
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
		phi 	= getAngle_(x, y);
		theta = getAngle_(z, sqrt(x * x + y * y));
	}

	template <typename T>
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
	T& TVector3<T>::operator [] (Index index)
	{
		if (index < 0)
			throw Exception::IndexUnderflow(__FILE__, __LINE__);

		if (index > 2)
			throw Exception::IndexOverflow(__FILE__, __LINE__);
	
		switch (index) {
			case 0: return x;
			case 1: return y;
			case 2:
			default:
				return z;
		}
	}

	template <typename T>
	BALL_INLINE 
	const T& TVector3<T>::operator [] (Index index) const
	{
		if (index < 0)
			throw Exception::IndexUnderflow(__FILE__, __LINE__);

		if (index > 2)
			throw Exception::IndexOverflow(__FILE__, __LINE__);
	
		switch (index) {
			case 0: return x;
			case 1: return y;
			case 2:
			default:
				return z;
		}
	}

	template <typename T>
	BALL_INLINE
	TVector3<T> TVector3<T>::operator + () const	
	{
		return TVector3<T>(x, y, z);
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
	TVector3<T> TVector3<T>::operator * (const T& scalar)
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
	TVector3<T> TVector3<T>::operator / (const T& lambda)
	{
		if (lambda == (T)0)
			throw Exception::DivisionByZero(__FILE__, __LINE__);
		
		return TVector3<T>(x / lambda, y / lambda, z / lambda);
	}

	template <typename T>
	TVector3<T>& TVector3<T>::operator /= (const T& lambda)
	{
		if (lambda == (T)0)
			throw Exception::DivisionByZero(__FILE__, __LINE__);		
		
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
			throw Exception::DivisionByZero(__FILE__, __LINE__);
		
		return acos(((*this) * vector) / sqrt(length_product));
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
	TAngle<T> TVector3<T>::getTorsionTAngle
		(const TVector3<T> &a, const TVector3<T> &b, 
		 const TVector3<T> &c, const TVector3<T> &d)
	{
		TVector3<T> ba(b.x - a.x, b.y - a.y, b.z - a.z);
		TVector3<T> bc(b.x - c.x, b.y - c.y, b.z - c.z);
		TVector3<T> cd(c.x - d.x, c.y - d.y, c.z - d.z);
		TVector3<T> cross_ba_bc
			(ba.y * bc.z - ba.z * bc.y,
			 ba.z * bc.x - ba.x * bc.z,
			 ba.x * bc.y - ba.y * bc.x);
		
		TVector3<T> cross_cd_bc
			(cd.y * bc.z - cd.z * bc.y,
			 cd.z * bc.x - cd.x * bc.z,
			 cd.x * bc.y - cd.y * bc.x);
		
		TVector3<T> cross_cross_cd_bc_cross_ba_bc
			(cross_cd_bc.y * cross_ba_bc.z - cross_cd_bc.z * cross_ba_bc.y,
			 cross_cd_bc.z * cross_ba_bc.x - cross_cd_bc.x * cross_ba_bc.z,
			 cross_cd_bc.x * cross_ba_bc.y - cross_cd_bc.y * cross_ba_bc.x);

		if (Maths::isLess(cross_cross_cd_bc_cross_ba_bc * bc, 0))
		{
			return -cross_cd_bc.getAngle(cross_ba_bc);
		} else {
			return cross_cd_bc.getAngle(cross_ba_bc);
		}
	}

	template <typename T>
	BALL_INLINE 
	bool TVector3<T>::operator == (const TVector3<T>& v) const
	{
		return (bool)(Maths::isEqual(x, v.x) && Maths::isEqual(y, v.y) && Maths::isEqual(z, v.z));
	}

	template <typename T>
	BALL_INLINE 
	bool TVector3<T>::operator != (const TVector3<T>& v) const
	{
		return (bool)(Maths::isNotEqual(x, v.x) || Maths::isNotEqual(y, v.y) || Maths::isNotEqual(z, v.z));
	}

	template <typename T>
	BALL_INLINE 
	bool TVector3<T>::isOrthogonalTo(TVector3<T>& v) const
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
		s << "  (x =  " << x << ", y = " << y << ", z = " << z << ")" << endl;

		BALL_DUMP_STREAM_SUFFIX(s);
	}

	/**	Default three-dimensional vector class
	*/
	typedef TVector3<float> Vector3;

	/**	Operators
	*/
	//@{
	/** Addition operator for two vectors
  		@return TVector3 - the new vector
	*/
	template <typename T>
	inline 
	TVector3<T> operator + (const TVector3<T>& a, const TVector3<T>& b)
	{
		return TVector3<T>(a.x + b.x, a.y + b.y, a.z + b.z);
	}

	/** Subtraction operator of two vectors
  		@return TVector3 the new vector
	*/
	template <typename T>
	inline
	TVector3<T> operator - (const TVector3<T>& a, const TVector3<T>& b)
	{
		return TVector3<T>(a.x - b.x, a.y - b.y, a.z - b.z);
	}

	/**	Multiply operator for a scalar with a vector
  		@return TVector3 - the new vector
	*/
	template <typename T>
	BALL_INLINE 
	TVector3<T> operator * (const T& scalar, const TVector3<T>& vector)
	{
		return TVector3<T>(scalar * vector.x, scalar * vector.y, scalar * vector.z);
	}

	/**	Multiply operator for a vector with a scalar
  		@return TVector3 the new vector
	*/
	template <typename T>
	TVector3<T> operator * (const TVector3<T>& vector, const T& scalar)
	{
		return TVector3<T>(scalar * vector.x, scalar * vector.y, scalar * vector.z);
	}

	/**	Input- Operator
			reads in four {\bf T} : x, y, z, h
	*/
	template <typename T>
	std::istream& operator >> (std::istream& s, TVector3<T>& v)
	{
		char c;
		s >> c;
		if (c == '(') 
		{
			s >> v.x >> v.y >> v.z;
			s >> c;
		}

		return s;
	}

	/**	Output- Operator
			gives three {\bf T} out: x, y, z
	*/
	template <typename T>
	std::ostream& operator << (std::ostream& s, const TVector3<T>& v)
	{
		s << '(' << v.x << ' ' << v.y << ' ' << v.z << ')';

		return s;
	}

	//@}
	//@}
		
} // namespace BALL

#endif // BALL_MATHS_VECTOR3_H

// $Id: quaternion.h,v 1.25 2001/01/22 13:22:03 oliver Exp $

#ifndef BALL_MATHS_QUATERNION_H
#define BALL_MATHS_QUATERNION_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#include <iostream>

#ifndef BALL_MATHS_MATRIX44_H
#	include <BALL/MATHS/matrix44.h>
#endif

#ifndef BALL_MATHS_VECTOR3_H
#	include <BALL/MATHS/vector3.h>
#endif

namespace BALL 
{

	/**	@name Quaternion
			@memo representing a rotation in three dimensional space: class \Ref{TQuaternion} and class \Ref{Quaternion}
	*/
	//@{
			

	/**	Generic Quaternion Class.
      {\bf Definition:} \URL{BALL/MATHS/quaternion.h}
      \\
	*/
	template <typename T>
	class TQuaternion
	{
		public:

		BALL_CREATE(TQuaternion<T>)
		

		/**	@name	Constructors and Destructors
		*/
		//@{

		/**	Default constructor.
				This method creates a new TQuaternion object. The axis-components
				are initialized to {\tt 0}, the angle is set to {\tt 1}.
		*/
		TQuaternion()
			throw();

		/**	Copy constructor.
				Create a new TQuaternion object from another.
				@param TQuaternion the TQuaternion object to be copied
		*/	
		TQuaternion(const TQuaternion& q)
			throw();

		/**	Detailed constructor.
				Create a new TQuaternion object from a variable of type {\em TVector3} and an angle.
				@param axis assigned to the axis
				@param angle assigned to the angle
		*/
		TQuaternion(const TVector3<T>& axis, const T &angle)
			throw();

		/**	Detailed constructor.
				Create a new TQuaternion object from three values of type {\bf T} and an angle.
				@param x assigned to the x-component of the axis
				@param y assigned to the y-component of the axis
				@param z assigned to the z-component of the axis
				@param angle assigned to the angle
		*/
		TQuaternion(const T& x, const T& y, const T& z, const T& angle)
			throw();

		/**	Destructor.	
				Destructs the TQuaternion object. As there are no dynamic
				data structures, nothing happens.
		*/
		virtual ~TQuaternion()
			throw();

		/**	Clear method.
				The value are set to 0.
		*/
		virtual void clear() 
			throw();

		//@}

		/**	@name	Assignment
		*/
		//@{
		///
		void set(const TQuaternion& q)
			throw();

		/**	Assign the TQuaternion components.
				@param axis the new axis component
				@param angle the new angle component
		*/
		void set(const TVector3<T>& axis, const T& angle)
			throw();

		/**	Assign the TQuaternion components.
				@param x assigned to the x-component of the axis
				@param y assigned to the y-component of the axis
				@param z assigned to the z-component of the axis
				@param angle assigned to the angle
		*/
		void set(const T& x, const T& y, const T& z, const T& angle)
			throw();

		/**	Assign from another TQuaternion.
				@param q the TQuaternion object to assign from
		*/
		const TQuaternion& operator = (const TQuaternion& q)
			throw();

		/**	Assign to another TQuaternion.
				Assigns the components to another TQuaternion.
				@param q the TQuaternion to be assigned to
		*/
		void get(TQuaternion& q) const
			throw();

		/**	Assign the components to the standard values.
				The axis-components are set to {\tt 0}, the angle is set to {\tt 1}.
		*/
		void setIdentity()
			throw();

		/**	Swap the contents of two TQuaternion.
				@param q the TQuaternion to swap contents with
		*/
		void swap(TQuaternion& q)
			throw();
		//@}


		/**	@name	Accessors
		*/
		//@{
		/**	Get the positive angle rotation.
				@return T the angle value
		*/
		T getAngle() const
			throw();

		/**	Get the normalized direction vector of the rotation axis.
				@return TVector3 the Axis
		*/
		TVector3<T> getAxis()
			throw(Exception::DivisionByZero);

		/**	Get the rotation matrix.
				@param m the matrix to compute from
				@return TMatrix4x4 the rotation matrix
		*/
		TMatrix4x4<T>& getRotationMatrix(TMatrix4x4<T>& m) const
			throw();

		/**	Negative sign.
		*/
		TQuaternion operator - () const
			throw();

		/**	Get the inverse TQuaternion.
				@return TQuaternion the inverse TQuaternion
		*/
		TQuaternion getInverse() const
			throw();

		/**	Return the conjugate TQuaternion.
				(The axis components are negated.)
				@return TQuaternion the conjugate TQuaternion
		*/
		TQuaternion getConjugate() const
			throw();

		/**	Add a TQuaternion to this TQuaternion.
				@param q the TQuaternion to add
				@return TQuaternion& {\em *this}
		*/
		TQuaternion& operator += (const TQuaternion& q)
			throw();

		/**	Substract a TQuaternion from this TQuaternion.
				@param q the TQuaternion to substract
				@return TQuaternion& {\em *this}
		*/
		TQuaternion& operator -= (const TQuaternion& q)
			throw();

		//@}


		/**	@name	Predicates
		*/
		//@{
		/**	Equality operator.
				@return bool, {\bf true} if all components are equal, {\bf false} otherwise
		*/
		bool operator == (const TQuaternion& q) const
			throw();

		/**	Inequality operator.
				@return bool, {\bf true} if the two TQuaternion differ in at least one component, {\bf false} otherwise
		*/
		bool operator != (const TQuaternion& q) const
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
		//@}


		/**	@name	Attributes
		*/
		//@{
		/**	x component of the axis. */
		T i;

		/**	y component of the axis. */
		T j;

		/**	z component of the axis. */
		T k;

		/**	Angle component. */
		T angle;
		//@}

	};

	template <typename T>
	TQuaternion<T>::TQuaternion()
		throw()
		:	i((T)0), 
			j((T)0), 
			k((T)0),
			angle((T)1)
	{
	}

	template <typename T>
	TQuaternion<T>::TQuaternion(const TQuaternion& q)
		throw()
		:	i(q.i),
			j(q.j),
			k(q.k),
			angle(q.angle)
	{
	}

	template <typename T>
	TQuaternion<T>::TQuaternion(const TVector3<T>& axis, const T& angle)
		throw()
	{
		set(axis.x, axis.y, axis.z, angle);
	}

	template <typename T>
	TQuaternion<T>::TQuaternion(const T& x, const T& y, const T& z, const T& new_angle)
		throw()
	{
		set(x, y, z, new_angle);
	}

	template <typename T>
	void TQuaternion<T>::clear()
		throw()
	{
		i = j = k = angle = (T)0;
	}

	template <typename T>
	TQuaternion<T>::~TQuaternion()
		throw()
	{
	}

	template <typename T>
	void TQuaternion<T>::set(const TQuaternion<T>& q)
		throw()
	{
		if (this != &q)
		{
			i = q.i;
			j = q.j;
			k = q.k;
			angle = q.angle;
		}
	}

	template <typename T>
	BALL_INLINE 
	void TQuaternion<T>::set(const TVector3<T>& axis, const T& new_angle)
		throw()
	{
		set(axis.x, axis.y, axis.z, new_angle);
	}

	template <typename T>
	void TQuaternion<T>::set(const T& x, const T& y, const T& z, const T& phi)
		throw()
	{
		T length = (T)sqrt(x * x + y * y + z * z);

		if (Maths::isEqual(length, (T)0))
		{
			i = j = k = (T)0;
			angle = (T)1;
		} 
		else 
		{
			T omega = phi * 0.5;
			T sin_omega = (T)::sin(omega);

			i = x * sin_omega / length;
			j = y * sin_omega / length;
			k = z * sin_omega / length;
			angle = (T)::cos(omega);
		}
	}

	template <typename T>
	BALL_INLINE 
	const TQuaternion<T>& TQuaternion<T>::operator = (const TQuaternion<T>& q)
		throw()
	{
		set(q);
		return *this;
	}

	template <typename T>
	BALL_INLINE 
	void TQuaternion<T>::get(TQuaternion<T>& q) const
		throw()
	{
		q.set(*this);
	}

	template <typename T>
	BALL_INLINE 
	void TQuaternion<T>::setIdentity()
		throw()
	{
		i = j = k = (T)0;
		angle = (T)1;
	}

	template <typename T>
	T TQuaternion<T>::getAngle() const
		throw()
	{
		return (T)(2.0 * atan2(sqrt(i * i + j * j + k * k), angle));
	}

	template <typename T>
	TVector3<T> TQuaternion<T>::getAxis()
		throw(Exception::DivisionByZero)
	{
		TVector3<T> vector(i, j, k);
		T length = vector.getLength();

  	if (length == (T)0)
    {
    	throw Exception::DivisionByZero(__FILE__, __LINE__);
    }		
		vector.x /= length;
		vector.y /= length;
		vector.z /= length;

		return vector;
	}

	template <typename T>
	TMatrix4x4<T>& TQuaternion<T>::getRotationMatrix(TMatrix4x4<T>& m) const
		throw()
	{
		m.set
			((T)(1.0 - 2.0 * (j * j + k * k)), 
			 (T)(2.0 * (i * j - k * angle)), 
			 (T)(2.0 * (k * i + j * angle)), 
			 (T)0, 
			 
			 (T)(2.0 * (i * j + k * angle)), 
			 (T)(1.0 - 2.0 * (k * k + i * i)), 
			 (T)(2.0 * (j * k - i * angle)), 
			 (T)0, 
			 
			 (T)(2.0 * (k * i - j * angle)),
			 (T)(2.0 * (j * k + i * angle)),
			 (T)(1.0 - 2.0 * (j * j + i * i)), 
			 (T)0,
			 
			 (T)0, 
			 (T)0, 
			 (T)0, 
			 (T)1); 
		
		return m;
	}

	template <typename T>
	TQuaternion<T> TQuaternion<T>::operator - () const
		throw()
	{
		T tmp = ::sqrt(angle * angle + i * i + j * j + k * k);

		if (tmp > (T)0)
		{
			tmp = (T)1 / tmp;

			return TQuaternion<T>(-i * tmp, -j * tmp, -k * tmp,	angle * tmp);
		} 
		else 
		{
			return TQuaternion<T>(-i, -j, -k, angle);
		}
	}

	template <typename T>
	BALL_INLINE 
	TQuaternion<T> TQuaternion<T>::getInverse() const
		throw()
	{
		return -TQuaternion(*this);
	}

	template <typename T>
	BALL_INLINE 
	TQuaternion<T> TQuaternion<T>::getConjugate() const
		throw()
	{
		TQuaternion<T> tmp;

		tmp.i = -i;
		tmp.j = -j;
		tmp.k = -k;
		tmp.angle = angle;

		return tmp;
	}

	template <typename T>
	TQuaternion<T>& TQuaternion<T>::operator += (const TQuaternion<T>& q)
		throw()
	{
		T tmp_angle = angle * q.angle - i * q.i - j * q.j - k * q.k;
		T tmp_i     = angle * q.i + i * q.angle + j * q.k - k * q.j;
		T tmp_j     = angle * q.j + j * q.angle + k * q.i - i * q.k;
		T tmp_k     = angle * q.k + k * q.angle + i * q.j - j * q.i;

		T tmp = tmp_i * tmp_i + tmp_j * tmp_j + tmp_k * tmp_k + tmp_angle * tmp_angle;

		if (Maths::isNotEqual(tmp, (T)0))
		{
			tmp = 1 / tmp;

			i = tmp_i * tmp;
			j = tmp_j * tmp;
			k = tmp_k * tmp;
			angle = tmp_angle * tmp;
		} 
		else 
		{
			i = j = k = (T)0;
			angle = (T)1;
		}

		return *this;
	}

	template <typename T>
	BALL_INLINE 
	TQuaternion<T>& TQuaternion<T>::operator -= (const TQuaternion<T>& q)
		throw()
	{
		return (*this += -q);
	}


	/** Addition operator for two Quaternions
			@return TQuaternion - the new Quaternion
	*/
	template <typename T>
	BALL_INLINE 
	TQuaternion<T> operator + (const TQuaternion<T>& a, const TQuaternion<T>& b)
		throw()
	{
		TQuaternion<T> q(a);
		q += b;
		return q;
	}

	/** Substraction operator for two Quaternions
			@return TQuaternion - the new Quaternion
	*/
	template <typename T>
	BALL_INLINE 
	TQuaternion<T> operator - (const TQuaternion<T>& a, const TQuaternion<T>& b)
		throw()
	{
		TQuaternion<T> q(a);
		q += -b;
		return q;
	}

	template <typename T>
	void TQuaternion<T>::swap(TQuaternion<T>& q)
		throw()
	{
		T tmp = q.i;
		q.i = i;
		i = tmp;

		tmp = q.j;
		q.j = j;
		j = tmp;

		tmp = q.k;
		q.k = k;
		k = tmp;

		tmp = q.angle;
		q.angle = angle;
		angle = tmp;
	}

	template <typename T>
	BALL_INLINE 
	bool  TQuaternion<T>::operator == (const TQuaternion<T>& q) const
		throw()
	{
		return (i == q.i && j == q.j && k == q.k && angle == q.angle);
	}

	template <typename T>
	BALL_INLINE 
	bool TQuaternion<T>::operator != (const TQuaternion<T>& q) const
		throw()
	{
		return (i != q.i || j != q.j || k != q.k || angle != q.angle);
	}

	/**	Input Operator.
			Read the values of the quaternion from an input stream.
			@param s	the input stream
			@param q  the quaternion to read 

	*/	
	template <typename T>
	std::istream& operator >>(std::istream& s, TQuaternion<T>& q)
		throw()
	{
		char c;
		s >> c >> q.i >> q.j >> q.k >> q.angle >> c;
		return s;
	}

	/**	Output Operator.
			Write the values of the quaternion to an output stream.
			The values of {\tt i}, {\tt j}, {\tt k}, and {\tt angle} are written to
			an output stream. They are enclosed in brackets.\\
			{\bf Example:}\\
			{\tt (0.32 0.45 0.12 1.0)}
			@param s	the output stream
			@param q  the quaternion to write 

	*/	
	template <typename T>
	std::ostream& operator << (std::ostream& s, const TQuaternion<T>& q)
		throw()
	{
		s << '(' << q.i << ' ' << q.j << ' '
				     << q.k << ' ' << q.angle << ')';

		return s;
	}   

	template <typename T>
	void TQuaternion<T>::dump(std::ostream& s, Size depth) const
		throw()
	{
		BALL_DUMP_STREAM_PREFIX(s);

		BALL_DUMP_HEADER(s, this, this);

		BALL_DUMP_DEPTH(s, depth);
		s << "  angle (real): " << angle << std::endl;

		BALL_DUMP_DEPTH(s, depth);
		s << "    i: " << i << std::endl;

		BALL_DUMP_DEPTH(s, depth);
		s << "    j: " << j << std::endl;

		BALL_DUMP_DEPTH(s, depth);
		s << "    k: " << k << std::endl;

		BALL_DUMP_STREAM_SUFFIX(s);
	}

	/**	The Default TQuaternion Type.
			If double precision is not needed, {\tt TQuaternion<float>} should
			be used. It is predefined as {\tt Quaternion} for convenience.
	*/
	typedef TQuaternion<float> Quaternion;
	//@}

} // namespace BALL

#endif // BALL_MATHS_QUATERNION_H

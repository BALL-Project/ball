// $Id: quaternion.h,v 1.17 2000/04/29 15:28:36 amoll Exp $

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
      {\bf Definition:} \URL{BALL/MATHS/.h}
      \\
	*/
	template <class T>
	class TQuaternion
	{
		public:

		BALL_CREATE_NODEEP(TQuaternion<T>)
		

		/**	@name	Constructors and Destructors
		*/
		//@{

		/**	Default constructor.
				This method creates a new TQuaternion object. The axis-components
				are initialized to {\tt 0}, the angle is set to  {\tt 1}.
		*/
		TQuaternion();

		/**	Copy constructor.
				Create a new TQuaternion object from an other.
				@param TQuaternion the TQuaternion object to be copied
		*/	
		TQuaternion(const TQuaternion& q);

		/**	Detailed constructor.
				Create a new TQuaternion object from a TVector3 and an angle.
				@param axis assigned to the axis
				@param angle assigned to the angle
		*/
		TQuaternion(const TVector3<T>& axis, const T &angle);

		/**	Detailed constructor.
				Create a new TQuaternion object from three {\tt T} values and an angle.
				@param x assigned to the x-component of the axis
				@param y assigned to the y-component of the axis
				@param z assigned to the z-component of the axis
				@param angle assigned to the angle
		*/
		TQuaternion(const T& x, const T& y, const T& z, const T& angle);

		/**	Destructor.	
				Destructs the TQuaternion object. As there are no dynamic
				data structures, nothing happens.
		*/
		virtual ~TQuaternion();
		//@}

		/**	@name	Assignment
		*/
		//@{
		///
		void set(const TQuaternion& q);

		/**	Assign the TQuaternion components.
				@param axis the new axis component
				@param angle the new angle component
		*/
		void set(const TVector3<T>& axis, const T& angle);

		/**	Assign the TQuaternion components.
				@param x assigned to the x-component of the axis
				@param y assigned to the y-component of the axis
				@param z assigned to the z-component of the axis
				@param angle assigned to the angle
		*/
		void set(const T& x, const T& y, const T& z, const T& angle);

		/**	Assign from an other TQuaternion.
				@param q the TQuaternion object to assign from
		*/
		TQuaternion& operator = (const TQuaternion& q);

		/**	Assign to an other TQuaternion.
				Assigns the components to an other TQuaternion.
				@param q the TQuaternion to be assigned to
		*/
		void get(TQuaternion& q) const;

		/**	Assign the components to the standard values.
				The axis-components are set to {\tt 0}, the angle is set to {\tt 1}.
		*/
		void setIdentity();

		/**	Swap the contents of two TQuaternion.
				@param q the TQuaternion to swap contents with
		*/
		void swap(TQuaternion& q);
		//@}


		/**	@name	Accessors
		*/
		//@{
		/**	Get the positive angle rotation.
				@return T the angle value
		*/
		T getAngle() const;

		/**	Get the normalized direction vector of the rotation axis.
				@return TVector3 the Axis
		*/
		TVector3<T> getAxis();

		/**	Get the rotation matrix.
				@return TMatrix4x4 the matrix
		*/
		TMatrix4x4<T>& getRotationMatrix(TMatrix4x4<T>& m) const;

		/**	Negative sign.
		*/
		TQuaternion operator - () const;

		/**	Get the inverse TQuaternion.
				@return TQuaternion the inverse TQuaternion
		*/
		TQuaternion getInverse() const;

		/**	Return the conjugate TQuaternion.
				(The axis components are negated.)
				@return TQuaternion&, the conjugate TQuaternion
		*/
		TQuaternion getConjugate() const;

		/**	Add a TQuaternion to this TQuaternion.
				@param q the TQuaternion to add
				@return TQuaternion&, {\tt *this}
		*/
		TQuaternion& operator += (const TQuaternion& q);

		/**	Substract a TQuaternion from this TQuaternion.
				@param q the TQuaternion to substract
				@return TQuaternion&, {\tt *this}
		*/
		TQuaternion& operator -= (const TQuaternion& q);

		//@}


		/**	@name	Predicates
		*/
		//@{
		/**	Equality operator.
				@return bool, {\bf true} if all components are equal, {\bf false} otherwise
		*/
		bool operator == (const TQuaternion& q) const;

		/**	Inequality operator.
				@return bool, {\bf true} if the two TQuaternion differ in at least one component, {\bf false} otherwise
		*/
		bool operator != (const TQuaternion& q) const;
		//@}


		/**	@name	Debugging and Diagnostics
		*/
		//@{
		///
		void dump(std::ostream& s = std::cout, Size depth = 0) const;
		//@}


		/**	@name	Attributes
		*/
		//@{
		/**	x component of the axis.
		*/
		T i;

		/**	y component of the axis.
		*/
		T j;

		/**	z component of the axis.
		*/
		T k;

		/**	angle component.
		*/
		T angle;
		//@}

	};

	template <class T>
	TQuaternion<T>::TQuaternion()
		:	i((T)0), 
			j((T)0), 
			k((T)0),
			angle((T)1)
	{
	}

	template <class T>
	TQuaternion<T>::TQuaternion(const TQuaternion& q)
		:	i(q.i),
			j(q.j),
			k(q.k),
			angle(q.angle)
	{
	}

	template <class T>
	TQuaternion<T>::TQuaternion(const TVector3<T>& axis, const T& angle)
	{
		set(axis.x, axis.y, axis.z, angle);
	}

	template <class T>
	TQuaternion<T>::TQuaternion(const T& x, const T& y, const T& z, const T& new_angle)
	{
		set(x, y, z, new_angle);
	}

	template <class T>
	TQuaternion<T>::~TQuaternion()
	{
	}

	template <class T>
	void TQuaternion<T>::set(const TQuaternion<T>& q)
	{
		if (this != &q)
		{
			i = q.i;
			j = q.j;
			k = q.k;
			angle = q.angle;
		}
	}

	template <class T>
	BALL_INLINE 
	void TQuaternion<T>::set(const TVector3<T>& axis, const T& new_angle)
	{
		set(axis.x, axis.y, axis.z, new_angle);
	}

	template <class T>
	void TQuaternion<T>::set(const T& x, const T& y, const T& z, const T& phi)
	{
		T length = (T)sqrt(x * x + y * y + z * z);

		if (Maths::isEqual(length, (T)0))
		{
			i = j = k = (T)0;
			angle = (T)1;
		} else {
			T omega = phi * 0.5;
			T sin_omega = (T)::sin(omega);

			i = x * sin_omega / length;
			j = y * sin_omega / length;
			k = z * sin_omega / length;
			angle = (T)::cos(omega);
		}
	}

	template <class T>
	BALL_INLINE 
	TQuaternion<T>& TQuaternion<T>::operator = (const TQuaternion<T>& q)
	{
		set(q);

		return *this;
	}

	template <class T>
	BALL_INLINE 
	void TQuaternion<T>::get(TQuaternion<T>& q) const
	{
		q.set(*this);
	}

	template <class T>
	BALL_INLINE 
	void TQuaternion<T>::setIdentity()
	{
		i = j = k = (T)0;
		angle = (T)1;
	}

	template <class T>
	T TQuaternion<T>::getAngle() const
	{
		return (T)(2.0 * atan2(sqrt(i * i + j * j + k * k), angle));
	}

	template <class T>
	TVector3<T> TQuaternion<T>::getAxis()
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

	template <class T>
	TMatrix4x4<T>& TQuaternion<T>::getRotationMatrix(TMatrix4x4<T>& m) const
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

	template <class T>
	TQuaternion<T> TQuaternion<T>::operator - () const
	{
		T tmp = ::sqrt(angle * angle + i * i + j * j + k * k);

		if (tmp > (T)0)
		{
			tmp = (T)1 / tmp;

			return TQuaternion<T>(-i * tmp, -j * tmp, -k * tmp,	angle * tmp);
		} else {
			return TQuaternion<T>(-i, -j, -k, angle);
		}
	}

	template <class T>
	BALL_INLINE 
	TQuaternion<T> TQuaternion<T>::getInverse() const
	{
		return -TQuaternion(*this);
	}

	template <class T>
	BALL_INLINE 
	TQuaternion<T> TQuaternion<T>::getConjugate() const
	{
		TQuaternion<T> tmp;

		tmp.i = -i;
		tmp.j = -j;
		tmp.k = -k;
		tmp.angle = angle;

		return tmp;
	}

	template <class T>
	TQuaternion<T>& TQuaternion<T>::operator += (const TQuaternion<T>& q)
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
		} else {
			i = j = k = (T)0;
			angle = (T)1;
		}

		return *this;
	}


	template <class T>
	BALL_INLINE 
	TQuaternion<T>& TQuaternion<T>::operator -= (const TQuaternion<T>& q)
	{
		return (*this += -q);
	}


	/** Addition operator for two Quaternions
			@return TQuaternion - the new Quaternion
	*/
	template <class T>
	BALL_INLINE 
	TQuaternion<T> operator + (const TQuaternion<T>& a, const TQuaternion<T>& b)
	{
		TQuaternion<T> q(a);

		q += b;

		return q;
	}

	/** Substraction operator for two Quaternions
			@return TQuaternion - the new Quaternion
	*/
	template <class T>
	BALL_INLINE 
	TQuaternion<T> operator - (const TQuaternion<T>& a, const TQuaternion<T>& b)
	{
		TQuaternion<T> q(a);

		q += -b;

		return q;
	}

	template <class T>
	void TQuaternion<T>::swap(TQuaternion<T>& q)
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

	template <class T>
	BALL_INLINE 
	bool  TQuaternion<T>::operator == (const TQuaternion<T>& q) const
	{
		return (bool)(i == q.i && j == q.j && k == q.k && angle == q.angle);
	}

	template <class T>
	BALL_INLINE 
	bool TQuaternion<T>::operator != (const TQuaternion<T>& q) const
	{
		return (bool)(i != q.i || j != q.j || k != q.k || angle != q.angle);
	}

	template <class T>
	std::istream& operator >>(std::istream& s, TQuaternion<T>& q)
	{
		char c;
		s >> c >> q.i >> q.j >> q.k >> q.angle >> c;

		return s;
	}

	/**	Output Operator.
			Write the values of the quaternion to an output stream.
			The values of {\tt i}, {\tt j}, {\tt k}, and {\tt angle} are written to
			an output stream. They are enclose in brackets.\\
			{\bf Example:}\\
			{\tt (0.32 0.45 0.12 1.0)}
	*/	
	template <class T>
	std::ostream& operator << (std::ostream& s, const TQuaternion<T>& q)
	{
		s << '(' << q.i << ' ' << q.j << ' '
				 << q.k << ' ' << q.angle << ')';

		return s;
	}   

	template <class T>
	void TQuaternion<T>::dump(std::ostream& s, Size depth) const
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

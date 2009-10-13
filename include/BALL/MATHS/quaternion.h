// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:

#ifndef BALL_MATHS_QUATERNION_H
#define BALL_MATHS_QUATERNION_H

#ifndef BALL_MATHS_MATRIX44_H
#	include <BALL/MATHS/matrix44.h>
#endif

#ifndef BALL_MATHS_VECTOR3_H
#	include <BALL/MATHS/vector3.h>
#endif

#include <boost/math/quaternion.hpp>
#include <iostream>

namespace BALL
{
	/** \defgroup Quaternions Quaternion
		\ingroup Primitives
	 */
	//@{

	/**	Generic Quaternion Class.

			Representing a rotation in three dimensional space.
	*/
	template <typename T>
	class TQuaternion
		: public boost::math::quaternion<T> //{/*...*/};
	{
		public:

		BALL_CREATE(TQuaternion<T>)

		/**	@name	Constructors and Destructors
		*/
		//@{

		/**	Default constructor.
				This method creates a new TQuaternion object.
				The quaternion is set to an identity quaternion.
		*/
		TQuaternion();

		/**	Copy constructor.
				Create a new TQuaternion object from another.
				@param TQuaternion the TQuaternion object to be copied
		*/
		TQuaternion(const TQuaternion& q);

		/**	Detailed constructor.
				Create a new TQuaternion object from a boost::math::quaternion.
				@param boost::math::quaternion<T>
		*/
		TQuaternion(const boost::math::quaternion<T>& q);

		/**	Detailed constructor.
				Create a new TQuaternion object from four values of type <b>T</b>.
				@param w assigned to the angular component w
				@param i assigned to the vector component i
				@param j assigned to the vector component j
				@param k assigned to the vector component k
		*/
		TQuaternion(const T& w, const T& i, const T& j, const T& k);

		/**	Detailed constructor.
				Create a new TQuaternion object from a variable of type <b>  TVector3 </b> and an angle.
				@param axis assigned to the axis
				@param angle assigned to the angle
		*/
		TQuaternion(const TVector3<T>& axis, const T& angle);

		/**	Destructor.
				Destructs the TQuaternion object. As there are no dynamic
				data structures, nothing happens.
		*/
		~TQuaternion();

		/**	Clear method.
				The values are set to the identity quaternion.
		*/
		void clear();

		//@}
		/**	@name	Assignment
		*/
		//@{

		///
		void set(const TQuaternion& q);

		///
		void set(const boost::math::quaternion<T>& q);

		/**	Assign the TQuaternion components.
				@param axis the new axis component
				@param angle the new angle component
				@deprecated use TQuaternion::fromAxisAngle() instead
		*/
		BALL_DEPRECATED
		void set(const TVector3<T>& axis, const T& angle);

		/**	Assign the TQuaternion components.
				@param w assigned to the angular component w
				@param i assigned to the vector component i
				@param j assigned to the vector component j
				@param k assigned to the vector component k
		*/
		void set(const T& w, const T& i, const T& j, const T& k);

		/**	Assignment operator.
				Assign the TQuaternion components form another TQuaternion.
		*/
		TQuaternion& operator = (const TQuaternion& q);

		/**	Assignment operator
				Assign the TQuaternion components from a boost::math::quaternion.
		*/
		TQuaternion& operator = (const boost::math::quaternion<T>& q);

		/** Set to an identity matrix.
			angular component w = 1;
			axis components i,j,k are set to 0;
		*/
		void setIdentity();

		/** Normalize the quaternion.
			 The quaternion is scaled with its norm:
			 @return TQuaternion&, a reference to the normalized quaternion
		*/
		TQuaternion<T>& normalize();

		/**	Swap the contents of two TQuaternion.
				@param q the TQuaternion to swap contents with
		*/
		void swap(TQuaternion& q);

		/**	Assign the TQuaternion from an rotation axis and an angle.
				@param axis the rotation axis
				@param angle the rotation angle
		*/
		void fromAxisAngle(const TVector3<T>& axis, const T& angle);

		/**	Assign the TQuaternion from Euler angles.
		 		Assume the following rotation order:.
				 	q' = roll( pitch( yaw(q) ) ).
				@param yaw the rotation about the yaw axis z-axis()
				@param pitch the rotation about the pitch axis (y-axis)
				@param roll the nrotation about the roll axis (x-axis)
		*/
		void fromEulerAngles(const T& yaw, const T& pitch, const T& roll);

		/**	Assign the TQuaternion to an axis and an angle.
				the rotation axis is normalized.
				@param axis the rotation axis
				@param angle the rotation angle
		*/
		void toAxisAngle(TVector3<T>& axis, T& angle);

		/**	Assign the TQuaternion to Euler angles.
		 		Assume the following rotation order:.
				 	q' = roll( pitch( yaw(q) ) ).
				@param yaw the rotation about the yaw axis z-axis()
				@param pitch the rotation about the pitch axis (y-axis)
				@param roll the nrotation about the roll axis (x-axis)
		*/
		void toEulerAngles(T& yaw, T& pitch, T& roll);
		//void toEA(T& yaw, T& pitch, T& roll);

		/**	Assign to another TQuaternion.
				Assigns the components to another TQuaternion.
				@param q the TQuaternion to be assigned to
		*/
		void get(TQuaternion& q) const;

		/**	Get the positive angle rotation.
				@return T the angle value
		*/
		T getAngle() const;

		/**	Get the normalized direction vector of the rotation axis.
				@return TVector3 the axis
		*/
		TVector3<T> getAxis();

		/**	Get the rotation matrix.
				@param m the matrix to compute from
				@return TMatrix4x4 the rotation matrix
		*/
		TMatrix4x4<T>& getRotationMatrix(TMatrix4x4<T>& m) const;

		/**	Get the inverse TQuaternion.
				@return TQuaternion the inverse TQuaternion
		*/
		TQuaternion getInverse() const;

		/**	Return the conjugate TQuaternion.
				(The axis components are negated.)
				@return TQuaternion the conjugate TQuaternion
		*/
		TQuaternion getConjugate() const;
		//@}
		/**	@name	Accessors
		*/
		//@{

		/**	Get the angular component w.
				@return get a reference to the angular component w
		*/
		T& w();

		/**	Get the constant angular component w.
				@return get a const reference to the angular component w
		*/
		const T& w() const;

		/**	Get the axis component i.
				@return get a reference to the axis component i
		*/
		T& i();

		/**	Get the constant axis component i.
				@return get a const reference to the axis component i
		*/
		const T& i() const;

		/**	Get the axis component j.
				@return get a reference to the axis component j
		*/
		T& j();

		/**	Get the constant axis component j.
				@return get a const reference to the axis component j
		*/
		const T& j() const;

		/**	Get the axis component k.
				@return get a reference to the axis component k
		*/
		T& k();

		/**	Get the constant axis component k.
				@return get a const reference to the axis component k
		*/
		const T& k() const;


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
		//@}

	};
	//@}

	template <typename T>
	TQuaternion<T>::TQuaternion()
		: boost::math::quaternion<T>()
	{
		this->setIdentity();
	}

	template <typename T>
	TQuaternion<T>::TQuaternion(const TQuaternion& q)
		:	boost::math::quaternion<T>(q)
	{
	}

	template <typename T>
	TQuaternion<T>::TQuaternion(const boost::math::quaternion<T>& q)
		:	boost::math::quaternion<T>(q)
	{
	}

	template <typename T>
	TQuaternion<T>::TQuaternion(const T& w, const T& i, const T& j, const T& k)
		: boost::math::quaternion<T>(w, i, j, k)
	{
	}

	template <typename T>
	TQuaternion<T>::TQuaternion(const TVector3<T>& axis, const T& angle)
		: boost::math::quaternion<T>()
	{
		fromAxisAngle(axis, angle);
	}

	template <typename T>
	TQuaternion<T>::~TQuaternion()
	{
	}

	template <typename T>
	void TQuaternion<T>::clear()
	{
		this->setIdentity();
	}

	template <typename T>
	void TQuaternion<T>::set(const TQuaternion<T>& q)
	{
		boost::math::quaternion<T>::operator= (q);
	}

	template <typename T>
	void TQuaternion<T>::set(const boost::math::quaternion<T>& q)
	{
		boost::math::quaternion<T>::operator= (q);
	}

	template <typename T>
	BALL_INLINE
	void TQuaternion<T>::set(const TVector3<T>& axis, const T& angle)
	{
		fromAxisAngle(axis, angle);
	}

	template <typename T>
	BALL_INLINE
	void TQuaternion<T>::set(const T& w, const T& i, const T& j, const T& k)
	{
		this->a = w;
		this->b = i;
		this->c = j;
		this->d = k;

	}

	template <typename T>
	BALL_INLINE
	TQuaternion<T>& TQuaternion<T>::operator = (const boost::math::quaternion<T>& q)
	{
		set(q);
		return *this;
	}

	template <typename T>
	BALL_INLINE
	TQuaternion<T>& TQuaternion<T>::operator = (const TQuaternion<T>& q)
	{
		set(q);
		return *this;
	}

	template <typename T>
	BALL_INLINE
	void TQuaternion<T>::setIdentity()
	{
		this->a = (T)1;
		this->b = this->c = this->d = (T)0;
	}

	template <typename T>
	BALL_INLINE
	TQuaternion<T>& TQuaternion<T>::normalize()
	{
		T length = boost::math::norm(*this);

		if (!(Maths::isEqual(length, (T)0)))
		{
			this->a /= length;
			this->b /= length;
			this->c /= length;
			this->d /= length;
		}
		return *this;
	}

	template <typename T>
	void TQuaternion<T>::swap(TQuaternion<T>& q)
	{
		T tmp = q.a;
		q.a = this->a;
		this->a = tmp;

		tmp = q.b;
		q.b = this->b;
		this->b = tmp;

		tmp = q.c;
		q.c = this->c;
		this->c = tmp;

		tmp = q.d;
		q.d = this->d;
		this->d = tmp;
	}

	template <typename T>
	void TQuaternion<T>::fromAxisAngle(const TVector3<T>& axis, const T& angle)
	{
		T length = axis.getLength();

		if (Maths::isEqual(length, (T)0))
		{
			this->b = this->c = this->d = (T)0;
			this->a = (T)1;
		}
		else
		{
			T omega = (T) (angle * 0.5);
			T sin_omega = (T)::sin(omega);

			this->a = (T)::cos(omega);
			this->b = axis.x * sin_omega / length;
			this->c = axis.y * sin_omega / length;
			this->d = axis.z * sin_omega / length;
		}
	}

	template <typename T>
	void TQuaternion<T>::fromEulerAngles(const T& yaw, const T& pitch, const T& roll)
	{
		T half_yaw = yaw / 2.0;
		T half_pitch = pitch / 2.0;
		T half_roll = roll / 2.0;

		T cosYaw = cos(half_yaw);
		T sinYaw = sin(half_yaw);

		T cosPitch = cos(half_pitch);
		T sinPitch = sin(half_pitch);

		T cosRoll = cos(half_roll);
		T sinRoll = sin(half_roll);


    this->a = cosRoll * cosPitch * cosYaw + sinRoll * sinPitch * sinYaw;
		this->b = sinRoll * cosPitch * cosYaw - cosRoll * sinPitch * sinYaw;
		this->c = cosRoll * sinPitch * cosYaw + sinRoll * cosPitch * sinYaw;
	  this->d = cosRoll * cosPitch * sinYaw - sinRoll * sinPitch * cosYaw;

	}

	template <typename T>
	void TQuaternion<T>::toAxisAngle(TVector3<T>& axis, T& angle)
	{
		T length = sqrt(this->b*this->b + this->c*this->c + this->d*this->d);

		if (Maths::isEqual(length, (T)0))
		{
			axis.x = axis.y = angle= (T)0;
			axis.y = (T)1;
		}
		else
		{
			angle = 2 * (T)::acos(this->a);
			axis.x = this->b / length;
			axis.y = this->c / length;
			axis.z = this->d / length;
		}
	}


	template <typename T>
	void TQuaternion<T>::toEulerAngles(T& yaw, T& pitch, T& roll)
	{
		TMatrix4x4<T> matrix;
		getRotationMatrix(matrix);
		T sinYaw, cosYaw, sinPitch, cosPitch, sinRoll, cosRoll;

		sinPitch = -matrix(2,0);
		cosPitch = sqrt(1 - sinPitch*sinPitch);

		if ( fabs(cosPitch) > Constants::EPSILON)
		{
			sinRoll = matrix(2,1) / cosPitch;
			cosRoll = matrix(2,2) / cosPitch;
			sinYaw = matrix(1,0) / cosPitch;
			cosYaw = matrix(0,0) / cosPitch;
		}
		else
		{
			sinRoll = -matrix(1,2);
			cosRoll = matrix(1,1);
			sinYaw = 0;
			cosYaw = 1;
		}

		/* yaw */
		yaw = atan2(sinYaw, cosYaw);

		/* pitch */
		pitch = atan2(sinPitch, cosPitch);

		/* roll */
		roll = atan2(sinRoll, cosRoll);

	}

	template <typename T>
	BALL_INLINE
	void TQuaternion<T>::get(TQuaternion<T>& q) const
	{
		q.set(*this);
	}

	template <typename T>
	T TQuaternion<T>::getAngle() const
	{
		T length = sqrt(this->b*this->b + this->c*this->c + this->d*this->d);
		if (Maths::isEqual(length, (T)0))
		{
			return (T)(0);
		}
		else
		{
			return (T)(2 * (T)::acos(this->a));
		}
	}

	template <typename T>
	TVector3<T> TQuaternion<T>::getAxis()
	{
		T length = sqrt(this->b*this->b + this->c*this->c + this->d*this->d);
		if (Maths::isEqual(length, (T)0))
		{
			return TVector3<T>((T)0,(T)0,(T)1);
		}
		else
		{
			return TVector3<T>(this->b/length, this->c/length, this->d/length);
		}
	}

	template <typename T>
	TMatrix4x4<T>& TQuaternion<T>::getRotationMatrix(TMatrix4x4<T>& m) const
	{
		T s = 2.0 / boost::math::norm(*this);
		m.set
			(
				(T)(1.0 - s * (this->c * this->c + this->d * this->d)),
				(T)(s * (this->b * this->c - this->d * this->a)),
				(T)(s * (this->d * this->b + this->c * this->a)),
				(T)0,

				(T)(s * (this->b * this->c + this->d * this->a)),
				(T)(1.0 - s * (this->d * this->d + this->b * this->b)),
				(T)(s * (this->c * this->d - this->b * this->a)),
				(T)0,

				(T)(s * (this->d * this->b - this->c * this->a)),
				(T)(s * (this->c * this->d + this->b * this->a)),
				(T)(1.0 - s * (this->c * this->c + this->b * this->b)),
				(T)0,

				(T)0,
				(T)0,
				(T)0,
				(T)1
			);

		return m;
	}

	template <typename T>
	BALL_INLINE
	TQuaternion<T> TQuaternion<T>::getInverse() const
	{

		return conj(*this) / boost::math::norm(*this);
	}

	template <typename T>
	BALL_INLINE
	TQuaternion<T> TQuaternion<T>::getConjugate() const
	{
		return conj(*this);
	}

	template <typename T>
	const T& TQuaternion<T>::w() const
	{
		return this->a;
	}

	template <typename T>
	T& TQuaternion<T>::w()
	{
		return this->a;
	}

	template <typename T>
	const T& TQuaternion<T>::i() const
	{
		return this->b;
	}

	template <typename T>
	T& TQuaternion<T>::i()
	{
		return this->b;
	}

	template <typename T>
	const T& TQuaternion<T>::j() const
	{
		return this->c;
	}

	template <typename T>
	T& TQuaternion<T>::j()
	{
		return this->c;
	}

	template <typename T>
	const T& TQuaternion<T>::k() const
	{
		return this->d;
	}

	template <typename T>
	T& TQuaternion<T>::k()
	{
		return this->d;
	}

	template <typename T>
	std::istream& operator >>(std::istream& s, TQuaternion<T>& q)
		
	{
		char c;
		s >> c >> q.w() >> c >> q.i() >> c >> q.j() >> c >>q.k() >> c;
		return s;
	}

	template <typename T>
	std::ostream& operator << (std::ostream& s, const TQuaternion<T>& q)
		
	{
		s << '(' << q.w() << ',' << q.i() << ','
				     << q.j() << ',' << q.k() << ')';

		return s;
	}

	template <typename T>
	void TQuaternion<T>::dump(std::ostream& s, Size depth) const
	{
		BALL_DUMP_STREAM_PREFIX(s);

		BALL_DUMP_HEADER(s, this, this);

		BALL_DUMP_DEPTH(s, depth);
		s << "    w: " << this->w() << std::endl;

		BALL_DUMP_DEPTH(s, depth);
		s << "    i: " << this->i() << std::endl;

		BALL_DUMP_DEPTH(s, depth);
		s << "    j: " << this->j() << std::endl;

		BALL_DUMP_DEPTH(s, depth);
		s << "    k: " << this->k() << std::endl;

		BALL_DUMP_STREAM_SUFFIX(s);
	}



	typedef TQuaternion<float> Quaternion;

} // namespace BALL

#endif // BALL_MATHS_QUATERNION_H

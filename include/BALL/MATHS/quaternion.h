// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:

#ifndef BALL_MATHS_QUATERNION_H
#define BALL_MATHS_QUATERNION_H


#include <BALL/MATHS/matrix44.h>
#include <BALL/MATHS/vector3.h>

#include <Eigen/Geometry>
#include <iostream>


namespace BALL
{
	/**
	 * Generic Quaternion Class.
	 *
	 * Representing a rotation in three dimensional space.
	 */
	template <typename T>
	class TQuaternion
		: public Eigen::Quaternion<T> //{/*...*/};
	{

		public:
		/**
		 * @name Constructors and Destructors
		 */
		//@{

		/**
		 * Default constructor.
		 * This method creates a new TQuaternion object.
		 * The quaternion is set to an identity quaternion.
		 */
		TQuaternion();

		/**
		 * Copy constructor.
		 * Create a new TQuaternion object from another.
		 * @param TQuaternion the TQuaternion object to be copied
		 */
		TQuaternion(const TQuaternion& q);

		/**
		 * Detailed constructor.
		 * Create a new TQuaternion object from an Eigen::Quaternion<T>.
		 * @param Eigen::Quaternion<T>
		 */
		TQuaternion(const Eigen::Quaternion<T>& q);

		/**
		 * Detailed constructor.
		 * Create a new TQuaternion object from four values of type <b>T</b>.
		 * @param w assigned to the angular component w
		 * @param i assigned to the vector component i
		 * @param j assigned to the vector component j
		 * @param k assigned to the vector component k
		 */
		TQuaternion(const T& a, const T& b, const T& c, const T& d);

		/**
		 * Detailed constructor.
		 * Create a new TQuaternion object from a variable of type <b>  TVector3 </b> and an angle.
		 * @param axis assigned to the axis
		 * @param angle assigned to the angle
		 */
		TQuaternion(const TVector3<T>& axis, const T& angle);

		/**
		 * Destructor.
		 * Destructs the TQuaternion object. As there are no dynamic
		 * data structures, nothing happens.
		 */
		~TQuaternion();

		/**
		 * Clear method.
		 * The values are set to the identity quaternion.
		 */
		void clear();

		//@}
		/**
		 * @name Assignment
		 */
		//@{

		/**
		 * Assign coefficients of q to this quaternion
		 * @param q the Quaternion to copy coefficients from
		 */
		void set(const TQuaternion& q);

		/**
		 * Assign coefficients of q to this quaternion
		 * @param q the Eigen::Quaternion<T> to copy coefficients from
		 */
		void set(const Eigen::Quaternion<T>& q);

		/**
		 * Assign the TQuaternion components.
		 * @param w assigned to the angular component w
		 * @param i assigned to the vector component i
		 * @param j assigned to the vector component j
		 * @param k assigned to the vector component k
		 */
		void set(const T& a, const T& b, const T& c, const T& d);

		/**
		 * Assignment operator.
		 * Assign the TQuaternion components form another TQuaternion.
		 */
		TQuaternion& operator = (const TQuaternion& q);

		/**
		 * Assignment operator
		 * Assign the TQuaternion components from an Eigen::Quaternion.
		 */
		TQuaternion& operator = (const Eigen::Quaternion<T>& q);

		/**
		 * @brief Equality operator
		 * Returns true if the given Quaternion q is equal to this.
		 */
		bool operator == (const TQuaternion<T>& q);

		/**
		 * Set to an identity matrix.
		 * angular component w = 1;
		 * axis components i,j,k are set to 0;
		 */
		void setIdentity();

		/**
		 * Normalize the quaternion.
		 * The quaternion is scaled with its norm:
		 * @return TQuaternion&, a reference to the normalized quaternion
		 */
		TQuaternion<T>& normalize();

		/**
		 * Swap the contents of two TQuaternion.
		 * @param q the TQuaternion to swap contents with
		 */
		void swap(TQuaternion& q);

		/**
		 * Assign the TQuaternion from an rotation axis and an angle.
		 * @param axis the rotation axis
		 * @param angle the rotation angle
		 */
		void fromAxisAngle(const TVector3<T>& axis, const T& angle);

		/**
		 * Assign the TQuaternion from Euler angles.
		 * Assume the following rotation order: q' = roll( pitch( yaw(q) ) ).
		 * @param yaw the rotation about the yaw axis z-axis()
		 * @param pitch the rotation about the pitch axis (y-axis)
		 * @param roll the nrotation about the roll axis (x-axis)
		 */
		void fromEulerAngles(const T& yaw, const T& pitch, const T& roll);

		/**
		 * Assign the TQuaternion to an axis and an angle.
		 * the rotation axis is normalized.
		 * @param axis the rotation axis
		 * @param angle the rotation angle
		 */
		void toAxisAngle(TVector3<T>& axis, T& angle);

		/**
		 * Assign the TQuaternion to Euler angles.
		 * Assume the following rotation order:  q' = roll( pitch( yaw(q) ) ).
		 * @param yaw the rotation about the yaw axis z-axis()
		 * @param pitch the rotation about the pitch axis (y-axis)
		 * @param roll the nrotation about the roll axis (x-axis)
		 */
		void toEulerAngles(T& yaw, T& pitch, T& roll);

		/**
		 * Assign to another TQuaternion.
		 * Assigns the components to another TQuaternion.
		 * @param q the TQuaternion to be assigned to
		 */
		void get(TQuaternion& q) const;

		/**
		 * Get the positive angle rotation.
		 * @return T the angle value
		 */
		T getAngle() const;

		/**
		 * Get the normalized direction vector of the rotation axis.
		 * @return TVector3 the axis
		 */
		TVector3<T> getAxis();

		/**
		 * Get the rotation matrix.
		 * @param m the matrix to compute from
		 * @return TMatrix4x4 the rotation matrix
		 */
		TMatrix4x4<T>& getRotationMatrix(TMatrix4x4<T>& m) const;

		/**
		 * Get the inverse TQuaternion.
		 * @return TQuaternion the inverse TQuaternion
		 */
		TQuaternion getInverse() const;

		/**
		 * Return the conjugate TQuaternion.
		 * (The axis components are negated.)
		 * @return TQuaternion the conjugate TQuaternion
		 */
		TQuaternion getConjugate() const;
		//@}
		/**
		 * @name Accessors
		 */
		//@{

		/**
		 * Get the angular component w.
		 * @return get a reference to the angular component w
		 */
		T& w();

		/**
		 * Get the constant angular component w.
		 * @return get a const reference to the angular component w
		 */
		const T& w() const;

		/**
		 * Get the axis component i.
		 * @return get a reference to the axis component i
		 */
		T& i();

		/**
		 * Get the constant axis component i.
		 * @return get a const reference to the axis component i
		 */
		const T& i() const;

		/**
		 * Get the axis component j.
		 * @return get a reference to the axis component j
		 */
		T& j();

		/**
		 * Get the constant axis component j.
		 * @return get a const reference to the axis component j
		 */
		const T& j() const;

		/**
		 * Get the axis component k.
		 * @return get a reference to the axis component k
		 */
		T& k();

		/**
		 * Get the constant axis component k.
		 * @return get a const reference to the axis component k
		 */
		const T& k() const;


		//@}
		/** @name Debugging and Diagnostics
		*/
		//@{

		/**
		 * Internal state dump.
		 * Dump the current internal state of {\em *this} to
		 * the output ostream <b>  s </b> with dumping depth <b>  depth </b>.
		 * @param   s - output stream where to output the internal state of {\em *this}
		 * @param   depth - the dumping depth
		 */
		void dump(std::ostream& s = std::cout, Size depth = 0) const;
		//@}
	};
	//@}

	template <typename T>
	TQuaternion<T>::TQuaternion()
		: Eigen::Quaternion<T>()
	{
		this->setIdentity();
	}

	template <typename T>
	TQuaternion<T>::TQuaternion(const TQuaternion& q)
		: Eigen::Quaternion<T>(q)
	{
	}

	template <typename T>
	TQuaternion<T>::TQuaternion(const Eigen::Quaternion<T>& q)
		: Eigen::Quaternion<T>(q)
	{
	}

	template <typename T>
	TQuaternion<T>::TQuaternion(const T& a, const T& b, const T& c, const T& d)
		: Eigen::Quaternion<T>(a, b, c, d)
	{
	}

	template <typename T>
	TQuaternion<T>::TQuaternion(const TVector3<T>& axis, const T& angle)
		: Eigen::Quaternion<T>()
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
		Eigen::Quaternion<T>::operator= (q);
	}

	template <typename T>
	void TQuaternion<T>::set(const Eigen::Quaternion<T>& q)
	{
		Eigen::Quaternion<T>::operator= (q);
	}

	template <typename T>
	BALL_INLINE
	void TQuaternion<T>::set(const T& a, const T& b, const T& c, const T& d)
	{
		w() = a;
		i() = b;
		j() = c;
		k() = d;
	}

	template <typename T>
	BALL_INLINE
	TQuaternion<T>& TQuaternion<T>::operator = (const Eigen::Quaternion<T>& q)
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
	bool TQuaternion<T>::operator == (const TQuaternion<T>& q)
	{
		return Eigen::Quaternion<T>::isApprox(q);
	}

	template <typename T>
	BALL_INLINE
	void TQuaternion<T>::setIdentity()
	{
		Eigen::Quaternion<T>::setIdentity();
	}

	template <typename T>
	BALL_INLINE
	TQuaternion<T>& TQuaternion<T>::normalize()
	{
		Eigen::Quaternion<T>::normalize();

		return *this;
	}

	template <typename T>
	void TQuaternion<T>::swap(TQuaternion<T>& q)
	{
		TQuaternion<T> tmp(q);

		q = *this;
		*this = tmp;
	}

	template <typename T>
	void TQuaternion<T>::fromAxisAngle(const TVector3<T>& axis, const T& angle)
	{
		TVector3<T> axis_normalized(axis);
		axis_normalized.normalize();

		Eigen::AngleAxis<T> aa(angle, Eigen::Matrix<T, 3, 1>(axis_normalized.x, axis_normalized.y, axis_normalized.z));
		Eigen::Quaternion<T> q(aa);

		set(q.w(), q.x(), q.y(), q.z());
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

		w() = cosRoll * cosPitch * cosYaw + sinRoll * sinPitch * sinYaw;
		i() = sinRoll * cosPitch * cosYaw - cosRoll * sinPitch * sinYaw;
		j() = cosRoll * sinPitch * cosYaw + sinRoll * cosPitch * sinYaw;
		k() = cosRoll * cosPitch * sinYaw - sinRoll * sinPitch * cosYaw;
	}

	template <typename T>
	void TQuaternion<T>::toAxisAngle(TVector3<T>& axis, T& angle)
	{
		Eigen::AngleAxis<T> aa(*this);

		Eigen::Matrix<T, 3, 1> eigen_axis = aa.axis();
		axis.set(eigen_axis[0], eigen_axis[1], eigen_axis[2]);
		angle = aa.angle();
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

		// yaw
		yaw = atan2(sinYaw, cosYaw);

		// pitch
		pitch = atan2(sinPitch, cosPitch);

		// roll
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
		Eigen::AngleAxis<T> aa(*this);

		return aa.angle();
	}

	template <typename T>
	TVector3<T> TQuaternion<T>::getAxis()
	{
		Eigen::AngleAxis<T> aa(*this);
		Eigen::Matrix<T, 3, 1> eigen_axis = aa.axis();

		return TVector3<T>(eigen_axis[0], eigen_axis[1], eigen_axis[2]);
	}

	template <typename T>
	TMatrix4x4<T>& TQuaternion<T>::getRotationMatrix(TMatrix4x4<T>& m) const
	{
		Eigen::Matrix<T, 3, 3> eigen_rot = this->toRotationMatrix();

		m.set(
					eigen_rot(0, 0), eigen_rot(0, 1), eigen_rot(0, 2), 0.0,
					eigen_rot(1, 0), eigen_rot(1, 1), eigen_rot(1, 2), 0.0,
					eigen_rot(2, 0), eigen_rot(2, 1), eigen_rot(2, 2), 0.0,
					0.0, 0.0, 0.0, 1.0
			);

		return m;
	}

	template <typename T>
	BALL_INLINE
	TQuaternion<T> TQuaternion<T>::getInverse() const
	{
		return Eigen::Quaternion<T>::inverse();
	}

	template <typename T>
	BALL_INLINE
	TQuaternion<T> TQuaternion<T>::getConjugate() const
	{
		return Eigen::Quaternion<T>::conjugate();
	}

	template <typename T>
	const T& TQuaternion<T>::w() const
	{
		return Eigen::Quaternion<T>::coeffs()[3];
	}

	template <typename T>
	T& TQuaternion<T>::w()
	{
		return Eigen::Quaternion<T>::coeffs()[3];
	}

	template <typename T>
	const T& TQuaternion<T>::i() const
	{
		return Eigen::Quaternion<T>::coeffs()[0];
	}

	template <typename T>
	T& TQuaternion<T>::i()
	{
		return Eigen::Quaternion<T>::coeffs()[0];
	}

	template <typename T>
	const T& TQuaternion<T>::j() const
	{
		return Eigen::Quaternion<T>::coeffs()[1];
	}

	template <typename T>
	T& TQuaternion<T>::j()
	{
		return Eigen::Quaternion<T>::coeffs()[1];
	}

	template <typename T>
	const T& TQuaternion<T>::k() const
	{
		return Eigen::Quaternion<T>::coeffs()[2];
	}

	template <typename T>
	T& TQuaternion<T>::k()
	{
		return Eigen::Quaternion<T>::coeffs()[2];
	}

	template <typename T>
	std::istream& operator >>(std::istream& s, TQuaternion<T>& q)
	{
		char c;
		s >> c >> q.w() >> c >> q.i() >> c >> q.j() >> c >> q.k() >> c;
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
		s << "    w: " << w() << std::endl;

		BALL_DUMP_DEPTH(s, depth);
		s << "    i: " << i() << std::endl;

		BALL_DUMP_DEPTH(s, depth);
		s << "    j: " << j() << std::endl;

		BALL_DUMP_DEPTH(s, depth);
		s << "    k: " << k() << std::endl;

		BALL_DUMP_STREAM_SUFFIX(s);
	}

	typedef TQuaternion<float> Quaternion;

} // namespace BALL

#endif // BALL_MATHS_QUATERNION_H

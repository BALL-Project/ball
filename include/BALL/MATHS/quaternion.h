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

#	include <boost/math/quaternion.hpp>
#include <iostream>

namespace BALL 
{
	template <typename T>
	class TQuaternion
		: public boost::math::quaternion<T> //{/*...*/};
	{
		public:

		BALL_CREATE(TQuaternion<T>)
		
		TQuaternion();
		TQuaternion(const boost::math::quaternion<T>& q);
		TQuaternion(const TQuaternion& q);
		TQuaternion(const T& w, const T& i, const T& j, const T& k);
		TQuaternion(const TVector3<T>& axis, const T& angle);
		virtual ~TQuaternion();

		virtual void clear(); 
		
		void set(const boost::math::quaternion<T>& q);
		void set(const T& w, const T& i, const T& j, const T& k);
		void constructFromAxis(const TVector3<T>& axis, const T& new_angle);
		
		TQuaternion& operator = (const boost::math::quaternion<T>& q);
		TQuaternion& operator = (const TQuaternion& q);
		TMatrix4x4<T>& getRotationMatrix(TMatrix4x4<T>& m) const;
		
		TQuaternion getConjugate() const
			throw();
		
		T& w();
		const T& w() const;
		T& i();
		const T& i() const;
		T& j();
		const T& j() const;
		T& k();
		const T& k() const;

		protected:
		
		T& w_;
		
		T& i_;

		T& j_;

		T& k_;

	};
	
	template <typename T>
	TQuaternion<T>::TQuaternion()
		: boost::math::quaternion<T>(),
			w_(this->a),
			i_(this->b),
			j_(this->c),
			k_(this->d)
	{
	}
	
	template <typename T>
	TQuaternion<T>::TQuaternion(const boost::math::quaternion<T>& q)
		:	boost::math::quaternion<T>(q),
			w_(this->a),
			i_(this->b),
			j_(this->c),
			k_(this->d)
	{
	}
	
	template <typename T>
	TQuaternion<T>::TQuaternion(const TQuaternion& q)
		:	boost::math::quaternion<T>(q),
			w_(this->a),
			i_(this->b),
			j_(this->c),
			k_(this->d)
	{
	}
	
	template <typename T>
	TQuaternion<T>::TQuaternion(const T& w, const T& i, const T& j, const T& k)
		: boost::math::quaternion<T>(w, i, j, k),
			w_(this->a),
			i_(this->b),
			j_(this->c),
			k_(this->d)
	{
	}
	
	template <typename T>
	TQuaternion<T>::TQuaternion(const TVector3<T>& axis, const T& new_angle)
		: boost::math::quaternion<T>(),
			w_(this->a),
			i_(this->b),
			j_(this->c),
			k_(this->d)
	{
		constructFromAxis(axis, new_angle);
	}
	
	template <typename T>
	TQuaternion<T>::~TQuaternion()
	{
	}

	template <typename T>
	void TQuaternion<T>::clear()
	{
		boost::math::quaternion<T>::operator= ((T)1);
	}
	
	template <typename T>
	void TQuaternion<T>::set(const boost::math::quaternion<T>& q)
	{
		boost::math::quaternion<T>::operator= (q);
	}
	
	template <typename T>
	BALL_INLINE 
	void TQuaternion<T>::set(const T& w, const T& i, const T& j, const T& k)
	{
		w_ = w;
		i_ = i;
		j_ = j;
		k_ = k;
		
	}

	template <typename T>
	void TQuaternion<T>::constructFromAxis(const TVector3<T>& axis, const T& new_angle)
	{
		T length = (T)sqrt(axis.x * axis.x + axis.y * axis.y + axis.z * axis.z);

		if (Maths::isEqual(length, (T)0))
		{
			i_ = j_ = k_ = (T)0;
			w_ = (T)1;
		} 
		else 
		{
			T omega = (T) (new_angle * 0.5);
			T sin_omega = (T)::sin(omega);

			w_ = (T)::cos(omega);
			i_ = axis.x * sin_omega / length;
			j_ = axis.y * sin_omega / length;
			k_ = axis.z * sin_omega / length;
		}
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
	TMatrix4x4<T>& TQuaternion<T>::getRotationMatrix(TMatrix4x4<T>& m) const
	{
		T s = 2.0 / (i_*i_ + j_*j_ + k_*k_ + w_*w_ );
		m.set
			(
				(T)(1.0 - s * (j_ * j_ + k_ * k_)), 
				(T)(s * (i_ * j_ - k_ * w_)), 
				(T)(s * (k_ * i_ + j_ * w_)), 
				(T)0, 
				
				(T)(s * (i_ * j_ + k_ * w_)), 
				(T)(1.0 - s * (k_ * k_ + i_ * i_)), 
				(T)(s * (j_ * k_ - i_ * w_)), 
				(T)0, 
				
				(T)(s * (k_ * i_ - j_ * w_)),
				(T)(s * (j_ * k_ + i_ * w_)),
				(T)(1.0 - s * (j_ * j_ + i_ * i_)), 
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
	TQuaternion<T> TQuaternion<T>::getConjugate() const
		throw()
	{
		return conj(this);
	}
	
	
	typedef TQuaternion<float> Quaternion;

} // namespace BALL

#endif // BALL_MATHS_QUATERNION_H

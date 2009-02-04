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
		void setIdentity();
		void normalize();
		void swap(TQuaternion& q);
		void fromAxisAngle(const TVector3<T>& axis, const T& angle);
		void toAxisAngle(TVector3<T>& axis, T& angle);
		
		T getAngle() const;
		TVector3<T> getAxis();
		TQuaternion& operator = (const boost::math::quaternion<T>& q);
		TQuaternion& operator = (const TQuaternion& q);
		void dump(std::ostream& s = std::cout, Size depth = 0) const;
		TMatrix4x4<T>& getRotationMatrix(TMatrix4x4<T>& m) const;
		
		TQuaternion getInverse() const;
		TQuaternion getConjugate() const;
		
		T& w();
		const T& w() const;
		T& i();
		const T& i() const;
		T& j();
		const T& j() const;
		T& k();
		const T& k() const;

	};
	
	template <typename T>
	TQuaternion<T>::TQuaternion()
		: boost::math::quaternion<T>()
	{
	}
	
	template <typename T>
	TQuaternion<T>::TQuaternion(const boost::math::quaternion<T>& q)
		:	boost::math::quaternion<T>(q)
	{
	}
	
	template <typename T>
	TQuaternion<T>::TQuaternion(const TQuaternion& q)
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
	void TQuaternion<T>::set(const boost::math::quaternion<T>& q)
	{
		boost::math::quaternion<T>::operator= (q);
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
	void TQuaternion<T>::setIdentity()
	{
		this->a = (T)1;
		this->b = this->c = this->d = (T)0;
	}

	template <typename T>
	BALL_INLINE 
	void TQuaternion<T>::normalize()
	{
		T length = (T) norm(*this);

		if (!(Maths::isEqual(length, (T)0)))
		{
			this->a /= length;
			this->b /= length;
			this->c /= length;
			this->d /= length;
		}
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
	void TQuaternion<T>::toAxisAngle(TVector3<T>& axis, T& angle)
	{
		T length = (T) norm(*this);

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
	T TQuaternion<T>::getAngle() const
	{
		T length = (T) norm(*this);
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
		T length = (T) norm(*this);
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
	std::istream& operator >>(std::istream& s, TQuaternion<T>& q)
		throw()
	{
		char c;
		s >> c >> q.w() >> q.i() >> q.j() >> q.k() >> c;
		return s;
	}

	template <typename T>
	std::ostream& operator << (std::ostream& s, const TQuaternion<T>& q)
		throw()
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
		s << "    w: " << w << std::endl;

		BALL_DUMP_DEPTH(s, depth);
		s << "    i: " << i << std::endl;

		BALL_DUMP_DEPTH(s, depth);
		s << "    j: " << j << std::endl;

		BALL_DUMP_DEPTH(s, depth);
		s << "    k: " << k << std::endl;

		BALL_DUMP_STREAM_SUFFIX(s);
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
		T s = 2.0 / (this->b*this->b + this->c*this->c + this->d*this->d + this->a*this->a );
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
		
		return conj(*this) / norm(*this);
	}
	
	template <typename T>
	BALL_INLINE 
	TQuaternion<T> TQuaternion<T>::getConjugate() const
	{
		return conj(*this);
	}
	
	
	typedef TQuaternion<float> Quaternion;

} // namespace BALL

#endif // BALL_MATHS_QUATERNION_H

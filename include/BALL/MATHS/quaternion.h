// $Id: quaternion.h,v 1.3 1999/12/04 18:34:20 oliver Exp $

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

	/**	Generic Quaternion Class.
      {\bf Definition:} \URL{BALL/MATHS/.h}
      \\
	*/
	template <class T>
	class TQuaternion
	{
		public:

		BALL_CREATE(TQuaternion<T>)
		

		/**	@name	Constructors and Destructors
		*/
		//@{

		///
		TQuaternion();

		///
		TQuaternion(const TQuaternion& q, bool deep = true);

		///
		TQuaternion(const TVector3<T>& axis, const T &angle);

		///
		TQuaternion(const T& x, const T& y, const T& z, const T& angle);

		///
		virtual ~TQuaternion();
		//@}

		/**	@name	Assignment
		*/
		//@{
		///
		void set(const TQuaternion& q, bool deep = true);

		///
		void set(const TVector3<T>& axis, const T& angle);

		///
		void set(const T& x, const T& y, const T& z, const T& angle);

		///
		TQuaternion& operator = (const TQuaternion& q);

		///
		void get(TQuaternion& q, bool deep = true);

		///
		void setIdentity();

		///
		void swap(TQuaternion& q);
		//@}


		/**	@name	Accessots
		*/
		//@{
		///
		T getAngle() const;

		///
		TVector3<T> getAxis(TVector3<T> &v);

		///
		TMatrix4x4<T>& getRotationMatrix(TMatrix4x4<T>& m) const;

		///
		TQuaternion operator - () const;

		///
		TQuaternion getInverse() const;

		///
		TQuaternion getConjugate() const;

		///
		TQuaternion& operator += (const TQuaternion& q);

		///
		template <typename C>
		friend TQuaternion<C> operator + (const TQuaternion<C>& a, const TQuaternion<C>& b);

		///
		TQuaternion& operator -= (const TQuaternion& q);

		///
		template <typename C>
		friend TQuaternion<C> operator - (const TQuaternion<C>& a, const TQuaternion<C>& b);
		//@}


		/**	@name	Predicates
		*/
		//@{
		///
		bool operator == (const TQuaternion& q) const;

		///
		bool operator != (const TQuaternion& q) const;
		//@}


		/**	@name	Storers
		*/
		//@{
		///
		template <typename C>
		friend ostream& operator << (ostream& s, const TQuaternion<C>& q);

		///
		template <typename C>
		friend istream &operator >> (istream &s, TQuaternion<C>& q);
		//@}

		/**	@name	Debugging and Diagnostics
		*/
		//@{
		///
		void dump(ostream& s = cout, unsigned long depth = 0) const;
		//@}


		/**	@name	Attributes
		*/
		//@{
		///
		T i;

		///
		T j;

		///
		T k;

		/// 
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
	TQuaternion<T>::TQuaternion(const TQuaternion& q, bool /* deep */)
		:	i(q.i),
			j(q.j),
			k(q.k),
			angle(q.angle)
	{
	}

	template <class T>
	TQuaternion<T>::TQuaternion(const TVector3<T>& axis, const T& angle)
	{
		set(axisv.x, axisv.y, axisv.z, angle);
	}

	template <class T>
	TQuaternion<T>::TQuaternion(const T& x, const T& y, const T& z, const T& angle)
	{
		set(x, y, z, angle);
	}

	template <class T>
	TQuaternion<T>::~TQuaternion()
	{
	}

	template <class T>
	void TQuaternion<T>::set(const TQuaternion<T>& q, bool /* deep */)
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
	void TQuaternion<T>::set(const TVector3<T>& axis, const T& angle)
	{
		set(axis.x, axis.y, axis.z, angle);
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
	void TQuaternion<T>::get(TQuaternion<T>& q, bool deep)
	{
		q.set(*this, deep);
	}

	template <class T>
	BALL_INLINE 
	void TQuaternion<T>::setIdentity()
	{
		i = j = k = (T)0;
		angle = (T)1;
	}

	// getAngle() -- get positive angle rotation

	template <class T>
	T TQuaternion<T>::getAngle() const
	{
		return (T)(2.0 * atan2(sqrt(i * i + j * j + k * k), angle));
	}

	// getAxis() -- get normalized direction vector of axis of rotation.

	template <class T>
	TVector3<T> TQuaternion<T>::getAxis(TVector3<T>& v)
	{
		TVector3<T> vector(i, j, k);
		T length = vector.getLength();

		BALL_PRECONDITION
			(length != (T)0,  
			 BALL_TYPED_QUATERNION_ERROR_HANDLER(ERROR__AXIS_NOT_WELL_DEFINED));
		
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
	TQuaternion<T>
	TQuaternion<T>::getConjugate() const
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
	TQuaternion<T> operator + (const TQuaternion<T>& a, const TQuaternion<T>& b)
	{
		TQuaternion<T> q(a);

		q += b;

		return q;
	}

	template <class T>
	BALL_INLINE 
	TQuaternion<T>& TQuaternion<T>::operator -= (const TQuaternion<T>& q)
	{
		return (*this += -q);
	}

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
	istream& operator >>(istream &s, TQuaternion<T>& q)
	{
		s >> q.i;
		s >> q.j;
		s >> q.k;
		s >> q.angle;

		return s;
	}

	template <class T>
	ostream& operator << (ostream& s, const TQuaternion<T>& q)
	{
		s << '(' << q.i << ' ' << q.j << ' '
				 << q.k << ' ' << q.angle << ')';

		return s;
	}   

	template <class T>
	void TQuaternion<T>::dump(ostream& s, unsigned long depth) const
	{
		BALL_DUMP_STREAM_PREFIX(s);

		BALL_DUMP_HEADER(s, this, this);

		BALL_DUMP_DEPTH(s, depth);
		s << "  angle (real): " << angle << endl;

		BALL_DUMP_DEPTH(s, depth);
		s << "    i: " << i << endl;

		BALL_DUMP_DEPTH(s, depth);
		s << "    j: " << j << endl;

		BALL_DUMP_DEPTH(s, depth);
		s << "    k: " << k << endl;

		BALL_DUMP_STREAM_SUFFIX(s);
	}

	///
	typedef TQuaternion<Real> Quaternion;

} // namespace BALL

#endif // BALL_MATHS_QUATERNION_H

// $Id: vector4.h,v 1.1 1999/08/26 07:53:18 oliver Exp $

#ifndef BALL_MATHS_VECTOR4_H
#define BALL_MATHS_VECTOR4_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifdef BALL_INCLUDE_IEEEFP
#	include <ieeefp.h>
#endif 

#include <math.h>
#include <iostream.h>

#ifndef BALL_CONCEPT_PROCESSOR_H
#	include <BALL/CONCEPT/processor.h>
#endif

#ifndef BALL_MATHS_ANGLE_H
#	include <BALL/MATHS/angle.h>
#endif

#ifndef BALL_MATHS_VECTOR_H
#	include <BALL/MATHS/vector.h>
#endif


namespace BALL 
{

	/**	Generic Four-Dimensional Vector.
			{\bf Definition:} \URL{BALL/MATHS/vector4.h}
			\\
	*/
	template <class T>
	class TVector4
	{
		public:

		BALL_CREATE(TVector4<T>)

		/**	@name	Constructors and Destructors
		*/
		//@{

		///	
		TVector4(void);

		///	
		TVector4(const T* ptr);

		///	
		TVector4(const TVector_<T>& vector);

		///	
		TVector4(const T& x, const T& y, const T& z, const T& h = (T)1);

		///	
		TVector4(const TVector4& vector, bool deep = true);

		///	
		virtual ~TVector4(void);
		//@}

		/**	@name	Converters
		*/
		//@{

		///	
		operator TVector_<T>(void) const;
		//@}

		/**	@name	Assignment
		*/
		//@{
	
		///	
		void set(const T* ptr);

		///	
		void set(const TVector_<T>& vector);

		///	
		void set(const T& x, const T& y, const T& z, const T& h = (T)1);

		///	
		void set(const TVector4& vector, bool deep = true);

		///	
		TVector4& operator = (const T* ptr);

		///	
		TVector4& operator = (const TVector_<T>& vector);

		///	
		TVector4 &operator = (const TVector4& vector);

		///	
		void get(T* ptr) const;

		///	
		void get(TVector_<T>& vector) const;

		///	
		void get(T& x, T& y, T& z, T& h) const;

		///	
		void get(TVector4& vector, bool deep = true) const;

		///	
		void swap(TVector4& vector);
		//@}

		/**	@name	Accessors
		*/
		//@{

		///	
		T getLength(void) const;

		///	
		T getSquareLength(void) const;

		///	
		void normalize(void);

		///
		static const TVector4& getZero(void);

		///
		void setUnit(void);

		///
		static const TVector4& getUnit(void);

		///
		void fill(const T& value = (T)1);

		///
		T& operator [] (Index index);

		///
		const T& operator [] (Index index) const;

		// --- unary operators ---

		///
		TVector4 operator + (void) const;
		
		///
		TVector4 operator - (void) const;

		// --- vector addition ---

		///
		template <typename C>
		friend TVector4<C> operator + (const TVector4<C>& a, const TVector4<C>& b);

		///
		TVector4& operator += (const TVector4& vector);

		// --- vector subtraction ---

		///
		template <typename C>
		friend TVector4<C> operator - (const TVector4<C>& a, const TVector4<C>& b);

		///
		TVector4& operator -= (const TVector4& vector);

		// --- scalar multiplication ---

		///
		TVector4 operator * (const T& scalar);

		///
		template <typename C>
		friend TVector4<C> operator * (const T& scalar,const TVector4<C>& vector);

		///
		TVector4& operator *= (const T& scalar);

		// --- scalar division --- 

		///
		TVector4 operator / (const T& scalar);

		///
		TVector4 &operator /= (const T& scalar);

		// --- dot product ---

		///
		T operator * (const TVector4& vector) const;

		///
		T getDistance(const TVector4& vector) const;

		///
		T getSquareDistance(const TVector4& vector) const;
		//@}

		/**	@name	Predicates
		*/
		//@{
			
		///
		bool operator == (const TVector4& vector) const;

		///
		bool operator != (const TVector4& vector) const;

		///
		bool isOrthogonalTo(TVector4& vector) const;
		//@}

		/**	@name	Debugging and Diagnostics
		*/
		//@{
		///
		bool isValid(void) const;

		///
		void dump(ostream& s = cout, unsigned long depth = 0) const;
		//@}

		/**	@name	Storers
		*/
		//@{

		///
		template <typename C>
		friend istream& operator >> (istream& s, TVector4<C>& vector);

		///
		template <typename C>
		friend ostream &operator << (ostream& s, const TVector4<C>& vector);
		//@}

		/**	@name	Attributes
		*/
		//@{

		///
		T x;

		///
		T y;

		///
		T z;

		///
		T h;
		//@}
	};

	template <class T>
	TVector4<T>::TVector4(void)
		:	x(0),
			y(0),
			z(0),
			h(0)
	{
	}

	template <class T>
	TVector4<T>::TVector4(const T* ptr)
	{
		if (ptr == 0)
			throw Exception::NullPointer(__FILE__, __LINE__);
		
		x = *ptr++;
		y = *ptr++;
		z = *ptr++;
		h = *ptr;
	}

	template <class T>
	TVector4<T>::TVector4(const TVector_<T>& v)
		:	x(v.x),
			y(v.y),
			z(v.z),
			h(v.h)
	{
	}

	template <class T>
	TVector4<T>::TVector4(const T& x,const T& y, const T& z, const T& h)
		: x(x),
			y(y),
			z(z),
			h(h)
	{
	}

	template <class T>
	TVector4<T>::TVector4(const TVector4<T>& v, bool /* deep */)
		:	x(v.x),
			y(v.y),
			z(v.z),
			h(v.h)
	{
	}

	template <class T>
	TVector4<T>::~TVector4(void)
	{
	}

	template <class T>
	BALL_INLINE
	TVector4<T>::operator TVector_<T>(void) const
	{
		return TVector_<T>(x, y, z, h);
	}

	template <class T>
	BALL_INLINE 
	void TVector4<T>::set(const T* ptr)
	{
		if (ptr == 0)
			throw Exception::NullPointer(__FILE__, __LINE__);
		
		x = *ptr++;
		y = *ptr++;
		z = *ptr++;
		h = *ptr;
	}

	template <class T>
	BALL_INLINE 
	void TVector4<T>::set(const TVector_<T>& v)
	{
		x = v.x;
		y = v.y;
		z = v.z;
		h = v.h;
	}

	template <class T>
	BALL_INLINE 
	void TVector4<T>::set(const T& x, const T& y, const T& z, const T& h)
	{
		x = x;
		y = y;
		z = z;
		h = h;
	}

	template <class T>
	BALL_INLINE 
	void TVector4<T>::set(const TVector4<T>& v, bool deep)
	{
		x = v.x;
		y = v.y;
		z = v.z;
		h = v.h;
	}

	template <class T>
	BALL_INLINE 
	TVector4<T>& TVector4<T>::operator = (const T* ptr)
	{
		if (ptr == 0)
			throw Exception::NullPointer(__FILE__, __LINE__);
		
		x = *ptr++;
		y = *ptr++;
		z = *ptr++;
		h = *ptr;

		return *this;
	}

	template <class T>
	BALL_INLINE 
	TVector4<T>& TVector4<T>::operator = (const TVector_<T>& v)
	{
		x = v.x;
		y = v.y;
		z = v.z;
		h = v.h;

		return *this;
	}

	template <class T>
	BALL_INLINE 
	TVector4<T>& TVector4<T>::operator = (const TVector4<T>& v)
	{
		x = v.x;
		y = v.y;
		z = v.z;
		h = v.h;

		return *this;
	}

	template <class T>
	BALL_INLINE 
	void TVector4<T>::get(T* ptr) const
	{
		if (ptr == 0)
			throw Exception::NullPointer(__FILE__, __LINE___);
		
		*ptr++ = x;
		*ptr++ = y;
		*ptr++ = z;
		*ptr   = h;
	}

	template <class T>
	BALL_INLINE 
	void TVector4<T>::get(TVector_<T>& v) const
	{
		v.x = x;
		v.y = y;
		v.z = z;
		v.h = h;
	}

	template <class T>
	BALL_INLINE 
	void TVector4<T>::get(T& rx, T& ry, T& rz, T& rh) const
	{
		rx = x;
		ry = y;
		rz = z;
		rh = h;
	}

	template <class T>
	BALL_INLINE 
	void TVector4<T>::get(TVector4<T>& v, bool deep) const
	{
		v.x = x;
		v.y = y;
		v.z = z;
		v.h = h;
	}

	template <class T>
	void TVector4<T>::swap(TVector4<T>& v)
	{
		T temp = x;
		x = v.x;
		v.x = temp;

		temp = y;
		y = v.y;
		v.y = temp;

		temp = z;
		z = v.z;
		v.z = temp;

		temp = h;
		h = v.h;
		v.h = temp;
	}

	template <class T>
	BALL_INLINE 
	T TVector4<T>::getLength(void) const
	{
		return (T)sqrt(x * x + y * y + z * z + h * h);
	}

	template <class T>
	BALL_INLINE 
	T TVector4<T>::getSquareLength(void) const
	{
		return (T)(x * x + y * y + z * z + h * h);
	}

	template <class T>
	void TVector4<T>::normalize(void)
	{
		T len = (T)sqrt(x * x + y * y + z * z + h * h);

		if (Maths::isZero(len))
			throw Exception::DivisionByZero(__FILE__, __LINE__);		
		
		x /= len;
		y /= len;
		z /= len;
		h /= len;
	}

	template <class T>
	BALL_INLINE 
	const TVector4<T>& TVector4<T>::getZero(void)
	{
		static TVector4<T> null4(0, 0, 0, 0);

		return null;
	}

	template <class T>
	BALL_INLINE 
	const TVector4<T>& TVector4<T>::getUnit(void)
	{
		static TVector4<T> unit_vector(1, 1, 1, 1);
		
		return unit_vector;
	}

	template <class T>
	BALL_INLINE 
	void TVector4<T>::fill(const T& value)
	{
		x = y = z = h = value;
	}

	template <class T>
	BALL_INLINE 
	T& TVector4<T>::operator [] (Index i)
	{
		if (i < 0)
			throw Exception::IndexUnderflow(__FILE__, __LINE__);

		if (i > 2)
			throw Exception::IndexOverflow(__FILE__, __LINE__);

		switch (i) {
			case 0: return x;
			case 1: return y;
			case 2: return z;
			case 3:
			default:
				return h;
		}
	}

	template <class T>
	BALL_INLINE 
	const T& TVector4<T>::operator [] (Index i) const
	{
		if (i < 0)
			throw Exception::IndexUnderflow(__FILE__, __LINE__);

		if (i > 2)
			throw Exception::IndexOverflow(__FILE__, __LINE__);
		
	
		switch (i) {
			case 0: return x;
			case 1: return y;
			case 2: return z;
			case 3:
			default:
				return h;
		}
	}

	template <class T>
	BALL_INLINE 
	TVector4<T> TVector4<T>::operator + (void) const
	{
		return *this;
	}

	template <class T>
	BALL_INLINE 
	TVector4<T> TVector4<T>::operator - (void) const
	{
		return TVector4<T>(-x, -y, -z, -h);
	}

	template <class T>
	BALL_INLINE 
	TVector4<T> operator + (const TVector4<T>& a, const TVector4<T>& b)
	{
		return TVector4<T>(a.x + b.x, a.y + b.y, a.z + b.z, a.h + b.h);
	}

	template <class T>
	BALL_INLINE 
	TVector4<T>& TVector4<T>::operator += (const TVector4<T>& v)
	{
		x += v.x;
		y += v.y;
		z += v.z;
		h += v.h;

		return *this;
	}

	template <class T>
	BALL_INLINE 
	TVector4<T> operator - (const TVector4<T>& a, const TVector4<T>& b)
	{
		return TVector4<T>(a.x - b.x, a.y - b.y, a.z - b.z, a.h - b.h);
	}

	template <class T>
	BALL_INLINE 
	TVector4<T>& TVector4<T>::operator -= (const TVector4<T> &v)
	{
		x -= v.x;
		y -= v.y;
		z -= v.z;
		h -= v.h;

		return *this;
	}

	template <class T>
	BALL_INLINE 
	TVector4<T> TVector4<T>::operator * (const T& scalar)
	{
		return TVector4<T>(x * scalar, y * scalar, z * scalar, h * scalar);
	}

	template <class T>
	BALL_INLINE 
	TVector4<T> operator * (const T& scalar, const TVector4<T>& v)
	{
		return TVector4<T>(scalar * v.x, scalar * v.y, scalar * v.z, scalar * v.h);
	}

	template <class T>
	BALL_INLINE 
	TVector4<T>& TVector4<T>::operator *= (const T &scalar)
	{
		x *= scalar;
		y *= scalar;
		z *= scalar;
		h *= scalar;

		return *this;
	}

	template <class T>
	TVector4<T>TVector4<T>::operator / (const T &scalar)
	{
		if (Maths::isZero(scalar))
			throw Exception::DivisionByZero(__FILE__, __LINE__);
		
		return TVector4<T>(x / scalar, y / scalar, z / scalar, h / scalar); 
	}

	template <class T>
	TVector4<T>& TVector4<T>::operator /= (const T& scalar)
	{
		if (Maths::isZero(scalar))
			throw Exception::DivisionByZero(__FILE__, __LINE__);
				
		x /= scalar;
		y /= scalar;
		z /= scalar;
		h /= scalar;

		return *this;
	}

	template <class T>
	BALL_INLINE 
	T TVector4<T>::operator * (const TVector4<T>& v) const
	{
		return (x * v.x + y * v.y + z * v.z + h * v.h);
	}

	template <class T>
	BALL_INLINE 
	T TVector4<T>::getDistance(const TVector4<T> &v) const
	{
		T da = x - v.x;
		T db = y - v.y;
		T dc = z - v.z;
		T dd = h - v.h;
		
		return (T)sqrt(da * da + db * db + dc * dc + dd * dd); 
	}

	template <class T>
	BALL_INLINE 
	T TVector4<T>::getSquareDistance(const TVector4<T> &v) const
	{
		T da = x - v.x;
		T db = y - v.y;
		T dc = z - v.z;
		T dd = h - v.h;
		
		return (da * da + db * db + dc * dc + dd * dd); 
	}

	template <class T>
	BALL_INLINE 
	bool TVector4<T>::operator == (const TVector4<T>& v) const
	{
		return (bool)(Maths::isEqual(x, v.x) && Maths::isEqual(y, v.y) 
									&& Maths::isEqual(z, v.z) && Maths::isEqual(h, v.h));
	}

	template <class T>
	BALL_INLINE 
	bool TVector4<T>::operator != (const TVector4<T>& v) const
	{
		return (bool)(Maths::isNotEqual(x, v.x) || Maths::isNotEqual(y, v.y)
									|| Maths::isNotEqual(z, v.z) || Maths::isNotEqual(h, v.h));
	}

	template <class T>
	BALL_INLINE 
	bool TVector4<T>::isOrthogonalTo(TVector4<T>& v) const
	{
		return Maths::isZero(*this * v);
	}

	template <class T>
	BALL_INLINE 
	bool TVector4<T>::isValid(void) const
	{
		return true;
	}

	template <class T>
	void TVector4<T>::dump(ostream& s, unsigned long depth) const
	{
		BALL_DUMP_STREAM_PREFIX(s);

		BALL_DUMP_HEADER(s, this, this);

		BALL_DUMP_DEPTH(s, depth);
		s << "x= " << x 
			<< ", y = " << y
			<< ", z = " << z
			<< ", h = " << h << endl;

		BALL_DUMP_STREAM_SUFFIX(s);
	}

	template <class T>
	istream& operator >> (istream& s, TVector4<T>& v)
	{
		s >> v.x >> v.y >> v.z >> v.h;

		return s;
	}

	template <class T>
	ostream& operator << (ostream& s, const TVector4<T>& v)
	{
		s << '(' <<v.x << ' ' << v.y << ' ' << v.z << ' ' << v.h << ')';
		
		return s;
	}


	///
	typedef TVector4<Real> Vector4;

} // namespace BALL

#endif // BALL_MATHS_VECTOR4_H

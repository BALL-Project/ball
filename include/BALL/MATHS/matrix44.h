// $Id: matrix44.h,v 1.7 2000/02/17 00:30:43 oliver Exp $

#ifndef BALL_MATHS_MATRIX44_H
#define BALL_MATHS_MATRIX44_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#include <math.h>
#include <iostream>

#ifndef BALL_MATHS_ANGLE_H
#	include <BALL/MATHS/angle.h>
#endif

#ifndef BALL_MATHS_VECTOR3_H
#	include <BALL/MATHS/vector3.h>
#endif

#ifndef BALL_MATHS_VECTOR4_H
#	include <BALL/MATHS/vector4.h>
#endif

namespace BALL 
{

	/**		@name	4x4 Matrix
				@memo matrix representing transformations: class \Ref{TMatrix4x4} and class \Ref{Matrix4x4}
	*/
	//@{
	template <typename T>
	class TMatrix4x4;

	template <typename T>
	std::istream& operator >> (std::istream& s, TMatrix4x4<T>& m);

	template <typename T>
	std::ostream& operator << (std::ostream& s, const TMatrix4x4<T>& m);
	
	/**	Generic 4x4 Matrix Class.
      {\bf Definition:} \URL{BALL/MATHS/.h}
      \\
 	*/
	template <class T>
	class TMatrix4x4
	{
		public:
	
		BALL_CREATE(TMatrix4x4)

		/**	@name	Constructors and Destructors
		*/
		//@{

		///
		TMatrix4x4();

		///
		TMatrix4x4(const T* ptr);

		///
		TMatrix4x4(const T* ptr[4]);

		///
		TMatrix4x4(const TMatrix4x4& m ,bool deep = true);

		///
		TMatrix4x4
			(const TVector4<T>& col1, const TVector4<T>& col2,
			 const TVector4<T>& col3, const TVector4<T>& col4);

		///
		TMatrix4x4
			(const T& m11, const T& m12, const T& m13, const T& m14, 
			 const T& m21, const T& m22, const T& m23, const T& m24, 
			 const T& m31, const T& m32, const T& m33, const T& m34, 
			 const T& m41, const T& m42, const T& m43, const T& m44);

		///
		virtual ~TMatrix4x4()
		{
		}
		//@}

		/**	@name	Assignment
		*/
		//@{
		///
		void set(register const T* ptr);

		///
		void set(const T* ptr[4]);

		///
		void set(const TMatrix4x4& m, bool deep = true);

		///
		void set
			(const TVector4<T>& col1, const TVector4<T>& col2,
			 const TVector4<T>& col3, const TVector4<T>& col4);

		///
		void set
			(const T& m11, const T& m12, const T& m13, const T& m14, 
			 const T& m21, const T& m22, const T& m23, const T& m24, 
			 const T& m31, const T& m32, const T& m33, const T& m34, 
			 const T& m41, const T& m42, const T& m43, const T& m44);

		///
		TMatrix4x4& operator = (register const T* ptr);

		///
		TMatrix4x4& operator = (const T* ptr[4]);

		///
		TMatrix4x4& operator = (const TMatrix4x4& m);

		///
		void get(T* ptr) const;

		///
		void get(T* ptr[4]) const;

		///
		void get(TMatrix4x4& m, bool deep = true) const;

		///
		void get
			(TVector4<T>& col1, TVector4<T>& col2,
			 TVector4<T>& col3, TVector4<T>& col4) const;

		///
		void get
			(T& m11, T& m12, T& m13, T& m14, 
			 T& m21, T& m22, T& m23, T& m24, 
			 T& m31, T& m32, T& m33, T& m34, 
			 T& m41, T& m42, T& m43, T& m44) const;

		///
		void swap(TMatrix4x4& m);

		//@}

		/**	@name	Accessors
		*/
		//@{

		///
		T getTrace() const;

		///
		static const TMatrix4x4& getZero();

		///
		static const TMatrix4x4& getIdentity();

		///
		void setIdentity();

		///
		void fill(const T& t = (T)1);

		///
		void transpose();

		///
		TVector4<T> getRow(Index row);

		///
		TVector4<T> getColumn(Index col);

		///
		TVector4<T> getDiagonal() const;
		
		///
		T& operator ()(Index row, Index col);

		///
		const T& operator ()(Index row, Index col) const;

		/// 
		TMatrix4x4 operator + () const;

		/// 
		TMatrix4x4 operator - () const;

		///
		TMatrix4x4 operator + (const TMatrix4x4& m) const;

		///
		TMatrix4x4& operator += (const TMatrix4x4& m);

		///
		TMatrix4x4 operator - (const TMatrix4x4& m) const;

		///
		TMatrix4x4& operator -= (const TMatrix4x4& m);

		///
		TMatrix4x4 operator * (const T& scalar) const;

		///
		TMatrix4x4& operator *= (const T& scalar);

		///
		TMatrix4x4 operator / (const T& scalar) const;

		///
		TMatrix4x4& operator /= (const T& scalar);

		///
		TMatrix4x4 operator * (const TMatrix4x4& m) const;

		///
		TMatrix4x4& operator *= (const TMatrix4x4& m);

		///
		TVector4<T> operator * (const TVector4<T> &) const;

		///
		bool invert(TMatrix4x4& inverse) const;

		///
		bool invert();

		///
		T getDeterminant() const;

		///
		void translate(const T &x, const T &y, const T &z);

		///
		void translate(const TVector3<T>& v);

		///
		void setTranslation(const T& x, const T& y, const T& z);

		///
		void setTranslation(const TVector3<T>& v);

		///
		static TMatrix4x4& getTranslation(const T& x, const T& y, const T& z, TMatrix4x4& m);

		///
		static TMatrix4x4& getTranslation(const TVector3<T>& v,TMatrix4x4& m);

		///
		void scale(const T& x_scale, const T& y_scale, const T& z_scale);

		///
		void scale(const T& scale);

		///
		void scale(const TVector3<T>& v);

		///
		void setScale(const T& x_scale, const T& y_scale, const T& z_scale);

		///
		void setScale(const T& scale);

		///
		void setScale(const TVector3<T> &);

		///
		static TMatrix4x4& getScale(const T& x_scale, const T& y_scale, const T& z_scale, TMatrix4x4& m);

		///
		static TMatrix4x4& getScale(const T& scale, TMatrix4x4& m);

		///
		static TMatrix4x4& getScale(const TVector3<T>& v, TMatrix4x4& m);

		///
		static TMatrix4x4& getScale(const TVector4<T>& v, TMatrix4x4& m);

		///
		void rotateX(const Angle& phi);

		///
		void setRotationX(const Angle& phi);

		///
		static TMatrix4x4& getRotationX(const Angle& phi, TMatrix4x4& m);

		///
		void rotateY(const Angle& phi);

		///
		void setRotationY(const Angle& phi);

		///
		static TMatrix4x4& getRotationY(const Angle& phi, TMatrix4x4& m);

		///
		void rotateZ(const Angle &phi);

		///
		void setRotationZ(const Angle &phi);

		///
		static TMatrix4x4& getRotationZ(const Angle& phi, TMatrix4x4& m);

		///
		void rotate(const Angle& phi, const T& axis_x, const T& axis_y, const T& axis_z);

		///
		void rotate(const Angle& phi, const TVector3<T>& axis);

		///
		void rotate(const Angle& phi, const TVector4<T>& axis);

		///
		void setRotation(const Angle& phi, const T& axis_x, const T& axis_y, const T& axis_z); 

		///
		void setRotation(const Angle& phi, const TVector3<T>& axis);

		///
		void setRotation(const Angle& phi, const TVector4<T>& axis);

		///
		static TMatrix4x4& getRotation
			(const Angle& phi, const T& axis_x,
			 const T& axis_y, const T& axis_z,
			 TMatrix4x4& m);

		///
		static TMatrix4x4& getRotation
			(const Angle& phi, const TVector3<T>& axis, TMatrix4x4& m);

		///
		static TMatrix4x4 &getRotation
			(const Angle& phi, const TVector4<T>& axis, TMatrix4x4& m);
		//@}

		/**	@name	Predicates
		*/
		//@{

		///
		bool operator == (const TMatrix4x4& m) const; 

		///
		bool operator != (const TMatrix4x4& m) const;

		///
		bool isIdentity() const;

		///
		bool isRegular() const;

		///
		bool isSingular() const;

		///
		bool isSymmetric() const;

		///
		bool isLowerTriangular() const;

		///
		bool isUpperTriangular() const;

		///
		bool isDiagonal() const;
		//@}

		/**	@name	Debugging and Diagnostics
		*/
		//@{

		///
		bool isValid() const;

		///
		void dump(std::ostream& s = std::cout, Size depth = 0) const;
		//@}


		/**	@name	Attributes
		*/
		//@{

		///
		T m11;
		///
		T m12;
		//
		T m13;
		///
		T m14;

		///
		T m21;
		///
		T m22;
		//
		T m23;
		///
		T m24;

		///
		T m31;
		///
		T m32;
		//
		T m33;
		///
		T m34;

		///
		T m41;
		///
		T m42;
		//
		T m43;
		///
		T m44;
		//@}

		private:

		TMatrix4x4(bool);

		TMatrix4x4(bool, const TMatrix4x4& m);

		TMatrix4x4
			(bool,
			 const T& m11, const T& m12, const T& m13, const T& m14, 
			 const T& m21, const T& m22, const T& m23, const T& m24, 
			 const T& m31, const T& m32, const T& m33, const T& m34, 
			 const T& m41, const T& m42, const T& m43, const T& m44);

		void initializeComponentPointers_()
		{
			register T **ptr = (T **)comp_ptr_;

			*ptr++ = &m11;
			*ptr++ = &m12;
			*ptr++ = &m13;
			*ptr++ = &m14;
			*ptr++ = &m21;
			*ptr++ = &m22;
			*ptr++ = &m23;
			*ptr++ = &m24;
			*ptr++ = &m31;
			*ptr++ = &m32;
			*ptr++ = &m33;
			*ptr++ = &m34;
			*ptr++ = &m41;
			*ptr++ = &m42;
			*ptr++ = &m43;
			*ptr   = &m44;
		}

		// pointers to the components of the matrix 
		T* comp_ptr_[4][4];
	};

	template <class T>
	TMatrix4x4<T>::TMatrix4x4()
		:	m11(0), m12(0), m13(0), m14(0), 
			m21(0), m22(0), m23(0), m24(0), 
			m31(0), m32(0), m33(0), m34(0), 
			m41(0), m42(0), m43(0), m44(0)
	{
		initializeComponentPointers_();
	}

	template <class T>
	TMatrix4x4<T>::TMatrix4x4(register const T* ptr)
	{
		if (ptr == 0)
		{
			throw Exception::NullPointer(__FILE__, __LINE__);
		}
		
		m11 = *ptr++; 
		m12 = *ptr++; 
		m13 = *ptr++; 
		m14 = *ptr++; 
		m21 = *ptr++; 
		m22 = *ptr++; 
		m23 = *ptr++; 
		m24 = *ptr++; 
		m31 = *ptr++; 
		m32 = *ptr++; 
		m33 = *ptr++; 
		m34 = *ptr++; 
		m41 = *ptr++; 
		m42 = *ptr++; 
		m43 = *ptr++; 
		m44 = *ptr; 

		initializeComponentPointers_();
	}

	template <class T>
	TMatrix4x4<T>::TMatrix4x4(const T* array_ptr[4])
	{
		if (array_ptr == 0)
			throw Exception::NullPointer(__FILE__, __LINE__);
		
		register const T *ptr = *array_ptr;
			
		m11 = *ptr++; 
		m12 = *ptr++; 
		m13 = *ptr++; 
		m14 = *ptr++; 
		m21 = *ptr++; 
		m22 = *ptr++; 
		m23 = *ptr++; 
		m24 = *ptr++; 
		m31 = *ptr++; 
		m32 = *ptr++; 
		m33 = *ptr++; 
		m34 = *ptr++; 
		m41 = *ptr++; 
		m42 = *ptr++; 
		m43 = *ptr++; 
		m44 = *ptr; 

		initializeComponentPointers_();
	}

	template <class T>
	TMatrix4x4<T>::TMatrix4x4(const TMatrix4x4<T>& m, bool /* deep */)
		:	m11(m.m11), m12(m.m12), m13(m.m13), m14(m.m14), 
			m21(m.m21), m22(m.m22), m23(m.m23), m24(m.m24), 
			m31(m.m31), m32(m.m32), m33(m.m33), m34(m.m34), 
			m41(m.m41), m42(m.m42), m43(m.m43), m44(m.m44)
	{
		initializeComponentPointers_();
	}


	template <class T>
	TMatrix4x4<T>::TMatrix4x4
		(const TVector4<T>& col1, const TVector4<T>& col2,
		 const TVector4<T>& col3,const TVector4<T>& col4)
		:	m11(col1.x), m12(col1.y), m13(col1.z), m14(col1.h), 
			m21(col2.x), m22(col2.y), m23(col2.z), m24(col2.h), 
			m31(col3.x), m32(col3.y), m33(col3.z), m34(col3.h), 
			m41(col4.x), m42(col4.y), m43(col4.z), m44(col4.h)
	{
		initializeComponentPointers_();
	}

	template <class T>
	TMatrix4x4<T>::TMatrix4x4
		(const T& m11, const T& m12, const T& m13, const T& m14, 
		 const T& m21, const T& m22, const T& m23, const T& m24, 
		 const T& m31, const T& m32, const T& m33, const T& m34, 
		 const T& m41, const T& m42, const T& m43, const T& m44)
		:	m11(m11), m12(m12), m13(m13), m14(m14), 
			m21(m21), m22(m22), m23(m23), m24(m24), 
			m31(m31), m32(m32), m33(m33), m34(m34), 
			m41(m41), m42(m42), m43(m43), m44(m44)
	{
		initializeComponentPointers_();
	}

	template <class T>
	TMatrix4x4<T>::TMatrix4x4(bool)
	{
	}

	template <class T>
	TMatrix4x4<T>::TMatrix4x4(bool, const TMatrix4x4<T>& m)
		:	m11(m.m11), m12(m.m12), m13(m.m13), m14(m.m14), 
			m21(m.m21), m22(m.m22), m23(m.m23), m24(m.m24), 
			m31(m.m31), m32(m.m32), m33(m.m33), m34(m.m34), 
			m41(m.m41), m42(m.m42), m43(m.m43), m44(m.m44)
	{
	}

	template <class T>
	TMatrix4x4<T>::TMatrix4x4
		(bool,
		 const T& m11, const T& m12, const T& m13, const T& m14, 
		 const T& m21, const T& m22, const T& m23, const T& m24, 
		 const T& m31, const T& m32, const T& m33, const T& m34, 
		 const T& m41, const T& m42, const T& m43, const T& m44)
		:	m11(m11), m12(m12), m13(m13), m14(m14), 
			m21(m21), m22(m22), m23(m23), m24(m24), 
			m31(m31), m32(m32), m33(m33), m34(m34), 
			m41(m41), m42(m42), m43(m43), m44(m44)
	{
	}

	template <class T>
	void TMatrix4x4<T>::set(const T* ptr)
	{
		if (ptr == 0)
			throw Exception::NullPointer(__FILE__, __LINE__);

		m11 = *ptr++; 
		m12 = *ptr++; 
		m13 = *ptr++; 
		m14 = *ptr++; 
		m21 = *ptr++; 
		m22 = *ptr++; 
		m23 = *ptr++; 
		m24 = *ptr++; 
		m31 = *ptr++; 
		m32 = *ptr++; 
		m33 = *ptr++; 
		m34 = *ptr++; 
		m41 = *ptr++; 
		m42 = *ptr++; 
		m43 = *ptr++; 
		m44 = *ptr; 
	}

	template <class T>
	void TMatrix4x4<T>::set(const T* array_ptr[4])
	{
    if (array_ptr == 0)
      throw Exception::NullPointer(__FILE__, __LINE__);
 		
		register const T *ptr = *array_ptr;

		m11 = *ptr++; 
		m12 = *ptr++; 
		m13 = *ptr++; 
		m14 = *ptr++; 
		m21 = *ptr++; 
		m22 = *ptr++; 
		m23 = *ptr++; 
		m24 = *ptr++; 
		m31 = *ptr++; 
		m32 = *ptr++; 
		m33 = *ptr++; 
		m34 = *ptr++; 
		m41 = *ptr++; 
		m42 = *ptr++; 
		m43 = *ptr++; 
		m44 = *ptr; 
	}

	template <class T>
	void TMatrix4x4<T>::set(const TMatrix4x4<T>& m, bool /* deep */)
	{
		m11 = m.m11; 
		m12 = m.m12;
		m13 = m.m13; 
		m14 = m.m14; 
		m21 = m.m21; 
		m22 = m.m22; 
		m23 = m.m23; 
		m24 = m.m24; 
		m31 = m.m31; 
		m32 = m.m32; 
		m33 = m.m33; 
		m34 = m.m34; 
		m41 = m.m41; 
		m42 = m.m42; 
		m43 = m.m43; 
		m44 = m.m44;
	}

	template <class T>
	void TMatrix4x4<T>::set
		(const TVector4<T>& col1, const TVector4<T>& col2,
		 const TVector4<T>& col3, const TVector4<T>& col4)
	{
		m11 = col1.x; 
		m12 = col1.y; 
		m13 = col1.z; 
		m14 = col1.h; 
		m21 = col2.x; 
		m22 = col2.y; 
		m23 = col2.z; 
		m24 = col2.h; 
		m31 = col3.x; 
		m32 = col3.y; 
		m33 = col3.z; 
		m34 = col3.h; 
		m41 = col4.x; 
		m42 = col4.y; 
		m43 = col4.z; 
		m44 = col4.h;
	}

	template <class T>
	void TMatrix4x4<T>::set
		(const T& c11, const T& c12, const T& c13, const T& c14, 
		 const T& c21, const T& c22, const T& c23, const T& c24, 
		 const T& c31, const T& c32, const T& c33, const T& c34, 
		 const T& c41, const T& c42, const T& c43, const T& c44)
	{
		m11 = c11;
		m12 = c12;
		m13 = c13;
		m14 = c14;
		m21 = c21;
		m22 = c22;
		m23 = c23;
		m24 = c24;
		m31 = c31;
		m32 = c32;
		m33 = c33;
		m34 = c34;
		m41 = c41;
		m42 = c42;
		m43 = c43;
		m44 = c44;
	}

	template <class T>
	BALL_INLINE 
	TMatrix4x4<T>& TMatrix4x4<T>::operator = (const T* ptr)
	{
		set(ptr);

		return *this;
	}

	template <class T>
	BALL_INLINE TMatrix4x4<T>& TMatrix4x4<T>::operator = (const T* array_ptr[4])
	{
		set(array_ptr);

		return *this;
	}

	template <class T>
	BALL_INLINE 
	TMatrix4x4<T>& TMatrix4x4<T>::operator = (const TMatrix4x4<T>& m)
	{
		set(m);

		return *this;
	}

	template <class T>
	void TMatrix4x4<T>::get(T* ptr) const
	{
    if (ptr == 0)
      throw Exception::NullPointer(__FILE__, __LINE__);

		*ptr++ = m11; 
		*ptr++ = m12; 
		*ptr++ = m13; 
		*ptr++ = m14; 
		*ptr++ = m21; 
		*ptr++ = m22; 
		*ptr++ = m23; 
		*ptr++ = m24; 
		*ptr++ = m31; 
		*ptr++ = m32; 
		*ptr++ = m33; 
		*ptr++ = m34; 
		*ptr++ = m41; 
		*ptr++ = m42; 
		*ptr++ = m43; 
		*ptr   = m44; 
	}

	template <class T>
	void TMatrix4x4<T>::get(T* array_ptr[4]) const
	{
    if (array_ptr == 0)
       throw Exception::NullPointer(__FILE__, __LINE__);
 
		register T *ptr = *array_ptr;

		*ptr++ = m11; 
		*ptr++ = m12; 
		*ptr++ = m13; 
		*ptr++ = m14; 
		*ptr++ = m21; 
		*ptr++ = m22; 
		*ptr++ = m23; 
		*ptr++ = m24; 
		*ptr++ = m31; 
		*ptr++ = m32; 
		*ptr++ = m33; 
		*ptr++ = m34; 
		*ptr++ = m41; 
		*ptr++ = m42; 
		*ptr++ = m43; 
		*ptr   = m44; 
	}

	template <class T>
	void TMatrix4x4<T>::get(TMatrix4x4<T>& m, bool deep) const
	{
		m.set(*this, deep);
	}

	template <class T>
	void TMatrix4x4<T>::get
		(TVector4<T>& col1, TVector4<T>& col2,
		 TVector4<T>& col3, TVector4<T>& col4) const
	{
		col1.x = m11; 
		col1.y = m12; 
		col1.z = m13; 
		col1.h = m14; 
		col2.x = m21; 
		col2.y = m22; 
		col2.z = m23; 
		col2.h = m24; 
		col3.x = m31; 
		col3.y = m32; 
		col3.z = m33; 
		col3.h = m34; 
		col4.x = m41; 
		col4.y = m42; 
		col4.z = m43; 
		col4.h = m44;
	}

	template <class T>
	void TMatrix4x4<T>::get
		(T& c11, T& c12, T& c13, T& c14, 
		 T& c21, T& c22, T& c23, T& c24, 
		 T& c31, T& c32, T& c33, T& c34, 
		 T& c41, T& c42, T& c43, T& c44) const
	{
		c11 = m11;
		c12 = m12;
		c13 = m13;
		c14 = m14;
		c21 = m21;
		c22 = m22;
		c23 = m23;
		c24 = m24;
		c31 = m31;
		c32 = m32;
		c33 = m33;
		c34 = m34;
		c41 = m41;
		c42 = m42;
		c43 = m43;
		c44 = m44;
	}

	template <class T>
	BALL_INLINE 
	T TMatrix4x4<T>::getTrace() const
	{
		return (m11 + m22 + m33 + m44);
	}

	template <class T>
	BALL_INLINE 
	const TMatrix4x4<T>& TMatrix4x4<T>::getZero()
	{
		static TMatrix4x4<T> null_matrix
			(0, 0, 0, 0,
			 0, 0, 0, 0,
			 0, 0, 0, 0,
			 0, 0, 0, 0);
		
		return null_matrix;
	}

	template <class T>
	BALL_INLINE
	void TMatrix4x4<T>::setIdentity()
	{
		m12 = m13 = m14 = m21 = m23 = m24 = m31 = m32 = m34 = m41 = m42 = m43 = 0;
		m11 = m22 = m33 = m44 = (T)1;
	}

	template <class T>
	BALL_INLINE 
	const TMatrix4x4<T>& TMatrix4x4<T>::getIdentity()
	{
		static TMatrix4x4<T> identity
			(1, 0, 0, 0,
			 0, 1, 0, 0,
			 0, 0, 1, 0,
			 0, 0, 0, 1);

		return identity;
	}

	template <class T>
	void TMatrix4x4<T>::fill(const T& t)
	{
			m11 = m12 = m13 = m14 
		= m21 = m22 = m23 = m24 
		= m31 = m32 = m33 = m34
		= m41 = m42 = m43 = m44
		= t;
	}

	template <class T>
	void TMatrix4x4<T>::swap(TMatrix4x4<T>& m)
	{
		T tmp = m11; m11 = m.m11; m.m11 = tmp;
			tmp = m12; m12 = m.m12; m.m12 = tmp;
			tmp = m13; m13 = m.m13; m.m13 = tmp;
			tmp = m14; m14 = m.m14; m.m14 = tmp;
			tmp = m21; m21 = m.m21; m.m21 = tmp;
			tmp = m22; m22 = m.m22; m.m22 = tmp;
			tmp = m23; m23 = m.m23; m.m23 = tmp;
			tmp = m24; m24 = m.m24; m.m24 = tmp;
			tmp = m31; m31 = m.m31; m.m31 = tmp;
			tmp = m32; m32 = m.m32; m.m32 = tmp;
			tmp = m33; m33 = m.m33; m.m33 = tmp;
			tmp = m34; m34 = m.m34; m.m34 = tmp;
			tmp = m41; m41 = m.m41; m.m41 = tmp;
			tmp = m42; m42 = m.m42; m.m42 = tmp;
			tmp = m43; m43 = m.m43; m.m43 = tmp;
			tmp = m44; m44 = m.m44; m.m44 = tmp;
	}

	template <class T>
	void TMatrix4x4<T>::transpose()
	{
		T tmp = m12;
		m12 = m21;
		m21 = tmp;

		tmp  = m13;
		m13 = m31;
		m31 = tmp;

		tmp  = m14;
		m14 = m41;
		m41 = tmp;

		tmp  = m23;
		m23 = m32;
		m32 = tmp;

		tmp  = m24;
		m24 = m42;
		m42 = tmp;

		tmp  = m34;
		m34 = m43;
		m43 = tmp;
	}

	template <class T>
	TVector4<T> TMatrix4x4<T>::getRow(Index row)
	{
		if (row < 0)
			throw Exception::IndexUnderflow(__FILE__, __LINE__, row, 0);

		if (row > 3)
			throw Exception::IndexOverflow(__FILE__, __LINE__, row, 3);

		T **ptr = (T **)comp_ptr_[row];

		return TVector4<T> (*ptr[0], *ptr[1], *ptr[2], *ptr[3]);
	}

	template <class T>
	TVector4<T> TMatrix4x4<T>::getColumn(Index col)
	{
		if (col < 0)
			throw Exception::IndexUnderflow(__FILE__, __LINE__, row, 0);

		if (col > 3)
			throw Exception::IndexOverflow(__FILE__, __LINE__, row, 3);

		return TVector4<T> (*comp_ptr_[0][col], *comp_ptr_[1][col], *comp_ptr_[2][col], *comp_ptr_[3][col]);
	}

	template <class T>
	TVector4<T>TMatrix4x4<T>::getDiagonal() const
	{
		return TVector4<T>(m11, m22, m33, m44);
	}

	template <class T>
	BALL_INLINE  
	T& TMatrix4x4<T>::operator () (Index row, Index col)
	{
    if (row < 0)
      throw Exception::IndexUnderflow(__FILE__, __LINE__, row, 0);

    if (row > 3)
      throw Exception::IndexOverflow(__FILE__, __LINE__, row, 3);

    if (col < 0)
      throw Exception::IndexUnderflow(__FILE__, __LINE__, row, 0);

    if (col > 3)
      throw Exception::IndexOverflow(__FILE__, __LINE__, row, 3);

		return *comp_ptr_[row][col];
	}

	template <class T>
	BALL_INLINE 
	const T& TMatrix4x4<T>::operator () (Index row, Index col) const
	{
    if (row < 0)
      throw Exception::IndexUnderflow(__FILE__, __LINE__, row, 0);

    if (row > 3)
      throw Exception::IndexOverflow(__FILE__, __LINE__, row, 3);

    if (col < 0)
      throw Exception::IndexUnderflow(__FILE__, __LINE__, row, 0);

    if (col > 3)
      throw Exception::IndexOverflow(__FILE__, __LINE__, row, 3);

		return *comp_ptr_[row][col];
	}

	template <class T>
	BALL_INLINE 
	TMatrix4x4<T> TMatrix4x4<T>::operator + () const
	{
		return *this;
	}

	template <class T>
	BALL_INLINE TMatrix4x4<T>
	TMatrix4x4<T>::operator - () const
	{
		return TMatrix4x4<T>
			(-m11, -m12, -m13, -m14,
			 -m21, -m22, -m23, -m24,
			 -m31, -m32, -m33, -m34,
			 -m41, -m42, -m43, -m44);
	}

	template <class T>
	TMatrix4x4<T> TMatrix4x4<T>::operator + (const TMatrix4x4<T>& m) const
	{
		return TMatrix4x4<T>
			(m11 + m.m11, m12 + m.m12, m13 + m.m13, m14 + m.m14,
			 m21 + m.m21, m22 + m.m22, m23 + m.m23, m24 + m.m24,
			 m31 + m.m31, m32 + m.m32, m33 + m.m33, m34 + m.m34,
			 m41 + m.m41, m42 + m.m42, m43 + m.m43, m44 + m.m44);
	}

	template <class T>
	TMatrix4x4<T>& TMatrix4x4<T>::operator += (const TMatrix4x4<T>& m)
	{
		m11 += m.m11;
		m12 += m.m12;
		m13 += m.m13;
		m14 += m.m14;
		m21 += m.m21;
		m22 += m.m22;
		m23 += m.m23;
		m24 += m.m24;
		m31 += m.m31;
		m32 += m.m32;
		m33 += m.m33;
		m34 += m.m34;
		m41 += m.m41;
		m42 += m.m42;
		m43 += m.m43;
		m44 += m.m44;

		return *this;
	}

	template <class T>
	TMatrix4x4<T> TMatrix4x4<T>::operator - (const TMatrix4x4<T>& m) const
	{
		return TMatrix4x4<T>
			(m11 - m.m11, m12 - m.m12, m13 - m.m13, m14 - m.m14,
			 m21 - m.m21, m22 - m.m22, m23 - m.m23, m24 - m.m24,
			 m31 - m.m31, m32 - m.m32, m33 - m.m33, m34 - m.m34,
			 m41 - m.m41, m42 - m.m42, m43 - m.m43, m44 - m.m44);
	}

	template <class T>
	TMatrix4x4<T>& TMatrix4x4<T>::operator -= (const TMatrix4x4<T>& m)
	{
		m11 -= m.m11;
		m12 -= m.m12;
		m13 -= m.m13;
		m14 -= m.m14;
		m21 -= m.m21;
		m22 -= m.m22;
		m23 -= m.m23;
		m24 -= m.m24;
		m31 -= m.m31;
		m32 -= m.m32;
		m33 -= m.m33;
		m34 -= m.m34;
		m41 -= m.m41;
		m42 -= m.m42;
		m43 -= m.m43;
		m44 -= m.m44;

		return *this;
	}

	template <class T>
	TMatrix4x4<T> TMatrix4x4<T>::operator * (const T& scalar) const
	{
		return TMatrix4x4<T>
			(m11 * scalar, m12 * scalar, m13 * scalar, m14 * scalar,
			 m21 * scalar, m22 * scalar, m23 * scalar, m24 * scalar,
			 m31 * scalar, m32 * scalar, m33 * scalar, m34 * scalar,
			 m41 * scalar, m42 * scalar, m43 * scalar, m44 * scalar);
	}

	template <class T>
	TMatrix4x4<T> operator * (const T& scalar, const TMatrix4x4<T>& m)
	{
		return TMatrix4x4<T>
			(scalar * m.m11, scalar * m.m12, scalar * m.m13, scalar * m.m14,
			 scalar * m.m21, scalar * m.m22, scalar * m.m23, scalar * m.m24,
			 scalar * m.m31, scalar * m.m32, scalar * m.m33, scalar * m.m34,
			 scalar * m.m41, scalar * m.m42, scalar * m.m43, scalar * m.m44);
	}

	template <class T>
	TMatrix4x4<T>& TMatrix4x4<T>::operator *= (const T& scalar)
	{
		m11 *= scalar;
		m12 *= scalar;
		m13 *= scalar;
		m14 *= scalar;
		m21 *= scalar;
		m22 *= scalar;
		m23 *= scalar;
		m24 *= scalar;
		m31 *= scalar;
		m32 *= scalar;
		m33 *= scalar;
		m34 *= scalar;
		m41 *= scalar;
		m42 *= scalar;
		m43 *= scalar;
		m44 *= scalar;

		return *this;
	}

	template <class T>
	TVector3<T> operator *(const TMatrix4x4<T>& matrix, const TVector3<T>& vector)
	{
		return TVector3<T>
			(matrix.m11 * vector.x + matrix.m12 * vector.y + matrix.m13 * vector.z + matrix.m14,
			 matrix.m21 * vector.x + matrix.m22 * vector.y + matrix.m23 * vector.z + matrix.m24,
			 matrix.m31 * vector.x + matrix.m32 * vector.y + matrix.m33 * vector.z + matrix.m34);
	}



	template <class T>
	BALL_INLINE 
	TMatrix4x4<T>TMatrix4x4<T>::operator / (const T& scalar) const
	{
		if (scalae == (T)0)
		{
			throw Exception::DivisionByZero(__FILE__, __LINE__);
		}
		
		return (*this * ((T)1 / scalar));
	}

	template <class T>
	BALL_INLINE 
	TMatrix4x4<T>& TMatrix4x4<T>::operator /= (const T& scalar)
	{
		if (scalae == (T)0)
		{
			throw Exception::DivisionByZero(__FILE__, __LINE__);
		}
		
		return (*this *= (T)1 / scalar);
	}

	template <class T>
	TMatrix4x4<T> TMatrix4x4<T>::operator * (const TMatrix4x4<T>& m) const
	{
		return TMatrix4x4<T>
				(m11 * m.m11 + m12 * m.m21 + m13 * m.m31 + m14 * m.m41,
				 m11 * m.m12 + m12 * m.m22 + m13 * m.m32 + m14 * m.m42,
				 m11 * m.m13 + m12 * m.m23 + m13 * m.m33 + m14 * m.m43,
				 m11 * m.m14 + m12 * m.m24 + m13 * m.m34 + m14 * m.m44,

				 m21 * m.m11 + m22 * m.m21 + m23 * m.m31 + m24 * m.m41,
				 m21 * m.m12 + m22 * m.m22 + m23 * m.m32 + m24 * m.m42,
				 m21 * m.m13 + m22 * m.m23 + m23 * m.m33 + m24 * m.m43,
				 m21 * m.m14 + m22 * m.m24 + m23 * m.m34 + m24 * m.m44,
		 
				 m31 * m.m11 + m32 * m.m21 + m33 * m.m31 + m34 * m.m41,
		 		 m31 * m.m12 + m32 * m.m22 + m33 * m.m32 + m34 * m.m42,
		 		 m31 * m.m13 + m32 * m.m23 + m33 * m.m33 + m34 * m.m43,
		 		 m31 * m.m14 + m32 * m.m24 + m33 * m.m34 + m34 * m.m44,
		 
				 m41 * m.m11 + m42 * m.m21 + m43 * m.m31 + m44 * m.m41,
				 m41 * m.m12 + m42 * m.m22 + m43 * m.m32 + m44 * m.m42,
				 m41 * m.m13 + m42 * m.m23 + m43 * m.m33 + m44 * m.m43,
				 m41 * m.m14 + m42 * m.m24 + m43 * m.m34 + m44 * m.m44);
	}

	template <class T>
	TMatrix4x4<T>& TMatrix4x4<T>::operator *= (const TMatrix4x4<T>& m)
	{
		set(m11 * m.m11 + m12 * m.m21 + m13 * m.m31 + m14 * m.m41,
				m11 * m.m12 + m12 * m.m22 + m13 * m.m32 + m14 * m.m42,
				m11 * m.m13 + m12 * m.m23 + m13 * m.m33 + m14 * m.m43,
				m11 * m.m14 + m12 * m.m24 + m13 * m.m34 + m14 * m.m44,
 
        m21 * m.m11 + m22 * m.m21 + m23 * m.m31 + m24 * m.m41,
        m21 * m.m12 + m22 * m.m22 + m23 * m.m32 + m24 * m.m42,
        m21 * m.m13 + m22 * m.m23 + m23 * m.m33 + m24 * m.m43,
        m21 * m.m14 + m22 * m.m24 + m23 * m.m34 + m24 * m.m44,

        m31 * m.m11 + m32 * m.m21 + m33 * m.m31 + m34 * m.m41,
        m31 * m.m12 + m32 * m.m22 + m33 * m.m32 + m34 * m.m42,
        m31 * m.m13 + m32 * m.m23 + m33 * m.m33 + m34 * m.m43,
        m31 * m.m14 + m32 * m.m24 + m33 * m.m34 + m34 * m.m44,

        m41 * m.m11 + m42 * m.m21 + m43 * m.m31 + m44 * m.m41,
        m41 * m.m12 + m42 * m.m22 + m43 * m.m32 + m44 * m.m42,
        m41 * m.m13 + m42 * m.m23 + m43 * m.m33 + m44 * m.m43,
        m41 * m.m14 + m42 * m.m24 + m43 * m.m34 + m44 * m.m44);

		return *this;
	}

	template <class T>
	TVector4<T> TMatrix4x4<T>::operator * (const TVector4<T>& v) const
	{
		return TVector4<T>
			(m11 * v.x + m12 * v.y + m13 * v.z + m14 * v.h,
			 m21 * v.x + m22 * v.y + m23 * v.z + m24 * v.h,
			 m31 * v.x + m32 * v.y + m33 * v.z + m34 * v.h,
			 m41 * v.x + m42 * v.y + m43 * v.z + m44 * v.h);
	}

	template <class T>
	bool TMatrix4x4<T>::invert(TMatrix4x4<T>& inverse) const
	{
		register Index k;
		register Index i;
		register Index j;
		T a[4][4];
		T b[4][4] =
		{
			{ m11, m12, m13, m14 },
			{ m21, m22, m23, m24 },
			{ m31, m32, m33, m34 },
			{ m41, m42, m43, m44 }
		};
		T tmp;
		T *k_ptr = 0;
		T *j_ptr = 0;
		
		a[0][0] = 1; a[0][1] = 0; a[0][2] = 0; a[0][3] = 0;
		a[1][0] = 0; a[1][1] = 1; a[1][2] = 0; a[1][3] = 0;
		a[2][0] = 0; a[2][1] = 0; a[2][2] = 1; a[2][3] = 0;
		a[3][0] = 0; a[3][1] = 0; a[3][2] = 0; a[3][3] = 1;

		for (k = 0; k < 4; k++)
		{
			if (b[k][k] == (T)0)
			{
				for (j = k + 1; j < 4 && b[j][k] == (T)0; j++);

				if (j < 4)
				{
					for (i = 0; i < 4; i++)
						{
							tmp = *(k_ptr = &b[k][i]); 
							*k_ptr = *(j_ptr = &b[j][i]);
							*j_ptr = tmp;

							tmp = *(k_ptr = &a[k][i]); 
							*k_ptr = *(j_ptr = &a[j][i]);
							*j_ptr = tmp;
						}
				} else {
					return false; // singular 4x4-matrix
				}
			}

			tmp = (T)1 / b[k][k];

			for(j = 0; j < 4; j++)
			{
				a[k][j] *= tmp;
				b[k][j] *= tmp;
			}
			
			for(i = 0; i < 4; i++)
			{
				if (i != k)
				{
					tmp = b[i][k];

					for (j = 0; j < 4; j++)
					{
						a[i][j] -= a[k][j] * tmp;
						b[i][j] -= b[k][j] * tmp;
					}
				}
			}
		}

		k_ptr = *a;
		inverse.m11 = *k_ptr++;
		inverse.m12 = *k_ptr++;
		inverse.m13 = *k_ptr++;
		inverse.m14 = *k_ptr++;
		inverse.m21 = *k_ptr++;
		inverse.m22 = *k_ptr++;
		inverse.m23 = *k_ptr++;
		inverse.m24 = *k_ptr++;
		inverse.m31 = *k_ptr++;
		inverse.m32 = *k_ptr++;
		inverse.m33 = *k_ptr++;
		inverse.m34 = *k_ptr++;
		inverse.m41 = *k_ptr++;
		inverse.m42 = *k_ptr++;
		inverse.m43 = *k_ptr++;
		inverse.m44 = *k_ptr;

		return true;
	}

	template <class T>
	BALL_INLINE bool TMatrix4x4<T>::invert()
	{
		return invert(*this);
	}

	template <class T>
	T TMatrix4x4<T>::getDeterminant() const
	{
		register Index i;
		register Index j;
		register Index k;
		T submatrix[3][3];
		T matrix[4][4] =
		{
			{ m11, m12, m13, m14 },
			{ m21, m22, m23, m24 },
			{ m31, m32, m33, m34 },
			{ m41, m42, m43, m44 }
		};
		T determinant = 0;
			
		for (i = 0; i < 4; i++)
		{
			for (j = 0; j < 3; j++)
			{
				for (k = 0; k < 3; k++)
				{
					submatrix[j][k] =
					matrix[j + 1][(k < i) ? k : k + 1];
				}
			}
			
			determinant += matrix[0][i] * (T)(i / 2.0 == (i >> 1) ? 1 : -1)
					 				* (submatrix[0][0] * submatrix[1][1] * submatrix[2][2] 
										 + submatrix[0][1] * submatrix[1][2] * submatrix[2][0] 
										 + submatrix[0][2] * submatrix[1][0] * submatrix[2][1] 
										 - submatrix[0][2] * submatrix[1][1] * submatrix[2][0] 
										 - submatrix[0][0] * submatrix[1][2] * submatrix[2][1] 
										 - submatrix[0][1] * submatrix[1][0] * submatrix[2][2]);
		}

		return determinant;
	}

	template <class T>
	void TMatrix4x4<T>::translate(const T& x, const T& y, const T& z)
	{
		m14 += m11 * x + m12 * y + m13 * z;
		m24 += m21 * x + m22 * y + m23 * z;
		m34 += m31 * x + m32 * y + m33 * z;
		m44 += m41 * x + m42 * y + m43 * z;
	}

	template <class T>
	void TMatrix4x4<T>::translate(const TVector3<T>& v)
	{
		m14 += m11 * v.x + m12 * v.y + m13 * v.z;
		m24 += m21 * v.x + m22 * v.y + m23 * v.z;
		m34 += m31 * v.x + m32 * v.y + m33 * v.z;
		m44 += m41 * v.x + m42 * v.y + m43 * v.z;
	}

	template <class T>
	void TMatrix4x4<T>::setTranslation(const T& x, const T& y, const T& z)
	{
		m11 = m22 = m33 = m44 = 1;

		m12 = m13 = 
		m21 = m23 = 
		m31 = m32 =  
		m41 = m42 = m43 = 0;

		m14 = x;
		m24 = y;
		m34 = z;
	}

	template <class T>
	void TMatrix4x4<T>::setTranslation(const TVector3<T>& v)
	{
		m11 = m22 = m33 = m44 = 1;

		m12 = m13 = 
		m21 = m23 = 
		m31 = m32 =  
		m41 = m42 = m43 = 0;

		m14 = v.x;
		m24 = v.y;
		m34 = v.z;
	}

	template <class T>
	BALL_INLINE 
	TMatrix4x4<T>& TMatrix4x4<T>::getTranslation
		(const T& x, const T& y, const T& z, TMatrix4x4<T>& m)
	{
		m.setTranslation(x, y, z);

		return m;
	}

	template <class T>
	BALL_INLINE 
	TMatrix4x4<T>& TMatrix4x4<T>::getTranslation
		(const TVector3<T>& v, TMatrix4x4<T>& m)
	{
		m.setTranslation(v);

		return m;
	}

	template <class T>
	void TMatrix4x4<T>::scale
		(const T& x_scale, const T& y_scale, const T& z_scale)
	{
		m11 *= x_scale;
		m21 *= x_scale;
		m31 *= x_scale;
		m41 *= x_scale;

		m12 *= y_scale;
		m22 *= y_scale;
		m32 *= y_scale;
		m42 *= y_scale;

		m13 *= z_scale;
		m23 *= z_scale;
		m33 *= z_scale;
		m43 *= z_scale;
	}

	template <class T>
	void TMatrix4x4<T>::scale(const T& scale)
	{
		m11 *= scale;
		m21 *= scale;
		m31 *= scale;
		m41 *= scale;

		m12 *= scale;
		m22 *= scale;
		m32 *= scale;
		m42 *= scale;

		m13 *= scale;
		m23 *= scale;
		m33 *= scale;
		m43 *= scale;
	}

	template <class T>
	void TMatrix4x4<T>::scale(const TVector3<T>& v)
	{
		m11 *= v.x;
		m21 *= v.x;
		m31 *= v.x;
		m41 *= v.x;

		m12 *= v.y;
		m22 *= v.y;
		m32 *= v.y;
		m42 *= v.y;

		m13 *= v.z;
		m23 *= v.z;
		m33 *= v.z;
		m43 *= v.z;
	}

	template <class T>
	void TMatrix4x4<T>::setScale(const T& x_scale, const T& y_scale, const T& z_scale)
	{
		m11 = x_scale;
		m22 = y_scale;
		m33 = z_scale;
		m44 = 1;

		m12 = m13 = m14 =
		m21 = m23 = m24 =
		m31 = m32 = m34 = 
		m41 = m42 = m43 = 0;
	}

	template <class T>
	void TMatrix4x4<T>::setScale(const T& scale)
	{
		m11 = scale;
		m22 = scale;
		m33 = scale;
		m44 = 1;

		m12 = m13 = m14 =
		m21 = m23 = m24 =
		m31 = m32 = m34 = 
		m41 = m42 = m43 = 0;
	}

	template <class T>
	void TMatrix4x4<T>::setScale(const TVector3<T>& v)
	{
		m11 = v.x;
		m22 = v.y;
		m33 = v.z;
		m44 = 1;

		m12 = m13 = m14 =
		m21 = m23 = m24 =
		m31 = m32 = m34 = 
		m41 = m42 = m43 = 0;
	}

	template <class T>
	BALL_INLINE 
	TMatrix4x4<T>& TMatrix4x4<T>::getScale(const T& x_scale, const T& y_scale, const T& z_scale, TMatrix4x4& m)
	{
		m.setScale(x_scale, y_scale, z_scale);

		return m;
	}

	template <class T>
	BALL_INLINE 
	TMatrix4x4<T>& TMatrix4x4<T>::getScale(const T& scale, TMatrix4x4& m)
	{
		m.setScale(scale);

		return m;
	}

	template <class T>
	BALL_INLINE 
	TMatrix4x4<T>& TMatrix4x4<T>::getScale(const TVector3<T>& v, TMatrix4x4& m)
	{
		m.setScale(v);

		return m;
	}

	template <class T>
	BALL_INLINE 
	TMatrix4x4<T>& TMatrix4x4<T>::getScale(const TVector4<T>& v,TMatrix4x4& m)
	{
		m.setScale(v);

		return m;
	}

	template <class T>
	BALL_INLINE 
	void TMatrix4x4<T>::rotateX(const Angle& phi)
	{
		TMatrix4x4<T> rotation(true);

		rotation.setRotationX(phi);
		*this *= rotation;
	}

	template <class T>
	void
	TMatrix4x4<T>::setRotationX(const Angle& phi)
	{
		m11 = m44 = 1;

			m12 = m13 = m14 
		= m21 = m24 
		= m31 = m34  
		= m41 = m42 = m43 
		= 0;

		m22 = m33 = cos(phi);
		m23 = -(m32 = sin(phi));
	}

	template <class T>
	BALL_INLINE 
	TMatrix4x4<T>& TMatrix4x4<T>::getRotationX(const Angle& phi, TMatrix4x4<T>& m)
	{
		m.setRotationX(phi);

		return m;
	}

	template <class T>
	BALL_INLINE 
	void TMatrix4x4<T>::rotateY(const Angle& phi)
	{
		TMatrix4x4<T> rotation(true);

		rotation.setRotationY(phi);
		*this *= rotation;
	}

	template <class T>
	void TMatrix4x4<T>::setRotationY(const Angle& phi)
	{
		m22 = m44 = 1;

			m12 = m14 
		= m21 = m23 = m24 
		= m32 = m34 
		= m41 = m42 = m43 
		= 0;

		m11 = m33 = cos(phi);
		m31 = -(m13 = sin(phi));
	}

	template <class T>
	BALL_INLINE 
	TMatrix4x4<T>& TMatrix4x4<T>::getRotationY(const Angle& phi, TMatrix4x4<T>& m)
	{
		m.setRotationY(phi);

		return m;
	}

	template <class T>
	BALL_INLINE 
	void TMatrix4x4<T>::rotateZ(const Angle& phi)
	{
		TMatrix4x4<T> rotation(true);

		rotation.setRotationZ(phi);
		*this *= rotation;
	}

	template <class T>
	void TMatrix4x4<T>::setRotationZ(const Angle& phi)
	{
		m33 = m44 = 1;

			m13 = m14 
		= m23 = m24 
		= m31 = m32 = m34 
		= m41 = m42 = m43 
		= 0;

		m11 = m22 = cos(phi);
		m12 = -(m21 = sin(phi));
	}

	template <class T>
	BALL_INLINE 
	TMatrix4x4<T>& TMatrix4x4<T>::getRotationZ(const Angle& phi, TMatrix4x4<T>& m)
	{
		m.setRotationZ(phi);

		return m;
	}

	template <class T>
	BALL_INLINE 
	void TMatrix4x4<T>::rotate(const Angle& phi, const TVector3<T>& v)
	{
		rotate(phi, v.x, v.y, v.z);
	}

	template <class T>
	BALL_INLINE 
	void TMatrix4x4<T>::rotate(const Angle& phi, const TVector4<T>& v)
	{
		rotate(phi, v.x, v.y, v.z);
	}

	//
	//     Arbitrary axis rotation matrix.
	//
	//  [Taken from the MESA-Library. But modified for additional Speed-Up.]
	//
	//  This function was contributed by Erich Boleyn (erich@uruk.org).
	//
	//  This is composed of 5 matrices, Rz, Ry, T, Ry', Rz', multiplied
	//  like so:  Rz * Ry * T * Ry' * Rz'.  T is the final rotation
	//  (which is about the X-axis), and the two composite transforms
	//  Ry' * Rz' and Rz * Ry are (respectively) the rotations necessary
	//  from the arbitrary axis to the X-axis then back.  They are
	//  all elementary rotations.
	//
	//  Rz' is a rotation about the Z-axis, to bring the axis vector
	//  into the x-z plane.  Then Ry' is applied, rotating about the
	//  Y-axis to bring the axis vector parallel with the X-axis.  The
	//  rotation about the X-axis is then performed.  Ry and Rz are
	//  simply the respective inverse transforms to bring the arbitrary
	//  axis back to it's original orientation.  The first transforms
	//  Rz' and Ry' are considered inverses, since the data from the
	//  arbitrary axis gives you info on how to get to it, not how
	//  to get away from it, and an inverse must be applied.
	//
	//  The basic calculation used is to recognize that the arbitrary
	//  axis vector (x, y, z), since it is of unit length, actually
	//  represents the sines and cosines of the angles to rotate the
	//  X-axis to the same orientation, with theta being the angle about
	//  Z and phi the angle about Y (in the order described above)
	//  as follows:
	//
	//  cos ( theta ) = x / sqrt ( 1 - z^2 )
	//  sin ( theta ) = y / sqrt ( 1 - z^2 )
	//
	//  cos ( phi ) = sqrt ( 1 - z^2 )
	//  sin ( phi ) = z
	//
	//  Note that cos ( phi ) can further be inserted to the above
	//  formulas:
	//
	//  cos ( theta ) = x / cos ( phi )
	//  sin ( theta ) = y / sin ( phi )
	//
	//  ...etc.  Because of those relations and the standard trigonometric
	//  relations, it is pssible to reduce the transforms down to what
	//  is used below.  It may be that any primary axis chosen will give the
	//  same results (modulo a sign convention) using thie method.
	//
	//  Particularly nice is to notice that all divisions that might
	//  have caused trouble when parallel to certain planes or
	//  axis go away with care paid to reducing the expressions.
	//  After checking, it does perform correctly under all cases, since
	//  in all the cases of division where the denominator would have
	//  been zero, the numerator would have been zero as well, giving
	//  the expected result.

	template <class T>
	void TMatrix4x4<T>::rotate(const Angle& phi, const T& ax, const T& ay, const T& az)
	{
		T xx, yy, zz, xy, yz, zx, xs, ys, zs, one_c;
		T x = ax;
		T y = ay;
		T z = az;

		double sin_angle = sin(phi);
		double cos_angle = cos(phi);

		xx = x * x;
		yy = y * y;
		zz = z * z;

		T mag = sqrt(xx + yy + zz);
		
		if (mag == (T)0) 
		{
			m12 = m13 = m14 = m21 = m23 = m24 = m31 = m32 = m34 = m41 = m42 = m43 = 0;
			m11 = m22 = m33 = m44 = (T)1;
		}

		x /= mag;
		y /= mag;
		z /= mag;

		xy = x * y;
		yz = y * z;
		zx = z * x;
		xs = x * sin_angle;
		ys = y * sin_angle;
		zs = z * sin_angle;
		one_c = 1 - cos_angle;

		m11 = (one_c * xx) + cos_angle;
		m12 = (one_c * xy) - zs;
		m13 = (one_c * zx) + ys;
		m14 = 0;
		
		m21 = (one_c * xy) + zs;
		m22 = (one_c * yy) + cos_angle;
		m23 = (one_c * yz) - xs;
		m24 = 0;
		
		m31 = (one_c * zx) - ys;
		m32 = (one_c * yz) + xs;
		m33 = (one_c * zz) + cos_angle;
		m34 = 0;
		 
		m41 = 0;
		m42 = 0;
		m43 = 0;
		m44 = 1;
	}

	template <class T>
	void TMatrix4x4<T>::setRotation(const Angle& phi, const T& x, const T& y, const T& z)
	{
		m12 = m13 = m14 = m21 = m23 = m24 = m31 = m32 = m34 = m41 = m42 = m43 = 0;
		m11 = m22 = m33 = m44 = (T)1;
		rotate(phi, x, y, z);
	}

	template <class T>
	BALL_INLINE 
	void TMatrix4x4<T>::setRotation (const Angle& phi, const TVector3<T>& v)
	{
		m12 = m13 = m14 = m21 = m23 = m24 = m31 = m32 = m34 = m41 = m42 = m43 = 0;
		m11 = m22 = m33 = m44 = (T)1;
		rotate(phi, v.x, v.y, v.z);
	}

	template <class T>
	BALL_INLINE 
	TMatrix4x4<T>& TMatrix4x4<T>::getRotation
		(const Angle& phi, const T& x, const T& y, const T& z, TMatrix4x4<T>& m)
	{
		m.setRotation(phi, x, y, z);

		return m;
	}

	template <class T>
	BALL_INLINE 
	TMatrix4x4<T>& TMatrix4x4<T>::getRotation
		(const Angle& phi,const TVector3<T>& v, TMatrix4x4<T>& m)
	{
		m.setRotation(phi, v.x, v.y,v.z);

		return m;
	}

	template <class T>
	BALL_INLINE 
	TMatrix4x4<T>& TMatrix4x4<T>::getRotation
		(const Angle& phi, const TVector4<T>& v, TMatrix4x4<T>& m)
	{
		m.setRotation(phi, v.x, v.y, v.z);

		return m;
	}

	template <class T>
	bool TMatrix4x4<T>::operator == (const TMatrix4x4<T>& m) const
	{
		return (bool)
			(   m11 == m.m11
			 && m12 == m.m12
			 && m13 == m.m13
			 && m14 == m.m14
			 && m21 == m.m21
			 && m22 == m.m22
			 && m23 == m.m23
			 && m24 == m.m24
			 && m31 == m.m31
			 && m32 == m.m32
			 && m33 == m.m33
			 && m34 == m.m34
			 && m41 == m.m41
			 && m42 == m.m42
			 && m43 == m.m43
			 && m44 == m.m44);
	}

	template <class T>
	bool TMatrix4x4<T>::operator != (const TMatrix4x4<T>& m) const
	{
		return (bool)
			(   m11 != m.m11
			 || m12 != m.m12
			 || m13 != m.m13
			 || m14 != m.m14
			 || m21 != m.m21
			 || m22 != m.m22
			 || m23 != m.m23
			 || m24 != m.m24
			 || m31 != m.m31
			 || m32 != m.m32
			 || m33 != m.m33
			 || m34 != m.m34
			 || m41 != m.m41
			 || m42 != m.m42
			 || m43 != m.m43
			 || m44 != m.m44);
	}

	template <class T>
	bool TMatrix4x4<T>::isIdentity() const
	{
		return (bool)
			(   m11 == (T)1
			 && m12 == (T)0
			 && m13 == (T)0
			 && m14 == (T)0
			 && m21 == (T)0
			 && m22 == (T)1
			 && m23 == (T)0
			 && m24 == (T)0
			 && m31 == (T)0
			 && m32 == (T)0
			 && m33 == (T)1
			 && m34 == (T)0
			 && m41 == (T)0
			 && m42 == (T)0
			 && m43 == (T)0
			 && m44 == (T)1);
	}

	template <class T>
	BALL_INLINE 
	bool TMatrix4x4<T>::isRegular() const
	{
		return (getDeterminant() != (T)0);
	}

	template <class T>
	BALL_INLINE
	bool TMatrix4x4<T>::isSingular() const
	{
		return (getDeterminant() == (T)0);
	}

	template <class T>
	bool TMatrix4x4<T>::isSymmetric() const
	{
		return (bool)(   m12 == m21 && m13 == m31
									&& m14 == m41 && m23 == m32
									&& m24 == m42 && m34 == m43);
	}

	template <class T>
	bool TMatrix4x4<T>::isLowerTriangular() const
	{
		return (bool)(   m12 == (T)0
									&& m13 == (T)0
									&& m14 == (T)0
									&& m23 == (T)0
									&& m24 == (T)0
									&& m34 == (T)0);
	}

	template <class T>
	bool TMatrix4x4<T>::isUpperTriangular() const
	{
		return (bool)(   m21 == (T)0
									&& m31 == (T)0
									&& m32 == (T)0
									&& m41 == (T)0
									&& m42 == (T)0
									&& m43 == (T)0);
	}

	template <class T>
	BALL_INLINE 
	bool TMatrix4x4<T>::isDiagonal() const
	{
		return (bool)(   m12 == (T)0
									&& m13 == (T)0
									&& m14 == (T)0
									&& m21 == (T)0
									&& m23 == (T)0
									&& m24 == (T)0
									&& m31 == (T)0
									&& m32 == (T)0
									&& m34 == (T)0
									&& m41 == (T)0
									&& m42 == (T)0
									&& m43 == (T)0);
	}

	template <class T>
	bool TMatrix4x4<T>::isValid() const
	{
		register T **ptr = (T **)comp_ptr_;
		
		return (bool)(   *ptr++ ==& m11
									&& *ptr++ ==& m12
									&& *ptr++ ==& m13
									&& *ptr++ ==& m14
									&& *ptr++ ==& m21
									&& *ptr++ ==& m22
									&& *ptr++ ==& m23
									&& *ptr++ ==& m24
									&& *ptr++ ==& m31
									&& *ptr++ ==& m32
									&& *ptr++ ==& m33
									&& *ptr++ ==& m34
									&& *ptr++ ==& m41
									&& *ptr++ ==& m42
									&& *ptr++ ==& m43
									&& *ptr   ==& m44);
	}

	template <class T>
	std::istream& operator >> (std::istream& s, TMatrix4x4<T>& m)
	{
		s >> m.m11 >> m.m12 >> m.m13 >> m.m14
		  >> m.m21 >> m.m22 >> m.m23 >> m.m24
		  >> m.m31 >> m.m32 >> m.m33 >> m.m34
			>> m.m41 >> m.m42 >> m.m43 >> m.m44;
		
		return s;
	}

	template <class T>
	std::ostream& operator << (std::ostream& s, const TMatrix4x4<T>& m)
	{
		s << '/'  <<  std::setw(14) << m.m11 << ' ' << std::setw(14) << m.m12 << ' ' << std::setw(14) << m.m13 << ' ' << std::setw(14) << m.m14 << " \\" << std::endl
			<< '|'  <<  std::setw(14) << m.m21 << ' ' << std::setw(14) << m.m22 << ' ' << std::setw(14) << m.m23 << ' ' << std::setw(14) << m.m24 << " |"  << std::endl
			<< '|'  <<  std::setw(14) << m.m31 << ' ' << std::setw(14) << m.m32 << ' ' << std::setw(14) << m.m33 << ' ' << std::setw(14) << m.m34 << " |"  << std::endl
			<< '\\' <<  std::setw(14) << m.m41 << ' ' << std::setw(14) << m.m42 << ' ' << std::setw(14) << m.m43 << ' ' << std::setw(14) << m.m44 << " /" << std::endl;

		return s;
	}

	template <class T>
	void TMatrix4x4<T>::dump(std::ostream& s, Size depth) const
	{
		BALL_DUMP_STREAM_PREFIX(s);

		BALL_DUMP_HEADER(s, this, this);

		BALL_DUMP_DEPTH(s, depth);
		s << m11 << " " << m12 << " " << m13 << " " << m14 << std::endl;

		BALL_DUMP_DEPTH(s, depth);
		s << m21 << " " << m22 << " " << m23 << " " << m24 << std::endl;

		BALL_DUMP_DEPTH(s, depth);
		s << m31 << " " << m32 << " " << m33 << " " << m34 << std::endl;

		BALL_DUMP_DEPTH(s, depth);
		s << m41 << " " << m42 << " " << m43 << " " << m44 << std::endl;

		BALL_DUMP_STREAM_SUFFIX(s);
	}

	template <class T>
	TMatrix4x4<T> operator * (const T& scalar, const TMatrix4x4<T>& m);

	template <class T>
	TVector3<T> operator * (const TMatrix4x4<T>& matrix, const TVector3<T>& vector);

	///
	typedef TMatrix4x4<float> Matrix4x4;
	//@}

} // namespace BALL

#endif // BALL_MATHS_MATRIX44_H

// $Id: matrix44.h,v 1.38 2001/02/14 01:59:58 amoll Exp $

#ifndef BALL_MATHS_MATRIX44_H
#define BALL_MATHS_MATRIX44_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#include <math.h>
#include <iostream>
#include <stdlib.h>

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

	/**	@name	4x4 Matrix
			@memo matrix representing transformations: class \Ref{TMatrix4x4} and class \Ref{Matrix4x4}
	*/
	//@{
	template <typename T>
	class TMatrix4x4;

	/**	Input Operator.
			Read sixteen values of type {\tt T} from an input stream.
			@param s	the input stream
			@param m the matrix to read 
	*/
	template <typename T>
	std::istream& operator >> (std::istream& s, TMatrix4x4<T>& m)
		throw();

	/**	Output Operator
			Writes sixteen values of type {\tt T} to an output stream.
			@param s	the output stream
			@param m  the matrix to write 
	*/
	template <typename T>
	std::ostream& operator << (std::ostream& s, const TMatrix4x4<T>& m)
		throw();
	
	/**	Generic 4x4 Matrix Class.
      {\bf Definition:} \URL{BALL/MATHS/matrix44.h} \\
 	*/
	template <typename T>
	class TMatrix4x4
	{
		public:
	
		BALL_CREATE(TMatrix4x4)

		/**	@name	Constructors and Destructors
		*/
		//@{

		/**	Default constructor.
				This method creates a new TMatrix4x4 object. The components
				are initialized to {\tt 0}.
		*/
		TMatrix4x4()
			throw();

		/**	Array constructor.
				This constructor creates a TMatrix4x4 object from the first
				sixteen elements pointed to by {\tt ptr}.
				@param ptr the array to construct from
				@exception NullPointer if {\tt ptr == 0}
		*/
		TMatrix4x4(const T* ptr)
			throw(Exception::NullPointer);

		/**	Array constructor.
				This constructor creates a TMatrix4x4 object from the
				sixteen elements in the array assigned by {\tt ptr}.
				@param ptr the array to construct from
				@exception NullPointer if {\tt ptr == 0}
		*/
		TMatrix4x4(const T ptr[4][4])
			throw(Exception::NullPointer);

		/**	Copy constructor.
				Create a new TMatrix4x4 object from another.
				@param TMatrix4x4 the TMatrix4x4 object to be copied
				@param bool ignored (just for interface consistency)
		*/	
		TMatrix4x4(const TMatrix4x4& m)
			throw();

		/**	Detailed constructor.
				Create a new TMatrix4x4 object from four TVector4.
				@param col1 assigned to the first column
				@param col2 assigned to the second column
				@param col3 assigned to the third column
				@param col4 assigned to the fourth column

		*/
		TMatrix4x4
			(const TVector4<T>& col1, const TVector4<T>& col2,
			 const TVector4<T>& col3, const TVector4<T>& col4)
			throw();

		/**	Detailed constructor.
				Create a new TMatrix4x4 object from sixteen {\tt T} values.
				@param m11 - m44 assigned to the components
		*/
		TMatrix4x4
			(const T& m11, const T& m12, const T& m13, const T& m14, 
			 const T& m21, const T& m22, const T& m23, const T& m24, 
			 const T& m31, const T& m32, const T& m33, const T& m34, 
			 const T& m41, const T& m42, const T& m43, const T& m44)
			throw();

		/**	Destructor.	
				Destructs the TMatrix4x4 object. As there are no dynamic
				data structures, nothing happens.
		*/
		virtual ~TMatrix4x4()
			throw()
		{
		}

		/**	Clear method.
				The values are set to 0.
		*/
		virtual void clear() 
			throw();

		//@}
		/**	@name	Assignment
		*/
		//@{

		/**	Assign from array-ptr.
				Assign from the first	sixteen elements pointed to by {\tt ptr}.
				@param ptr the array to construct from
				@exception NullPointer if {\tt ptr == 0}
		*/
		void set( const T* ptr)
			throw(Exception::NullPointer);

		/**	Assign from the first sixteen elements.
				pointed to by the array assigned by {\tt ptr}.
				@param ptr the array to construct from
				@exception NullPointer if {\tt ptr == 0}
		*/
		void set(const T ptr[4][4])
			throw(Exception::NullPointer);

		/**	Assign from another instance.
				@param TMatrix4x4	the TMatrix4x4 object to assign from
		*/
		void set(const TMatrix4x4& m)
			throw();

		/**	Assign from four TVector4.
				@param col1 assigned to the first column
				@param col2 assigned to the second column
				@param col3 assigned to the third column
				@param col4 assigned to the fourth column
		*/
		void set
			(const TVector4<T>& col1, const TVector4<T>& col2,
			 const TVector4<T>& col3, const TVector4<T>& col4)
			throw();

		/**	Assign from sixteen values of type T.
				@param m11 - m44 assigned to the components
		*/
		void set
			(const T& m11, const T& m12, const T& m13, const T& m14, 
			 const T& m21, const T& m22, const T& m23, const T& m24, 
			 const T& m31, const T& m32, const T& m33, const T& m34, 
			 const T& m41, const T& m42, const T& m43, const T& m44)
			throw();

		/**	Assignment operator.
				Assign the components from the first 16 values assigned by {\tt ptr}.
				@param ptr the array to construct from
		**/
		const TMatrix4x4& operator = ( const T* ptr)
			throw(Exception::NullPointer);

		/**	Assignment operator.
				Assign the components from the first 16 values assigned by {\tt ptr}.
				@param ptr the array to construct from
		**/
		const TMatrix4x4& operator = (const T ptr[4][4])
			throw(Exception::NullPointer);

		/**	Assignment operator.
				Assign the components from another instance of TMatrix4x4.
				@param TMatrix4x4 the TMatrix4x4 to assign from
		**/
		const TMatrix4x4& operator = (const TMatrix4x4& m)
			throw();

		/**	Assign to an array.
				Assigns the components to a pointer of an array of sixteen values of type {\tt T}.
				@exception NullPointer if {\tt ptr == 0}
				@param ptr the pointer to assign to
		*/
		void get(T* ptr) const
			throw(Exception::NullPointer);

		/**	Assign to an array.
				Assigns the components to an array of sixteen values of type {\tt T}.
				@exception NullPointer if {\tt ptr == 0}
				@param ptr the array to assign to
		*/
		void get(T ptr[4][4]) const
			throw(Exception::NullPointer);

		/**	Assign to another instance.
				Assigns the components to another TMatrix4x4.
				@param TMatrix4x4	the TMatrix4x4 to be assigned to
		*/
		void get(TMatrix4x4& m) const
			throw();

		/**	Assign to four variables of type {\em TVector4}.
				@param col1 the TVector4 to obtain the values of the first column
				@param col2 the TVector4 to obtain the values of the second column
				@param col3 the TVector4 to obtain the values of the third column
				@param col4 the TVector4 to obtain the values of the fourth column
		*/
		void get
			(TVector4<T>& col1, TVector4<T>& col2,
			 TVector4<T>& col3, TVector4<T>& col4) const
			throw();

		/**	Assign to sixteen variables of type {\tt T}.
				@param m11 - m44 the variables to assign to
		*/
		void get
			(T& m11, T& m12, T& m13, T& m14, 
			 T& m21, T& m22, T& m23, T& m24, 
			 T& m31, T& m32, T& m33, T& m34, 
			 T& m41, T& m42, T& m43, T& m44) const
			throw();

		/**	Swap the contents of two instances of TMatrix4x4.
				@param TMatrix4x4 the TMatrix4x4 to swap contents with
		*/
		void swap(TMatrix4x4& m)
			throw();

		//@}
		/**	@name	Accessors
		*/
		//@{

		/** Compute the trace.
				Get the sum of the diagonal elements (m11 + m22 + m33 + m44).
				@return T the trace
		*/
		T getTrace() const
			throw();

		/** Create a zero matrix.
				A new matrix object is created and all elements set to 0.
		*/
		static const TMatrix4x4& getZero()
			throw();

		/** Create an identity matrix.
				A new matrix object is created and all elements but the diagonal are 
				set to zero. The diagonal elements are set to 1.
		*/
		static const TMatrix4x4& getIdentity()
			throw();

		/** Set to an identity matrix.
				m11, m22, m33, m44 = 1;
				the other cells have the value 0;
		*/
		void setIdentity()
			throw();

		/** Set the diagonal elements to the given value.
				All other elements are set to 0.
				@param T the value to fill with (default: 1)
		*/
		void set(const T& t = (T)1)
			throw();

		/** Mirror the Matrix at the diagonal.
				All values are swaped by the mirrored value.
				(I.e. m12 <=> m21 , m13 <=> m31 , ...)
		*/
		void transpose()
			throw();

		/** Get a row of the matrix.
				@param row the number of the row (0-3)
				@exception IndexOverflow if {\tt row > 3}
				@return TVector4 the row
		*/
		TVector4<T> getRow(Position row) const
			throw(Exception::IndexOverflow);

		/** Get a column of the matrix.
				@param col the number of the column (0-3)
				@exception IndexOverflow if {\tt col > 3}
				@return TVector4 the column
		*/
		TVector4<T> getColumn(Position col) const
			throw(Exception::IndexOverflow);

		/** Set a row of the matrix.
				@param row the number of the row (0-3)
				@param row_value the new value of the row
				@exception IndexOverflow if {\tt row > 3}
		*/
		void setRow(Position row, const TVector4<T> row_value)
			throw(Exception::IndexOverflow);

		/** Set a column of the matrix.
				@param col the number of the column (0-3)
				@param col_value the new value of the col
				@exception IndexOverflow if {\tt col > 3}
		*/
		void setColumn(Position col, const TVector4<T> col_value)
			throw(Exception::IndexOverflow);

		/** Test whether two matrices are equal.
				Two matrices are considered equal, if \Ref{Maths::isEqual} returns {\bf true} 
				for each pair of corresponding elements.
				@param m the matrix to compare with
				@return bool, {\bf true} if all components are equal, {\bf false} otherwise
		*/
		bool isEqual(const TMatrix4x4& m) const
			throw();

		/** Get the diagonal of the matrix.
				@return TVector4 the diagonal
		*/
		TVector4<T> getDiagonal() const
			throw();
		
		/** Access operator of a cell.
				@param row the number of the row (0-3)
				@param col the number of the column (0-3)
				@exception IndexOverflow if {\tt col >3 || row > 3}
				@return T& a reference to the cell
		*/
		T& operator () (Position row, Position col)
			throw(Exception::IndexOverflow);

		/** Constant access operator of a cell.
				@param row the number of the row (0-3)
				@param col the number of the column (0-3)
				@exception IndexOverflow if {\tt col ||row > 3}
				@return T& a const reference to the cell
		*/
		const T& operator () (Position row, Position col) const
			throw(Exception::IndexOverflow);

		/**	Constant random access operator.
				Access single elements of the matrix. {\tt index} may assume
				values in the range of 0 - 15. The elements of the matrix
				are returned rows first, i.e., in the following order: {\tt m11}, {\tt m12}, {\tt m13}...
				
		*/
		const T& operator [] (Position index) const
			throw(Exception::IndexOverflow);

		/**	Mutable random access operator.
				@see operator[]
		*/
		T& operator [] (Position index)
			throw(Exception::IndexOverflow);

		/**	Positive sign.
		*/
		TMatrix4x4 operator + () const
			throw();

		/**	Negative sign.
		*/
		TMatrix4x4 operator - () const
			throw();

		/** Addition operator.
				Adds another matrix to this matrix and return the result.
				@param m the matrix to add
				@return TMatrix4x4 the result
		*/
		TMatrix4x4 operator + (const TMatrix4x4& m) const
			throw();

		/** Addition operator.
				Adds another matrix to this matrix.
				@param m the matrix to add
				@return TMatrix4x4&, {\em *this}
		*/
		TMatrix4x4& operator += (const TMatrix4x4& m)
			throw();

		/** Subtraction operator.
				Subtract another matrix from this matrix and return the result
				@param m the matrix to subtract
				@return TMatrix4x4 the result
		*/
		TMatrix4x4 operator - (const TMatrix4x4& m) const
			throw();

		/** Subtraction operator.
				Subtract another matrix from this matrix.
				@param m the matrix to subtract
				@return TMatrix4x4&, {\em *this}
		*/
		TMatrix4x4& operator -= (const TMatrix4x4& m)
			throw();

		/**	Multiply by a scalar.
				Operator for multiplying every cell value with a scalar value.
				@return TMatrix4x4 the result
		*/
		TMatrix4x4 operator * (const T& scalar) const
			throw();

		/**	Multiply by a scalar.
				Operator for multiplying every cell value with a scalar value.
				@return TMatrix4x4&, {\em *this}
		*/
		TMatrix4x4& operator *= (const T& scalar)
			throw();

		/**	Divide by a scalar.
				Operator for dividing every cell value by a scalar value.
				@exception DivisionByZero if {\tt scalar == 0}
				@return TMatrix4x4 the result
		*/
		TMatrix4x4 operator / (const T& scalar) const
			throw(Exception::DivisionByZero);

		/**	Divide by a scalar.
				Operator for dividing every cell value by a scalar value.
				@exception DivisionByZero if {\tt scalar == 0}
				@return TMatrix4x4&, {\em *this}
		*/
		TMatrix4x4& operator /= (const T& scalar)
			throw(Exception::DivisionByZero);

		/**	Multiply two matrices.
				@return TMatrix4x4 the result
		*/
		TMatrix4x4 operator * (const TMatrix4x4& m) const
			throw();

		/**	Multiply two matrices
				@return TMatrix4x4&, {\em *this}
		*/
		TMatrix4x4& operator *= (const TMatrix4x4& m)
			throw();

		/**	Multiplication by an instance of type {\em TVector4}.
				@return TMatrix4x4&, {\em *this}
		*/
		TVector4<T> operator * (const TVector4<T> &) const
			throw();

		/**	Invert the matrix.
				Tests if the matrix can be inverted.
				If possible, the result will be inverted and the result returned in {\em inverse}.
				@param inverse is assigned the inverse matrix 
				@return bool true if the inverse matrix could be calculated, otherwise false.
		*/
		bool invert(TMatrix4x4& inverse) const
			throw();

		/**	Invert the matrix.
				Tests if the matrix can be inverted.
				If this is possible, the result is stored in the matrix.
				@return bool true if the inverse matrix could be calculated, otherwise false.
		*/
		bool invert()
			throw();

		/**	Compute the determinant.
				@return T the determinant.
		*/
		T getDeterminant() const
			throw();

		/**	Translate the matrix.
				@param x the x-component of the translation
				@param y the y-component of the translation
				@param z the z-component of the translation
		*/
		void translate(const T &x, const T &y, const T &z)
			throw();

		/**	Translate the matrix.
				@param v the vector to translate with
		*/
		void translate(const TVector3<T>& v)
			throw();

		/**	Set the matrix to a translation matrix.
				@param x the x-component of the translation
				@param y the y-component of the translation
				@param z the z-component of the translation
		*/
		void setTranslation(const T& x, const T& y, const T& z)
			throw();

		/**	Set the matrix to a translation matrix.
				@param v the vector to translate with
		*/
		void setTranslation(const TVector3<T>& v)
			throw();

		/**	Scale the matrix.
				@param x_scale the x scale factor
				@param y_scale the y scale factor
				@param z_scale the z scale factor
		*/
		void scale(const T& x_scale, const T& y_scale, const T& z_scale)
			throw();

		/**	Scale the matrix.
				@param scale the scale factor
		*/
		void scale(const T& scale)
			throw();

		/**	Scale the matrix.
				@param v the vector with the scale factor
		*/
		void scale(const TVector3<T>& v)
			throw();

		/**	Set the matrix to a scalation matrix.
				@param x_scale the x scale factor
				@param y_scale the y scale factor
				@param z_scale the z scale factor
		*/
		void setScale(const T& x_scale, const T& y_scale, const T& z_scale)
			throw();

		/**	Set the matrix to a scalation matrix.
				@param scale the scale factor
		*/
		void setScale(const T& scale)
			throw();

		/**	Set the matrix to a scalation matrix.
				@param v the vector with the scale factor
		*/
		void setScale(const TVector3<T>& v)
			throw();

		/**	Rotate the matrix around the x axis.
				@param phi the rotation angle
		*/
		void rotateX(const Angle& phi)
			throw();

		/**	Set the matrix to a x rotation matrix.
				@param phi the rotation angle
		*/
		void setRotationX(const Angle& phi)
			throw();

		/**	Rotate the matrix around the y axis.
				@param phi the rotation angle
		*/
		void rotateY(const Angle& phi)
			throw();

		/**	Set the matrix to a y rotation matrix.
				@param phi the rotation angle
		*/
		void setRotationY(const Angle& phi)
			throw();

		/**	Rotate the matrix around the z axis.
				@param phi the rotation angle
		*/
		void rotateZ(const Angle &phi)
			throw();

		/**	Set the matrix to a z rotation matrix.
				@param phi the rotation angle
		*/
		void setRotationZ(const Angle &phi)
			throw();

		/** Rotate the matrix around a given axis.
				@param phi the rotation angle
				@param axis_x the x component of the axis
				@param axis_y the y component of the axis
				@param axis_z the z component of the axis
		*/
		void rotate(const Angle& phi, const T& axis_x, const T& axis_y, const T& axis_z)
			throw();

		/** Rotate the matrix around a given axis.
				@param phi the rotation angle
				@param axis the axis vector
		*/
		void rotate(const Angle& phi, const TVector3<T>& axis)
			throw();

		/** Rotate the matrix around a given axis.
				@param phi the rotation angle
				@param axis the axis vector, the fourth component of the vector is ignored
		*/
		void rotate(const Angle& phi, const TVector4<T>& axis)
			throw();

		/**	Set the matrix to a rotation matrix.
				@param phi the rotation angle
				@param axis_x the x component of the axis
				@param axis_y the y component of the axis
				@param axis_z the z component of the axis				
		*/
		void setRotation(const Angle& phi, const T& axis_x, const T& axis_y, const T& axis_z)
			throw();

		/**	Set the matrix to a rotation matrix.
				@param phi the rotation angle
				@param axis the axis vector
		*/
		void setRotation(const Angle& phi, const TVector3<T>& axis)
			throw();

		/**	Set the matrix to a rotation matrix.
				@param phi the rotation angle
				@param axis the axis vector, the fourth component of the vector is ignored
		*/
		void setRotation(const Angle& phi, const TVector4<T>& axis)
			throw();

		//@}
		/**	@name	Predicates
		*/
		//@{

		/**	Equality operator.
				Instead of this operator isEqual should be used.
				\Ref{isEqual}
				@return bool, {\bf true} if all components are equal, {\bf false} otherwise
		*/
		bool operator == (const TMatrix4x4& m) const
			throw(); 

		/**	Inequality operator.
				Instead of this operator isEqual should be used.
				\Ref{isEqual}
				@return bool, {\bf true} if the two TMatrix4x4 differ in at least one component, {\bf false} otherwise
		*/
		bool operator != (const TMatrix4x4& m) const
			throw();

		/** Test whether this matrix is an identity matrix.
				(I.e. m11, m22, m33, m44 = 1 and the other cells have the value 0)
				@return bool, {\bf true} if identity matrix, {\bf false} otherwise
		*/
		bool isIdentity() const
			throw();

		/** Test whether this matrix is regular.
				@return bool, {\bf true} if (Determinant != 0), {\bf false} otherwise
		*/
		bool isRegular() const
			throw();

		/** Test whether this matrix is singular.
				@return bool, {\bf true} if (Determinant == 0), {\bf false} otherwise
		*/
		bool isSingular() const
			throw();

		/** Test whether this matrix is symmetric.
				(m12 = m21, m31 = m13, ...)
				@return bool, {\bf true} if symmatric, {\bf false} otherwise
		*/
		bool isSymmetric() const
			throw();

		/** Test whether the lower triangular is zero.
				@return bool, {\bf true} if (m12 = m13 = m14 = m23 = m24 = m34 = 0), {\bf false} otherwise
		*/
		bool isLowerTriangular() const
			throw();

		/** Test whether the upper triangular is zero.
				@return bool, {\bf true} if (m21 = m31 = m32 = m41 = m42 = m43 = 0), {\bf false} otherwise
		*/
		bool isUpperTriangular() const
			throw();

		/** Test whether all cells but the diagonal are zero.
				@return bool, {\bf true} or {\bf false}
		*/
		bool isDiagonal() const
			throw();

		//@}
		/**	@name	Debugging and Diagnostics
		*/
		//@{

		/**	Test whether instance is valid.
				Always returns true.
				@return bool {\bf true}
		*/
		bool isValid() const
			throw();

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

		/**	1st cell in the 1st row
		*/
		T m11;

		/**	2nd cell in the 1st row
		*/
		T m12;

		/**	3rd cell in the 1st row
		*/
		T m13;

		/**	4th cell in the 1st row
		*/
		T m14;

		/**	1st cell in the 2nd row
		*/
		T m21;

		/**	2nd cell in the 2nd row
		*/
		T m22;

		/**	3rd cell in the 2nd row
		*/
		T m23;

		/**	4th cell in the 2nd row
		*/
		T m24;

		/**	1st cell in the 3rd row
		*/
		T m31;

		/**	2nd cell in the 3rd row
		*/
		T m32;

		/**	3rd cell in the 3rd row
		*/
		T m33;

		/**	4th cell in the 3rd row
		*/
		T m34;

		/**	1st cell in the 4th row
		*/
		T m41;

		/**	2nd cell in the 4th row
		*/
		T m42;

		/**	3rd cell in the 4th row
		*/
		T m43;

		/**	4th cell in the 4th row
		*/
		T m44;
		//@}

		private:

		void initializeComponentPointers_()
			throw()
		{
			T **ptr = (T **)comp_ptr_;

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
		T* comp_ptr_[16];
	};

	template <typename T>
	TMatrix4x4<T>::TMatrix4x4()
		throw()
		:	m11(0), m12(0), m13(0), m14(0), 
			m21(0), m22(0), m23(0), m24(0), 
			m31(0), m32(0), m33(0), m34(0), 
			m41(0), m42(0), m43(0), m44(0)
	{
		initializeComponentPointers_();
	}

	template <typename T>
	TMatrix4x4<T>::TMatrix4x4( const T* ptr)
		throw(Exception::NullPointer)
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

	template <typename T>
	TMatrix4x4<T>::TMatrix4x4(const T array_ptr[4][4])
		throw(Exception::NullPointer)
	{
		if (array_ptr == 0)
		{
			throw Exception::NullPointer(__FILE__, __LINE__);
		}
		
		const T *ptr = *array_ptr;
			
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

	template <typename T>
	TMatrix4x4<T>::TMatrix4x4(const TMatrix4x4<T>& m)
		throw()
		:	m11(m.m11), m12(m.m12), m13(m.m13), m14(m.m14), 
			m21(m.m21), m22(m.m22), m23(m.m23), m24(m.m24), 
			m31(m.m31), m32(m.m32), m33(m.m33), m34(m.m34), 
			m41(m.m41), m42(m.m42), m43(m.m43), m44(m.m44)
	{
		initializeComponentPointers_();
	}


	template <typename T>
	TMatrix4x4<T>::TMatrix4x4
		(const TVector4<T>& col1, const TVector4<T>& col2,
		 const TVector4<T>& col3,const TVector4<T>& col4)
		throw()
		:	m11(col1.x), m12(col1.y), m13(col1.z), m14(col1.h), 
			m21(col2.x), m22(col2.y), m23(col2.z), m24(col2.h), 
			m31(col3.x), m32(col3.y), m33(col3.z), m34(col3.h), 
			m41(col4.x), m42(col4.y), m43(col4.z), m44(col4.h)
	{
		initializeComponentPointers_();
	}

	template <typename T>
	TMatrix4x4<T>::TMatrix4x4
		(const T& m11, const T& m12, const T& m13, const T& m14, 
		 const T& m21, const T& m22, const T& m23, const T& m24, 
		 const T& m31, const T& m32, const T& m33, const T& m34, 
		 const T& m41, const T& m42, const T& m43, const T& m44)
		throw()
		:	m11(m11), m12(m12), m13(m13), m14(m14), 
			m21(m21), m22(m22), m23(m23), m24(m24), 
			m31(m31), m32(m32), m33(m33), m34(m34), 
			m41(m41), m42(m42), m43(m43), m44(m44)
	{
		initializeComponentPointers_();
	}

	template <typename T>
	void TMatrix4x4<T>::clear()
		throw()
	{
		set((T)0);
	}

	template <typename T>
	void TMatrix4x4<T>::set(const T* ptr)
		throw(Exception::NullPointer)
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
	}

	template <typename T>
	void TMatrix4x4<T>::set(const T array_ptr[4][4])
		throw(Exception::NullPointer)
	{
    if (array_ptr == 0)
		{
      throw Exception::NullPointer(__FILE__, __LINE__);
		}
 		
		const T *ptr = *array_ptr;

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

	template <typename T>
	void TMatrix4x4<T>::set(const TMatrix4x4<T>& m)
		throw()
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

	template <typename T>
	void TMatrix4x4<T>::set
		(const TVector4<T>& col1, const TVector4<T>& col2,
		 const TVector4<T>& col3, const TVector4<T>& col4)
		throw()
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

	template <typename T>
	void TMatrix4x4<T>::set
		(const T& c11, const T& c12, const T& c13, const T& c14, 
		 const T& c21, const T& c22, const T& c23, const T& c24, 
		 const T& c31, const T& c32, const T& c33, const T& c34, 
		 const T& c41, const T& c42, const T& c43, const T& c44)
		throw()
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

	template <typename T>
	BALL_INLINE 
	const TMatrix4x4<T>& TMatrix4x4<T>::operator = (const T* ptr)
		throw(Exception::NullPointer)
	{
		set(ptr);
		return *this;
	}

	template <typename T>
	BALL_INLINE 
	const TMatrix4x4<T>& TMatrix4x4<T>::operator = (const T array_ptr[4][4])
		throw(Exception::NullPointer)
	{
		set(array_ptr);
		return *this;
	}

	template <typename T>
	BALL_INLINE 
	const TMatrix4x4<T>& TMatrix4x4<T>::operator = (const TMatrix4x4<T>& m)
		throw()
	{
		set(m);
		return *this;
	}

	template <typename T>
	void TMatrix4x4<T>::get(T* ptr) const
		throw(Exception::NullPointer)
	{
    if (ptr == 0)
		{
      throw Exception::NullPointer(__FILE__, __LINE__);
		}

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

	template <typename T>
	void TMatrix4x4<T>::get(T array_ptr[4][4]) const
		throw(Exception::NullPointer)
	{
    if (array_ptr == 0)
		{
       throw Exception::NullPointer(__FILE__, __LINE__);
		}
 
		T *ptr = *array_ptr;

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

	template <typename T>
	void TMatrix4x4<T>::get(TMatrix4x4<T>& m) const
		throw()
	{
		m.set(*this);
	}

	template <typename T>
	void TMatrix4x4<T>::get
		(TVector4<T>& col1, TVector4<T>& col2,
		 TVector4<T>& col3, TVector4<T>& col4) const
		throw()
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

	template <typename T>
	void TMatrix4x4<T>::get
		(T& c11, T& c12, T& c13, T& c14, 
		 T& c21, T& c22, T& c23, T& c24, 
		 T& c31, T& c32, T& c33, T& c34, 
		 T& c41, T& c42, T& c43, T& c44) const
		throw()
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

	template <typename T>
	BALL_INLINE 
	T TMatrix4x4<T>::getTrace() const
		throw()
	{
		return (m11 + m22 + m33 + m44);
	}

	template <typename T>
	BALL_INLINE 
	const TMatrix4x4<T>& TMatrix4x4<T>::getZero()
		throw()
	{
		static TMatrix4x4<T> null_matrix
			(0, 0, 0, 0,
			 0, 0, 0, 0,
			 0, 0, 0, 0,
			 0, 0, 0, 0);
		
		return null_matrix;
	}


	template <typename T>
	BALL_INLINE
	void TMatrix4x4<T>::setIdentity()
		throw()
	{
		m12 = m13 = m14 = m21 = m23 = m24 = m31 = m32 = m34 = m41 = m42 = m43 = 0;
		m11 = m22 = m33 = m44 = (T)1;
	}
	template <typename T>
	BALL_INLINE 
	const TMatrix4x4<T>& TMatrix4x4<T>::getIdentity()
		throw()
	{
		static TMatrix4x4<T> identity
			(1, 0, 0, 0,
			 0, 1, 0, 0,
			 0, 0, 1, 0,
			 0, 0, 0, 1);

		return identity;
	}

	template <typename T>
	void TMatrix4x4<T>::set(const T& t)
		throw()
	{
			m11 = m12 = m13 = m14 
		= m21 = m22 = m23 = m24 
		= m31 = m32 = m33 = m34
		= m41 = m42 = m43 = m44
		= t;
	}

	template <typename T>
	void TMatrix4x4<T>::swap(TMatrix4x4<T>& m)
		throw()
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

	template <typename T>
	void TMatrix4x4<T>::transpose()
		throw()
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

	template <typename T>
	TVector4<T> TMatrix4x4<T>::getRow(Position row) const
		throw(Exception::IndexOverflow)
	{
		if (row > 3)
		{
			throw Exception::IndexOverflow(__FILE__, __LINE__, row, 3);
		}

		// calculate the start of the row in the array
		const T* ptr = comp_ptr_[4 * row];
		return TVector4<T> (ptr[0], ptr[1], ptr[2], ptr[3]);
	}

	template <typename T>
	TVector4<T> TMatrix4x4<T>::getColumn(Position col) const
		throw(Exception::IndexOverflow)
	{
		if (col > 3)
		{
			throw Exception::IndexOverflow(__FILE__, __LINE__, col, 3);
		}
		
		const T* ptr = comp_ptr_[col];

		return TVector4<T> (ptr[0], ptr[4], ptr[8], ptr[12]);
	}


	template <typename T>
	void TMatrix4x4<T>::setRow(Position row, const TVector4<T> row_value)
		throw(Exception::IndexOverflow)
	{
		if (row > 3)
		{
			throw Exception::IndexOverflow(__FILE__, __LINE__, row, 3);
		}

		// calculate a pointer to the start of the row
		T* ptr = comp_ptr_[4 * row];

		ptr[0] = row_value.x;
		ptr[1] = row_value.y;
		ptr[2] = row_value.z;
		ptr[3] = row_value.h;
	}

	template <typename T>
	void TMatrix4x4<T>::setColumn(Position col, const TVector4<T> col_value)
		throw(Exception::IndexOverflow)
	{
		if (col > 3)
		{
			throw Exception::IndexOverflow(__FILE__, __LINE__, col, 3);
		}

		// calculate a pointer to the start of the column
		T* ptr = comp_ptr_[col];

		ptr[0] = col_value.x;
		ptr[4] = col_value.y;
		ptr[8] = col_value.z;
		ptr[12] = col_value.h;
	}

	template <typename T>
	bool TMatrix4x4<T>::isEqual(const TMatrix4x4<T>& m) const
		throw()
	{
		// iterate over all component pointers
		// and compare the elements for approximate equality
		for (Position i = 0; i < 16; i++)
		{
			if (Maths::isEqual(*comp_ptr_[i], *m.comp_ptr_[i]) == false)
			{
				return false;
			} 
		}

		return true;
	}

	template <typename T>
	TVector4<T>TMatrix4x4<T>::getDiagonal() const
		throw()
	{
		return TVector4<T>(m11, m22, m33, m44);
	}

	template <typename T>
	BALL_INLINE  
	T& TMatrix4x4<T>::operator () (Position row, Position col)
		throw(Exception::IndexOverflow)
	{
    if ((row > 3) || (col > 3))
		{
      throw Exception::IndexOverflow(__FILE__, __LINE__, row + col, 3);
		}

		return *comp_ptr_[4 * row + col];
	}

	template <typename T>
	BALL_INLINE 
	const T& TMatrix4x4<T>::operator () (Position row, Position col) const
		throw(Exception::IndexOverflow)
	{
    if ((row > 3) || (col > 3))
		{
			throw Exception::IndexOverflow(__FILE__, __LINE__, row + col, 3);
		}

		return *comp_ptr_[4 * row + col];
	}

	template <typename T>
	BALL_INLINE
	const T& TMatrix4x4<T>::operator [] (Position index) const
		throw(Exception::IndexOverflow)
	{
		if (index > 15)
		{
			throw Exception::IndexOverflow(__FILE__, __LINE__, index, 15);
		}
		return *comp_ptr_[index];
	}

	template <typename T>
	BALL_INLINE
	T& TMatrix4x4<T>::operator [] (Position index)
		throw(Exception::IndexOverflow)
	{
		if (index > 15)
		{
			throw Exception::IndexOverflow(__FILE__, __LINE__, index, 15);
		}
		return *comp_ptr_[index];
	}

	template <typename T>
	BALL_INLINE 
	TMatrix4x4<T> TMatrix4x4<T>::operator + () const
		throw()
	{
		return *this;
	}

	template <typename T>
	BALL_INLINE TMatrix4x4<T>
	TMatrix4x4<T>::operator - () const
		throw()
	{
		return TMatrix4x4<T>
			(-m11, -m12, -m13, -m14,
			 -m21, -m22, -m23, -m24,
			 -m31, -m32, -m33, -m34,
			 -m41, -m42, -m43, -m44);
	}

	template <typename T>
	TMatrix4x4<T> TMatrix4x4<T>::operator + (const TMatrix4x4<T>& m) const
		throw()
	{
		return TMatrix4x4<T>
			(m11 + m.m11, m12 + m.m12, m13 + m.m13, m14 + m.m14,
			 m21 + m.m21, m22 + m.m22, m23 + m.m23, m24 + m.m24,
			 m31 + m.m31, m32 + m.m32, m33 + m.m33, m34 + m.m34,
			 m41 + m.m41, m42 + m.m42, m43 + m.m43, m44 + m.m44);
	}

	template <typename T>
	TMatrix4x4<T>& TMatrix4x4<T>::operator += (const TMatrix4x4<T>& m)
		throw()
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

	template <typename T>
	TMatrix4x4<T> TMatrix4x4<T>::operator - (const TMatrix4x4<T>& m) const
		throw()
	{
		return TMatrix4x4<T>
			(m11 - m.m11, m12 - m.m12, m13 - m.m13, m14 - m.m14,
			 m21 - m.m21, m22 - m.m22, m23 - m.m23, m24 - m.m24,
			 m31 - m.m31, m32 - m.m32, m33 - m.m33, m34 - m.m34,
			 m41 - m.m41, m42 - m.m42, m43 - m.m43, m44 - m.m44);
	}

	template <typename T>
	TMatrix4x4<T>& TMatrix4x4<T>::operator -= (const TMatrix4x4<T>& m)
		throw()
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

	template <typename T>
	TMatrix4x4<T> TMatrix4x4<T>::operator * (const T& scalar) const
		throw()
	{
		return TMatrix4x4<T>
			(m11 * scalar, m12 * scalar, m13 * scalar, m14 * scalar,
			 m21 * scalar, m22 * scalar, m23 * scalar, m24 * scalar,
			 m31 * scalar, m32 * scalar, m33 * scalar, m34 * scalar,
			 m41 * scalar, m42 * scalar, m43 * scalar, m44 * scalar);
	}

	template <typename T>
	TMatrix4x4<T> operator * (const T& scalar, const TMatrix4x4<T>& m)
		throw()
	{
		return TMatrix4x4<T>
			(scalar * m.m11, scalar * m.m12, scalar * m.m13, scalar * m.m14,
			 scalar * m.m21, scalar * m.m22, scalar * m.m23, scalar * m.m24,
			 scalar * m.m31, scalar * m.m32, scalar * m.m33, scalar * m.m34,
			 scalar * m.m41, scalar * m.m42, scalar * m.m43, scalar * m.m44);
	}

	template <typename T>
	TMatrix4x4<T>& TMatrix4x4<T>::operator *= (const T& scalar)
		throw()
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

	template <typename T>
	TVector3<T> operator *(const TMatrix4x4<T>& matrix, const TVector3<T>& vector)
		throw()
	{
		return TVector3<T>
			(matrix.m11 * vector.x + matrix.m12 * vector.y + matrix.m13 * vector.z + matrix.m14,
			 matrix.m21 * vector.x + matrix.m22 * vector.y + matrix.m23 * vector.z + matrix.m24,
			 matrix.m31 * vector.x + matrix.m32 * vector.y + matrix.m33 * vector.z + matrix.m34);
	}

	template <typename T>
	BALL_INLINE 
	TMatrix4x4<T>TMatrix4x4<T>::operator / (const T& scalar) const
		throw(Exception::DivisionByZero)
	{
		if (scalar == (T)0)
		{
			throw Exception::DivisionByZero(__FILE__, __LINE__);
		}
		
		return (*this * ((T)1 / scalar));
	}

	template <typename T>
	BALL_INLINE 
	TMatrix4x4<T>& TMatrix4x4<T>::operator /= (const T& scalar)
		throw(Exception::DivisionByZero)
	{
		if (scalar == (T)0)
		{
			throw Exception::DivisionByZero(__FILE__, __LINE__);
		}
		
		return (*this *= (T)1 / scalar);
	}

	template <typename T>
	TMatrix4x4<T> TMatrix4x4<T>::operator * (const TMatrix4x4<T>& m) const
		throw()
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

	template <typename T>
	TMatrix4x4<T>& TMatrix4x4<T>::operator *= (const TMatrix4x4<T>& m)
		throw()
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

	template <typename T>
	TVector4<T> TMatrix4x4<T>::operator * (const TVector4<T>& v) const
		throw()
	{
		return TVector4<T>
			(m11 * v.x + m12 * v.y + m13 * v.z + m14 * v.h,
			 m21 * v.x + m22 * v.y + m23 * v.z + m24 * v.h,
			 m31 * v.x + m32 * v.y + m33 * v.z + m34 * v.h,
			 m41 * v.x + m42 * v.y + m43 * v.z + m44 * v.h);
	}

	template <typename T>
	bool TMatrix4x4<T>::invert(TMatrix4x4<T>& inverse) const
		throw()
	{
		Position k;
		Position i;
		Position j;
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
				} 
				else 
				{
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

	template <typename T>
	BALL_INLINE bool TMatrix4x4<T>::invert()
		throw()
	{
		return invert(*this);
	}

	template <typename T>
	T TMatrix4x4<T>::getDeterminant() const
		throw()
	{
		Position i;
		Position j;
		Position k;
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

	template <typename T>
	void TMatrix4x4<T>::translate(const T& x, const T& y, const T& z)
		throw()
	{
		m14 += m11 * x + m12 * y + m13 * z;
		m24 += m21 * x + m22 * y + m23 * z;
		m34 += m31 * x + m32 * y + m33 * z;
		m44 += m41 * x + m42 * y + m43 * z;
	}

	template <typename T>
	void TMatrix4x4<T>::translate(const TVector3<T>& v)
		throw()
	{
		m14 += m11 * v.x + m12 * v.y + m13 * v.z;
		m24 += m21 * v.x + m22 * v.y + m23 * v.z;
		m34 += m31 * v.x + m32 * v.y + m33 * v.z;
		m44 += m41 * v.x + m42 * v.y + m43 * v.z;
	}

	template <typename T>
	void TMatrix4x4<T>::setTranslation(const T& x, const T& y, const T& z)
		throw()
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

	template <typename T>
	void TMatrix4x4<T>::setTranslation(const TVector3<T>& v)
		throw()
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

	template <typename T>
	void TMatrix4x4<T>::scale(const T& x_scale, const T& y_scale, const T& z_scale)
		throw()
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

	template <typename T>
	void TMatrix4x4<T>::scale(const T& scale)
		throw()
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

	template <typename T>
	void TMatrix4x4<T>::scale(const TVector3<T>& v)
		throw()
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

	template <typename T>
	void TMatrix4x4<T>::setScale(const T& x_scale, const T& y_scale, const T& z_scale)
		throw()
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

	template <typename T>
	void TMatrix4x4<T>::setScale(const T& scale)
		throw()
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

	template <typename T>
	void TMatrix4x4<T>::setScale(const TVector3<T>& v)
		throw()
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

	template <typename T>
	BALL_INLINE 
	void TMatrix4x4<T>::rotateX(const Angle& phi)
		throw()
	{
		TMatrix4x4<T> rotation;

		rotation.setRotationX(phi);
		*this *= rotation;
	}

	template <typename T>
	void TMatrix4x4<T>::setRotationX(const Angle& phi)
		throw()
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

	template <typename T>
	BALL_INLINE 
	void TMatrix4x4<T>::rotateY(const Angle& phi)
		throw()
	{
		TMatrix4x4<T> rotation;

		rotation.setRotationY(phi);
		*this *= rotation;
	}

	template <typename T>
	void TMatrix4x4<T>::setRotationY(const Angle& phi)
		throw()
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

	template <typename T>
	BALL_INLINE 
	void TMatrix4x4<T>::rotateZ(const Angle& phi)
		throw()
	{
		TMatrix4x4<T> rotation;

		rotation.setRotationZ(phi);
		*this *= rotation;
	}

	template <typename T>
	void TMatrix4x4<T>::setRotationZ(const Angle& phi)
		throw()
	{
		m33 = m44 = 1;

		m13 = m14 = m23 = m24 = m31 = 
		m32 = m34 = m41 = m42 = m43 = 0;

		m11 =  m22 = cos(phi);
		m12 = -(m21 = sin(phi));
	}

	template <typename T>
	BALL_INLINE 
	void TMatrix4x4<T>::rotate(const Angle& phi, const TVector3<T>& v)
		throw()
	{
		rotate(phi, v.x, v.y, v.z);
	}

	template <typename T>
	BALL_INLINE 
	void TMatrix4x4<T>::rotate(const Angle& phi, const TVector4<T>& v)
		throw()
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

	template <typename T>
	void TMatrix4x4<T>::rotate(const Angle& phi, const T& ax, const T& ay, const T& az)
		throw()
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

	template <typename T>
	void TMatrix4x4<T>::setRotation(const Angle& phi, const T& x, const T& y, const T& z)
		throw()
	{
		m12 = m13 = m14 = m21 = m23 = m24 = m31 = m32 = m34 = m41 = m42 = m43 = 0;
		m11 = m22 = m33 = m44 = (T)1;
		rotate(phi, x, y, z);
	}

	template <typename T>
	BALL_INLINE 
	void TMatrix4x4<T>::setRotation(const Angle& phi, const TVector3<T>& v)
		throw()
	{
		m12 = m13 = m14 = m21 = m23 = m24 = m31 = m32 = m34 = m41 = m42 = m43 = 0;
		m11 = m22 = m33 = m44 = (T)1;
		rotate(phi, v.x, v.y, v.z);
	}

	template <typename T>
	BALL_INLINE 
	void TMatrix4x4<T>::setRotation(const Angle& phi, const TVector4<T>& v)
		throw()
	{
		m12 = m13 = m14 = m21 = m23 = m24 = m31 = m32 = m34 = m41 = m42 = m43 = 0;
		m11 = m22 = m33 = m44 = (T)1;
		rotate(phi, v.x, v.y, v.z);
	}

	template <typename T>
	bool TMatrix4x4<T>::operator == (const TMatrix4x4<T>& m) const
		throw()
	{
		return 
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

	template <typename T>
	bool TMatrix4x4<T>::operator != (const TMatrix4x4<T>& m) const
		throw()
	{
		return 
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

	template <typename T>
	bool TMatrix4x4<T>::isIdentity() const
		throw()
	{
		return 
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

	template <typename T>
	BALL_INLINE 
	bool TMatrix4x4<T>::isRegular() const
		throw()
	{
		return (getDeterminant() != (T)0);
	}

	template <typename T>
	BALL_INLINE
	bool TMatrix4x4<T>::isSingular() const
		throw()
	{
		return (getDeterminant() == (T)0);
	}

	template <typename T>
	bool TMatrix4x4<T>::isSymmetric() const
		throw()
	{
		return (   m12 == m21 && m13 == m31
						&& m14 == m41 && m23 == m32
						&& m24 == m42 && m34 == m43);
	}

	template <typename T>
	bool TMatrix4x4<T>::isLowerTriangular() const
		throw()
	{
		return (   m12 == (T)0
						&& m13 == (T)0
						&& m14 == (T)0
						&& m23 == (T)0
						&& m24 == (T)0
						&& m34 == (T)0);
	}

	template <typename T>
	bool TMatrix4x4<T>::isUpperTriangular() const
		throw()
	{
		return (   m21 == (T)0
						&& m31 == (T)0
					  && m32 == (T)0
					  && m41 == (T)0
						&& m42 == (T)0
						&& m43 == (T)0);
	}

	template <typename T>
	BALL_INLINE 
	bool TMatrix4x4<T>::isDiagonal() const
		throw()
	{
		return (   m12 == (T)0
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

	template <typename T>
	bool TMatrix4x4<T>::isValid() const
		throw()
	{
		T **ptr = (T **)comp_ptr_;
		
		return (   *ptr++ == &m11
						&& *ptr++ == &m12
						&& *ptr++ == &m13
						&& *ptr++ == &m14
						&& *ptr++ == &m21
						&& *ptr++ == &m22
					  && *ptr++ == &m23
						&& *ptr++ == &m24
						&& *ptr++ == &m31
						&& *ptr++ == &m32
						&& *ptr++ == &m33
						&& *ptr++ == &m34
						&& *ptr++ == &m41
						&& *ptr++ == &m42
						&& *ptr++ == &m43
						&& *ptr   == &m44);
	}

	template <typename T>
	std::istream& operator >> (std::istream& s, TMatrix4x4<T>& m)
		throw()
	{		
		char c;
		s >> c
		  >> m.m11 >> m.m12 >> m.m13 >> m.m14 >> c >> c
		  >> m.m21 >> m.m22 >> m.m23 >> m.m24 >> c >> c
		  >> m.m31 >> m.m32 >> m.m33 >> m.m34 >> c >> c
			>> m.m41 >> m.m42 >> m.m43 >> m.m44 >> c;
		
		return s;
	}

	template <typename T>
	std::ostream& operator << (std::ostream& s, const TMatrix4x4<T>& m)
		throw()
	{ 
		s << '/'  <<  std::setw(14) << m.m11 << ' ' << std::setw(14) << m.m12 << ' ' << std::setw(14) << m.m13 << ' ' << std::setw(14) << m.m14 << " \\" << std::endl
			<< '|'  <<  std::setw(14) << m.m21 << ' ' << std::setw(14) << m.m22 << ' ' << std::setw(14) << m.m23 << ' ' << std::setw(14) << m.m24 << " |"  << std::endl
			<< '|'  <<  std::setw(14) << m.m31 << ' ' << std::setw(14) << m.m32 << ' ' << std::setw(14) << m.m33 << ' ' << std::setw(14) << m.m34 << " |"  << std::endl
			<< '\\' <<  std::setw(14) << m.m41 << ' ' << std::setw(14) << m.m42 << ' ' << std::setw(14) << m.m43 << ' ' << std::setw(14) << m.m44 << " /" << std::endl;

		return s;
	}

	template <typename T>
	void TMatrix4x4<T>::dump(std::ostream& s, Size depth) const
		throw()
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

	///
	template <typename T>
	TMatrix4x4<T> operator * (const T& scalar, const TMatrix4x4<T>& m)
		throw();

	///
	template <typename T>
	TVector3<T> operator * (const TMatrix4x4<T>& matrix, const TVector3<T>& vector)
		throw();

	/**	The Default TMatrix4x4 Type.
			This default is predefined for convenience for those cases where single precision is sufficient.
	*/
	typedef TMatrix4x4<float> Matrix4x4;

	//@}

} // namespace BALL

#endif // BALL_MATHS_MATRIX44_H

// $Id: analyticalGeometry.h,v 1.22 2000/04/03 18:15:35 amoll Exp $

#ifndef BALL_MATHS_ANALYTICALGEOMETRY_H
#define BALL_MATHS_ANALYTICALGEOMETRY_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_MATHS_ANGLE_H
#	include <BALL/MATHS/angle.h>
#endif

#ifndef BALL_MATHS_CIRCLE3_H
#	include <BALL/MATHS/circle3.h>
#endif

#ifndef BALL_MATHS_LINE3_H
#	include <BALL/MATHS/line3.h>
#endif

#ifndef BALL_MATHS_PLANE3_H
#	include <BALL/MATHS/plane3.h>
#endif

#ifndef BALL_MATHS_SPHERE3_H
#	include <BALL/MATHS/sphere3.h>
#endif

#ifndef BALL_MATHS_VECTOR3_H
#	include <BALL/MATHS/vector3.h>
#endif

#define BALL_MATRIX_CELL(m,dim,row,col)   *((m) + (row) * (dim) + (col))
#define BALL_CELL(x,y)                          *((m) + (y) * (dim) + (x))

namespace BALL 
{
	/**	@name Analytical Geometry
			@memo representation of analytical geometry functions,
						using the classes: common, TAngle, TCircle3, TLine3, TPlane3, TSphere3, TVector3.
	*/
	//@{

	/**	Subroutine to get the determinant of any matrix.
			Direct usage of this function should be avoided.
			Instead use {\tt T GetDeterminant(const T* m, Size dim) }
			@param	m pointer to matrix
			@param	dim dimension of the matrix
	*/
	template <class T>
	BALL_INLINE
	T GetDeterminant_(const T* m, Size dim)
	{
		T determinant = 0;
		Index dim1 = dim - 1;

		if (dim > 1)
		{
			T* submatrix = new T[dim1 * dim1];

			for (Index i = 0; i < (Index)dim; ++i) 
			{
				for (Index j = 0; j < dim1; ++j) 
				{
					for (Index k = 0; k < dim1; ++k) 
					{
						*(submatrix + j * dim1 + k) = *(m + (j + 1) * dim + (k < i ? k : k + 1));
					}
				}
				determinant += *(m + i) * (i / 2.0 == i / 2 ? 1 : -1) * GetDeterminant_(submatrix, dim1);
			}

			delete [] submatrix;
		} 
		else 
		{
			determinant = *m;
		}

		return determinant;
	}

	/**	Get the determinant of any matrix.
			@param	m pointer to matrix
			@param	dim dimension of the matrix
	*/
	template <class T>
	BALL_INLINE 
	T GetDeterminant(const T* m, Size dim)
	{
		if (dim == 2)
		{
			return (BALL_CELL(0,0) * BALL_CELL(1,1) - BALL_CELL(0,1) * BALL_CELL(1,0));
		} 
		else if (dim == 3)
		{
			return (  BALL_CELL(0,0) * BALL_CELL(1,1) * BALL_CELL(2,2) 
							+ BALL_CELL(0,1) * BALL_CELL(1,2) * BALL_CELL(2,0) 
							+ BALL_CELL(0,2) * BALL_CELL(1,0) * BALL_CELL(2,1) 
							- BALL_CELL(0,2) * BALL_CELL(1,1) * BALL_CELL(2,0) 
							- BALL_CELL(0,0) * BALL_CELL(1,2) * BALL_CELL(2,1) 
							- BALL_CELL(0,1) * BALL_CELL(1,0) * BALL_CELL(2,2)); 
		} else {
			return GetDeterminant_(m, dim);
		}
	}

	/**	Get the determinant of an 2x2 matrix.
			@param	m pointer to matrix
	*/
	template <class T>
	BALL_INLINE 
	T GetDeterminant2(const T* m)
	{
		Size dim = 2;
		return (BALL_CELL(0,0) * BALL_CELL(1,1) - BALL_CELL(0,1) * BALL_CELL(1,0));
	}
	/**	Get the determinant of an 2x2 matrix.
			@param	m00 first value of the matrix
			@param	m01 second value of the matrix
			@param	m10 third value of the matrix
			@param	m11 fourth value of the matrix
	*/
	template <class T>
	BALL_INLINE 
	T GetDeterminant2(const T& m00, const T& m01, const T& m10, const T& m11)
	{
		return (m00 * m11 - m01 * m10);
	}

	/**	Get the determinant of an 3x3 matrix.
			@param	m pointer to matrix
	*/
	template <class T>
	BALL_INLINE 
	T GetDeterminant3(const T *m)
	{
		Size dim = 3;
		return (  BALL_CELL(0,0) * BALL_CELL(1,1) * BALL_CELL(2,2) 
						+ BALL_CELL(0,1) * BALL_CELL(1,2) * BALL_CELL(2,0) 
						+ BALL_CELL(0,2) * BALL_CELL(1,0) * BALL_CELL(2,1) 
						- BALL_CELL(0,2) * BALL_CELL(1,1) * BALL_CELL(2,0) 
						- BALL_CELL(0,0) * BALL_CELL(1,2) * BALL_CELL(2,1) 
						- BALL_CELL(0,1) * BALL_CELL(1,0) * BALL_CELL(2,2)); 
	}

	/**	Get the determinant of an 3x3 matrix.
			@param	m00 - m22 the elements of the matrix
	*/
	template <class T>
	BALL_INLINE T 
	GetDeterminant3
		(const T& m00, const T& m01, const T& m02,
		 const T& m10, const T& m11, const T& m12,
		 const T& m20, const T& m21, const T& m22)
	{
		return (  m00 * m11 * m22
						+ m01 * m12 * m20
						+ m02 * m10 * m21
						- m02 * m11 * m20
						- m00 * m12 * m21
						- m01 * m10 * m22); 
	}

	/**	Solve a system of equations of a given size in the form
		  $a_1 x_1 + b_1 x_2 + .. + N_1 x_n = c_1$.
			@param	m pointer to the factors in the equations
			@param	x pointer in which the results are stored
			@param  dim the size of the equation system (number of variables)
			@return bool {\tt true} if a solution is found
	*/
	template <class T>
	bool SolveSystem(const T* m, T* x, const Size dim)
	{
		T pivot;
		Index i, j, k, p;
		T* matrix = new T[dim * (dim + 1)];
		const T* source = m;
		T* target = (T*)matrix;
		T* end = (T*)&BALL_MATRIX_CELL(matrix, dim, dim - 1, dim);

		while(target <= end)
		{
			*target++ = *source++;
		}

		for (i = 0; i < (Index)dim; ++i)
		{
			pivot = BALL_MATRIX_CELL(matrix, dim, i, i);
			p = i;
			for (j = i + 1; j < (Index)dim; ++j)
			{
				if (Maths::isLess(pivot, BALL_MATRIX_CELL(matrix, dim, j, i)))
				{
					pivot = BALL_MATRIX_CELL(matrix, dim, j, i);
					p = j;
				}
			}

			if (p != i)
			{
				T tmp;

				for (k = i; k < (Index)dim + 1; ++k)
				{
					tmp = BALL_MATRIX_CELL(matrix, dim, i, k);
					BALL_MATRIX_CELL(matrix, dim, i, k) = BALL_MATRIX_CELL(matrix, dim, p, k);
					BALL_MATRIX_CELL(matrix, dim, p, k) = tmp;
				}
			}
			else if (Maths::isZero(pivot) || Maths::isNan(pivot))
			{ // invariant: matrix m is singular

				delete [] matrix;
				
				return false;
			}

			for (j = dim; j >= i; --j) 
			{
				BALL_MATRIX_CELL(matrix, dim, i, j) /= pivot;
			}

			for (j = i + 1; j < (Index)dim; ++j)
			{
				pivot = BALL_MATRIX_CELL(matrix, dim, j, i);

				for (k = dim; k>= i; --k) 
					BALL_MATRIX_CELL(matrix, dim, j, k) -= pivot * BALL_MATRIX_CELL(matrix, dim, i, k);
			}
		}

		x[dim - 1] = BALL_MATRIX_CELL(matrix, dim, dim - 1, dim);

		for (i = dim - 2; i >= 0; --i)
		{
			x[i] = BALL_MATRIX_CELL(matrix, dim, i, dim);

			for (j = i + 1; j < (Index)dim; ++j) 
			{
				x[i] -= BALL_MATRIX_CELL(matrix, dim, i, j) * x[j];	
			}
		}

		delete [] matrix;
		
		return true;
	}

	#undef BALL_CELL

	/**	Solve a system of two equations of the form
		  a_1 x_1 + b_1 x_2 = c_1
		  a_2 x_1 + b_2 x_2 = c_2.
			@param	a1
			@param	a2
			@param	a1 
			@param	a2
			@param	c1 
			@param	c2
			@param x1 the first solution
			@param x2 the second solution
			@return bool {\tt true} if a solution is found
	*/
	template <class T>
	BALL_INLINE 
	bool SolveSystem2
		(const T& a1, const T& b1, const T& c1,
		 const T& a2, const T& b2, const T& c2,
		 T& x1, T& x2)
	{
		T quot = (a1 * b2 - a2 * b1);

		if (Maths::isZero(quot))
			return false;

		x1 = (c1 * b2 - c2 * b1) / quot;
		x2 = (a1 * c2 - a2 * c1) / quot;

		return true;
	}

	/**	Solve a square equation of the form
			a x^2 + b x + c = 0.
			@param	a
			@param	b 
			@param	c 
			@param x1 the first solution
			@param x2 the second solution
			@return short the number of solutions (0 - 2)
	*/
	template <class T>
	short	SolveSquaredEquality
		(const T& a, const T& b, const T &c,
		 T &x1, T &x2)
	{
		if (a == 0)
		{
			if (b == 0)
			{
				return 0;
			}
			x1 = x2 = c / b;
			return 1;
		}
		T discriminant = b * b - 4 * a * c;

		if (Maths::isLess(discriminant, 0))
			return 0;

		T sqrt_discriminant = sqrt(discriminant);
		if (Maths::isZero(sqrt_discriminant))
		{
			x1 = x2 = -b / (2 * a);

			return 1;
		} else {
			x1 = (-b + sqrt_discriminant) / (2 * a);
			x2 = (-b - sqrt_discriminant) / (2 * a);

			return 2;
		}
	}

	/**	Get the partition of two vectors.
			@param	a the first vector
			@param	b the second vector
			@return TVector3 the partition
	*/
	template <class T>
	BALL_INLINE 
	TVector3<T> GetPartition(const TVector3<T>& a, const TVector3<T>& b)
	{
		return TVector3<T>((b.x + a.x) / 2, (b.y + a.y) / 2, (b.z + a.z) / 2);
	}

	/**	Get the partition of two vectors, calculated
			with two ratio factors.
			@param	a the first vector
			@param	b the second vector
			@param  r the ratio factor of the first vector
			@param  s the ratio factor of the second vector
			@return TVector3 the partition
	*/
	template <class T>
	BALL_INLINE 
	TVector3<T> GetPartition
		(const TVector3<T>& a, const TVector3<T>& b,
		 const T& r, const T& s)
	{
		T sum = r + s;
		if (sum == (T)0)
		{
			throw Exception::DivisionByZero(__FILE__, __LINE__);
		}		
		return TVector3<T>
			((s * a.x + r * b.x) / sum,
			 (s * a.y + r * b.y) / sum,
			 (s * a.z + r * b.z) / sum);
	}

	/**	Get the distance between two points.
			@param	a the first point
			@param	b the second point
			@return T the distance
	*/
	template <class T>
	BALL_INLINE 
	T GetDistance(const TVector3<T>& a, const TVector3<T>& b)
	{
		T dx = a.x - b.x;
		T dy = a.y - b.y;
		T dz = a.z - b.z;
		
		return sqrt(dx * dx + dy * dy + dz * dz); 
	}

	/**	Get the distance between a line and a point.
			@param	line the line
			@param	point the point
			@return T the distance
	*/
	template <class T>
	BALL_INLINE 
	T GetDistance(const TLine3<T>& line, const TVector3<T>& point)
	{
		if (line.d.getLength() == (T)0)
		{
			throw Exception::DivisionByZero(__FILE__, __LINE__);
		}		
		return ((line.d % (point - line.p)).getLength() / line.d.getLength());
	}

	/**	Get the distance between a point and a line.
			@param	point the point
			@param	line the line
			@return T the distance
	*/
	template <class T>
	BALL_INLINE 
	T GetDistance(const TVector3<T>& point, const TLine3<T>& line)
	{
		return GetDistance(line, point);
	}

	/**	Get the distance between two lines.
			@param	a the first line
			@param	b the second line
			@return T the distance
	*/
	template <class T>
	T GetDistance(const TLine3<T>& a, const TLine3<T>& b)
	{
		T cross_product_length = (a.d % b.d).getLength();
		
		if (Maths::isZero(cross_product_length))
		{ // invariant: parallel lines
			if (a.d.getLength() == (T)0)
			{
				throw Exception::DivisionByZero(__FILE__, __LINE__);
			}					
			return ((a.d % (b.p - a.p)).getLength() / a.d.getLength());
		} else {
			T spat_product = TVector3<T>::getSpatProduct(a.d, b.d, b.p - a.p);

			if (Maths::isNotZero(spat_product))
			{ // invariant: windschiefe lines
				
				return (Maths::abs(spat_product) / cross_product_length);
			} else { // invariant: intersecting lines

				return 0;
			}
		}
	}

	/**	Get the distance between a point and a plane.
			@param	point the point
			@param	plane the plane
			@return T the distance
	*/
	template <class T>
	BALL_INLINE 
	T GetDistance(const TVector3<T>& point, const TPlane3<T>& plane)
	{
		T length = plane.n.getLength();

		if (length == (T)0)
		{
			throw Exception::DivisionByZero(__FILE__, __LINE__);
		}
		return (Maths::abs(plane.n * (point - plane.p)) / length);
	}

	/**	Get the distance between a plane and a point.
			@param	plane the plane
			@param	point the point
			@return T the distance
	*/
	template <class T>
	BALL_INLINE 
	T GetDistance(const TPlane3<T>& plane, const TVector3<T>& point)
	{
		return GetDistance(point, plane);
	}


	/**	Get the distance between a line and a plane.
			@param	line the line
			@param	plane the plane
			@return T the distance
	*/
	template <class T>
	BALL_INLINE 
	T GetDistance(const TLine3<T>& line, const TPlane3<T>& plane)
	{
		T length = plane.n.getLength();
		if (length == (T)0)
		{
			throw Exception::DivisionByZero(__FILE__, __LINE__);
		}
		return (Maths::abs(plane.n * (line.p - plane.p)) / length);
	}

	/**	Get the distance between a plane and a line.
			@param	plane the plane
			@param	line the line
			@return T the distance
	*/
	template <class T>
	BALL_INLINE 
	T GetDistance(const TPlane3<T>& plane, const TLine3<T>& line)
	{
		return GetDistance(line, plane);
	}

	/**	Get the distance between two planes.
			@param	plane the plane
			@param	line the line
			@return T the distance
	*/
	template <class T>
	BALL_INLINE 
	T GetDistance(const TPlane3<T>& a, const TPlane3<T>& b)
	{
		T length = a.n.getLength();
		if (length == (T)0)
		{
			throw Exception::DivisionByZero(__FILE__, __LINE__);
		}		
		return (Maths::abs(a.n * (a.p - b.p)) / length);
	}

	/**	Get the angle between two Vector3.
			@param	a the Vector3
			@param	b the Vector3
			@param	intersection_angle the resulting angle
			@return bool, always true
	*/
	template <class T>
	BALL_INLINE 
	bool GetAngle(const TVector3<T>& a, const TVector3<T>& b,
					  		TAngle<T> &intersection_angle)
	{
		T length_product = a.getSquareLength() *  b.getSquareLength();
		if(Maths::isZero(length_product))
			return false;

		intersection_angle = a.getAngle(b);

		return true;
	}

	/**	Get the angle between two lines.
			@param	a the first line
			@param	b the second line
			@param	intersection_angle the resulting angle
			@return bool, true if an angle can be calculated, otherwise false
	*/
	template <class T>
	BALL_INLINE 
	bool GetAngle(const TLine3<T>& a, const TLine3<T>& b, TAngle<T>& intersection_angle)
	{
		T length_product = a.d.getSquareLength() *  b.d.getSquareLength();

		if(Maths::isZero(length_product))
			return false;
		
		intersection_angle = acos(Maths::abs(a.d * b.d) / sqrt(length_product));

		return true;
	}

	/**	Get the angle between a plane and a Vector3.
			@param	plane the plane
			@param	vector the Vector3 
			@param	intersection_angle the resulting angle
			@return bool, true if an angle can be calculated, otherwise false
	*/
	template <class T>
	BALL_INLINE 
	bool GetAngle(const TPlane3<T>& plane, const TVector3<T> &Vector3,
					 		  TAngle<T> &intersection_angle)
	{
		T length_product = plane.n.getSquareLength() * Vector3.getSquareLength();
		
		if (Maths::isZero(length_product))
		{
			return false;
		} else {
			intersection_angle = asin(Maths::abs(plane.n * Vector3) / sqrt(length_product));
			
			return true;
		}
	}

	/**	Get the angle between a vector3 and a plane.
			@param	vector the vector3
			@param	plane the plane
			@param	intersection_angle the resulting angle
			@return bool, true if an angle can be calculated, otherwise false
	*/
	template <class T>
	BALL_INLINE 
	bool GetAngle
		(const TVector3<T>& vector ,const TPlane3<T>& plane,
		 TAngle<T> &intersection_angle)
	{
		return GetAngle(plane, vector, intersection_angle);
	}

	/**	Get the angle between a plane and a line.
			@param	plane the plane
			@param	line the line
			@param	intersection_angle the resulting angle
			@return bool, true if an angle can be calculated, otherwise false
	*/
	template <class T>
	BALL_INLINE 
	bool GetAngle
		(const TPlane3<T>& plane,const TLine3<T>& line,
		 TAngle<T>& intersection_angle)
	{
		T length_product = plane.n.getSquareLength() * line.d.getSquareLength();
		
		if (Maths::isZero(length_product))
		{
			return false;
		} else {
			intersection_angle = asin(Maths::abs(plane.n * line.d) / sqrt(length_product));
			
			return true;
		}
	}

	/**	Get the angle between a line and a plane.
			@param	line the line
			@param	plane the plane
			@param	intersection_angle the resulting angle
			@return bool, true if an angle can be calculated, otherwise false
	*/
	template <class T>
	BALL_INLINE 
	bool GetAngle
		(const TLine3<T>& line, const TPlane3<T>& plane,
		 TAngle<T>& intersection_angle)
	{
		return GetAngle(plane, line, intersection_angle);
	}


	/**	Get the angle between two planes.
			@param	a the first plane
			@param	b the second plane
			@param	intersection_angle the resulting angle
			@return bool, true if an angle can be calculated, otherwise false
	*/
	template <class T>
	BALL_INLINE 
	bool GetAngle
		(const TPlane3<T>& a, const TPlane3<T>& b,
		 TAngle<T>& intersection_angle)
	{
		T length_product = a.n.getSquareLength() * b.n.getSquareLength();

		if(Maths::isZero(length_product))
			return false;
		
		intersection_angle = acos(Maths::abs(a.n * b.n) / sqrt(length_product));

		return true;
	}

	/**	Get the intersection point between two lines.
			@param	a the first line
			@param	b the second line
			@param	point the resulting intersection
			@return bool, true if an intersection can be calculated, otherwise false
	*/
	template <class T>
	bool GetIntersection
		(const TLine3<T>& a, const TLine3<T>& b,
		 TVector3<T>& point)
	{
		T c1, c2;

		if ((SolveSystem2(a.d.x, -b.d.x, b.p.x - a.p.x, a.d.y,  -b.d.y, b.p.y - a.p.y,
						 				  c1, c2) == true
				 && Maths::isEqual(a.p.z + a.d.z * c1, b.p.z + b.d.z * c2))

				|| (SolveSystem2(a.d.x, -b.d.x, b.p.x - a.p.x, a.d.z, -b.d.z, b.p.z - a.p.z,
						 						 c1, c2) == true
						&& Maths::isEqual(a.p.y + a.d.y * c1, b.p.y + b.d.y * c2))

				|| (SolveSystem2(a.d.y, -b.d.y, b.p.y - a.p.y, a.d.z, -b.d.z, b.p.z - a.p.z,
				 								 c1, c2) == true
						&& Maths::isEqual(a.p.x + a.d.x * c1, b.p.x + b.d.x * c2)))
		{
			point.set(a.p.x + a.d.x * c1, a.p.y + a.d.y * c1, a.p.z + a.d.z * c1);
			
			return true;
		} else {
			return false;
		}
	}

	/**	Get the intersection point between a plane and a line.
			@param	plane the plane
			@param	line the line
			@param	point the resulting intersection
			@return bool, true if an intersection can be calculated, otherwise false
	*/
	template <class T>
	BALL_INLINE 
	bool GetIntersection
		(const TPlane3<T>& plane, const TLine3<T>& line,
		 TVector3<T>& intersection_point)
	{
		T dot_product = plane.n * line.d;

		if (Maths::isZero(dot_product))
		{
			return false;
		} 
		else 
		{
			intersection_point.set(line.p + (plane.n * (plane.p - line.p)) * line.d / dot_product);

			return true;
		}
	}

	/**	Get the intersection point between a line and a plane.
			@param	line the line
			@param	plane the plane
			@param	point the resulting intersection
			@return bool, true if an intersection can be calculated, otherwise false
	*/
	template <class T>
	BALL_INLINE 
	bool GetIntersection
		(const TLine3<T>& line, const TPlane3<T>& plane,
		 TVector3<T> &intersection_point)
	{
		return GetIntersection(plane, line, intersection_point);
	}

	/**	Get the intersection line between two planes.
			@param	a the first plane
			@param	b the second plane
			@param	line the resulting intersection
			@return bool, true if an intersection can be calculated, otherwise false
	*/
	template <class T>
	bool GetIntersection
		(const TPlane3<T>& a, const TPlane3<T>& b, TLine3<T>& line)
	{
		TVector3<T> p;
		T aa, ab, ac, ad;
		T ba, bb, bc, bd;
		T x1, x2;

		aa = a.n.x;
		ab = a.n.y;
		ac = a.n.z;
		ad = -(a.n * a.p);

		ba = b.n.x;
		bb = b.n.y;
		bc = b.n.z;
		bd = -(b.n * b.p);

		if (SolveSystem2(aa, ab, -ad, ba, bb, -bd,x1, x2) == false)
		{
			if (SolveSystem2(aa, ac, -ad, ba, bc, -bd, x1, x2) == false)
			{
				if (SolveSystem2(ab, ac, -ad, bb, bc, -bd, x1, x2) == false)
				{
					return false;
				} else {
					p.set(0, x1, x2);
				}
			} else {
				p.set(x1, 0, x2);
			}
		} else {
			p.set(x1, x2, 0);
		}

		line.set(p, a.n % b.n);

		return true;
	}
	/**	Get the intersection point between a sphere and a line.
			@param	sphere the sphere
			@param	line the line
			@param	intersection_point1 the first intersection point
			@param	intersection_point2 the second intersection point
			@return bool, true if an intersection can be calculated, otherwise false
	*/
	template <class T>
	bool GetIntersection
		(const TSphere3<T>& sphere, const TLine3<T>& line,
		 TVector3<T>& intersection_point1, TVector3<T>& intersection_point2)
	{
		T x1, x2;
		short number_of_solutions 
			= SolveSquaredEquality
					(line.d * line.d,
					 (line.p - sphere.p) * line.d * 2,
					 (line.p - sphere.p) * (line.p - sphere.p) - sphere.radius * sphere.radius,
					 x1, x2);

		if (number_of_solutions == 0)
		{
			return false;
		}

		intersection_point1 = line.p + x1 * line.d;
		intersection_point2 = line.p + x2 * line.d;

		return true;
	}

	/**	Get the intersection point between a line and a sphere.
			@param	line the line
			@param	sphere the sphere
			@param	intersection_point1 the first intersection point
			@param	intersection_point2 the second intersection point
			@return bool, true if an intersection can be calculated, otherwise false
	*/
	template <class T>
	BALL_INLINE 
	bool GetIntersection
		(const TLine3<T>& line, const TSphere3<T>& sphere,
		 TVector3<T>& intersection_point1, TVector3<T>& intersection_point2)
	{
		return GetIntersection(sphere, line, intersection_point1, intersection_point2);
	}

	/**	Get the intersection circle between a sphere and a plane.
			@param	sphere the sphere
			@param	plane the plane
			@param	intersection_circle the intersection circle
			@return bool, true if an intersection can be calculated, otherwise false
	*/
	template <class T>
	bool GetIntersection
		(const TSphere3<T>& sphere, const TPlane3<T>& plane,
		 TCircle3<T>& intersection_circle)
	{
		T distance = GetDistance(sphere.p, plane);

		if (Maths::isGreater(distance, sphere.radius))
		{
			return false;
		}
		else if (Maths::isEqual(distance, sphere.radius))
		{
			TVector3<T> Vector3(plane.n);

			Vector3.normalize();

			intersection_circle.set(sphere.p + sphere.radius * Vector3, plane.n, 0);

			return true;
		} else {
			TVector3<T> Vector3(plane.n);

			Vector3.normalize();

			intersection_circle.set
				(sphere.p + distance * Vector3,
				 plane.n,
				 sqrt(sphere.radius * sphere.radius - distance * distance));

			return true;
		}
	}

	/**	Get the intersection circle between a plane and a sphere.
			@param	plane the plane
			@param	sphere the sphere
			@param	intersection_circle the intersection circle
			@return bool, true if an intersection can be calculated, otherwise false
	*/
	template <class T>
	BALL_INLINE bool
	GetIntersection
		(const TPlane3<T>& plane, const TSphere3<T>& sphere,
		 TCircle3<T>& intersection_circle)
	{
		return GetIntersection(sphere, plane, intersection_circle);
	}
	 
	/**	Get the intersection circle between two spheres.
			@param	a the first sphere
			@param	intersection_circle the intersection circle
			@return bool, true if an intersection can be calculated, otherwise false
	*/
	template <class T>
	bool GetIntersection
		(const TSphere3<T>& a, const TSphere3<T>& b, 
		 TCircle3<T>& intersection_circle)
	{
		TPlane3<T> plane
			(-2 * b.p.x + 2 * a.p.x,
			 -2 * b.p.y + 2 * a.p.y,
			 -2 * b.p.z + 2 * a.p.z,
				 b.p.x * b.p.x 
			 + b.p.y * b.p.y 
			 + b.p.z * b.p.z
			 - b.radius * b.radius
			 - a.p.x * a.p.x 
			 - a.p.y * a.p.y 
			 - a.p.z * a.p.z
			 + a.radius * a.radius);

		plane.hessify();


		T d = plane.n * plane.p;

		intersection_circle.set(d * plane.n, plane.n, sqrt(a.radius * a.radius - d * d));

		return true;
	}

	/**	Test if two vector3 are collinear
			@param	a the first vector3
			@param	b the second vector3
			@return bool, true or false
	*/
	template <class T>
	BALL_INLINE 
	bool isCollinear(const TVector3<T>& a, const TVector3<T>& b)
	{
		return (a % b).isZero();
	}

	/**	Test if three vector3 are complanar
			@param	a the first vector3
			@param	b the second vector3
			@param	c the third vector3
			@return bool, true or false
	*/
	template <class T>
	BALL_INLINE 
	bool isComplanar(const TVector3<T>& a, const TVector3<T>& b, const TVector3<T>& c)
	{
		return Maths::isZero(TVector3<T>::getSpatProduct(a, b, c));
	}

	/**	Test if four vector3 are complanar
			@param	a the first vector3
			@param	b the second vector3
			@param	c the third vector3
			@param	d the fourth vector3
			@return bool, true or false
	*/
	template <class T>
	BALL_INLINE 
	bool isComplanar(const TVector3<T>& a, const TVector3<T>& b, const TVector3<T>& c, const TVector3<T>& d)
	{
		return isComplanar(a - b, a - c, a - d);
	}

	/**	Test if two vector3 are orthogonal
			@param	a the first vector3
			@param	b the second vector3
			@return bool, true or false
	*/
	template <class T>
	BALL_INLINE 
	bool isOrthogonal(const TVector3<T>& a, const TVector3<T>& b)
	{
		return Maths::isZero(a * b);
	}

	/**	Test if a vector3 and a line are orthogonal
			@param	vector the vector
			@param	line the line
			@return bool, true or false
	*/
	template <class T>
	BALL_INLINE 
	bool isOrthogonal(const TVector3<T>& vector, const TLine3<T>& line)
	{
		return Maths::isZero(vector * line.d);
	}

	/**	Test if a line and a vector3 are orthogonal
			@param	line the line
			@param	vector the vector
			@return bool, true or false
	*/
	template <class T>
	BALL_INLINE 
	bool isOrthogonal(const TLine3<T>& line, const TVector3<T>& vector)
	{
		return isOrthogonal(vector, line);
	}

	/**	Test if two lines are orthogonal.
			@param	a the first line
			@param	b the second line
			@return bool, true or false
	*/
	template <class T>
	BALL_INLINE 
	bool isOrthogonal(const TLine3<T>& a, const TLine3<T>& b)
	{
		return Maths::isZero(a.d * b.d);
	}

	/**	Test if a vector3 and a plane are orthogonal.
			@param	vector the vector3
			@param	plane the plane
			@return bool, true or false
	*/
	template <class T>
	BALL_INLINE 
	bool isOrthogonal(const TVector3<T>& vector, const TPlane3<T>& plane)
	{
		return isCollinear(vector, plane.n);
	}

	/**	Test if a plane and a vector3 are orthogonal.
			@param	plane the plane
			@param	vector the vector3
			@return bool, true or false
	*/
	template <class T>
	BALL_INLINE 
	bool isOrthogonal(const TPlane3<T>& plane, const TVector3<T>& vector)
	{
		return isOrthogonal(vector, plane);
	}

	/**	Test if two planes are orthogonal.
			@param	a the first plane
			@param	b the second plane
			@return bool, true or false
	*/
	template <class T>
	BALL_INLINE 
	bool isOrthogonal(const TPlane3<T>& a, const TPlane3<T>& b)
	{
		return Maths::isZero(a.n * b.n);
	}

	/**	Test if a line is intersecting a point.
			@param	point the point
			@param	line the line
			@return bool, true or false
	*/
	template <class T>
	BALL_INLINE 
	bool isIntersecting(const TVector3<T>& point, const TLine3<T>& line)
	{
		return Maths::isZero(GetDistance(point, line));
	}

	/**	Test if a line is intersecting a point.
			@param	line the line
			@param	point the point
			@return bool, true or false
	*/
	template <class T>
	BALL_INLINE 
	bool isIntersecting(const TLine3<T>& line, const TVector3<T>& point)
	{
		return isIntersecting(point, line);
	}

	/**	Test if two line are intersecting.
			@param	a the first line
			@param	b the second line
			@return bool, true or false
	*/
	template <class T>
	BALL_INLINE 
	bool isIntersecting(const TLine3<T>& a, const TLine3<T>& b)
	{
		return Maths::isZero(GetDistance(a, b));
	}

	/**	Test if a point is intersecting a plane.
			@param	point the point
			@param	plane the plane
			@return bool, true or false
	*/
	template <class T>
	BALL_INLINE 
	bool isIntersecting(const TVector3<T>& point, const TPlane3<T>& plane)
	{
		return Maths::isZero(GetDistance(point, plane));
	}

	/**	Test if a point is intersecting a plane.
			@param	plane the plane
			@param	point the point
			@return bool, true or false
	*/
	template <class T>
	BALL_INLINE 
	bool isIntersecting(const TPlane3<T>& plane, const TVector3<T>& point)
	{
		return isIntersecting(point, plane);
	}

	/**	Test if a line is intersecting a plane.
			@param	line the line
			@param	plane the plane
			@return bool, true or false
	*/
	template <class T>
	BALL_INLINE 
	bool isIntersecting(const TLine3<T>& line, const TPlane3<T>& plane)
	{
		return Maths::isZero(GetDistance(line, plane));
	}

	/**	Test if a plane is intersecting a line.
			@param	plane the plane
			@param	line the line
			@return bool, true or false
	*/
	template <class T>
	BALL_INLINE 
	bool isIntersecting(const TPlane3<T>& plane, const TLine3<T>& line)
	{
		return isIntersecting(line, plane);
	}

	/**	Test if two planes are intersecting.
			@param	a the first plane
			@param	b the second plane
			@return bool, true or false
	*/
	template <class T>
	BALL_INLINE 
	bool isIntersecting(const TPlane3<T>& a, const TPlane3<T>& b)
	{
		return Maths::isZero(GetDistance(a, b));
	}

	/**	Test if a line and a plane are parallel.
			@param	line the line
			@param	plane the plane
			@return bool, true or false
	*/
	template <class T>
	BALL_INLINE 
	bool isParallel(const TLine3<T>& line, const TPlane3<T>& plane)
	{
		return isOrthogonal(line.d, plane.n);
	}

	/**	Test if a plane and a line are parallel.
			@param	plane the plane
			@param	line the line
			@return bool, true or false
	*/
	template <class T>
	BALL_INLINE 
	bool isParallel(const TPlane3<T>& plane, const TLine3<T>& line)
	{
		return isParallel(line, plane);
	}

	/**	Test if two planes are parallel.
			@param	a the first plane
			@param	b the second plane
			@return bool, true or false
	*/
	template <class T>
	BALL_INLINE 
	bool isParallel(const TPlane3<T>& a, const TPlane3<T>& b)
	{
		return isCollinear(a.n, b.n);
	}

	/**	Return the torsion angle of four points to eachother.
			@param TVector3& ax 1. vector x component
			@param TVector3& ay 1. vector y component
			@param TVector3& az 1. vector z component
			@param TVector3& bx 2. vector x component
			@param TVector3& by 2. vector y component
			@param TVector3& bz 2. vector z component
			@param TVector3& cx 3. vector x component
			@param TVector3& cy 3. vector y component
			@param TVector3& cz 3. vector z component
			@param TVector3& dx 4. vector x component
			@param TVector3& dy 4. vector y component
			@param TVector3& dz 4. vector z component
			@return TAngle the torsion angle
	*/
	template <typename T>
	TAngle<T> getTorsionAngle
		(const T& ax, const T& ay, const T& az,
		 const T& bx, const T& by, const T& bz,
		 const T& cx, const T& cy, const T& cz, 
		 const T& dx, const T& dy, const T& dz)
	{
		T abx = ax - bx;
		T aby = ay - by;
		T abz = az - bz;

		T cbx = cx - bx;
		T cby = cy - by;
		T cbz = cz - bz;

		T cdx = cx - dx;
		T cdy = cy - dy;
		T cdz = cz - dz;

		// Calculate the normals to the two planes n1 and n2
		// this is given as the cross products:
		//		 AB x BC
		//		--------- = n1
		//		|AB x BC|
		//
		//		 BC x CD
		// 	  --------- = n2
		// 	  |BC x CD|

		// Normal to plane 1 
		T ndax = aby * cbz - abz * cby; 
		T nday = abz * cbx - abx * cbz;
		T ndaz = abx * cby - aby * cbx;

		// Normal to plane 2 
		T neax = cbz * cdy - cby * cdz; 
		T neay = cbx * cdz - cbz * cdx;
		T neaz = cby * cdx - cbx * cdy;

		// Calculate the length of the two normals 
		T bl = (T) sqrt((double)ndax * ndax + nday * nday + ndaz * ndaz);
		T el = (T) sqrt((double)neax * neax + neay * neay + neaz * neaz);
		T bel = (T) (ndax * neax + nday * neay + ndaz * neaz);
		
		// if one or both planes are degenerated
		if (bl * el == 0)
		{
			throw Exception::DivisionByZero(__FILE__, __LINE__);
		}
		bel /= (bl * el);
		if (bel > 1.0) 
		{
			bel = 1;
		} 
		else if (bel < -1.0) 
		{
			bel = -1;
		}

		T acosbel = (T) acos(bel);

		if ((cbx * (ndaz * neay - nday * neaz) 
				 + cby * (ndax * neaz - ndaz * neax) 
				 + cbz * (nday * neax - ndax * neay))
				< 0)
		{
			acosbel = -acosbel;
		}
		
		acosbel = (acosbel > 0.0) 
			? Constants::PI - acosbel 
			: -(Constants::PI + acosbel);
		
		return TAngle<T>(acosbel);
	}
	//@}

} // namespace BALL


#endif // BALL_MATHS_ANALYTICALGEOMETRY_H

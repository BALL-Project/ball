// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: complex.h,v 1.10 2002/10/12 17:06:47 oliver Exp $

#ifndef BALL_MATHS_COMPLEX_H
#define BALL_MATHS_COMPLEX_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifdef BALL_HAS_FFTW
# include <fftw.h>
#endif

#include <math.h>
#include <complex>
#include <iostream>

namespace BALL 
{
	/**	@name	Complex numbers.
	*/
	//@{

	template <typename T>
	class TComplex;

	/**	@name	Global binary operators for complex numbers.
	*/
	//@{

	/** Multiply a real number with a complex number. 
			The symmetric case is a member of the \Ref{TComplex} class.
	*/
	template <typename T1, typename T2>
	BALL_INLINE 
	TComplex<T2> operator * (const T1& r, const TComplex<T2>& cpx)
		throw();

	/** Input stream.
	*/
	template <typename T>
	std::istream& operator >> (std::istream& s, TComplex<T>& cpx)
		throw();

	/* Output stream.
	*/
	template <typename T>
	std::ostream& operator << (std::ostream& s, const TComplex<T>& complex)
		throw();
	
	//@}

	/** Generic Complex Number.
      {\bf Definition:} \URL{BALL/MATHS/complex.h}
      \\
	*/
	template <typename T>
	class TComplex
	{
		public:

		/**	@name	Constructors and Destructors
		*/
		//@{

		/**	Default constructor.
				This method creates a new complex object. The two real and imaginary part
				are initialized to {\tt (T)0}.
		*/
		TComplex()
			throw();

		/**	Constructor from ANSI C++ complex type.
		*/
		TComplex(const std::complex<T>& c) 
			throw();

		/**	Detailed constructor.
				Create a new complex number from a real and imaginary value.
				The imaginary part may be omitted, in which case it is set to zero.
				@param	new_re assigned to {\tt re}
				@param  new_im assigned to {\tt im}
		*/
		TComplex(const T& new_re, const T& new_im = (T)0)
			throw();

		/**	Copy constructor.
				@param cpx the complex number to be copied
		*/	
		TComplex(const TComplex& cpx)
			throw();

		/**	Destructor.	
				Destructs the complex number. For efficiency reasons,
				TComplex does not provide any virtual methods, so the
				destructor is non-virtual as well. The destructor has no effect.
		*/	
		~TComplex()
			throw();

		//@}

		/**	@name	Assignment 
		*/
		//@{

		/**	Assignment operator.
				@param cpx the complex number to assign from
		**/
		const TComplex& operator = (const TComplex& cpx)
			throw();

		/** Scalar assignment operator.
				Assign the real part and set the imaginary part to zero.
				@param value the constant to assign to {\tt re}
		**/
		const TComplex& operator = (const T& value)
			throw();

		/**	Array assignment operator.
				Assigns the first two elements of an array to the real and imaginary part.
				@param	ptr the array
				@exception	NullPointer if {\tt ptr == 0}
		*/
		const TComplex& operator = (const T* ptr)
			throw(Exception::NullPointer);

		/** Clear method
				Real and imaginary part are set to 0.
		*/
		void clear()
			throw();

		//@}


		/**	@name	Arithmetic operators
		*/
		//@{

		/**	Positive sign.
		*/
		const TComplex& operator + () const
			throw();

		/**	Negative sign.
		*/
		TComplex operator - () const
			throw();

		/** Addition.
		*/
		TComplex operator + (const TComplex& cpx) const
			throw();

		/** Subtraction.
		*/
		TComplex operator - (const TComplex& cpx) const
			throw();

		/**	Add two complex numbers.
				Add the real and imaginary parts of the complex numbers.
				@param cpx the complex numer to add
				@return TComplex&, {\em *this}
		*/
		const TComplex& operator += (const TComplex& cpx)
			throw();

		/**	Subtract a complex number.
				@param cpx the number to subtract
		*/
		const TComplex& operator -= (const TComplex& cpx)
			throw();

		/**	Subtract a real number.
				@param cpx the number to subtract
		*/
		const TComplex& operator -= (const T& r)
			throw();

		/**	Multiply by a real number.
				@param scalar, the scalar to multiply by
				@return TComplex, the product of the two numbers
		*/
		TComplex operator * (const T& r) const
			throw();

		/**	Multiply by a complex number.
				@param cpx, the complex number to multiply by
				@return TComplex, the product of the two numbers
		*/
		TComplex operator * (const TComplex& r) const
			throw();

		/**	Multiply by a real number.
				@param real the number to multiply by
				@return TComplex&, {\em *this}
		*/
		const TComplex& operator *= (const T& real)
			throw();

		/**	Multiply by a complex number.
				@param cpx the to multiply by
				@return TComplex&, {\em *this}
		*/
		const TComplex& operator *= (const TComplex<T>& cpx)
			throw();

		/**	Divide by a real number.
				@exception Exception::DivisionByZero if {\tt r == (T)0}
		*/
		TComplex operator / (const T& r) const
			throw(Exception::DivisionByZero);

		/**	Divide by a real number.
				@exception Exception::DivisionByZero if {\tt r == (T)0}
		*/
		const TComplex& operator /= (const T& r)
			throw(Exception::DivisionByZero);

		/**	Divide by a complex number.
				@exception Exception::DivisionByZero if {\tt cpx == (T)0}
		*/
		TComplex operator / (const TComplex<T>& cpx) const
			throw(Exception::DivisionByZero);

		/**	Divide by a complex number.
				@exception Exception::DivisionByZero if {\tt cpx == (T)0}
		*/
		const TComplex& operator /= (const TComplex<T>& cpx)
			throw(Exception::DivisionByZero);

		/**	Conjugate.
				Return the complex conjugate.
		*/
		TComplex conjugate() const throw();
		//@}

		/**	@name	Predicates
		*/
		//@{

		/**	Equality operator.
				@return bool, {\bf true} if real and imaginary parts are equal, {\bf false} otherwise
		*/
		bool operator == (const TComplex& cpx) const
			throw();
	
		/**	Inequality operator.
				@return bool, {\bf true} if the two numbers differ in either the imaginary or the real part, {\bf false} otherwise
		*/
		bool operator != (const TComplex& vector) const
			throw();

		/**	Equality operator.
				@return bool, {\bf true} if real part equals {\tt r} and the imaginary part is zero, {\bf false} otherwise
		*/
		bool operator == (const T& r) const
			throw();
	
		/**	Inequality operator.
				@return bool, {\bf true} if real part differs from {\tt r} or the imaginary part is non-zero, {\bf false} otherwise
		*/
		bool operator != (const T& r) const
			throw();

		/**	Zero predicate.
				@return {\bf true}, if real and imaginary part are zero.
				@see Maths::isZero
		*/
		bool isZero() const
			throw();

		/**	Real predicate.
				@return {\bf true}, if the imaginary part is zero.
				@see Maths::isZero
		*/
		bool isReal() const
			throw();

		//@}


		/**	@name	Attributes
				For easier access, the real and imaginary part are
				are public attributes.
		*/
		//@{
	
		/**	Real part of the complex number.
		*/
		T re;

		/**	Imaginary part of the complex number.
		*/
		T im;

		//@}

		/**	@name Convertors.
		*/
		//@{
		/** Conversion to ANSI C++ complex type
		*/
		operator std::complex<T> () 
			throw();
		//@}
	};

	/**	@name Default complex type.
	*/
	typedef TComplex<BALL_COMPLEX_PRECISION> Complex;
		
	//@}

	template <typename T>
	BALL_INLINE
	TComplex<T>::TComplex() throw()
		:	re((T)0),
			im((T)0)
	{
	}
		
	template <typename T>
	BALL_INLINE
	TComplex<T>::TComplex(const std::complex<T>& c) 
		throw()
		:	re(c.real()),
			im(c.img())
	{
	}
		
	template <typename T>
	BALL_INLINE
	TComplex<T>::TComplex(const T& new_re, const T& new_im) throw()
		:	re(new_re),
			im(new_im)
	{
	}

	template <typename T>
	BALL_INLINE
	TComplex<T>::TComplex(const TComplex<T>& cpx) throw()
		:	re(cpx.re),
			im(cpx.im)
	{
	}
	
	template <typename T>
	BALL_INLINE
	TComplex<T>::~TComplex() throw()
	{
	}

	template <typename T>
	BALL_INLINE
	const TComplex<T>& TComplex<T>::operator = (const TComplex<T>& rhs) throw()
	{
		re = rhs.re;
		im = rhs.im;
		return *this;
	}

	template <typename T>
	BALL_INLINE
	const TComplex<T>& TComplex<T>::operator = (const T& value) throw()
	{
		re = value;
		im = (T)0;
		return *this;
	}

	template <typename T>
	BALL_INLINE
	const TComplex<T>& TComplex<T>::operator = (const T* ptr)
		throw(Exception::NullPointer)
	{
		if (ptr == 0)
		{
			throw Exception::NullPointer(__FILE__, __LINE__);
		}

		re = *++ptr;
		im = *ptr;

		return *this;
	}

	template <typename T>
	BALL_INLINE
	void TComplex<T>::clear() throw()
	{
		re = im = (T)0;
	}

	template <typename T>
	BALL_INLINE
	const TComplex<T>& TComplex<T>::operator + () const throw()
	{
		return *this;
	}

	template <typename T>
	BALL_INLINE
	TComplex<T> TComplex<T>::operator - () const throw()
	{
		return TComplex<T>(-re, -im);
	}
	
	template <typename T>
	BALL_INLINE
	TComplex<T> TComplex<T>::operator + (const TComplex<T>& cpx) const throw()
	{
		return TComplex<T>(re + cpx.re, im + cpx.im);
	}

	template <typename T>
	BALL_INLINE
	const TComplex<T>& TComplex<T>::operator += (const TComplex<T>& cpx) throw()
	{
		re += cpx.re;
		im += cpx.im;
		return *this;
	}

	template <typename T>
	BALL_INLINE
	TComplex<T> TComplex<T>::operator - (const TComplex<T>& cpx) const throw()
	{
		return TComplex<T>(re - cpx.re, im - cpx.im);
	}

	template <typename T>
	BALL_INLINE
	const TComplex<T>& TComplex<T>::operator -= (const TComplex<T>& cpx) throw()
	{
		re -= cpx.re;
		im -= cpx.im;
		return *this;
	}

	template <typename T>
	BALL_INLINE
	TComplex<T> TComplex<T>::operator * (const T& r) const throw()
	{
		return TComplex<T>(re * r, im * r);
	}

	template <typename T>
	BALL_INLINE
	TComplex<T> TComplex<T>::operator * (const TComplex<T>& cpx) const throw()
	{
		return TComplex<T>(re * cpx.re - im * cpx.im, re * cpx.im + im * cpx.re);
	}

	template <typename T>
	BALL_INLINE
  const TComplex<T>& TComplex<T>::operator *= (const T& real)
    throw()
	{
		re = re * real;
		im = im * real;
		return *this;
	}

	template <typename T>
	BALL_INLINE
  const TComplex<T>& TComplex<T>::operator *= (const TComplex<T>& cpx)
    throw()
	{
		T old_re = re;
		re = re * cpx.re - im * cpx.im;
		im = old_re * cpx.im + im * cpx.re;

		return *this;
	}

	template <typename T>
	BALL_INLINE
	TComplex<T> TComplex<T>::operator / (const TComplex<T>& cpx) const
		throw(Exception::DivisionByZero)
	{
		T u2_v2(cpx.im * cpx.im + cpx.re * cpx.re);
		if (u2_v2 == (T)0)
		{
			throw Exception::DivisionByZero(__FILE__, __LINE__);
		}
		return TComplex<T>((re * cpx.re + im * cpx.im) / u2_v2,
											 (-re * cpx.im + im * cpx.re) / u2_v2);
	}

	template <typename T>
	BALL_INLINE
	TComplex<T> TComplex<T>::operator / (const T& r) const
		throw(Exception::DivisionByZero)
	{
		if (r == (T)0)
		{
			throw Exception::DivisionByZero(__FILE__, __LINE__);
		}
		return TComplex<T>(re / r, im / r);
	}

	template <typename T>
	BALL_INLINE
	const TComplex<T>& TComplex<T>::operator /= (const TComplex<T>& cpx)
		throw(Exception::DivisionByZero)
	{
		T u2_v2(cpx.im * cpx.im + cpx.re * cpx.re);
		if (u2_v2 == (T)0)
		{
			throw Exception::DivisionByZero(__FILE__, __LINE__);
		}

		T tmp((re * cpx.re + im * cpx.im) / u2_v2);
		im = (-re * cpx.im + im * cpx.re) / u2_v2;
		re = tmp;

		return *this;
	}

	template <typename T>
	BALL_INLINE
	const TComplex<T>& TComplex<T>::operator /= (const T& r)
		throw(Exception::DivisionByZero)
	{
		if (r == (T)0)
		{
			throw Exception::DivisionByZero(__FILE__, __LINE__);
		}

		re /= r;
		im /= r;

		return *this;
	}

	template <typename T>
	BALL_INLINE
	bool TComplex<T>::operator == (const TComplex& cpx) const throw()
	{
		return (re == cpx.re) && (im == cpx.im);
	}

	template <typename T>
	BALL_INLINE
	bool TComplex<T>::operator != (const TComplex& cpx) const throw()
	{
		return (re != cpx.re) || (im != cpx.im);
	}

	template <typename T>
	BALL_INLINE
	bool TComplex<T>::operator == (const T& r) const throw()
	{
		return (re == r) && (im == (T)0);
	}

	template <typename T>
	BALL_INLINE
	bool TComplex<T>::operator != (const T& r) const throw()
	{
		return (re != r) || (im != (T)0);
	}

	template <typename T>
	BALL_INLINE
	bool TComplex<T>::isZero() const throw()
	{
		return (re = (T)0) && (im == (T)0);
	}
	
	template <typename T>
	BALL_INLINE
	bool TComplex<T>::isReal() const throw()
	{
		return (im == (T)0);
	}
	
	template <typename T>
	BALL_INLINE
	TComplex<T> TComplex<T>::conjugate() const throw()
	{
		return TComplex<T>(re, -im);
	}
	
	template <typename T1, typename T2>
	BALL_INLINE 
	TComplex<T2> operator * (const T1& r, const TComplex<T2>& cpx)
		throw()
	{
		return TComplex<T2>(r * cpx.re, r * cpx.im);
	}

	template <typename T>
	std::istream& operator >> (std::istream& s, TComplex<T>& cpx)
		throw()
	{
		return s >> cpx.re >> cpx.im;
	}

	template <typename T>
	std::ostream& operator << (std::ostream& s, const TComplex<T>& cpx)
		throw()
	{
		
		return s << cpx.re << " " << cpx.im;
	}
	
#ifdef BALL_HAS_FFTW
	std::istream& operator >> (std::istream& s, FFTW_COMPLEX& cpx)
		throw();

	std::ostream& operator << (std::ostream& s, const FFTW_COMPLEX& cpx)
		throw();
#endif
	
}// namespace BALL

#endif // BALL_MATHS_COMPLEX_H

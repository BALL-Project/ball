#ifndef BALL_LINALG_VECTOR_H
# include <BALL/MATHS/LINALG/vector.h>
#endif

namespace BALL {

	// ----- Constructors -----
  template <>
  Vector<float>::Vector(const Vector<float>& v) throw(Exception::OutOfMemory)
	:	data_(),
		is_column_vector_(v.is_column_vector_),
		n_(v.n_) {
	#ifdef USE_BLAS
	#ifdef DEBUG
		std::cout << "Info: using blas for copy constructor" << std::endl;
	#endif // DEBUG
		try {
			data_.resize(n_);
		}
		catch (std::bad_alloc&) {
			throw Exception::OutOfMemory(__FILE__, __LINE__, 
			data_.size() * sizeof(float));
		}
		cblas_scopy(data_.size(), &v[0], 1, (&data_[0]), 1);
	#else // no blas support
		try {
			data_ = v.data_;
		}
		catch (std::bad_alloc&) {
			throw Exception::OutOfMemory(__FILE__, __LINE__, 
			data_.size() * sizeof(float));
		}
	#endif
  }

  template <>
  Vector<double>::Vector(const Vector<double>& v) throw(Exception::OutOfMemory)
	:	data_(),
		is_column_vector_(v.is_column_vector_),
		n_(v.n_) {
	#ifdef USE_BLAS
	#ifdef DEBUG
		std::cout << "Info: using blas for copy constructor" << std::endl;
	#endif // DEBUG
		try {
			data_.resize(n_);
		}
		catch (std::bad_alloc&) {
			throw Exception::OutOfMemory(__FILE__, __LINE__, data_.size() * sizeof(double));
		}
		cblas_dcopy(data_.size(), &v[0], 1, (&data_[0]), 1);
	#else // no blas support
		try {
			data_ = v.data_;
		}
		catch (std::bad_alloc&) {
			throw Exception::OutOfMemory(__FILE__, __LINE__, data_.size() * sizeof(double));
		}
	#endif
  }
  
  template <>
  Vector<ComplexFloat>::Vector(const Vector<ComplexFloat>& v)	
    throw(Exception::OutOfMemory)
	:	data_(),
		is_column_vector_(v.is_column_vector_),n_(v.n_) {
	#ifdef USE_BLAS
	#ifdef DEBUG
		std::cout << "Info: using blas for copy constructor" << std::endl;
	#endif // DEBUG
		try {
			data_.resize(n_);
		}
		catch (std::bad_alloc&){
			throw Exception::OutOfMemory(__FILE__, __LINE__, data_.size() * sizeof(ComplexFloat));
		}
		cblas_ccopy(data_.size(), &v[0], 1, (&data_[0]), 1);
	#else // no blas support
		try {
			data_ = v.data_;
		}
		catch (std::bad_alloc&) {
			throw Exception::OutOfMemory(__FILE__, __LINE__, data_.size() * sizeof(ComplexFloat));
		}
	#endif
    }
  
  template <>
  Vector<ComplexDouble>::Vector(const Vector<ComplexDouble>& v)
		throw(Exception::OutOfMemory)
	:	data_(),
		is_column_vector_(v.is_column_vector_),n_(v.n_) {
	#ifdef USE_BLAS
	#ifdef DEBUG
		std::cout << "Info: using blas for copy constructor" << std::endl;
	#endif // DEBUG
		try {
			data_.resize(n_);
		}
		catch (std::bad_alloc&) {
			throw Exception::OutOfMemory(__FILE__, __LINE__, data_.size() * sizeof(ComplexDouble));
		}
		cblas_zcopy(data_.size(), &v[0], 1, (&data_[0]), 1);
	#else // no blas support
		try {
			data_ = v.data_;
		}
		catch (std::bad_alloc&) {
			throw Exception::OutOfMemory(__FILE__, __LINE__, data_.size() * sizeof(ComplexDouble));
		}
	#endif
		}
////////////////////////////////////////////////////////////

  template <>
  Vector<float>& Vector<float>::operator = (const Vector<float>& v)
		throw(Exception::OutOfMemory) {
	is_column_vector_ = v.is_column_vector_;
	n_ = v.n_;

	#ifdef USE_BLAS
	#ifdef DEBUG
		std::cout << "Info: using blas for assignment" << std::endl;
	#endif // DEBUG
		try {
			data_.resize(n_);
		}
		catch (std::bad_alloc&) {
			throw Exception::OutOfMemory(__FILE__, __LINE__, data_.size() * sizeof(float));
		}
		cblas_scopy(data_.size(), &v[0], 1, (&data_[0]), 1);
	#else // no blas support
		try {
			data_ = v.data_;
		}
		catch (std::bad_alloc&) {
			throw Exception::OutOfMemory(__FILE__, __LINE__, data_.size() * sizeof(float));
		}
	#endif

	return *this;
  }

  template <>
  Vector<double>& Vector<double>::operator = (const Vector<double>& v)
		throw(Exception::OutOfMemory) {
	is_column_vector_ = v.is_column_vector_;
	n_ = v.n_;

	#ifdef USE_BLAS
	#ifdef DEBUG
		std::cout << "Info: using blas for assignment" << std::endl;
	#endif // DEBUG
		try {
			data_.resize(n_);
		}
		catch (std::bad_alloc&) {
			throw Exception::OutOfMemory(__FILE__, __LINE__, data_.size() * sizeof(double));
		}
		cblas_dcopy(data_.size(), &v[0], 1, (&data_[0]), 1);
	#else // no blas support
		try  {
			data_ = v.data_;
		}
		catch (std::bad_alloc&) {
			throw Exception::OutOfMemory(__FILE__, __LINE__, data_.size() * sizeof(double));
		}
	#endif

	return *this;
  }   
  
  template <>
  Vector<ComplexFloat>& Vector<ComplexFloat>::operator = (const Vector<ComplexFloat>& v)
    throw(Exception::OutOfMemory) {
        is_column_vector_ = v.is_column_vector_;
	n_ = v.n_;

	#ifdef USE_BLAS
	#ifdef DEBUG
		std::cout << "Info: using blas for assignment" << std::endl;
	#endif // DEBUG
		try {
			data_.resize(n_);
		}
		catch (std::bad_alloc&) {
			throw Exception::OutOfMemory(__FILE__, __LINE__, data_.size() * sizeof(ComplexFloat));
		}
		cblas_ccopy(data_.size(), &v[0], 1, (&data_[0]), 1);
	#else // no blas support
		try {
			data_ = v.data_;
		}
		catch (std::bad_alloc&) {
			throw Exception::OutOfMemory(__FILE__, __LINE__, data_.size() * sizeof(ComplexFloat));
		}
	#endif

	return *this;
    }

  template <>
  Vector<ComplexDouble>& Vector<ComplexDouble>::operator = (const Vector<ComplexDouble>& v)
    throw(Exception::OutOfMemory) 
	{
		is_column_vector_ = v.is_column_vector_;
		n_ = v.n_;

		#ifdef USE_BLAS
		#ifdef DEBUG
			std::cout << "Info: using blas for assignment" << std::endl;
		#endif // DEBUG
			try {
				data_.resize(n_);
			}
			catch (std::bad_alloc&) {
				throw Exception::OutOfMemory(__FILE__, __LINE__, data_.size() * sizeof(ComplexDouble));
			}
			cblas_zcopy(data_.size(), &v[0], 1, (&data_[0]), 1);
		#else // no blas support
			try {
				data_ = v.data_;
			}
			catch (std::bad_alloc&) {
				throw Exception::OutOfMemory(__FILE__, __LINE__, data_.size() * sizeof(ComplexDouble));
			}
		#endif

		return *this;
  }

	// ----- inline addition -----
	template <> 
	void Vector<float>::add_inline_blas(const Vector<float>& B)
	{ 
#ifdef HAS_BLAS
		cblas_saxpy(n_, 1., &B[0], 1, &data_[0], 1);
#else
		std::cerr << "BALL has been configured without BLAS support! Try the generic function instead!" << std::endl;
#endif
	}

	template <>
	void Vector<double>::add_inline_blas(const Vector<double>& B)
	{
#ifdef HAS_BLAS
		cblas_daxpy(n_, 1., &B[0], 1, &data_[0], 1);
#else
		std::cerr << "BALL has been configured without BLAS support! Try the generic function instead!" << std::endl;
#endif
	}

	template <>
	void Vector<ComplexFloat>::add_inline_blas(const Vector<ComplexFloat>& B)
	{
#ifdef HAS_BLAS
		ComplexFloat unity(1.,0);
		cblas_caxpy(n_, &unity, &B[0], 1, &data_[0], 1);
#else
		std::cerr << "BALL has been configured without BLAS support! Try the generic function instead!" << std::endl;
#endif
	}

	template <>
	void Vector<ComplexDouble>::add_inline_blas(const Vector<ComplexDouble>& B)
	{
#ifdef HAS_BLAS
		ComplexDouble unity(1.,0);
		cblas_zaxpy(n_, &unity, &B[0], 1, &data_[0], 1);
#else
		std::cerr << "BALL has been configured without BLAS support! Try the generic function instead!" << std::endl;
#endif
	}

	// ----- addition -----
	template <>
	void Vector<float>::add_blas(Vector<float>& result, const Vector<float>& B) const
	{
#ifdef HAS_BLAS
		// copy the content of B into result
		cblas_scopy(n_, &B[0], 1, &result[0], 1);

		// and perform the addition
		cblas_saxpy(n_, 1., &data_[0], 1, &result[0], 1);

		bool B_is_column_vector = B.isColumnVector();

		if (is_column_vector_ == B_is_column_vector)
		{
			result.setVectorType(is_column_vector_);
		}
#else
		std::cerr << "BALL has been configured without BLAS support! Try the generic function instead!" << std::endl;
#endif
	}
	
	template <>
	void Vector<double>::add_blas(Vector<double>& result, const Vector<double>& B) const
	{
#ifdef HAS_BLAS
		// copy the content of B into result
		cblas_dcopy(n_, &B[0], 1, &result[0], 1);

		// and perform the addition
		cblas_daxpy(n_, 1., &data_[0], 1, &result[0], 1);

		bool B_is_column_vector = B.isColumnVector();

		if (is_column_vector_ == B_is_column_vector)
		{
			result.setVectorType(is_column_vector_);
		}
#else
		std::cerr << "BALL has been configured without BLAS support! Try the generic function instead!" << std::endl;
#endif
	}

	template <>
	void Vector<ComplexFloat>::add_blas(Vector<ComplexFloat>& result, const Vector<ComplexFloat>& B) const
	{
#ifdef HAS_BLAS
		ComplexFloat unity(1.,0);

		// copy the content of B into result
		cblas_ccopy(n_, &B[0], 1, &result[0], 1);

		// and perform the addition
		cblas_caxpy(n_, &unity, &data_[0], 1, &result[0], 1);

		bool B_is_column_vector = B.isColumnVector();

		if (is_column_vector_ == B_is_column_vector)
		{
			result.setVectorType(is_column_vector_);
		}
#else
		std::cerr << "BALL has been configured without BLAS support! Try the generic function instead!" << std::endl;
#endif
	}

	template <>
	void Vector<ComplexDouble>::add_blas(Vector<ComplexDouble>& result, const Vector<ComplexDouble>& B) const
	{
#ifdef HAS_BLAS
		ComplexDouble unity(1.,0);

		// copy the content of B into result
		cblas_zcopy(n_, &B[0], 1, &result[0], 1);

		// and perform the addition
		cblas_zaxpy(n_, &unity, &data_[0], 1, &result[0], 1);

		bool B_is_column_vector = B.isColumnVector();

		if (is_column_vector_ == B_is_column_vector)
		{
			result.setVectorType(is_column_vector_);
		}
#else
		std::cerr << "BALL has been configured without BLAS support! Try the generic function instead!" << std::endl;
#endif
	}

	// ----- inline subtraction -----
	template <>
	void Vector<float>::subtract_inline_blas(const Vector<float>& B)
	{
#ifdef HAS_BLAS
		cblas_saxpy(n_, -1., &B[0], 1, &data_[0], 1);
#else
		std::cerr << "BALL has been configured without BLAS support! Try the generic function instead!" << std::endl;
#endif
	}
	
	template <>
	void Vector<double>::subtract_inline_blas(const Vector<double>& B)
	{
#ifdef HAS_BLAS
		cblas_daxpy(n_, -1., &B[0], 1, &data_[0], 1);
#else
		std::cerr << "BALL has been configured without BLAS support! Try the generic function instead!" << std::endl;
#endif
	}

	template <>
	void Vector<ComplexFloat>::subtract_inline_blas(const Vector<ComplexFloat>& B)
	{
#ifdef HAS_BLAS
		ComplexFloat neg_unity(-1.,0);
		cblas_caxpy(n_, &neg_unity, &B[0], 1, &data_[0], 1);
#else
		std::cerr << "BALL has been configured without BLAS support! Try the generic function instead!" << std::endl;
#endif
	}

	template <>
	void Vector<ComplexDouble>::subtract_inline_blas(const Vector<ComplexDouble>& B)
	{
#ifdef HAS_BLAS
		ComplexDouble neg_unity(-1.,0);
		cblas_zaxpy(n_, &neg_unity, &B[0], 1, &data_[0], 1);
#else
		std::cerr << "BALL has been configured without BLAS support! Try the generic function instead!" << std::endl;
#endif
	}
	
	// ----- subtraction -----
	template <>
	void Vector<float>::subtract_blas(Vector<float>& result, const Vector<float>& B) const
	{
#ifdef HAS_BLAS
		// copy the content of this vector into result
		cblas_scopy(n_, &data_[0], 1, &result[0], 1);

		// and perform the subtraction
		cblas_saxpy(n_, -1., &B[0], 1, &result[0], 1);

		bool B_is_column_vector = B.isColumnVector();

		if (is_column_vector_ == B_is_column_vector)
		{
			result.setVectorType(is_column_vector_);
		}
#else
		std::cerr << "BALL has been configured without BLAS support! Try the generic function instead!" << std::endl;
#endif
	}

	template <>
	void Vector<double>::subtract_blas(Vector<double>& result, const Vector<double>& B) const
	{
#ifdef HAS_BLAS
		// copy the content of this vector into result
		cblas_dcopy(n_, &data_[0], 1, &result[0], 1);

		// and perform the subtraction
		cblas_daxpy(n_, -1., &B[0], 1, &result[0], 1);

		bool B_is_column_vector = B.isColumnVector();

		if (is_column_vector_ == B_is_column_vector)
		{
			result.setVectorType(is_column_vector_);
		}
#else
		std::cerr << "BALL has been configured without BLAS support! Try the generic function instead!" << std::endl;
#endif
	}

	template <>
	void Vector<ComplexFloat>::subtract_blas(Vector<ComplexFloat>& result, const Vector<ComplexFloat>& B) const
	{
#ifdef HAS_BLAS
		ComplexFloat neg_unity(-1.,0);

		// copy the content of this vector into result
		cblas_ccopy(n_, &data_[0], 1, &result[0], 1);

		// and perform the subtraction
		cblas_caxpy(n_, &neg_unity, &B[0], 1, &result[0], 1);

		bool B_is_column_vector = B.isColumnVector();

		if (is_column_vector_ == B_is_column_vector)
		{
			result.setVectorType(is_column_vector_);
		}
#else
		std::cerr << "BALL has been configured without BLAS support! Try the generic function instead!" << std::endl;
#endif
	}

	template <>
	void Vector<ComplexDouble>::subtract_blas(Vector<ComplexDouble>& result, const Vector<ComplexDouble>& B) const
	{
#ifdef HAS_BLAS
		ComplexDouble neg_unity(-1.,0);

		// copy the content of this vector into result
		cblas_zcopy(n_, &data_[0], 1, &result[0], 1);

		// and perform the subtraction
		cblas_zaxpy(n_, &neg_unity, &B[0], 1, &result[0], 1);

		bool B_is_column_vector = B.isColumnVector();

		if (is_column_vector_ == B_is_column_vector)
		{
			result.setVectorType(is_column_vector_);
		}
#else
		std::cerr << "BALL has been configured without BLAS support! Try the generic function instead!" << std::endl;
#endif
	}
	
	// ----- inline multiplication with scalar -----
	template <>
	void Vector<float>::multiply_inline_blas(const float& a)
	{
#ifdef HAS_BLAS
		cblas_sscal(n_, a, &data_[0], 1);
#else
		std::cerr << "BALL has been configured without BLAS support! Try the generic function instead!" << std::endl;
#endif
	} 

	template <>
	void Vector<double>::multiply_inline_blas(const double& a)
	{
#ifdef HAS_BLAS
		cblas_dscal(n_, a, &data_[0], 1);
#else
		std::cerr << "BALL has been configured without BLAS support! Try the generic function instead!" << std::endl;
#endif
	}

	template <>
	void Vector<ComplexFloat>::multiply_inline_blas(const ComplexFloat& a)
	{
#ifdef HAS_BLAS
		cblas_cscal(n_, reinterpret_cast<const void*>(&a), &data_[0], 1);
#else
		std::cerr << "BALL has been configured without BLAS support! Try the generic function instead!" << std::endl;
#endif
	}

	template <>
	void Vector<ComplexDouble>::multiply_inline_blas(const ComplexDouble& a)
	{
#ifdef HAS_BLAS
		cblas_zscal(n_, reinterpret_cast<const void*>(&a), &data_[0], 1);
#else
		std::cerr << "BALL has been configured without BLAS support! Try the generic function instead!" << std::endl;
#endif
	}
	
	// ----- multiplication with scalar -----
	template <>
	void Vector<float>::multiply_blas(Vector<float>& result, const float& a) const
	{
#ifdef HAS_BLAS
		cblas_sscal(n_, a, &result[0], 1);
#else
		std::cerr << "BALL has been configured without BLAS support! Try the generic function instead!" << std::endl;
#endif
	}

	template <>
	void Vector<double>::multiply_blas(Vector<double>& result, const double& a) const
	{
#ifdef HAS_BLAS
		cblas_dscal(n_, a, &result[0], 1);
#else
		std::cerr << "BALL has been configured without BLAS support! Try the generic function instead!" << std::endl;
#endif
	}

	/** TODO: multiplication by float instead of ComplexFloat **/
	template <>
	void Vector<ComplexFloat>::multiply_blas(Vector<ComplexFloat>& result, const ComplexFloat& a) const
	{
#ifdef HAS_BLAS
		cblas_cscal(n_, reinterpret_cast<const void*>(&a), &result[0], 1);
#else
		std::cerr << "BALL has been configured without BLAS support! Try the generic function instead!" << std::endl;
#endif
	}

	template <>
	void Vector<ComplexDouble>::multiply_blas(Vector<ComplexDouble>& result, const ComplexDouble& a) const
	{
#ifdef HAS_BLAS
		cblas_zscal(n_, reinterpret_cast<const void*>(&a), &result[0], 1);
#else
		std::cerr << "BALL has been configured without BLAS support! Try the generic function instead!" << std::endl;
#endif
	} 
	
	// ----- scalar product ------
	template <>
	ComplexFloat Vector<ComplexFloat>::scalarproduct_native(const Vector<ComplexFloat>& a) const
	{
		// NOTE: the scalar product of two complex vectors uses the conjugate of the first!
		ComplexFloat result(0.,0.);
		for (size_t i=1; i<n_; i++)
			result += ComplexFloat( data_[i].real()*a[i].real() + data_[i].imag()*a[i].imag(),
														 -data_[i].imag()*a[i].real() + data_[i].real()*a[i].imag());

		return result;
	}

	template <>
	ComplexDouble Vector<ComplexDouble>::scalarproduct_native(const Vector<ComplexDouble>& a) const
	{
		ComplexDouble result(0.,0.);
		for (size_t i=1; i<n_; i++)
			result += ComplexDouble( data_[i].real()*a[i].real() + data_[i].imag()*a[i].imag(),
															-data_[i].imag()*a[i].real() + data_[i].real()*a[i].imag());

		return result;
	}
	
	template <>
	float Vector<float>::scalarproduct_blas(const Vector<float>& a) const
	{
		return cblas_sdot(n_, &(data_[0]), 1, &(a[0]), 1);
	}

	template <>
	double Vector<double>::scalarproduct_blas(const Vector<double>& a) const
	{
		return cblas_ddot(n_, &(data_[0]), 1, &(a[0]), 1);
	}

	template <>
	ComplexFloat Vector<ComplexFloat>::scalarproduct_blas(const Vector<ComplexFloat>& a) const
	{
		ComplexFloat result;
		cblas_cdotc_sub(n_, &(data_[0]), 1, &(a[0]), 1, reinterpret_cast<void*>(&result));

		return result;
	}

	template <>
	ComplexDouble Vector<ComplexDouble>::scalarproduct_blas(const Vector<ComplexDouble>& a) const
	{
		ComplexDouble result;
		cblas_zdotc_sub(n_, &(data_[0]), 1, &(a[0]), 1, reinterpret_cast<void*>(&result));

		return result;
	}
	
	// ----- matrix multiplication -----
	template <> template <> 
	void Vector<float>::multiply_blas(Vector<float>& result, const Matrix<float, StandardTraits>& A, bool right) const
	{
#ifdef HAS_BLAS
		size_t A_n = A.getRowCount();
		size_t A_m = A.getColumnCount();
		if (A.isRowMajor()) // A is row major
		{	
			if (right)
			{	
				cblas_sgemv(CblasRowMajor, CblasNoTrans, A_n, A_m, 1., const_cast<float*>(&A[0]), A_m, const_cast<float*>(&data_[0]), 1, 0., const_cast<float*>(&result[0]), 1);
			} 
			else
			{	
				cblas_sgemv(CblasRowMajor, CblasTrans, A_n, A_m, 1., const_cast<float*>(&A[0]), A_m, const_cast<float*>(&data_[0]), 1, 0., const_cast<float*>(&result[0]), 1);
			}
		}
		else
		{	
			if (right)
			{
				cblas_sgemv(CblasColMajor, CblasNoTrans, A_n, A_m, 1., const_cast<float*>(&A[0]), A_n, const_cast<float*>(&data_[0]), 1, 0., const_cast<float*>(&result[0]), 1);
			} 
			else
			{
				cblas_sgemv(CblasColMajor, CblasTrans, A_n, A_m, 1., const_cast<float*>(&A[0]), A_n, const_cast<float*>(&data_[0]), 1, 0., const_cast<float*>(&result[0]), 1);
			}	
		}
#else
		std::cerr << "BALL has been configured without BLAS support! Try the generic function instead!" << std::endl;
#endif
	}

	template <> template <>
	void Vector<double>::multiply_blas(Vector<double>& result, const Matrix<double, StandardTraits>& A, bool right) const
	{
#ifdef HAS_BLAS
		size_t A_n = A.getRowCount();
		size_t A_m = A.getColumnCount();
		if (A.isRowMajor()) // A is row major
		{		
			if (right)
			{
				cblas_dgemv(CblasRowMajor, CblasNoTrans, A_n, A_m, 1., const_cast<double*>(&A[0]), A_m, const_cast<double*>(&data_[0]), 1, 0., const_cast<double*>(&result[0]), 1);
			} 
			else
			{
				cblas_dgemv(CblasRowMajor, CblasTrans, A_n, A_m, 1., const_cast<double*>(&A[0]), A_m, const_cast<double*>(&data_[0]), 1, 0., const_cast<double*>(&result[0]), 1);
			}
		}
		else
		{		
			if (right)
			{
				cblas_dgemv(CblasColMajor, CblasNoTrans, A_n, A_m, 1., const_cast<double*>(&A[0]), A_n, const_cast<double*>(&data_[0]), 1, 0., const_cast<double*>(&result[0]), 1);
			} 
			else
			{
				cblas_dgemv(CblasColMajor, CblasTrans, A_n, A_m, 1., const_cast<double*>(&A[0]), A_n, const_cast<double*>(&data_[0]), 1, 0., const_cast<double*>(&result[0]), 1);
			}
		}
#else
		std::cerr << "BALL has been configured without BLAS support! Try the generic function instead!" << std::endl;
#endif
	}

	template <> template <>
	void Vector<ComplexFloat>::multiply_blas(Vector<ComplexFloat>& result, const Matrix<ComplexFloat, StandardTraits>& A, bool right) const
	{
#ifdef HAS_BLAS
		size_t A_n = A.getRowCount();
		size_t A_m = A.getColumnCount();

		ComplexFloat  unity_value(1.,0);
		void* unity = reinterpret_cast<void*>(&unity_value);
		ComplexFloat  zero_value(0,0);
		void* zero = reinterpret_cast<void*>(&zero_value);

		if (A.isRowMajor()) // A is row major
		{			
			if (right)
			{
				cblas_cgemv(CblasRowMajor, CblasNoTrans, A_n, A_m, unity, &A[0], A_m, &data_[0], 1, zero, &result[0], 1);
			} else
			{
				cblas_cgemv(CblasRowMajor, CblasTrans, A_n, A_m, unity, &A[0], A_m, &data_[0], 1, zero, &result[0], 1);
			} 
		}
		else
		{			
			if (right)
			{
				cblas_cgemv(CblasColMajor, CblasNoTrans, A_n, A_m, unity, &A[0], A_n, &data_[0], 1, zero, &result[0], 1);
			} else
			{
				cblas_cgemv(CblasColMajor, CblasTrans, A_n, A_m, unity, &A[0], A_n, &data_[0], 1, zero, &result[0], 1);
			}
		}
#else
		std::cerr << "BALL has been configured without BLAS support! Try the generic function instead!" << std::endl;
#endif
	}

	template <> template <>
	void Vector<ComplexDouble>::multiply_blas(Vector<ComplexDouble>& result, const Matrix<ComplexDouble, StandardTraits>& A, bool right) const
	{
#ifdef HAS_BLAS
		size_t A_n = A.getRowCount();
		size_t A_m = A.getColumnCount();

		ComplexDouble  unity_value(1.,0);
		void* unity = reinterpret_cast<void*>(&unity_value);
		ComplexDouble  zero_value(0,0);
		void* zero = reinterpret_cast<void*>(&zero_value);

		if (A.isRowMajor()) // A is row major
		{			
			if (right)
			{	
				cblas_zgemv(CblasRowMajor, CblasNoTrans, A_n, A_m, unity, &A[0], A_m, &data_[0], 1, zero, &result[0], 1);
			} else
			{	
				cblas_zgemv(CblasRowMajor, CblasTrans, A_n, A_m, unity, &A[0], A_m, &data_[0], 1, zero, &result[0], 1);
			}
		}
		else
		{			
			if (right)
			{
				cblas_zgemv(CblasColMajor, CblasNoTrans, A_n, A_m, unity, &A[0], A_n, &data_[0], 1, zero, &result[0], 1);
			} else
			{
				cblas_zgemv(CblasColMajor, CblasTrans, A_n, A_m, unity, &A[0], A_n, &data_[0], 1, zero, &result[0], 1);
			}
		}
#else
		std::cerr << "BALL has been configured without BLAS support! Try the generic function instead!" << std::endl;
#endif
	} 

	template <> template <> 
	void Vector<float>::multiply_blas(Vector<float>& result, const Matrix<float, SymmetricTraits>& A, bool right) const
	{
#ifdef HAS_BLAS
		size_t A_n = A.getRowCount();

		// since the matrix is symmetric, we do not need to distinguish between right and left multiplication
		if (A.isRowMajor())
		{	
			cblas_sspmv(CblasRowMajor, CblasUpper, A_n, 1., const_cast<float*>(&A[0]), const_cast<float*>(&data_[0]), 1, 0., const_cast<float*>(&result[0]), 1);
		}
		else
		{	
			cblas_sspmv(CblasColMajor, CblasUpper, A_n, 1., const_cast<float*>(&A[0]), const_cast<float*>(&data_[0]), 1, 0., const_cast<float*>(&result[0]), 1);
		}
#else
		std::cerr << "BALL has been configured without BLAS support! Try the generic function instead!" << std::endl;
#endif
	}

	template <> template <> 
	void Vector<double>::multiply_blas(Vector<double>& result, const Matrix<double, SymmetricTraits>& A, bool right) const
	{
#ifdef HAS_BLAS
		size_t A_n = A.getRowCount();

		// since the matrix is symmetric, we do not need to distinguish between right and left multiplication
		if (A.isRowMajor())
		{	
			cblas_dspmv(CblasRowMajor, CblasUpper, A_n, 1., const_cast<double*>(&A[0]), const_cast<double*>(&data_[0]), 1, 0., const_cast<double*>(&result[0]), 1);
		}
		else
		{	
			cblas_dspmv(CblasColMajor, CblasUpper, A_n, 1., const_cast<double*>(&A[0]), const_cast<double*>(&data_[0]), 1, 0., const_cast<double*>(&result[0]), 1);
		}
#else
		std::cerr << "BALL has been configured without BLAS support! Try the generic function instead!" << std::endl;
#endif
	}

	// ----- normalization -----
	template <>
	void Vector<ComplexFloat>::normalize()
	{
		ComplexFloat norm(1.,0);
		norm /=  get2Norm();

#ifdef USE_BLAS
#ifdef DEBUG
		std::cout << "Info: using multiply_inline_blas" << std::endl;
#endif
		multiply_inline_blas(norm);
#else
#ifdef DEBUG
		std::cout << "Info: using multiply_inline_native" << std::endl;
#endif
		multiply_inline_native(norm);
#endif
	}

	template <>
	void Vector<ComplexDouble>::normalize()
	{
		ComplexDouble norm(1.,0);
		norm /=  get2Norm();

#ifdef USE_BLAS
#ifdef DEBUG
		std::cout << "Info: using multiply_inline_blas" << std::endl;
#endif
		multiply_inline_blas(norm);
#else
#ifdef DEBUG
		std::cout << "Info: using multiply_inline_native" << std::endl;
#endif
		multiply_inline_native(norm);
#endif
	}

	// ---- compute norm -----
	template <>
	float Vector<float>::compute_length_blas() const
	{
		return cblas_snrm2(n_, &(data_[0]), 1);
	}

	template <>
	double Vector<double>::compute_length_blas() const
	{
		return cblas_dnrm2(n_, &(data_[0]), 1);
	}

	template <>
	ComplexFloat Vector<ComplexFloat>::compute_length_blas() const
	{
		float result;
		result = cblas_scnrm2(n_, &(data_[0]), 1);
		
		return ComplexFloat(result, 0.);
	}

	template <>
	ComplexDouble Vector<ComplexDouble>::compute_length_blas() const
	{
		double result;
		result = cblas_dznrm2(n_, &(data_[0]), 1);
		
		return ComplexDouble(result, 0.);
	}

////////////////////////////////////////////////////////////

template class Vector<float>;
template class Vector<double>;
template class Vector<ComplexFloat>;
template class Vector<ComplexDouble>;

} // namespace BALL

#ifndef BALL_LINALG_MATRIX_H
# include <BALL/MATHS/LINALG/matrix.h>
#endif

#include <limits>


namespace BALL 
{
	
	// ----- constructors -----
	template <>
	Matrix<float, StandardTraits>::Matrix(const Matrix<float, StandardTraits>& m)
		:	data_(m.n_*m.m_),
			row_major_(m.row_major_),
			n_(m.n_),
			m_(m.m_) 
	{
		#ifdef BALL_HAS_BLAS
		#ifdef DEBUG
			std::cout << "Info: using blas for copy constructor" << std::endl;
		#endif // DEBUG
			cblas_scopy(data_.size(), const_cast<float*>(&m[0]), 1, (&data_[0]), 1);
		#else // no blas support
			float*           this_ptr   = &data_[0];
			float const* m_ptr          = &m[0];

			for (size_t i=0; i<data_.size(); i++)
				*this_ptr++ = *m_ptr++;
		#endif
	}

	template <>
	Matrix<double, StandardTraits>::Matrix(const Matrix<double, StandardTraits>& m)
		:	data_(m.n_*m.m_),
			row_major_(m.row_major_),
			n_(m.n_),
			m_(m.m_) 
	{
		#ifdef BALL_HAS_BLAS
		#ifdef DEBUG
			std::cout << "Info: using blas for copy constructor" << std::endl;
		#endif // DEBUG
			cblas_dcopy(data_.size(), const_cast<double*>(&m[0]), 1, (&data_[0]), 1);
		#else // no blas support
			double*              this_ptr   = &data_[0];
			double const*  m_ptr            = &m[0];

			for (size_t i=0; i<data_.size(); i++)
				*this_ptr++ = *m_ptr++;
		#endif
	}   

	////////////////////////////////////////////////////////////
	
	
	template <>
	Matrix<float, StandardTraits>& Matrix<float, StandardTraits>::operator = (const Matrix<float, StandardTraits>& m) 
	{
		#ifdef BALL_HAS_BLAS
		#ifdef DEBUG
			std::cout << "Info: using blas for assignment constructor" << std::endl;
		#endif // DEBUG
			data_.resize(m.n_*m.m_);
			cblas_scopy(data_.size(), const_cast<float*>(&m[0]), 1, (&data_[0]), 1);
			row_major_ = m.row_major_;
			n_ = m.n_;
			m_ = m.m_;
		#else // no blas support
			data_ = m.data_;
			row_major_ = m.row_major_;
			n_ = m.n_;
			m_ = m.m_;
		#endif
		return *this;
	}

	template <>
	Matrix<double, StandardTraits>& Matrix<double, StandardTraits>::operator = (const Matrix<double, StandardTraits>& m) 
	{
		#ifdef BALL_HAS_BLAS
		#ifdef DEBUG
			std::cout << "Info: using blas for assignment constructor" << std::endl;
		#endif // DEBUG
			data_.resize(m.n_*m.m_);
			cblas_dcopy(data_.size(), const_cast<double*>(&m[0]), 1, (&data_[0]), 1);
			row_major_ = m.row_major_;
			n_ = m.n_;
			m_ = m.m_;
		#else // no blas support        
			data_ = m.data_;
			row_major_ = m.row_major_;
			n_ = m.n_;
			m_ = m.m_;
		#endif
		return *this;
	}

	
	
	
	// ----- addition -----
	template <>
	void Matrix<float, StandardTraits>::add_blas(Matrix<float, StandardTraits>& result, const Matrix<float, StandardTraits>& B) const
	{
#ifdef BALL_HAS_BLAS
		size_t size = data_.size();
		
		// We have to take care of possible shallow transposal
		bool B_row_major = B.isRowMajor();

		if (row_major_ == B_row_major)
		{
			// copy B into result
			cblas_scopy(size, &B[0], 1, &result[0], 1);

			// and just perform the addition
			cblas_saxpy(size, 1., &data_[0], 1, &result[0], 1);

			// and take care of the ordering
			result.setRowMajor(row_major_);
		}
		else 
		{
			if (row_major_) // in this case, matrix B is shallowly transposed
			{
				// it is easier if we copy our data into "result", since then, result
				// is already in default storage order
				cblas_scopy(size, &data_[0], 1, &result[0], 1);

				// we need to perform a number of additions with stride == m_
				for (size_t i=0; i<n_; i++)
				{
					cblas_saxpy(m_, 1., &B[i], n_, &result[i*m_], 1);
				}	
			}
			else // only we are shallowly transposed
			{
				// it is easier if we copy B's data into "result", since then, result
				// is already in default storage order
				cblas_scopy(size, &B[0], 1, &result[0], 1);

				// we need to perform a number of additions with stride == m_
				for (size_t i=0; i<n_; i++)
				{
					cblas_saxpy(m_, 1., &data_[i], n_, &result[i*m_], 1);
				}	
			}
		}
#else
		std::cerr << "BALL has been configured without BLAS support! Try the generic function instead!" << std::endl;
#endif
	}

	template <>
	void Matrix<double, StandardTraits>::add_blas(Matrix<double, StandardTraits>& result, const Matrix<double, StandardTraits>& B) const
	{
#ifdef BALL_HAS_BLAS
		size_t size = data_.size();
		
		// We have to take care of possible shallow transposal
		bool B_row_major = B.isRowMajor();

		if (row_major_ == B_row_major)
		{
			// copy B into result
			cblas_dcopy(size, &B[0], 1, &result[0], 1);

			// and just perform the addition
			cblas_daxpy(size, 1., &data_[0], 1, &result[0], 1);

			// and take care of the ordering
			result.setRowMajor(row_major_);
		}
		else 
		{
			if (row_major_) // in this case, matrix B is shallowly transposed
			{
				// it is easier if we copy our data into "result", since then, result
				// is already in default storage order
				cblas_dcopy(size, &data_[0], 1, &result[0], 1);

				// we need to perform a number of additions with stride == m_
				for (size_t i=0; i<n_; i++)
				{
					cblas_daxpy(m_, 1., &B[i], n_, &result[i*m_], 1);
				}	
			}
			else // only we are shallowly transposed
			{
				// it is easier if we copy B's data into "result", since then, result
				// is already in default storage order
				cblas_dcopy(size, &B[0], 1, &result[0], 1);

				// we need to perform a number of additions with stride == m_
				for (size_t i=0; i<n_; i++)
				{
					cblas_daxpy(m_, 1., &data_[i], n_, &result[i*m_], 1);
				}	
			}
		}
#else
		std::cerr << "BALL has been configured without BLAS support! Try the generic function instead!" << std::endl;
#endif
	}

	template <> 
	void Matrix<ComplexFloat, StandardTraits>::add_blas(Matrix<ComplexFloat, StandardTraits>& result, const Matrix<ComplexFloat, StandardTraits>& B) const
	{
#ifdef BALL_HAS_BLAS
		size_t size = data_.size();
		
		// We have to take care of possible shallow transposal
		bool B_row_major = B.isRowMajor();

		// We'll need that one for the axpy
		ComplexFloat  unity_value(1.,0);
		void* unity = reinterpret_cast<void*>(&unity_value);

		const ComplexFloat* b_ptr  	 = &B[0];
		const ComplexFloat* this_ptr = &data_[0];

		if (row_major_ == B_row_major)
		{
			// copy B into result
			cblas_ccopy(size, reinterpret_cast<const void*>(b_ptr), 1, reinterpret_cast<void*>(&result[0]), 1);

			// and just perform the addition
			cblas_caxpy(size, unity, reinterpret_cast<const void*>(this_ptr), 1, reinterpret_cast<void*>(&result[0]), 1);

			// and take care of the ordering
			result.setRowMajor(row_major_);
		}
		else 
		{
			if (row_major_) // in this case, matrix B is shallowly transposed
			{
				// it is easier if we copy our data into "result", since then, result
				// is already in default storage order
				cblas_ccopy(size, reinterpret_cast<const void*>(this_ptr), 1, reinterpret_cast<void*>(&result[0]), 1);

				// we need to perform a number of additions with stride == m_
				for (size_t i=0; i<n_; i++)
				{
					cblas_caxpy(m_, unity, reinterpret_cast<const void*>(b_ptr+i), n_, reinterpret_cast<void*>(&result[i*m_]), 1);
				}	
			}
			else // only we are shallowly transposed
			{
				// it is easier if we copy B's data into "result", since then, result
				// is already in default storage order
				cblas_ccopy(size, reinterpret_cast<const void*>(b_ptr), 1, reinterpret_cast<void*>(&result[0]), 1);

				// we need to perform a number of additions with stride == m_
				for (size_t i=0; i<n_; i++)
				{
					cblas_caxpy(m_, unity, reinterpret_cast<const void*>(this_ptr+i), n_, reinterpret_cast<void*>(&result[i*m_]), 1);
				}	
			}
		}
#else
		std::cerr << "BALL has been configured without BLAS support! Try the generic function instead!" << std::endl;
#endif
	}

	template <>
	void Matrix<ComplexDouble, StandardTraits>::add_blas(Matrix<ComplexDouble, StandardTraits>& result, const Matrix<ComplexDouble, StandardTraits>& B) const
	{
#ifdef BALL_HAS_BLAS
		size_t size = data_.size();
		
		// We have to take care of possible shallow transposal
		bool B_row_major = B.isRowMajor();

		// We'll need that one for the axpy
		ComplexDouble  unity_value(1.,0);
		void* unity = reinterpret_cast<void*>(&unity_value);

		const ComplexDouble* b_ptr  	 = &B[0];
		const ComplexDouble* this_ptr = &data_[0];

		if (row_major_ == B_row_major)
		{
			// copy B into result
			cblas_zcopy(size, reinterpret_cast<const void*>(b_ptr), 1, reinterpret_cast<void*>(&result[0]), 1);

			// and just perform the addition
			cblas_zaxpy(size, unity, reinterpret_cast<const void*>(this_ptr), 1, reinterpret_cast<void*>(&result[0]), 1);

			// and take care of the ordering
			result.setRowMajor(row_major_);
		}
		else 
		{
			if (row_major_) // in this case, matrix B is shallowly transposed
			{
				// it is easier if we copy our data into "result", since then, result
				// is already in default storage order
				cblas_zcopy(size, reinterpret_cast<const void*>(this_ptr), 1, reinterpret_cast<void*>(&result[0]), 1);

				// we need to perform a number of additions with stride == m_
				for (size_t i=0; i<n_; i++)
				{
					cblas_zaxpy(m_, unity, reinterpret_cast<const void*>(b_ptr+i), n_, reinterpret_cast<void*>(&result[i*m_]), 1);
				}	
			}
			else // only we are shallowly transposed
			{
				// it is easier if we copy B's data into "result", since then, result
				// is already in default storage order
				cblas_zcopy(size, reinterpret_cast<const void*>(b_ptr), 1, reinterpret_cast<void*>(&result[0]), 1);

				// we need to perform a number of additions with stride == m_
				for (size_t i=0; i<n_; i++)
				{
					cblas_zaxpy(m_, unity, reinterpret_cast<const void*>(this_ptr+i), n_, reinterpret_cast<void*>(&result[i*m_]), 1);
				}	
			}
		}
#else
		std::cerr << "BALL has been configured without BLAS support! Try the generic function instead!" << std::endl;
#endif
	}

	// ----- inline addition -----
	template <>
	void Matrix<float, StandardTraits>::add_inline_blas(const Matrix<float, StandardTraits>& B)
	{
#ifdef BALL_HAS_BLAS
		size_t size = data_.size();
		
		// We have to take care of possible shallow transposal
		bool B_row_major = B.isRowMajor();

		if (row_major_ == B_row_major)
		{
			// and just perform the addition
			cblas_saxpy(size, 1., &B[0], 1, &data_[0], 1);
		}
		else 
		{
			if (row_major_) // in this case, matrix B is shallowly transposed
			{
				// we need to perform a number of additions with stride == m_
				for (size_t i=0; i<n_; i++)
				{
					cblas_saxpy(m_, 1., &B[i], n_, &data_[i*m_], 1);
				}	
			}
			else // only we are shallowly transposed
			{
				// we need to perform a number of additions with stride == n_
				for (size_t i=0; i<m_; i++)
				{
					cblas_saxpy(n_, 1., &B[i], m_, &data_[i*n_], 1);
				}	
			}
		}
#else
		std::cerr << "BALL has been configured without BLAS support! Try the generic function instead!" << std::endl;
#endif
	}

	template <>
	void Matrix<double, StandardTraits>::add_inline_blas(const Matrix<double, StandardTraits>& B) 
	{
#ifdef BALL_HAS_BLAS
		size_t size = data_.size();
		
		// We have to take care of possible shallow transposal
		bool B_row_major = B.isRowMajor();

		if (row_major_ == B_row_major)
		{
			// and just perform the addition
			cblas_daxpy(size, 1., &B[0], 1, &data_[0], 1);
		}
		else 
		{
			if (row_major_) // in this case, matrix B is shallowly transposed
			{
				// we need to perform a number of additions with stride == m_
				for (size_t i=0; i<n_; i++)
				{
					cblas_daxpy(m_, 1., &B[i], n_, &data_[i*m_], 1);
				}	
			}
			else // only we are shallowly transposed
			{
				// we need to perform a number of additions with stride == n_
				for (size_t i=0; i<m_; i++)
				{
					cblas_daxpy(n_, 1., &B[i], m_, &data_[i*n_], 1);
				}	
			}
		}
#else
		std::cerr << "BALL has been configured without BLAS support! Try the generic function instead!" << std::endl;
#endif
	}

	template <>
	void Matrix<ComplexFloat, StandardTraits>::add_inline_blas(const Matrix<ComplexFloat, StandardTraits>& B)
	{
#ifdef BALL_HAS_BLAS
		size_t size = data_.size();

		// We'll need that one for the axpy
		ComplexFloat  unity_value(1.,0);
		void* unity = reinterpret_cast<void*>(&unity_value);
		
		// We have to take care of possible shallow transposal
		bool B_row_major = B.isRowMajor();

		if (row_major_ == B_row_major)
		{
			// and just perform the addition
			cblas_caxpy(size, unity, &B[0], 1, &data_[0], 1);
		}
		else 
		{
			if (row_major_) // in this case, matrix B is shallowly transposed
			{
				// we need to perform a number of additions with stride == m_
				for (size_t i=0; i<n_; i++)
				{
					cblas_caxpy(m_, unity, &B[i], n_, &data_[i*m_], 1);
				}	
			}
			else // only we are shallowly transposed
			{
				// we need to perform a number of additions with stride == n_
				for (size_t i=0; i<m_; i++)
				{
					cblas_caxpy(n_, unity, &B[i], m_, &data_[i*n_], 1);
				}	
			}
		}
#else
		std::cerr << "BALL has been configured without BLAS support! Try the generic function instead!" << std::endl;
#endif
	}

	template <>
	void Matrix<ComplexDouble, StandardTraits>::add_inline_blas(const Matrix<ComplexDouble, StandardTraits>& B)
	{
#ifdef BALL_HAS_BLAS
		size_t size = data_.size();

		// We'll need that one for the axpy
		ComplexDouble  unity_value(1.,0);
		void* unity = reinterpret_cast<void*>(&unity_value);
		
		// We have to take care of possible shallow transposal
		bool B_row_major = B.isRowMajor();

		if (row_major_ == B_row_major)
		{
			// and just perform the addition
			cblas_zaxpy(size, unity, &B[0], 1, &data_[0], 1);
		}
		else 
		{
			if (row_major_) // in this case, matrix B is shallowly transposed
			{
				// we need to perform a number of additions with stride == m_
				for (size_t i=0; i<n_; i++)
				{
					cblas_zaxpy(m_, unity, &B[i], n_, &data_[i*m_], 1);
				}	
			}
			else // only we are shallowly transposed
			{
				// we need to perform a number of additions with stride == n_
				for (size_t i=0; i<m_; i++)
				{
					cblas_zaxpy(n_, unity, &B[i], m_, &data_[i*n_], 1);
				}	
			}
		}
#else
		std::cerr << "BALL has been configured without BLAS support! Try the generic function instead!" << std::endl;
#endif
	}

	// ----- subtraction -----
	template <>
	void Matrix<float, StandardTraits>::subtract_blas(Matrix<float, StandardTraits>& result, const Matrix<float, StandardTraits>& B) const
	{
#ifdef BALL_HAS_BLAS
		size_t size = data_.size();
		
		// We have to take care of possible shallow transposal
		bool B_row_major = B.isRowMajor();

		if (row_major_ == B_row_major)
		{
			// copy this matrix into result
			cblas_scopy(size, &data_[0], 1, &result[0], 1);

			// and just perform the addition
			cblas_saxpy(size, -1., &B[0], 1, &result[0], 1);

			// and take care of the ordering
			result.setRowMajor(row_major_);
		}
		else 
		{
			if (row_major_) // in this case, matrix B is shallowly transposed
			{
				// it is easier if we copy our data into "result", since then, result
				// is already in default storage order
				cblas_scopy(size, &data_[0], 1, &result[0], 1);

				// we need to perform a number of additions with stride == m_
				for (size_t i=0; i<n_; i++)
				{
					cblas_saxpy(m_, -1., &B[i], n_, &result[i*m_], 1);
				}	
			}
			else // only we are shallowly transposed
			{
				// it is easier if we copy B's data into "result", since then, result
				// is already in default storage order
				//
				// Note: due to the restrictions of _axpy, we cannot easily subtract the
				// second argument from the first. Thus we'll have to compute -(B-A) instead.
				cblas_scopy(size, &B[0], 1, &result[0], 1);

				// we need to perform a number of additions with stride == m_
				for (size_t i=0; i<n_; i++)
				{
					cblas_saxpy(m_, -1., &data_[i], n_, &result[i*m_], 1);
				}	

				// unfortunately, we need to negate now
				cblas_sscal(size, -1., &result[0], 1);	
			}
		}
#else
		std::cerr << "BALL has been configured without BLAS support! Try the generic function instead!" << std::endl;
#endif
	}

	template <>
	void Matrix<double, StandardTraits>::subtract_blas(Matrix<double, StandardTraits>& result, const Matrix<double, StandardTraits>& B) const
	{
#ifdef BALL_HAS_BLAS
		size_t size = data_.size();
		
		// We have to take care of possible shallow transposal
		bool B_row_major = B.isRowMajor();

		if (row_major_ == B_row_major)
		{
			// copy this matrix into result
			cblas_dcopy(size, &data_[0], 1, &result[0], 1);

			// and just perform the addition
			cblas_daxpy(size, -1., &B[0], 1, &result[0], 1);

			// and take care of the ordering
			result.setRowMajor(row_major_);
		}
		else 
		{
			if (row_major_) // in this case, matrix B is shallowly transposed
			{
				// it is easier if we copy our data into "result", since then, result
				// is already in default storage order
				cblas_dcopy(size, &data_[0], 1, &result[0], 1);

				// we need to perform a number of additions with stride == m_
				for (size_t i=0; i<n_; i++)
				{
					cblas_daxpy(m_, -1., &B[i], n_, &result[i*m_], 1);
				}	
			}
			else // only we are shallowly transposed
			{
				// it is easier if we copy B's data into "result", since then, result
				// is already in default storage order
				//
				// Note: due to the restrictions of _axpy, we cannot easily subtract the
				// second argument from the first. Thus we'll have to compute -(B-A) instead.
				cblas_dcopy(size, &B[0], 1, &result[0], 1);

				// we need to perform a number of additions with stride == m_
				for (size_t i=0; i<n_; i++)
				{
					cblas_daxpy(m_, -1., &data_[i], n_, &result[i*m_], 1);
				}	

				// unfortunately, we need to negate now
				cblas_dscal(size, -1., &result[0], 1);	
			}
		}
#else
		std::cerr << "BALL has been configured without BLAS support! Try the generic function instead!" << std::endl;
#endif
	}

	template <>
	void Matrix<ComplexFloat, StandardTraits>::subtract_blas(Matrix<ComplexFloat, StandardTraits>& result, const Matrix<ComplexFloat, StandardTraits>& B) const
	{
#ifdef BALL_HAS_BLAS
		size_t size = data_.size();
		
		// We have to take care of possible shallow transposal
		bool B_row_major = B.isRowMajor();

		// We'll need that one for the axpy
		ComplexFloat  unity_value(-1.,0);
		void* unity = reinterpret_cast<void*>(&unity_value);

		const ComplexFloat* b_ptr  	 = &B[0];
		const ComplexFloat* this_ptr = &data_[0];

		if (row_major_ == B_row_major)
		{
			// copy B into result
			cblas_ccopy(size, reinterpret_cast<const void*>(this_ptr), 1, reinterpret_cast<void*>(&result[0]), 1);

			// and just perform the addition
			cblas_caxpy(size, unity, reinterpret_cast<const void*>(b_ptr), 1, reinterpret_cast<void*>(&result[0]), 1);

			// and take care of the ordering
			result.setRowMajor(row_major_);
		}
		else 
		{
			if (row_major_) // in this case, matrix B is shallowly transposed
			{
				// it is easier if we copy our data into "result", since then, result
				// is already in default storage order
				cblas_ccopy(size, reinterpret_cast<const void*>(this_ptr), 1, reinterpret_cast<void*>(&result[0]), 1);

				// we need to perform a number of additions with stride == m_
				for (size_t i=0; i<n_; i++)
				{
					cblas_caxpy(m_, unity, reinterpret_cast<const void*>(b_ptr+i), n_, reinterpret_cast<void*>(&result[i*m_]), 1);
				}	
			}
			else // only we are shallowly transposed
			{
				// it is easier if we copy B's data into "result", since then, result
				// is already in default storage order
				cblas_ccopy(size, reinterpret_cast<const void*>(b_ptr), 1, reinterpret_cast<void*>(&result[0]), 1);

				// we need to perform a number of additions with stride == m_
				for (size_t i=0; i<n_; i++)
				{
					cblas_caxpy(m_, unity, reinterpret_cast<const void*>(this_ptr+i), n_, reinterpret_cast<void*>(&result[i*m_]), 1);
				}

				// unfortunately, we need to negate now
				cblas_cscal(size, unity, &result[0], 1);		
			}
		}
#else
		std::cerr << "BALL has been configured without BLAS support! Try the generic function instead!" << std::endl;
#endif
	}

	template <>
	void Matrix<ComplexDouble, StandardTraits>::subtract_blas(Matrix<ComplexDouble, StandardTraits>& result, const Matrix<ComplexDouble, StandardTraits>& B) const
	{
#ifdef BALL_HAS_BLAS
		size_t size = data_.size();
		
		// We have to take care of possible shallow transposal
		bool B_row_major = B.isRowMajor();

		// We'll need that one for the axpy
		ComplexDouble  unity_value(-1.,0);
		void* unity = reinterpret_cast<void*>(&unity_value);

		const ComplexDouble* b_ptr  	 = &B[0];
		const ComplexDouble* this_ptr = &data_[0];

		if (row_major_ == B_row_major)
		{
			// copy B into result
			cblas_zcopy(size, reinterpret_cast<const void*>(this_ptr), 1, reinterpret_cast<void*>(&result[0]), 1);

			// and just perform the addition
			cblas_zaxpy(size, unity, reinterpret_cast<const void*>(b_ptr), 1, reinterpret_cast<void*>(&result[0]), 1);

			// and take care of the ordering
			result.setRowMajor(row_major_);
		}
		else 
		{
			if (row_major_) // in this case, matrix B is shallowly transposed
			{
				// it is easier if we copy our data into "result", since then, result
				// is already in default storage order
				cblas_zcopy(size, reinterpret_cast<const void*>(this_ptr), 1, reinterpret_cast<void*>(&result[0]), 1);

				// we need to perform a number of additions with stride == m_
				for (size_t i=0; i<n_; i++)
				{
					cblas_zaxpy(m_, unity, reinterpret_cast<const void*>(b_ptr+i), n_, reinterpret_cast<void*>(&result[i*m_]), 1);
				}	
			}
			else // only we are shallowly transposed
			{
				// it is easier if we copy B's data into "result", since then, result
				// is already in default storage order
				cblas_zcopy(size, reinterpret_cast<const void*>(b_ptr), 1, reinterpret_cast<void*>(&result[0]), 1);

				// we need to perform a number of additions with stride == m_
				for (size_t i=0; i<n_; i++)
				{
					cblas_zaxpy(m_, unity, reinterpret_cast<const void*>(this_ptr+i), n_, reinterpret_cast<void*>(&result[i*m_]), 1);
				}

				// unfortunately, we need to negate now
				cblas_zscal(size, unity, &result[0], 1);		
			}
		}
#else
		std::cerr << "BALL has been configured without BLAS support! Try the generic function instead!" << std::endl;
#endif
	}

	// ----- inline subtraction -----
	template <>
	void Matrix<float, StandardTraits>::subtract_inline_blas(const Matrix<float, StandardTraits>& B)
	{
#ifdef BALL_HAS_BLAS
		size_t size = data_.size();
		
		// We have to take care of possible shallow transposal
		bool B_row_major = B.isRowMajor();

		if (row_major_ == B_row_major)
		{
			// and just perform the addition
			cblas_saxpy(size, -1., &B[0], 1, &data_[0], 1);
		}
		else 
		{
			if (row_major_) // in this case, matrix B is shallowly transposed
			{
				// we need to perform a number of additions with stride == m_
				for (size_t i=0; i<n_; i++)
				{
					cblas_saxpy(m_, -1., &B[i], n_, &data_[i*m_], 1);
				}	
			}
			else // only we are shallowly transposed
			{
				// we need to perform a number of additions with stride == n_
				for (size_t i=0; i<m_; i++)
				{
					cblas_saxpy(n_, -1., &B[i], m_, &data_[i*n_], 1);
				}	
			}
		}
#else
		std::cerr << "BALL has been configured without BLAS support! Try the generic function instead!" << std::endl;
#endif
	}
	
	template <>
	void Matrix<double, StandardTraits>::subtract_inline_blas(const Matrix<double, StandardTraits>& B) 
	{
#ifdef BALL_HAS_BLAS
		size_t size = data_.size();
		
		// We have to take care of possible shallow transposal
		bool B_row_major = B.isRowMajor();

		if (row_major_ == B_row_major)
		{
			// and just perform the addition
			cblas_daxpy(size, -1., &B[0], 1, &data_[0], 1);
		}
		else 
		{
			if (row_major_) // in this case, matrix B is shallowly transposed
			{
				// we need to perform a number of additions with stride == m_
				for (size_t i=0; i<n_; i++)
				{
					cblas_daxpy(m_, -1., &B[i], n_, &data_[i*m_], 1);
				}	
			}
			else // only we are shallowly transposed
			{
				// we need to perform a number of additions with stride == n_
				for (size_t i=0; i<m_; i++)
				{
					cblas_daxpy(n_, -1., &B[i], m_, &data_[i*n_], 1);
				}	
			}
		}
#else
		std::cerr << "BALL has been configured without BLAS support! Try the generic function instead!" << std::endl;
#endif
	}

	template <>
	void Matrix<ComplexFloat, StandardTraits>::subtract_inline_blas(const Matrix<ComplexFloat, StandardTraits>& B)
	{
#ifdef BALL_HAS_BLAS
		size_t size = data_.size();

		// We'll need that one for the axpy
		ComplexFloat  unity_value(-1.,0);
		void* unity = reinterpret_cast<void*>(&unity_value);
		
		// We have to take care of possible shallow transposal
		bool B_row_major = B.isRowMajor();

		if (row_major_ == B_row_major)
		{
			// and just perform the addition
			cblas_caxpy(size, unity, &B[0], 1, &data_[0], 1);
		}
		else 
		{
			if (row_major_) // in this case, matrix B is shallowly transposed
			{
				// we need to perform a number of additions with stride == m_
				for (size_t i=0; i<n_; i++)
				{
					cblas_caxpy(m_, unity, &B[i], n_, &data_[i*m_], 1);
				}	
			}
			else // only we are shallowly transposed
			{
				// we need to perform a number of additions with stride == n_
				for (size_t i=0; i<m_; i++)
				{
					cblas_caxpy(n_, unity, &B[i], m_, &data_[i*n_], 1);
				}	
			}
		}
#else
		std::cerr << "BALL has been configured without BLAS support! Try the generic function instead!" << std::endl;
#endif
	}

	template <>
	void Matrix<ComplexDouble, StandardTraits>::subtract_inline_blas(const Matrix<ComplexDouble, StandardTraits>& B)
	{
#ifdef BALL_HAS_BLAS
		size_t size = data_.size();

		// We'll need that one for the axpy
		ComplexDouble  unity_value(-1.,0);
		void* unity = reinterpret_cast<void*>(&unity_value);
		
		// We have to take care of possible shallow transposal
		bool B_row_major = B.isRowMajor();

		if (row_major_ == B_row_major)
		{
			// and just perform the addition
			cblas_zaxpy(size, unity, &B[0], 1, &data_[0], 1);
		}
		else 
		{
			if (row_major_) // in this case, matrix B is shallowly transposed
			{
				// we need to perform a number of additions with stride == m_
				for (size_t i=0; i<n_; i++)
				{
					cblas_zaxpy(m_, unity, &B[i], n_, &data_[i*m_], 1);
				}	
			}
			else // only we are shallowly transposed
			{
				// we need to perform a number of additions with stride == n_
				for (size_t i=0; i<m_; i++)
				{
					cblas_zaxpy(n_, unity, &B[i], m_, &data_[i*n_], 1);
				}	
			}
		}
#else
		std::cerr << "BALL has been configured without BLAS support! Try the generic function instead!" << std::endl;
#endif
	}

	// ----- scalar multiplication -----
	template <>
	void Matrix<float, StandardTraits>::multiply_blas(Matrix<float, StandardTraits>& result, const float& a) const
	{
#ifdef BALL_HAS_BLAS
		cblas_sscal(result.getSize(), a, &result[0], 1);
#else
		std::cerr << "BALL has been configured without BLAS support! Try the generic function instead!" << std::endl;
#endif
	}

	template <>
	void Matrix<double, StandardTraits>::multiply_blas(Matrix<double, StandardTraits>& result, const double& a) const
	{
#ifdef BALL_HAS_BLAS
		cblas_dscal(result.getSize(), a, &result[0], 1);
#else
		std::cerr << "BALL has been configured without BLAS support! Try the generic function instead!" << std::endl;
#endif
	}

	/** TODO: multiplication by float instead of ComplexFloat **/
	template <>
	void Matrix<ComplexFloat, StandardTraits>::multiply_blas(Matrix<ComplexFloat, StandardTraits>& result, const ComplexFloat& a) const
	{
#ifdef BALL_HAS_BLAS
		cblas_cscal(result.getSize(), reinterpret_cast<const void*>(&a), &result[0], 1);
#else
		std::cerr << "BALL has been configured without BLAS support! Try the generic function instead!" << std::endl;
#endif
	}

	template <>
	void Matrix<ComplexDouble, StandardTraits>::multiply_blas(Matrix<ComplexDouble, StandardTraits>& result, const ComplexDouble& a) const
	{
#ifdef BALL_HAS_BLAS
		cblas_zscal(result.getSize(), reinterpret_cast<const void*>(&a), &result[0], 1);
#else
		std::cerr << "BALL has been configured without BLAS support! Try the generic function instead!" << std::endl;
#endif
	}

	// ----- scalar inline multiplication -----
	template <>
	void Matrix<float, StandardTraits>::multiply_inline_blas(const float& a)
	{
#ifdef BALL_HAS_BLAS
		cblas_sscal(data_.size(), a, &data_[0], 1);
#else
		std::cerr << "BALL has been configured without BLAS support! Try the generic function instead!" << std::endl;
#endif
	}
	
	template <>
	void Matrix<double, StandardTraits>::multiply_inline_blas(const double& a)
	{
#ifdef BALL_HAS_BLAS
		cblas_dscal(data_.size(), a, &data_[0], 1);
#else
		std::cerr << "BALL has been configured without BLAS support! Try the generic function instead!" << std::endl;
#endif
	}

	template <>
	void Matrix<ComplexFloat, StandardTraits>::multiply_inline_blas(const ComplexFloat& a)
	{
#ifdef BALL_HAS_BLAS
		cblas_cscal(data_.size(), reinterpret_cast<const void*>(&a), &data_[0], 1);
#else
		std::cerr << "BALL has been configured without BLAS support! Try the generic function instead!" << std::endl;
#endif
	}

	template <>
	void Matrix<ComplexDouble, StandardTraits>::multiply_inline_blas(const ComplexDouble& a)
	{
#ifdef BALL_HAS_BLAS
		cblas_zscal(data_.size(), reinterpret_cast<const void*>(&a), &data_[0], 1);
#else
		std::cerr << "BALL has been configured without BLAS support! Try the generic function instead!" << std::endl;
#endif
	}

	// ----- matrix multiplication -----
	template <>
	void Matrix<float, StandardTraits>::multiply_blas(Matrix<float, StandardTraits>& result, const Matrix<float, StandardTraits>& B) const
	{
#ifdef BALL_HAS_BLAS
		// we'll need that one more than once...
		size_t B_n = B.getRowCount();
		size_t B_m = B.getColumnCount();

		// if A and B have the same storage order, we'll use it for result as well
		bool B_row_major = B.isRowMajor();
		
		if (row_major_ == B_row_major)
		{
			// decide the storage format for the result
			if (row_major_)
			{
				cblas_sgemm(CblasRowMajor, 
										CblasNoTrans, CblasNoTrans, 
										n_, B_m, m_, 1.,
										const_cast<float*>(&data_[0]), m_,
										const_cast<float*>(&B[0]), B_m,
										0., const_cast<float*>(&result[0]), B_m);	
			}
			else
			{
				cblas_sgemm(CblasColMajor, 
										CblasNoTrans, CblasNoTrans, 
										n_, B_m, m_, 1.,
										const_cast<float*>(&data_[0]), n_,
										const_cast<float*>(&B[0]), B_n,
										0., const_cast<float*>(&result[0]), n_);	

				result.setRowMajor(false);
			}
		}
		else // the storage formats differ
		{
			if (row_major_) // B is column major
			{
				cblas_sgemm(CblasRowMajor, 
										CblasNoTrans, CblasTrans, 
										n_, B_m, m_, 1.,
										const_cast<float*>(&data_[0]), m_,
										const_cast<float*>(&B[0]), B_n,
										0., const_cast<float*>(&result[0]), B_m);	
			}
			else // this matrix is column major, B is row major
			{
				cblas_sgemm(CblasRowMajor, 
										CblasTrans, CblasNoTrans, 
										n_, B_m, m_, 1.,
										const_cast<float*>(&data_[0]), n_,
										const_cast<float*>(&B[0]), B_m,
										0., const_cast<float*>(&result[0]), B_m);	
			}
		}

#else
		std::cerr << "BALL has been configured without BLAS support! Try the generic function instead!" << std::endl;
#endif
	}

	template <>
	void Matrix<double, StandardTraits>::multiply_blas(Matrix<double, StandardTraits>& result, const Matrix<double, StandardTraits>& B) const
	{
#ifdef BALL_HAS_BLAS
		// we'll need that one more than once...
		size_t B_n = B.getRowCount();
		size_t B_m = B.getColumnCount();

		// if A and B have the same storage order, we'll use it for result as well
		bool B_row_major = B.isRowMajor();
		
		if (row_major_ == B_row_major)
		{
			// decide the storage format for the result
			if (row_major_)
			{
				cblas_dgemm(CblasRowMajor, 
										CblasNoTrans, CblasNoTrans, 
										n_, B_m, m_, 1.,
										const_cast<double*>(&data_[0]), m_,
										const_cast<double*>(&B[0]), B_m,
										0., const_cast<double*>(&result[0]), B_m);	
			}
			else
			{
				cblas_dgemm(CblasColMajor, 
										CblasNoTrans, CblasNoTrans, 
										n_, B_m, m_, 1.,
										const_cast<double*>(&data_[0]), n_,
										const_cast<double*>(&B[0]), B_n,
										0., const_cast<double*>(&result[0]), n_);	

				result.setRowMajor(false);
			}
		}
		else // the storage formats differ
		{
			if (row_major_) // B is column major
			{
				cblas_dgemm(CblasRowMajor, 
										CblasNoTrans, CblasTrans, 
										n_, B_m, m_, 1.,
										const_cast<double*>(&data_[0]), m_,
										const_cast<double*>(&B[0]), B_n,
										0., const_cast<double*>(&result[0]), B_m);	
			}
			else // this matrix is column major, B is row major
			{
				cblas_dgemm(CblasRowMajor, 
										CblasTrans, CblasNoTrans, 
										n_, B_m, m_, 1.,
										const_cast<double*>(&data_[0]), n_,
										const_cast<double*>(&B[0]), B_m,
										0., const_cast<double*>(&result[0]), B_m);	
			}
		}

#else
		std::cerr << "BALL has been configured without BLAS support! Try the generic function instead!" << std::endl;
#endif
	}

	template <>
	void Matrix<ComplexFloat, StandardTraits>::multiply_blas(Matrix<ComplexFloat, StandardTraits>& result, const Matrix<ComplexFloat, StandardTraits>& B) const
	{
#ifdef BALL_HAS_BLAS
		// we'll need that one more than once...
		size_t B_n = B.getRowCount();
		size_t B_m = B.getColumnCount();

		// We'll need that one for the axpy
		ComplexFloat  unity_value(1.,0);
		void* unity = reinterpret_cast<void*>(&unity_value);
		ComplexFloat  zero_value(0,0);
		void* zero = reinterpret_cast<void*>(&unity_value);

		// if A and B have the same storage order, we'll use it for result as well
		bool B_row_major = B.isRowMajor();
		
		if (row_major_ == B_row_major)
		{
			// decide the storage format for the result
			if (row_major_)
			{
				cblas_cgemm(CblasRowMajor, 
										CblasNoTrans, CblasNoTrans, 
										n_, B_m, m_, unity,
										&data_[0], m_,
										&B[0], B_m,
										zero, &result[0], B_m);	
			}
			else
			{
				cblas_cgemm(CblasColMajor, 
										CblasNoTrans, CblasNoTrans, 
										n_, B_m, m_, unity,
										&data_[0], n_,
										&B[0], B_n,
										zero, &result[0], n_);	

				result.setRowMajor(false);
			}
		}
		else // the storage formats differ
		{
			if (row_major_) // B is column major
			{
				cblas_cgemm(CblasRowMajor, 
										CblasNoTrans, CblasTrans, 
										n_, B_m, m_, unity,
										&data_[0], m_,
										&B[0], B_n,
										zero, &result[0], B_m);	
			}
			else // this matrix is column major, B is row major
			{
				cblas_cgemm(CblasRowMajor, 
										CblasTrans, CblasNoTrans, 
										n_, B_m, m_, unity,
										&data_[0], n_,
										&B[0], B_m,
										zero, &result[0], B_m);	
			}
		}

#else
		std::cerr << "BALL has been configured without BLAS support! Try the generic function instead!" << std::endl;
#endif
	}

	template <>
	void Matrix<ComplexDouble, StandardTraits>::multiply_blas(Matrix<ComplexDouble, StandardTraits>& result, const Matrix<ComplexDouble, StandardTraits>& B) const
	{
#ifdef BALL_HAS_BLAS
		// we'll need that one more than once...
		size_t B_n = B.getRowCount();
		size_t B_m = B.getColumnCount();

		// We'll need that one for the axpy
		ComplexDouble  unity_value(1.,0);
		void* unity = reinterpret_cast<void*>(&unity_value);
		ComplexDouble  zero_value(0,0);
		void* zero = reinterpret_cast<void*>(&unity_value);

		// if A and B have the same storage order, we'll use it for result as well
		bool B_row_major = B.isRowMajor();
		
		if (row_major_ == B_row_major)
		{
			// decide the storage format for the result
			if (row_major_)
			{
				cblas_zgemm(CblasRowMajor, 
										CblasNoTrans, CblasNoTrans, 
										n_, B_m, m_, unity,
										&data_[0], m_,
										&B[0], B_m,
										zero, &result[0], B_m);	
			}
			else
			{
				cblas_zgemm(CblasColMajor, 
										CblasNoTrans, CblasNoTrans, 
										n_, B_m, m_, unity,
										&data_[0], n_,
										&B[0], B_n,
										zero, &result[0], n_);	

				result.setRowMajor(false);
			}
		}
		else // the storage formats differ
		{
			if (row_major_) // B is column major
			{
				cblas_zgemm(CblasRowMajor, 
										CblasNoTrans, CblasTrans, 
										n_, B_m, m_, unity,
										&data_[0], m_,
										&B[0], B_n,
										zero, &result[0], B_m);	
			}
			else // this matrix is column major, B is row major
			{
				cblas_zgemm(CblasRowMajor, 
										CblasTrans, CblasNoTrans, 
										n_, B_m, m_, unity,
										&data_[0], n_,
										&B[0], B_m,
										zero, &result[0], B_m);	
			}
		}

#else
		std::cerr << "BALL has been configured without BLAS support! Try the generic function instead!" << std::endl;
#endif
	}
	
/// BasicFunctions

//template class Matrix<float>;
//template class Matrix<double>;
//template class Matrix<ComplexFloat>;
//template class Matrix<ComplexDouble>;


} // namespace BALL

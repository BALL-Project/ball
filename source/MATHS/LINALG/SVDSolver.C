#include <BALL/MATHS/LINALG/SVDSolver.h>

namespace BALL
{
	template <>
	bool SVDSolver<float, StandardTraits>::computeSVD_lapack()
		throw(Exception::OutOfMemory)
	{
#if defined(BALL_HAS_BLAS) && defined(BALL_HAS_LAPACK)
		// we need a copy of the bound matrix since lapack overwrites the memory.
		// NOTE: we could probably use either U or V_transposed for this task! We just
		//       would have to resize the matrix accordingly after the computation to make
		// 			 sure we prune away the residual space that might exist without erasing the
		//			 "real" singular vectors
		size_t size = matrix_->getSize();
		std::vector<float> matrix_copy(size);
		cblas_scopy(size, &(*matrix_)[0], 1, (float*)&(matrix_copy[0]), 1);

		// lapack wants things in col major always. If our array was stored in row major,
		// we will have to compute the SVD of the transposed matrix instead. This means that
		// the roles of U and V will be exchanged and that we will have to transpose them afterwards.
		// This should be no problem since we want to return them as row-major matrices, while
		// lapack treats them as col-major ones. In addition, they are square, so we don't have to
		// do anything.
		bool row_major = matrix_->isRowMajor();

		int n = matrix_->getRowCount();
		int m = matrix_->getColumnCount();

		int   rows = (!row_major) ? n : m;
		int   cols = (!row_major) ? m : n;

		// convenient short names
		Matrix<SingularVectorType, StandardTraits>& U 						= left_singular_vectors_;
		Matrix<SingularVectorType, StandardTraits>& V_transposed = right_singular_vectors_;

		U.resize(n, n);
		U.setRowMajor(row_major);

		V_transposed.resize(m, m);
		V_transposed.setRowMajor(row_major);

		int min_mn = std::min(n, m);
		int max_mn = std::max(n, m);
		singular_values_.resize(min_mn);

		// we will need a fixed-size integer workspace for lapack
		std::vector<int> integer_workspace(8*min_mn);

		// and floating point workspace of a size which we try to determine using lapack

		float workspace_dummy;
		int   worksize = -1;
		int   info = 0;

		char jobtype = 'A';

		// perform worksize query
		if (!row_major)
			sgesdd_(&jobtype, &rows, &cols, &(matrix_copy[0]), &rows,
					&(singular_values_[0]), &(U[0]), &rows, &(V_transposed[0]), &cols,
					&(workspace_dummy), &worksize, &(integer_workspace[0]), &info);
		else // exchange the role of U and Vt
			sgesdd_(&jobtype, &rows, &cols, &(matrix_copy[0]), &rows,
					&(singular_values_[0]), &(V_transposed[0]), &rows, &(U[0]), &cols,
					&(workspace_dummy), &worksize, &(integer_workspace[0]), &info);

		// if _this_ does not work we should better not try anything at all
		if (info)
			return false;

		// try to allocate the memory
		float *workspace = (float*) malloc((int)workspace_dummy*sizeof(float));
		if (workspace == 0)
		{
			bool finished = false;
			// try to half the size as long as the workspace is still large enough
			while (workspace_dummy >= 2*(3*min_mn*min_mn +
													std::max(max_mn,4*min_mn*min_mn+4*min_mn)) && !finished)
			{
				workspace_dummy /= 2;
				float *workspace = (float*) malloc((int)workspace_dummy*sizeof(float));
				if (workspace != 0)
					finished = true;
			}

			if (!finished)
			{
				// try one last time
				workspace_dummy = (3*min_mn*min_mn +std::max(max_mn,4*min_mn*min_mn+4*min_mn));
				float *workspace = (float*) malloc((int)workspace_dummy*sizeof(float));

				if (workspace == 0)
					return false;
			}
		}

		worksize = (int)workspace_dummy;

		// and perform the computation
		if (!row_major)
			sgesdd_(&jobtype, &rows, &cols, &(matrix_copy[0]), &rows,
					&(singular_values_[0]), &(U[0]), &rows, &(V_transposed[0]), &cols,
					workspace, &worksize, &(integer_workspace[0]), &info);
		else // exchange the role of U and Vt
			sgesdd_(&jobtype, &rows, &cols, &(matrix_copy[0]), &rows,
					&(singular_values_[0]), &(V_transposed[0]), &rows, &(U[0]), &cols,
					workspace, &worksize, &(integer_workspace[0]), &info);

		// clean up
		if (workspace)
			free(workspace);

		// TODO: better error handling
		if (info)
			return false;

		// done.
		return true;
#else
		Log.error() << "SVDSolver.C: Can not compute SVD! BALL was configured without BLAS or LAPACK support!" << std::endl;

		return false;
#endif
	}
			
					
	template <>
	bool SVDSolver<double, StandardTraits>::computeSVD_lapack()
		throw(Exception::OutOfMemory)
	{
#if defined(BALL_HAS_BLAS) && defined(BALL_HAS_LAPACK)
		// we need a copy of our matrix since lapack overwrites the memory.
		// NOTE: we could probably use either U or V_transposed for this task! We just
		//       would have to resize the matrix accordingly after the computation to make
		// 			 sure we prune away the residual space that might exist without erasing the
		//			 "real" singular vectors
		size_t size = matrix_->getSize();
		std::vector<double> matrix_copy(size);
		cblas_dcopy(size, &(*matrix_)[0], 1, (double*)&(matrix_copy[0]), 1);
		
		
		// Regularize matrix_copy in order to prevent strange effects when netlib-lapack assumes matissa of length 53bit instead of 52bit.
		// Else lapack sometimes computes incorrect singular values that might even turn out to be 'nan' !!
		// Note that (on most systems) you still need to force your program to use the exact IEEE754 double-precision (52bit mantissa).
		for(unsigned int i=0; i<matrix_copy.size();i+=matrix_->getColumnCount()+1)
		{
			matrix_copy[i] += 1e-14;
		}
		

		// lapack wants things in col major always. If our array was stored in row major,
		// we will have to compute the SVD of the transposed matrix instead. This means that
		// the roles of U and V will be exchanged and that we will have to transpose them afterwards.
		// This should be no problem since we want to return them as row-major matrices, while
		// lapack treats them as col-major ones. In addition, they are square, so we don't have to
		// do anything.
		bool row_major = matrix_->isRowMajor();

		int n = matrix_->getRowCount();
		int m = matrix_->getColumnCount();

		int   rows = (!row_major) ? n : m;
		int   cols = (!row_major) ? m : n;

		// convenient short names
		Matrix<SingularVectorType, StandardTraits>& U 						= left_singular_vectors_;
		Matrix<SingularVectorType, StandardTraits>& V_transposed = right_singular_vectors_;

		U.resize(n, n);
		U.setRowMajor(row_major);

		V_transposed.resize(m, m);
		V_transposed.setRowMajor(row_major);

		int min_mn = std::min(n, m);
		int max_mn = std::max(n, m);
		singular_values_.resize(min_mn);

		// we will need a fixed-size integer workspace for lapack
		std::vector<int> integer_workspace(8*min_mn);

		// and floating point workspace of a size which we try to determine using lapack

		double workspace_dummy;
		int    worksize = -1;
		int    info = 0;

		char jobtype = 'A';

		// perform worksize query
		if (!row_major)
			dgesdd_(&jobtype, &rows, &cols, &(matrix_copy[0]), &rows,
					&(singular_values_[0]), &(U[0]), &rows, &(V_transposed[0]), &cols,
					&(workspace_dummy), &worksize, &(integer_workspace[0]), &info);
		else // exchange the role of U and Vt
			dgesdd_(&jobtype, &rows, &cols, &(matrix_copy[0]), &rows,
					&(singular_values_[0]), &(V_transposed[0]), &rows, &(U[0]), &cols,
					&(workspace_dummy), &worksize, &(integer_workspace[0]), &info);

		// if _this_ does not work we should better not try anything at all
		if (info)
			return false;

		// try to allocate the memory
		double *workspace = (double*) malloc((int)workspace_dummy*sizeof(double));
		if (workspace == 0)
		{
			bool finished = false;
			// try to half the size as long as the workspace is still large enough
			while (workspace_dummy >= 2*(3*min_mn*min_mn +
													std::max(max_mn,4*min_mn*min_mn+4*min_mn)) && !finished)
			{
				workspace_dummy /= 2;
				double *workspace = (double*) malloc((int)workspace_dummy*sizeof(double));
				if (workspace != 0)
					finished = true;
			}

			if (!finished)
			{
				// try one last time
				workspace_dummy = (3*min_mn*min_mn +std::max(max_mn,4*min_mn*min_mn+4*min_mn));
				double *workspace = (double*) malloc((int)workspace_dummy*sizeof(double));

				if (workspace == 0)
					return false;
			}
		}

		worksize = (int)workspace_dummy;

		// and perform the computation
		if (!row_major)
			dgesdd_(&jobtype, &rows, &cols, &(matrix_copy[0]), &rows,
					&(singular_values_[0]), &(U[0]), &rows, &(V_transposed[0]), &cols,
					workspace, &worksize, &(integer_workspace[0]), &info);
		else // exchange the role of U and Vt
			dgesdd_(&jobtype, &rows, &cols, &(matrix_copy[0]), &rows,
					&(singular_values_[0]), &(V_transposed[0]), &rows, &(U[0]), &cols,
					workspace, &worksize, &(integer_workspace[0]), &info);

		// clean up
		if (workspace)
			free(workspace);

		// TODO: better error handling
		if (info)
			return false;

		// done.
		return true;
#else
		Log.error() << "SVDSolver.C: Can not compute SVD! BALL was configured without BLAS or LAPACK support!" << std::endl;

		return false;
#endif
	}

	template <>
	bool SVDSolver<ComplexFloat, StandardTraits>::computeSVD_lapack()
		throw(Exception::OutOfMemory)
	{
#if defined(BALL_HAS_BLAS) && defined(BALL_HAS_LAPACK)
		// we need a copy of our matrix since lapack overwrites the memory.
		// NOTE: we could probably use either U or V_transposed for this task! We just
		//       would have to resize the matrix accordingly after the computation to make
		// 			 sure we prune away the residual space that might exist without erasing the
		//			 "real" singular vectors
		size_t size = matrix_->getSize();
		std::vector<ComplexFloat> matrix_copy(size);
		cblas_ccopy(size, &(*matrix_)[0], 1, (float*)&(matrix_copy[0]), 1);

		// lapack wants things in col major always. If our array was stored in row major,
		// we will have to compute the SVD of the transposed matrix instead. This means that
		// the roles of U and V will be exchanged and that we will have to transpose them afterwards.
		// This should be no problem since we want to return them as row-major matrices, while
		// lapack treats them as col-major ones. In addition, they are square, so we don't have to
		// do anything.
		bool row_major = matrix_->isRowMajor();

		int n = matrix_->getRowCount();
		int m = matrix_->getColumnCount();

		int   rows = (!row_major) ? n : m;
		int   cols = (!row_major) ? m : n;

		// convenient short names
		Matrix<SingularVectorType, StandardTraits>& U 					 = left_singular_vectors_;
		Matrix<SingularVectorType, StandardTraits>& V_transposed = right_singular_vectors_;

		U.resize(n, n);
		U.setRowMajor(row_major);

		V_transposed.resize(m, m);
		V_transposed.setRowMajor(row_major);

		int min_mn = std::min(n, m);
		int max_mn = std::max(n, m);
		singular_values_.resize(min_mn);

		// we will need a fixed-size integer workspace for lapack
		std::vector<int> integer_workspace(8*min_mn);

		// and a fixed-size real workspace
		std::vector<float> real_workspace(5*min_mn*min_mn+7*min_mn);

		// and floating point workspace of a size which we try to determine using lapack

		ComplexFloat workspace_dummy;
		int   			 worksize = -1;
		int   			 info = 0;

		char jobtype = 'A';

		// perform worksize query
		if (!row_major)
			cgesdd_(&jobtype, &rows, &cols, &(matrix_copy[0]), &rows,
					&(singular_values_[0]), &(U[0]), &rows, &(V_transposed[0]), &cols,
					&(workspace_dummy), &worksize, &(real_workspace[0]), &(integer_workspace[0]), &info);
		else // exchange the role of U and Vt
			cgesdd_(&jobtype, &rows, &cols, &(matrix_copy[0]), &rows,
					&(singular_values_[0]), &(V_transposed[0]), &rows, &(U[0]), &cols,
					&(workspace_dummy), &worksize, &(real_workspace[0]), &(integer_workspace[0]), &info);

		// if _this_ does not work we should better not try anything at all
		if (info)
			return false;

		// try to allocate the memory
		ComplexFloat *workspace = (ComplexFloat*) malloc((int)workspace_dummy.real()*sizeof(ComplexFloat));
		if (workspace == 0)
		{
			bool finished = false;
			// try to half the size as long as the workspace is still large enough
			while (workspace_dummy.real() >= (min_mn*min_mn+2*min_mn+max_mn) && !finished)
			{
				workspace_dummy /= 2;
				ComplexFloat *workspace = (ComplexFloat*) malloc((int)workspace_dummy.real()*sizeof(ComplexFloat));
				if (workspace != 0)
					finished = true;
			}

			if (!finished)
			{
				// try one last time
				workspace_dummy = ComplexFloat((min_mn*min_mn+2*min_mn+max_mn), 0);
				ComplexFloat *workspace = (ComplexFloat*) malloc((int)workspace_dummy.real()*sizeof(ComplexFloat));

				if (workspace == 0)
					return false;
			}
		}

		worksize = (int)workspace_dummy.real();

		// and perform the computation
		if (!row_major)
			cgesdd_(&jobtype, &rows, &cols, &(matrix_copy[0]), &rows,
					&(singular_values_[0]), &(U[0]), &rows, &(V_transposed[0]), &cols,
					workspace, &worksize, &(real_workspace[0]), &(integer_workspace[0]), &info);
		else // exchange the role of U and Vt
			cgesdd_(&jobtype, &rows, &cols, &(matrix_copy[0]), &rows,
					&(singular_values_[0]), &(V_transposed[0]), &rows, &(U[0]), &cols,
					workspace, &worksize, &(real_workspace[0]), &(integer_workspace[0]), &info);

		// clean up
		if (workspace)
			free(workspace);

		// TODO: better error handling
		if (info)
			return false;

		// done.
		return true;
#else
		Log.error() << "SVDSolver.C: Can not compute SVD! BALL was configured without BLAS or LAPACK support!" << std::endl;

		return false;
#endif
	}

	template <>
	bool SVDSolver<ComplexDouble, StandardTraits>::computeSVD_lapack()
		throw(Exception::OutOfMemory)
	{
#if defined(BALL_HAS_BLAS) && defined(BALL_HAS_LAPACK)
		// we need a copy of our matrix since lapack overwrites the memory.
		// NOTE: we could probably use either U or V_transposed for this task! We just
		//       would have to resize the matrix accordingly after the computation to make
		// 			 sure we prune away the residual space that might exist without erasing the
		//			 "real" singular vectors
		size_t size = matrix_->getSize();
		std::vector<ComplexDouble> matrix_copy(size);
		cblas_zcopy(size, &(*matrix_)[0], 1, (float*)&(matrix_copy[0]), 1);

		// lapack wants things in col major always. If our array was stored in row major,
		// we will have to compute the SVD of the transposed matrix instead. This means that
		// the roles of U and V will be exchanged and that we will have to transpose them afterwards.
		// This should be no problem since we want to return them as row-major matrices, while
		// lapack treats them as col-major ones. In addition, they are square, so we don't have to
		// do anything.
		bool row_major = matrix_->isRowMajor();

		int n = matrix_->getRowCount();
		int m = matrix_->getColumnCount();

		int   rows = (!row_major) ? n : m;
		int   cols = (!row_major) ? m : n;
		
		// convenient short names
		Matrix<SingularVectorType, StandardTraits>& U 						= left_singular_vectors_;
		Matrix<SingularVectorType, StandardTraits>& V_transposed = right_singular_vectors_;

		U.resize(n, n);
		U.setRowMajor(row_major);

		V_transposed.resize(m, m);
		V_transposed.setRowMajor(row_major);

		int min_mn = std::min(n, m);
		int max_mn = std::max(n, m);
		singular_values_.resize(min_mn);

		// we will need a fixed-size integer workspace for lapack
		std::vector<int> integer_workspace(8*min_mn);

		// and a fixed-size real workspace
		std::vector<double> real_workspace(5*min_mn*min_mn+7*min_mn);

		// and floating point workspace of a size which we try to determine using lapack

		ComplexDouble workspace_dummy;
		int   			  worksize = -1;
		int   			  info = 0;

		char jobtype = 'A';

		// perform worksize query
		if (!row_major)
			zgesdd_(&jobtype, &rows, &cols, &(matrix_copy[0]), &rows,
					&(singular_values_[0]), &(U[0]), &rows, &(V_transposed[0]), &cols,
					&(workspace_dummy), &worksize, &(real_workspace[0]), &(integer_workspace[0]), &info);
		else // exchange the role of U and Vt
			zgesdd_(&jobtype, &rows, &cols, &(matrix_copy[0]), &rows,
					&(singular_values_[0]), &(V_transposed[0]), &rows, &(U[0]), &cols,
					&(workspace_dummy), &worksize, &(real_workspace[0]), &(integer_workspace[0]), &info);

		// if _this_ does not work we should better not try anything at all
		if (info)
			return false;

		// try to allocate the memory
		ComplexDouble *workspace = (ComplexDouble*) malloc((int)workspace_dummy.real()*sizeof(ComplexDouble));
		if (workspace == 0)
		{
			bool finished = false;
			// try to half the size as long as the workspace is still large enough
			while (workspace_dummy.real() >= (min_mn*min_mn+2*min_mn+max_mn) && !finished)
			{
				workspace_dummy /= 2;
				ComplexDouble *workspace = (ComplexDouble*) malloc((int)workspace_dummy.real()*sizeof(ComplexDouble));
				if (workspace != 0)
					finished = true;
			}

			if (!finished)
			{
				// try one last time
				workspace_dummy = ComplexDouble((min_mn*min_mn+2*min_mn+max_mn), 0);
				ComplexDouble *workspace = (ComplexDouble*) malloc((int)workspace_dummy.real()*sizeof(ComplexDouble));

				if (workspace == 0)
					return false;
			}
		}

		worksize = (int)workspace_dummy.real();

		// and perform the computation
		if (!row_major)
			zgesdd_(&jobtype, &rows, &cols, &(matrix_copy[0]), &rows,
					&(singular_values_[0]), &(U[0]), &rows, &(V_transposed[0]), &cols,
					workspace, &worksize, &(real_workspace[0]), &(integer_workspace[0]), &info);
		else // exchange the role of U and Vt
			zgesdd_(&jobtype, &rows, &cols, &(matrix_copy[0]), &rows,
					&(singular_values_[0]), &(V_transposed[0]), &rows, &(U[0]), &cols,
					workspace, &worksize, &(real_workspace[0]), &(integer_workspace[0]), &info);

		// clean up
		if (workspace)
			free(workspace);

		// TODO: better error handling
		if (info)
			return false;

		// done.
		return true;
#else
		Log.error() << "SVDSolver.C: Can not compute SVD! BALL was configured without BLAS or LAPACK support!" << std::endl;

		return false;
#endif
	}
}

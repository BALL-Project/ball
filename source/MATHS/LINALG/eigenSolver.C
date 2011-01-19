#ifndef BALL_LINALG_EIGENSOLVER_H
# include <BALL/MATHS/LINALG/eigenSolver.h>
#endif

#include <algorithm>

namespace BALL {

	// ----- EigenSolver::solveRightEigenProblem_lapack ----- //
	template <> 
	bool EigenSolver<float, StandardTraits>::solveRightEigenProblem_lapack()
	{
#if defined(BALL_HAS_BLAS) && defined(BALL_HAS_LAPACK)
		// we need a copy of the bound matrix since lapack overwrites the memory.
		// also, we need a _complex_ matrix and can do this quite simply here
		size_t size = matrix_->getSize();
		std::vector<ComplexFloat> matrix_copy(size);
		cblas_scopy(size, &(*matrix_)[0], 1, (float*)&(matrix_copy[0]), 2);

		// lapack wants things in col major always. If our array was stored in
		// row major, compute the left eigenvectors instead.
		char left_eigenvecs, right_eigenvecs;
		if (matrix_->isRowMajor())
		{
			left_eigenvecs  = 'V';
			right_eigenvecs = 'N';
		} else {
			right_eigenvecs = 'V';
			left_eigenvecs  = 'N';
		}

		size_t n = matrix_->getRowCount();

		// clear the input matrix and ensure it uses the right storage format
		right_eigenvectors_.resize(n, n);
		right_eigenvectors_.setRowMajor(false);

		// make room for the right_eigenvalues_
		right_eigenvalues_.resize(n);

		// and some floating point workspace for cgeev_
		std::vector<float> real_workspace(2*n);

		// now we need to determine the size of the complex workspace lapack wants us to
		// provide
		ComplexFloat workspace_dummy;
		int worksize = -1;
		int info = 0;
		int rows = n;
		
		cgeev_(&left_eigenvecs, &right_eigenvecs, &rows, &(matrix_copy[0]), &rows,
					 &(right_eigenvalues_[0]), &(right_eigenvectors_[0]), &rows,
					 &(right_eigenvectors_[0]), &rows, &workspace_dummy, &worksize, &(real_workspace[0]), &info);

		// try to allocate the memory
		ComplexFloat *workspace = (ComplexFloat*) malloc((int)workspace_dummy.real()*sizeof(ComplexFloat));
		if (workspace == 0)
		{
			// try four times to half the size... if that doesn't work, we give up and
			// use no workspace at all
			for (int i=0; i<4; i++)
			{
				workspace_dummy /= 2;
				workspace = (ComplexFloat*) malloc((int)workspace_dummy.real()*sizeof(ComplexFloat));
				if (workspace != 0)
					break;
			}
		}

		worksize = (int)workspace_dummy.real();

		// and perform the computation
		cgeev_(&left_eigenvecs, &right_eigenvecs, &rows, &(matrix_copy[0]), &rows,
					 &(right_eigenvalues_[0]), &(right_eigenvectors_[0]), &rows,
					 &(right_eigenvectors_[0]), &rows, workspace, &worksize, &(real_workspace[0]), &info);

		// if we have used the transpose for the eigenvector computation, we need to conjugate it back
		if (matrix_->isRowMajor())
			for (size_t i=0; i<right_eigenvectors_.getSize(); i++)
				*(reinterpret_cast<float*>(&right_eigenvectors_[i])+1) *=-1; // this hack gives us the imaginary part...

		// clean up
		if (workspace)
			free(workspace);

		// TODO: better error handling
		if (info)
			return false;

		// done.
		return true;
#else
		Log.error() << "eigenSolver.C: Can not compute eigenvalues / eigenvectors! BALL was configured without BLAS or LAPACK support!" << std::endl;

		return false;
#endif
	}

	template <> 
	bool EigenSolver<double, StandardTraits>::solveRightEigenProblem_lapack()
	{
#if defined(BALL_HAS_BLAS) && defined(BALL_HAS_LAPACK)
		// we need a copy of our matrix since lapack overwrites the memory.
		// also, we need a _complex_ matrix and can do this quite simply here
		size_t size = matrix_->getSize();
		std::vector<ComplexDouble> matrix_copy(size);
		cblas_dcopy(size, &(*matrix_)[0], 1, (double*)&(matrix_copy[0]), 2);

		// lapack wants things in col major always. If our array was stored in
		// row major, compute the left eigenvectors instead.
		char left_eigenvecs, right_eigenvecs;
		if (matrix_->isRowMajor())
		{
			left_eigenvecs  = 'V';
			right_eigenvecs = 'N';
		} else {
			right_eigenvecs = 'V';
			left_eigenvecs  = 'N';
		}

		size_t n = matrix_->getRowCount();

		// clear the input matrix and ensure it uses the right storage format
		right_eigenvectors_.resize(n, n);
		right_eigenvectors_.setRowMajor(false);

		// make room for the right_eigenvalues_
		right_eigenvalues_.resize(n);

		// and some floating point workspace for cgeev_
		std::vector<double> real_workspace(2*n);

		// now we need to determine the size of the complex workspace lapack wants us to
		// provide
		ComplexDouble workspace_dummy;
		int worksize = -1;
		int info = 0;
		int rows = n;
		
		zgeev_(&left_eigenvecs, &right_eigenvecs, &rows, &(matrix_copy[0]), &rows,
					 &(right_eigenvalues_[0]), &(right_eigenvectors_[0]), &rows,
					 &(right_eigenvectors_[0]), &rows, &workspace_dummy, &worksize, &(real_workspace[0]), &info);

		// try to allocate the memory
		ComplexDouble *workspace = (ComplexDouble*) malloc((int)workspace_dummy.real()*sizeof(ComplexDouble));
		if (workspace == 0)
		{
			// try four times to half the size... if that doesn't work, we give up and
			// use no workspace at all
			for (int i=0; i<4; i++)
			{
				workspace_dummy /= 2;
				workspace = (ComplexDouble*) malloc((int)workspace_dummy.real()*sizeof(ComplexDouble));
				if (workspace != 0)
					break;
			}
		}

		worksize = (int)workspace_dummy.real();

		// and perform the computation
		zgeev_(&left_eigenvecs, &right_eigenvecs, &rows, &(matrix_copy[0]), &rows,
					 &(right_eigenvalues_[0]), &(right_eigenvectors_[0]), &rows,
					 &(right_eigenvectors_[0]), &rows, workspace, &worksize, &(real_workspace[0]), &info);

		// if we have used the transpose for the eigenvector computation, we need to conjugate it back
		if (matrix_->isRowMajor())
			for (size_t i=0; i<right_eigenvectors_.getSize(); i++)
				*(reinterpret_cast<double*>(&right_eigenvectors_[i])+1) *=-1; // this hack gives us the imaginary part...

		// clean up
		if (workspace)
			free(workspace);

		// TODO: better error handling
		if (info)
			return false;

		// done.
		return true;
#else
		Log.error() << "eigenSolver.C: Can not compute eigenvalues / eigenvectors! BALL was configured without BLAS or LAPACK support!" << std::endl;

		return false;
#endif
	}

	template <>
	bool EigenSolver<ComplexFloat, StandardTraits>::solveRightEigenProblem_lapack()
	{
#if defined(BALL_HAS_BLAS) && defined(BALL_HAS_LAPACK)
		// we need a copy of our matrix since lapack overwrites the memory.
		size_t size = matrix_->getSize();
		std::vector<ComplexFloat> matrix_copy(size);
		cblas_ccopy(size, &(*matrix_)[0], 1, &(matrix_copy[0]), 1);

		// lapack wants things in col major always. If our array was stored in
		// row major, compute the left eigenvectors instead.
		char left_eigenvecs, right_eigenvecs;
		if (matrix_->isRowMajor())
		{
			left_eigenvecs  = 'V';
			right_eigenvecs = 'N';
		} else {
			right_eigenvecs = 'V';
			left_eigenvecs  = 'N';
		}

		size_t n = matrix_->getRowCount();

		// clear the input matrix and ensure it uses col major (lapack wants it that way)
		right_eigenvectors_.resize(n, n);
		right_eigenvectors_.setRowMajor(false);

		// make room for the right_eigenvalues_
		right_eigenvalues_.resize(n);

		// and some floating point workspace for cgeev_
		std::vector<float> real_workspace(2*n);

		// now we need to determine the size of the complex workspace lapack wants us to
		// provide
		ComplexFloat workspace_dummy;
		int worksize = -1;
		int info = 0;
		int rows = n;
		cgeev_(&left_eigenvecs, &right_eigenvecs, &rows, &(matrix_copy[0]), &rows,
					 &(right_eigenvalues_[0]), &(right_eigenvectors_[0]), &rows,
					 &(right_eigenvectors_[0]), &rows, &workspace_dummy, &worksize, &(real_workspace[0]), &info);

		// try to allocate the memory
		ComplexFloat *workspace = (ComplexFloat*) malloc((int)(workspace_dummy.real())*sizeof(ComplexFloat));
		if (workspace == 0)
		{
			// try four times to half the size... if that doesn't work, we give up and
			// use no workspace at all
			for (int i=0; i<4; i++)
			{
				workspace_dummy /= 2;
				workspace = (ComplexFloat*) malloc((int)(workspace_dummy.real())*sizeof(ComplexFloat));
				if (workspace != 0)
					break;
			}
		}

		worksize = (int)workspace_dummy.real();

		// and perform the computation
		cgeev_(&left_eigenvecs, &right_eigenvecs, &rows, &(matrix_copy[0]), &rows,
					 &(right_eigenvalues_[0]), &(right_eigenvectors_[0]), &rows,
					 &(right_eigenvectors_[0]), &rows, workspace, &worksize, &(real_workspace[0]), &info);

		// if we have used the transpose for the eigenvector computation, we need to conjugate it back
		if (matrix_->isRowMajor())
			for (size_t i=0; i<right_eigenvectors_.getSize(); i++)
				*(reinterpret_cast<float*>(&right_eigenvectors_[i])+1) *=-1; // this hack gives us the imaginary part...

		// clean up
		if (workspace)
			free(workspace);

		// TODO: better error handling
		if (info)
			return false;

		// done.
		return true;
#else
		Log.error() << "eigenSolver.C: Can not compute eigenvalues / eigenvectors! BALL was configured without BLAS or LAPACK support!" << std::endl;

		return false;
#endif
	}

	template <>
	bool EigenSolver<ComplexDouble, StandardTraits>::solveRightEigenProblem_lapack()
	{
#if defined(BALL_HAS_BLAS) && defined(BALL_HAS_LAPACK)
		// we need a copy of our matrix since lapack overwrites the memory.
		size_t size = matrix_->getSize();
		std::vector<ComplexDouble> matrix_copy(size);
		cblas_zcopy(size, &(*matrix_)[0], 1, &(matrix_copy[0]), 1);

		// lapack wants things in col major always. If our array was stored in
		// row major, compute the left eigenvectors instead.
		char left_eigenvecs, right_eigenvecs;
		if (matrix_->isRowMajor())
		{
			left_eigenvecs  = 'V';
			right_eigenvecs = 'N';
		} else {
			right_eigenvecs = 'V';
			left_eigenvecs  = 'N';
		}

		size_t n = matrix_->getRowCount();
		
		// clear the input matrix and ensure it uses col major (lapack wants it that way)
		right_eigenvectors_.resize(n, n);
		right_eigenvectors_.setRowMajor(false);

		// make room for the right_eigenvalues_
		right_eigenvalues_.resize(n);

		// and some floating point workspace for cgeev_
		std::vector<double> real_workspace(2*n);

		// now we need to determine the size of the complex workspace lapack wants us to
		// provide
		ComplexDouble workspace_dummy;
		int worksize = -1;
		int info = 0;
		int rows = n;
		zgeev_(&left_eigenvecs, &right_eigenvecs, &rows, &(matrix_copy[0]), &rows,
				&(right_eigenvalues_[0]), &(right_eigenvectors_[0]), &rows,
				&(right_eigenvectors_[0]), &rows, &workspace_dummy, &worksize, &(real_workspace[0]), &info);

		// try to allocate the memory
		ComplexDouble *workspace = (ComplexDouble*) malloc((int)(workspace_dummy.real())*sizeof(ComplexDouble));
		if (workspace == 0)
		{
			// try four times to half the size... if that doesn't work, we give up and
			// use no workspace at all
			for (int i=0; i<4; i++)
			{
				workspace_dummy /= 2;
				workspace = (ComplexDouble*) malloc((int)(workspace_dummy.real())*sizeof(ComplexDouble));
				if (workspace != 0)
					break;
			}
		}

		worksize = (int)workspace_dummy.real();

		// and perform the computation
		zgeev_(&left_eigenvecs, &right_eigenvecs, &rows, &(matrix_copy[0]), &rows,
				&(right_eigenvalues_[0]), &(right_eigenvectors_[0]), &rows,
				&(right_eigenvectors_[0]), &rows, workspace, &worksize, &(real_workspace[0]), &info);

		// if we have used the transpose for the eigenvector computation, we need to conjugate it back
		if (matrix_->isRowMajor())
			for (size_t i=0; i<right_eigenvectors_.getSize(); i++)
				*(reinterpret_cast<double*>(&right_eigenvectors_[i])+1) *=-1; // this hack gives us the imaginary part...

		// clean up
		if (workspace)
			free(workspace);

		// TODO: better error handling
		if (info)
			return false;

		// done.
		return true;
#else
		Log.error() << "eigenSolver.C: Can not compute eigenvalues / eigenvectors! BALL was configured without BLAS or LAPACK support!" << std::endl;

		return false;
#endif
	} 

	template <> 
	bool EigenSolver<float, SymmetricTraits>::solveRightEigenProblem_lapack()
	{
#if defined(BALL_HAS_BLAS) && defined(BALL_HAS_LAPACK)
		// we need a copy of the bound matrix since lapack overwrites the memory.
		size_t size = matrix_->getSize();
		std::vector<float> matrix_copy(size);
		cblas_scopy(size, &(*matrix_)[0], 1, (float*)&(matrix_copy[0]), 1);

		// lapack wants things in col major always. Fortunately, for symmetric matrices,
		// the left and right eigenvectors correspond, so we don't mind which one is used 
		// we just have to switch between upper and lower triangular storage
		char eigenvecs = 'V';
		char uplo = matrix_->isRowMajor() ? 'L' : 'U';
		size_t n = matrix_->getRowCount();

		// clear the input matrix and ensure it uses the right storage format
		right_eigenvectors_.resize(n, n);
		right_eigenvectors_.setRowMajor(false);

		// make room for the right_eigenvalues_
		right_eigenvalues_.resize(n);

		// now we need to determine the size of the workspace lapack wants us to provide
		float real_workspace_dummy;
		int    int_workspace_dummy;

		int real_worksize = -1;
		int  int_worksize = -1;

		int info = 0;
		int rows = n;
		
		sspevd_(&eigenvecs, &uplo, &rows, &(matrix_copy[0]),
						&(right_eigenvalues_[0]), &(right_eigenvectors_[0]), &rows, 
						&real_workspace_dummy, &real_worksize, &int_workspace_dummy, &int_worksize, &info);

		// try to allocate the memory
		float *real_workspace = (float*) malloc((int)real_workspace_dummy*sizeof(float));
		if (real_workspace == 0)
		{
			// try four times to half the size... if that doesn't work, we give up and
			// use no workspace at all
			for (int i=0; i<4; i++)
			{
				real_workspace_dummy /= 2;
				real_workspace = (float*) malloc((int)real_workspace_dummy*sizeof(float));
				if (real_workspace != 0)
					break;
			}
		}
		real_worksize = (int)real_workspace_dummy;

		int *int_workspace = (int*) malloc((int)int_workspace_dummy*sizeof(int));
		if (int_workspace == 0)
		{
			// try four times to half the size... if that doesn't work, we give up and
			// use no workspace at all
			for (int i=0; i<4; i++)
			{
				int_workspace_dummy /= 2;
				int_workspace = (int*) malloc((int)int_workspace_dummy*sizeof(int));
				if (int_workspace != 0)
					break;
			}
		}
		int_worksize = (int)int_workspace_dummy;

		// and perform the computation
		sspevd_(&eigenvecs, &uplo, &rows, &(matrix_copy[0]),
						&(right_eigenvalues_[0]), &(right_eigenvectors_[0]), &rows, 
						real_workspace, &real_worksize, int_workspace, &int_worksize, &info);

		// clean up
		free(real_workspace);
		free( int_workspace);

		// TODO: better error handling
		if (info)
			return false;

		// done.
		return true;
#else
		Log.error() << "eigenSolver.C: Can not compute eigenvalues / eigenvectors! BALL was configured without BLAS or LAPACK support!" << std::endl;

		return false;
#endif
	}

	template <> 
	bool EigenSolver<double, SymmetricTraits>::solveRightEigenProblem_lapack()
	{
#if defined(BALL_HAS_BLAS) && defined(BALL_HAS_LAPACK)
		// we need a copy of the bound matrix since lapack overwrites the memory.
		size_t size = matrix_->getSize();
		std::vector<double> matrix_copy(size);
		cblas_dcopy(size, &(*matrix_)[0], 1, (double*)&(matrix_copy[0]), 1);

		// lapack wants things in col major always. Fortunately, for symmetric matrices,
		// the left and right eigenvectors correspond, so we don't mind which one is used 
		// we just have to switch between upper and lower triangular storage
		char eigenvecs = 'V';
		char uplo = matrix_->isRowMajor() ? 'L' : 'U';
		size_t n = matrix_->getRowCount();

		// clear the input matrix and ensure it uses the right storage format
		right_eigenvectors_.resize(n, n);
		right_eigenvectors_.setRowMajor(false);

		// make room for the right_eigenvalues_
		right_eigenvalues_.resize(n);

		// now we need to determine the size of the workspace lapack wants us to provide
		double real_workspace_dummy;
		int    int_workspace_dummy;

		int real_worksize = -1;
		int  int_worksize = -1;

		int info = 0;
		int rows = n;
		
		dspevd_(&eigenvecs, &uplo, &rows, &(matrix_copy[0]),
						&(right_eigenvalues_[0]), &(right_eigenvectors_[0]), &rows, 
						&real_workspace_dummy, &real_worksize, &int_workspace_dummy, &int_worksize, &info);

		// try to allocate the memory
		double *real_workspace = (double*) malloc((int)real_workspace_dummy*sizeof(double));
		if (real_workspace == 0)
		{
			// try four times to half the size... if that doesn't work, we give up and
			// use no workspace at all
			for (int i=0; i<4; i++)
			{
				real_workspace_dummy /= 2;
				real_workspace = (double*) malloc((int)real_workspace_dummy*sizeof(double));
				if (real_workspace != 0)
					break;
			}
		}
		real_worksize = (int)real_workspace_dummy;

		int *int_workspace = (int*) malloc((int)int_workspace_dummy*sizeof(int));
		if (int_workspace == 0)
		{
			// try four times to half the size... if that doesn't work, we give up and
			// use no workspace at all
			for (int i=0; i<4; i++)
			{
				int_workspace_dummy /= 2;
				int_workspace = (int*) malloc((int)int_workspace_dummy*sizeof(int));
				if (int_workspace != 0)
					break;
			}
		}
		int_worksize = (int)int_workspace_dummy;

		// and perform the computation
		dspevd_(&eigenvecs, &uplo, &rows, &(matrix_copy[0]),
						&(right_eigenvalues_[0]), &(right_eigenvectors_[0]), &rows, 
						real_workspace, &real_worksize, int_workspace, &int_worksize, &info);

		// clean up
		free(real_workspace);
		free( int_workspace);

		// TODO: better error handling
		if (info)
			return false;

		// done.
		return true;
#else
		Log.error() << "eigenSolver.C: Can not compute eigenvalues / eigenvectors! BALL was configured without BLAS or LAPACK support!" << std::endl;

		return false;
#endif
	}

	// ----- EigenSolver::sort ----- //

	template <>
	void EigenSolver<float, StandardTraits>::sort()
	{
		// performs an inline sort of the matrix using one vector as additional storage
		size_t n = right_eigenvalues_.getSize();

		// compute the permutation sorting the eigenvalues
		std::vector<std::pair<EigenValueType, Index> > sorting_permutation(n);
		for (size_t i=0; i<n; i++)
			sorting_permutation[i] = std::pair<EigenValueType, Index>(right_eigenvalues_[i], i);

		EigenSolver<float, StandardTraits>::EigenValueComparator_ comparator;
		std::sort(sorting_permutation.begin(), sorting_permutation.end(), comparator);

		// now we need a second array telling us which position each vector in the old array 
		// takes in the sorted one; this corresponds to sorting the indices of the sorting permutation again
		std::vector<Index> new_index_of_old_vector(n);
		for (size_t i=0; i<n; i++)
			new_index_of_old_vector[sorting_permutation[i].second] = i;

		// we need to know which vectors we already swapped
		std::vector<bool> already_visited(n);

		// we start our walk over the eigenvectors with the eigenvector corresponding to the 
		// first eigenvalue in the new sorting
		size_t cycle_index = 0;
		size_t index_new_order;
		size_t index_old_order;
		Vector<EigenVectorType> current_eigenvector(right_eigenvectors_.getRowCount());
		// TODO: we can get rid of this vector! we just need one temporary vector to swap
		Vector<EigenVectorType> temp(right_eigenvectors_.getRowCount());
		EigenValueType current_eigenvalue;

		// if the cycle index reaches the end of the vector, all vectors have been visited
		while (cycle_index < n)
		{
			index_new_order = cycle_index;
			index_old_order = sorting_permutation[index_new_order].second;

			right_eigenvectors_.copyColumnToVector(current_eigenvector, index_old_order+1);
			current_eigenvalue = right_eigenvalues_[index_old_order];

			while (!already_visited[index_new_order])
			{
				already_visited[index_new_order] = true;

				std::swap(right_eigenvalues_ [index_new_order],  current_eigenvalue);

				temp = current_eigenvector;
				right_eigenvectors_.copyColumnToVector(current_eigenvector, index_new_order+1);
				right_eigenvectors_.copyVectorToColumn(temp, index_new_order+1);

				index_new_order = new_index_of_old_vector[index_new_order];
			}

			while (already_visited[++cycle_index]) ;
		}
	}

	template <>
	void EigenSolver<double, StandardTraits>::sort()
	{
		// performs an inline sort of the matrix using one vector as additional storage
		size_t n = right_eigenvalues_.getSize();

		// compute the permutation sorting the eigenvalues
		std::vector<std::pair<EigenValueType, Index> > sorting_permutation(n);
		for (size_t i=0; i<n; i++)
			sorting_permutation[i] = std::pair<EigenValueType, Index>(right_eigenvalues_[i], i);

		EigenSolver<double, StandardTraits>::EigenValueComparator_ comparator;
		std::sort(sorting_permutation.begin(), sorting_permutation.end(), comparator);

		// now we need a second array telling us which position each vector in the old array 
		// takes in the sorted one; this corresponds to sorting the indices of the sorting permutation again
		std::vector<Index> new_index_of_old_vector(n);
		for (size_t i=0; i<n; i++)
			new_index_of_old_vector[sorting_permutation[i].second] = i;

		// we need to know which vectors we already swapped
		std::vector<bool> already_visited(n);

		// we start our walk over the eigenvectors with the eigenvector corresponding to the 
		// first eigenvalue in the new sorting
		size_t cycle_index = 0;
		size_t index_new_order;
		size_t index_old_order;
		Vector<EigenVectorType> current_eigenvector(right_eigenvectors_.getRowCount());
		// TODO: we can get rid of this vector! we just need one temporary vector to swap
		Vector<EigenVectorType> temp(right_eigenvectors_.getRowCount());
		EigenValueType current_eigenvalue;

		// if the cycle index reaches the end of the vector, all vectors have been visited
		while (cycle_index < n)
		{
			index_new_order = cycle_index;
			index_old_order = sorting_permutation[index_new_order].second;

			right_eigenvectors_.copyColumnToVector(current_eigenvector, index_old_order+1);
			current_eigenvalue = right_eigenvalues_[index_old_order];

			while (!already_visited[index_new_order])
			{
				already_visited[index_new_order] = true;

				std::swap(right_eigenvalues_ [index_new_order],  current_eigenvalue);

				temp = current_eigenvector;
				right_eigenvectors_.copyColumnToVector(current_eigenvector, index_new_order+1);
				right_eigenvectors_.copyVectorToColumn(temp, index_new_order+1);

				index_new_order = new_index_of_old_vector[index_new_order];
			}

			while (already_visited[++cycle_index]) ;
		}
	}

	template <>
	void EigenSolver<ComplexFloat, StandardTraits>::sort()
	{
		// performs an inline sort of the matrix using one vector as additional storage
		size_t n = right_eigenvalues_.getSize();

		// compute the permutation sorting the eigenvalues
		std::vector<std::pair<EigenValueType, Index> > sorting_permutation(n);
		for (size_t i=0; i<n; i++)
			sorting_permutation[i] = std::pair<EigenValueType, Index>(right_eigenvalues_[i], i);

		EigenSolver<ComplexFloat, StandardTraits>::EigenValueComparator_ comparator;
		std::sort(sorting_permutation.begin(), sorting_permutation.end(), comparator);

		// now we need a second array telling us which position each vector in the old array 
		// takes in the sorted one; this corresponds to sorting the indices of the sorting permutation again
		std::vector<Index> new_index_of_old_vector(n);
		for (size_t i=0; i<n; i++)
			new_index_of_old_vector[sorting_permutation[i].second] = i;

		// we need to know which vectors we already swapped
		std::vector<bool> already_visited(n);

		// we start our walk over the eigenvectors with the eigenvector corresponding to the 
		// first eigenvalue in the new sorting
		size_t cycle_index = 0;
		size_t index_new_order;
		size_t index_old_order;
		Vector<EigenVectorType> current_eigenvector(right_eigenvectors_.getRowCount());
		// TODO: we can get rid of this vector! we just need one temporary vector to swap
		Vector<EigenVectorType> temp(right_eigenvectors_.getRowCount());
		EigenValueType current_eigenvalue;

		// if the cycle index reaches the end of the vector, all vectors have been visited
		while (cycle_index < n)
		{
			index_new_order = cycle_index;
			index_old_order = sorting_permutation[index_new_order].second;

			right_eigenvectors_.copyColumnToVector(current_eigenvector, index_old_order+1);
			current_eigenvalue = right_eigenvalues_[index_old_order];

			while (!already_visited[index_new_order])
			{
				already_visited[index_new_order] = true;

				std::swap(right_eigenvalues_ [index_new_order],  current_eigenvalue);

				temp = current_eigenvector;
				right_eigenvectors_.copyColumnToVector(current_eigenvector, index_new_order+1);
				right_eigenvectors_.copyVectorToColumn(temp, index_new_order+1);

				index_new_order = new_index_of_old_vector[index_new_order];
			}

			while (already_visited[++cycle_index]) ;
		}
	}

	template <>
	void EigenSolver<ComplexDouble, StandardTraits>::sort()
	{
		// performs an inline sort of the matrix using one vector as additional storage
		size_t n = right_eigenvalues_.getSize();

		// compute the permutation sorting the eigenvalues
		std::vector<std::pair<EigenValueType, Index> > sorting_permutation(n);
		for (size_t i=0; i<n; i++)
			sorting_permutation[i] = std::pair<EigenValueType, Index>(right_eigenvalues_[i], i);

		EigenSolver<ComplexDouble, StandardTraits>::EigenValueComparator_ comparator;
		std::sort(sorting_permutation.begin(), sorting_permutation.end(), comparator);

		// now we need a second array telling us which position each vector in the old array 
		// takes in the sorted one; this corresponds to sorting the indices of the sorting permutation again
		std::vector<Index> new_index_of_old_vector(n);
		for (size_t i=0; i<n; i++)
			new_index_of_old_vector[sorting_permutation[i].second] = i;

		// we need to know which vectors we already swapped
		std::vector<bool> already_visited(n);

		// we start our walk over the eigenvectors with the eigenvector corresponding to the 
		// first eigenvalue in the new sorting
		size_t cycle_index = 0;
		size_t index_new_order;
		size_t index_old_order;
		Vector<EigenVectorType> current_eigenvector(right_eigenvectors_.getRowCount());
		// TODO: we can get rid of this vector! we just need one temporary vector to swap
		Vector<EigenVectorType> temp(right_eigenvectors_.getRowCount());
		EigenValueType current_eigenvalue;

		// if the cycle index reaches the end of the vector, all vectors have been visited
		while (cycle_index < n)
		{
			index_new_order = cycle_index;
			index_old_order = sorting_permutation[index_new_order].second;

			right_eigenvectors_.copyColumnToVector(current_eigenvector, index_old_order+1);
			current_eigenvalue = right_eigenvalues_[index_old_order];

			while (!already_visited[index_new_order])
			{
				already_visited[index_new_order] = true;

				std::swap(right_eigenvalues_ [index_new_order],  current_eigenvalue);

				temp = current_eigenvector;
				right_eigenvectors_.copyColumnToVector(current_eigenvector, index_new_order+1);
				right_eigenvectors_.copyVectorToColumn(temp, index_new_order+1);

				index_new_order = new_index_of_old_vector[index_new_order];
			}

			while (already_visited[++cycle_index]) ;
		}
	}

	// TODO: better *test* for sortedness
	template <>
	void EigenSolver<float, SymmetricTraits>::sort()
	{
		// lapack already sorts eigenvectors for symmetric matrices
		return;
	}
	
	// TODO: better *test* for sortedness
	template <>
	void EigenSolver<double, SymmetricTraits>::sort()
	{
		// lapack already sorts eigenvectors for symmetric matrices
		return;
	}

	template <>
	bool EigenSolver<float, SymmetricTraits>::EigenValueComparator_::operator ()
		(const std::pair<float, Index>& a,
		 const std::pair<float, Index>& b) const
	{
		return a < b;
	}

	template <>
	bool EigenSolver<double, SymmetricTraits>::EigenValueComparator_::operator ()
		(const std::pair<double, Index>& a,
		 const std::pair<double, Index>& b) const
	{
		return a < b;
	}

template class EigenSolver<float>;
template class EigenSolver<double>;
template class EigenSolver<ComplexFloat>;
template class EigenSolver<ComplexDouble>;
template class EigenSolver<float, SymmetricTraits>;

}

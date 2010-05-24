// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/CONCEPT/classTest.h>
#include <BALLTestConfig.h>

///////////////////////////
#include <BALL/MATHS/LINALG/SVDSolver.h>
#include <BALL/SYSTEM/file.h>
///////////////////////////

START_TEST(SVDSolver, "")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

SVDSolver<float>* svd_f_ptr;

CHECK(SVDSolver<float>())
	svd_f_ptr = new SVDSolver<float>();
	TEST_NOT_EQUAL(svd_f_ptr, 0);

	TEST_EXCEPTION(Exception::Uninitialized, svd_f_ptr->computeSVD());
RESULT

CHECK(SVDSolver<float>::~SVDSolver())
	delete svd_f_ptr;
RESULT


CHECK(bool SVDSolver<float>::testComputeSVD())
	Matrix<float> A(7,4);
	File infile(BALL_TEST_DATA_PATH(svd_input_real.dat), std::ios::in);
	infile >> A;

	Matrix<float> real_U(7,7);
	infile.close();
	infile.open(BALL_TEST_DATA_PATH(svd_results_real_U.dat), std::ios::in);
	infile >> real_U;

	Matrix<float> real_Vt(4,4);
	infile.close();
	infile.open(BALL_TEST_DATA_PATH(svd_results_real_Vt.dat), std::ios::in);
	infile >> real_Vt;

	Vector<float> real_S(4);
	infile.close();
	infile.open(BALL_TEST_DATA_PATH(svd_results_real_S.dat), std::ios::in);
	infile >> real_S;

	Matrix<float> U, Vt;
	Vector<float> S;

	SVDSolver<float> solver;

	TEST_EQUAL(solver.setup(A), true)
	TEST_EQUAL(solver.computeSVD(), true)

	U  = solver.getLeftSingularVectors();
	Vt = solver.getRightSingularVectors();
	S  = solver.getSingularValues(); 

	// and test the values and vectors
	// NOTE: since we can not be sure about the distribution of signs in the solution,
	//       we will now only test the absolute values and later test whether the decomposition
	//			 is correct
	PRECISION(1e-6)
	for (size_t i=1; i<=U.getRowCount(); i++)
	{
		for (size_t j=1; j<=U.getColumnCount(); j++)
		{
			TEST_REAL_EQUAL(fabs(U(i,j)), fabs(real_U(i,j)))
		}
	}

	for (size_t i=1; i<=Vt.getRowCount(); i++)
	{
		for (size_t j=1; j<=Vt.getColumnCount(); j++)
		{
			TEST_REAL_EQUAL(fabs(Vt(i,j)), fabs(real_Vt(i,j)));
		}
	}

	for (size_t i=1; i<=(size_t)S.getSize(); i++)
	{
		TEST_REAL_EQUAL(S(i), real_S(i));
	}

	Matrix<float> Sigma(A.getRowCount(), A.getColumnCount());
	for (size_t i=1; i<=(size_t)A.getRowCount(); i++)
	{
		for (size_t j=1; j<=(size_t)A.getColumnCount(); j++)
		{
			Sigma(i,j) = (i==j) ? S(i) : 0.;
		}
	}

	Matrix<float> A_reconstructed = U*Sigma*Vt;

	for (size_t i=1; i<=(size_t)A.getRowCount(); i++)
	{
		for (size_t j=1; j<=(size_t)A.getColumnCount(); j++)
		{
			TEST_REAL_EQUAL(A(i,j), A_reconstructed(i,j));
		}
	}

	// repeat the stuff for column major
	A.transpose(true);
	A.transpose(false);

	TEST_EQUAL(solver.setup(A), true)
	TEST_EQUAL(solver.computeSVD(), true)

	U  = solver.getLeftSingularVectors();
	Vt = solver.getRightSingularVectors();
	S  = solver.getSingularValues(); 

	// and test the values and vectors
	// NOTE: since we can not be sure about the distribution of signs in the solution,
	//       we will now only test the absolute values and later test whether the decomposition
	//			 is correct
	PRECISION(1e-6)
	for (size_t i=1; i<=U.getRowCount(); i++)
	{
		for (size_t j=1; j<=U.getColumnCount(); j++)
		{
			TEST_REAL_EQUAL(fabs(U(i,j)), fabs(real_U(i,j)))
		}
	}

	for (size_t i=1; i<=Vt.getRowCount(); i++)
	{
		for (size_t j=1; j<=Vt.getColumnCount(); j++)
		{
			TEST_REAL_EQUAL(fabs(Vt(i,j)), fabs(real_Vt(i,j)));
		}
	}

	for (size_t i=1; i<=(size_t)S.getSize(); i++)
	{
		TEST_REAL_EQUAL(S(i), real_S(i));
	}

	for (size_t i=1; i<=(size_t)A.getRowCount(); i++)
	{
		for (size_t j=1; j<=(size_t)A.getColumnCount(); j++)
		{
			Sigma(i,j) = (i==j) ? S(i) : 0.;
		}
	}

	A_reconstructed = U*Sigma*Vt;

	for (size_t i=1; i<=(size_t)A.getRowCount(); i++)
	{
		for (size_t j=1; j<=(size_t)A.getColumnCount(); j++)
		{
			TEST_REAL_EQUAL(A(i,j), A_reconstructed(i,j));
		}
	}
RESULT


SVDSolver<double>* svd_d_ptr;

CHECK(SVDSolver<double>())
	svd_d_ptr = new SVDSolver<double>();
	TEST_NOT_EQUAL(svd_d_ptr, 0);

	TEST_EXCEPTION(Exception::Uninitialized, svd_d_ptr->computeSVD());
RESULT

CHECK(SVDSolver<double>::~SVDSolver())
	delete svd_d_ptr;
RESULT

CHECK(bool SVDSolver<double>::testComputeSVD())
	Matrix<double> A(7,4);
	File infile(BALL_TEST_DATA_PATH(svd_input_real.dat), std::ios::in);
	infile >> A;

	Matrix<double> real_U(7,7);
	infile.close();
	infile.open(BALL_TEST_DATA_PATH(svd_results_real_U.dat), std::ios::in);
	infile >> real_U;

	Matrix<double> real_Vt(4,4);
	infile.close();
	infile.open(BALL_TEST_DATA_PATH(svd_results_real_Vt.dat), std::ios::in);
	infile >> real_Vt;

	Vector<double> real_S(4);
	infile.close();
	infile.open(BALL_TEST_DATA_PATH(svd_results_real_S.dat), std::ios::in);
	infile >> real_S;

	Matrix<double> U, Vt;
	Vector<double> S;

	SVDSolver<double> solver;

	TEST_EQUAL(solver.setup(A), true)
	TEST_EQUAL(solver.computeSVD(), true)

	U  = solver.getLeftSingularVectors();
	Vt = solver.getRightSingularVectors();
	S  = solver.getSingularValues(); 

	// and test the values and vectors
	// NOTE: since we can not be sure about the distribution of signs in the solution,
	//       we will now only test the absolute values and later test whether the decomposition
	//			 is correct
	PRECISION(1e-6)
	for (size_t i=1; i<=U.getRowCount(); i++)
	{
		for (size_t j=1; j<=U.getColumnCount(); j++)
		{
			TEST_REAL_EQUAL(fabs(U(i,j)), fabs(real_U(i,j)))
		}
	}

	for (size_t i=1; i<=Vt.getRowCount(); i++)
	{
		for (size_t j=1; j<=Vt.getColumnCount(); j++)
		{
			TEST_REAL_EQUAL(fabs(Vt(i,j)), fabs(real_Vt(i,j)));
		}
	}

	for (size_t i=1; i<=(size_t)S.getSize(); i++)
	{
		TEST_REAL_EQUAL(S(i), real_S(i));
	}

	Matrix<double> Sigma(A.getRowCount(), A.getColumnCount());
	for (size_t i=1; i<=(size_t)A.getRowCount(); i++)
	{
		for (size_t j=1; j<=(size_t)A.getColumnCount(); j++)
		{
			Sigma(i,j) = (i==j) ? S(i) : 0.;
		}
	}

	Matrix<double> A_reconstructed = U*Sigma*Vt;

	for (size_t i=1; i<=(size_t)A.getRowCount(); i++)
	{
		for (size_t j=1; j<=(size_t)A.getColumnCount(); j++)
		{
			TEST_REAL_EQUAL(A(i,j), A_reconstructed(i,j));
		}
	}

	// repeat the stuff for column major
	A.transpose(true);
	A.transpose(false);

	TEST_EQUAL(solver.setup(A), true)
	TEST_EQUAL(solver.computeSVD(), true)

	U  = solver.getLeftSingularVectors();
	Vt = solver.getRightSingularVectors();
	S  = solver.getSingularValues(); 

	// and test the values and vectors
	// NOTE: since we can not be sure about the distribution of signs in the solution,
	//       we will now only test the absolute values and later test whether the decomposition
	//			 is correct
	PRECISION(1e-6)
	for (size_t i=1; i<=U.getRowCount(); i++)
	{
		for (size_t j=1; j<=U.getColumnCount(); j++)
		{
			TEST_REAL_EQUAL(fabs(U(i,j)), fabs(real_U(i,j)))
		}
	}

	for (size_t i=1; i<=Vt.getRowCount(); i++)
	{
		for (size_t j=1; j<=Vt.getColumnCount(); j++)
		{
			TEST_REAL_EQUAL(fabs(Vt(i,j)), fabs(real_Vt(i,j)));
		}
	}

	for (size_t i=1; i<=(size_t)S.getSize(); i++)
	{
		TEST_REAL_EQUAL(S(i), real_S(i));
	}

	for (size_t i=1; i<=(size_t)A.getRowCount(); i++)
	{
		for (size_t j=1; j<=(size_t)A.getColumnCount(); j++)
		{
			Sigma(i,j) = (i==j) ? S(i) : 0.;
		}
	}

	A_reconstructed = U*Sigma*Vt;

	for (size_t i=1; i<=(size_t)A.getRowCount(); i++)
	{
		for (size_t j=1; j<=(size_t)A.getColumnCount(); j++)
		{
			TEST_REAL_EQUAL(A(i,j), A_reconstructed(i,j));
		}
	}
RESULT

END_TEST

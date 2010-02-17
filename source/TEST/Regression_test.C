#include <BALL/CONCEPT/classTest.h>

#include <BALL/QSAR/QSARData.h>
#include <BALL/QSAR/mlrModel.h>
#include <BALL/QSAR/pcrModel.h>
#include <BALL/QSAR/plsModel.h>

using namespace BALL;
using namespace BALL::QSAR;


START_TEST(Regression, "$Id: Regression_test.C$")

PRECISION(1E-7)

QSARData data;
data.readCSVFile("data/Regression_test.csv",1,1,1,"	",0,0);

CHECK(MLR-model)
	MLRModel mlr(data);
	mlr.readTrainingData();
	mlr.train();
	const Matrix<double>* res = mlr.getTrainingResult();
	TEST_REAL_EQUAL(res->getRowCount(),3)
	TEST_REAL_EQUAL((*res)[0],0.1407737)
	TEST_REAL_EQUAL((*res)[1],0.5902179)
	TEST_REAL_EQUAL((*res)[2],0.4159182)
RESULT


CHECK(PCR-model)
	PCRModel pcr(data);
	pcr.readTrainingData();

	// now just check if SVDSolver works correctly. MLR has already been checked above
	Matrix<double> eigenvectors;
	Matrix<double> XX = pcr.getDescriptorMatrix()->t() * *pcr.getDescriptorMatrix();
	pcr.calculateEigenvectors(XX,1.0,eigenvectors);
	TEST_REAL_EQUAL(eigenvectors.getColumnCount(),3)
	TEST_REAL_EQUAL(eigenvectors.getRowCount(),3)
 	TEST_REAL_EQUAL(eigenvectors(1,1),-0.1998991)
 	TEST_REAL_EQUAL(eigenvectors(2,1),-0.5353127)
 	TEST_REAL_EQUAL(eigenvectors(3,1),-0.8206587)
 	TEST_REAL_EQUAL(eigenvectors(1,2),0.1287921)
 	TEST_REAL_EQUAL(eigenvectors(2,2),0.8159410)
 	TEST_REAL_EQUAL(eigenvectors(3,2),-0.5636070)
 	TEST_REAL_EQUAL(eigenvectors(1,3),0.97131506)
 	TEST_REAL_EQUAL(eigenvectors(2,3),-0.21835886)
 	TEST_REAL_EQUAL(eigenvectors(3,3),-0.09416187)
RESULT


CHECK(PLS-model)
 	PLSModel pls(data);
	pls.readTrainingData();
	pls.train();
	const Matrix<double>* res = pls.getTrainingResult();
	TEST_REAL_EQUAL(res->getRowCount(),3)
	TEST_REAL_EQUAL((*res)[0],0.1407737)
	TEST_REAL_EQUAL((*res)[1],0.5902179)
	TEST_REAL_EQUAL((*res)[2],0.4159182)
RESULT


END_TEST


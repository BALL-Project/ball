#include <BALLTestConfig.h>
#include <BALL/CONCEPT/classTest.h>

#include <BALL/QSAR/QSARData.h>
#include <BALL/QSAR/pcrModel.h>

using namespace BALL;
using namespace BALL::QSAR;


START_TEST(PCR-model)

PRECISION(1E-7)

QSARData data;
data.readCSVFile(BALL_TEST_DATA_PATH(Regression_test.csv),1,1,1,"	",0,0);

CHECK(PCR-model)
	PCRModel pcr(data);
	pcr.readTrainingData();

	// now just check if SVDSolver works correctly. MLR has already been checked separately
	Eigen::MatrixXd eigenvectors;
	Eigen::MatrixXd XX = pcr.getDescriptorMatrix()->transpose() * *pcr.getDescriptorMatrix();
	pcr.calculateEigenvectors(XX,1.0,eigenvectors);
	TEST_EQUAL(eigenvectors.cols(),3)
	TEST_EQUAL(eigenvectors.rows(),3)
	TEST_REAL_EQUAL(eigenvectors(0,0),0.1998991)
	TEST_REAL_EQUAL(eigenvectors(1,0),0.5353127)
	TEST_REAL_EQUAL(eigenvectors(2,0),0.8206587)
	TEST_REAL_EQUAL(eigenvectors(0,1),0.1287921)
	TEST_REAL_EQUAL(eigenvectors(1,1),0.8159410)
	TEST_REAL_EQUAL(eigenvectors(2,1),-0.5636070)
	TEST_REAL_EQUAL(eigenvectors(0,2),0.97131506)
	TEST_REAL_EQUAL(eigenvectors(1,2),-0.21835886)
	TEST_REAL_EQUAL(eigenvectors(2,2),-0.09416187)
RESULT


END_TEST

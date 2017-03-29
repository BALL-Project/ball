#include <BALLTestConfig.h>
#include <BALL/CONCEPT/classTest.h>

#include <BALL/QSAR/QSARData.h>
#include <BALL/QSAR/gpModel.h>

using namespace BALL;
using namespace BALL::QSAR;


START_TEST(GP-model)

PRECISION(1E-2)

QSARData data;
data.readCSVFile(BALL_TEST_DATA_PATH(Regression_test.csv),1,1,1,"	",0,0);

CHECK(GP-model)
	GPModel model(data,2,0.005);
	model.readTrainingData();
	model.train();
	const Eigen::MatrixXd* res = model.getTrainingResult();
	TEST_EQUAL(res->rows(),5)
	//	NOTE: The first and last line are numerically quite unstable ... so we don't test them
	//	      since we would always drown in false positives
//	TEST_REAL_EQUAL((*res)[0],61.60615)
	TEST_REAL_EQUAL((*res)(1,0),307.402)
	TEST_REAL_EQUAL((*res)(2,0),-478.8941)
	TEST_REAL_EQUAL((*res)(3,0),56.16572)
//	TEST_REAL_EQUAL((*res)[4],61.60615)
RESULT


END_TEST

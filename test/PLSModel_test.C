#include <BALLTestConfig.h>
#include <BALL/CONCEPT/classTest.h>

#include <BALL/QSAR/QSARData.h>
#include <BALL/QSAR/plsModel.h>

using namespace BALL;
using namespace BALL::QSAR;


START_TEST(PLS-model)

PRECISION(1E-7)

QSARData data;
data.readCSVFile(BALL_TEST_DATA_PATH(Regression_test.csv),1,1,1,"	",0,0);

CHECK(PLS-model)
	PLSModel pls(data);
	pls.readTrainingData();
	pls.train();
	const Eigen::MatrixXd* res = pls.getTrainingResult();
	TEST_EQUAL(res->rows(),3)
	TEST_REAL_EQUAL((*res)(0,0),0.1407737)
	TEST_REAL_EQUAL((*res)(1,0),0.5902179)
	TEST_REAL_EQUAL((*res)(2,0),0.4159182)
RESULT


END_TEST

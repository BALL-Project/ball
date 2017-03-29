#include <BALLTestConfig.h>
#include <BALL/CONCEPT/classTest.h>

#include <BALL/QSAR/QSARData.h>
#include <BALL/QSAR/kplsModel.h>

using namespace BALL;
using namespace BALL::QSAR;


START_TEST(KPLS-model)

PRECISION(1E-5)

QSARData data;
data.readCSVFile(BALL_TEST_DATA_PATH(Regression_test.csv),1,1,1,"	",0,0);

CHECK(KPLS-model)
	KPLSModel model(data,2,0.005);
	model.readTrainingData();
	model.train();
	const Eigen::MatrixXd* res = model.getTrainingResult();
	TEST_EQUAL(res->rows(),5)
	TEST_REAL_EQUAL((*res)(0,0),1.097792)
	TEST_REAL_EQUAL((*res)(1,0),1.098163)
	TEST_REAL_EQUAL((*res)(2,0),1.099770)
	TEST_REAL_EQUAL((*res)(3,0),1.091865)
	TEST_REAL_EQUAL((*res)(4,0),1.097792)
RESULT


END_TEST

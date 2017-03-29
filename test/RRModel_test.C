#include <BALLTestConfig.h>
#include <BALL/CONCEPT/classTest.h>

#include <BALL/QSAR/QSARData.h>
#include <BALL/QSAR/rrModel.h>


using namespace BALL;
using namespace BALL::QSAR;


START_TEST(RR-model)

PRECISION(1E-7)

QSARData data;
data.readCSVFile(BALL_TEST_DATA_PATH(Regression_test.csv),1,1,1,"	",0,0);

CHECK(RR-model training)
	RRModel model(data);
	model.readTrainingData();
	model.train();
	const Eigen::MatrixXd* res = model.getTrainingResult();
	TEST_EQUAL(res->rows(),3)
	TEST_REAL_EQUAL((*res)(0,0),0.1407803)
	TEST_REAL_EQUAL((*res)(1,0),0.5901905)
	TEST_REAL_EQUAL((*res)(2,0),0.4159267)
RESULT

END_TEST

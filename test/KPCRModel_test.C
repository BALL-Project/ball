#include <BALLTestConfig.h>
#include <BALL/CONCEPT/classTest.h>

#include <BALL/QSAR/QSARData.h>
#include <BALL/QSAR/kpcrModel.h>

using namespace BALL;
using namespace BALL::QSAR;


START_TEST(KPCR-model)

PRECISION(1E-5)

QSARData data;
data.readCSVFile(BALL_TEST_DATA_PATH(Regression_test.csv),1,1,1,"	",0,0);

CHECK(KPCR-model)
	KPCRModel model(data,2,0.005);
	model.readTrainingData();
	model.train();
	const Eigen::MatrixXd* res = model.getTrainingResult();
	TEST_EQUAL(res->rows(),5)
	TEST_REAL_EQUAL((*res)(0,0),-23.62431)
	TEST_REAL_EQUAL((*res)(1,0),14.25058)
	TEST_REAL_EQUAL((*res)(2,0),1.784667)
	TEST_REAL_EQUAL((*res)(3,0),37.10245)
	TEST_REAL_EQUAL((*res)(4,0),-23.62431)
RESULT


END_TEST

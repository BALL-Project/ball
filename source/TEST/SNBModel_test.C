#include <BALLTestConfig.h>
#include <BALL/CONCEPT/classTest.h>

#include <BALL/QSAR/QSARData.h>
#include <BALL/QSAR/snBModel.h>

using namespace BALL;
using namespace BALL::QSAR;


START_TEST(SNB-model)

PRECISION(1E-7)

QSARData data;
data.readCSVFile(BALL_TEST_DATA_PATH(Regression_test.csv),1,1,1,"	",0,0);

CHECK(SNB-model)
	SNBModel model(data);
	model.readTrainingData();
	model.train();
	model.validation->testInputData();
	double quality=model.validation->getFitRes();
	TEST_REAL_EQUAL(quality,0.25)
RESULT


END_TEST

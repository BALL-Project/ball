#include <BALLTestConfig.h>
#include <BALL/CONCEPT/classTest.h>

#include <BALL/QSAR/QSARData.h>
#include <BALL/QSAR/allModel.h>

using namespace BALL;
using namespace BALL::QSAR;


START_TEST(ALL-model)

PRECISION(1E-7)

QSARData data;
data.readCSVFile(BALL_TEST_DATA_PATH(Regression_test.csv),1,1,1,"	",0,0);

CHECK(ALL-model)
	ALLModel model(data,3);
	model.readTrainingData();
	model.train();
	model.validation->testInputData();
	double r2=model.validation->getFitRes();
	TEST_REAL_EQUAL(r2,0.4702708)
RESULT


END_TEST

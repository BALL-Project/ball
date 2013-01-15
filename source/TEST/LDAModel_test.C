#include <BALLTestConfig.h>
#include <BALL/CONCEPT/classTest.h>

#include <BALL/QSAR/QSARData.h>
#include <BALL/QSAR/ldaModel.h>

using namespace BALL;
using namespace BALL::QSAR;


START_TEST(LDA-model)

PRECISION(1E-7)


QSARData data;
data.readCSVFile(BALL_TEST_DATA_PATH(Regression_test.csv),1,1,1,"\t",0,0);

QSARData data2;
data2.readCSVFile(BALL_TEST_DATA_PATH(Regression_test2.csv),1,1,1,"\t",0,0);

CHECK(LDA-model)
	LDAModel model(data);
	model.readTrainingData();
	model.train();
	model.validation->testInputData();
	double quality=model.validation->getFitRes();
	TEST_REAL_EQUAL(quality, 1.0)
	TEST_REAL_EQUAL((*model.validation->getClassResults())[0], 1.00)
	TEST_REAL_EQUAL((*model.validation->getClassResults())[1], 1.00)
	TEST_REAL_EQUAL((*model.validation->getClassResults())[2], 1.00)
	TEST_REAL_EQUAL((*model.validation->getClassResults())[3], 1.00)
RESULT

CHECK(LDA-model2)
	LDAModel model(data2);
	model.readTrainingData();
	model.train();
	model.validation->testInputData();
	double quality=model.validation->getFitRes();
	TEST_REAL_EQUAL(quality, 0.75)
	TEST_REAL_EQUAL((*model.validation->getClassResults())[0], 0.75)
	TEST_REAL_EQUAL((*model.validation->getClassResults())[1], 1.00)
	TEST_REAL_EQUAL((*model.validation->getClassResults())[2], 0.50)
RESULT

END_TEST

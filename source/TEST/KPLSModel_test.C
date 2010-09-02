#include <BALLTestConfig.h>
#include <BALL/CONCEPT/classTest.h>

#include <BALL/QSAR/QSARData.h>
#include <BALL/QSAR/kplsModel.h>

using namespace BALL;
using namespace BALL::QSAR;


START_TEST(KPLS-model, "$Id: KPLS_test.C$")

PRECISION(1E-5)

QSARData data;
data.readCSVFile(BALL_TEST_DATA_PATH(Regression_test.csv),1,1,1,"	",0,0);

CHECK(KPLS-model)
	KPLSModel model(data,2,0.005);
	model.readTrainingData();
	model.train();
	const Matrix<double>* res = model.getTrainingResult();
	TEST_REAL_EQUAL(res->getRowCount(),5)
	TEST_REAL_EQUAL((*res)[0],1.097792)
	TEST_REAL_EQUAL((*res)[1],1.098163)
	TEST_REAL_EQUAL((*res)[2],1.099770)
	TEST_REAL_EQUAL((*res)[3],1.091865)
	TEST_REAL_EQUAL((*res)[4],1.097792)
RESULT


END_TEST

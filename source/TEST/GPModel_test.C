#include <BALL/CONCEPT/classTest.h>

#include <BALL/QSAR/QSARData.h>
#include <BALL/QSAR/gpModel.h>

using namespace BALL;
using namespace BALL::QSAR;


START_TEST(GP-model, "$Id: GP_test.C$")

PRECISION(1E-3)

QSARData data;
data.readCSVFile("data/Regression_test.csv",1,1,1,"	",0,0);

CHECK(GP-model)
	GPModel model(data,2,0.005);
	model.readTrainingData();
	model.train();
	const Matrix<double>* res = model.getTrainingResult();
	TEST_REAL_EQUAL(res->getRowCount(),5)
	TEST_REAL_EQUAL((*res)[0],65.3125)
	TEST_REAL_EQUAL((*res)[1],307.402)
	TEST_REAL_EQUAL((*res)[2],-478.8941)
	TEST_REAL_EQUAL((*res)[3],56.16572)
	TEST_REAL_EQUAL((*res)[4],57.875)
RESULT


END_TEST

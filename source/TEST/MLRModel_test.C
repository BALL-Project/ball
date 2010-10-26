#include <BALLTestConfig.h>
#include <BALL/CONCEPT/classTest.h>

#include <BALL/QSAR/QSARData.h>
#include <BALL/QSAR/mlrModel.h>


using namespace BALL;
using namespace BALL::QSAR;


START_TEST(MLR-model, "$Id: MLR_test.C$")

PRECISION(1E-7)

QSARData data;
data.readCSVFile(BALL_TEST_DATA_PATH(Regression_test.csv),1,1,1,"	",0,0);

CHECK(MLR-model)
	MLRModel mlr(data);
	mlr.readTrainingData();
	mlr.train();
	const Matrix<double>* res = mlr.getTrainingResult();
	TEST_EQUAL(res->getRowCount(),3)
	TEST_REAL_EQUAL((*res)[0],0.1407737)
	TEST_REAL_EQUAL((*res)[1],0.5902179)
	TEST_REAL_EQUAL((*res)[2],0.4159182)
RESULT

END_TEST

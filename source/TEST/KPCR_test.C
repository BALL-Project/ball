#include <BALL/CONCEPT/classTest.h>

#include <BALL/QSAR/QSARData.h>
#include <BALL/QSAR/kpcrModel.h>

using namespace BALL;
using namespace BALL::QSAR;


START_TEST(KPCR-model, "$Id: KPCR_test.C$")

PRECISION(1E-5)

QSARData data;
data.readCSVFile("data/Regression_test.csv",1,1,1,"	",0,0);

CHECK(KPCR-model)
	KPCRModel model(data,2,0.005);
	model.readTrainingData();
	model.train();
	const Matrix<double>* res = model.getTrainingResult();
	TEST_REAL_EQUAL(res->getRowCount(),5)
	TEST_REAL_EQUAL((*res)[0],-23.62431)
	TEST_REAL_EQUAL((*res)[1],14.25058)
	TEST_REAL_EQUAL((*res)[2],1.784667)
	TEST_REAL_EQUAL((*res)[3],37.10245)
	TEST_REAL_EQUAL((*res)[4],-23.62431)
RESULT


END_TEST


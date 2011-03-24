#include <BALL/CONCEPT/classTest.h>

#include <BALL/QSAR/QSARData.h>
#include <BALL/QSAR/oplsModel.h>

using namespace BALL;
using namespace BALL::QSAR;


START_TEST(OPLS-model)

PRECISION(1E-7)

QSARData data;
data.readCSVFile("data/Regression_test.csv",1,1,1,"	",0,0);

CHECK(PLS-model)
 	OPLSModel model(data);
	vector<double> p;
	p.push_back(1);p.push_back(1);
	model.setParameters(p);
	model.readTrainingData();
	model.train();
	const Eigen::MatrixXd* res = model.getTrainingResult();
	TEST_REAL_EQUAL(res->rows(),3)
	TEST_REAL_EQUAL((*res)[0],0.141237)
	TEST_REAL_EQUAL((*res)[1],0.3923251)
	TEST_REAL_EQUAL((*res)[2],0.5555323)
RESULT


END_TEST


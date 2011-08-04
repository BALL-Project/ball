#include <BALLTestConfig.h>
#include <BALL/CONCEPT/classTest.h>

#include <BALL/QSAR/QSARData.h>
#include <BALL/QSAR/pcrModel.h>
#include <BALL/QSAR/featureSelection.h>

using namespace BALL;
using namespace BALL::QSAR;


START_TEST(Regression)

PRECISION(1E-5)

QSARData data;
std::multiset<int> activities;
activities.insert(0);
data.readSDFile(BALL_TEST_DATA_PATH(QSAR_test.sdf),activities,0,0);
data.centerData(true);
PCRModel pcr(data,0.95);
double cutoff = 0.001;


CHECK(RemoveColinearFeatures)
	FeatureSelection fs(pcr);
	fs.setQualityIncreaseCutoff(cutoff);
	double cor_threshold=0.95;
	fs.removeHighlyCorrelatedFeatures(cor_threshold);
	TEST_EQUAL(pcr.getDescriptorIDs()->size(),26);
	pcr.validation->crossValidation(5);
	double q2 = pcr.validation->getQ2();
	TEST_REAL_EQUAL(q2,0.653379);
	pcr.readTrainingData();
	pcr.train();
RESULT

CHECK(BackwardSelection)
	PCRModel pcr1(data);
	pcr1 = pcr; // copy model
	FeatureSelection fs(pcr1);
	fs.setQualityIncreaseCutoff(cutoff);
	fs.backwardSelection(5);
	TEST_EQUAL(pcr1.getDescriptorIDs()->size(),20);
	pcr1.validation->crossValidation(5);
	double q2 = pcr1.validation->getQ2();
	TEST_REAL_EQUAL(q2,0.718207);
RESULT

CHECK(StepwiseSelection)
	PCRModel pcr1(data,0.95);
	pcr1 = pcr; // copy model
	FeatureSelection fs(pcr1);
	fs.setQualityIncreaseCutoff(cutoff);
	fs.stepwiseSelection(5);
	TEST_EQUAL(pcr1.getDescriptorIDs()->size(),5);
	pcr1.validation->crossValidation(5);
	double q2 = pcr1.validation->getQ2();
	TEST_REAL_EQUAL(q2,0.69725);
RESULT


CHECK(ForwardSelection)
	PCRModel pcr1(data,0.95);
	pcr1 = pcr; // copy model
	FeatureSelection fs(pcr1);
	fs.setQualityIncreaseCutoff(cutoff);
	fs.forwardSelection(5);
	TEST_EQUAL(pcr1.getDescriptorIDs()->size(),5);
	pcr1.validation->crossValidation(5);
	double q2 = pcr1.validation->getQ2();
	TEST_REAL_EQUAL(q2,0.69725);
RESULT


CHECK(RemoveLowResponseCorrelation)
	PCRModel pcr1(data,0.95);
	pcr1 = pcr; // copy model
	FeatureSelection fs(pcr1);
	fs.setQualityIncreaseCutoff(cutoff);
	double cor_threshold=0.15;
	fs.removeLowResponseCorrelation(cor_threshold);
	TEST_EQUAL(pcr1.getDescriptorIDs()->size(),25);
	pcr1.validation->crossValidation(5);
	double q2 = pcr1.validation->getQ2();
	TEST_REAL_EQUAL(q2,0.635891);
RESULT

CHECK(TwinScan)
	PCRModel pcr1(data,0.95);
	pcr1 = pcr; // copy model
	FeatureSelection fs(pcr1);
	fs.setQualityIncreaseCutoff(cutoff);
	fs.twinScan(5);
	TEST_EQUAL(pcr1.getDescriptorIDs()->size(), 10);
	pcr1.validation->crossValidation(5);
	double q2 = pcr1.validation->getQ2();
	TEST_REAL_EQUAL(q2, 0.672368);
RESULT


END_TEST


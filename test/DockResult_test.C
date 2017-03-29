#include <BALL/CONCEPT/classTest.h>

#include <BALL/DOCKING/COMMON/dockResult.h>
///////////////////////////

START_TEST(DockResult)

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

DockResult* dr;
CHECK(DockResult() throw())
	dr = new DockResult;
	TEST_NOT_EQUAL(dr, 0)
	TEST_EQUAL(dr->getConformationSet(), 0)
	TEST_EQUAL(dr->isSortedBy(), 0)
	TEST_EQUAL(dr->getDockingAlgorithm(), "")
RESULT

CHECK(DockResult(const String& docking_algorithm,ConformationSet* conformation_set,const Options& docking_options) throw())
	ConformationSet *cs = new ConformationSet;
	Options opt;
	DockResult dr2("alg_name",cs,opt);
	TEST_NOT_EQUAL(dr2.getConformationSet(), 0)
	TEST_EQUAL(dr2.getDockingAlgorithm(), "alg_name")
	//TEST_EQUAL(dr2.getDockingOptions(), opt)
	TEST_EQUAL(dr2.isSortedBy(), 0)
RESULT

Options s_opt;
CHECK(addScoring(const String& name, const Options& options, vector<ConformationSet::Conformation> scores) throw())
	vector<ConformationSet::Conformation> conf;
	conf.push_back(ConformationSet::Conformation(0,2.3));
	conf.push_back(ConformationSet::Conformation(1,3.5));
	conf.push_back(ConformationSet::Conformation(2,1.2));
	conf.push_back(ConformationSet::Conformation(3,5.7));
	conf.push_back(ConformationSet::Conformation(4,4.0));
	dr->addScoring("scoring_name", s_opt,conf);
	vector<ConformationSet::Conformation> conf_check = dr->getScores(0);
	TEST_EQUAL(conf_check.size(),5)
	TEST_EQUAL(conf_check[0].first, 2)
	TEST_REAL_EQUAL(conf_check[0].second, 1.2)
	TEST_EQUAL(conf_check[1].first, 0)
	TEST_REAL_EQUAL(conf_check[1].second, 2.3)
	TEST_EQUAL(conf_check[2].first, 1)
	TEST_REAL_EQUAL(conf_check[2].second, 3.5)
	TEST_EQUAL(conf_check[3].first, 4)
	TEST_REAL_EQUAL(conf_check[3].second, 4.0)
	TEST_EQUAL(conf_check[4].first, 3)
	TEST_REAL_EQUAL(conf_check[4].second, 5.7)
RESULT

CHECK(void sortBy(Index scoring_index))
	vector<ConformationSet::Conformation> conf2;
	conf2.push_back(ConformationSet::Conformation(4,2.1));
	conf2.push_back(ConformationSet::Conformation(3,2.1));
	conf2.push_back(ConformationSet::Conformation(1,1.4));
	conf2.push_back(ConformationSet::Conformation(2,5.7));
	conf2.push_back(ConformationSet::Conformation(0,1.0));
	dr->addScoring("scoring_name2", s_opt,conf2);
	dr->sortBy(1);
	TEST_EQUAL(dr->isSortedBy(),1)
RESULT

CHECK(float operator()(Position i, Position j) throw(Exception::IndexOverflow))
	TEST_REAL_EQUAL((*dr)(0,0),2.3)
	TEST_REAL_EQUAL((*dr)(1,0),3.5)
	TEST_REAL_EQUAL((*dr)(2,0),4.0)
	TEST_REAL_EQUAL((*dr)(3,0),5.7)
	TEST_REAL_EQUAL((*dr)(4,0),1.2)
	TEST_REAL_EQUAL((*dr)(0,1),1.0)
	TEST_REAL_EQUAL((*dr)(1,1),1.4)
	TEST_REAL_EQUAL((*dr)(2,1),2.1)
	TEST_REAL_EQUAL((*dr)(3,1),2.1)
	TEST_REAL_EQUAL((*dr)(4,1),5.7)
	
	dr->sortBy(-1);
	TEST_REAL_EQUAL((*dr)(0,0),2.3)
	TEST_REAL_EQUAL((*dr)(1,0),3.5)
	TEST_REAL_EQUAL((*dr)(2,0),1.2)
	TEST_REAL_EQUAL((*dr)(3,0),5.7)
	TEST_REAL_EQUAL((*dr)(4,0),4.0)
	TEST_REAL_EQUAL((*dr)(0,1),1.0)
	TEST_REAL_EQUAL((*dr)(1,1),1.4)
	TEST_REAL_EQUAL((*dr)(2,1),5.7)
	TEST_REAL_EQUAL((*dr)(3,1),2.1)
	TEST_REAL_EQUAL((*dr)(4,1),2.1)
RESULT

CHECK(~DockResult() throw())
	delete dr;
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

END_TEST

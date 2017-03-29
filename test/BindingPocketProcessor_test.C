#include <BALL/CONCEPT/classTest.h>
#include <BALLTestConfig.h>
#include <BALL/FORMAT/PDBFile.h>
///////////////////////////

#include <BALL/STRUCTURE/bindingPocketProcessor.h>
#include <vector>

///////////////////////////

START_TEST(BindingPocketProcessor)

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

PRECISION(0.1)

BindingPocketProcessor* bpp = 0;
CHECK(BindingPocketProcessor())
  bpp = new BindingPocketProcessor;
TEST_NOT_EQUAL(bpp, 0)
RESULT								

CHECK(~BindingPocketProcessor())
  delete bpp;
bpp = 0;
RESULT

CHECK(getActiveSitePoints())
	System sys;
	PDBFile pdbf(BALL_TEST_DATA_PATH(HSD1.pdb));
	pdbf >> sys;

	BindingPocketProcessor bp;
	sys.apply(bp);
	vector<std::pair<Vector3,double> > v = bp.getActiveSitePoints();
  TEST_EQUAL(v.size(),5);
  TEST_REAL_EQUAL(v[4].first.getDistance(Vector3(56.9969,7.84631,45.0952)),0);
  TEST_REAL_EQUAL(fabs(v[4].second - 1831.21),0)
RESULT

CHECK(getLayers())
	System sys;
	PDBFile pdbf(BALL_TEST_DATA_PATH(HSD1.pdb));
	pdbf >> sys;

	BindingPocketProcessor bp;
	sys.apply(bp);

  vector<vector<std::pair<Vector3,double> > > v2  = bp.getLayers();
  TEST_EQUAL(v2.size(),9); 
  TEST_EQUAL(v2[7].size(),1);
  TEST_REAL_EQUAL(v2[7][0].first.getDistance(Vector3(38.5656,-9.65292,34.183)),0);
  TEST_EQUAL(v2[7][0].second,60);
RESULT   


/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST


// $Id: CreateSpectrumProcessor_test.C,v 1.2 2000/09/22 11:15:11 amoll Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////

#include <BALL/NMR/createSpectrumProcessor.h>
#include <BALL/FORMAT/HINFile.h>
#include <BALL/FORMAT/PDBFile.h>
#include <BALL/STRUCTURE/defaultProcessors.h>

///////////////////////////

START_TEST(CreateSpectrumProcessor, "$Id: CreateSpectrumProcessor_test.C,v 1.2 2000/09/22 11:15:11 amoll Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

CreateSpectrumProcessor* sp = 0;
CHECK(CreateSpectrumProcessor::CreateSpectrumProcessor() throw())
	sp = new CreateSpectrumProcessor;
	TEST_NOT_EQUAL(sp, 0)
RESULT

CHECK(CreateSpectrumProcessor::~CreateSpectrumProcessor() throw())
  delete sp;
RESULT

CHECK(CreateSpectrumProcessor::start() throw())
  //BAUSTELLE
RESULT

CHECK(CreateSpectrumProcessor::operator () (Atom& atom) throw())
  //BAUSTELLE
RESULT

CHECK(CreateSpectrumProcessor::finish() throw())
  //BAUSTELLE
RESULT

HINFile f("data/CreateSpectrumProcessor_test.hin");
System S;
f >> S;
ClearChargeProcessor clear_charge;
S.apply(clear_charge);

CHECK(list<Peak1D>& getPeakList())
//BAUSTELLE
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST

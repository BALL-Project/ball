// $Id: CreateSpectrumProcessor_test.C,v 1.4 2000/09/27 12:01:03 oliver Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////

#include <BALL/NMR/createSpectrumProcessor.h>
#include <BALL/FORMAT/HINFile.h>
#include <BALL/FORMAT/PDBFile.h>
#include <BALL/NMR/shiftModel.h>
#include <BALL/STRUCTURE/defaultProcessors.h>

///////////////////////////

START_TEST(CreateSpectrumProcessor, "$Id: CreateSpectrumProcessor_test.C,v 1.4 2000/09/27 12:01:03 oliver Exp $")

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
ShiftModel sm("data/ShiftModel_test.ini");
S.apply(sm);

CHECK(list<Peak1D>& getPeakList())
	CreateSpectrumProcessor csp;
	TEST_EQUAL(S.apply(csp), true)
	TEST_EQUAL(csp.getPeakList().size(), 15)
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST

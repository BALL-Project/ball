// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/CONCEPT/classTest.h>

///////////////////////////
#include <BALL/FORMAT/HINFile.h>
#include <BALL/FORMAT/PDBFile.h>
#include <BALL/STRUCTURE/fragmentDB.h>
#include <BALL/STRUCTURE/residueChecker.h>
#include <BALL/NMR/spectrum.h>
#include <BALL/NMR/peak.h>
#include <BALL/KERNEL/system.h>
///////////////////////////

START_TEST(Spectrum1D)

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;
using namespace std;

Spectrum1D* p;

CHECK(Spectrum1D::Spectrum1D())
	p = new Spectrum1D;
	TEST_NOT_EQUAL(p, 0);
RESULT

CHECK(Spectrum1D::~Spectrum1D())
	delete p;
RESULT


/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST

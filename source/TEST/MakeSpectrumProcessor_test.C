// $Id: MakeSpectrumProcessor_test.C,v 1.2 2000/09/22 11:15:11 amoll Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////

#include <BALL/NMR/makeSpectrumProcessor.h>
#include <BALL/FORMAT/HINFile.h>
#include <BALL/FORMAT/PDBFile.h>
#include <BALL/STRUCTURE/defaultProcessors.h>
#include <BALL/KERNEL/system.h>

///////////////////////////

START_TEST(MakeSpectrumProcessor, "$Id: MakeSpectrumProcessor_test.C,v 1.2 2000/09/22 11:15:11 amoll Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

MakeSpectrumProcessor* sp = 0;
CHECK(MakeSpectrumProcessor::MakeSpectrumProcessor() throw())
	sp = new MakeSpectrumProcessor;
	TEST_NOT_EQUAL(sp, 0)
RESULT

CHECK(MakeSpectrumProcessor::~MakeSpectrumProcessor() throw())
  delete sp;
RESULT

CHECK(MakeSpectrumProcessor::Processor::Result operator () (Composite& composite) throw())
  //BAUSTELLE
RESULT

CHECK(MakeSpectrumProcessor::finish() throw())
  //BAUSTELLE
RESULT

HINFile f("data/MakeSpectrumProcessor_test.hin");
/*
System S;
f >> S;
ClearChargeProcessor clear_charge;
S.apply(clear_charge);
*/
CHECK(chemical shifts)
	PRECISION(0.0001)
	MakeSpectrumProcessor sp;
/*
	TEST_EQUAL(S.countAtoms(), 31)
	if (S.countAtoms() == 31)
	{
		S.apply(sp);

		AtomIterator atom_it = S.beginAtom();
		Position i = 0;
		for (; +atom_it; ++atom_it)
		{
			if (atom_it->hasProperty("chemical_shift"))
			{
				float shift = atom_it->getProperty("chemical_shift").getFloat();
				STATUS("shift of " << atom_it->getFullName() << ": " << shift)
				switch (i++)
				{//BAUSTELLE
					case  0: TEST_REAL_EQUAL(shift, -0.02835) break;
				}
			}
		}
	}	
	*/
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST

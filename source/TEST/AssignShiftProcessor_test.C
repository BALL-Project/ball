// $Id: AssignShiftProcessor_test.C,v 1.10 2001/07/15 20:24:47 oliver Exp $

#include <BALL/CONCEPT/classTest.h>

#include <BALL/NMR/assignShiftProcessor.h>
#include <BALL/NMR/createSpectrumProcessor.h>
#include <BALL/NMR/spectrum.h>
#include <BALL/STRUCTURE/fragmentDB.h>
#include <BALL/STRUCTURE/residueChecker.h>
#include <BALL/FORMAT/HINFile.h>
#include <BALL/FORMAT/NMRStarFile.h>
#include <BALL/SYSTEM/path.h>

START_TEST(AssignShiftProcessor, "$Id: AssignShiftProcessor_test.C,v 1.10 2001/07/15 20:24:47 oliver Exp $")

using namespace BALL;
using namespace std;

System s;
//AssignShiftProcessor asp;

CHECK(preparations)
	STATUS("opening HIN file")
	HINFile f;
	f.open("data/AssignShiftProcessor_test.hin");
	STATUS("reading structure")
	f >> s;
	
	STATUS("opening fragment DB")
	FragmentDB frag_db;

	STATUS("constructing residue checker")
	ResidueChecker rc(frag_db);
	STATUS("checking structure")
	s.apply(rc);
RESULT

CHECK(AssignShiftProcessor::AssignShiftProcessor(const vector<NMRAtomData*>& atom_data)/apply())
	NMRStarFile rs("data/bmr4789.str");
	TEST_EQUAL(rs.getData()[0]->atom_data.size(), 797)
	TEST_EQUAL(rs.getNumberOfAtoms(), 797)
	TEST_EQUAL(s.countAtoms(), 1944)

	AssignShiftProcessor asp(rs.getData()[0]->atom_data);
	s.apply(asp);
RESULT

CHECK(results)
	int number_of_shiftatoms = 0;
	AtomIterator atom_it = s.beginAtom();
	for (; +atom_it; ++atom_it)
	{
		if (atom_it->hasProperty(ShiftModule::PROPERTY__SHIFT))
		{
			number_of_shiftatoms++;
		}
		else
		{
			Log.info() << atom_it->getFullName() << " has no shift property." << endl;
		}
	}
	TEST_EQUAL(number_of_shiftatoms, 940)

	CreateSpectrumProcessor csp;
	s.apply(csp);
	const PeakList1D& peak_list = csp.getPeakList();

	TEST_EQUAL(peak_list.size(), 581)

	// Spectrum1D spectrum(peak_list);
	// spectrum.plotSpectrum("parv_synth.dat");
	// spectrum.writePeaks("parv_synth.peaks");
RESULT

END_TEST

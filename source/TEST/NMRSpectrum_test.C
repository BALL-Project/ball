// $Id: NMRSpectrum_test.C,v 1.5 2001/05/06 23:11:35 oliver Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////
#include <BALL/FORMAT/HINFile.h>
#include <BALL/FORMAT/PDBFile.h>
#include <BALL/STRUCTURE/fragmentDB.h>
#include <BALL/STRUCTURE/residueChecker.h>
#include <BALL/NMR/NMRSpectrum.h>
#include <BALL/NMR/peak.h>
#include <BALL/KERNEL/system.h>
///////////////////////////

START_TEST(NMRSpectrum, "$Id: NMRSpectrum_test.C,v 1.5 2001/05/06 23:11:35 oliver Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;
using namespace std;

NMRSpectrum* p;

CHECK(NMRSpectrum::NMRSpectrum())
	p = new NMRSpectrum;
	TEST_NOT_EQUAL(p, 0);
RESULT

CHECK(NMRSpectrum::~NMRSpectrum())
	delete p;
RESULT

CHECK(NMRSpectrum::setSystem(System* s)/getSystem())	
	NMRSpectrum spectrum;
	System system;
	TEST_EQUAL(spectrum.getSystem(), 0)
	spectrum.setSystem(&system);
	TEST_EQUAL(spectrum.getSystem(), &system)
RESULT

CHECK(NMRSpectrum::setDensity(Size density)/getDensity())
	NMRSpectrum spectrum;
	TEST_EQUAL(spectrum.getDensity(), 100)
	spectrum.setDensity(50);
	TEST_EQUAL(spectrum.getDensity(), 50)
	spectrum.setDensity(100);
RESULT

ShiftModel sm("data/ShiftModel_test.ini");
CHECK(NMRSpectrum::setShiftModel(const ShiftModel& model))
	NMRSpectrum spectrum;
	spectrum.setShiftModel(sm);
RESULT


CHECK(NMRSpectrum::getShiftModel() const)
	NMRSpectrum spectrum;
	spectrum.setShiftModel(sm);
	// TEST_EQUAL(spectrum.getShiftModel(), sm)
RESULT

HINFile f("data/NMRSpectrum_test.hin");
System system;
f >> system;

CHECK(NMRSpectrum::calculateShifts())
	NMRSpectrum spectrum;
	spectrum.setShiftModel(sm);
	spectrum.setSystem(&system);
  spectrum.calculateShifts();
RESULT

CHECK(NMRSpectrum::createSpectrum())
	NMRSpectrum spectrum;
	spectrum.setSystem(&system);
	spectrum.setShiftModel(sm);
	spectrum.calculateShifts();
  spectrum.createSpectrum();
	TEST_EQUAL(spectrum.getPeakList().size(), 873)
RESULT

list<Peak1D> peaks;

CHECK(NMRSpectrum::getPeakList() const)
	NMRSpectrum spectrum;
	peaks = spectrum.getPeakList();
	TEST_EQUAL(peaks.size(), 0)
RESULT

CHECK(NMRSpectrum::getSpectrumMin() const )
	NMRSpectrum spectrum;
	TEST_EQUAL(spectrum.getSpectrumMin(), 0)
RESULT

CHECK(NMRSpectrum::getSpectrumMax() const )
	NMRSpectrum spectrum;
	TEST_EQUAL(spectrum.getSpectrumMax(), 0) // ???
RESULT

CHECK(NMRSpectrum::setPeakList(const list<Peak1D>& spectrum))
	NMRSpectrum spectrum;
	spectrum.setPeakList(peaks);
RESULT

CHECK(NMRSpectrum::sortSpectrum())
	NMRSpectrum spectrum;
	spectrum.sortSpectrum();
RESULT

NMRSpectrum spectrum;
spectrum.setSystem(&system);
spectrum.setShiftModel(sm);
spectrum.calculateShifts();
spectrum.sortSpectrum();
spectrum.createSpectrum();

String filename;
CHECK(NMRSpectrum::plotSpectrum(const String& filename) const )
	NEW_TMP_FILE(filename)
	spectrum.setDensity(32768);
	spectrum.plotSpectrum(filename);
	TEST_FILE(filename.c_str(), "data/NMRSpectrum_test.dat", false)
RESULT

CHECK(NMRSpectrum::plotPeaks(const String& filename) const )
	NEW_TMP_FILE(filename)
	spectrum.plotPeaks(filename); 
	TEST_FILE(filename.c_str(), "data/NMRSpectrum_test/plotPeaks.txt", false)
RESULT

CHECK(NMRSpectrum::writePeaks(const String& filename) const )
	NEW_TMP_FILE(filename)
	spectrum.writePeaks(filename); 
	TEST_FILE(filename.c_str(), "data/NMRSpectrum_test/writePeaks.txt", false)
RESULT

CHECK(NMRSpectrum::makeDifference(const float&, const String&, const String&, const String&))
  //BAUSTELLE
RESULT

CHECK(NMRSpectrum::setDifference(NMRSpectrum*, NMRSpectrum*, String, String))
  //BAUSTELLE
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST

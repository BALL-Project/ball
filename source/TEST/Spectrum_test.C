// $Id: Spectrum_test.C,v 1.2 2001/07/05 18:59:03 oliver Exp $
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

START_TEST(Spectrum1D, "$Id: Spectrum_test.C,v 1.2 2001/07/05 18:59:03 oliver Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;
using namespace std;
/*  BAUSTELLE
Spectrum1D* p;

CHECK(Spectrum1D::Spectrum1D())
	p = new Spectrum1D;
	TEST_NOT_EQUAL(p, 0);
RESULT

CHECK(Spectrum1D::~Spectrum1D())
	delete p;
RESULT

CHECK(Spectrum1D::setSystem(System* s)/getSystem())	
	Spectrum1D spectrum;
	System system;
	TEST_EQUAL(spectrum.getSystem(), 0)
	spectrum.setSystem(&system);
	TEST_EQUAL(spectrum.getSystem(), &system)
RESULT

CHECK(Spectrum1D::setDensity(Size density)/getDensity())
	Spectrum1D spectrum;
	TEST_EQUAL(spectrum.getDensity(), 100)
	spectrum.setDensity(50);
	TEST_EQUAL(spectrum.getDensity(), 50)
	spectrum.setDensity(100);
RESULT

ShiftModel sm("data/Spectrum1D_test.ini");
CHECK(Spectrum1D::setShiftModel(const ShiftModel& model))
	Spectrum1D spectrum;
	spectrum.setShiftModel(sm);
RESULT


CHECK(Spectrum1D::getShiftModel() const)
	Spectrum1D spectrum;
	spectrum.setShiftModel(sm);
	// TEST_EQUAL(spectrum.getShiftModel(), sm)
RESULT

HINFile f("data/Spectrum1D_test.hin");
System system;
f >> system;

CHECK(Spectrum1D::calculateShifts())
	Spectrum1D spectrum;
	spectrum.setShiftModel(sm);
	spectrum.setSystem(&system);
  spectrum.calculateShifts();
RESULT

CHECK(Spectrum1D::createSpectrum())
	Spectrum1D spectrum;
	spectrum.setSystem(&system);
	spectrum.setShiftModel(sm);
	spectrum.calculateShifts();
  spectrum.createSpectrum();
	TEST_EQUAL(spectrum.getPeakList().size(), 873)
RESULT

list<Peak1D> peaks;

CHECK(Spectrum1D::getPeakList() const)
	Spectrum1D spectrum;
	peaks = spectrum.getPeakList();
	TEST_EQUAL(peaks.size(), 0)
RESULT

CHECK(Spectrum1D::getSpectrumMin() const )
	Spectrum1D spectrum;
	TEST_EQUAL(spectrum.getSpectrumMin(), 0)
RESULT

CHECK(Spectrum1D::getSpectrumMax() const )
	Spectrum1D spectrum;
	TEST_EQUAL(spectrum.getSpectrumMax(), 0) // ???
RESULT

CHECK(Spectrum1D::setPeakList(const list<Peak1D>& spectrum))
	Spectrum1D spectrum;
	spectrum.setPeakList(peaks);
RESULT

CHECK(Spectrum1D::sortSpectrum())
	Spectrum1D spectrum;
	spectrum.sortSpectrum();
RESULT

Spectrum1D spectrum;
spectrum.setSystem(&system);
spectrum.setShiftModel(sm);
spectrum.calculateShifts();
spectrum.sortSpectrum();
spectrum.createSpectrum();

String filename;
CHECK(Spectrum1D::plotSpectrum(const String& filename) const )
	NEW_TMP_FILE(filename)
	spectrum.setDensity(32768);
	spectrum.plotSpectrum(filename);
	TEST_FILE(filename.c_str(), "data/Spectrum1D_test.dat", false)
RESULT

CHECK(Spectrum1D::plotPeaks(const String& filename) const )
	NEW_TMP_FILE(filename)
	spectrum.plotPeaks(filename); 
	TEST_FILE(filename.c_str(), "data/Spectrum1D_test/plotPeaks.txt", false)
RESULT

CHECK(Spectrum1D::writePeaks(const String& filename) const )
	NEW_TMP_FILE(filename)
	spectrum.writePeaks(filename); 
	TEST_FILE(filename.c_str(), "data/Spectrum1D_test/writePeaks.txt", false)
RESULT

CHECK(Spectrum1D::makeDifference(const float&, const String&, const String&, const String&))
  //BAUSTELLE
RESULT
*/
CHECK(Spectrum1D::setDifference(Spectrum1D*, Spectrum1D*, String, String))
  //BAUSTELLE
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST

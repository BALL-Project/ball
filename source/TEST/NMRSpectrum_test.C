// $Id: NMRSpectrum_test.C,v 1.3 2000/09/22 14:13:58 amoll Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////
#include <BALL/FORMAT/HINFile.h>
#include <BALL/NMR/NMRSpectrum.h>
#include <BALL/NMR/peak.h>
#include <BALL/KERNEL/system.h>
///////////////////////////

START_TEST(NMRSpectrum, "$Id: NMRSpectrum_test.C,v 1.3 2000/09/22 14:13:58 amoll Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

NMRSpectrum* p;

CHECK(NMRSpectrum::NMRSpectrum())
	p = new NMRSpectrum;
	TEST_NOT_EQUAL(p, 0);
RESULT

CHECK(NMRSpectrum::~NMRSpectrum())
	delete p;
RESULT

NMRSpectrum spectrum;
HINFile f("data/NMRSpectrum_test/NMRSpectrum_test.hin");
System system;
f >> system;

CHECK(NMRSpectrum::setSystem(System* s)/getSystem())
	TEST_EQUAL(spectrum.getSystem(), 0)
	spectrum.setSystem(&system);
	TEST_EQUAL(spectrum.getSystem(), &system)
RESULT

CHECK(NMRSpectrum::setDensity(Size density)/getDensity())
	TEST_EQUAL(spectrum.getDensity(), 100)
	spectrum.setDensity(50);
	TEST_EQUAL(spectrum.getDensity(), 50)
	spectrum.setDensity(100);
RESULT

CHECK(NMRSpectrum::calculateShifts())
  spectrum.calculateShifts();
RESULT

CHECK(NMRSpectrum::createSpectrum())
  spectrum.createSpectrum();
RESULT

list<Peak1D> peaks;

CHECK(NMRSpectrum::getPeakList() const )
	peaks = spectrum.getPeakList();
	TEST_EQUAL(peaks.size(), 15)
	TEST_EQUAL(peaks.front().getValue(), 0)
	TEST_EQUAL(peaks.back().getValue(), 0)
RESULT

CHECK(NMRSpectrum::getSpectrumMin() const )
	TEST_EQUAL(spectrum.getSpectrumMin(), 0)
RESULT

CHECK(NMRSpectrum::getSpectrumMax() const )
	TEST_EQUAL(spectrum.getSpectrumMax(), 0) // ???
RESULT

CHECK(NMRSpectrum::setPeakList(const list<Peak1D>& spectrum))
	NMRSpectrum spectrum2;
	spectrum2.setPeakList(peaks);
RESULT

CHECK(NMRSpectrum::sortSpectrum())
	spectrum.sortSpectrum();
RESULT

String filename;

CHECK(NMRSpectrum::plotSpectrum(const String& filename) const )
// terminiert nicht ???
/*
	NEW_TMP_FILE(filename)
	spectrum.plotSpectrum(filename);
	TEST_FILE(filename.c_str(), "data/NMRSpectrum_test/plotSpectrum.txt", false)*/
RESULT

CHECK(NMRSpectrum::plotPeaks(const String& filename) const )
	NEW_TMP_FILE(filename)
	spectrum.plotPeaks(filename); // ???
	TEST_FILE(filename.c_str(), "data/NMRSpectrum_test/plotPeaks.txt", false)
RESULT

CHECK(NMRSpectrum::writePeaks(const String& filename) const )
	NEW_TMP_FILE(filename)
	spectrum.writePeaks(filename); // ???
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

// $Id: CreateSpectrumProcessor_test.C,v 1.7 2001/12/30 13:28:56 sturm Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////

#include <BALL/NMR/createSpectrumProcessor.h>
#include <BALL/FORMAT/HINFile.h>
#include <BALL/FORMAT/PDBFile.h>
#include <BALL/KERNEL/PTE.h>
#include <BALL/NMR/shiftModel.h>
#include <BALL/STRUCTURE/defaultProcessors.h>

///////////////////////////

START_TEST(CreateSpectrumProcessor, "$Id: CreateSpectrumProcessor_test.C,v 1.7 2001/12/30 13:28:56 sturm Exp $")

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

CHECK(CreateSpectrumProcessor::isValid() throw())
	CreateSpectrumProcessor csp;
	TEST_EQUAL(csp.isValid(), true)
RESULT

System S;
Atom& a1 = *new Atom;
Atom& a2 = *new Atom;
Atom& a3 = *new Atom;
Molecule& m = *new Molecule;
S.insert(m);
m.insert(a1);
m.insert(a2);
m.insert(a3);
a1.setElement(PTE[Element::H]);
a1.setProperty(ShiftModule::PROPERTY__SHIFT, (float)1.0);
a2.setElement(PTE[Element::H]);
a2.setProperty(ShiftModule::PROPERTY__SHIFT, (float)4.0);
a3.setElement(PTE[Element::H]);
a3.setProperty(ShiftModule::PROPERTY__SHIFT, (float)7.0);


CHECK(CreateSpectrumProcessor::getPeakList() const)
	CreateSpectrumProcessor csp;
	TEST_EQUAL(csp.getPeakList().size(), 0)
RESULT

CHECK(CreateSpectrumProcessor::start() throw())
	CreateSpectrumProcessor csp;	
	csp.start();
	TEST_EQUAL(csp.start(), true)
	TEST_EQUAL(csp.isValid(), true)
	TEST_EQUAL(csp.getPeakList().size(), 0)
RESULT

CHECK(CreateSpectrumProcessor::getWidth() const)
	CreateSpectrumProcessor csp;
	TEST_REAL_EQUAL(csp.getWidth(), 1.0)
RESULT

CHECK(CreateSpectrumProcessor::setWidth() const)
	CreateSpectrumProcessor csp;
	TEST_REAL_EQUAL(csp.getWidth(), 1.0)
	csp.setWidth(123.4);
	float width = csp.getWidth();
	PRECISION(0.001)
	TEST_REAL_EQUAL(width, 123.4)			
RESULT

CHECK(CreateSpectrumProcessor::getAtomAveraging() const throw())
	CreateSpectrumProcessor csp;
	TEST_EQUAL(csp.getAtomAveraging(), true)
RESULT

CHECK(CreateSpectrumProcessor::setAtomAveraging(bool flag) throw())
	CreateSpectrumProcessor csp;
	TEST_EQUAL(csp.getAtomAveraging(), true)
	csp.setAtomAveraging(false);
	TEST_EQUAL(csp.getAtomAveraging(), false)
RESULT

CHECK(CreateSpectrumProcessor::getAtomIgnoring() const throw())
	CreateSpectrumProcessor csp;
	TEST_EQUAL(csp.getAtomIgnoring(), true)
RESULT

CHECK(CreateSpectrumProcessor::setAtomIgnoring(bool flag) throw())
	CreateSpectrumProcessor csp;
	TEST_EQUAL(csp.getAtomIgnoring(), true)
	csp.setAtomIgnoring(false);
	TEST_EQUAL(csp.getAtomIgnoring(), false)
RESULT

CHECK(CreateSpectrumProcessor::operator () (Atom& atom) throw())
	CreateSpectrumProcessor csp;
	// ?????
RESULT

CHECK(CreateSpectrumProcessor::init(const String& filename) throw(Exception::FileNotFound))
	CreateSpectrumProcessor csp;
	// ?????
RESULT

CHECK(CreateSpectrumProcessor::finish() throw())
  //?????
RESULT

CHECK(PeakList<Peak1D>& getPeakList())
	CreateSpectrumProcessor csp;
	S.apply(csp);
	TEST_EQUAL(csp.getPeakList().size(), 3)
	if (csp.getPeakList().size() == 3)
	{
		PeakList1D::ConstIterator list_it = csp.getPeakList().begin();
		TEST_EQUAL(list_it->getPosition(), 1.0)
		list_it++;
		TEST_EQUAL(list_it->getPosition(), 4.0)
		list_it++;
		TEST_EQUAL(list_it->getPosition(), 7.0)
	}
RESULT

CHECK(const RegularData1D& operator << (RegularData1D& data, const PeakList1D& peak_list) throw())
	RegularData1D data;
	data.resize(100);
	data.setBoundaries(0.0, 10.0);
	CreateSpectrumProcessor csp;
	S.apply(csp);
	data << csp.getPeakList();
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST

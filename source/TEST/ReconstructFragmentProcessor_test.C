// $Id: ReconstructFragmentProcessor_test.C,v 1.1.2.2 2002/05/12 13:44:27 oliver Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////

#include <BALL/STRUCTURE/reconstructFragmentProcessor.h>

#include <BALL/STRUCTURE/fragmentDB.h>
#include <BALL/STRUCTURE/structureMapper.h>
#include <BALL/KERNEL/PTE.h>
#include <BALL/KERNEL/atom.h>
#include <BALL/KERNEL/residue.h>
#include <BALL/KERNEL/system.h>
#include <BALL/FORMAT/HINFile.h>

///////////////////////////

START_TEST(ReconstructFragmentProcessor, "$Id: ReconstructFragmentProcessor_test.C,v 1.1.2.2 2002/05/12 13:44:27 oliver Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

ReconstructFragmentProcessor* ptr = 0;
CHECK(ReconstructFragmentProcessor::ReconstructFragmentProcessor())
	ptr = new	ReconstructFragmentProcessor;
	TEST_NOT_EQUAL(ptr, 0)
RESULT

CHECK(ReconstructFragmentProcessor::~ReconstructFragmentProcessor())
	delete ptr;
RESULT

CHECK(const FragmentDB* ReconstructFragmentProcessor::getFragmentDB() const)
	ReconstructFragmentProcessor rfp;
	TEST_EQUAL(rfp.getFragmentDB(), 0)
RESULT

FragmentDB frag_db;
CHECK(void ReconstructFragmentProcessor::getFragmentDB(FragmentDB& frag_db))
	ReconstructFragmentProcessor rfp;
	TEST_EQUAL(rfp.getFragmentDB(), 0)
	rfp.setFragmentDB(frag_db);
	TEST_EQUAL(rfp.getFragmentDB(), &frag_db)
RESULT

CHECK(ReconstructFragmentProcessor::ReconstructFragmentProcessor(FragmentDB& frag_db))
	ReconstructFragmentProcessor rfp(frag_db);
	TEST_EQUAL(rfp.getFragmentDB(), &frag_db)
RESULT

CHECK(ReconstructFragmentProcessor::getNumberOfInsertedAtoms() const)
	ReconstructFragmentProcessor rfp(frag_db);
	TEST_EQUAL(rfp.getNumberOfInsertedAtoms(), 0)	
RESULT

CHECK(ReconstructFragmentProcessor::reconstructFragment(Fragment& fragment, const Fragment& tplate))
	Residue res;
	PDBAtom& CA(*new PDBAtom);
	CA.setName("CA");
	CA.setElement(PTE[Element::C]);
	res.insert(CA);
	Residue res2(res);

	const Fragment& ref = *frag_db.getFragment("LYS");

	Size n = ReconstructFragmentProcessor::reconstructFragment(res2, ref);
	TEST_EQUAL(n, ref.countAtoms() - 1)
	TEST_EQUAL(res2.countAtoms(), ref.countAtoms())
RESULT

CHECK(ReconstructFragmentProcessor::operator ())
	ReconstructFragmentProcessor rfp(frag_db);

	// try to fix C and N-termini
	System S;
	HINFile f("data/ReconstructFragmentProcessor_test1.hin");
	f >> S;
	TEST_EQUAL(S.countAtoms(), 27)
	ABORT_IF(S.countAtoms() != 27)

	S.apply(rfp);
	TEST_EQUAL(rfp.getNumberOfInsertedAtoms(), 4)
	TEST_EQUAL(S.countAtoms(), 31)
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST

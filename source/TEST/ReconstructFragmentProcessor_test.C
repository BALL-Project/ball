// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/CONCEPT/classTest.h>
#include <BALLTestConfig.h>

///////////////////////////

#include <BALL/STRUCTURE/reconstructFragmentProcessor.h>

#include <BALL/STRUCTURE/fragmentDB.h>
#include <BALL/STRUCTURE/structureMapper.h>
#include <BALL/STRUCTURE/residueChecker.h>
#include <BALL/KERNEL/PTE.h>
#include <BALL/KERNEL/atom.h>
#include <BALL/KERNEL/residue.h>
#include <BALL/KERNEL/system.h>
#include <BALL/FORMAT/HINFile.h>

///////////////////////////

START_TEST(ReconstructFragmentProcessor)

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

FragmentDB frag_db("fragments/Fragments.db");
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

	list<Atom*> reconstructed_atoms = ReconstructFragmentProcessor::reconstructFragment(res2, ref);
	TEST_EQUAL(reconstructed_atoms.size(), ref.countAtoms() - 1)
	TEST_EQUAL(res2.countAtoms(), ref.countAtoms())
RESULT

CHECK(ReconstructFragmentProcessor::operator ())
	ReconstructFragmentProcessor rfp(frag_db);

	// try to fix C and N-termini
	System S;
	HINFile f(BALL_TEST_DATA_PATH(ReconstructFragmentProcessor_test1.hin));
	f >> S;
	TEST_EQUAL(S.countAtoms(), 27)
	ABORT_IF(S.countAtoms() != 27)

	S.apply(rfp);
	TEST_EQUAL(rfp.getNumberOfInsertedAtoms(), 4)
	TEST_EQUAL(S.countAtoms(), 31)
RESULT

CHECK([EXTRA]Contents of FragmentDB -- proteinogenous amino acids)
	ReconstructFragmentProcessor rfp(frag_db);
	ResidueChecker rc(frag_db);
	rc.disable(ResidueChecker::OVERLAPPING_ATOMS);

	String aa_names[20] = {"ALA", "ARG", "ASN", "ASP", "CYS", 
												 "GLN", "GLU", "GLY", "HIS", "ILE",
												 "LEU", "LYS", "MET", "PHE", "PRO",
												 "SER", "THR", "TRP", "TYR", "VAL"};
	for (Position i = 0; i < 20; i++)
	{
		System S;
		PDBAtom* atom = new PDBAtom;
		atom->setName("CA");
		atom->setElement(PTE[Element::C]);
		Residue* res = new Residue;
		res->setName(aa_names[i]);
		res->insert(*atom);
		Chain* chain = new Chain;
		chain->insert(*res);
		chain->setName("C");
		Protein* protein = new Protein;
		protein->insert(*chain);
		protein->setName("P");
		S.insert(*protein);
		S.apply(frag_db.normalize_names);
		S.apply(frag_db.build_bonds);
		S.apply(rfp);
		S.apply(frag_db.build_bonds);
		S.apply(rc);
		TEST_EQUAL(rc.getStatus(), true)
	}
                                
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST

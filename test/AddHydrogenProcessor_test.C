// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/CONCEPT/classTest.h>
#include <BALLTestConfig.h>

#include <BALL/KERNEL/PTE.h>
#include <BALL/KERNEL/system.h>
#include <BALL/STRUCTURE/addHydrogenProcessor.h>
#include <BALL/STRUCTURE/fragmentDB.h>
#include <BALL/STRUCTURE/peptideBuilder.h>

START_TEST(class_name)

using namespace BALL;

FragmentDB db("");

CHECK(AddHydrogenProcessor::AddHydrogenProcessor())
	AddHydrogenProcessor proc;

	TEST_EQUAL(proc.getNumberOfAddedHydrogens(), 0);
RESULT

CHECK(AddHydrogenProcessor::start())
	AddHydrogenProcessor proc;
	System s;
	Molecule m;
	Atom a(PTE["C"], "C");
	m.insert(a);
	s.insert(m);

	s.apply(proc);

	TEST_EQUAL(proc.getNumberOfAddedHydrogens(), 4);
	TEST_EQUAL(proc.start(), true);
	TEST_EQUAL(proc.getNumberOfAddedHydrogens(), 0);
RESULT

CHECK(AddHydrogenProcessor::operator()(Atom&))
	AddHydrogenProcessor proc;
	Atom a(PTE[Element::C], "C");

	proc(a);

	TEST_EQUAL(a.countBonds(), 4);

	// Proper cleanup
	std::vector<Atom*> partners;
	for(Atom::BondIterator it = a.beginBond(); +it; ++it) {
		partners.push_back(it->getPartner(a));
	}

	for(std::vector<Atom*>::iterator it = partners.begin(); it != partners.end(); ++it) {
		delete *it;
	}

	TEST_EQUAL(proc.getNumberOfAddedHydrogens(), 4);
RESULT

CHECK(AddHydrogenProcessor::operator()(Residue&))
	AddHydrogenProcessor proc;
	// Build a dipeptide
	Peptides::PeptideBuilder builder("GG");
	builder.setFragmentDB(&db);
	Protein* prot = builder.construct();

	// Delete the peptide bond H
	Fragment* res = prot->getChain(0)->getResidue(1);

	Atom* hatom = res->getAtom("H");
	res->remove(*hatom);
	delete hatom;

	proc(*res);

	TEST_EQUAL(proc.getNumberOfAddedHydrogens(), 1);

	delete prot;
RESULT

CHECK(AddHydrogenProcessor::getConnectivity(const Atom&) const)
	AddHydrogenProcessor proc;
	Atom c(PTE[Element::C], "C");
	Atom o(PTE[Element::O], "O");
	Atom n(PTE[Element::N], "N");
	Atom p(PTE[Element::P], "P");
	Atom f(PTE[Element::F], "F");

	TEST_EQUAL(proc.getConnectivity(c), 4);
	TEST_EQUAL(proc.getConnectivity(o), 2);
	TEST_EQUAL(proc.getConnectivity(n), 3);
	TEST_EQUAL(proc.getConnectivity(p), 3);
	TEST_EQUAL(proc.getConnectivity(f), 1);
RESULT

CHECK(Composite::apply(AddHydrogenProcessor&))
	AddHydrogenProcessor proc;
	// Build a dipeptide
	Peptides::PeptideBuilder builder("GG");
	builder.setFragmentDB(&db);
	Protein* prot = builder.construct();

	TEST_EQUAL(prot->countAtoms(), 17);

	// Delete the peptide bond H
	Fragment* res = prot->getChain(0)->getResidue(1);

	Atom* hatom = res->getAtom("H");
	res->remove(*hatom);
	delete hatom;

	TEST_EQUAL(prot->countAtoms(), 16);
	TEST_EQUAL(proc.getNumberOfAddedHydrogens(), 0);

	prot->apply(proc);

	TEST_EQUAL(proc.getNumberOfAddedHydrogens(), 2);
	TEST_EQUAL(prot->countAtoms(), 18);

	delete prot;
RESULT


//getNumberOfAddedHydrogens() const is already implicitly checked
//setRings() is not really checkable

END_TEST

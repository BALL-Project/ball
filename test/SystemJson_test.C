// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// BALL 2.0 — System + Molecule JSON persistence (K0.6.5).
//

#include <BALL/CONCEPT/classTest.h>
#include <BALLTestConfig.h>

///////////////////////////
#include <BALL/KERNEL/systemJson.h>
#include <BALL/KERNEL/system.h>
#include <BALL/KERNEL/molecule.h>
#include <BALL/KERNEL/atom.h>
#include <BALL/KERNEL/moleculeStore.h>
#include <BALL/KERNEL/PTE.h>
#include <BALL/EXTERNAL/nlohmann_json.hpp>
#include <sstream>
#include <string>
///////////////////////////

START_TEST(SystemJson)

using namespace BALL;
using nlohmann::json;

CHECK(K0.6.5 empty System round-trip)
	System src;
	src.setName("empty_sys");

	std::ostringstream os;
	saveSystemJSON(src, os);
	json doc = json::parse(os.str());
	TEST_EQUAL(doc["document_type"].get<std::string>(), std::string("System"))
	TEST_EQUAL(doc["format_version"].get<int>(), SYSTEM_JSON_VERSION)
	TEST_EQUAL(doc["name"].get<std::string>(), std::string("empty_sys"))
	TEST_EQUAL(doc["molecules"].size(), 0u)

	System dst;
	std::istringstream is(os.str());
	loadSystemJSON(dst, is);
	TEST_EQUAL(dst.getName(), String("empty_sys"))
	TEST_EQUAL(dst.countMolecules(), 0u)
	TEST_EQUAL(dst.countAtoms(),     0u)
RESULT

CHECK(K0.6.5 single Molecule with three Atoms round-trip)
	System src;
	src.setName("water_molecule_demo");

	Molecule* m = new Molecule;
	m->setName("WAT");
	src.insert(*m);

	Atom* o = new Atom;
	o->setElement(PTE[Element::OXYGEN]);
	o->setName("O");
	o->setPosition(Vector3(0.f, 0.f, 0.f));
	m->insert(*o);

	Atom* h1 = new Atom;
	h1->setElement(PTE[Element::HYDROGEN]);
	h1->setName("H1");
	h1->setPosition(Vector3(0.96f, 0.f, 0.f));
	m->insert(*h1);

	Atom* h2 = new Atom;
	h2->setElement(PTE[Element::HYDROGEN]);
	h2->setName("H2");
	h2->setPosition(Vector3(-0.24f, 0.93f, 0.f));
	m->insert(*h2);

	std::ostringstream os;
	saveSystemJSON(src, os, 2);

	System dst;
	std::istringstream is(os.str());
	loadSystemJSON(dst, is);

	TEST_EQUAL(dst.getName(),         String("water_molecule_demo"))
	TEST_EQUAL(dst.countMolecules(),  1u)
	TEST_EQUAL(dst.countAtoms(),      3u)
	Molecule* loaded_m = dst.getMolecule(0);
	TEST_NOT_EQUAL(loaded_m, (Molecule*)nullptr)
	TEST_EQUAL(loaded_m->getName(),   String("WAT"))
	TEST_EQUAL(loaded_m->countAtoms(),3u)

	// Walk atoms; verify per-atom payload survived the round-trip.
	int n_walked = 0;
	bool saw_O = false, saw_H1 = false, saw_H2 = false;
	for (AtomIterator it = loaded_m->beginAtom(); +it; ++it)
	{
		++n_walked;
		if (it->getName() == String("O"))
		{
			saw_O = true;
			TEST_EQUAL(it->getElement(), PTE[Element::OXYGEN])
			TEST_EQUAL(it->getPosition(), Vector3(0.f, 0.f, 0.f))
		}
		else if (it->getName() == String("H1"))
		{
			saw_H1 = true;
			TEST_EQUAL(it->getElement(), PTE[Element::HYDROGEN])
			TEST_EQUAL(it->getPosition(), Vector3(0.96f, 0.f, 0.f))
		}
		else if (it->getName() == String("H2"))
		{
			saw_H2 = true;
			TEST_EQUAL(it->getElement(), PTE[Element::HYDROGEN])
		}
	}
	TEST_EQUAL(n_walked, 3)
	TEST_EQUAL(saw_O,  true)
	TEST_EQUAL(saw_H1, true)
	TEST_EQUAL(saw_H2, true)
RESULT

CHECK(K0.6.5 multiple Molecules round-trip preserves grouping)
	System src;
	for (int i = 0; i < 3; ++i)
	{
		Molecule* m = new Molecule;
		m->setName(String("MOL_") + String(i));
		src.insert(*m);
		// 2 atoms per molecule, all carbon, named C1/C2 within their mol.
		for (int j = 0; j < 2; ++j)
		{
			Atom* a = new Atom;
			a->setElement(PTE[Element::CARBON]);
			a->setName(String("C") + String(j + 1));
			m->insert(*a);
		}
	}
	TEST_EQUAL(src.countMolecules(), 3u)
	TEST_EQUAL(src.countAtoms(),     6u)

	std::ostringstream os;
	saveSystemJSON(src, os);
	System dst;
	std::istringstream is(os.str());
	loadSystemJSON(dst, is);

	TEST_EQUAL(dst.countMolecules(), 3u)
	TEST_EQUAL(dst.countAtoms(),     6u)
	for (Position mi = 0; mi < 3; ++mi)
	{
		Molecule* m = dst.getMolecule(mi);
		TEST_NOT_EQUAL(m, (Molecule*)nullptr)
		TEST_EQUAL(m->getName(),     String("MOL_") + String(mi))
		TEST_EQUAL(m->countAtoms(),  2u)
	}
RESULT

CHECK(K0.6.5 bonds round-trip when restoring a small molecule)
	System src;
	Molecule* m = new Molecule;
	m->setName("BOND_DEMO");
	src.insert(*m);
	Atom* c = new Atom; c->setElement(PTE[Element::CARBON]);   m->insert(*c);
	Atom* o = new Atom; o->setElement(PTE[Element::OXYGEN]);   m->insert(*o);
	// Add a bond via store-direct API after both atoms are adopted.
	src.getStore().add_bond(c->getStoreIndex(), o->getStoreIndex(), 2, 0);
	TEST_EQUAL(src.getStore().live_bond_count(), 1u)

	std::ostringstream os;
	saveSystemJSON(src, os);
	System dst;
	std::istringstream is(os.str());
	loadSystemJSON(dst, is);

	TEST_EQUAL(dst.countMolecules(), 1u)
	TEST_EQUAL(dst.countAtoms(),     2u)
	TEST_EQUAL(dst.getStore().live_bond_count(), 1u)
	// Bond order survives.
	bool found_order_2 = false;
	for (std::size_t i = 0; i < dst.getStore().bond_count(); ++i)
	{
		if (dst.getStore().is_bond_dead(static_cast<std::uint32_t>(i))) continue;
		if (dst.getStore().bond(static_cast<std::uint32_t>(i)).order == 2)
			found_order_2 = true;
	}
	TEST_EQUAL(found_order_2, true)
RESULT

CHECK(K0.6.5 wrong document_type rejected)
	// Forge a doc with document_type "MoleculeStore" (would load via
	// loadStoreJSON, not loadSystemJSON).
	std::string forged =
		"{\"document_type\":\"MoleculeStore\",\"format_version\":1,"
		"\"name\":\"x\",\"store\":{},\"molecules\":[]}";
	System dst;
	std::istringstream is(forged);
	bool threw = false;
	try { loadSystemJSON(dst, is); }
	catch (Exception::ParseError&) { threw = true; }
	TEST_EQUAL(threw, true)
RESULT

CHECK(K0.6.5 atom_index out-of-range rejected)
	// Build a doc with 1 atom + 1 molecule referencing atom_index 5.
	System src;
	Molecule* m = new Molecule;
	src.insert(*m);
	Atom* a = new Atom; m->insert(*a);

	std::ostringstream os;
	saveSystemJSON(src, os);

	// Splice atom_indices to reference an out-of-range slot.
	json doc = json::parse(os.str());
	doc["molecules"][0]["atom_indices"] = json::array({999});
	std::string forged = doc.dump();

	System dst;
	std::istringstream is(forged);
	bool threw = false;
	try { loadSystemJSON(dst, is); }
	catch (Exception::ParseError&) { threw = true; }
	TEST_EQUAL(threw, true)
RESULT

END_TEST

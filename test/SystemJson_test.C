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

// ============================================================
// K0.6.5b — PropertyManager bag + orphan atoms
// ============================================================

CHECK(K0.6.5b System-level named properties round-trip)
	System src;
	src.setName("WithProps");
	src.setProperty(std::string("answer"), 42);
	src.setProperty(std::string("pi"), 3.14159f);
	src.setProperty(std::string("greeting"), String("hello"));
	src.setProperty(std::string("ready"), true);

	std::ostringstream os;
	saveSystemJSON(src, os);
	System dst;
	std::istringstream is(os.str());
	loadSystemJSON(dst, is);

	TEST_EQUAL(dst.getName(), String("WithProps"))
	TEST_EQUAL(dst.countNamedProperties(), 4u)
	bool has_answer = false, has_pi = false, has_greet = false, has_ready = false;
	for (Position i = 0; i < dst.countNamedProperties(); ++i)
	{
		const NamedProperty& np = dst.getNamedProperty(i);
		if (np.getName() == std::string("answer") && np.getType() == NamedProperty::INT)
		{
			has_answer = (np.getInt() == 42);
		}
		else if (np.getName() == std::string("pi") && np.getType() == NamedProperty::FLOAT)
		{
			has_pi = (np.getFloat() == 3.14159f);
		}
		else if (np.getName() == std::string("greeting") && np.getType() == NamedProperty::STRING)
		{
			has_greet = (np.getString() == String("hello"));
		}
		else if (np.getName() == std::string("ready") && np.getType() == NamedProperty::BOOL)
		{
			has_ready = (np.getBool() == true);
		}
	}
	TEST_EQUAL(has_answer, true)
	TEST_EQUAL(has_pi,     true)
	TEST_EQUAL(has_greet,  true)
	TEST_EQUAL(has_ready,  true)
RESULT

CHECK(K0.6.5b per-Molecule named properties round-trip)
	System src;
	Molecule* m = new Molecule;
	m->setName("WaterWithProps");
	m->setProperty(std::string("formula"), String("H2O"));
	m->setProperty(std::string("mw"), 18.015);
	src.insert(*m);
	Atom* o = new Atom; o->setElement(PTE[Element::OXYGEN]); m->insert(*o);

	std::ostringstream os;
	saveSystemJSON(src, os);
	System dst;
	std::istringstream is(os.str());
	loadSystemJSON(dst, is);

	Molecule* loaded = dst.getMolecule(0);
	TEST_NOT_EQUAL(loaded, (Molecule*)nullptr)
	TEST_EQUAL(loaded->countNamedProperties(), 2u)
	bool found_formula = false, found_mw = false;
	for (Position i = 0; i < loaded->countNamedProperties(); ++i)
	{
		const NamedProperty& np = loaded->getNamedProperty(i);
		if (np.getName() == std::string("formula") && np.getString() == String("H2O"))
			found_formula = true;
		if (np.getName() == std::string("mw") && np.getDouble() == 18.015)
			found_mw = true;
	}
	TEST_EQUAL(found_formula, true)
	TEST_EQUAL(found_mw,      true)
RESULT

CHECK(K0.6.5b per-Atom named properties round-trip)
	System src;
	Molecule* m = new Molecule;
	src.insert(*m);
	Atom* a = new Atom;
	a->setName("CA");
	a->setProperty(std::string("partial_charge"), -0.41f);
	a->setProperty(std::string("residue_seq"), 7);
	a->setProperty(std::string("is_aromatic"), false);
	m->insert(*a);

	std::ostringstream os;
	saveSystemJSON(src, os);
	System dst;
	std::istringstream is(os.str());
	loadSystemJSON(dst, is);

	AtomIterator it = dst.getMolecule(0)->beginAtom();
	Atom& la = *it;
	TEST_EQUAL(la.getName(), String("CA"))
	TEST_EQUAL(la.countNamedProperties(), 3u)
	bool pc = false, rs = false, ar = false;
	for (Position i = 0; i < la.countNamedProperties(); ++i)
	{
		const NamedProperty& np = la.getNamedProperty(i);
		if (np.getName() == std::string("partial_charge") && np.getFloat() == -0.41f) pc = true;
		if (np.getName() == std::string("residue_seq")    && np.getInt()   == 7)      rs = true;
		if (np.getName() == std::string("is_aromatic")    && np.getBool()  == false)  ar = true;
	}
	TEST_EQUAL(pc, true)
	TEST_EQUAL(rs, true)
	TEST_EQUAL(ar, true)
RESULT

CHECK(K0.6.5b orphan atoms (System direct children) round-trip)
	System src;
	// One molecule with one atom.
	Molecule* m = new Molecule;
	m->setName("InMolecule");
	src.insert(*m);
	Atom* mol_atom = new Atom;
	mol_atom->setName("MOL_A");
	m->insert(*mol_atom);
	// Two atoms attached directly to the System (no molecule).
	// System's insert(Molecule&) overload hides the inherited
	// AtomContainer::insert(Atom&), so call it explicitly through
	// the base class.
	Atom* orph1 = new Atom; orph1->setName("ORPH1"); src.AtomContainer::insert(*orph1);
	Atom* orph2 = new Atom; orph2->setName("ORPH2"); src.AtomContainer::insert(*orph2);

	TEST_EQUAL(src.countMolecules(), 1u)
	TEST_EQUAL(src.countAtoms(),     3u)

	std::ostringstream os;
	saveSystemJSON(src, os);
	json doc = json::parse(os.str());
	TEST_EQUAL(doc.contains("system_atom_indices"), true)
	TEST_EQUAL(doc["system_atom_indices"].size(), 2u)

	System dst;
	std::istringstream is(os.str());
	loadSystemJSON(dst, is);

	TEST_EQUAL(dst.countMolecules(), 1u)
	TEST_EQUAL(dst.countAtoms(),     3u)
	TEST_EQUAL(dst.getMolecule(0)->getName(), String("InMolecule"))
	TEST_EQUAL(dst.getMolecule(0)->countAtoms(), 1u)
	// Two orphan atoms ended up as System direct children — countAtoms
	// on System sees them, but they're not inside any molecule.
RESULT

CHECK(K0.6.5b OBJECT-typed property survives as NONE placeholder)
	// Non-scalar properties (OBJECT, SMART_OBJECT) are not portable;
	// the writer emits a placeholder so the key survives. Verify the
	// key round-trips (even though the value pointer doesn't).
	System src;
	int local_int = 999;
	src.setProperty(std::string("kept_int"), 5);
	src.setProperty(std::string("dropped_ptr"), (void*)&local_int);  // OBJECT path

	std::ostringstream os;
	saveSystemJSON(src, os);
	System dst;
	std::istringstream is(os.str());
	loadSystemJSON(dst, is);

	bool saw_kept = false, saw_dropped = false;
	for (Position i = 0; i < dst.countNamedProperties(); ++i)
	{
		const NamedProperty& np = dst.getNamedProperty(i);
		if (np.getName() == std::string("kept_int"))    saw_kept    = true;
		if (np.getName() == std::string("dropped_ptr")) saw_dropped = true;
	}
	TEST_EQUAL(saw_kept,    true)
	TEST_EQUAL(saw_dropped, true)
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

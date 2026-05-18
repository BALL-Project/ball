// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// BALL 2.0 — K0.6.4 / Track B B0.2 converter round-trip tests.
//

#include <BALL/CONCEPT/classTest.h>
#include <BALLTestConfig.h>

///////////////////////////
#include <BALL/KERNEL/v1ToV2JsonConverter.h>
#include <BALL/KERNEL/systemJson.h>
#include <BALL/KERNEL/system.h>
#include <BALL/KERNEL/molecule.h>
#include <BALL/KERNEL/atom.h>
#include <BALL/KERNEL/moleculeStore.h>
#include <BALL/KERNEL/PTE.h>
#include <BALL/CONCEPT/textPersistenceManager.h>
#include <sstream>
#include <string>
///////////////////////////

START_TEST(V1ToV2JsonConverter)

using namespace BALL;

CHECK(B0.2 empty System: v1.bal -> v2.json round-trip)
	// Build a v1 .bal stream by writing an empty System through
	// TextPersistenceManager; convert; load into a fresh System;
	// verify shape preserved.
	System src;
	src.setName("EMPTY_V1");
	std::ostringstream v1_stream;
	{
		TextPersistenceManager pm;
		pm.setOstream(v1_stream);
		pm.registerClass(RTTI::getStreamName<System>(),
		                 (PersistenceManager::CreateMethod)&System::createDefault);
		pm << src;
	}
	TEST_NOT_EQUAL(v1_stream.str().size(), 0u)

	std::istringstream v1_in(v1_stream.str());
	std::ostringstream v2_out;
	const bool ok = convertV1BalToV2JSON(v1_in, v2_out);
	TEST_EQUAL(ok, true)

	System dst;
	std::istringstream v2_in(v2_out.str());
	loadSystemJSON(dst, v2_in);
	TEST_EQUAL(dst.getName(), String("EMPTY_V1"))
	TEST_EQUAL(dst.countMolecules(), 0u)
	TEST_EQUAL(dst.countAtoms(),     0u)
RESULT

CHECK(B0.2 single Molecule with three atoms: v1.bal -> v2.json round-trip)
	System src;
	src.setName("WATER_V1");
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

	// Serialize as v1 text.
	std::ostringstream v1_stream;
	{
		TextPersistenceManager pm;
		pm.setOstream(v1_stream);
		pm.registerClass(RTTI::getStreamName<System>(),
		                 (PersistenceManager::CreateMethod)&System::createDefault);
		pm << src;
	}

	// Convert.
	std::istringstream v1_in(v1_stream.str());
	std::ostringstream v2_out;
	const bool ok = convertV1BalToV2JSON(v1_in, v2_out);
	TEST_EQUAL(ok, true)

	// Load back from v2 JSON + verify shape.
	System dst;
	std::istringstream v2_in(v2_out.str());
	loadSystemJSON(dst, v2_in);

	TEST_EQUAL(dst.getName(),          String("WATER_V1"))
	TEST_EQUAL(dst.countMolecules(),   1u)
	TEST_EQUAL(dst.countAtoms(),       3u)
	Molecule* dm = dst.getMolecule(0);
	TEST_NOT_EQUAL(dm, (Molecule*)nullptr)
	TEST_EQUAL(dm->getName(),          String("WAT"))
	TEST_EQUAL(dm->countAtoms(),       3u)

	// Walk + verify atoms — element + name + position survived.
	bool saw_O = false, saw_H1 = false, saw_H2 = false;
	for (AtomIterator it = dm->beginAtom(); +it; ++it)
	{
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
	TEST_EQUAL(saw_O,  true)
	TEST_EQUAL(saw_H1, true)
	TEST_EQUAL(saw_H2, true)
RESULT

CHECK(B0.2 malformed v1 stream returns false)
	std::istringstream bad("this is not a BAL persistence stream");
	std::ostringstream out;
	const bool ok = convertV1BalToV2JSON(bad, out);
	TEST_EQUAL(ok, false)
RESULT

END_TEST

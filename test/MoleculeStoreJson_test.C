// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// BALL 2.0 — MoleculeStore JSON writer (K0.6.1).
// Tests writer correctness by parsing the output JSON back with
// nlohmann::json and verifying column equality. K0.6.2 adds the
// proper reader and a full saveJSON -> loadJSON -> saveJSON
// idempotence test.
//

#include <BALL/CONCEPT/classTest.h>
#include <BALLTestConfig.h>

///////////////////////////
#include <BALL/KERNEL/moleculeStoreJson.h>
#include <BALL/KERNEL/moleculeStore.h>
#include <BALL/EXTERNAL/nlohmann_json.hpp>
#include <sstream>
#include <vector>
#include <string>
///////////////////////////

START_TEST(MoleculeStoreJson)

using namespace BALL;
using nlohmann::json;

CHECK(K0.6.1 empty store -> minimal JSON)
	MoleculeStore s;
	std::ostringstream os;
	saveStoreJSON(s, os);
	json doc = json::parse(os.str());
	TEST_EQUAL(doc["format_version"].get<int>(), MOLECULE_STORE_JSON_VERSION)
	TEST_EQUAL(doc["size"].get<int>(), 0)
	TEST_EQUAL(doc["live_atom_count"].get<int>(), 0)
	TEST_EQUAL(doc["atoms"]["positions"].size(), 0u)
	TEST_EQUAL(doc["bonds"].size(), 0u)
RESULT

CHECK(K0.6.1 single atom round-trip via JSON parse)
	MoleculeStore s;
	auto i = s.allocate_atom();
	s.position(i)      = Vector3(1.f, 2.f, 3.f);
	s.velocity(i)      = Vector3(4.f, 5.f, 6.f);
	s.force(i)         = Vector3(7.f, 8.f, 9.f);
	s.charge(i)        = -0.5f;
	s.radius(i)        = 1.7f;
	s.atom_type(i)     = 42;
	s.formal_charge(i) = -1;
	s.element_index(i) = 6;  // Carbon
	s.set_selected(i, true);
	s.set_name(i, "CA");
	s.set_type_name(i, "C.3");

	std::ostringstream os;
	saveStoreJSON(s, os);
	json doc = json::parse(os.str());

	TEST_EQUAL(doc["size"].get<int>(), 1)
	TEST_EQUAL(doc["live_atom_count"].get<int>(), 1)

	auto& a = doc["atoms"];
	TEST_EQUAL(a["positions"][0][0].get<float>(), 1.f)
	TEST_EQUAL(a["positions"][0][1].get<float>(), 2.f)
	TEST_EQUAL(a["positions"][0][2].get<float>(), 3.f)
	TEST_EQUAL(a["velocities"][0][0].get<float>(), 4.f)
	TEST_EQUAL(a["forces"][0][2].get<float>(), 9.f)
	TEST_EQUAL(a["charges"][0].get<float>(), -0.5f)
	TEST_EQUAL(a["radii"][0].get<float>(), 1.7f)
	TEST_EQUAL(a["atom_types"][0].get<int>(), 42)
	TEST_EQUAL(a["formal_charges"][0].get<int>(), -1)
	TEST_EQUAL(a["element_indices"][0].get<int>(), 6)
	TEST_EQUAL(a["selection"][0].get<int>(), 1)
	TEST_EQUAL(a["names"][0].get<std::string>(), std::string("CA"))
	TEST_EQUAL(a["type_names"][0].get<std::string>(), std::string("C.3"))
	TEST_EQUAL(a["is_freed"][0].get<int>(), 0)
RESULT

CHECK(K0.6.1 freed slot preserved with is_freed flag)
	// Index identity matters for bonds; freed slots stay in place with
	// is_freed=1, so the reader can replay release_atom after rebuild.
	MoleculeStore s;
	auto a_idx = s.allocate_atom();
	auto b_idx = s.allocate_atom();
	auto c_idx = s.allocate_atom();
	s.set_name(a_idx, "A");
	s.set_name(c_idx, "C");
	s.release_atom(b_idx);

	std::ostringstream os;
	saveStoreJSON(s, os);
	json doc = json::parse(os.str());

	TEST_EQUAL(doc["size"].get<int>(), 3)
	TEST_EQUAL(doc["live_atom_count"].get<int>(), 2)
	TEST_EQUAL(doc["atoms"]["is_freed"][0].get<int>(), 0)
	TEST_EQUAL(doc["atoms"]["is_freed"][1].get<int>(), 1)   // freed
	TEST_EQUAL(doc["atoms"]["is_freed"][2].get<int>(), 0)
	TEST_EQUAL(doc["atoms"]["names"][0].get<std::string>(), std::string("A"))
	TEST_EQUAL(doc["atoms"]["names"][2].get<std::string>(), std::string("C"))
RESULT

CHECK(K0.6.1 bond table excludes tombstones)
	MoleculeStore s;
	auto a = s.allocate_atom();
	auto b = s.allocate_atom();
	auto c = s.allocate_atom();
	auto bond_ab = s.add_bond(a, b, 1, 0);
	auto bond_bc = s.add_bond(b, c, 2, 0);
	auto bond_ac = s.add_bond(a, c, 3, 0);
	(void)bond_ab; (void)bond_ac;
	s.remove_bond(bond_bc);  // tombstone

	std::ostringstream os;
	saveStoreJSON(s, os);
	json doc = json::parse(os.str());

	TEST_EQUAL(doc["bonds"].size(), 2u)   // dead bond filtered
	// First emitted bond is a-b (order 1), second is a-c (order 3).
	// Order in output matches store iteration order.
	bool found_ab = false, found_ac = false;
	for (auto& br : doc["bonds"])
	{
		int aa = br["a"].get<int>();
		int bb = br["b"].get<int>();
		int oo = br["order"].get<int>();
		if (aa == (int)a && bb == (int)b && oo == 1) found_ab = true;
		if (aa == (int)a && bb == (int)c && oo == 3) found_ac = true;
	}
	TEST_EQUAL(found_ab, true)
	TEST_EQUAL(found_ac, true)
RESULT

CHECK(K0.6.1 compact vs pretty output)
	MoleculeStore s;
	(void)s.allocate_atom();
	std::ostringstream compact, pretty;
	saveStoreJSON(s, compact, -1);
	saveStoreJSON(s, pretty,  2);
	// Pretty output is strictly larger (more whitespace).
	TEST_EQUAL(pretty.str().size() > compact.str().size(), true)
	// Both parse to identical JSON.
	TEST_EQUAL(json::parse(compact.str()) == json::parse(pretty.str()), true)
RESULT

END_TEST

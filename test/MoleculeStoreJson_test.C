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
#include <cstring>
#include <cstdint>
#include <limits>
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

// ============================================================
// K0.6.2 — JSON reader (loadStoreJSON) round-trip tests
// ============================================================

CHECK(K0.6.2 empty store round-trip)
	MoleculeStore src;
	std::ostringstream os;
	saveStoreJSON(src, os);

	MoleculeStore dst;
	std::istringstream is(os.str());
	loadStoreJSON(dst, is);

	TEST_EQUAL(dst.size(), 0u)
	TEST_EQUAL(dst.live_atom_count(), 0u)
	TEST_EQUAL(dst.bond_count(), 0u)
RESULT

CHECK(K0.6.2 single-atom round-trip preserves every column)
	MoleculeStore src;
	auto i = src.allocate_atom();
	src.position(i)      = Vector3(1.f, 2.f, 3.f);
	src.velocity(i)      = Vector3(4.f, 5.f, 6.f);
	src.force(i)         = Vector3(7.f, 8.f, 9.f);
	src.charge(i)        = -0.5f;
	src.radius(i)        = 1.7f;
	src.atom_type(i)     = 42;
	src.formal_charge(i) = -1;
	src.element_index(i) = 6;
	src.set_selected(i, true);
	src.set_name(i, "CA");
	src.set_type_name(i, "C.3");

	std::ostringstream os;
	saveStoreJSON(src, os);
	MoleculeStore dst;
	std::istringstream is(os.str());
	loadStoreJSON(dst, is);

	TEST_EQUAL(dst.size(), 1u)
	TEST_EQUAL(dst.live_atom_count(), 1u)
	TEST_EQUAL(dst.position(0), Vector3(1.f, 2.f, 3.f))
	TEST_EQUAL(dst.velocity(0), Vector3(4.f, 5.f, 6.f))
	TEST_EQUAL(dst.force(0),    Vector3(7.f, 8.f, 9.f))
	TEST_EQUAL(dst.charge(0), -0.5f)
	TEST_EQUAL(dst.radius(0),  1.7f)
	TEST_EQUAL(dst.atom_type(0), 42)
	TEST_EQUAL(dst.formal_charge(0), -1)
	TEST_EQUAL((int)dst.element_index(0), 6)
	TEST_EQUAL(dst.selected(0), true)
	TEST_EQUAL(dst.get_name(0), std::string("CA"))
	TEST_EQUAL(dst.get_type_name(0), std::string("C.3"))
RESULT

CHECK(K0.6.2 freed-slot round-trip preserves index identity)
	MoleculeStore src;
	auto a = src.allocate_atom();
	auto b = src.allocate_atom();
	auto c = src.allocate_atom();
	src.set_name(a, "A");
	src.set_name(c, "C");
	src.release_atom(b);

	std::ostringstream os;
	saveStoreJSON(src, os);
	MoleculeStore dst;
	std::istringstream is(os.str());
	loadStoreJSON(dst, is);

	// Size + live count preserved.
	TEST_EQUAL(dst.size(), 3u)
	TEST_EQUAL(dst.live_atom_count(), 2u)
	// Slots at 0 and 2 are live with the right names; slot 1 stayed freed.
	TEST_EQUAL(dst.is_freed(a), false)
	TEST_EQUAL(dst.is_freed(b), true)
	TEST_EQUAL(dst.is_freed(c), false)
	TEST_EQUAL(dst.get_name(a), std::string("A"))
	TEST_EQUAL(dst.get_name(c), std::string("C"))
RESULT

CHECK(K0.6.2 bond table round-trip)
	MoleculeStore src;
	auto a = src.allocate_atom();
	auto b = src.allocate_atom();
	auto c = src.allocate_atom();
	src.add_bond(a, b, 1, 0);
	auto dead = src.add_bond(b, c, 2, 0);
	src.add_bond(a, c, 3, 0);
	src.remove_bond(dead);

	std::ostringstream os;
	saveStoreJSON(src, os);
	MoleculeStore dst;
	std::istringstream is(os.str());
	loadStoreJSON(dst, is);

	// 2 live bonds re-added; their endpoints + orders match.
	TEST_EQUAL(dst.live_bond_count(), 2u)
	// CSR-walk both bonds via for_each_bond_of and confirm orders 1 + 3 are present.
	int saw_one = 0, saw_three = 0;
	for (std::size_t i = 0; i < dst.bond_count(); ++i)
	{
		if (dst.is_bond_dead(static_cast<std::uint32_t>(i))) continue;
		const auto& br = dst.bond(static_cast<std::uint32_t>(i));
		if (br.order == 1) ++saw_one;
		if (br.order == 3) ++saw_three;
	}
	TEST_EQUAL(saw_one,   1)
	TEST_EQUAL(saw_three, 1)
RESULT

CHECK(K0.6.2 load into a non-empty store wipes existing state)
	MoleculeStore dst;
	auto x = dst.allocate_atom();
	auto y = dst.allocate_atom();
	dst.set_name(x, "STALE_X");
	dst.set_name(y, "STALE_Y");
	dst.add_bond(x, y, 1, 0);
	TEST_EQUAL(dst.size(), 2u)
	TEST_EQUAL(dst.live_atom_count(), 2u)

	MoleculeStore src;
	auto a = src.allocate_atom();
	src.set_name(a, "FRESH");

	std::ostringstream os;
	saveStoreJSON(src, os);
	std::istringstream is(os.str());
	loadStoreJSON(dst, is);

	// dst now reflects src functionally: 1 live atom named FRESH, no bonds.
	// size() may be >= 1 since reset_store_ only releases (doesn't compact);
	// allocate_atom() reuses the free-list so the new slot lands at an old
	// slot's index. The stale bond was wiped before the slot was freed.
	TEST_EQUAL(dst.live_atom_count(), 1u)
	TEST_EQUAL(dst.live_bond_count(), 0u)
	// Walk every live slot, verify exactly one and that it's named FRESH.
	std::size_t live_count = 0;
	std::string live_name;
	for (std::size_t i = 0; i < dst.size(); ++i)
	{
		if (dst.is_freed(i)) continue;
		++live_count;
		live_name = dst.get_name(i);
	}
	TEST_EQUAL(live_count, 1u)
	TEST_EQUAL(live_name, std::string("FRESH"))
RESULT

CHECK(K0.6.2 malformed JSON throws ParseError)
	MoleculeStore dst;
	std::istringstream is("{not valid json");
	bool threw = false;
	try { loadStoreJSON(dst, is); }
	catch (Exception::ParseError&) { threw = true; }
	TEST_EQUAL(threw, true)
RESULT

CHECK(K0.6.2 future format_version rejected with ParseError)
	MoleculeStore dst;
	// Hand-craft a doc with format_version = 999, otherwise minimal.
	std::string forged =
		"{\"format_version\":999,\"size\":0,\"live_atom_count\":0,"
		"\"generation\":0,\"atoms\":{\"positions\":[],\"velocities\":[],"
		"\"forces\":[],\"charges\":[],\"radii\":[],\"atom_types\":[],"
		"\"formal_charges\":[],\"element_indices\":[],\"selection\":[],"
		"\"names\":[],\"type_names\":[],\"stable_ids\":[],\"is_freed\":[]},"
		"\"bonds\":[]}";
	std::istringstream is(forged);
	bool threw = false;
	try { loadStoreJSON(dst, is); }
	catch (Exception::ParseError&) { threw = true; }
	TEST_EQUAL(threw, true)
RESULT

CHECK(K0.6.2 column-length mismatch rejected with ParseError)
	MoleculeStore dst;
	// size=2 declared but positions has only 1 entry.
	std::string forged =
		"{\"format_version\":1,\"size\":2,\"live_atom_count\":2,"
		"\"generation\":0,\"atoms\":{\"positions\":[[0,0,0]],\"velocities\":[],"
		"\"forces\":[],\"charges\":[],\"radii\":[],\"atom_types\":[],"
		"\"formal_charges\":[],\"element_indices\":[],\"selection\":[],"
		"\"names\":[],\"type_names\":[],\"stable_ids\":[],"
		"\"is_freed\":[]},\"bonds\":[]}";
	std::istringstream is(forged);
	bool threw = false;
	try { loadStoreJSON(dst, is); }
	catch (Exception::ParseError&) { threw = true; }
	TEST_EQUAL(threw, true)
RESULT

// ============================================================
// K0.6.3 — schema versioning + invariants + bit-exact float +
//          stable_id round-trip
// ============================================================

CHECK(K0.6.3 writer emits format_minor + float_format header)
	MoleculeStore s;
	std::ostringstream os;
	saveStoreJSON(s, os);
	json doc = json::parse(os.str());
	TEST_EQUAL(doc.contains("format_minor"), true)
	TEST_EQUAL(doc["format_minor"].get<int>(), MOLECULE_STORE_JSON_VERSION_MINOR)
	TEST_EQUAL(doc["float_format"].get<std::string>(), std::string("decimal"))
RESULT

CHECK(K0.6.3 reader accepts higher-minor docs silently)
	// Forge a doc with format_minor 999 but the correct major + columns.
	std::string forged =
		"{\"format_version\":1,\"format_minor\":999,"
		"\"float_format\":\"decimal\","
		"\"size\":0,\"live_atom_count\":0,\"generation\":0,"
		"\"atoms\":{\"positions\":[],\"velocities\":[],\"forces\":[],"
		"\"charges\":[],\"radii\":[],\"atom_types\":[],"
		"\"formal_charges\":[],\"element_indices\":[],\"selection\":[],"
		"\"names\":[],\"type_names\":[],\"stable_ids\":[],"
		"\"is_freed\":[]},\"bonds\":[],\"future_top_level_key\":42}";
	MoleculeStore dst;
	std::istringstream is(forged);
	// Higher minor + an unknown top-level key should not throw.
	loadStoreJSON(dst, is);
	TEST_EQUAL(dst.size(), 0u)
RESULT

CHECK(K0.6.3 K0.6.1/.2 docs without format_minor still load)
	// Pre-K0.6.3 documents omitted format_minor entirely. Reader must
	// treat the absence as minor=0 (forward-compat with old writers).
	std::string forged =
		"{\"format_version\":1,\"size\":0,\"live_atom_count\":0,"
		"\"generation\":0,\"atoms\":{\"positions\":[],\"velocities\":[],"
		"\"forces\":[],\"charges\":[],\"radii\":[],\"atom_types\":[],"
		"\"formal_charges\":[],\"element_indices\":[],\"selection\":[],"
		"\"names\":[],\"type_names\":[],\"stable_ids\":[],"
		"\"is_freed\":[]},\"bonds\":[]}";
	MoleculeStore dst;
	std::istringstream is(forged);
	loadStoreJSON(dst, is);
	TEST_EQUAL(dst.size(), 0u)
RESULT

CHECK(K0.6.3 bit-exact float round-trip survives edge cases)
	MoleculeStore src;
	auto a = src.allocate_atom();
	auto b = src.allocate_atom();
	auto c = src.allocate_atom();
	auto d = src.allocate_atom();
	auto e = src.allocate_atom();

	// Edge-case floats: pi, denormal, +0, -0, +inf.
	src.position(a) = Vector3(3.141592653589793f, 0.f, 0.f);
	src.charge(a)   = 3.141592653589793f;
	// Smallest positive denormal (about 1.4e-45).
	src.charge(b)   = std::numeric_limits<float>::denorm_min();
	src.charge(c)   = +0.0f;
	src.charge(d)   = -0.0f;
	src.charge(e)   = std::numeric_limits<float>::infinity();

	std::ostringstream os;
	saveStoreJSON(src, os, -1, JsonFloatFormat::BIT_EXACT_HEX);
	// Confirm the JSON contains hex strings (not decimal floats).
	json doc = json::parse(os.str());
	TEST_EQUAL(doc["float_format"].get<std::string>(), std::string("bit_exact_hex"))
	TEST_EQUAL(doc["atoms"]["charges"][a].is_string(), true)

	MoleculeStore dst;
	std::istringstream is(os.str());
	loadStoreJSON(dst, is);

	// Bit-exact round-trip — memcmp the IEEE-754 bits, not float ==
	// (which would treat -0 == +0 and NaN != NaN).
	auto bits_of = [](float f) {
		std::uint32_t b; std::memcpy(&b, &f, sizeof(b)); return b;
	};
	TEST_EQUAL(bits_of(dst.charge(a)), bits_of(3.141592653589793f))
	TEST_EQUAL(bits_of(dst.charge(b)), bits_of(std::numeric_limits<float>::denorm_min()))
	TEST_EQUAL(bits_of(dst.charge(c)), bits_of(+0.0f))
	TEST_EQUAL(bits_of(dst.charge(d)), bits_of(-0.0f))
	TEST_EQUAL(bits_of(dst.charge(e)), bits_of(std::numeric_limits<float>::infinity()))
	// Position x coord round-trips exactly too.
	TEST_EQUAL(bits_of(dst.position(a).x), bits_of(3.141592653589793f))
RESULT

CHECK(K0.6.3 decimal docs still round-trip when reader uses auto-decode)
	// Reader auto-detects per element: writer using DECIMAL emits
	// numbers; reader's decode_float_ takes the number path. This
	// protects against accidental hex/decimal mixing.
	MoleculeStore src;
	auto a = src.allocate_atom();
	src.charge(a) = 1.5f;
	std::ostringstream os;
	saveStoreJSON(src, os, -1, JsonFloatFormat::DECIMAL);
	MoleculeStore dst;
	std::istringstream is(os.str());
	loadStoreJSON(dst, is);
	TEST_EQUAL(dst.charge(0), 1.5f)
RESULT

CHECK(K0.6.3 stable_ids round-trip and reseed next_stable_id_)
	MoleculeStore src;
	auto a = src.allocate_atom();
	auto b = src.allocate_atom();
	auto c = src.allocate_atom();
	const auto id_a = src.stable_id(a);
	const auto id_b = src.stable_id(b);
	const auto id_c = src.stable_id(c);
	// IDs should be 1, 2, 3 for a fresh store.
	TEST_EQUAL(id_a, 1u)
	TEST_EQUAL(id_b, 2u)
	TEST_EQUAL(id_c, 3u)

	std::ostringstream os;
	saveStoreJSON(src, os);
	MoleculeStore dst;
	std::istringstream is(os.str());
	loadStoreJSON(dst, is);

	// IDs preserved.
	TEST_EQUAL(dst.stable_id(0), id_a)
	TEST_EQUAL(dst.stable_id(1), id_b)
	TEST_EQUAL(dst.stable_id(2), id_c)
	// next_stable_id_ should be reseeded past the max, so a NEW
	// allocation gets id 4 (not 1, which would collide with id_a).
	auto fresh = dst.allocate_atom();
	TEST_EQUAL(dst.stable_id(fresh) >= 4u, true)
RESULT

CHECK(K0.6.3 element_index out-of-range rejected)
	std::string forged =
		"{\"format_version\":1,\"size\":1,\"live_atom_count\":1,"
		"\"generation\":0,\"atoms\":{\"positions\":[[0,0,0]],"
		"\"velocities\":[[0,0,0]],\"forces\":[[0,0,0]],"
		"\"charges\":[0],\"radii\":[0],\"atom_types\":[0],"
		"\"formal_charges\":[0],\"element_indices\":[999],"
		"\"selection\":[0],\"names\":[\"X\"],\"type_names\":[\"X\"],"
		"\"stable_ids\":[1],\"is_freed\":[0]},\"bonds\":[]}";
	MoleculeStore dst;
	std::istringstream is(forged);
	bool threw = false;
	try { loadStoreJSON(dst, is); }
	catch (Exception::ParseError&) { threw = true; }
	TEST_EQUAL(threw, true)
RESULT

CHECK(K0.6.3 selection-must-be-0-or-1 rejected)
	std::string forged =
		"{\"format_version\":1,\"size\":1,\"live_atom_count\":1,"
		"\"generation\":0,\"atoms\":{\"positions\":[[0,0,0]],"
		"\"velocities\":[[0,0,0]],\"forces\":[[0,0,0]],"
		"\"charges\":[0],\"radii\":[0],\"atom_types\":[0],"
		"\"formal_charges\":[0],\"element_indices\":[0],"
		"\"selection\":[2],\"names\":[\"X\"],\"type_names\":[\"X\"],"
		"\"stable_ids\":[1],\"is_freed\":[0]},\"bonds\":[]}";
	MoleculeStore dst;
	std::istringstream is(forged);
	bool threw = false;
	try { loadStoreJSON(dst, is); }
	catch (Exception::ParseError&) { threw = true; }
	TEST_EQUAL(threw, true)
RESULT

CHECK(K0.6.3b writer emits document_type discriminator)
	MoleculeStore s;
	std::ostringstream os;
	saveStoreJSON(s, os);
	json doc = json::parse(os.str());
	TEST_EQUAL(doc["document_type"].get<std::string>(), std::string("MoleculeStore"))
RESULT

CHECK(K0.6.3b wrong document_type rejected with ParseError)
	std::string forged =
		"{\"document_type\":\"System\",\"format_version\":1,"
		"\"format_minor\":1,\"size\":0,\"live_atom_count\":0,"
		"\"generation\":0,\"atoms\":{\"positions\":[],\"velocities\":[],"
		"\"forces\":[],\"charges\":[],\"radii\":[],\"atom_types\":[],"
		"\"formal_charges\":[],\"element_indices\":[],\"selection\":[],"
		"\"names\":[],\"type_names\":[],\"stable_ids\":[],"
		"\"is_freed\":[]},\"bonds\":[]}";
	MoleculeStore dst;
	std::istringstream is(forged);
	bool threw = false;
	try { loadStoreJSON(dst, is); }
	catch (Exception::ParseError&) { threw = true; }
	TEST_EQUAL(threw, true)
RESULT

CHECK(K0.6.3b malformed hex float -> ParseError not std::invalid_argument)
	std::string forged =
		"{\"document_type\":\"MoleculeStore\",\"format_version\":1,"
		"\"format_minor\":1,\"size\":1,\"live_atom_count\":1,"
		"\"generation\":0,\"atoms\":{"
		"\"positions\":[[\"0xZZZZZZZZ\",[0],[0]]],"
		"\"velocities\":[[0,0,0]],\"forces\":[[0,0,0]],"
		"\"charges\":[0],\"radii\":[0],\"atom_types\":[0],"
		"\"formal_charges\":[0],\"element_indices\":[0],"
		"\"selection\":[0],\"names\":[\"X\"],\"type_names\":[\"X\"],"
		"\"stable_ids\":[1],\"is_freed\":[0]},\"bonds\":[]}";
	MoleculeStore dst;
	std::istringstream is(forged);
	bool threw_parse  = false;
	bool threw_other = false;
	try { loadStoreJSON(dst, is); }
	catch (Exception::ParseError&)     { threw_parse  = true; }
	catch (...)                        { threw_other = true; }
	// Must be a ParseError, not a leaked std::invalid_argument.
	TEST_EQUAL(threw_parse,  true)
	TEST_EQUAL(threw_other, false)
RESULT

CHECK(K0.6.3b is_freed strict-0/1 rejected when value is 2)
	std::string forged =
		"{\"document_type\":\"MoleculeStore\",\"format_version\":1,"
		"\"format_minor\":1,\"size\":1,\"live_atom_count\":1,"
		"\"generation\":0,\"atoms\":{"
		"\"positions\":[[0,0,0]],\"velocities\":[[0,0,0]],"
		"\"forces\":[[0,0,0]],\"charges\":[0],\"radii\":[0],"
		"\"atom_types\":[0],\"formal_charges\":[0],"
		"\"element_indices\":[0],\"selection\":[0],\"names\":[\"X\"],"
		"\"type_names\":[\"X\"],\"stable_ids\":[1],\"is_freed\":[2]},"
		"\"bonds\":[]}";
	MoleculeStore dst;
	std::istringstream is(forged);
	bool threw = false;
	try { loadStoreJSON(dst, is); }
	catch (Exception::ParseError&) { threw = true; }
	TEST_EQUAL(threw, true)
RESULT

CHECK(K0.6.3b live_atom_count cross-check)
	// size=2, is_freed=[0,0] -> 2 live; declared live_atom_count=1 should reject.
	std::string forged =
		"{\"document_type\":\"MoleculeStore\",\"format_version\":1,"
		"\"format_minor\":1,\"size\":2,\"live_atom_count\":1,"
		"\"generation\":0,\"atoms\":{"
		"\"positions\":[[0,0,0],[0,0,0]],"
		"\"velocities\":[[0,0,0],[0,0,0]],"
		"\"forces\":[[0,0,0],[0,0,0]],"
		"\"charges\":[0,0],\"radii\":[0,0],\"atom_types\":[0,0],"
		"\"formal_charges\":[0,0],\"element_indices\":[0,0],"
		"\"selection\":[0,0],\"names\":[\"A\",\"B\"],"
		"\"type_names\":[\"A\",\"B\"],\"stable_ids\":[1,2],"
		"\"is_freed\":[0,0]},\"bonds\":[]}";
	MoleculeStore dst;
	std::istringstream is(forged);
	bool threw = false;
	try { loadStoreJSON(dst, is); }
	catch (Exception::ParseError&) { threw = true; }
	TEST_EQUAL(threw, true)
RESULT

CHECK(K0.6.3b duplicate stable_id rejected)
	// size=2 with two atoms sharing stable_id=42 -> reject via
	// restore_stable_ids_for_load_ duplicate check.
	std::string forged =
		"{\"document_type\":\"MoleculeStore\",\"format_version\":1,"
		"\"format_minor\":1,\"size\":2,\"live_atom_count\":2,"
		"\"generation\":0,\"atoms\":{"
		"\"positions\":[[0,0,0],[0,0,0]],"
		"\"velocities\":[[0,0,0],[0,0,0]],"
		"\"forces\":[[0,0,0],[0,0,0]],"
		"\"charges\":[0,0],\"radii\":[0,0],\"atom_types\":[0,0],"
		"\"formal_charges\":[0,0],\"element_indices\":[0,0],"
		"\"selection\":[0,0],\"names\":[\"A\",\"B\"],"
		"\"type_names\":[\"A\",\"B\"],\"stable_ids\":[42,42],"
		"\"is_freed\":[0,0]},\"bonds\":[]}";
	MoleculeStore dst;
	std::istringstream is(forged);
	bool threw_parse        = false;
	bool threw_invalid_arg  = false;
	try { loadStoreJSON(dst, is); }
	catch (Exception::ParseError&)     { threw_parse = true; }
	catch (Exception::InvalidArgument&){ threw_invalid_arg = true; }
	// Either typed exception is acceptable (both are clear errors); we
	// just don't want a raw nlohmann/std exception to leak.
	TEST_EQUAL(threw_parse || threw_invalid_arg, true)
RESULT

CHECK(K0.6.3 bond endpoint out-of-range rejected)
	std::string forged =
		"{\"format_version\":1,\"size\":2,\"live_atom_count\":2,"
		"\"generation\":0,\"atoms\":{\"positions\":[[0,0,0],[0,0,0]],"
		"\"velocities\":[[0,0,0],[0,0,0]],\"forces\":[[0,0,0],[0,0,0]],"
		"\"charges\":[0,0],\"radii\":[0,0],\"atom_types\":[0,0],"
		"\"formal_charges\":[0,0],\"element_indices\":[0,0],"
		"\"selection\":[0,0],\"names\":[\"A\",\"B\"],"
		"\"type_names\":[\"A\",\"B\"],\"stable_ids\":[1,2],"
		"\"is_freed\":[0,0]},"
		// bond.b = 5 is past size=2 -> reject
		"\"bonds\":[{\"a\":0,\"b\":5,\"order\":1,\"type\":0,\"flags\":0}]}";
	MoleculeStore dst;
	std::istringstream is(forged);
	bool threw = false;
	try { loadStoreJSON(dst, is); }
	catch (Exception::ParseError&) { threw = true; }
	TEST_EQUAL(threw, true)
RESULT

END_TEST

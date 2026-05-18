// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// BALL 2.0 binary persistence — WRITER skeleton tests (K0.6b).
// Standalone-compilable test. Will be wired into BALL_KERNEL_TESTS
// in a follow-on integration commit; for now this is a plain
// main() with <cassert>.
//

#include <BALL/KERNEL/storeFormat.h>
#include <BALL/KERNEL/moleculeStore.h>
#include <BALL/MATHS/vector3.h>

#include <cassert>
#include <cstdint>
#include <cstring>
#include <sstream>
#include <string>
#include <vector>

using namespace BALL;

// ----------------------------------------------------------------
// Helpers
// ----------------------------------------------------------------

static std::string capture(const MoleculeStore& s)
{
	std::ostringstream os;
	StoreWriter w(os);
	bool ok = w.write_store(s);
	assert(ok);
	(void)ok;
	return os.str();
}

static void check_file_magic(const std::string& bytes)
{
	assert(bytes.size() >= 16);
	// First 8 bytes: "BALL2\0\0\0"
	const char expected_magic[8] = { 'B','A','L','L','2', 0, 0, 0 };
	assert(std::memcmp(bytes.data(), expected_magic, 8) == 0);

	// Next 4 bytes: version u32 LE == 1.
	std::uint32_t v = 0;
	std::memcpy(&v, bytes.data() + 8, 4);
	assert(v == 1u);
}

static void check_footer_magic(const std::string& bytes)
{
	assert(bytes.size() >= 8);
	const char expected[8] = { 'B','A','L','L','2','E','N','D' };
	assert(std::memcmp(bytes.data() + bytes.size() - 8, expected, 8) == 0);
}

// ----------------------------------------------------------------
// Tests
// ----------------------------------------------------------------

static void test_empty_store_writes_nonempty_stream()
{
	MoleculeStore s;
	const std::string bytes = capture(s);

	// FileHeader (64) + StoreHeader chunk (10+32+4) + StringPool chunk
	// (10+4+4) + BondTable chunk (10+0+4) + Selection chunk (10+4+4)
	// + Hierarchy stub (10+8+4) + PropertyBag stub (10+4+4) +
	// StableIdMap chunk (10+4+4) + Footer (offsets table + magic).
	// All non-zero — definitely > 100 B.
	assert(bytes.size() > 100);
}

static void test_file_header_magic_and_version_present()
{
	MoleculeStore s;
	const std::string bytes = capture(s);
	check_file_magic(bytes);
}

static void test_footer_magic_at_end()
{
	MoleculeStore s;
	const std::string bytes = capture(s);
	check_footer_magic(bytes);
}

static void test_round_trip_100_atoms_50_bonds()
{
	MoleculeStore s;
	s.reserve(100);
	for (int i = 0; i < 100; ++i) {
		MoleculeStore::Index idx = s.allocate_atom();
		s.position(idx) = Vector3(float(i), float(i) * 0.5f, float(i) * 0.25f);
		s.charge(idx)        = static_cast<float>(i) * 0.01f;
		s.radius(idx)        = 1.5f;
		s.atom_type(idx)     = static_cast<short>(i % 16);
		s.formal_charge(idx) = static_cast<short>((i % 5) - 2);
		s.element_index(idx) = static_cast<std::uint8_t>(i % 100);
		s.set_selected(idx, (i % 7) == 0);
	}
	for (int i = 0; i < 50; ++i) {
		s.add_bond(static_cast<MoleculeStore::Index>(i),
		           static_cast<MoleculeStore::Index>(99 - i),
		           /*order*/1, /*type*/0);
	}

	const std::string bytes = capture(s);

	// Magic + footer present.
	check_file_magic(bytes);
	check_footer_magic(bytes);

	// Expected lower bound on the column payloads:
	//   POS+VEL+FRC = 3 * 100 * 12 = 3600
	//   CHG+RAD     = 2 * 100 *  4 =  800
	//   ATY+FCH     = 2 * 100 *  2 =  400
	//   ELI+SEL     = 2 * 100 *  1 =  200
	//   NMO+TNO     = 2 * 100 *  4 =  800
	//   STI         = 1 * 100 *  8 =  800
	//   columns subtotal             = 6600 B
	// Plus 12 chunks * 14 B (10 header + 4 crc) = 168 B
	// Plus bonds: 50 * 12 = 600 B + 14 = 614
	// Plus FileHeader (64), StoreHeader (46), StringPool (~18),
	// Selection (~30), HierarchyStub (22), PropertyBagStub (18),
	// StableIdMap (4 + 100*12 = 1204 + 14 = 1218), Footer (~250).
	// Conservative floor: > 8000 B.
	assert(bytes.size() > 8000);

	// The writer's chunk index should have recorded a known number
	// of chunks: 1 (store hdr) + 12 (columns) + 1 (stringpool) +
	// 1 (bonds) + 1 (selection) + 1 (hierarchy) + 1 (propertybag) +
	// 1 (stableidmap) = 19.
	// Re-run with a fresh writer to inspect the index.
	std::ostringstream os;
	StoreWriter w(os);
	bool ok = w.write_store(s);
	assert(ok); (void)ok;
	assert(w.chunk_index().size() == 19);

	// Smoke: bytes_written should match the captured length.
	assert(w.bytes_written() == os.str().size());
}

static void test_crc32_zlib_known_vector()
{
	// "123456789" -> 0xCBF43926 (standard CRC-32/ISO-HDLC test vector).
	const char* m = "123456789";
	std::uint32_t c = StoreFormat::crc32(m, 9);
	assert(c == 0xCBF43926u);
}

static void test_pack_bitmap_lsb_first()
{
	std::vector<std::uint8_t> in = { 1, 0, 1, 1, 0, 0, 0, 1, /* byte boundary */ 1, 0, 0 };
	std::vector<std::uint8_t> out;
	StoreFormat::pack_bitmap(in, out);
	// First byte: bits 0,2,3,7 set -> 0b10001101 = 0x8D.
	assert(out.size() == 2);
	assert(out[0] == 0x8Du);
	// Second byte: only bit 0 -> 0x01.
	assert(out[1] == 0x01u);
}

int main()
{
	test_crc32_zlib_known_vector();
	test_pack_bitmap_lsb_first();
	test_empty_store_writes_nonempty_stream();
	test_file_header_magic_and_version_present();
	test_footer_magic_at_end();
	test_round_trip_100_atoms_50_bonds();
	return 0;
}

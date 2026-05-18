// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// BALL 2.0 binary persistence — WRITER skeleton (K0.6b).
// Implementation of include/BALL/KERNEL/storeFormat.h.
//
// Wire-format reference: .planning/v2.x/STORE-FORMAT-SPEC.md §2.
//

#include <BALL/KERNEL/storeFormat.h>

#include <algorithm>
#include <cstring>
#include <cstdint>

namespace BALL
{

	// ----------------------------------------------------------------
	// CRC-32 (zlib polynomial 0xEDB88320), table-driven.
	// ----------------------------------------------------------------
	namespace {

		struct Crc32Table {
			std::uint32_t t[256];
			Crc32Table() noexcept {
				for (std::uint32_t i = 0; i < 256; ++i) {
					std::uint32_t c = i;
					for (int k = 0; k < 8; ++k) {
						c = (c & 1u) ? (0xEDB88320u ^ (c >> 1)) : (c >> 1);
					}
					t[i] = c;
				}
			}
		};

		const Crc32Table& crc32_table() noexcept {
			static const Crc32Table tbl;
			return tbl;
		}

		// Little-endian POD writers. The host may be big-endian; this
		// build assumes little-endian (Q3 in the spec — BE writer is a
		// follow-on). The writes below would need explicit byte-swap on
		// a BE host; that path is unreachable in CI today.
		inline void put_u8 (std::uint8_t* p, std::uint8_t  v) { p[0] = v; }
		inline void put_u16(std::uint8_t* p, std::uint16_t v) {
			p[0] = static_cast<std::uint8_t>(v       & 0xFF);
			p[1] = static_cast<std::uint8_t>((v>> 8) & 0xFF);
		}
		inline void put_u32(std::uint8_t* p, std::uint32_t v) {
			p[0] = static_cast<std::uint8_t>(v        & 0xFF);
			p[1] = static_cast<std::uint8_t>((v >>  8)& 0xFF);
			p[2] = static_cast<std::uint8_t>((v >> 16)& 0xFF);
			p[3] = static_cast<std::uint8_t>((v >> 24)& 0xFF);
		}
		inline void put_u64(std::uint8_t* p, std::uint64_t v) {
			for (int i = 0; i < 8; ++i) {
				p[i] = static_cast<std::uint8_t>((v >> (8*i)) & 0xFF);
			}
		}
	} // namespace

	std::uint32_t StoreFormat::crc32(const void* data, std::size_t len) noexcept
	{
		const std::uint8_t* p = static_cast<const std::uint8_t*>(data);
		const std::uint32_t* tbl = crc32_table().t;
		std::uint32_t c = 0xFFFFFFFFu;
		for (std::size_t i = 0; i < len; ++i) {
			c = tbl[(c ^ p[i]) & 0xFFu] ^ (c >> 8);
		}
		return c ^ 0xFFFFFFFFu;
	}

	void StoreFormat::pack_bitmap(const std::vector<std::uint8_t>& sel,
	                              std::vector<std::uint8_t>& out)
	{
		const std::size_t n = sel.size();
		out.assign((n + 7) / 8, 0);
		for (std::size_t i = 0; i < n; ++i) {
			if (sel[i]) {
				out[i >> 3] |= static_cast<std::uint8_t>(1u << (i & 7));
			}
		}
	}

	// ----------------------------------------------------------------
	// StoreWriter
	// ----------------------------------------------------------------

	StoreWriter::StoreWriter(std::ostream& os) noexcept
		: os_(os)
	{
	}

	void StoreWriter::write_raw_(const void* data, std::size_t len)
	{
		os_.write(static_cast<const char*>(data), static_cast<std::streamsize>(len));
		bytes_written_ += len;
	}

	void StoreWriter::write_chunk_(std::uint8_t id, std::uint8_t version,
	                               const void* payload, std::uint64_t payload_len)
	{
		const std::uint64_t start_off = bytes_written_;

		// chunk_id u8, version u8, length u64
		std::uint8_t hdr[10];
		put_u8 (hdr + 0, id);
		put_u8 (hdr + 1, version);
		put_u64(hdr + 2, payload_len);
		write_raw_(hdr, sizeof(hdr));

		// payload
		if (payload_len > 0 && payload != nullptr) {
			write_raw_(payload, static_cast<std::size_t>(payload_len));
		}

		// CRC-32 over payload (per task description)
		std::uint8_t crcbuf[4];
		const std::uint32_t crc = StoreFormat::crc32(payload, static_cast<std::size_t>(payload_len));
		put_u32(crcbuf, crc);
		write_raw_(crcbuf, sizeof(crcbuf));

		ChunkIndexEntry e;
		e.id     = id;
		e.offset = start_off;
		e.length = (bytes_written_ - start_off);
		chunk_index_.push_back(e);
	}

	void StoreWriter::write_file_header_(const MoleculeStore& /*s*/)
	{
		std::uint8_t h[StoreFormat::FILE_HEADER_LEN];
		std::memset(h, 0, sizeof(h));

		// [0..8)   magic
		std::memcpy(h + 0, StoreFormat::FILE_MAGIC, 8);
		// [8..12)  version u32
		put_u32(h + 8, StoreFormat::FORMAT_VERSION);
		// [12]     endianness byte
		put_u8 (h + 12, StoreFormat::ENDIAN_LE);
		// [13..15) flags u16 (per task ordering: endianness, flags, header_crc)
		put_u16(h + 13, 0);
		// [15..19) header_crc u32 placeholder (computed below)
		// [19..64) reserved padding (already zero)

		// header_crc covers bytes [0,15) (everything up to the CRC field itself).
		const std::uint32_t crc = StoreFormat::crc32(h, 15);
		put_u32(h + 15, crc);

		write_raw_(h, sizeof(h));
	}

	void StoreWriter::write_store_header_(const MoleculeStore& s)
	{
		// 32-byte payload: atom_count u32, bond_count u32, generation u64,
		// selection_generation u64, stable_id_next u64, flags u32 reserved.
		std::uint8_t buf[32];
		std::memset(buf, 0, sizeof(buf));
		put_u32(buf +  0, static_cast<std::uint32_t>(s.size()));
		put_u32(buf +  4, static_cast<std::uint32_t>(s.live_bond_count()));
		put_u64(buf +  8, static_cast<std::uint64_t>(s.generation()));
		put_u64(buf + 16, static_cast<std::uint64_t>(s.selection_generation()));
		// stable_id_next is not directly exposed; spec calls for the high
		// water mark. We derive it from the columns (max stable_id + 1)
		// because next_stable_id_ is private. For empty stores: 1.
		std::uint64_t next_id = 1;
		for (std::size_t i = 0; i < s.size(); ++i) {
			if (s.stable_id(i) >= next_id) next_id = s.stable_id(i) + 1;
		}
		put_u64(buf + 24, next_id);
		// flags u32 reserved already zero (bytes 28..32 would be flags;
		// only 24..32 left = 8 bytes — last 4 are zero padding/reserved).
		// Layout note: 4+4+8+8+8 = 32; no room for separate flags here,
		// so flags ride alongside future amendments via next_stable_id
		// header growth or a separate chunk.

		write_chunk_(StoreFormat::CHUNK_STORE_HEADER, /*version*/1,
		             buf, sizeof(buf));
	}

	void StoreWriter::write_column_chunks_(const MoleculeStore& s)
	{
		const std::size_t n = s.size();
		if (n == 0) {
			// §4.6: empty stores emit no column chunks.
			return;
		}

		// --- POS (n * 12 B) -------------------------------------------------
		{
			std::vector<std::uint8_t> buf(n * 12);
			for (std::size_t i = 0; i < n; ++i) {
				const Vector3& v = s.position(i);
				const float xyz[3] = {
					static_cast<float>(v.x),
					static_cast<float>(v.y),
					static_cast<float>(v.z)
				};
				std::memcpy(buf.data() + i * 12, xyz, 12);
			}
			write_chunk_(StoreFormat::CHUNK_COL_POS, 1, buf.data(), buf.size());
		}

		// --- VEL ------------------------------------------------------------
		{
			std::vector<std::uint8_t> buf(n * 12);
			for (std::size_t i = 0; i < n; ++i) {
				const Vector3& v = s.velocity(i);
				const float xyz[3] = {
					static_cast<float>(v.x),
					static_cast<float>(v.y),
					static_cast<float>(v.z)
				};
				std::memcpy(buf.data() + i * 12, xyz, 12);
			}
			write_chunk_(StoreFormat::CHUNK_COL_VEL, 1, buf.data(), buf.size());
		}

		// --- FRC ------------------------------------------------------------
		{
			std::vector<std::uint8_t> buf(n * 12);
			for (std::size_t i = 0; i < n; ++i) {
				const Vector3& v = s.force(i);
				const float xyz[3] = {
					static_cast<float>(v.x),
					static_cast<float>(v.y),
					static_cast<float>(v.z)
				};
				std::memcpy(buf.data() + i * 12, xyz, 12);
			}
			write_chunk_(StoreFormat::CHUNK_COL_FRC, 1, buf.data(), buf.size());
		}

		// --- CHG (f32) ------------------------------------------------------
		{
			std::vector<std::uint8_t> buf(n * 4);
			for (std::size_t i = 0; i < n; ++i) {
				const float c = s.charge(i);
				std::memcpy(buf.data() + i * 4, &c, 4);
			}
			write_chunk_(StoreFormat::CHUNK_COL_CHG, 1, buf.data(), buf.size());
		}

		// --- RAD (f32) ------------------------------------------------------
		{
			std::vector<std::uint8_t> buf(n * 4);
			for (std::size_t i = 0; i < n; ++i) {
				const float r = s.radius(i);
				std::memcpy(buf.data() + i * 4, &r, 4);
			}
			write_chunk_(StoreFormat::CHUNK_COL_RAD, 1, buf.data(), buf.size());
		}

		// --- ATY (i16) ------------------------------------------------------
		{
			std::vector<std::uint8_t> buf(n * 2);
			for (std::size_t i = 0; i < n; ++i) {
				put_u16(buf.data() + i * 2, static_cast<std::uint16_t>(s.atom_type(i)));
			}
			write_chunk_(StoreFormat::CHUNK_COL_ATY, 1, buf.data(), buf.size());
		}

		// --- FCH (i16) ------------------------------------------------------
		{
			std::vector<std::uint8_t> buf(n * 2);
			for (std::size_t i = 0; i < n; ++i) {
				put_u16(buf.data() + i * 2, static_cast<std::uint16_t>(s.formal_charge(i)));
			}
			write_chunk_(StoreFormat::CHUNK_COL_FCH, 1, buf.data(), buf.size());
		}

		// --- ELI (u8) -------------------------------------------------------
		{
			std::vector<std::uint8_t> buf(n);
			for (std::size_t i = 0; i < n; ++i) {
				buf[i] = s.element_index(i);
			}
			write_chunk_(StoreFormat::CHUNK_COL_ELI, 1, buf.data(), buf.size());
		}

		// --- SEL raw (u8) — packed bitmap goes in CHUNK_SELECTION instead. -
		// The task description lists "selection (n*1 B)" among the column
		// blobs *and* a separate packed SelectionChunk. We emit the raw
		// u8 column here for symmetry with the in-memory layout; the
		// packed bitmap is written in write_selection_().
		{
			std::vector<std::uint8_t> buf(n);
			for (std::size_t i = 0; i < n; ++i) {
				buf[i] = s.selected(i) ? 1 : 0;
			}
			write_chunk_(StoreFormat::CHUNK_COL_SEL, 1, buf.data(), buf.size());
		}

		// --- NMO (u32) ------------------------------------------------------
		{
			std::vector<std::uint8_t> buf(n * 4);
			for (std::size_t i = 0; i < n; ++i) {
				put_u32(buf.data() + i * 4, s.name_offset(i));
			}
			write_chunk_(StoreFormat::CHUNK_COL_NMO, 1, buf.data(), buf.size());
		}

		// --- TNO (u32) ------------------------------------------------------
		{
			std::vector<std::uint8_t> buf(n * 4);
			for (std::size_t i = 0; i < n; ++i) {
				put_u32(buf.data() + i * 4, s.type_name_offset(i));
			}
			write_chunk_(StoreFormat::CHUNK_COL_TNO, 1, buf.data(), buf.size());
		}

		// --- STI (u64) ------------------------------------------------------
		{
			std::vector<std::uint8_t> buf(n * 8);
			for (std::size_t i = 0; i < n; ++i) {
				put_u64(buf.data() + i * 8, static_cast<std::uint64_t>(s.stable_id(i)));
			}
			write_chunk_(StoreFormat::CHUNK_COL_STI, 1, buf.data(), buf.size());
		}
	}

	void StoreWriter::write_string_pool_(const MoleculeStore& s)
	{
		const std::string& pool = s.string_pool();
		// payload = u32 length + raw bytes
		std::vector<std::uint8_t> buf(4 + pool.size());
		put_u32(buf.data(), static_cast<std::uint32_t>(pool.size()));
		if (!pool.empty()) {
			std::memcpy(buf.data() + 4, pool.data(), pool.size());
		}
		write_chunk_(StoreFormat::CHUNK_STRING_POOL, 1, buf.data(), buf.size());
	}

	void StoreWriter::write_bond_table_(const MoleculeStore& s)
	{
		// Collect live bonds only (skip tombstoned), sort by (min(a,b), max(a,b))
		// per §2.6 determinism rule.
		struct Rec { std::uint32_t a,b; std::uint8_t order,type; std::uint16_t flags; };
		std::vector<Rec> live;
		live.reserve(s.live_bond_count());
		for (std::size_t i = 0; i < s.bond_count(); ++i) {
			if (s.is_bond_dead(static_cast<std::uint32_t>(i))) continue;
			const BondRecord& br = s.bond(static_cast<std::uint32_t>(i));
			Rec r;
			r.a     = std::min(br.a, br.b);
			r.b     = std::max(br.a, br.b);
			r.order = br.order;
			r.type  = br.type;
			r.flags = br.flags;
			live.push_back(r);
		}
		std::sort(live.begin(), live.end(),
			[](const Rec& x, const Rec& y) {
				if (x.a != y.a) return x.a < y.a;
				if (x.b != y.b) return x.b < y.b;
				if (x.order != y.order) return x.order < y.order;
				if (x.type  != y.type ) return x.type  < y.type;
				return x.flags < y.flags;
			});

		// payload: 12 B per bond — u32 a, u32 b, u8 order, u8 type, u16 flags.
		std::vector<std::uint8_t> buf(live.size() * 12);
		for (std::size_t i = 0; i < live.size(); ++i) {
			std::uint8_t* p = buf.data() + i * 12;
			put_u32(p + 0, live[i].a);
			put_u32(p + 4, live[i].b);
			put_u8 (p + 8, live[i].order);
			put_u8 (p + 9, live[i].type);
			put_u16(p +10, live[i].flags);
		}
		write_chunk_(StoreFormat::CHUNK_BOND_TABLE, 1, buf.data(), buf.size());
	}

	void StoreWriter::write_selection_(const MoleculeStore& s)
	{
		const std::size_t n = s.size();
		std::vector<std::uint8_t> raw(n);
		for (std::size_t i = 0; i < n; ++i) {
			raw[i] = s.selected(i) ? 1 : 0;
		}
		std::vector<std::uint8_t> packed;
		StoreFormat::pack_bitmap(raw, packed);
		// payload: u32 n_atoms + packed bitmap bytes.
		std::vector<std::uint8_t> buf(4 + packed.size());
		put_u32(buf.data(), static_cast<std::uint32_t>(n));
		if (!packed.empty()) {
			std::memcpy(buf.data() + 4, packed.data(), packed.size());
		}
		write_chunk_(StoreFormat::CHUNK_SELECTION, 1, buf.data(), buf.size());
	}

	void StoreWriter::write_hierarchy_stub_(const MoleculeStore& /*s*/)
	{
		// K0.6b skeleton: emit a HierarchyChunk with zero children.
		// payload: u32 n_nodes=0, u32 n_children_total=0.
		std::uint8_t buf[8];
		put_u32(buf + 0, 0);
		put_u32(buf + 4, 0);
		write_chunk_(StoreFormat::CHUNK_HIERARCHY, /*version*/0, buf, sizeof(buf));
	}

	void StoreWriter::write_property_bag_stub_(const MoleculeStore& /*s*/)
	{
		// K0.6b skeleton: zero entries.
		std::uint8_t buf[4];
		put_u32(buf, 0);
		write_chunk_(StoreFormat::CHUNK_PROPERTY_BAG, /*version*/0, buf, sizeof(buf));
	}

	void StoreWriter::write_stable_id_map_(const MoleculeStore& s)
	{
		const std::size_t n = s.size();
		// payload: u32 n_entries + n * (u64 stable_id, u32 slot_idx).
		// Layout per task: "n entries, slot_idx -> stable_id" — we
		// emit (slot_idx, stable_id) pairs in slot order so reader can
		// stream-load into a flat array without an extra sort.
		std::vector<std::uint8_t> buf(4 + n * 12);
		put_u32(buf.data(), static_cast<std::uint32_t>(n));
		for (std::size_t i = 0; i < n; ++i) {
			std::uint8_t* p = buf.data() + 4 + i * 12;
			put_u32(p + 0, static_cast<std::uint32_t>(i));
			put_u64(p + 4, static_cast<std::uint64_t>(s.stable_id(i)));
		}
		write_chunk_(StoreFormat::CHUNK_STABLE_ID_MAP, 1, buf.data(), buf.size());
	}

	void StoreWriter::write_footer_()
	{
		// Footer payload: u32 n_chunks + n * (u8 id, u64 offset, u64 length) = 17 B each.
		const std::uint32_t n = static_cast<std::uint32_t>(chunk_index_.size());
		std::vector<std::uint8_t> tbl(4 + std::size_t{n} * 17);
		put_u32(tbl.data(), n);
		for (std::size_t i = 0; i < n; ++i) {
			std::uint8_t* p = tbl.data() + 4 + i * 17;
			put_u8 (p + 0,  chunk_index_[i].id);
			put_u64(p + 1,  chunk_index_[i].offset);
			put_u64(p + 9,  chunk_index_[i].length);
		}

		// Write footer payload + CRC (over the table), then trailing magic.
		write_raw_(tbl.data(), tbl.size());

		std::uint8_t crcbuf[4];
		put_u32(crcbuf, StoreFormat::crc32(tbl.data(), tbl.size()));
		write_raw_(crcbuf, sizeof(crcbuf));

		write_raw_(StoreFormat::FOOTER_MAGIC, 8);
	}

	bool StoreWriter::write_store(const MoleculeStore& s)
	{
		write_file_header_(s);
		write_store_header_(s);
		write_column_chunks_(s);
		write_string_pool_(s);
		write_bond_table_(s);
		write_selection_(s);
		write_hierarchy_stub_(s);
		write_property_bag_stub_(s);
		write_stable_id_map_(s);
		write_footer_();
		return os_.good();
	}

} // namespace BALL

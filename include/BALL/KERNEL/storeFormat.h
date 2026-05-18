// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// BALL 2.0 binary persistence — WRITER skeleton (K0.6b).
// See .planning/v2.x/STORE-FORMAT-SPEC.md for the wire-format contract.
//
// This header declares the writer-only API. The reader (and the
// ball-convert-v1-v2 tool that drives it) ship as separate
// follow-on commits in K0.6b.
//

#ifndef BALL_KERNEL_STOREFORMAT_H
#define BALL_KERNEL_STOREFORMAT_H

#ifndef BALL_KERNEL_MOLECULESTORE_H
# include <BALL/KERNEL/moleculeStore.h>
#endif

#ifndef BALL_MATHS_VECTOR3_H
# include <BALL/MATHS/vector3.h>
#endif

#include <cstdint>
#include <ostream>
#include <string>
#include <vector>

namespace BALL
{

	/**	StoreFormat — wire-format constants for the BALL 2.0 binary
			persistence format (".bf2").

			Every constant here is a normative byte-level value of the
			on-disk format and must agree with
			.planning/v2.x/STORE-FORMAT-SPEC.md §2. If a constant moves,
			format_minor must bump.
	*/
	struct BALL_EXPORT StoreFormat
	{
		// FileHeader magic — 8 bytes, ASCII "BALL2" + 3 NULs.
		static constexpr char         FILE_MAGIC[8]   = { 'B','A','L','L','2', 0, 0, 0 };
		static constexpr std::uint32_t FORMAT_VERSION  = 1;        // wire-format version (caller spec uses u32=1)
		static constexpr std::uint8_t  ENDIAN_LE       = 0x01;
		static constexpr std::size_t   FILE_HEADER_LEN = 64;

		// Footer magic — 8 bytes, ASCII "BALL2END".
		static constexpr char         FOOTER_MAGIC[8] = { 'B','A','L','L','2','E','N','D' };

		// Chunk IDs. The user-task header uses a single u8 chunk_id; we
		// allocate one ID per chunk kind here. (The spec also defines
		// 4-byte FourCC tags per chunk for the long-form on-disk
		// representation; the skeleton uses the compact u8 form per
		// the task description, with the FourCC values kept here for
		// reference and follow-on alignment.)
		enum ChunkId : std::uint8_t {
			CHUNK_STORE_HEADER  = 0x01,
			CHUNK_COL_POS       = 0x10,
			CHUNK_COL_VEL       = 0x11,
			CHUNK_COL_FRC       = 0x12,
			CHUNK_COL_CHG       = 0x13,
			CHUNK_COL_RAD       = 0x14,
			CHUNK_COL_ATY       = 0x15,
			CHUNK_COL_FCH       = 0x16,
			CHUNK_COL_ELI       = 0x17,
			CHUNK_COL_SEL       = 0x18,   // raw u8 column (not the packed bitmap)
			CHUNK_COL_NMO       = 0x19,
			CHUNK_COL_TNO       = 0x1A,
			CHUNK_COL_STI       = 0x1B,
			CHUNK_STRING_POOL   = 0x20,
			CHUNK_BOND_TABLE    = 0x30,
			CHUNK_SELECTION     = 0x40,   // packed bitmap, n bits LSB-first
			CHUNK_HIERARCHY     = 0x50,
			CHUNK_PROPERTY_BAG  = 0x60,
			CHUNK_STABLE_ID_MAP = 0x70
		};

		// CRC-32 (zlib polynomial 0xEDB88320), one-shot over a buffer.
		// Returned value is *not* finalised-inverted: matches the
		// canonical CRC-32 output (e.g. zlib's crc32()).
		static std::uint32_t crc32(const void* data, std::size_t len) noexcept;

		// Pack a uint8 boolean column into an LSB-first bitmap.
		// out is resized to ceil(n/8); bit i = (sel[i] != 0).
		static void pack_bitmap(const std::vector<std::uint8_t>& sel,
		                        std::vector<std::uint8_t>& out);
	};

	/**	StoreWriter — write a single MoleculeStore to a std::ostream
			in the v2.0 binary format.

			Usage:
			    std::ofstream f("mol.bf2", std::ios::binary);
			    StoreWriter w(f);
			    w.write_store(store);

			Streaming, single forward pass. The footer's chunk-offset
			table is built up in memory as chunks are emitted; on
			write_store completion the footer is written and the
			FileHeader.footer_offset is back-patched only if the stream
			is seekable. For non-seekable streams the footer's absolute
			offset can still be derived by the reader from the
			trailing "BALL2END" magic.
	*/
	class BALL_EXPORT StoreWriter
	{
		public:

		struct ChunkIndexEntry {
			std::uint8_t  id;
			std::uint64_t offset;
			std::uint64_t length;
		};

		explicit StoreWriter(std::ostream& os) noexcept;
		~StoreWriter() = default;

		StoreWriter(const StoreWriter&)            = delete;
		StoreWriter& operator=(const StoreWriter&) = delete;

		// Write a complete file: FileHeader + one Store section + Footer.
		// Returns true on success (stream still good() at end).
		bool write_store(const MoleculeStore& s);

		// Total bytes written so far (running counter).
		std::uint64_t bytes_written() const noexcept { return bytes_written_; }

		const std::vector<ChunkIndexEntry>& chunk_index() const noexcept { return chunk_index_; }

		private:

		// Low-level raw write; updates bytes_written_.
		void write_raw_(const void* data, std::size_t len);

		// Write a chunk with the uniform header documented in the task:
		//   chunk_id u8, version u8, length u64, payload[length], crc32 u32
		// Records an entry in chunk_index_ pointing at the chunk's
		// start offset (the byte of chunk_id).
		void write_chunk_(std::uint8_t id, std::uint8_t version,
		                  const void* payload, std::uint64_t payload_len);

		// Section emitters.
		void write_file_header_(const MoleculeStore& s);
		void write_store_header_(const MoleculeStore& s);
		void write_column_chunks_(const MoleculeStore& s);
		void write_string_pool_(const MoleculeStore& s);
		void write_bond_table_(const MoleculeStore& s);
		void write_selection_(const MoleculeStore& s);
		void write_hierarchy_stub_(const MoleculeStore& s);
		void write_property_bag_stub_(const MoleculeStore& s);
		void write_stable_id_map_(const MoleculeStore& s);
		void write_footer_();

		std::ostream&                  os_;
		std::uint64_t                  bytes_written_ = 0;
		std::vector<ChunkIndexEntry>   chunk_index_;
	};

} // namespace BALL

#endif // BALL_KERNEL_STOREFORMAT_H

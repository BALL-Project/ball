// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// v1.7.2 build acceleration — hand-rolled Ward cluster-tree serializer.
//
// This translation unit used to instantiate four boost::serialization archive
// flavors (binary/text x in/out) over the whole boost::graph adjacency_list via
// the boost graph adjacency-list serialization header. boost::serialization is
// the single heaviest Boost
// component to compile and dominated the ~57-min MSVC compile of
// poseClustering.C. The data the Ward tree actually holds is trivial, so we now
// emit it with a small hand-rolled, magic+version-headed format and no longer
// instantiate any boost archive. This drops the TU's compile from tens of
// minutes to ~seconds and removes the boost::serialization dependency entirely.
//
// On-disk format (magic "BALLWARD" + version 2):
//   header  : magic + version, then in the BINARY variant a width/byte-order
//             descriptor (sizeof(Size), sizeof(Index), sizeof(float) and a
//             byte-order marker word) so a build/arch mismatch is detected and
//             rejected rather than silently decoding corrupt data. In the TEXT
//             variant the header is the printable token line "BALLWARD 2\n".
//   payload : (1) graph order = num_vertices(cluster_tree_)
//             (2) root vertex index = cluster_tree_[boost::graph_bundle]
//             (3) per vertex v in [0, order): poses (count + each Index),
//                 Size size, float merged_at  (center is NOT serialized — this
//                 matches the previous boost payload exactly)
//             (4) edge list: edge count, then each (parent, child) index pair
//
// PORTABILITY: the BINARY variant writes raw, native-width, native-byte-order
// values. It is therefore little-endian-only and width-specific by design; it is
// NOT a portable cross-architecture format. The header records the assumed byte
// widths and a byte-order marker, and the reader throws Exception::InvalidFormat
// on any mismatch (no byte-swapping is attempted — no supported BALL platform is
// big-endian). The TEXT variant (the default) is portable and writes floats at
// full round-trip precision.
//
// FORMAT BREAK: old boost-serialized .ward/.dat trees will NOT load — the magic
// header check rejects them loudly via BALL::Exception::InvalidFormat. See
// .planning/RELEASE-NOTES-v1.7.md (v1.7.2 Breaking changes).
//

#include <BALL/DOCKING/COMMON/poseClustering.h>

#include <BALL/COMMON/exception.h>

#include <boost/graph/adjacency_list.hpp>

#include <cstdint>
#include <ios>
#include <limits>
#include <string>

using namespace std;

namespace BALL
{
	namespace
	{
		// Magic + version identifying the hand-rolled Ward cluster-tree format.
		static const char  POSECLUSTERING_WARD_MAGIC[] = "BALLWARD";
		static const Size  POSECLUSTERING_WARD_MAGIC_LEN = 8; // strlen("BALLWARD"), without the trailing NUL
		// Version 2: binary header now records the assumed byte widths + a
		// byte-order marker so a build/arch mismatch fails loudly (WR-02).
		static const unsigned int POSECLUSTERING_WARD_VERSION = 2;

		// Byte-order marker written little-endian into the binary header. A reader
		// on a host that decodes the four raw bytes 0x01020304 as anything other
		// than this value has a differing endianness and must reject the stream.
		static const uint32_t POSECLUSTERING_WARD_BYTE_ORDER_MARK = 0x01020304u;

		// Sanity ceiling on count words read from a (possibly corrupt/hostile)
		// stream, used before any per-element loop or allocation. A real Ward tree
		// over a docking run never approaches this; an absurd/corrupt count word
		// (e.g. a byte-swapped or garbage value) is rejected loudly instead of
		// driving an unbounded allocation/hang (CR-01).
		static const Size POSECLUSTERING_WARD_MAX_COUNT = 100000000u; // 1e8

		// ---- raw binary helpers (native width, native byte order) --------------
		// NOTE: these write/read in host byte order at the native sizeof(T). The
		// binary format is little-endian-only and width-specific by design; the
		// header's width/byte-order descriptor lets the reader detect a mismatch
		// and throw rather than silently decode corrupt data (see WR-01/WR-02).

		template <typename T>
		inline void writeBinary_(std::ostream& out, const T& value)
		{
			out.write(reinterpret_cast<const char*>(&value), sizeof(T));
		}

		template <typename T>
		inline void readBinary_(std::istream& in, T& value)
		{
			in.read(reinterpret_cast<char*>(&value), sizeof(T));
		}
	}

	void PoseClustering::serializeWardClusterTree(std::ostream& out, bool binary)
	{
		const unsigned int root = cluster_tree_[boost::graph_bundle];
		const Size order = (Size)boost::num_vertices(cluster_tree_);

		if (binary)
		{
			// Header: raw magic bytes + raw version.
			out.write(POSECLUSTERING_WARD_MAGIC, POSECLUSTERING_WARD_MAGIC_LEN);
			writeBinary_(out, POSECLUSTERING_WARD_VERSION);

			// Width/byte-order descriptor (version 2): record the assumed byte
			// widths and a byte-order marker so a build/arch mismatch is detected
			// and rejected on read instead of silently decoding corrupt data.
			const unsigned char size_w  = (unsigned char)sizeof(Size);
			const unsigned char index_w = (unsigned char)sizeof(Index);
			const unsigned char float_w = (unsigned char)sizeof(float);
			writeBinary_(out, size_w);
			writeBinary_(out, index_w);
			writeBinary_(out, float_w);
			writeBinary_(out, POSECLUSTERING_WARD_BYTE_ORDER_MARK);

			writeBinary_(out, order);
			writeBinary_(out, root);

			for (Size v = 0; v < order; ++v)
			{
				const ClusterProperties& props = cluster_tree_[v];

				const Size num_poses = (Size)props.poses.size();
				writeBinary_(out, num_poses);
				for (std::set<Index>::const_iterator it = props.poses.begin(); it != props.poses.end(); ++it)
				{
					const Index pose = *it;
					writeBinary_(out, pose);
				}

				writeBinary_(out, props.size);
				writeBinary_(out, props.merged_at);
			}

			const Size num_edges = (Size)boost::num_edges(cluster_tree_);
			writeBinary_(out, num_edges);

			boost::graph_traits<ClusterTree>::edge_iterator e, e_end;
			for (boost::tie(e, e_end) = boost::edges(cluster_tree_); e != e_end; ++e)
			{
				const unsigned int parent = (unsigned int)boost::source(*e, cluster_tree_);
				const unsigned int child  = (unsigned int)boost::target(*e, cluster_tree_);
				writeBinary_(out, parent);
				writeBinary_(out, child);
			}
		}
		else
		{
			// Emit floats at full round-trip precision so the text round-trip is
			// exact: a 32-bit float needs max_digits10 (== 9) significant decimal
			// digits to recover its bits. merged_at drives the threshold
			// comparisons in extractClustersForThreshold and feeds getClusterScore,
			// so default 6-digit precision could flip a partition (WR-03).
			const std::streamsize old_prec = out.precision(std::numeric_limits<float>::max_digits10);

			// Header: printable ASCII token line so the fixture stays ASCII text.
			out << POSECLUSTERING_WARD_MAGIC << ' ' << POSECLUSTERING_WARD_VERSION << '\n';

			out << order << ' ' << root << '\n';

			for (Size v = 0; v < order; ++v)
			{
				const ClusterProperties& props = cluster_tree_[v];

				out << props.poses.size();
				for (std::set<Index>::const_iterator it = props.poses.begin(); it != props.poses.end(); ++it)
				{
					out << ' ' << *it;
				}
				out << ' ' << props.size << ' ' << props.merged_at << '\n';
			}

			const Size num_edges = (Size)boost::num_edges(cluster_tree_);
			out << num_edges << '\n';

			boost::graph_traits<ClusterTree>::edge_iterator e, e_end;
			for (boost::tie(e, e_end) = boost::edges(cluster_tree_); e != e_end; ++e)
			{
				const unsigned int parent = (unsigned int)boost::source(*e, cluster_tree_);
				const unsigned int child  = (unsigned int)boost::target(*e, cluster_tree_);
				out << parent << ' ' << child << '\n';
			}

			out.precision(old_prec);
		}

		// Surface a write failure (disk full, broken pipe, ...) at serialization
		// time rather than as a confusing deserialize failure later (IN-03).
		if (!out)
		{
			throw Exception::InvalidFormat(__FILE__, __LINE__,
				"PoseClustering Ward tree: output stream failed during serialization.");
		}
	}

	void PoseClustering::deserializeWardClusterTree(std::istream& in, bool binary)
	{
		// Start from a clean tree so a reused PoseClustering object does not
		// accumulate stale vertices.
		cluster_tree_ = ClusterTree();

		Size order = 0;
		unsigned int root = 0;

		Size num_edges = 0;

		if (binary)
		{
			// Verify magic + version.
			char magic[POSECLUSTERING_WARD_MAGIC_LEN] = {};
			in.read(magic, POSECLUSTERING_WARD_MAGIC_LEN);
			if (   !in
			    || std::string(magic, POSECLUSTERING_WARD_MAGIC_LEN) != std::string(POSECLUSTERING_WARD_MAGIC, POSECLUSTERING_WARD_MAGIC_LEN))
			{
				throw Exception::InvalidFormat(__FILE__, __LINE__,
					String("PoseClustering Ward tree: bad magic — expected '") + POSECLUSTERING_WARD_MAGIC
					+ "' (binary format). The file is not a v1.7.2+ BALLWARD tree (old boost-serialized files are no longer supported).");
			}

			unsigned int version = 0;
			readBinary_(in, version);
			if (!in)
			{
				throw Exception::InvalidFormat(__FILE__, __LINE__,
					"PoseClustering Ward tree: truncated stream while reading format version (binary).");
			}
			if (version != POSECLUSTERING_WARD_VERSION)
			{
				throw Exception::InvalidFormat(__FILE__, __LINE__,
					String("PoseClustering Ward tree: unsupported format version ") + String(version)
					+ " (expected " + String(POSECLUSTERING_WARD_VERSION) + ").");
			}

			// Width / byte-order descriptor (version 2). Reject a build/arch
			// mismatch loudly instead of silently decoding corrupt data (WR-02).
			unsigned char size_w = 0, index_w = 0, float_w = 0;
			readBinary_(in, size_w);
			readBinary_(in, index_w);
			readBinary_(in, float_w);
			uint32_t byte_order = 0;
			readBinary_(in, byte_order);
			if (!in)
			{
				throw Exception::InvalidFormat(__FILE__, __LINE__,
					"PoseClustering Ward tree: truncated stream while reading width/byte-order header (binary).");
			}
			if (   size_w  != (unsigned char)sizeof(Size)
			    || index_w != (unsigned char)sizeof(Index)
			    || float_w != (unsigned char)sizeof(float))
			{
				throw Exception::InvalidFormat(__FILE__, __LINE__,
					String("PoseClustering Ward tree: binary type-width mismatch (file sizeof Size/Index/float = ")
					+ String((int)size_w) + "/" + String((int)index_w) + "/" + String((int)float_w)
					+ ", this build = " + String((int)sizeof(Size)) + "/" + String((int)sizeof(Index)) + "/" + String((int)sizeof(float))
					+ "). The binary variant is not portable across builds/architectures; re-export with the text variant.");
			}
			if (byte_order != POSECLUSTERING_WARD_BYTE_ORDER_MARK)
			{
				throw Exception::InvalidFormat(__FILE__, __LINE__,
					"PoseClustering Ward tree: binary byte-order mismatch — the file was written on a host with a different endianness (no byte-swapping is supported; re-export with the text variant).");
			}

			readBinary_(in, order);
			readBinary_(in, root);
			if (!in)
			{
				throw Exception::InvalidFormat(__FILE__, __LINE__,
					"PoseClustering Ward tree: truncated header (could not read order/root) (binary).");
			}
			if (order > POSECLUSTERING_WARD_MAX_COUNT)
			{
				throw Exception::InvalidFormat(__FILE__, __LINE__,
					String("PoseClustering Ward tree: implausible graph order ") + String(order)
					+ " exceeds sanity ceiling (corrupt stream?).");
			}

			for (Size v = 0; v < order; ++v)
			{
				ClusterTreeNode node = boost::add_vertex(cluster_tree_);
				ClusterProperties& props = cluster_tree_[node];

				Size num_poses = 0;
				readBinary_(in, num_poses);
				if (!in)
				{
					throw Exception::InvalidFormat(__FILE__, __LINE__,
						"PoseClustering Ward tree: truncated stream while reading vertex pose count (binary).");
				}
				// A vertex's pose set cannot exceed the total number of leaves,
				// which is bounded by the graph order; reject an absurd count
				// before the insert loop (CR-01).
				if (num_poses > order)
				{
					throw Exception::InvalidFormat(__FILE__, __LINE__,
						String("PoseClustering Ward tree: vertex pose count ") + String(num_poses)
						+ " exceeds graph order " + String(order) + " (corrupt stream?).");
				}
				for (Size p = 0; p < num_poses; ++p)
				{
					Index pose = 0;
					readBinary_(in, pose);
					if (!in)
					{
						throw Exception::InvalidFormat(__FILE__, __LINE__,
							"PoseClustering Ward tree: truncated stream while reading vertex pose (binary).");
					}
					props.poses.insert(pose);
				}

				readBinary_(in, props.size);
				readBinary_(in, props.merged_at);
				if (!in)
				{
					throw Exception::InvalidFormat(__FILE__, __LINE__,
						"PoseClustering Ward tree: truncated stream in vertex payload (binary).");
				}
			}

			readBinary_(in, num_edges);
			if (!in)
			{
				throw Exception::InvalidFormat(__FILE__, __LINE__,
					"PoseClustering Ward tree: truncated stream before edge list (binary).");
			}
			if (num_edges > POSECLUSTERING_WARD_MAX_COUNT)
			{
				throw Exception::InvalidFormat(__FILE__, __LINE__,
					String("PoseClustering Ward tree: implausible edge count ") + String(num_edges)
					+ " exceeds sanity ceiling (corrupt stream?).");
			}
			for (Size i = 0; i < num_edges; ++i)
			{
				unsigned int parent = 0;
				unsigned int child  = 0;
				readBinary_(in, parent);
				readBinary_(in, child);
				if (!in || parent >= order || child >= order)
				{
					throw Exception::InvalidFormat(__FILE__, __LINE__,
						"PoseClustering Ward tree: edge references out-of-range vertex or stream truncated (binary).");
				}
				boost::add_edge(parent, child, cluster_tree_);
			}
		}
		else
		{
			// Verify magic + version (ASCII token line).
			std::string magic;
			unsigned int version = 0;
			in >> magic >> version;
			// Check the stream first so a malformed/truncated header is reported
			// as such rather than as a misleading "unsupported version 0" (WR-04).
			if (!in)
			{
				throw Exception::InvalidFormat(__FILE__, __LINE__,
					"PoseClustering Ward tree: truncated or unparsable header (could not read magic/version) (text).");
			}
			if (magic != std::string(POSECLUSTERING_WARD_MAGIC))
			{
				throw Exception::InvalidFormat(__FILE__, __LINE__,
					String("PoseClustering Ward tree: bad magic — expected '") + POSECLUSTERING_WARD_MAGIC
					+ "' (text format). The file is not a v1.7.2+ BALLWARD tree (old boost-serialized files are no longer supported).");
			}
			if (version != POSECLUSTERING_WARD_VERSION)
			{
				throw Exception::InvalidFormat(__FILE__, __LINE__,
					String("PoseClustering Ward tree: unsupported format version ") + String(version)
					+ " (expected " + String(POSECLUSTERING_WARD_VERSION) + ").");
			}

			in >> order >> root;
			if (!in)
			{
				throw Exception::InvalidFormat(__FILE__, __LINE__,
					"PoseClustering Ward tree: truncated header (could not read order/root) (text).");
			}
			if (order > POSECLUSTERING_WARD_MAX_COUNT)
			{
				throw Exception::InvalidFormat(__FILE__, __LINE__,
					String("PoseClustering Ward tree: implausible graph order ") + String(order)
					+ " exceeds sanity ceiling (corrupt stream?).");
			}

			for (Size v = 0; v < order; ++v)
			{
				ClusterTreeNode node = boost::add_vertex(cluster_tree_);
				ClusterProperties& props = cluster_tree_[node];

				Size num_poses = 0;
				in >> num_poses;
				if (!in)
				{
					throw Exception::InvalidFormat(__FILE__, __LINE__,
						"PoseClustering Ward tree: truncated stream while reading vertex pose count (text).");
				}
				if (num_poses > order)
				{
					throw Exception::InvalidFormat(__FILE__, __LINE__,
						String("PoseClustering Ward tree: vertex pose count ") + String(num_poses)
						+ " exceeds graph order " + String(order) + " (corrupt stream?).");
				}
				for (Size p = 0; p < num_poses; ++p)
				{
					Index pose = 0;
					in >> pose;
					if (!in)
					{
						throw Exception::InvalidFormat(__FILE__, __LINE__,
							"PoseClustering Ward tree: truncated stream while reading vertex pose (text).");
					}
					props.poses.insert(pose);
				}

				in >> props.size >> props.merged_at;
				if (!in)
				{
					throw Exception::InvalidFormat(__FILE__, __LINE__,
						"PoseClustering Ward tree: truncated stream in vertex payload (text).");
				}
			}

			in >> num_edges;
			if (!in)
			{
				throw Exception::InvalidFormat(__FILE__, __LINE__,
					"PoseClustering Ward tree: truncated stream before edge list (text).");
			}
			if (num_edges > POSECLUSTERING_WARD_MAX_COUNT)
			{
				throw Exception::InvalidFormat(__FILE__, __LINE__,
					String("PoseClustering Ward tree: implausible edge count ") + String(num_edges)
					+ " exceeds sanity ceiling (corrupt stream?).");
			}
			for (Size i = 0; i < num_edges; ++i)
			{
				unsigned int parent = 0;
				unsigned int child  = 0;
				in >> parent >> child;
				if (!in || parent >= order || child >= order)
				{
					throw Exception::InvalidFormat(__FILE__, __LINE__,
						"PoseClustering Ward tree: edge references out-of-range vertex or stream truncated (text).");
				}
				boost::add_edge(parent, child, cluster_tree_);
			}
		}

		// The root must reference a real vertex. An out-of-range root would later
		// index past the vertex set in extraction/traversal (CR-01). An empty tree
		// (order == 0) carries no valid root, which is also rejected.
		if (root >= order)
		{
			throw Exception::InvalidFormat(__FILE__, __LINE__,
				String("PoseClustering Ward tree: root index ") + String(root)
				+ " is out of range for a tree of order " + String(order) + " (corrupt stream?).");
		}

		// Because ClusterTree uses vecS for vertices, add_vertex returns
		// descriptors 0..order-1 in order, so the stored integer indices map
		// directly onto vertex descriptors — no remapping is necessary.
		cluster_tree_[boost::graph_bundle] = root;
	}
} // namespace BALL

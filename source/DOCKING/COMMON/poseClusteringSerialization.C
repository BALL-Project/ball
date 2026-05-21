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
// On-disk format (magic "BALLWARD" + version 1):
//   header  : magic + version (raw bytes in the binary variant, the ASCII token
//             line "BALLWARD 1\n" in the text variant)
//   payload : (1) graph order = num_vertices(cluster_tree_)
//             (2) root vertex index = cluster_tree_[boost::graph_bundle]
//             (3) per vertex v in [0, order): poses (count + each Index),
//                 Size size, float merged_at  (center is NOT serialized — this
//                 matches the previous boost payload exactly)
//             (4) edge list: edge count, then each (parent, child) index pair
//
// FORMAT BREAK: old boost-serialized .ward/.dat trees will NOT load — the magic
// header check rejects them loudly via BALL::Exception::InvalidFormat. See
// .planning/RELEASE-NOTES-v1.7.md (v1.7.2 Breaking changes).
//

#include <BALL/DOCKING/COMMON/poseClustering.h>

#include <BALL/COMMON/exception.h>

#include <boost/graph/adjacency_list.hpp>

#include <string>

using namespace std;

namespace BALL
{
	namespace
	{
		// Magic + version identifying the hand-rolled Ward cluster-tree format.
		static const char  POSECLUSTERING_WARD_MAGIC[] = "BALLWARD";
		static const Size  POSECLUSTERING_WARD_MAGIC_LEN = 8; // strlen("BALLWARD"), without the trailing NUL
		static const unsigned int POSECLUSTERING_WARD_VERSION = 1;

		// ---- raw binary helpers (fixed-width, host byte order) -----------------

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
		}
	}

	void PoseClustering::deserializeWardClusterTree(std::istream& in, bool binary)
	{
		// Start from a clean tree so a reused PoseClustering object does not
		// accumulate stale vertices.
		cluster_tree_ = ClusterTree();

		Size order = 0;
		unsigned int root = 0;

		if (binary)
		{
			// Verify magic + version.
			char magic[16];
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
			if (version != POSECLUSTERING_WARD_VERSION)
			{
				throw Exception::InvalidFormat(__FILE__, __LINE__,
					String("PoseClustering Ward tree: unsupported format version ") + String(version)
					+ " (expected " + String(POSECLUSTERING_WARD_VERSION) + ").");
			}

			readBinary_(in, order);
			readBinary_(in, root);

			for (Size v = 0; v < order; ++v)
			{
				ClusterTreeNode node = boost::add_vertex(cluster_tree_);
				ClusterProperties& props = cluster_tree_[node];

				Size num_poses = 0;
				readBinary_(in, num_poses);
				for (Size p = 0; p < num_poses; ++p)
				{
					Index pose = 0;
					readBinary_(in, pose);
					props.poses.insert(pose);
				}

				readBinary_(in, props.size);
				readBinary_(in, props.merged_at);
			}

			Size num_edges = 0;
			readBinary_(in, num_edges);
			for (Size i = 0; i < num_edges; ++i)
			{
				unsigned int parent = 0;
				unsigned int child  = 0;
				readBinary_(in, parent);
				readBinary_(in, child);
				boost::add_edge(parent, child, cluster_tree_);
			}
		}
		else
		{
			// Verify magic + version (ASCII token line).
			std::string magic;
			unsigned int version = 0;
			in >> magic >> version;
			if (!in || magic != std::string(POSECLUSTERING_WARD_MAGIC))
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

			for (Size v = 0; v < order; ++v)
			{
				ClusterTreeNode node = boost::add_vertex(cluster_tree_);
				ClusterProperties& props = cluster_tree_[node];

				Size num_poses = 0;
				in >> num_poses;
				for (Size p = 0; p < num_poses; ++p)
				{
					Index pose = 0;
					in >> pose;
					props.poses.insert(pose);
				}

				in >> props.size >> props.merged_at;
			}

			Size num_edges = 0;
			in >> num_edges;
			for (Size i = 0; i < num_edges; ++i)
			{
				unsigned int parent = 0;
				unsigned int child  = 0;
				in >> parent >> child;
				boost::add_edge(parent, child, cluster_tree_);
			}
		}

		// Because ClusterTree uses vecS for vertices, add_vertex returns
		// descriptors 0..order-1 in order, so the stored integer indices map
		// directly onto vertex descriptors — no remapping is necessary.
		cluster_tree_[boost::graph_bundle] = root;
	}
} // namespace BALL

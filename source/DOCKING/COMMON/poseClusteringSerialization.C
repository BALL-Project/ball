// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// v1.7.2 build acceleration — boost::serialization of the Ward cluster tree
// is quarantined in its own translation unit.
//
// boost::serialization (here: four archive flavors — binary/text x in/out —
// instantiated over the whole boost::graph adjacency_list via
// adj_list_serialize.hpp) is the single heaviest Boost component to compile and
// dominated the ~57-min MSVC compile of poseClustering.C. Moving it here keeps
// the bulk of poseClustering.C (the clustering algorithms) free of the archive
// template instantiation, lets the three DOCKING TUs compile in parallel, and
// lets ccache cache this stable serialization code independently of edits to
// the clustering logic. Behaviour and the on-disk format are UNCHANGED — this
// is a pure relocation (the serialize/deserialize round-trip in
// test/PoseClustering_test3.C still passes byte-for-byte).
//

#include <BALL/DOCKING/COMMON/poseClustering.h>

#include <boost/version.hpp>
#include <boost/graph/iteration_macros.hpp>
#include <boost/graph/adj_list_serialize.hpp>

#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

#include <boost/serialization/set.hpp>

using namespace std;

namespace BALL
{
	template <class Archive>
	void PoseClustering::ClusterProperties::serialize(Archive& ar, const unsigned int /*version*/)
	{
		ar & poses;
		ar & size;
		//		TODO: handle serialization of eigen matrix
//		ar & center;
		ar & merged_at;
	}

	void PoseClustering::serializeWardClusterTree(std::ostream& out, bool binary)
	{
		if (binary)
		{
			boost::archive::binary_oarchive oa(out);
			oa << cluster_tree_;
		}
		else
		{
			boost::archive::text_oarchive oa(out);
			oa << cluster_tree_;
		}
	}

	void PoseClustering::deserializeWardClusterTree(std::istream& in, bool binary)
	{
		if (binary)
		{
			boost::archive::binary_iarchive ia(in);
			ia >> cluster_tree_;
		}
		else
		{
			boost::archive::text_iarchive ia(in);
			ia >> cluster_tree_;
		}

		// unfortunately, old versions of boost don't serialize / deserialize graph_bundle properties... *sigh*
		// so, be nice to them...
#if BOOST_VERSION < 105100
		// iterate over all nodes in the graph and find the one which has no in_edges
		// this would be much simpler if boost would store in_degrees for directed graphs... we don't want a bidirectional graph, though
		HashMap<ClusterTreeNode, Size> in_degrees;
		BGL_FORALL_VERTICES(current_vertex, cluster_tree_, ClusterTree)
		{
			boost::graph_traits<ClusterTree>::out_edge_iterator e, e_end;

			for (boost::tie(e, e_end) = boost::out_edges(current_vertex, cluster_tree_); e != e_end; ++e)
			{
				ClusterTreeNode target = boost::target(*e, cluster_tree_);
				if (in_degrees.find(target) == in_degrees.end())
				{
					in_degrees[target] = 1;
				}
				else
				{
					in_degrees[target]++;
				}
			}
		}

		// now, iterate over the tree again to find the one node without a parent
		BGL_FORALL_VERTICES(current_vertex, cluster_tree_, ClusterTree)
		{
			if (in_degrees[current_vertex] == 0)
			{
				cluster_tree_[boost::graph_bundle] = current_vertex;
				break;
			}
		}
#endif

	}
} // namespace BALL

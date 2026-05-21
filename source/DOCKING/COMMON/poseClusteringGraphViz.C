// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// v1.7.2 build acceleration — the GraphViz export of the Ward cluster tree is
// quarantined in its own translation unit.
//
// boost::write_graphviz instantiates a large template over the
// boost::graph adjacency_list; keeping it out of poseClustering.C removes that
// expansion from the main clustering TU (the three DOCKING TUs then compile in
// parallel and ccache caches this stable code separately). Behaviour and the
// emitted DOT output are UNCHANGED — pure relocation (see
// test/PoseClustering_test1.C / _test3.C exportWardClusterTreeToGraphViz).
//

#include <BALL/DOCKING/COMMON/poseClustering.h>

#include <boost/graph/graphviz.hpp>

using namespace std;

namespace BALL
{
	void PoseClustering::ClusterTreeWriter_::operator() (std::ostream& out, const ClusterTreeNode& v) const
	{
		out << "[label=\"";
		if ((*cluster_tree_)[v].poses.size() > 0)
			out << *((*cluster_tree_)[v].poses.begin());
		else
		{
			out << (*cluster_tree_)[v].merged_at;
#ifdef POSECLUSTERING_DEBUG
		  out	<< "\t" << (*cluster_tree_)[v].current_cluster_id;
#endif
		}
		out << "\"]";
	}

	void PoseClustering::exportWardClusterTreeToGraphViz(std::ostream& out)
	{
		boost::write_graphviz(out, cluster_tree_, ClusterTreeWriter_(&cluster_tree_));
	}
} // namespace BALL

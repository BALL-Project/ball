#include <BALL/DATATYPE/GRAPH/treeWidth.h>

#include <BALL/KERNEL/atom.h>

namespace BALL
{
	template <>
	void TreeWidth<MolecularGraph>::BagContentWriter::operator() (std::ostream& out, const TreeDecompositionBag& v) const
	{
		out << "[label=\"";

		// find all vertices in the current bag
		TreeDecompositionContent content = boost::get(boost::vertex_bag_content, *td_, v);

		for (TreeDecompositionContent::const_iterator tdc_it = content.begin(); tdc_it != content.end(); ++tdc_it)
		{
			TreeWidth<MolecularGraph>::OriginalVertexType ov = *tdc_it;
			Atom const* atom = boost::get(boost::vertex_atom_ptr, *original_graph_, ov);

			out << atom->getFullName() << " (" << atom << ") " << "\\n";
		}

		out << "\"]";
	}
}

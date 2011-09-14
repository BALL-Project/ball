#include <BALL/DATATYPE/GRAPH/molecularGraph.h>

#include <BALL/KERNEL/atom.h>
#include <BALL/KERNEL/bond.h>
#include <BALL/KERNEL/atomContainer.h>

#include <BALL/COMMON/exception.h>

namespace BALL
{
	class BondVisitor : public UnaryProcessor<Bond>
	{
		public:
			BondVisitor(MolecularGraph& molgraph, std::map<const Bond*, MolecularGraph::Edge>& bte)
				: molgraph_(molgraph),
				  bte_(bte),
				  bond_ptrs_(boost::get(boost::edge_bond_ptr, molgraph))
			{
			}

			Processor::Result operator()(Bond& bond)
			{
				MolecularGraph::Vertex a = molgraph_.getVertex(bond.getFirstAtom());
				MolecularGraph::Vertex b = molgraph_.getVertex(bond.getSecondAtom());

				MolecularGraph::Edge edge = boost::add_edge(a, b, molgraph_).first;
				boost::put(bond_ptrs_, edge, &bond);
				bte_.insert(std::make_pair(&bond, edge));

				return Processor::CONTINUE;
			}

		private:
			MolecularGraph& molgraph_;
			std::map<const Bond*, MolecularGraph::Edge>& bte_;
			MolecularGraph::BondPtrMap bond_ptrs_;
	};

	MolecularGraph::MolecularGraph(AtomContainer& ac, MolecularGraph::ExportOptions /*opts*/)
		: MolecularGraphBase(ac.countAtoms())
	{
		// Create a bijective mapping between Vertices and Atom*
		AtomPtrMap atom_ptrs = get(boost::vertex_atom_ptr, *this);

		AtomConstIterator ait = ac.beginAtom();
		VertexIterator vi, vi_end;
		boost::tie(vi, vi_end) = boost::vertices(*this);

		for(; +ait; ++vi, ++ait)
		{
			atom_to_vertex_.insert(std::make_pair(&*ait,*vi));
			boost::put(atom_ptrs, *vi, &*ait);
		}

		//Create all bonds
		BondVisitor visitor(*this, bond_to_edge_);
		ac.applyIntraBond(visitor);
	}

	const MolecularGraph::Edge& MolecularGraph::getEdge(const Bond* bond) const
	{
		std::map<const Bond*, Edge>::const_iterator it = bond_to_edge_.find(bond);

		if(it == bond_to_edge_.end())
		{
			throw Exception::InvalidArgument(__FILE__, __LINE__, "An unknown Bond pointer was specified");
		}

		return it->second;
	}

	const MolecularGraph::Vertex& MolecularGraph::getVertex(const Atom* atom) const
	{
		std::map<const Atom*, Vertex>::const_iterator it = atom_to_vertex_.find(atom);

		if(it == atom_to_vertex_.end())
		{
			throw Exception::InvalidArgument(__FILE__, __LINE__, "An unknown Atom pointer was specified");
		}

		return it->second;
	}

	void MolecularGraph::editableCopy(EditableGraph& eg)
	{
		boost::copy_graph(*dynamic_cast<MolecularGraphBase*>(this), eg, 
				vertex_copy(GRAPH::makeEditableVertexCopier(*this, eg)).edge_copy(GRAPH::makeEditableEdgeCopier(*this, eg)));
	}
}


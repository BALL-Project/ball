// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: molecularGraph.h,v 1.4 2003/02/21 16:06:47 anhi Exp $

#ifndef BALL_STRUCTURE_MOLECULARGRAPH_H
#define BALL_STRUCTURE_MOLECULARGRAPH_H

#ifndef BALL_KERNEL_ATOM_H
#	include <BALL/KERNEL/atom.h>
#endif
#ifndef BALL_KERNEL_BOND_H
#	include <BALL/KERNEL/bond.h>
#endif
#ifndef BALL_STRUCTURE_MOLECULE_H
#	include <BALL/KERNEL/molecule.h>
#endif

#include <list>
#include <iostream>
#include <algorithm>

namespace BALL
{

	// forward declarations to resolve nasty dependencies
	template <typename Node, typename Edge> 
	class EdgeItem;

	template <typename Node, typename Edge> 
	class TMolecularGraph;

	/**	The node type in a molecular graph
	*/
	template <typename Node, typename Edge>
	class NodeItem
	{
		public:
		/**	@name	Type definitions
		*/
		//@{
		///
		typedef NodeItem<Node, Edge> NodeItemType;

		///
		typedef EdgeItem<Node, Edge> EdgeItemType;

		///
		typedef typename std::list<EdgeItem<Node, Edge>*>::iterator Iterator;
		///
		typedef typename std::list<EdgeItem<Node, Edge>*>::const_iterator ConstIterator;
		//@}

		friend class TMolecularGraph<Node, Edge>;

		NodeItem() throw();
		NodeItem(const Atom& atom) throw();

		Node& getData() throw();
		const Node& getData() const throw();
		void setData(const Node& data) throw();

		const Atom* getAtom() const throw();
		Atom* getAtom() throw();

		Iterator begin() throw();
		ConstIterator begin() const throw();
		Iterator end() throw();
		ConstIterator end() const throw();

		Size getDegree() const throw();

		bool operator == (const NodeItem& item) const throw();
		bool operator != (const NodeItem& item) const throw();

		protected:

		void deleteEdge_(EdgeItemType* item)
			throw();

		Node	data_;
		Atom* atom_;
		std::list<EdgeItemType*> adjacent_edges_;
	};

	template <typename Node, typename Edge>
	class EdgeItem
	{
		public:
		typedef NodeItem<Node, Edge> NodeItemType;
		typedef EdgeItem<Node, Edge> EdgeItemType;
		typedef typename std::list<NodeItem<Node, Edge>*>::iterator Iterator;
		typedef typename std::list<NodeItem<Node, Edge>*>::const_iterator ConstIterator;

		EdgeItem() 
			: bond_(0), source_(0), target_(0)
		{}

		EdgeItem(const Bond& bond);
		EdgeItem(const Bond& bond, NodeItemType* source, NodeItemType* target);

		NodeItemType& getSource() {return *source_;}
		NodeItemType& getTarget() {return *target_;}
		const NodeItemType& getSource() const {return *source_;}
		const NodeItemType& getTarget() const {return *target_;}
		
		Node& getData() {return data_;}
		const Node& getData() const {return data_;}
		void setData(const Edge& data) { data_ = data; };

		Bond* getBond() { return bond_; }
		const Bond* getBond() const { return bond_; }

		bool operator == (const EdgeItem& item) const { return (bond_ == item.bond_); }
		bool operator != (const EdgeItem& item) const { return (bond_ != item.bond_); }

		protected:
		Edge	data_;
		Bond* bond_;
		NodeItemType* source_;
		NodeItemType* target_;
	};

	template <typename Node, typename Edge>
	EdgeItem<Node, Edge>::EdgeItem(const Bond& bond)
		:	bond_(const_cast<Bond*>(&bond))
	{
	}

	template <typename Node, typename Edge>
	EdgeItem<Node, Edge>::EdgeItem(const Bond& bond, NodeItem<Node, Edge>* first, NodeItem<Node, Edge>* second)
		:	bond_(const_cast<Bond*>(&bond)),
			source_(first),
			target_(second)
	{
	}

	template <typename Node, typename Edge>
	class TMolecularGraph
	{
		public:
		typedef NodeItem<Node, Edge> NodeItemType;
		typedef EdgeItem<Node, Edge> EdgeItemType;
		typedef typename std::list<NodeItemType>::iterator NodeIterator;
		typedef typename std::list<NodeItemType>::const_iterator NodeConstIterator;
		typedef typename std::list<EdgeItemType>::iterator EdgeIterator;
		typedef typename std::list<EdgeItemType>::const_iterator EdgeConstIterator;

		TMolecularGraph() throw();
		TMolecularGraph(const Molecule& molecule) throw();

		bool newNode(const Atom& atom) throw();
		bool newEdge(const Bond& bond) throw();

		bool deleteNode(NodeItemType& node);
		bool deleteEdge(EdgeItemType& edge);
				
		bool deleteNode(const Atom& atom);
		bool deleteEdge(const Bond& bond);
				
		NodeIterator beginNode() { return nodes_.begin(); }
		NodeConstIterator beginNode() const { return nodes_.begin(); }
		EdgeIterator beginEdge() { return edges_.begin(); }
		EdgeConstIterator beginEdge() const { return edges_.begin(); }
		NodeIterator endNode() { return nodes_.end(); }
		NodeConstIterator endNode() const { return nodes_.end(); }
		EdgeIterator endEdge() { return edges_.end(); }
		EdgeConstIterator endEdge() const { return edges_.end(); }
		
		bool has(const Atom& atom) const { return atom_to_node_.has(const_cast<Atom*>(&atom)); }
		bool has(const Bond& atom) const { return bond_to_bond_.has(const_cast<Bond*>(&bond)); }

		NodeItemType& getNode(Position index) { return nodes_[index]; };
		const NodeItemType& getNode(Position index) const { return nodes_[index]; };
		EdgeItemType& getEdge(Position index) { return edges_[index]; };
		const EdgeItemType& getEdge(Position index) const { return edges_[index]; };

		/** Return the number of nodes in the graph
		*/
		Size getNumberOfNodes() const throw();

		/** Return the number of nodes in the graph
		*/
		Size getNumberOfEdges() const throw();

		protected:
		std::list<NodeItemType>	nodes_;
		std::list<EdgeItemType> edges_;
		HashMap<Atom*, NodeItemType*> atom_to_node_;
		HashMap<Bond*, EdgeItemType*> bond_to_edge_;
	};

	/**	Default molecular graph type.
			Each edge and node has a label of type  \link Index Index \endlink .
	*/
	typedef TMolecularGraph<Index, Index> MolecularGraph;

	template <typename Node, typename Edge>
	TMolecularGraph<Node, Edge>::TMolecularGraph()
			throw()
		:	nodes_(),
			edges_(),
			atom_to_node_(),
			bond_to_edge_()
	{
	}

	template <typename Node, typename Edge>
	TMolecularGraph<Node, Edge>::TMolecularGraph(const Molecule& molecule)
			throw()
		:	nodes_(),
			edges_(),
			atom_to_node_(),
			bond_to_edge_()
	{
		AtomConstIterator ai = molecule.beginAtom();
		Atom::BondConstIterator bi;
		for (; +ai; ++ai)
		{
			newNode(*ai);
		}
		for (ai = molecule.beginAtom(); +ai; ++ai)
		{
			for (bi = ai->beginBond(); +bi; ++bi)
			{
				if (bi->getFirstAtom() == &*ai)
				{
					newEdge(*bi);
				}
			}
		}
	}

	template <typename Node, typename Edge>
	bool TMolecularGraph<Node, Edge>::newNode(const Atom& atom)
		throw()
	{
		Atom* atom_ptr = const_cast<Atom*>(&atom);

		if (atom_to_node_.has(atom_ptr))
		{
			return false;
		}

		nodes_.push_back(NodeItemType(atom));
		atom_to_node_.insert(std::pair<Atom*, NodeItemType*>(atom_ptr, &(nodes_.back())));

		return true;
	}

	template <typename Node, typename Edge>
	bool TMolecularGraph<Node, Edge>::newEdge(const Bond& bond)
		throw()
	{
		Atom* first = const_cast<Atom*>(bond.getFirstAtom());
		Atom* second = const_cast<Atom*>(bond.getSecondAtom());	
		if (!atom_to_node_.has(first) || !atom_to_node_.has(second))
		{
			return false;
		}

		NodeItemType* first_item = atom_to_node_[first];
		NodeItemType* second_item = atom_to_node_[second];
		edges_.push_back(EdgeItemType(bond, first_item, second_item));
		bond_to_edge_.insert(std::pair<Bond*, EdgeItemType*>(const_cast<Bond*>(&bond), &edges_.back()));
		first_item->adjacent_edges_.push_back(&edges_.back());
		second_item->adjacent_edges_.push_back(&edges_.back());

		return true;
	}

	template <typename Node, typename Edge>
	std::ostream& operator << (std::ostream& os, const TMolecularGraph<Node, Edge>& G)
	{		
		os << "Nodes:" << std::endl;

		typename TMolecularGraph<Node, Edge>::NodeConstIterator node = G.beginNode();
		Size count = 0;
		for (; node != G.endNode(); ++node)
		{
			os << count++ << ": " << node->getAtom()->getFullName() << " [" << node->getDegree() << "] '" << node->getAtom() << "'" << std::endl;			
		}

		os << "Edges:" << std::endl;	

		typename TMolecularGraph<Node, Edge>::EdgeConstIterator edge = G.beginEdge();
		count = 0;
		for (; edge != G.endEdge(); ++edge)
		{
			os << count++ << ": " << edge->getSource().getAtom() << "-" << edge->getTarget().getAtom() << " '" << edge->getBond() << "'" << std::endl;
		}

		return os;
	}

	template <typename Node, typename Edge>
	bool TMolecularGraph<Node, Edge>::deleteNode(const Atom& atom)
	{
		if (!atom_to_node_.has(const_cast<Atom*>(&atom)))
		{
			return false;
		}
		
		return deleteNode(*atom_to_node_[const_cast<Atom*>(&atom)]);
	}

	template <typename Node, typename Edge>
	bool TMolecularGraph<Node, Edge>::deleteEdge(const Bond& bond)
	{
		if (!bond_to_edge_.has(const_cast<Bond*>(&bond)))
		{
			return false;
		}
		
		return deleteEdge(*bond_to_edge_[const_cast<Bond*>(&bond)]);
	}

	template <typename Node, typename Edge>
	bool TMolecularGraph<Node, Edge>::deleteNode(typename TMolecularGraph<Node, Edge>::NodeItemType& node)
	{
		NodeIterator node_it = std::find(nodes_.begin(), nodes_.end(), node);
		if (node_it == nodes_.end())
		{
			return false;
		}

		bool remove = true;
		while (remove)
		{
			remove = false;
			typename NodeItemType::Iterator edge(node.begin());
			for (; edge != node.end(); ++edge)
			{
				deleteEdge(**edge);
				break;
			}
		}

		atom_to_node_.erase(node_it->getAtom());
		nodes_.erase(node_it);
	
		return true;
	}

	template <typename Node, typename Edge>
	bool TMolecularGraph<Node, Edge>::deleteEdge(typename TMolecularGraph<Node, Edge>::EdgeItemType& edge)
	{
		typename std::list<EdgeItemType>::iterator edge_it = std::find(edges_.begin(), edges_.end(), edge);
		if (edge_it == edges_.end())
		{
			return false;
		}
		edge.getSource().deleteEdge_(&edge);
		edge.getTarget().deleteEdge_(&edge);
		bond_to_edge_.erase(edge_it->getBond());
		edges_.erase(edge_it);
		
		return true;
	}


	template <typename Node, typename Edge>
	NodeItem<Node, Edge>::NodeItem()
		throw()
		: atom_(0)
	{
	}

	template <typename Node, typename Edge>
	NodeItem<Node, Edge>::NodeItem(const Atom& atom)
		throw()
		:	atom_(const_cast<Atom*>(&atom))
	{
	}

	template <typename Node, typename Edge>
	Node& NodeItem<Node, Edge>::getData()
		throw()
	{
		return data_;
	}

	template <typename Node, typename Edge>
	const Node& NodeItem<Node, Edge>::getData() const 
		throw()
	{	
		return data_;
	}


	template <typename Node, typename Edge>
	void NodeItem<Node, Edge>::setData(const Node& data) 
		throw()
	{ 
		data_ = data; 
	}

	
	template <typename Node, typename Edge>
	const Atom* NodeItem<Node, Edge>::getAtom() const 
		throw()
	{ 
		return atom_;
	}

	template <typename Node, typename Edge>
	Atom* NodeItem<Node, Edge>::getAtom() 
		throw()
	{ 
		return atom_;
	}

	template <typename Node, typename Edge>
	typename NodeItem<Node, Edge>::Iterator NodeItem<Node, Edge>::begin() 
		throw()
	{ 
		return adjacent_edges_.begin(); 
	}
	
	template <typename Node, typename Edge>
	typename NodeItem<Node, Edge>::ConstIterator NodeItem<Node, Edge>::begin() const 
		throw()
	{ 
		return adjacent_edges_.begin(); 
	}

	template <typename Node, typename Edge>
	typename NodeItem<Node, Edge>::Iterator NodeItem<Node, Edge>::end() 
		throw()
	{ 
		return adjacent_edges_.end(); 
	}

	template <typename Node, typename Edge>
	typename NodeItem<Node, Edge>::ConstIterator NodeItem<Node, Edge>::end() const		
		throw()
	{ 
		return adjacent_edges_.end(); 
	}

	template <typename Node, typename Edge>
	Size NodeItem<Node, Edge>::getDegree() const 
		throw()
	{ 
		return (Size)adjacent_edges_.size(); 
	}

	template <typename Node, typename Edge>
	bool NodeItem<Node, Edge>::operator == (const NodeItem& item) const 
		throw()
	{ 
		return (atom_ == item.atom_); 
	}

	template <typename Node, typename Edge>
	bool NodeItem<Node, Edge>::operator != (const NodeItem& item) const 
		throw()
	{ 
		return (atom_ != item.atom_); 
	}

	template <typename Node, typename Edge>
	void NodeItem<Node, Edge>::deleteEdge_(EdgeItemType* item)
		throw()
	{
		Iterator it(std::find(adjacent_edges_.begin(), adjacent_edges_.end(), item));
		if (it != adjacent_edges_.end())
		{
			adjacent_edges_.erase(it);
		}
	}

	template <typename Node, typename Edge>
	BALL_INLINE
	Size TMolecularGraph<Node, Edge>::getNumberOfNodes() const
		throw()
	{
		return atom_to_node_.size();
	}

	template <typename Node, typename Edge>
	BALL_INLINE
	Size TMolecularGraph<Node, Edge>::getNumberOfEdges() const
		throw()
	{
		return bond_to_edge_.size();
	}


} // namespace BALL

#endif // BALL_STRUCTURE_MOLECULARGRAPH_H

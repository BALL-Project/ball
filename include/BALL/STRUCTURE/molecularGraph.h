// $Id: molecularGraph.h,v 1.1.2.1 2002/05/28 00:05:10 oliver Exp $

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
	class MolecularGraph;

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

		friend class MolecularGraph<Node, Edge>;

		NodeItem();
			: atom_(0)
		{
		}

		NodeItem(const Atom& atom)
			:	atom_(const_cast<Atom*>(&atom))
		{
		}

		Node& getData() {return data_;}
		const Node& getData() const {return data_;}
		void setData(const Node& data) { data_ = data; };

		const Atom* getAtom() const { return atom_;}
		Atom* getAtom() { return atom_;}

		Iterator begin() { return adjacent_edges_.begin(); }
		ConstIterator begin() const { return adjacent_edges_.begin(); }
		Iterator end() { return adjacent_edges_.end(); }
		ConstIterator end() const { return adjacent_edges_.end(); }

		Size getDegree() const { return (Size)adjacent_edges_.size(); }

		bool operator == (const NodeItem& item) const { return (atom_ == item.atom_); }

		protected:

		void deleteEdge_(EdgeItemType* item)
		{
			cout << "entering deleteEdge_(atom = " << getAtom() << ", bond = " << item->getBond() << ")" << endl;
			Iterator it(find(adjacent_edges_.begin(), adjacent_edges_.end(), item));
			if (it != adjacent_edges_.end())
			{
				cout << "erasing edge " << item->getBond() << endl;
				adjacent_edges_.erase(it);
			}
			else
			{
				cout << "cannot erase edge " << item->getBond() << endl;
			}
		}
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
	class MolecularGraph
	{
		public:
		typedef NodeItem<Node, Edge> NodeItemType;
		typedef EdgeItem<Node, Edge> EdgeItemType;
		typedef typename std::list<NodeItemType>::iterator NodeIterator;
		typedef typename std::list<NodeItemType>::const_iterator NodeConstIterator;
		typedef typename std::list<EdgeItemType>::iterator EdgeIterator;
		typedef typename std::list<EdgeItemType>::const_iterator EdgeConstIterator;
			
		MolecularGraph(const Molecule& molecule);

		bool newNode(const Atom& atom);
		bool newEdge(const Bond& bond);

		bool deleteNode(NodeItemType& node);
		bool deleteEdge(EdgeItemType& edge);
				
		NodeIterator beginNode() { return nodes_.begin(); }
		NodeConstIterator beginNode() const { return nodes_.begin(); }
		EdgeIterator beginEdge() { return edges_.begin(); }
		EdgeConstIterator beginEdge() const { return edges_.begin(); }
		NodeIterator endNode() { return nodes_.end(); }
		NodeConstIterator endNode() const { return nodes_.end(); }
		EdgeIterator endEdge() { return edges_.end(); }
		EdgeConstIterator endEdge() const { return edges_.end(); }
		
		bool has(const Atom& atom) const {return atom_to_node_.has(const_cast<Atom*>(&atom));}
		bool has(const Bond& atom) const {return bond_to_bond_.has(const_cast<Bond*>(&bond));}

		NodeItemType& getNode(Position index) { return nodes_[index]; };
		const NodeItemType& getNode(Position index) const { return nodes_[index]; };
		EdgeItemType& getEdge(Position index) { return edges_[index]; };
		const EdgeItemType& getEdge(Position index) const { return edges_[index]; };

		protected:
		std::list<NodeItemType>	nodes_;
		std::list<EdgeItemType> edges_;
		HashMap<Atom*, NodeItemType*> atom_to_node_;
		HashMap<Bond*, EdgeItemType&> bond_to_edge_;
	};


	template <typename Node, typename Edge>
	MolecularGraph<Node, Edge>::MolecularGraph(const Molecule& molecule)
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
	bool MolecularGraph<Node, Edge>::newNode(const Atom& atom)
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
	bool MolecularGraph<Node, Edge>::newEdge(const Bond& bond)
	{
		Atom* first = const_cast<Atom*>(bond.getFirstAtom());
		Atom* second = const_cast<Atom*>(bond.getSecondAtom());	
		if (!atom_to_node_.has(first) || !atom_to_node_.has(second))
		{
			return false;
		}

		NodeItemType* first_item = &nodes_[atom_to_node_[first]];
		NodeItemType* second_item = &nodes_[atom_to_node_[second]];
		edges_.push_back(EdgeItemType(bond, first_item, second_item));
		bond_to_edge_.insert(std::pair<Bond*, EdgeItemType*>(const_cast<Bond*>(&bond), &edges_.back()));
		first_item->adjacent_edges_.push_back(&edges_.back());
		second_item->adjacent_edges_.push_back(&edges_.back());

		return true;
	}

	template <typename Node, typename Edge>
	std::ostream& operator << (std::ostream& os, const MolecularGraph<Node, Edge>& G)
	{		
		os << "Nodes:" << std::endl;

		MolecularGraph<Node, Edge>::NodeConstIterator node = G.beginNode();
		Size count = 0;
		for (; node != G.endNode(); ++node)
		{
			os << count++ << ": " << node->getAtom()->getFullName() << " [" << node->getDegree() << "] '" << node->getAtom() << "'" << std::endl;			
		}

		os << "Edges:" << std::endl;	

		MolecularGraph<Node, Edge>::EdgeConstIterator edge = G.beginEdge();
		count = 0;
		for (; edge != G.endEdge(); ++edge)
		{
			os << count++ << ": " << edge->getSource().getAtom() << "-" << edge->getTarget().getAtom() << " '" << edge->getBond() << "'" << std::endl;
		}

		return os;
	}

	template <typename Node, typename Edge>
	bool MolecularGraph<Node, Edge>::deleteNode(MolecularGraph<Node, Edge>::NodeItemType& node)
	{
		cout << "entering deleteNode(atom = " << node.getAtom() <<  ")" << endl;
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

		std::cout << "Deleted edges... " << std::endl;
		atom_to_node_.erase(node_it->getAtom());
		nodes_.erase(node_it);
	
		return true;
	}

	template <typename Node, typename Edge>
	bool MolecularGraph<Node, Edge>::deleteEdge(MolecularGraph<Node, Edge>::EdgeItemType& edge)
	{
		cout << "entering deleteEdge(edge = " << edge.getBond() <<  ")" << endl;
		typename std::list<EdgeItemType>::iterator edge_it = std::find(edges_.begin(), edges_.end(), edge);
		if (edge_it == edges_.end())
		{
			cout << "Can't delete edge: " << edge.getBond() << " (false)" << std::endl;
			return false;
		}
		cout << "Deleted edge " << edge.getBond() << std::endl;
		edge.getSource().deleteEdge_(&edge);
		edge.getTarget().deleteEdge_(&edge);
		edges_.erase(edge_it);
		
		return true;
	}


	template <typename T>
	NodeItem<T>::NodeItem();
		: atom_(0)
	{
	}

	template <typename T>
	NodeItem<T>::NodeItem(const Atom& atom)
		:	atom_(const_cast<Atom*>(&atom))
	{
	}

	template <typename T>
	typename NodeItem<T>::Node& NodeItem<T>::getData() 
	{
		return data_;
	}

	template <typename T>
	const typename NodeItem<T>::Node& NodeItem<T>::getData() const 
	{	
		return data_;
	}


	template <typename T>
	void NodeItem<T>::setData(const typename NodeItem<T>::Node& data) 
	{ 
		data_ = data; 
	};

	//???
	const Atom* getAtom() const { return atom_;}
	Atom* getAtom() { return atom_;}

		Iterator begin() { return adjacent_edges_.begin(); }
		ConstIterator begin() const { return adjacent_edges_.begin(); }
		Iterator end() { return adjacent_edges_.end(); }
		ConstIterator end() const { return adjacent_edges_.end(); }

		Size getDegree() const { return (Size)adjacent_edges_.size(); }

		bool operator == (const NodeItem& item) const { return (atom_ == item.atom_); }

		protected:

		void deleteEdge_(EdgeItemType* item)
		{
			cout << "entering deleteEdge_(atom = " << getAtom() << ", bond = " << item->getBond() << ")" << endl;
			Iterator it(find(adjacent_edges_.begin(), adjacent_edges_.end(), item));
			if (it != adjacent_edges_.end())
			{
				cout << "erasing edge " << item->getBond() << endl;
				adjacent_edges_.erase(it);
			}
			else
			{
				cout << "cannot erase edge " << item->getBond() << endl;
			}
		}


} // namespace BALL

#endif // BALL_STRUCTURE_MOLECULARGRAPH_H

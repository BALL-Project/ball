// $Id: standardPredicates.C,v 1.30.4.4 2002/12/03 22:57:07 anker Exp $

#include <BALL/KERNEL/standardPredicates.h>

#include <BALL/CONCEPT/selectable.h>
#include <BALL/KERNEL/atom.h>
#include <BALL/KERNEL/PTE.h>
#include <BALL/KERNEL/residue.h>
#include <BALL/KERNEL/protein.h>
#include <BALL/KERNEL/chain.h>
#include <BALL/KERNEL/secondaryStructure.h>
#include <BALL/KERNEL/nucleotide.h>
#include <BALL/KERNEL/nucleicAcid.h>
#include <BALL/KERNEL/bond.h>

#include <algorithm>

using namespace std;

namespace BALL 
{
	// True predicate

	bool TruePredicate::operator () (const Atom& /* atom */) const
		throw()
	{
    return true;
 	}

	// False predicate

	bool FalsePredicate::operator () (const Atom& /* atom */) const
		throw()
	{
    return false;
 	}

	// selected predicate

	bool SelectedPredicate::operator () (const Atom& atom) const
		throw()
	{
    return atom.isSelected();
 	}

	// Atom name predicate

	bool AtomNamePredicate::operator () (const Atom& atom) const
		throw()
	{
    return (atom.getName() == argument_);
 	}

	// Atom type predicate

	bool AtomTypePredicate::operator () (const Atom& atom) const
		throw()
	{
		return (atom.getTypeName() == argument_);
 	}


	// element predicate

	bool ElementPredicate::operator () (const Atom& atom) const
		throw()
	{
		return (atom.getElement().getSymbol() == argument_);
	}

	// residue predicate

	bool ResiduePredicate::operator () (const Atom& atom) const
		throw()
	{
		const Residue*	res = atom.getAncestor(RTTI::getDefault<Residue>());
		if (res != 0)
		{
			return (res->getName() == argument_);
		}

		return false;
	}
	
	// residue ID predicate

	bool ResidueIDPredicate::operator () (const Atom& atom) const
		throw()
	{
    const Residue*	res = atom.getAncestor(RTTI::getDefault<Residue>());
		if (res != 0)
		{
			return (res->getID() == argument_);
		}
		
		return false;
	}
	
	// protein predicate

	bool ProteinPredicate::operator () (const Atom& atom) const
		throw()
	{
		const Protein* protein = atom.getAncestor(RTTI::getDefault<Protein>());
		if (protein != 0)
		{
			return (protein->getName() == argument_);
		}

		return false;
	}
	
	// chain predicate

	bool ChainPredicate::operator () (const Atom& atom) const
		throw()
	{
 		const Chain* chain = atom.getAncestor(RTTI::getDefault<Chain>());
		if (chain != 0)
		{
			return (chain->getName() == argument_);
		}

		return false;
	}
	
	// secondary structure predicate

	bool SecondaryStructurePredicate::operator () (const Atom& atom) const
		throw()
	{
 		const SecondaryStructure* sec_struct 
			= atom.getAncestor(RTTI::getDefault<SecondaryStructure>());
		return ((sec_struct != 0) && (sec_struct->getName() == argument_));
	}
	
	// solvent predicate

	bool SolventPredicate::operator () (const Atom& atom) const
		throw()
	{
		const Molecule* molecule = atom.getMolecule();
		return ((molecule != 0) && (molecule->hasProperty(Molecule::IS_SOLVENT)));
	}
	
	// molecule predicate

	bool MoleculePredicate::operator () (const Atom& atom) const
		throw()
	{
		const Molecule* molecule = atom.getMolecule();
		return ((molecule != 0) && (molecule->getName() == argument_));
	}

	// backbone predicate

	bool BackBonePredicate::operator () (const Atom& atom) const
		throw()
	{
		if (atom.hasAncestor(RTTI::getDefault<Residue>()))
		{
			String name = RTTI::castTo<Atom>(atom)->getName();
			if ((name == "C") || (name == "N") || (name == "CA") || (name == "O"))
			{
				return true;
			}
		}

		return false;
	}
	
	// nucleic acid predicate

	bool NucleicAcidPredicate::operator () (const Atom& atom) const
		throw()
	{
		const NucleicAcid* nucleic_acid 
			= atom.getAncestor(RTTI::getDefault<NucleicAcid>());
		if (nucleic_acid != 0)
		{
			return (nucleic_acid->getName() == argument_);
		}
		return false;
	}

	// nucleotide predicate

	bool NucleotidePredicate::operator () (const Atom& atom) const
		throw()
	{
		return RTTI::isKindOf<Nucleotide>(atom);
	}

	// in ring predicate

	bool InRingPredicate::dfs_(const Atom& atom, const Atom& first_atom,
			const Size limit, HashSet<const Bond*>& visited) const
		throw()
	{
		// the following recursive function performs an ad-hoc dfs and returns
		// true, if a ring was found and false otherwise.

		if (limit == 0) 
		{
			if (atom == first_atom) 
			{
				// Found first atom at limit
				return true;
			}
			else
			{
				// Reached limit without finding the first atom
				return false;
			}
		}
		Size i;
		const Bond* bond;
		Atom* descend;
		HashSet<const Bond*> my_visited(visited);

		// Now iterate over all Bonds an store the visited bonds.

		for (i = 0; i < atom.countBonds(); ++i)
		{
			bond = atom.getBond(i);
			if (!my_visited.has(bond))
			{
				descend = bond->getPartner(atom);
				my_visited.insert(bond);
				if (dfs_(*descend, first_atom, limit-1, my_visited))
				{
					return true;
				}
			}
		}
		// No partner matched
		return false;
	}


	bool InRingPredicate::operator () (const Atom& atom) const
		throw()
	{
		int n;
		if (argument_.size() == 1)
		{
			if (argument_.isDigit())
			{
				n = argument_.toInt();
			}
			else
			{
				Log.error() << "InRingPredicate::operator () (): "
					<< "Expected a number < 9: " << argument_ << endl;
				return false;
			}
		}
		else
		{
			Log.error() << "InRingPredicate::operator () (): "
				<< "Expected a number < 9: " << argument_ << endl;
			return false;
		}

		// There are no rings with less than 3 atoms
		if (n < 3) 
		{
			return false;
		}

		HashSet<const Bond*> visited;
		if (dfs_(atom,atom,n,visited))
		{
			return true;
		}
		else
		{
			return false;
		}

	}


	bool DoubleBondsPredicate::operator () (const Atom& atom) const
		throw()
	{
		return testPredicate_(atom, Bond::ORDER__DOUBLE);
	}


	bool DoubleBondsPredicate::testPredicate_(const Atom& atom, 
			Bond::Order order) const
		throw()
	{
		String s = argument_;
		s.trim();

		if (s.size() > 2)
		{
			// There can only be an operator followed by a number < 9
			Log.error() << "DoubleBondsPredicate::operator () (): "
				<< "argument_ too long " << endl;
			return false;
		}
		
		Size count = 0;
		Size i = 0;
		for (; i < atom.countBonds(); ++i)
		{
			if ((atom.getBond(i))->getOrder() == order)
			{
				count++;
			}
		}

		Size n;
		if (s.size() == 2)
		{
			n = ((String) s[1]).toInt();
			switch (s[0]) 
			{
				case '<' :
					if (count < n)
					{
						return true;
					}
					else
					{
						return false;
					}

				case '>' :
					if (count > n)
					{
						return true;
					}
					else 
					{
						return false;
					}

				case '=':
					if (count == n)
					{
						return true;
					}
					else 
					{
						return false;
					}

				default:
					Log.error() << "doubleBond::operator (): Illegal operator " 
						<< s[0] << endl;
					return false;
			}
		}
		else 
		{
			n = ((String) s[0]).toInt();
			if (count == n)
			{
				return true;
			}
			else 
			{
				return false;
			}
		}
	}
	

	bool SingleBondsPredicate::operator () (const Atom& atom) const
		throw()
	{
		return testPredicate_(atom, Bond::ORDER__SINGLE);
	}

	
	bool TripleBondsPredicate::operator () (const Atom& atom) const
		throw()
	{
		return testPredicate_(atom, Bond::ORDER__TRIPLE);
	}
	
	bool AromaticBondsPredicate::operator () (const Atom& atom) const
		throw()
	{
		return testPredicate_(atom, Bond::ORDER__AROMATIC);
	}

	bool NumberOfBondsPredicate::operator () (const Atom& atom) const
		throw()
	{
		String s = argument_;
		s.trim();

		if (s.size() > 2)
		{
			// There can only be an operator followed by a number < 9
			Log.error() << "DoubleBondsPredicate::operator () (): argument_ too long " << endl;
			return false;
		}
		
		Size count = atom.countBonds();

		Size n;
		if (s.size() == 2)
		{
			n = ((String) s[1]).toInt();
			switch (s[0]) 
			{
				case '<' :
					if (count < n)
					{
						return true;
					}
					else
					{
						return false;
					}

				case '>' :
					if (count > n)
					{
						return true;
					}
					else 
					{
						return false;
					}

				case '=':
					if (count == n)
					{
						return true;
					}
					else 
					{
						return false;
					}

				default:
					Log.error() << "doubleBond::operator (): Illegal operator " 
						<< s[0] << endl;
					return false;
			}
		}
		else 
		{
			n = ((String) s[0]).toInt();
			if (count == n)
			{
				return true;
			}
			else 
			{
				return false;
			}
		}
	}


	ConnectedToPredicate::CTPNode::CTPNode()
		throw()
		:	element_symbol_("<uninitialized>"),
			bond_type_(BONDTYPE__UNINITIALISED),
			children_(),
			parent_(0),
			finished_(false),
			linked_(false),
			link_list_()
	{
	}

	ConnectedToPredicate::CTPNode::CTPNode(const ConnectedToPredicate::CTPNode& node)
		throw()
		:	element_symbol_(node.element_symbol_),
			bond_type_(node.bond_type_),
			children_(node.children_),
			parent_(node.parent_),
			finished_(node.finished_),
			linked_(node.linked_),
			link_list_(node.link_list_)
	{
	}

	ConnectedToPredicate::CTPNode::~CTPNode()
		throw()
	{
		// ?????
		// TODO
	}

	void ConnectedToPredicate::CTPNode::setParent(ConnectedToPredicate::CTPNode* parent)
		throw()
	{
		if (parent == 0)
		{
			Log.error() << "ConnectedToPredicate::CTPNode::setParent(): "
				<< "Trying to set NULL as parent. Ignoring." << endl;
		}
		else
		{
			parent_ = parent;
		}
	}

	ConnectedToPredicate::CTPNode* ConnectedToPredicate::CTPNode::getParent() const
		throw()
	{
		return parent_;
	}

	void ConnectedToPredicate::CTPNode::addChild(ConnectedToPredicate::CTPNode* child)
		throw()
	{
		if (child == 0)
		{
			Log.error() << "ConnectedToPredicate::CTPNode::addChild(): "
				<< "Trying to add NULL as child. Ignoring." << endl;
		}
		else
		{
			children_.push_back(child);
		}
	}

	ConnectedToPredicate::CTPNode::Iterator ConnectedToPredicate::CTPNode::begin()
		throw()
	{
		return children_.begin();
	}

	ConnectedToPredicate::CTPNode::ConstIterator ConnectedToPredicate::CTPNode::begin() const
		throw()
	{
		return children_.begin();
	}

	ConnectedToPredicate::CTPNode::Iterator ConnectedToPredicate::CTPNode::end()
		throw()
	{
		return children_.end();
	}

	ConnectedToPredicate::CTPNode::ConstIterator ConnectedToPredicate::CTPNode::end() const
		throw()
	{
		return children_.end();
	}

	/* 
	ConnectedToPredicate::CTPNode* ConnectedToPredicate::CTPNode::getChild(Size index) const
		throw()
	{
		if (index < getNumberOfChildren())
		{
			ConnectedToPredicate::CTPNode* child = children_[index];
			return child;
		}
		else
		{
			Log.error() << "ConnectedToPredicate::CTPNode::getChild(): "
				<< "Trying to access non-existant child with index " << index
				<< ". Returning NULL." << endl;
			return 0;
		}
	}
	*/

	void ConnectedToPredicate::CTPNode::removeChild(CTPNode* child)
		throw()
	{
		CTPNode::Iterator it = std::find(begin(), end(), child);
		if (it != end())
		{
			children_.erase(it);
		}
	}

	Size ConnectedToPredicate::CTPNode::getNumberOfChildren() const
		throw()
	{
		return children_.size();
	}

	void ConnectedToPredicate::CTPNode::setBondType(Size type)
		throw()
	{
		// ?????
		// TODO: check whether type exists...
		bond_type_ = type;
	}

	void ConnectedToPredicate::CTPNode::setBondType(char type)
		throw()
	{
		switch (type)
		{
			case '.':
				bond_type_ = BONDTYPE__ANY;
				break;

			case '-':
				bond_type_ = BONDTYPE__SINGLE;
				break;

			case '=':
				bond_type_ = BONDTYPE__DOUBLE;
				break;

			case '#':
				bond_type_ = BONDTYPE__TRIPLE;
				break;

			case '~':
				bond_type_ = BONDTYPE__AROMATIC;
				break;

			default:
				Log.error() << "ConnectedToPredicate::CTPNode::setBondType(): "
					<< "Unknown bond type character, defaulting to <any>." << endl;
				bond_type_ = BONDTYPE__ANY;
		}
	}

	Size ConnectedToPredicate::CTPNode::getBondType() const
		throw()
	{
		return bond_type_;
	}

	char ConnectedToPredicate::CTPNode::getBondTypeChar() const
		throw()
	{
		switch (bond_type_)
		{
			case BONDTYPE__ANY:
				return('.');

			case BONDTYPE__SINGLE:
				return('-');

			case BONDTYPE__DOUBLE:
				return('=');

			case BONDTYPE__TRIPLE:
				return('#');

			case BONDTYPE__QUADRUPLE:
			  return('%');

			case BONDTYPE__AROMATIC:
				return('~');

			case BONDTYPE__UNINITIALISED:
				return('U');

			default:
				return('?');
		}
	}

	String ConnectedToPredicate::CTPNode::getSymbol() const
		throw()
	{
		return element_symbol_;
	}

	void ConnectedToPredicate::CTPNode::setSymbol(const String& symbol)
		throw()
	{
		element_symbol_ = symbol;
	}

	void ConnectedToPredicate::CTPNode::setFinished()
		throw()
	{
		finished_ = true;
	}

	void ConnectedToPredicate::CTPNode::unsetFinished()
		throw()
	{
		finished_ = false;
	}

	bool ConnectedToPredicate::CTPNode::isFinished() const
		throw()
	{
		return finished_;
	}

	void ConnectedToPredicate::CTPNode::setLinked()
		throw()
	{
		linked_ = true;
	}

	void ConnectedToPredicate::CTPNode::unsetLinked()
		throw()
	{
		linked_ = false;
	}

	bool ConnectedToPredicate::CTPNode::isLinked() const
		throw()
	{
		return linked_;
	}

	void ConnectedToPredicate::CTPNode::linkWith(const ConnectedToPredicate::CTPNode* partner)
		throw()
	{
		if (partner == 0)
		{
			Log.error() << "ConnectedToPredicate::CTPNode::linkWith(): "
				<< "Trying to link with NULL. Ignoring." << endl;
			return;
		}
		link_list_.push_back(partner);
	}

	const std::list<const ConnectedToPredicate::CTPNode*>& ConnectedToPredicate::CTPNode::getLinkList() const
		throw()
	{
		return link_list_;
	}

	ConnectedToPredicate::ConnectedToPredicate()
		throw()
		:	tree_(0),
			link_map_(),
			link_mark_(0)
	{
	}

	ConnectedToPredicate::~ConnectedToPredicate()
		throw()
	{
	}
			
	ConnectedToPredicate::CTPNode* ConnectedToPredicate::createNewNode_(ConnectedToPredicate::CTPNode* node)
		throw()
	{
		// whenever we create a new node, the old one is finished.
		node->setFinished();
		CTPNode* child = new CTPNode;
		child->setParent(node);
		node->addChild(child);
		if (link_mark_ != 0)
		{
			pair<CTPNode*, CTPNode*> tmp(child, 0);
			link_map_.insert(pair<char, pair<CTPNode*, CTPNode*> >(link_mark_, tmp));
			child->setLinked();
			link_mark_ = 0;
		}
		return child;
	}

	ConnectedToPredicate::CTPNode* ConnectedToPredicate::parse_() 
		throw()
	{
		return(parse_(argument_));
	}

	ConnectedToPredicate::CTPNode* ConnectedToPredicate::parse_(const String& input) 
		throw()
	{
		Size depth = 0;
		int bracket_count = 0;
		Size position = 0;
		CTPNode* current = 0;
		Size verbosity = 1;
		std::list<CTPNode*> all_nodes;
		std::vector<CTPNode*> bracket_stack;

		CTPNode* root = new CTPNode;
		root->setSymbol("/");
		current = root;
		all_nodes.push_back(root);

		String bond_chars(".-=#%~");
		String uppercase("ABCDEFGHIJKLMNOPQRSTUVWXYZ");
		String lowercase("abcdefghijklmnopqrstuvwxyz");
		String numbers("12345678");


		for (; position < input.size(); ++position)
		{
			if (verbosity > 90)
			{
				Log.info() << "Examining character " << input[position] << endl;
			}
			if (input[position] == '(')
			{
				if (bond_chars.has(input[position - 1]))
				{
					Log.error() << "ConnectedToPredicate:parse_(): " << endl
						<< "\tparse error: bond char before bracket." << endl;
					return(false);
				}
				bracket_stack.push_back(current);
				current = createNewNode_(current);
				all_nodes.push_back(current);
				depth++;
				bracket_count++;
				if (verbosity > 90)
				{
					Log.info() << "Found (, created new node, new depth is " << depth
						<< endl;
				}
			}
			else
			{
				if (input[position] == ')')
				{
					if (current == 0)
					{
						Log.error() << "ConnectedToPredicate::parse_(): " << endl
							<< "\ttried to access a NULL pointer. Aborting." << endl;
						return(false);
					}
					current->setFinished();
					depth--;
					bracket_count--;
					if (verbosity > 90)
					{
						Log.info() << "Found ), new depth is " << depth
							<< endl;
					}
					if (bracket_stack.size() > 0)
					{
						current = bracket_stack.back();
						bracket_stack.pop_back();
					}
					else
					{
						Log.error() << "ConnectedToPredicate::parse_(): " << endl
							<< "\tparse error: missing opening bracket." << endl;
						return(false);
					}
				}
				else
				{
					if (bond_chars.has(input[position]))
					{
						if (current->getSymbol() != "<uninitialized>")
						{
							current = createNewNode_(current);
							all_nodes.push_back(current);
							depth++;
							if (verbosity > 90)
							{
								Log.info() << "Tried to set bond type of finished node, created a new one, new depth is " << depth << endl;
							}
						}
						current->setBondType(input[position]);
					}
					else
					{
						if (uppercase.has(input[position]) || (input[position] == '*'))
						{
							if (current->getSymbol() != "<uninitialized>")
							{
								current = createNewNode_(current);
								all_nodes.push_back(current);
								depth++;
								if (verbosity > 90)
								{
									Log.info() << "Found uppercase letter without prior \"(\", created new node, new depth is " << depth
										<< endl;
								}
							}
							// We are in a fresh node.
							current->setSymbol(input[position]);
							if (current->getBondType() == CTPNode::BONDTYPE__UNINITIALISED)
							{
								current->setBondType((Size) CTPNode::BONDTYPE__ANY);
							}
						}
						else
						{
							if (lowercase.has(input[position]))
							{
								if (current->isFinished() == true)
								{
									Log.error() << "ConnectedToPredicate::parse_(): " << endl
										<< "\tparse error: trying to add a lowercase char to an already finished node." 
										<< endl;
									return(0);
								}
								String symbol = current->getSymbol();
								if (symbol.size() != 1)
								{
									Log.error() << "ConnectedToPredicate::parse_(): "
										<< "\tparse error: trying to add a lowercase char to a symbol with length != 1." 
										<< endl;
									return(0);
								}
								if (symbol == '*')
								{
									Log.error() << "ConnectedToPredicate::parse_(): " << endl
										<< "\tparse error: trying to add a lowercase char to a \"*\"." 
										<< endl;
									return(0);
								}
								symbol += input[position];
								current->setSymbol(symbol);
							}
							else
							{
								if (input[position] == '@')
								{
									link_mark_ = input[++position];
									if (numbers.has(link_mark_))
									{

										if (link_map_.has(link_mark_) == true)
										{
											if (link_map_[link_mark_].second != 0)
											{
												Log.error() << "ConnectedToPredicate::parse_(): "
													<< "\tparse error: triple mark: " << link_mark_ << endl;
												return(0);
											}
											link_map_[link_mark_].second = current;
											CTPNode* first = link_map_[link_mark_].first;
											current->linkWith(first);
										}
									}
									else
									{
										Log.error() << "ConnectedToPredicate::parse_(): "
											<< "\tparse error: only numbers are allowed as marks." << endl;
										return(0);
									}
								}
								else
								{
									if (numbers.has(input[position]))
									{
										char n = input[position];
										Size N = atoi(&n);
										CTPNode* parent = current->getParent();
										if (parent == 0)
										{
											Log.error() << "ConnectedToPredicate::parse_(): " << endl
												<< "\tmultiplier without parent. Aborting."
												<< endl;
											return(0);
										}
										CTPNode* new_child = 0;
										for (Size i = 0; i < (N - 1); ++i)
										{
											new_child = new CTPNode(*current);
											all_nodes.push_back(new_child);
											parent->addChild(new_child);
										}
										current = parent;
									}
									else
									{
										Log.error() << "ConnectedToPredicate::parse_(): " << endl
											<< "\tparse error: unknown input char: " 
											<< input[position] << endl;
										return(false);
									}
								}
							}
						}
					}
				}
			}
		}

		if (bracket_count > 0)
		{
			Log.error() << "ConnectedToPredicate::parse_(): " << endl
				<< "\tparse error: too many opening brackets." << endl;
			return(0);
		}

		if (bracket_count < 0)
		{
			Log.error() << "ConnectedToPredicate::parse_(): " << endl
				<< "\tparse error: Too many closing brackets." << endl;
			return(0);
		}

		HashMap<char, pair<CTPNode*, CTPNode*> >::Iterator it = link_map_.begin();
		for (; +it; ++it)
		{
			if (it->second.first == 0)
			{
				it->second.first = root;
			}

			if (it->second.second == 0)
			{
				Log.error() << "ConnectedToPredicate::parse_(): " << endl
					<< "unresolved mark: " << it->first << endl;
				return(0);
			}

			it->second.first->addChild(it->second.second);

		}

		std::list<CTPNode*>::iterator sort_it = all_nodes.begin();
		for (; sort_it != all_nodes.end(); ++sort_it)
		{
			CTPNode::Iterator child_it = (*sort_it)->begin();
			for (; child_it != (*sort_it)->end(); ++child_it)
			{
				if ((*child_it)->getSymbol() == "*")
				{
					(*sort_it)->removeChild(*child_it);
					(*sort_it)->addChild(*child_it);
				}
			}
		}

		tree_ = root;
		return(root);
	}

	bool ConnectedToPredicate::bondOrderMatch_(const Bond& bond, const ConnectedToPredicate::CTPNode& node) const
		throw()
	{

		bool result = false;

		switch (node.getBondType())
		{
			case CTPNode::BONDTYPE__UNINITIALISED:
				Log.error() << "ConnectedToPredicate::bondOrderMatch_(): " << endl
					<< "\tuninitialized bond. Returning false." << endl;
				result = false;
				break;

			case CTPNode::BONDTYPE__ANY:
				result = true;
				break;

			case CTPNode::BONDTYPE__SINGLE:
				if (bond.getOrder() == Bond::ORDER__SINGLE) result = true;
				else result = false;
				break;

			case CTPNode::BONDTYPE__DOUBLE:
				if (bond.getOrder() == Bond::ORDER__DOUBLE) result = true;
				else result = false;
				break;

			case CTPNode::BONDTYPE__TRIPLE:
				if (bond.getOrder() == Bond::ORDER__TRIPLE) result = true;
				else result = false;
				break;

			case CTPNode::BONDTYPE__QUADRUPLE:
				if (bond.getOrder() == Bond::ORDER__QUADRUPLE) result = true;
				else result = false;
				break;

			case CTPNode::BONDTYPE__AROMATIC:
				if (bond.getOrder() == Bond::ORDER__AROMATIC) result = true;
				else result = false;
				break;

			default:
				Log.error() << "ConnectedToPredicate::bondOrderMatch_(): " << endl
					<< "\tunknown bond type " << node.getBondType() << endl;
				result = false;
		}
		return result;

	}

	bool ConnectedToPredicate::find_(const Atom& atom, const ConnectedToPredicate::CTPNode* current, HashSet<const Bond*>& visited) const
		throw()
	{
		Atom* partner;
		const Bond* bond;
		Size verbosity = 1;

		if (current == 0)
		{
			Log.error() << "ConnectedToPredicate::find_(): " << endl
				<< "\tencountered NULL as node pointer, aborting." << endl;
			return(false);
		}

		CTPNode::ConstIterator child_it = current->begin();
		for (; child_it != current->end(); ++child_it)
		{
			bool this_result = false;

			for (Size j = 0; j < atom.countBonds(); ++j)
			{
				bond = atom.getBond(j);
				if (visited.has(bond) == false)
				{
					if (bondOrderMatch_(*bond, **child_it) == true)
					{
						partner = bond->getPartner(atom);
						if (((*child_it)->getSymbol() == "*")
							|| (partner->getElement().getSymbol() == (*child_it)->getSymbol()))
						{
							visited.insert(bond);
							this_result = find_(*partner, *child_it, visited);
							if (this_result == true)
							{
								if (verbosity > 90)
								{
									Log.info() << "found " 
										<< partner->getElement().getSymbol() << endl;
								}
								break;
							}
							else
							{
								visited.erase(bond);
							}
						}
					}
				}
			}
			if (this_result == false)
			{
				return(false);
			}
		}
		return(true);
	}

	void ConnectedToPredicate::dump() const
		throw()
	{
		Log.info() << endl;
		dump(tree_);
		Log.info() << flush << endl << endl;
	}

	void ConnectedToPredicate::dump(const ConnectedToPredicate::CTPNode* current) const
		throw()
	{
		if (current == 0)
		{
			Log.error() << "dump(): got 0" << endl;
			return;
		}
		if (current->isLinked())
		{
			Log.info() << "@{" << current << "}";
		}
		Log.info() << current->getBondTypeChar() << current->getSymbol() << flush;
		list<const CTPNode*>::const_iterator it = current->getLinkList().begin();
		for(; it != current->getLinkList().end(); ++it)
		{
			Log.info() << "@[" << *it << "]";
		}
		CTPNode::ConstIterator child_it = current->begin();
		for (; child_it != current->end(); ++child_it)
		{
			Log.info() << "(" << flush;
			dump(*child_it);
			Log.info() << ")" << flush;
		}
	}

	void ConnectedToPredicate::setArgument(const String& argument)
		throw()
	{
		argument_ = argument;
		if (tree_ != 0)
		{
			delete(tree_);
		}
		tree_ = parse_(argument_);
		link_map_.clear();
		link_mark_ = 0;
		// DEBUG
		// dump();
		// /DEBUG
	}

	bool ConnectedToPredicate::operator () (const Atom& atom) const
		throw()
	{
		HashSet<const Bond*> visited;
		return find_(atom, tree_, visited);
	}


	bool SpHybridizedPredicate::operator () (const Atom& atom) const
		throw()
	{
		int dcount = 0;
		int tcount = 0;
		Size i;
		for (i = 0; i < atom.countBonds(); ++i)
		{
			if ((atom.getBond(i))->getOrder() == Bond::ORDER__DOUBLE)
			{
				dcount++;
			}
			if ((atom.getBond(i))->getOrder() == Bond::ORDER__TRIPLE)
			{
				tcount++;
			}
		}
		if ((dcount == 2) || (tcount == 1))
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	bool Sp2HybridizedPredicate::operator () (const Atom& atom) const
		throw()
	{
		int dcount = 0;
		int acount = 0;
		Size i;
		for (i = 0; i < atom.countBonds(); ++i)
		{
			if ((atom.getBond(i))->getOrder() == Bond::ORDER__DOUBLE)
			{
				dcount++;
			}
			if ((atom.getBond(i))->getOrder() == Bond::ORDER__AROMATIC)
			{
				acount++;
			}
		}
		if ((dcount == 1) || (acount > 1))
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	bool Sp3HybridizedPredicate::operator () (const Atom& atom) const
		throw()
	{
		Size i;
		if (atom.countBonds() != 4)
		{
			if (atom.getElement() == PTE[Element::O])
			{
				if (atom.countBonds() == 2)
				{
					return true;
				}
			}
			return false;
		}
		else
		{
			for (i = 0; i < atom.countBonds(); ++i)
			{
				if ((atom.getBond(i))->getOrder() != Bond::ORDER__SINGLE)
				{
					return false;
				}
			}
			return true;
		}
	}


	bool ChargePredicate::operator () (const Atom& atom) const
		throw()
	{
		String s = argument_;
		s.trim();

		float crg = atom.getCharge();
		bool result = true;

		if (s.hasPrefix("<="))
		{
			result = (crg <= s.after("<=").toString().toFloat());
		}
		else if (s.hasPrefix(">="))
		{
			result = (crg >= s.after(">=").toString().toFloat());
		}
		else if (s.hasPrefix("<"))
		{
			result = (crg < s.after("<").toString().toFloat());
		}
		else if (s.hasPrefix(">"))
		{
			result = (crg > s.after(">").toString().toFloat());
		}
		else if (s.hasPrefix("="))
		{
			result = (fabs(crg - s.after("=").toString().toFloat()) < Constants::EPSILON);
		}
		else					
		{
			result = (fabs(crg - s.toFloat()) < Constants::EPSILON);
		}

		return result;
	}


} // namespace BALL

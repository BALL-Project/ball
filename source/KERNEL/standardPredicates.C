// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: standardPredicates.C,v 1.58.24.1 2007/03/25 22:00:23 oliver Exp $
//

#include <BALL/KERNEL/standardPredicates.h>

#include <BALL/KERNEL/atom.h>
#include <BALL/KERNEL/PTE.h>
#include <BALL/KERNEL/residue.h>
#include <BALL/KERNEL/protein.h>
#include <BALL/KERNEL/chain.h>
#include <BALL/KERNEL/secondaryStructure.h>
#include <BALL/KERNEL/nucleotide.h>
#include <BALL/KERNEL/nucleicAcid.h>
#include <BALL/KERNEL/bond.h>

// #define DEBUG

namespace BALL 
{
	// True predicate

	bool TruePredicate::operator () (const Atom& /* atom */) const
	{
    return true;
 	}

	// False predicate

	bool FalsePredicate::operator () (const Atom& /* atom */) const
	{
    return false;
 	}

	// selected predicate

	bool SelectedPredicate::operator () (const Atom& atom) const
	{
    return atom.isSelected();
 	}

	// Atom name predicate

	bool AtomNamePredicate::operator () (const Atom& atom) const
	{
    return (atom.getName() == argument_);
 	}

	// Atom type predicate

	bool AtomTypePredicate::operator () (const Atom& atom) const
	{
		return (atom.getTypeName() == argument_);
 	}


	// element predicate

	bool ElementPredicate::operator () (const Atom& atom) const
	{
		return (atom.getElement().getSymbol() == argument_);
	}

	// residue predicate

	bool ResiduePredicate::operator () (const Atom& atom) const
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
	{
    const Residue*	res = atom.getAncestor(RTTI::getDefault<Residue>());
		if (res != 0)
		{
			if (!argument_.has('-'))
			{
				return (res->getID() == argument_);
			}
			else	
			{
				try
				{
					Size first = argument_.before("-").toString().toUnsignedInt();
					Size last = argument_.after("-").toString().toUnsignedInt();
					Size idx = res->getID().toUnsignedInt();
					return (idx >= first && idx <= last);
				}
				catch(...)
				{
					Log.error() << "ResidueIDPredicate::operator () (): "
						<< "argument could not be parsed: " << argument_ << std::endl;
					return(false);
				}
			}
		}
		
		return false;
	}
	
	// protein predicate

	bool ProteinPredicate::operator () (const Atom& atom) const
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
	{
 		const SecondaryStructure* sec_struct 
			= atom.getAncestor(RTTI::getDefault<SecondaryStructure>());
		return ((sec_struct != 0) && (sec_struct->getName() == argument_));
	}
	
	// solvent predicate

	bool SolventPredicate::operator () (const Atom& atom) const
	{
		const Molecule* molecule = atom.getMolecule();
		return ((molecule != 0) && (molecule->hasProperty(Molecule::IS_SOLVENT)));
	}
	
	// molecule predicate

	bool MoleculePredicate::operator () (const Atom& atom) const
	{
		const Molecule* molecule = atom.getMolecule();
		return ((molecule != 0) && (molecule->getName() == argument_));
	}

	// backbone predicate

	bool BackBonePredicate::operator () (const Atom& atom) const
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
	{
        return RTTI::isKindOf<Nucleotide>(&atom);
	}

	// in ring predicate

	InRingPredicate::InRingPredicate()
		: ExpressionPredicate()
	{
	}

	InRingPredicate::InRingPredicate(Size n)
		: ExpressionPredicate()
	{
		String argument(n);
		setArgument(argument);
	}

	InRingPredicate::~InRingPredicate()
	{
	}

	bool InRingPredicate::operator () (const Atom& atom) const
	{
		// the size of the ring to be found
		int n;

		if (argument_.size() == 1)
		{
			if (argument_.isDigit())
			{
				try
				{
					n = argument_.toInt();
				}
				catch (Exception::InvalidFormat& e)
				{
					Log.error() << "InRingPredicate::operator () (): "
						<< "argument format is broken: " << argument_ << std::endl;
					return(false);
				}
				// There are no rings with less than 3 atoms
				if (n < 3) 
				{
					return false;
				}
			}
			else
			{
				Log.error() << "InRingPredicate::operator () (): "
					<< "Expected a number < 9: " << argument_ << std::endl;
				return false;
			}
		}
		else
		{
			if (argument_ == "")
			{
				n = 0;
			}
			else
			{
				Log.error() << "InRingPredicate::operator () (): "
					<< "Expected a number < 9: " << argument_ << std::endl;
				return false;
			}
		}

		// if the atom does not have bonds, we cannot find any rings.
		if (atom.countBonds() > 0)
		{
			RingFinder find_my_ring(n);

			if (find_my_ring(atom) == true)
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		else
		{
			return(false);
		}

	}

	NumberOfBondsPredicate::NumberOfBondsPredicate()
	{
		argument_ = ">0";
	}

	bool NumberOfBondsPredicate::testPredicate_(const Atom& atom, 
			Bond::Order order) const
	{
		String s = argument_;
		s.trim();

		if (s.size() > 2)
		{
			// There can only be an operator followed by a number < 9
			Log.error() << "NumberOfBondsPredicate::testPredicate_(): "
				<< "argument_ too long " << std::endl;
			return false;
		}
		
		Size count = 0;
		Size i = 0;
		for (; i < atom.countBonds(); ++i)
		{
			if (order == Bond::ORDER__ANY) count++;
			else
			{
				if ((atom.getBond(i))->getOrder() == order)
				{
					count++;
				}
			}
		}

		Size n;
		if (s.size() == 2)
		{
			try
			{
				n = ((String) s[1]).toInt();
			}
			catch (Exception::InvalidFormat& e)
			{
				Log.error() << "NumberOfBondsPredicate::testPredicate_(): "
					<< "argument format is broken: " << argument_ << std::endl;
				return(false);
			}
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
					Log.error() << "NumberOfBondsPredicate::testPredicate_(): Illegal operator " 
						<< s[0] << std::endl;
					return false;
			}
		}
		else 
		{
			try
			{
				n = ((String) s[0]).toInt();
			}
			catch (Exception::InvalidFormat& e)
			{
				Log.error() << "NumberOfBondsPredicate::testPredicate_(): "
					<< "argument format is broken: " << argument_ << std::endl;
				return(false);
			}
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

	bool NumberOfBondsPredicate::operator () (const Atom& atom) const
	{
		return testPredicate_(atom, Bond::ORDER__ANY);
	}

	bool SingleBondsPredicate::operator () (const Atom& atom) const
	{
		return testPredicate_(atom, Bond::ORDER__SINGLE);
	}

	bool DoubleBondsPredicate::operator () (const Atom& atom) const
	{
		return testPredicate_(atom, Bond::ORDER__DOUBLE);
	}

	bool TripleBondsPredicate::operator () (const Atom& atom) const
	{
		return testPredicate_(atom, Bond::ORDER__TRIPLE);
	}

	bool AromaticBondsPredicate::operator () (const Atom& atom) const
	{
		return testPredicate_(atom);
	}

	bool AromaticBondsPredicate::testPredicate_(const Atom& atom) const
	{
		String s = argument_;
		s.trim();

		if (s.size() > 2)
		{
			// There can only be an operator followed by a number < 9
			Log.error() << "AromaticBondsPredicate::operator () (): "
				<< "argument_ too long " << std::endl;
			return false;
		}
		
		Size count = 0;
		Size i = 0;
		for (; i < atom.countBonds(); ++i)
		{
			const Bond* bond = atom.getBond(i);

			if (bond->isAromatic())
				count++;
		}

		Size n;
		if (s.size() == 2)
		{
			try
			{
				n = ((String) s[1]).toInt();
			}
			catch (Exception::InvalidFormat& e)
			{
				Log.error() << "AromaticBondsPredicate::testPredicate_(): "
					<< "argument format is broken: " << argument_ << std::endl;
				return(false);
			}
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
					Log.error() << "AromaticBondsPredicate::testPredicate_(): Illegal operator " 
						<< s[0] << std::endl;
					return false;
			}
		}
		else 
		{
			try
			{
				n = ((String) s[0]).toInt();
			}
			catch (Exception::InvalidFormat& e)
			{
				Log.error() << "AromaticBondsPredicate::testPredicate_(): "
					<< "argument format is broken: " << argument_ << std::endl;
				return(false);
			}
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
		:	element_symbol_("<uninitialized>"),
			bond_type_(BONDTYPE__UNINITIALISED),
			children_(),
			parent_(0),
			finished_(false),
			linked_(false),
			link_set_()
	{
	}

	ConnectedToPredicate::CTPNode::CTPNode(const ConnectedToPredicate::CTPNode& node)
		:	element_symbol_(node.element_symbol_),
			bond_type_(node.bond_type_),
			children_(node.children_),
			parent_(node.parent_),
			finished_(node.finished_),
			linked_(node.linked_),
			link_set_(node.link_set_)
	{
	}

	ConnectedToPredicate::CTPNode::~CTPNode()
	{
		destroy();
	}

	void ConnectedToPredicate::CTPNode::destroy()
	{
		// This is a bit weird. The situation is the following: If the current
		// iterator (denoting a child!) is pointing to something which is in
		// the link list of this node, then we will not delete it, because the
		// node will be deleted from its origianl parent node. So we do not
		// want to delete a partial "tree" which might be used by someone else
		// (which is highly improbable, but nevertheless possible.

		// First we have to remove links to the lower part of the tree
		if (link_set_.size() != 0)
		{
			HashSet<CTPNode*>::iterator it = link_set_.begin();
			for (; +it; ++it)
			{
				removeChild(*it);
			}
			// Now clear the link_list
			link_set_.clear();
		}

		// Now delete those children we really are parents of
		for (Iterator it = begin(); it != end(); ++it) 
		{
			delete *it;
		}
		
		// clear the children list
		children_.clear();
	}

	void ConnectedToPredicate::CTPNode::setParent(ConnectedToPredicate::CTPNode* parent)
	{
		if (parent == 0)
		{
			Log.error() << "ConnectedToPredicate::CTPNode::setParent(): "
				<< "Trying to set NULL as parent. Ignoring." << std::endl;
		}
		else
		{
			parent_ = parent;
		}
	}

	ConnectedToPredicate::CTPNode* ConnectedToPredicate::CTPNode::getParent() const
	{
		return parent_;
	}

	void ConnectedToPredicate::CTPNode::addChild(ConnectedToPredicate::CTPNode* child)
	{
		if (child == 0)
		{
			Log.error() << "ConnectedToPredicate::CTPNode::addChild(): "
				<< "Trying to add NULL as child. Ignoring." << std::endl;
		}
		else
		{
			children_.push_back(child);
		}
	}

	ConnectedToPredicate::CTPNode::Iterator ConnectedToPredicate::CTPNode::begin()
	{
		return children_.begin();
	}

	ConnectedToPredicate::CTPNode::ConstIterator ConnectedToPredicate::CTPNode::begin() const
	{
		return children_.begin();
	}

	ConnectedToPredicate::CTPNode::Iterator ConnectedToPredicate::CTPNode::end()
	{
		return children_.end();
	}

	ConnectedToPredicate::CTPNode::ConstIterator ConnectedToPredicate::CTPNode::end() const
	{
		return children_.end();
	}

	void ConnectedToPredicate::CTPNode::removeChild(CTPNode* child)
	{
		Iterator it = std::find(begin(), end(), child);
		if (it != end())
		{
			children_.erase(it);
		}
	}

	::std::list<ConnectedToPredicate::CTPNode*>& ConnectedToPredicate::CTPNode::getChildren()
	{
		return(children_);
	}

	Size ConnectedToPredicate::CTPNode::getNumberOfChildren() const
	{
		return children_.size();
	}

	void ConnectedToPredicate::CTPNode::setBondType(Size type)
	{
		bond_type_ = type;
	}

	void ConnectedToPredicate::CTPNode::setBondType(char type)
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
					<< "Unknown bond type character, defaulting to <any>." << std::endl;
				bond_type_ = BONDTYPE__ANY;
		}
	}

	Size ConnectedToPredicate::CTPNode::getBondType() const
	{
		return bond_type_;
	}

	char ConnectedToPredicate::CTPNode::getBondTypeChar() const
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
	{
		return element_symbol_;
	}

	void ConnectedToPredicate::CTPNode::setSymbol(const String& symbol)
	{
		element_symbol_ = symbol;
	}

	void ConnectedToPredicate::CTPNode::setFinished()
	{
		finished_ = true;
	}

	void ConnectedToPredicate::CTPNode::unsetFinished()
	{
		finished_ = false;
	}

	bool ConnectedToPredicate::CTPNode::isFinished() const
	{
		return finished_;
	}

	void ConnectedToPredicate::CTPNode::setLinked()
	{
		linked_ = true;
	}

	void ConnectedToPredicate::CTPNode::unsetLinked()
	{
		linked_ = false;
	}

	bool ConnectedToPredicate::CTPNode::isLinked() const
	{
		return linked_;
	}

	void ConnectedToPredicate::CTPNode::linkWith(ConnectedToPredicate::CTPNode* partner)
	{
		if (partner == 0)
		{
			Log.error() << "ConnectedToPredicate::CTPNode::linkWith(): "
				<< "Trying to link with NULL. Ignoring." << std::endl;
			return;
		}
		partner->link_set_.insert(this);
		partner->addChild(this);
		setLinked();
	}

	const HashSet<ConnectedToPredicate::CTPNode*>& ConnectedToPredicate::CTPNode::getLinkSet() const
	{
		return link_set_;
	}

	ConnectedToPredicate::ConnectedToPredicate()
		:	tree_(0),
			link_map_(),
			link_mark_(0)
	{
	}

	ConnectedToPredicate::ConnectedToPredicate(const ConnectedToPredicate& predicate)
		:	ExpressionPredicate(predicate),
			tree_(0),
			link_map_(predicate.link_map_),
			link_mark_(predicate.link_mark_)
	{
		// we have to copy the whole tree
		// fast solution: parse_() again.
		if (argument_ != "")
		{
			tree_ = parse_();
		}
	}

	ConnectedToPredicate::~ConnectedToPredicate()
	{
		if (tree_ != 0) 
		{
			delete tree_;
		}
	}
			
	ConnectedToPredicate::CTPNode* ConnectedToPredicate::createNewNode_(ConnectedToPredicate::CTPNode* node)
	{
		// PARANOIA
		if (node == 0)
		{
			Log.error() << "ConnectedToPredicate::createNewNode_: "
				<< "got NULL as argument" << std::endl;
			return(0);
		}
		// /PARANOIA

		// whenever we create a new node, the old one is finished.
		node->setFinished();
		CTPNode* child = new CTPNode;

		// PARANOIA
		if (child == 0)
		{
			Log.error() << "ConnectedToPredicate::createNewNode_: "
				<< "Could not create a child node" << std::endl;
			return(0);
		}
		// /PARANOIA

		child->setParent(node);
		node->addChild(child);

		// if there is a link mark between this node an its child, create an
		// entry in the link map and mark the child as linked. The link partner
		// is still unknown.
		if (link_mark_ != 0)
		{
			std::pair<CTPNode*, CTPNode*> tmp(child, (CTPNode*)0);
			link_map_.insert(std::pair<char, std::pair<CTPNode*, CTPNode*> >(link_mark_, tmp));
			link_mark_ = 0;
		}
		return child;
	}

	ConnectedToPredicate::CTPNode* ConnectedToPredicate::parse_() 
	{
		return(parse_(argument_));
	}

	ConnectedToPredicate::CTPNode* ConnectedToPredicate::parse_(const String& input) 
	{
		Size depth = 0;
		int bracket_count = 0;
		Size position = 0;
		CTPNode* current = 0;
		Size verbosity = 0;
		std::list<CTPNode*> all_nodes;
		std::vector<CTPNode*> bracket_stack;

		CTPNode* root = new CTPNode;
		
		// PARANOIA
		if (root == 0)
		{
			Log.error() << "ConnectedToPredicate::parse_(): "
				<< "could not create the root node" << std::endl;
			return(0);
		}
		// /PARANOIA
		
		root->setSymbol("/");
		current = root;
		all_nodes.push_back(root);

		String bond_chars(".-=#%~");
		String uppercase("ABCDEFGHIJKLMNOPQRSTUVWXYZ");
		String lowercase("abcdefghijklmnopqrstuvwxyz");
		String numbers("12345678");

		for (; position < input.size(); ++position)
		{
			// PARANOIA
			if (current == 0)
			{
				Log.error() << ""
					<< "current is NULL at beginning of for loop." << std::endl;
			}
			// /PARANOIA

			if (verbosity > 90)
			{
				Log.info() << "Examining character " << input[position] << std::endl;
			}

			if (input[position] == '(')
			{
				if ((position > 0) && bond_chars.has(input[position - 1]))
				{
					Log.error() << "ConnectedToPredicate:parse_(): " << std::endl
						<< "\tparse error: bond char before bracket." << std::endl;
					return 0;
				}
				bracket_stack.push_back(current);
				current = createNewNode_(current);
				all_nodes.push_back(current);
				depth++;
				bracket_count++;
				if (verbosity > 90)
				{
					Log.info() << "Found (, created new node, new depth is " << depth
						<< std::endl;
				}
			}
			else
			{
				if (input[position] == ')')
				{
					if (current == 0)
					{
						Log.error() << "ConnectedToPredicate::parse_():\n"
							<< "\ttried to access a NULL pointer. Aborting." << std::endl;
						return 0;
					}
					current->setFinished();
					depth--;
					bracket_count--;
					if (verbosity > 90)
					{
						Log.info() << "Found ), new depth is " << depth
							<< std::endl;
					}
					if (!bracket_stack.empty())
					{
						current = bracket_stack.back();
						bracket_stack.pop_back();
					}
					else
					{
						Log.error() << "ConnectedToPredicate::parse_():\n"
							<< "\tparse error: missing opening bracket." << std::endl;

						// If the current node was already integrated into the tree,
						// deleting the root will take care of it. Otherwise kill it
						// manually
						if (current->getParent() == 0) delete current;
						current = 0;
						// Delete the tree
						delete root;
						root = 0;

						return 0;
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
								Log.info() << "Tried to set bond type of finished node, created a new one, new depth is " << depth << std::endl;
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
										<< std::endl;
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
									Log.error() << "ConnectedToPredicate::parse_():\n"
										<< "\tparse error: trying to add a lowercase char to an already finished node." 
										<< std::endl;
									return(0);
								}
								String symbol = current->getSymbol();
								if (symbol.size() != 1)
								{
									Log.error() << "ConnectedToPredicate::parse_():\n"
										<< "\tparse error: trying to add a lowercase char to a symbol with length != 1." 
										<< std::endl;
									return(0);
								}
								if (symbol == '*')
								{
									Log.error() << "ConnectedToPredicate::parse_():\n"
										<< "\tparse error: trying to add a lowercase char to a \"*\"." 
										<< std::endl;
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
												Log.error() << "ConnectedToPredicate::parse_():\n"
													<< "\tparse error: triple mark: " << link_mark_ << std::endl;
												return(0);
											}
											link_map_[link_mark_].second = current;
											CTPNode* first = link_map_[link_mark_].first;
											if (first == 0) first = root;
											current->linkWith(first);
										}
									}
									else
									{
										Log.error() << "ConnectedToPredicate::parse_():\n"
											<< "\tparse error: only numbers are allowed as marks." << std::endl;
										return(0);
									}
								}
								else
								{
									if (numbers.has(input[position]))
									{
										char n = input[position];
										Size N((Size)n - (Size)'0');
										CTPNode* parent = current->getParent();
										if (parent == 0)
										{
											Log.error() << "ConnectedToPredicate::parse_():\n"
												<< "\tmultiplier without parent. Aborting."
												<< std::endl;
											return(0);
										}
										CTPNode* new_child = 0;
										for (Size i = 0; i < (N - 1); ++i)
										{
											new_child = new CTPNode(*current);
											all_nodes.push_back(new_child);
											parent->addChild(new_child);
											// The following could be unnecessary (taken care of
											// ba copy constructor)
											new_child->setParent(parent);
										}
										current = parent;
									}
									else
									{
										Log.error() << "ConnectedToPredicate::parse_():\n"
											<< "\tparse error: unknown input char: " 
											<< input[position] << std::endl;
										return 0;
									}
								}
							}
						}
					}
				}
			}
		}

		// check for errors

		if (bracket_count > 0)
		{
			Log.error() << "ConnectedToPredicate::parse_():\n"
				<< "\tparse error: too many opening brackets." << std::endl;
			if (current->getParent() == 0) delete current;
			current = 0;
			delete root;
			root = 0;
			return(0);
		}

		if (bracket_count < 0)
		{
			Log.error() << "ConnectedToPredicate::parse_():\n"
				<< "\tparse error: Too many closing brackets." << std::endl;
			if (current->getParent() == 0) delete current;
			current = 0;
			delete root;
			root = 0;
			return(0);
		}

		// Sort the nodes in order to have 
		std::list<CTPNode*>::iterator sort_it = all_nodes.begin();
		for (; sort_it != all_nodes.end(); ++sort_it)
		{

			// This is absolutely low tech. But it works.

			::std::list<CTPNode*> non_stars;
			::std::list<CTPNode*> stars;

			::std::list<CTPNode*>& children = (*sort_it)->getChildren();

			CTPNode::Iterator child_it = children.begin();
			for (; child_it != children.end(); ++child_it)
			{
				if ((*child_it)->getSymbol() == "*")
				{
					stars.push_back(*child_it);
				}
				else
				{
					non_stars.push_back(*child_it);
				}
			}

			if (stars.size() > 0)
			{
				children.clear();

				child_it = non_stars.begin();
				for (; child_it != non_stars.end(); ++child_it)
				{
					children.push_back(*child_it);
				}
			
				child_it = stars.begin();
				for (; child_it != stars.end(); ++child_it)
				{
					children.push_back(*child_it);
				}
			
				/* I don't know why, but this didn't work.
				::std::list<CTPNode*>::iterator it = children.begin();
				copy(non_stars.begin(), non_stars.end(), it);
				copy(stars.begin(), stars.end(), it);
				*/

			}

		}

		tree_ = root;

		return(root);

	}

	bool ConnectedToPredicate::bondOrderMatch_(const Bond& bond, const ConnectedToPredicate::CTPNode& node) const
	{

		bool result = false;

		switch (node.getBondType())
		{
			case CTPNode::BONDTYPE__UNINITIALISED:
				Log.error() << "ConnectedToPredicate::bondOrderMatch_():\n"
					<< "\tuninitialized bond. Returning false." << std::endl;
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
				if (bond.isAromatic()) result = true;
				else result = false;
				break;

			default:
				Log.error() << "ConnectedToPredicate::bondOrderMatch_():\n"
					<< "\tunknown bond type " << node.getBondType() << std::endl;
				result = false;
		}
		return result;

	}

	bool ConnectedToPredicate::find_(const Atom& atom, const ConnectedToPredicate::CTPNode* current, HashSet<const Bond*>& visited) const
	{
		Atom* partner;
		const Bond* bond;
		Size verbosity = 0;

		AxialPredicate isAxial;
		InRingPredicate inRing;

		if (current == 0)
		{
			// Log.error() << "ConnectedToPredicate::find_():\n"
			//	<< "\tencountered NULL as node pointer, aborting." << std::endl;
			return(false);
		}

		// Iterate over all children in order to find a matching one.
		CTPNode::ConstIterator child_it = current->begin();
		for (; child_it != current->end(); ++child_it)
		{

			// This variable contains the "local" result which will be merged
			// with those from deeper tree levels
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
								|| (partner->getElement().getSymbol() == (*child_it)->getSymbol())
								|| (((*child_it)->getSymbol() == "E") 
										&& (partner->getElement().getElectronegativity()
											> atom.getElement().getElectronegativity()))
								|| (((*child_it)->getSymbol() == "A") 
										&& (isAxial(*partner)))
								|| (((*child_it)->getSymbol() == "R") 
										&& (inRing(*partner))))
						{
							visited.insert(bond);
							this_result = find_(*partner, *child_it, visited);
							if (this_result == true)
							{
								if (verbosity > 90)
								{
									Log.info() << "found " 
										<< partner->getElement().getSymbol() << std::endl;
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

		// Everything went OK, so return true.
		return(true);
	}

	void ConnectedToPredicate::dump() const
	{
		Log.info() << std::endl;
		dump(tree_);
		Log.info() << std::flush << std::endl << std::endl;
	}

	void ConnectedToPredicate::dump(const ConnectedToPredicate::CTPNode* current) const
	{
		if (current == 0)
		{
			Log.error() << "ConnectedToPredicate::dump(): got 0" << std::endl;
			return;
		}
		if (current->isLinked())
		{
			Log.info() << "@{" << current << "}";
		}
		Log.info() << current->getBondTypeChar() << current->getSymbol() 
			<< std::flush;
		HashSet<CTPNode*>::ConstIterator it = current->getLinkSet().begin();
		for(; it != current->getLinkSet().end(); ++it)
		{
			Log.info() << "@[" << *it << "]";
		}
		CTPNode::ConstIterator child_it = current->begin();
		for (; child_it != current->end(); ++child_it)
		{
			if (!current->getLinkSet().has(*child_it))
			{
				Log.info() << "(" << std::flush;
				dump(*child_it);
				Log.info() << ")" << std::flush;
			}
		}
	}

	void ConnectedToPredicate::clear()
	{
		if ( tree_ != 0 ) delete tree_;
		tree_ = 0;
		link_map_.clear();
		link_mark_ = 0;
	}

	void ConnectedToPredicate::setArgument(const String& argument)
	{
		clear();
		argument_ = argument;
		tree_ = parse_(argument_);
	}

	bool ConnectedToPredicate::operator () (const Atom& atom) const
	{
		HashSet<const Bond*> visited;
		return find_(atom, tree_, visited);
	}


	bool SpHybridizedPredicate::operator () (const Atom& atom) const
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
			if ((atom.getBond(i))->isAromatic())
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


	bool AxialPredicate::operator () (const Atom& atom) const
	{

#ifdef DEBUG
		std::cout << "ATOM: " << atom.getFullName() << std::endl;
#endif

		// if it's not a carbon, go home.
		if (atom.getElement() != PTE[Element::C]) 
		{
#ifdef DEBUG
			std::cout << "No carbon." << std::endl;
#endif
			return(false);
		}

		// if atom's not sp3, go away.
		Sp3HybridizedPredicate is_sp3;
		if (!is_sp3(atom)) 
		{
#ifdef DEBUG
			std::cout << "Not sp3." << std::endl;
#endif
			return(false);
		}

		// make sure we are in a 5 or 6 memebered ring.
		RingFinder in_ring;
		if (!in_ring(atom) == true)
		{
#ifdef DEBUG
			std::cout << "Not in a ring." << std::endl;
#endif
			return(false);
		}

		const vector<const Atom*>& ring_atoms = in_ring.getRingAtoms();
		const HashSet<const Bond*>& visited_bonds = in_ring.getVisitedBonds();

		if ((ring_atoms.size() < 5) || (ring_atoms.size() > 6))
		{
#ifdef DEBUG
			std::cout << "Wrong ring size " << ring_atoms.size() << "." << std::endl;
#endif
			return(false);
		}

		// find all the atoms we need.

		// the variable name C5 suggests that this routine can only handle
		// six-membered rings. that's not true, five-membered rings are also
		// possible. it is questionable, on the other hand, whether 5-rings
		// make sense at all...

		const Atom* C1 = &atom;
		Vector3 c1 = C1->getPosition();
		const Atom* C3;
		Vector3 c3;
		const Atom* C5;
		Vector3 c5;
		const Atom* H = 0;
		Vector3 h;
		const Atom* R = 0;
		Vector3 r;

		// it is actually not really necessary to know which one is C3 and
		// which one C5 but it makes sign determination easier.
		
		C3 = ring_atoms[ring_atoms.size() - 3];
		c3 = C3->getPosition();
		C5 = ring_atoms[1];
		c5 = C5->getPosition();
#ifdef DEBUG
		std::cout << "C3: " << C3->getFullName() << std::endl;
		std::cout << "C5: " << C5->getFullName() << std::endl;
#endif
		
		// This code takes the hydrogen as means of measuring the angle. This
		// is NOT applicable for every sugar.

		// NOTE that this loop doesn't check whether this H is the *only* H of
		// this carbon.
		AtomBondConstIterator bond_it = atom.beginBond();
		for (; +bond_it; ++bond_it)
		{
			if (!visited_bonds.has(&*bond_it))
			{
				if ((H == 0)
						&& (bond_it->getPartner(atom)->getElement() == PTE[Element::H]))
				{
					H = bond_it->getPartner(atom);
					h = H->getPosition();
#ifdef DEBUG
					std::cout << "H: " << H->getFullName() << std::endl;
#endif
				}
				else
				{
					R = bond_it->getPartner(atom);
					r = R->getPosition();
#ifdef DEBUG
					std::cout << "R: " << R->getFullName() << std::endl;
#endif
				}
			}
		}

		// if we didn't find a hydrogen, we don't know how to find out its
		// position... ;)
		if (H == 0)
		{
#ifdef DEBUG
			std::cout << "No hydrogen neighbour." << std::endl;
#endif
			return(false);
		}

		// compute the angle between the normal vector of the plane and the
		// vector pointing from C1 to H.
		Vector3 n = (c1 - c3) % (c1 - c5);
		float angle_C1_H = n.getAngle(h - c1).toDegree();
		float angle_C1_R = n.getAngle(r - c1).toDegree();

		if (angle_C1_H > 90)
		{
			angle_C1_H -= 180;
			angle_C1_R -= 180;
		}

#ifdef DEBUG
		std::cout << "c1: " << c1 << std::endl;
		std::cout << "c3: " << c3 << std::endl;
		std::cout << "c5: " << c5 << std::endl;
		std::cout << "c1 - c3: " << c1 - c3 << std::endl;
		std::cout << "c1 - c5: " << c1 - c5 << std::endl;
		std::cout << "h: " << h << std::endl;
		std::cout << "h - c1: " << h - c1 << std::endl;
		std::cout << "Angle(C1, H): " << angle_C1_H << " " 
			<< fabs(angle_C1_H) << std::endl;
		std::cout << "Angle(C1, R): " << angle_C1_R << " "
			<< fabs(angle_C1_R) - 109.5 << std::endl;
#endif

		if ((fabs(angle_C1_H) < 15.0) 
				&& ((fabs(angle_C1_R) - 109.5) < 15.0))
		{
			// if the hydrogen stands in axial position and the substituent in
			// equatorial position the carbon is equatorially substituted
			return(false);
		}
		else
		{
			return(true);
		}

	}

	bool Conformation4C1Predicate::operator () (const Atom& atom) const
	{
		RingFinder in_6_ring(6);
		if (!in_6_ring(atom))
		{
			// if we are not in a six-membered ring, the predicate doesn't make
			// any sense.
			return(false);
		}

		vector<const Atom*> ring_atoms = in_6_ring.getRingAtoms();

		ConnectedToPredicate isC5;
		isC5.setArgument("(C)(C)(O)");

		Size O_index = 0;
		Size C1_index = 0;
		Size C2_index = 0;
		Size C5_index = 0;

		while ((ring_atoms[O_index]->getElement() != PTE[Element::O])
			&& (O_index < 6))
		{
			++O_index;
		}

		if (isC5(*(ring_atoms[((O_index == 5) ? 0 : (O_index + 1) % 6)])))
		{
			C1_index = (O_index == 0) ? 5 : (O_index - 1) % 6;
			C2_index = (C1_index == 0) ? 5 : (C1_index - 1) % 6;
			C5_index = (O_index == 5) ? 0 : (O_index + 1) % 6;
		}
		else
		{
			if (isC5(*(ring_atoms[((O_index == 0) ? 5 : (O_index - 1) % 6)])))
			{
				C1_index = (O_index == 5) ? 0 : (O_index + 1) % 6;
				C2_index = (C1_index == 5) ? 0 : (C1_index + 1) % 6;
				C5_index = (O_index == 0) ? 5 : (O_index - 1) % 6;
			}
			else
			{
				// could not find the sugar specific structures
				return(false);
			}
		}

		// now compute the halfspace of C1

		Vector3 c1 = ring_atoms[C1_index]->getPosition();
		Vector3 c2 = ring_atoms[C2_index]->getPosition();
		Vector3 c5 = ring_atoms[C5_index]->getPosition();
		Vector3 o = ring_atoms[O_index]->getPosition();
		Vector3 n = (c5 - o) % (c2 - o);
		Vector3 d = (c1 - o);

#ifdef DEBUG
		std::cout << "C1: " << ring_atoms[C1_index]->getFullName() << std::endl;
		std::cout << "C2: " << ring_atoms[C2_index]->getFullName() << std::endl;
		std::cout << "C5: " << ring_atoms[C5_index]->getFullName() << std::endl;
		std::cout << "O: " << ring_atoms[O_index]->getFullName() << std::endl;
		std::cout << "c1: " << c1 << std::endl;
		std::cout << "c2: " << c2 << std::endl;
		std::cout << "c5: " << c5 << std::endl;
		std::cout << "o: " << o << std::endl;
		std::cout << "n: " << n << std::endl;
		std::cout << "d: " << d << std::endl;
		std::cout << "d * n: " << d * n << std::endl;
#endif

		if ((d * n) > 0)
		{
			return(false);
		}
		else
		{
			return(true);
		}
	}


	RingFinder::RingFinder()
		: first_atom_(0),
			n_(0),
			exact_(true),
			visited_bonds_(),
			ring_atoms_()
	{
	}

	RingFinder::RingFinder(Size n)
		: first_atom_(0),
			n_(n),
			exact_(true),
			visited_bonds_(),
			ring_atoms_()
	{
	}

	RingFinder::~RingFinder()
	{
	}

	bool RingFinder::operator () (const Atom& atom)
	{
		exact_ = true;

		// do we search a ring of a certain number of members or just a ring?
		if (n_ == 0)
		{
			n_ = 99;
			exact_ = false;
		}
		else
		{
			if (n_ < 3) 
			{
				// there are no rings with less than three atoms
				return false;
			}
		}

		first_atom_ = &atom;
		visited_bonds_.clear();
		ring_atoms_.clear();

		bool result = dfs(atom, n_);

		return(result);
	}

	bool RingFinder::dfs(const Atom& atom, const Size limit)
	{
		// the following recursive function performs an ad-hoc dfs and returns
		// true, if a ring was found and false otherwise.

		if (exact_ == true)
		{
			if (limit == 0) 
			{
				if (atom == *first_atom_) 
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
		}
		else
		{
			if ((atom == *first_atom_) && (visited_bonds_.size() > 0))
			{
				return true;
			}
			if (limit == 0)
			{
				return false;
			}
		}

		Size i;
		const Bond* bond;
		Atom* descend;

		// Now iterate over all Bonds an store the visited bonds.

		for (i = 0; i < atom.countBonds(); ++i)
		{
			bond = atom.getBond(i);
			if (!visited_bonds_.has(bond))
			{
				descend = bond->getPartner(atom);
				visited_bonds_.insert(bond);
				if (dfs(*descend, limit-1) == true)
				{
// #ifdef DEBUG
// 					std::cout << "Pushing back " << atom.getFullName() << std::endl;
// #endif
					ring_atoms_.push_back(&atom);
					return true;
				}
				else
				{
					visited_bonds_.erase(bond);
				}
			}
		}
		// No partner matched
		return false;
	}

	void RingFinder::setRingSize(Size n)
	{
		n_ = n;
	}

	const HashSet<const Bond*>& RingFinder::getVisitedBonds() const
	{
		return visited_bonds_;
	}

	const vector<const Atom*>& RingFinder::getRingAtoms() const
	{
		return ring_atoms_;
	}

	/////////////////////////////////////////////////////////////////
	
	HashMap<Molecule*, TimeStamp> SMARTSPredicate::call_time_map_;
	Molecule SMARTSPredicate::dummy_molecule_;

	SMARTSPredicate::SMARTSPredicate()
		: ExpressionPredicate(),
			last_molecule_(0)
	{
		arom_proc_.options.setBool(AromaticityProcessor::Option::OVERWRITE_BOND_ORDERS, false);
	}

	SMARTSPredicate::SMARTSPredicate(const SMARTSPredicate& pred)
		: ExpressionPredicate(pred),
			last_molecule_(0),
			arom_proc_(pred.arom_proc_)
	{
	}

	SMARTSPredicate::~SMARTSPredicate()
	{
	}

	bool SMARTSPredicate::operator () (const Atom& atom) const
	{
		Molecule* mol = (Molecule*) atom.getAncestor(dummy_molecule_);
		if (mol == 0) return false;
		if (last_molecule_ == mol)
 		{
			return matches_.has((Atom*)&atom);
 		}

		HashMap<Molecule*, TimeStamp>::Iterator it = call_time_map_.find(mol);
		if ((it == call_time_map_.end()) || it->second.isOlderThan(mol->getModificationTime()))
		{
			mol->apply(ring_proc_);
			mol->apply(arom_proc_);
			TimeStamp stamp;
			stamp.stamp();
			call_time_map_[mol] = stamp;
		}

		matches_.clear();
		last_molecule_ = mol;
		vector<std::set<const Atom*> > result;

		try
		{
			matcher_.match(result, *mol, argument_);
		}
		catch(...)
		{
			Log.error() << "Problem in SMARTS expression: " << argument_ << std::endl;
			return false;
		}

		for (Position p = 0; p < result.size(); p++)
		{
			std::set<const Atom*>::const_iterator it = result[p].begin();
			for (; it != result[p].end(); ++it)
			{
				matches_.insert((Atom*)*it);
			}
		}

		return matches_.has((Atom*)&atom);
	}


} // namespace BALL

// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: smartsParser.C,v 1.7 2006/02/25 16:10:28 bertsch Exp $
//

#include <BALL/STRUCTURE/smartsParser.h>
#include <BALL/KERNEL/PTE.h>
#include <BALL/KERNEL/standardPredicates.h>

#include <algorithm>
#include <sstream>
#include <stack>

#define SMARTS_PARSER_DEBUG
#undef  SMARTS_PARSER_DEBUG


// defined in the lexer (smartsParserLexer.l)
extern void SmartsParser_initBuffer(const char* buf);
extern void SmartsParser_delBuffer();
extern int SmartsParserparse();

using namespace std;

namespace BALL
{
	/* 
	 * Implementation SPNode
	 */
	SmartsParser::SPNode::SPNode()
		:	internal_(false),
			is_not_(false),
			recursive_(false),
			in_brackets_(false),
			log_op_(SmartsParser::NOOP),
			first_edge_((SPEdge*)0),
			second_edge_((SPEdge*)0),
			sp_atom_((SPAtom*)0),
			component_no_(-1)
	{
	}

	SmartsParser::SPNode::SPNode(SPAtom* atom)
		:	internal_(false),
			is_not_(false),
			recursive_(false),
			in_brackets_(false),
			log_op_(SmartsParser::NOOP),
			first_edge_((SPEdge*)0),
			second_edge_((SPEdge*)0),
			sp_atom_(atom),
			component_no_(-1)
	{
		#ifdef SMARTS_PARSER_DEBUG
		cerr << "SmartsParser::SPNode::SPNode(SPAtom* " << atom << "), this=" << this << endl;
		#endif
	}

	SmartsParser::SPNode::SPNode(SPNode* first, LogicalOperator log_op, SPNode* second)
		:	internal_(true),
			is_not_(false),
			recursive_(false),
			in_brackets_(false),
			log_op_(log_op),
			first_edge_(new SmartsParser::SPEdge()),
			second_edge_(new SmartsParser::SPEdge()),
			sp_atom_((SPAtom*)0),
			component_no_(-1)
	{
		first_edge_->setFirstSPNode(this);
		first_edge_->setSecondSPNode(first);
		first_edge_->setInternal(true);
		second_edge_->setFirstSPNode(this);
		second_edge_->setSecondSPNode(second);
		second_edge_->setInternal(true);
	}

	SmartsParser::SPNode::~SPNode()
	{
	}

	void SmartsParser::SPNode::setRecursive(bool recursive)
	{
		for (EdgeIterator it = begin(); it != end(); ++it)
		{
			SmartsParser::state.current_parser->addRecursiveEdge(*it);
		}
		recursive_ = recursive;
	}

	/* 
	 * Implementation SPEdge
	 */
	SmartsParser::SPEdge::SPEdge()
		:	internal_(false),
			is_not_(false),
			first_((SmartsParser::SPNode*)0),
			second_((SmartsParser::SPNode*)0),
			bond_((SmartsParser::SPBond*)0),
			first_edge_((SmartsParser::SPEdge*)0),
			second_edge_((SmartsParser::SPEdge*)0),
			log_op_(SmartsParser::NOOP)
	{
		#ifdef SMARTS_PARSER_DEBUG
		cerr << "SmartsParser::SPEdge::SPEdge()" << endl;
		#endif
	}

	SmartsParser::SPEdge::~SPEdge()
	{
	}

	SmartsParser::SPBond::SPBond()
		:	ze_type_(SmartsParser::NONE),
			bond_order_(SmartsParser::SPBond::ANY),
			not_(false)
	{
		#ifdef SMARTS_PARSER_DEBUG
		cerr << "SmartsPArser::SPBond::SPBond()" << endl;
		#endif
	}

	bool SmartsParser::SPBond::equals(const Bond* bond) const
	{
		// TODO - Z/E isomer types
		// TODO errors in error log
		
		bool matches(false);
		switch (bond_order_)
		{
			case SINGLE:	
				if (bond->getOrder() == Bond::ORDER__SINGLE)
				{
					matches = true;
				}
				break;
			case SINGLE_UP:
				if (bond->getOrder() == Bond::ORDER__SINGLE)
				{
					matches = true;
					cerr << "chiral bond definitions are not implemented yet" << endl;
				}
				break;
			case SINGLE_UP_OR_ANY:
				if (bond->getOrder() == Bond::ORDER__SINGLE)
				{
					matches = true;
					cerr << "chiral bond definitions are not implemented yet" << endl;
				}
				break;
			case SINGLE_DOWN:
				if (bond->getOrder() == Bond::ORDER__SINGLE)
				{
					matches = true;
					cerr << "chiral bond definitions are not implemented yet" << endl;
				}
				break;
			case SINGLE_DOWN_OR_ANY:
				if (bond->getOrder() == Bond::ORDER__SINGLE)
				{
					matches = true;
					cerr << "chiral bond definitions are not implemented yet" << endl;
				}
				break;
			case DOUBLE:
				if (bond->getOrder() == Bond::ORDER__DOUBLE)
				{
					matches = true;
				}
				break;
			case TRIPLE:
				if (bond->getOrder() == Bond::ORDER__TRIPLE)
				{
					matches = true;
				}
				break;
			case AROMATIC: 
				if (bond->getOrder() == Bond::ORDER__AROMATIC)
				{
					matches = true;
				}
				break;
			case ANY:
				matches = true;
				break;
			case NOT_NECESSARILY_CONNECTED:
				matches = true;
				break;
			case IN_RING:
				if (bond->getProperty("InRing").getBool())
				{
					matches = true;
				}
				break;
			default:
				cerr << "unknown or not implemented bond order: " << bond_order_ << endl;
		}
		
		return not_ ? !matches : matches;
	}

	/*	
	 * Implementation of SPAtom
	 */
	SmartsParser::SPAtom::SPAtom()
	{
#ifdef SMARTS_PARSER_DEBUG
		cerr << "SmartsParser::SPAtom::SPAtom(), this=" << this << endl;
#endif
	}

	SmartsParser::SPAtom::SPAtom(const String& symbol)
	{
		#ifdef SMARTS_PARSER_DEBUG
		cerr << "SmartsParser::SPAtom::SPAtom(" << symbol << "), this=" << this << endl;
		#endif
		if (symbol != "*" && symbol != "")
		{
			String s(symbol);
			//cerr << "'" << s << "'" << endl;
			if (symbol.isDigit())
			{
				s = PTE[symbol.toUnsignedInt()].getSymbol();
			}
			if (islower(symbol[0]))
			{
				s.toUpper(0, 1);
				setProperty("Aromatic", true);
			}
			else
			{
				setProperty("Aliphatic", true);
			}
			setProperty("Symbol", s);
#ifdef SMARTS_PARSER_DEBUG
			cerr << "setting property symbol=" << s << endl;
#endif
		}
		else
		{
			setProperty("Symbol", "");
		}
	}
	
	SmartsParser::SPAtom::~SPAtom()
		throw()
	{
	}
	
	void SmartsParser::SPAtom::addAtomProperty(NamedProperty property) throw(Exception::ParseError)
	{
#ifdef SMARTS_PARSER_DEBUG
		cerr << "void SmartsParser::SPAtom::addAtomProperty(NamedProperty " << property.getName() << ")" << endl;
#endif
		if (!hasProperty(property.getName()))
		{
			setProperty(property);
		}
		else
		{
			stringstream s;
			property.dump(s);
			throw Exception::ParseError(__FILE__, __LINE__, String(s), "");
		}
		return;
	}

	Size SmartsParser::SPAtom::getDefaultValence(const Atom* atom) const
	{
		Size an(0);
		//if (getProperty("Symbol").getString() != "")
		//{
		//	an = PTE.getElement(getProperty("Symbol").getString()).getAtomicNumber();
		//}
		an = atom->getElement().getAtomicNumber();
			
		switch (an)
		{
			case  1: return 1; // hydrogen
			case  5: return 3; // boron
			case  6: return 4; // carbon
			case  7: return 3; // nitrogen
			case  8: return 2; // oxygen
			case 15: return 3; // phosphorus
			case 16: return 2; // sulfur
			case  9:
			case 17:
			case 35:
			case 53: return 1; // halogens
			default:
				cerr << "SP: default valence not defined of " << an << endl;
				break;
		};
		return 0;
	}

	Size SmartsParser::SPAtom::countRealValences(const Atom* atom) const
	{
		double count(0);
		for (Atom::BondConstIterator bit = atom->beginBond(); +bit; ++bit)
		{
			switch(bit->getOrder())
			{
				case Bond::ORDER__SINGLE:		
					count += 1; 
					break;
				case Bond::ORDER__AROMATIC: 
					count += 1.5; 
					break;
				case Bond::ORDER__DOUBLE:		
					count += 2;
					break;
				case Bond::ORDER__TRIPLE:		
					count += 3;
					break;
				default:
					// TODO
					cerr << "error: bond order (" << bit->getOrder() << ")" << endl;
			}
		}
		return Size(count);
	}

	Size SmartsParser::SPAtom::getNumberOfImplicitHydrogens(const Atom* atom) const
	{
		// TODO charges?
		return getDefaultValence(atom) - countRealValences(atom);
	}

	bool SmartsParser::SPAtom::equals(const Atom * atom) const
	{
		#ifdef SMARTS_PARSER_DEBUG
		cerr << "bool SmartsParser::SPAtom::equals(const Atom * atom " << atom << ") const (this=" << this << ", #properties="<< countProperties() << ")" << endl;
		#endif

		bool isnot(false);
		if (hasProperty("Not"))
		{
			isnot = true;
		}

		for (Size i=0; i!=countProperties(); ++i)
		{
			#ifdef SMARTS_PARSER_DEBUG
			cerr << getNamedProperty(i).getName() << "\t";
			#endif
			String property_name = getNamedProperty(i).getName();
			NamedProperty p = getNamedProperty(i);
			if (property_name == "Symbol")
			{
				//cerr << p.getString() << endl;
				String property_value = p.getString();
				String s(property_value);
/*				if (property_value.size() > 0 && islower(property_value[i]))
				{
					addAtomProperty(NamedProperty("Aromatic", true));
					s.toUpper(0, 1);
				}*/
				if (s.size() > 0 && s != atom->getElement().getSymbol()) // and not not :)
				{
					if (!isnot)
					{
						return false;
					}
					else
					{
						continue;
					}
				}
				else
				{
					if (!isnot)
					{
						continue;
					}
					else
					{
						return false;
					}
				}
			}

			if (property_name == "Not")
			{
				continue;
			}

			// in num rings
			if (property_name == "InNumRings" || property_name == "NotInNumRings")
			{
				Size ring_count(0);
				for (vector<HashSet<const Atom*> >::const_iterator it = SmartsParser::sssr_->begin(); it != SmartsParser::sssr_->end(); ++it)
				{
					if (it->has(atom))
					{
						++ring_count;
					}
				}
				if (p.getUnsignedInt() == 999)
				{
					if (property_name == "InNumRings")
					{
						if (ring_count == 0)
						{
							return false;
						}
						else
						{
							continue;
						}
					}
					else
					{
						if (ring_count != 0)
						{
							return false;
						}
						else
						{
							continue;
						}
					}
				}
				
				if (property_name == "InNumRings")
				{
					if (ring_count != p.getUnsignedInt())
					{
						return false;
					}
					else
					{
						continue;
					}
				}
				else
				{
					if (ring_count == p.getUnsignedInt())
					{
						return false;
					}
					else
					{
						continue;
					}
				}
			}

			// isotopes
			if (property_name == "Isotope" || property_name == "NotIsotope")
			{
				cerr << "warning: isotopes are not supported for now" << endl;
			}

			// atomic weight
			if (property_name == "AtomicMass")
			{
				cerr << "warning: atomic mass property of an atom might be misinterpreted!" << endl;
				if (atom->getElement().getAtomicWeight() != p.getUnsignedInt())
				{
					return false;
				}
				else
				{
					continue;
				}
			}

			if (property_name == "NotAtomicMass")
			{
				cerr << "warning atomic mass property of an atom might be misinterpreted!" << endl;
				if (atom->getElement().getAtomicWeight() == p.getUnsignedInt())
				{
					return false;
				}
				else
				{
					continue;
				}
			}

			// valence
			if (property_name == "Valence")
			{
				if (countRealValences(atom) != p.getUnsignedInt())
				{
					return false;
				}
				else
				{
					continue;
				}
			}
			if (property_name == "NotValence")
			{
				if (countRealValences(atom) == p.getUnsignedInt())
				{
					return false;
				}
				else
				{
					continue;
				}
			}

			// total connections
			if (property_name == "Connectivity")
			{
				if (atom->countBonds() + getNumberOfImplicitHydrogens(atom) != p.getUnsignedInt())
				{
					return false;
				}
				else
				{
					continue;
				}
			}
			if (property_name == "NotConnectivity")
			{
				if (atom->countBonds() + getNumberOfImplicitHydrogens(atom) == p.getUnsignedInt())
				{
					return false;
				}
				else
				{
					continue;
				}
			}

			// explicit hydrogens
			if (property_name == "ExplicitHydrogens" || property_name == "NotExplicitHydrogens")
			{
				Size h_count(0);
				for (Atom::BondConstIterator bit = atom->beginBond(); +bit; ++bit)
				{
					if (bit->getPartner(*atom)->getElement() == PTE[Element::H])
					{
						++h_count;
					}
				}
				if (property_name == "ExplicitHydrogens")
				{
					if (h_count != p.getUnsignedInt())
					{
						return false;
					}
					else
					{
						continue;
					}
				}
				else
				{
					if (h_count == p.getUnsignedInt())
					{
						return false;
					}
					else
					{
						continue;
					}
				}
			}

			// implicit hydrogens
			if (property_name == "ImplicitHydrogens")
			{
				if (getNumberOfImplicitHydrogens(atom) != p.getUnsignedInt())
				{
					return false;
				}
				else
				{
					continue;
				}
			}
			if (property_name == "NotImplicitHydrogens")
			{
				if (getNumberOfImplicitHydrogens(atom) == p.getUnsignedInt())
				{
					return false;
				}
				else
				{
					continue;
				}
			}

			/*
			 * Aromatic, NotAromatic
			 * Aliphatic, NotAliphatic
			 */
			if (property_name == "Aromatic")
			{
				if (!atom->getProperty("IsAromatic").getBool())
				{
					return false;
				}
			}
			if (property_name == "NotAromatic")
			{
				if (atom->getProperty("IsAromatic").getBool())
				{
					return false;
				}
				continue;
			}
			if (property_name == "Aliphatic")
			{
				if (atom->getProperty("IsAromatic").getBool())
				{
					return false;
				}
				continue;
			}
			if (property_name == "NotAliphatic")
			{
				if (!atom->getProperty("IsAromatic").getBool())
				{
					return false;
				}
				continue;
			}

			// charge
			if (property_name == "Charge")
			{
				if (atom->getFormalCharge() != p.getInt())
				{
					return false;
				}
				continue;
			}
			if (property_name == "NotCharge")
			{
				if (atom->getFormalCharge() == p.getInt())
				{
					return false;
				}
				continue;
			}

			// in ring of size
			if (property_name == "InRingSize")
			{
				if (!atom->getProperty("InRing").getBool())
				{
					return false;
				}
				if (p.getUnsignedInt() != 0)
				{
					InRingPredicate in_ring(p.getUnsignedInt());
					if(!in_ring(*atom))
					{
						return false;
					}
				}
				continue;
			}
			if (property_name == "NotInRingSize")
			{
				if (atom->getProperty("InRing").getBool())
				{
					if (p.getUnsignedInt() == 0)
					{
						return false;
					}
					InRingPredicate in_ring(p.getUnsignedInt());
					if (in_ring(*atom))
					{
						return false;
					}
				}
				continue;
			}

			// degree, not degree
			// all explicit connections, no H is counted (correct? TODO)
			if (property_name == "Degree")
			{
				#ifdef SMARTS_PARSER_DEBUG
				cerr << atom->countBonds() << " " << p.getUnsignedInt() << endl;
				#endif
				Size bond_count(0);
				for (Atom::BondConstIterator bit = atom->beginBond(); +bit; ++bit)
				{
					if (bit->getPartner(*atom)->getElement() != PTE[Element::H])
					{
						++bond_count;
					}
				}
				if (/*atom->countBonds()*/bond_count != p.getUnsignedInt())
				{
					return false;
				}
				continue;
			}
			if (property_name == "NotDegree")
			{
				Size bond_count(0);
				for (Atom::BondConstIterator bit = atom->beginBond(); +bit; ++bit)
				{
					if (bit->getPartner(*atom)->getElement() != PTE[Element::H])
					{
						++bond_count;
					}
				}
				if (/*atom->countBonds()*/bond_count == p.getUnsignedInt())
				{
					return false;
				}
				continue;
			}

			// ring connectivity
			if (property_name == "RingConnected")
			{
				Size num(0);
				for (Atom::BondConstIterator it=atom->beginBond(); +it; ++it)
				{
					if (it->getProperty("InRing").getBool())
					{
						++num;
					}
				}
				if (num != p.getUnsignedInt())
				{
					return false;
				}
				continue;
			}
			if (property_name == "NotRingConnected")
			{
				Size num(0);
				for (Atom::BondConstIterator it=atom->beginBond(); +it; ++it)
				{
					if (it->getProperty("InRing").getBool())
					{
						++num;
					}
				}
				if (num == p.getUnsignedInt())
				{
					return false;
				}
				continue;
			}

			if (property_name == "Chirality" || property_name == "NotChirality")
			{
				// so far BALL has no Code to set the chirality,
				// hence all definitiones will fail here (except *_OR_UNSPECIFIED)
				SmartsParser::ChiralClass chiral_class = (SmartsParser::ChiralClass)p.getUnsignedInt();
				switch (chiral_class)
				{
					case CW_DEFAULT_OR_UNSPECIFIED:
					case CCW_DEFAULT_OR_UNSPECIFIED:
					case CW_TH_OR_UNSPECIFIED:
					case CCW_TH_OR_UNSPECIFIED:
					case CW_AL_OR_UNSPECIFIED:
					case CCW_AL_OR_UNSPECIFIED:
					case CW_SP_OR_UNSPECIFIED:
					case CCW_SP_OR_UNSPECIFIED:
					case CW_OH_OR_UNSPECIFIED:
					case CCW_OH_OR_UNSPECIFIED:
					case CW_TB_OR_UNSPECIFIED:
					case CCW_TB_OR_UNSPECIFIED:
					case CHIRAL_CLASS_UNSPECIFIED:
						continue;
						break;
					default:
						return false;
				}
			}

			cerr << "SP: unknown property: " << property_name << endl;	
		}
			
		#ifdef SMARTS_PARSER_DEBUG
		cerr << "true" << endl;
		#endif
		return true;
	}
	
	void SmartsParser::SPAtom::init_()
	{
/*		setProperty("Not", false);
		setProperty("Isotope", Size(0));
		setProperty("NotIsotope", false);
		setProperty("Charge", Index(0));
		setProperty("NotCharge", false);
		setProperty("Aromatic", false);
		setProperty("NotAromatic", false);
		setProperty("Aliphatic", false);
		setProperty("NotAliphatic", false);
		setProperty("InNumRings", Size(0));
		setProperty("NotInNumRings", false);
		setProperty("InBrackets", false);
		setProperty("InRingSize", Size(0));
		setProperty("NotInRingSize", false);
		setProperty("Connected", Size(0));
		setProperty("NotConnected", false);
		setProperty("ExplicitHydrogens", Size(0));
		setProperty("NotExplicitHydrogens", false);
		setProperty("ImplicitHydrogens", Size(0));
		setProperty("NotExplicitHydrogens", false);
		setProperty("Valence", Size(0));
		setProperty("NotValence", false);
		setProperty("Degree", Size(0));
		setProperty("NotDegree", false);
		setProperty("RingConnected", Size(0));
		setProperty("NotRingConnected", false);
		setProperty("NotChirality", false);
		setProperty("Symbol", "");
*/
		return;
	}
	
	/* 
	 * Implementation SmartsParser
	 */
	 
	SmartsParser::SmartsParser()
		:	needs_SSSR_(false),
			recursive_(false),
			component_grouping_(false),
			root_((SPNode*)0)
	{
	}

	SmartsParser::SmartsParser(const SmartsParser& parser)
		:	needs_SSSR_(parser.needs_SSSR_),
			recursive_(parser.recursive_),
			component_grouping_(parser.component_grouping_),
			root_(parser.root_)
	{
		// TODO new states!
	}

	SmartsParser::~SmartsParser()
	{
		clear();
	}

	void SmartsParser::clear()
	{
		// delete all SPNodes
		for (HashSet<SPNode*>::Iterator it=nodes_.begin(); +it; ++it)
		{
			if (!(*it)->isInternal())
			{
				SPAtom* a = (*it)->getSPAtom();
				if (a != 0)
				{
					delete a;
				}
			}
			delete *it;
		}
		nodes_.clear();

		// delete all SPEdges
		for (HashSet<SPEdge*>::Iterator it=edges_.begin(); +it; ++it)
		{
			if (!(*it)->isInternal())
			{
				SPBond* b = (*it)->getSPBond();
				if (b != 0)
				{
					delete b;
				}
			}
			delete *it;
		}
		edges_.clear();

		ring_connections_.clear();
		root_ = 0;
		if (sssr_ != 0)
		{
			sssr_->clear();
			sssr_ = 0;
		}
		needs_SSSR_ = false;
		recursive_ = false;
		component_grouping_ = false;
		component_no_ = 0;
		rec_edges_.clear();
	}

	void SmartsParser::parse(const String& s)
		throw(Exception::ParseError)
	{
		// clear out stuff from previous run
		clear();

		// make the internals of this parser available for all
		state.current_parser = this;
		state.buffer = s.c_str();
		state.char_count = 0;
		
		try
		{
			SmartsParser_initBuffer(state.buffer);
			SmartsParserparse();
			SmartsParser_delBuffer();	
		}
		catch (Exception::ParseError& e)
		{
			// Clean up the parser buffer.
			SmartsParser_delBuffer();

			// Clean up allocated memory (atoms).
			clear();

			// Propagate the parse error upwards.
			throw e;
		}

		//dumpTree();
	}
	
	void SmartsParser::addRingConnection(SPNode* spnode, Size index)
	{
		ring_connections_[index].push_back(spnode);
	}

	HashMap<Size, vector<SmartsParser::SPNode*> > SmartsParser::getRingConnections() const
	{
		return ring_connections_;
	}

	void SmartsParser::setSSSR(const vector<vector<Atom*> >& new_sssr)
	{
		static vector<HashSet<const Atom*> > sssr;
		sssr.clear();
		for (vector<vector<Atom*> >::const_iterator it1 = new_sssr.begin(); it1 != new_sssr.end(); ++it1)
		{
			HashSet<const Atom*> ring;
			for (vector<Atom*>::const_iterator it2 = it1->begin(); it2 != it1->end(); ++it2)
			{
				ring.insert(*it2);
			}
			sssr.push_back(ring);
		}
		sssr_ = &sssr;
	}


//	SmartsParser::SPBond::SPBond
//		(SmartsParser::SPAtom* /* left */, SmartsParser::SPAtom* /* right */, SPBondOrder order)
//		:	ze_type_(SmartsParser::NONE),
//			bond_order_(order)
//	{
		// TODO
		//left->createBond(*this, *right);
//	}

	SmartsParser::SPBond::SPBond(SPBondOrder order)
		:	ze_type_(SmartsParser::NONE),
			bond_order_(order),
			not_(false)
	{
		#ifdef SMARTS_PARSER_DEBUG
		cerr << "SmartsParser::SPBond::SPBond(" << order << ")" << endl;
		#endif
	}

	SmartsParser::SPBond::~SPBond()
		throw()
	{
	}

	struct SmartsParser::State SmartsParser::state;
	vector<HashSet<const Atom*> >* SmartsParser::sssr_ = 0;
	
	void SmartsParser::dumpTree()
	{
		// TODO
		cerr << "The current tree is: " << endl;
		bool consider_as_noninternal(false);
		if (root_->isInternal())
		{
			cerr << "root is internal: " << endl;
			dumpTreeRecursive_(root_->getFirstEdge(), 1);
			if (root_->getLogicalOperator() == AND_LOW) cerr << "AND_LOW" << endl;
			if (root_->getLogicalOperator() == AND) cerr << "AND" << endl;
			if (root_->getLogicalOperator() == OR) cerr << "OR" << endl;
			dumpTreeRecursive_(root_->getSecondEdge(), 1);
			if (root_->countEdges() != 0)
			{
				consider_as_noninternal = true;
			}
		}
		
		if (!root_->isInternal() || consider_as_noninternal)
		{
			if (!consider_as_noninternal)
			{
				cerr << "root (#properties=" << root_->getSPAtom()->countNamedProperties();
				if (root_->getSPAtom()->hasProperty("Symbol"))
				{
					cerr << ", Symbol=" << root_->getSPAtom()->getProperty("Symbol").getString();
				}		
				cerr << ")" << endl;
			}
			Size count(1);
			for (SPNode::EdgeIterator eit = root_->begin(); eit != root_->end(); ++eit, ++count)
			{
				cerr << count << "."<< endl;
				dumpTreeRecursive_(*eit, 1);
			}
		}
		cerr << endl;
	}

	void SmartsParser::dumpTreeRecursive_(SPNode* node, Size depth)
	{
		//cerr << String('\t', depth) << "dumpTreeRecursive_(SPNode*=" << node << ", " << depth << ")" << endl;
		bool consider_as_noninternal(false);
		//cerr << node << " " << node->countEdges() << endl;
		if (node->isInternal())
		{
			cerr << String('\t', depth) << "node (internal): " << node << "[recursive=" << node->isRecursive() << "]" << endl;
			dumpTreeRecursive_(node->getFirstEdge(), depth + 1);
			if (node->getLogicalOperator() == AND_LOW) cerr << String('\t', depth) << "AND_LOW" << endl;
			if (node->getLogicalOperator() == AND) cerr << String('\t', depth) << "AND" << endl;
			if (node->getLogicalOperator() == OR) cerr << String('\t', depth) << "OR" << endl;
			dumpTreeRecursive_(node->getSecondEdge(), depth + 1);
			if (node->countEdges() != 0)
			{
				consider_as_noninternal = true;
			}
		}
		
		if (!node->isInternal() || consider_as_noninternal)
		{
			if (!consider_as_noninternal)
			{
				cerr << String('\t', depth) << "node (#properties=" << node->getSPAtom()->countNamedProperties();
				if (node->getSPAtom()->hasProperty("Symbol"))
				{
					cerr << ", Symbol=" << node->getSPAtom()->getProperty("Symbol").getString();
				}
				cerr << ") " << node << "[recursive=" << node->isRecursive() << "]" << endl;
			}
			Size count(1);
			for (SPNode::EdgeIterator eit = node->begin(); eit != node->end(); ++eit, ++count)
			{
				cerr << String('\t', depth) << count << "." << endl;
				dumpTreeRecursive_(*eit, depth + 1);
			}
		}
	}

	void SmartsParser::dumpTreeRecursive_(SPEdge* edge, Size depth)
	{
		//cerr << String('\t', depth) << "dumpTreeRecursive_(SPEdge*=" << edge << ", " << depth << ")" << endl;
		if (edge->isInternal() && edge->getSecondSPEdge() != 0)
		{
			cerr << String('\t', depth) << "edge (internal): " << edge << endl;
			dumpTreeRecursive_(edge->getFirstSPEdge(), depth + 1);
			if (edge->getLogicalOperator() == AND_LOW) cerr << String('\t', depth) << "AND_LOW" << endl;
			if (edge->getLogicalOperator() == AND) cerr << String('\t', depth) << "AND" << endl;
			if (edge->getLogicalOperator() == OR) cerr << String('\t', depth) << "OR" << endl;
			dumpTreeRecursive_(edge->getSecondSPEdge(), depth + 1);
			if (edge->getSecondSPNode() != 0)
			{
				dumpTreeRecursive_(edge->getSecondSPNode(), depth + 1);
			}
		}
		else
		{
			cerr << String('\t', depth) << "edge ";
			if (edge->getSPBond() != 0)
			{
				cerr << "(bond order: " << edge->getSPBond()->getBondOrder() << ", not=" << edge->getSPBond()->getNot() << ") " << edge << endl;
			}
			else
			{
				cerr << endl;
			}

			if (edge->getSecondSPNode() != 0)
			{
				dumpTreeRecursive_(edge->getSecondSPNode(), depth + 1);
			}
		}
	}

	void SmartsParser::setNextComponentNumberToSubTree(SPNode* spnode)
	{
		component_no_++;
		stack<SPNode*> nodes;
		nodes.push(spnode);
		while (nodes.size() != 0)
		{
			//cerr << count++ << ". node ";
			SPNode* node = nodes.top();
			node->setComponentNumber(component_no_);
			nodes.pop();
			if (node->getFirstEdge() != 0 && node->getFirstEdge()->getSecondSPNode() != 0)
			{
				nodes.push(node->getFirstEdge()->getSecondSPNode());
			}
			if (node->getSecondEdge() != 0 && node->getSecondEdge()->getSecondSPNode() != 0)
			{
				nodes.push(node->getSecondEdge()->getSecondSPNode());
			}
			for (SPNode::EdgeIterator it = node->begin(); it != node->end(); ++it)
			{
				nodes.push((*it)->getSecondSPNode());
			}
		}
	}
	
} // namespace BALL

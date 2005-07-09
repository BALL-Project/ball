// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: smartsParser.C,v 1.2 2005/07/09 18:35:04 bertsch Exp $
//

#include <BALL/STRUCTURE/smartsParser.h>
#include <BALL/KERNEL/PTE.h>
#include <BALL/KERNEL/standardPredicates.h>

#include <algorithm>
#include <sstream>


#define SMARTS_PARSER_DEBUG
#undef  SMARTS_PARSER_DEBUG


// defined in the lexer (smartsParserLexer.l)
extern void SmartsParser_initBuffer(const char* buf);
extern void SmartsParser_delBuffer();
extern int SmartsParserparse();

namespace BALL
{
	/* 
	 * Implementation SPNode
	 */
	SmartsParser::SPNode::SPNode()
		:	internal_(false),
			is_not_(false),
			recursive_(false),
			first_edge_((SPEdge*)0),
			second_edge_((SPEdge*)0),
			sp_atom_((SPAtom*)0)
	{
		log_op_ = SmartsParser::AND;
	}

	SmartsParser::SPNode::SPNode(SPAtom* atom)
		:	internal_(false),
			is_not_(false),
			recursive_(false),
			first_edge_((SPEdge*)0),
			second_edge_((SPEdge*)0),
			sp_atom_(atom)
	{
		#ifdef SMARTS_PARSER_DEBUG
		cerr << "SmartsParser::SPNode::SPNode(SPAtom* atom)" << endl;
		#endif
	}

	SmartsParser::SPNode::SPNode(SPNode* first, LogicalOperator log_op, SPNode* second)
		:	first_edge_(new SmartsParser::SPEdge()),
			second_edge_(new SmartsParser::SPEdge())
	{
		first_edge_->setFirstSPNode(this);
		first_edge_->setSecondSPNode(first);
		second_edge_->setFirstSPNode(this);
		second_edge_->setSecondSPNode(second);
		log_op_ = log_op;
	}

	SmartsParser::SPNode::~SPNode()
	{
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
			log_op_(SmartsParser::AND)
	{
		//log_op = SmartsParser::AND;
		#ifdef SMARTS_PARSER_DEBUG
		cerr << "SmartsParser::SPEdge::SPEdge()" << endl;
		#endif
	}

	SmartsParser::SPEdge::~SPEdge()
	{
	}

	SmartsParser::SPBond::SPBond()
	{
		#ifdef SMARTS_PARSER_DEBUG
		cerr << "SmartsPArser::SPBond::SPBond()" << endl;
		#endif
	}

	bool SmartsParser::SPBond::equals(const Bond* bond) const
	{
		// TODO - Z/E isomer types
		// 			- additional type, inRing, not nec. con. ...
		
		bool matches(false);
		switch (bond_order_)
		{
			case SINGLE:	
				if (bond->getOrder() == Bond::ORDER__SINGLE)
				{
					matches = true;
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
			default:
				// TODO error message to log stream
				cerr << "unknown or not implemented bond order: " << bond_order_ << endl;
		}
		
		return not_ ? !matches : matches;
	}

	/*	
	 * Implementation of SPAtom
	 */
	SmartsParser::SPAtom::SPAtom()
	{
	}

	SmartsParser::SPAtom::SPAtom(const String& symbol)
	{
		#ifdef SMARTS_PARSER_DEBUG
		cerr << "SmartsParser::SPAtom::SPAtom(" << symbol << ")" << endl;
		#endif
		if (symbol != "*" && symbol != "")
		{
			String s(symbol);
			if (symbol.isDigit())
			{
				s = PTE[symbol.toUnsignedInt()-1].getSymbol();
			}
			setProperty("Symbol", s);
#ifdef SMARTS_PARSER_DEBUG
			cerr << "setting property symbol=" << s << endl;
#endif
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

	Size SmartsParser::SPAtom::getDefaultValence() const
	{
		Size an(0);
		if (getProperty("Symbol").getString() != "")
		{
			an = PTE.getElement(getProperty("Symbol").getString()).getAtomicNumber();
		}
			
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

	bool SmartsParser::SPAtom::equals(const Atom * atom) const
	{
		#ifdef SMARTS_PARSER_DEBUG
		//cerr << "bool SmartsParser::SPAtom::equals(const Atom * atom) const" << endl;
		#endif
		// TODO 
		// isotopes ???
		// explicit hydrogen, attached hydrogens ???
		// implicit hydrogen
		// connectivity
		// InNumRings
		// valence
		// atomic mass ???
		// all chirality options!

		for (Size i=0; i!=countProperties(); ++i)
		{
			#ifdef SMARTS_PARSER_DEBUG
			cerr << getNamedProperty(i).getName() << "\t";
			#endif
			String property_name = getNamedProperty(i).getName();
			NamedProperty p = getNamedProperty(i);
			if (property_name == "Symbol")
			{
				String property_value = p.getString();
				String s(property_value);
				if (property_value.size() > 0 && islower(property_value[i]))
				{
					s.toUpper(0, 1);
				}
				if (s.size() > 0 && s != atom->getElement().getSymbol())
				{
					#ifdef SMARTS_PARSER_DEBUG
					//cerr << "false" << endl;
					#endif
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
			if (property_name == "Degree")
			{
				#ifdef SMARTS_PARSER_DEBUG
				cerr << atom->countBonds() << " " << p.getUnsignedInt() << endl;
				#endif
				if (atom->countBonds() != p.getUnsignedInt())
				{
					return false;
				}
				continue;
			}
			if (property_name == "NotDegree")
			{
				if (atom->countBonds() == p.getUnsignedInt())
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
	{
	}

	SmartsParser::SmartsParser(const SmartsParser& /* parser */)
	{
		// TODO
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
	}
	
	SmartsParser::SPBond::SPBond
		(SmartsParser::SPAtom* /* left */, SmartsParser::SPAtom* /* right */, SPBondOrder order)
		:	ze_type_(SmartsParser::NONE),
			bond_order_(order)
	{
		// TODO
		//left->createBond(*this, *right);
	}

	SmartsParser::SPBond::SPBond(SPBondOrder order)
		:	ze_type_(SmartsParser::NONE),
			bond_order_(order)			
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
	
	void SmartsParser::createBonds
		(SmartsParser::SPAtom* /* atom */, const SmartsParser::ConnectionList* /* conns */)
	{
	}

	void SmartsParser::dumpTree()
	{
		// TODO
		cerr << " The current tree is: " << endl;
		
		if (root_->isInternal())
		{
			cerr << " root (internal) " << root_->getLogicalOperator() << endl;
			cerr << " \t first: " << 
				root_->getFirstEdge()->getSecondSPNode()->getSPAtom()->getProperty("Symbol").getString() << endl;
			cerr << " \t second: " << 
				root_->getSecondEdge()->getSecondSPNode()->getSPAtom()->getProperty("Symbol").getString() << endl;
			
		}
		else
		{
			cerr << " root -> ";
			SPAtom* sp_atom = root_->getSPAtom();
			if (sp_atom != 0)
			{
				cerr << sp_atom->getProperty("Symbol").getString() << endl;
			}
		}
	}

	void SmartsParser::dumpTreeRecursive_(SPNode* /* node */)
	{
		// TODO
	}
} // namespace BALL

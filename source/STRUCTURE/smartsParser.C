// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: smartsParser.C,v 1.12 2006/06/08 21:36:36 bertsch Exp $
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
#ifdef SMARTS_PARSER_DEBUG
		cerr << "bool SmartsParser::SPBond::equals(const Bond* " << bond << ") const" <<endl;
#endif
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
			case SINGLE_OR_AROMATIC:
				if (bond->getOrder() == Bond::ORDER__SINGLE || bond->getOrder() == Bond::ORDER__AROMATIC)
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
		//PropertyValue val;
		#ifdef SMARTS_PARSER_DEBUG
		cerr << "SmartsParser::SPAtom::SPAtom(" << symbol << "), this=" << this << endl;
		#endif
		//if (symbol != "")
		//{
			String s(symbol);
			//cerr << "'" << s << "'" << endl;
			if (symbol.isDigit())
			{
				s = PTE[symbol.toUnsignedInt()].getSymbol();
			}
			else
			{
				if (islower(symbol[0]))
				{
					s.toUpper(0, 1);
					setProperty(AROMATIC, true);
				}
				else
				{
					setProperty(ALIPHATIC, true);
				}
			}
			setProperty(SYMBOL, &PTE[s]);
#ifdef SMARTS_PARSER_DEBUG
			cerr << "setting property symbol=" << s << endl;
#endif
		//}
		//else
		//{
			//val.str_value = "";
			//setProperty(SYMBOL, val);
		//}
	}
	
	SmartsParser::SPAtom::~SPAtom()
		throw()
	{
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
		cerr << "bool SmartsParser::SPAtom::equals(const Atom * atom " << atom->getName() << ") const (this=" << this << ", #properties="<< countProperties() << ")" << endl;
		#endif

		/*
		bool isnot(false);
		if (hasProperty(IS_NOT))
		{
			isnot = true;
		}*/
	
		int tmp(0);
		vector<int> ring_sizes;

		for (map<PropertyType, PropertyValue>::const_iterator it = properties_.begin(); it != properties_.end(); ++it)
		{
			#ifdef SMARTS_PARSER_DEBUG
			cerr << it->first << "\t" << it->second << endl;
			#endif

			switch (it->first)
			{
				case SYMBOL:
					if (atom->getElement() != *it->second.element_value)
					{
						if (not_properties_.find(SYMBOL) == not_properties_.end())
						{
							return false;
						}
					}
					else
					{
						if (not_properties_.find(SYMBOL) != not_properties_.end())
						{
							return false;
						}						
					}
					break;

				case ISOTOPE:
					cerr << "SP: isotope not implemented, ignoring" << endl;
					break;
					
				case CHARGE:
					if (atom->getFormalCharge() != it->second.int_value)
					{
						if (not_properties_.find(CHARGE) == not_properties_.end())
						{
							return false;
						}
					}
					else
					{
						if (not_properties_.find(CHARGE) != not_properties_.end())
						{
							return false;
						}
					}
					break;
					
				case AROMATIC:
					if (atom->getProperty("IsAtomatic").getBool())
					{
						if (not_properties_.find(AROMATIC) != not_properties_.end())
						{
							return false;
						}
					}
					else
					{
						if (not_properties_.find(AROMATIC) == not_properties_.end())
						{
							return false;
						}
					}
					break;
					
				case ALIPHATIC:
					if (atom->getProperty("IsAtomatic").getBool())
					{
						if (not_properties_.find(ALIPHATIC) == not_properties_.end())
						{
							return false;
						}
					}
					else
					{
						if (not_properties_.find(ALIPHATIC) != not_properties_.end())
						{
							return false;
						}
					}
					break;
					
				case IN_NUM_RINGS:
					tmp = 0;
					for (vector<std::set<const Atom*> >::const_iterator it1 = SmartsParser::sssr_->begin(); it1 != SmartsParser::sssr_->end(); ++it1)
					{
						if (it1->find(atom) != it1->end())
						{
							++tmp;
						}
					}
					bool not_properties_has(not_properties_.find(IN_NUM_RINGS) != not_properties_.end());
					if (it->second.int_value == 999)
					{
						if (tmp == 0 && !not_properties_has ||
								tmp != 0 && not_properties_has)
						{
							return false;							
						}
					}
					else
					{
						if (tmp == it->second.int_value && not_properties_has ||
								tmp != it->second.int_value && !not_properties_has)
						{
							return false;
						}
					}
					break;
					
				case IN_RING_SIZE:
					if (!atom->getProperty("InRing").getBool())
					{
						return false;
					}
					ring_sizes.clear();
					for (vector<std::set<const Atom*> >::const_iterator it1 = SmartsParser::sssr_->begin(); it1 != SmartsParser::sssr_->end(); ++it1)
					{
						if (it1->find(atom) != it1->end())
						{
							ring_sizes.push_back(it1->size());
						}
					}
					if (find(ring_sizes.begin(), ring_sizes.end(), it->second.int_value) != ring_sizes.end())
					{
						if (not_properties_.find(IN_RING_SIZE) == not_properties_.end())
						{
							return false;
						}
					}
					else
					{
						if (not_properties_.find(IN_RING_SIZE) != not_properties_.end())
						{
							return false;
						}
					}
					
					break;
					
				case IN_BRACKETS:
					// TODO what to implement?
					break;
					
				case CONNECTED:
					if ((int)(atom->countBonds() + getNumberOfImplicitHydrogens(atom)) != it->second.int_value)
					{
						if (not_properties_.find(CONNECTED) == not_properties_.end())
						{
							return false;
						}
					}
					else
					{
						if (not_properties_.find(CONNECTED) != not_properties_.end())
						{
							return false;
						}
					}
					break;
					
				case EXPLICIT_HYDROGENS:
					tmp = 0;
					for (Atom::BondConstIterator bit = atom->beginBond(); +bit; ++bit)
					{
						if (bit->getPartner(*atom)->getElement() == PTE[Element::H])
						{
							++tmp;
						}
					}
					
					if (tmp == it->second.int_value)
					{
						if (not_properties_.find(EXPLICIT_HYDROGENS) != not_properties_.end())
						{
							return false;
						}							
					}
					else
					{
						if (not_properties_.find(EXPLICIT_HYDROGENS) == not_properties_.end())
						{
							return false;
						}
					}
					break;
					
				case VALENCE:
					if ((int)countRealValences(atom) != it->second.int_value)
					{
						if (not_properties_.find(VALENCE) == not_properties_.end())
						{
							return false;
						}
					}
					else
					{
						if (not_properties_.find(VALENCE) != not_properties_.end())
						{
							return false;
						}
					}
					break;
					
				case IMPLICIT_HYDROGENS:
					if ((int)getNumberOfImplicitHydrogens(atom) == it->second.int_value)
					{
						if (not_properties_.find(IMPLICIT_HYDROGENS) != not_properties_.end())
						{
							return false;
						}
					}
					else
					{
						if (not_properties_.find(IMPLICIT_HYDROGENS) == not_properties_.end())
						{
							return false;
						}
					}
					break;
					
				case DEGREE:
					tmp = 0;
					for (Atom::BondConstIterator bit = atom->beginBond(); +bit; ++bit)
					{
						if (bit->getPartner(*atom)->getElement() != PTE[Element::H])
						{
							++tmp;
						}
					}
					if (tmp == it->second.int_value)
					{
						if (not_properties_.find(DEGREE) != not_properties_.end())
						{
							return false;
						}
					}
					else
					{
						if (not_properties_.find(DEGREE) == not_properties_.end())
						{
							return false;
						}
					}
					break;
					
				case RING_CONNECTED:
					tmp = 0;
					for (Atom::BondConstIterator it1 = atom->beginBond(); +it1; ++it1)
					{
						if (it1->getProperty("InRing").getBool())
						{
							++tmp;
						}
					}
					if (tmp == it->second.int_value)
					{
						if (not_properties_.find(RING_CONNECTED) != not_properties_.end())
						{
							return false;
						}
					}
					else
					{
						if (not_properties_.find(RING_CONNECTED) == not_properties_.end())
						{
							return false;
						}
					}
					break;
					
				case CHIRALITY:
					//cerr << "SP: chirality option not implemented yet" << endl;
					return false;
					break;
					
				default:
					cerr << "not nothing about '" << it->first << "'" << endl;
					break;
			}

		/*	
			String property_name = getNamedProperty(i).getName();
			NamedProperty p = getNamedProperty(i);
			if (property_name == "Symbol")
			{
				//cerr << p.getString() << endl;
				String property_value = p.getString();
				String s(property_value);
				if (property_value.size() > 0 && islower(property_value[i]))
				{
					addAtomProperty(NamedProperty("Aromatic", true));
					s.toUpper(0, 1);
				}
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
				for (vector<std::set<const Atom*> >::const_iterator it = SmartsParser::sssr_->begin(); it != SmartsParser::sssr_->end(); ++it)
				{
					if (it->find(atom) != it->end())
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
			}*/

			/*
			 * Aromatic, NotAromatic
			 * Aliphatic, NotAliphatic
			 */
			 /*
			if (property_name == "Aromatic")
			{
				if (!atom->getProperty("IsAromatic").getBool())
				{
					return false;
				}
				continue;
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
*/
			// degree, not degree
			// all explicit connections, no H is counted (correct? TODO)
			/*
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
				if (//atom->countBonds()bond_count != p.getUnsignedInt())
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
				if (//atom->countBonds()bond_count == p.getUnsignedInt())
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
			*/
		}
			
		#ifdef SMARTS_PARSER_DEBUG
		cerr << "true" << endl;
		#endif
		return true;
	}

	void SmartsParser::SPAtom::setProperty(PropertyType type, int value)
	{
		PropertyValue p;
		p.int_value = value;
		properties_[type] = p;
	}

	void SmartsParser::SPAtom::setProperty(PropertyType type, bool value)
	{
		PropertyValue p;
		p.bool_value = value;
		properties_[type] = p;
	}

	void SmartsParser::SPAtom::setProperty(PropertyType type, const Element* value)
	{
		PropertyValue p;
		p.element_value = value;
		properties_[type] = p;
	}

	void SmartsParser::SPAtom::setProperty(PropertyType type, ChiralClass value)
	{
		PropertyValue p;
		p.chiral_class_value = value;
		properties_[type] = p;
	}

	void SmartsParser::SPAtom::setProperty(Property p)
	{
		properties_[p.getType()] = p.getValue();
	}

	void SmartsParser::SPAtom::addPropertiesFromSPAtom(SPAtom* atom)
	{
		for (map<PropertyType, PropertyValue>::const_iterator it = atom->properties_.begin(); it != atom->properties_.end(); ++it)
		{
			properties_[it->first] = it->second;
		}
	}

	void SmartsParser::SPAtom::setNotProperty(PropertyType type)
	{
		not_properties_.insert(type);
	}

	bool SmartsParser::SPAtom::hasProperty(PropertyType type) const
	{
		return properties_.find(type) != properties_.end();
	}

	SmartsParser::SPAtom::PropertyValue SmartsParser::SPAtom::getProperty(PropertyType type)
	{
		return properties_[type];
	}

	Size SmartsParser::SPAtom::countProperties() const
	{
		return properties_.size();
	}

	SmartsParser::SPAtom::Property::Property(PropertyType type, int value)
	{
		type_ = type;
		value_.int_value = value;
	}

	SmartsParser::SPAtom::Property::Property(PropertyType type, bool value)
	{
		type_ = type;
		value_.bool_value = value;
	}

	SmartsParser::SPAtom::Property::Property(PropertyType type, const Element* value)
	{
		type_ = type;
		value_.element_value = value;
	}

	SmartsParser::SPAtom::Property::Property(PropertyType type, ChiralClass value)
	{
		type_ = type;
		value_.chiral_class_value = value;
	}

	SmartsParser::SPAtom::Property::~Property()
	{
	}

	void SmartsParser::SPAtom::Property::operator = (const Property& property)
	{
		type_ = property.type_;
		value_ = property.value_;
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
		for (set<SPNode*>::iterator it = nodes_.begin(); it != nodes_.end(); ++it)
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
		for (set<SPEdge*>::iterator it = edges_.begin(); it != edges_.end(); ++it)
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

	map<Size, vector<SmartsParser::SPNode*> > SmartsParser::getRingConnections() const
	{
		return ring_connections_;
	}

	void SmartsParser::setSSSR(const vector<vector<Atom*> >& new_sssr)
	{
		static vector<set<const Atom*> > sssr;
		sssr.clear();
		for (vector<vector<Atom*> >::const_iterator it1 = new_sssr.begin(); it1 != new_sssr.end(); ++it1)
		{
			set<const Atom*> ring;
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
	vector<set<const Atom*> >* SmartsParser::sssr_ = 0;
	
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
				cerr << "root (#properties=" << root_->getSPAtom()->countProperties();
				if (root_->getSPAtom()->hasProperty(SPAtom::SYMBOL))
				{
					cerr << ", Symbol=" << root_->getSPAtom()->getProperty(SPAtom::SYMBOL).element_value->getSymbol();
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
				cerr << String('\t', depth) << "node (#properties=" << node->getSPAtom()->countProperties();
				if (node->getSPAtom()->hasProperty(SPAtom::SYMBOL))
				{
					cerr << ", Symbol=" << node->getSPAtom()->getProperty(SPAtom::SYMBOL).element_value->getSymbol();
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

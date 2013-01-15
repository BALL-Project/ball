#include <BALL/MOLMEC/AMBER/GAFFCESParser.h>

#ifndef BALL_KERNEL_ATOM_H
#include <BALL/KERNEL/atom.h>
#endif

#ifndef BALL_KERNEL_BOND_H
#include <BALL/KERNEL/bond.h>
#endif

#ifndef BALL_KERNEL_PTE_H
#include <BALL/KERNEL/PTE.h>
#endif

#include <BALL/MOLMEC/AMBER/GAFFTypeProcessor.h>

//defined in the lexer (GAFFCESParserLexer.l)
extern void GAFFCESParser_destroy();
extern int  GAFFCESParserparse();

//#define DEBUG
#undef DEBUG

namespace BALL
{		

	GAFFCESParser::APSMatcher::APSMatcher()
		: aps_terms(1)
	{
	}

	GAFFCESParser::APSMatcher::~APSMatcher()
	{
	}	

	//checks if the current atom is in a ring with size n
	int GAFFCESParser::APSMatcher::isNRingAtom(Size size, Atom& atom)
	{
		String property = "NoSuchPropertyXXX";
		switch (size)
		{
			case 3: property = "In3Ring";
							break;
			case 4: property = "In4Ring";
							break;
			case 5: property = "In5Ring";
							break;
			case 6: property = "In6Ring";
							break;
			case 7: property = "In7Ring";
							break;
			case 8: property = "In8Ring";
							break;
			case 9: property = "In9Ring";
							break;
		}
		return atom.getProperty(property).getInt();
	}

	//checks if current atom is in a ring
	bool GAFFCESParser::APSMatcher::isRingAtom(Atom& atom)
	{
		// yes, this can probably be done in a better way :-)
		// we don't merely check for "InRing" because rings need to be smaller than 10 for GAFF
		for (Size i=3; i<10; i++)
		{
			if (isNRingAtom(i, atom) != 0)
				return true;
		}

		return false;
	}

	//check if the current atom is not a ringatom or
	//in a ten-membered or larger ring
	bool GAFFCESParser::APSMatcher::isNonRingAtom(Atom& atom)
	{
		return (!isRingAtom(atom));
	}	

	// check if atom forms a bond of type bond_type with partner.
	// if partner == NULL, check whether there is a bond of this kind at all
	bool GAFFCESParser::APSMatcher::hasBond_(Atom* atom, Atom* partner, int bond_type, int feature_number)
	{
		// count the occurence of bond_type for an atom 
		int occurence = 0;
		bool result = false;
		Atom::BondConstIterator bond_it;
		for (bond_it = atom->beginBond(); +bond_it; ++bond_it)
		{
			if (partner == NULL || bond_it->getBoundAtom(*atom) == partner)
			{
				if (bond_it->getProperty("GAFFBondType").getInt() == bond_type)
				{
						result = true;
						occurence++;
				}
			}
		}
//		atom->setProperty(bond_type, (int) occurence);

		// do we need to check for a certain number of bonds?
		if (result && (feature_number > 0))
			result &= (feature_number == occurence);
		else if (feature_number == 0)
			result = !result;

		return result;
	}

	//check if required APSTypes are fullfild
	bool GAFFCESParser::APSMatcher::checkGAFFProperties(Atom& atom, Atom& predecessor, APSTerm aps)	
	{
		bool result = false;

		switch(aps.type)
		{
			case IS_RING_ATOM:
				result = isRingAtom(atom);
				break;
			case IS_NON_RING_ATOM:
				result = isNonRingAtom(atom);
				break;
			case IS_PLANAR:
				result = atom.getProperty("IsPlanarRingAtom").getBool();
				break;
			case IS_PLANAR_WITH_DB_TO_NR:
				result = atom.getProperty("IsPlanarWithDBtoNR").getBool();
				break;
			case IS_PURELY_AROMATIC:
				result = atom.getProperty("IsPureAromatic").getBool();
				break;
			case IS_PURELY_ALIPHATIC:
				result = atom.getProperty("IsPureAliphatic").getBool();
				break;
			case IS_OTHER_RING:
				if(isRingAtom(atom))
				{
					if( 	 (!atom.getProperty("IsPlanarRingAtom").getBool())
							&& (!atom.getProperty("IsPlanarWithDBtoNR").getBool())
							&& (!atom.getProperty("IsPureAromatic").getBool())
							&& (!atom.getProperty("IsPureAliphatic").getBool()))
					{
						atom.setProperty("isOther", true);
						result = true;
					}
				}
				break;
			case IS_3_RING_ATOM:
				if(atom.getProperty("In3Ring").getBool())
				{
						if(  (aps.feature_number == atom.getProperty("NumberOf3Rings").getInt())
							 ||	(aps.feature_number < 0))
							{
									result = true;
							}
				}
				break;
			case IS_4_RING_ATOM:
				if(atom.getProperty("In4Ring").getBool())
				{
						if(  (aps.feature_number == atom.getProperty("NumberOf4Rings").getInt())
							 ||	(aps.feature_number < 0))
							{
									result = true;
							}
				}
				break;
			case IS_5_RING_ATOM:
				if(atom.getProperty("In5Ring").getBool())
				{
						if(  (aps.feature_number == atom.getProperty("NumberOf5Rings").getInt())
							 ||	(aps.feature_number < 0))
							{
									result = true;
							}
				}
				break;
			case IS_6_RING_ATOM:
				if(atom.getProperty("In6Ring").getBool())
				{
						if(  (aps.feature_number == atom.getProperty("NumberOf6Rings").getInt())
							 ||	(aps.feature_number < 0))
							{
									result = true;
							}
				}
				break;
			case IS_7_RING_ATOM:
				if(atom.getProperty("In7Ring").getBool())
				{
						if(  (aps.feature_number == atom.getProperty("NumberOf7Rings").getInt())
							 ||	(aps.feature_number < 0))
							{
									result = true;
							}
				}
				break;
			case IS_8_RING_ATOM:
			if(atom.getProperty("In8Ring").getBool())
				{
						if(  (aps.feature_number == atom.getProperty("NumberOf8Rings").getInt())
							 ||	(aps.feature_number < 0))
							{
									result = true;
							}
				}
				break;
			case IS_9_RING_ATOM:
				if(atom.getProperty("In9Ring").getBool())
				{
						if(  (aps.feature_number == atom.getProperty("NumberOf9Rings").getInt())
							 ||	(aps.feature_number < 0))
							{
									result = true;
							}
				}
				break;
			case PURE_SINGLE_BOND:
				return hasBond_(&atom, NULL, GAFFTypeProcessor::SB, aps.feature_number);
			case PURE_SINGLE_BOND_TO_PARENT:
				return hasBond_(&atom, &predecessor, GAFFTypeProcessor::SB, aps.feature_number);
			case NO_PURE_SINGLE_BOND_TO_PARENT:
				return !hasBond_(&atom, &predecessor, GAFFTypeProcessor::SB, aps.feature_number);
			case SINGLE_BOND:
				return ( (hasBond_(&atom, NULL, GAFFTypeProcessor::sb, aps.feature_number))
								|| (hasBond_(&atom, NULL, GAFFTypeProcessor::SB, aps.feature_number)));
			case SINGLE_BOND_TO_PARENT:
				return ((hasBond_(&atom, &predecessor, GAFFTypeProcessor::sb, aps.feature_number))
								||(hasBond_(&atom, &predecessor, GAFFTypeProcessor::SB, aps.feature_number)));
			case NO_SINGLE_BOND_TO_PARENT:
				return ((!hasBond_(&atom, &predecessor, GAFFTypeProcessor::sb, aps.feature_number))
								&& (!hasBond_(&atom, &predecessor, GAFFTypeProcessor::SB, aps.feature_number)));
			case PURE_DOUBLE_BOND:
				return hasBond_(&atom, NULL, GAFFTypeProcessor::DB, aps.feature_number);
			case PURE_DOUBLE_BOND_TO_PARENT:
				return hasBond_(&atom, &predecessor, GAFFTypeProcessor::DB, aps.feature_number);
			case NO_PURE_DOUBLE_BOND_TO_PARENT:
				return !hasBond_(&atom, &predecessor, GAFFTypeProcessor::DB, aps.feature_number);
			case DOUBLE_BOND:
				return ( (hasBond_(&atom, NULL, GAFFTypeProcessor::db, aps.feature_number))
								|| (hasBond_(&atom, NULL, GAFFTypeProcessor::DB, aps.feature_number)));
			case DOUBLE_BOND_TO_PARENT:
				return ((hasBond_(&atom, &predecessor, GAFFTypeProcessor::db, aps.feature_number))
								||(hasBond_(&atom, &predecessor, GAFFTypeProcessor::DB, aps.feature_number)));
				break;
			case NO_DOUBLE_BOND_TO_PARENT:
				return ((!hasBond_(&atom, &predecessor, GAFFTypeProcessor::db, aps.feature_number))
								&& (!hasBond_(&atom, &predecessor, GAFFTypeProcessor::DB, aps.feature_number)));
			case TRIPLE_BOND:
				return hasBond_(&atom, NULL, GAFFTypeProcessor::TB, aps.feature_number);
			case TRIPLE_BOND_TO_PARENT:
				return hasBond_(&atom, &predecessor, GAFFTypeProcessor::TB, aps.feature_number);
			case NO_TRIPLE_BOND_TO_PARENT:
				return !hasBond_(&atom, &predecessor, GAFFTypeProcessor::TB, aps.feature_number);
			case DELOCALIZED_BOND:
				return hasBond_(&atom, NULL, GAFFTypeProcessor::DL, aps.feature_number);
			case DELOCALIZED_BOND_TO_PARENT:
				return hasBond_(&atom, &predecessor, GAFFTypeProcessor::DL, aps.feature_number);
			case NO_DELOCALIZED_BOND_TO_PARENT:
				return !hasBond_(&atom, &predecessor, GAFFTypeProcessor::DL, aps.feature_number);
			case AROMATIC_BOND:
				return hasBond_(&atom, NULL, GAFFTypeProcessor::AB, aps.feature_number);
			case AROMATIC_BOND_TO_PARENT:
				return hasBond_(&atom, &predecessor, GAFFTypeProcessor::AB, aps.feature_number);
			case NO_AROMATIC_BOND_TO_PARENT:
				return !hasBond_(&atom, &predecessor, GAFFTypeProcessor::AB, aps.feature_number);
			case APS_TRUE:
				return true;
		}

		return result;
	}

	String GAFFCESParser::APSMatcher::printAPS()
	{
		String result = "";

		if ((aps_terms.size() == 1) && (aps_terms[0].size() == 0))
			result = "[*]";
		else
		{
			result = "[";
			// this will create a string with a trailing & which we will later replace
			for (Position i=0; i<aps_terms.size(); i++)
			{
				std::vector<GAFFCESParser::APSMatcher::APSTerm >& or_terms = aps_terms[i];
				if (aps_terms[i].size() == 0 || ((aps_terms[i].size() == 1) && (or_terms[0].type == APS_TRUE)))
					result += "*";
				else
				{
					for (Position j=0; j<or_terms.size(); j++)
					{
						result += String(or_terms[j].feature_number)+"x"+String(or_terms[j].type)+"|";
					}
					result.truncate(result.size()-1);
				}
				result += "&";
			}
			result.truncate(result.size()-1);
			result += "]";
		}
		return result;
	}

	// check if atom matches atomic property string
	bool GAFFCESParser::APSMatcher::operator() (Atom& atom, Atom& predecessor)
	{
#ifdef DEBUG
		Log.info() << "GAFFCESParser::APSMatcher: trying to match atom " << atom.getFullName() << " against APS: " << printAPS() << " ";
#endif
	// all and-terms must be true
		bool and_result = true;

		// iterate over all the and-terms in aps_terms
		for (Position i=0; i<aps_terms.size(); i++)
		{
			// one of the or-terms must be true...
			bool or_result = false;

			// ...but if there is none, this is an "always match" case
			if (aps_terms[i].size() == 0)
			{
				or_result = true;
			}
			else
			{
				std::vector<GAFFCESParser::APSMatcher::APSTerm >& or_terms = aps_terms[i];
				// iterate over all the or-terms
				for (Position j=0; j<or_terms.size(); j++)
				{
					if (checkGAFFProperties(atom, predecessor, (or_terms[j])))
					{
						or_result = true;
						break;
					}
				}
			}
			and_result &= or_result;
		}
#ifdef DEBUG
		if (and_result)
			Log.info() << "found a match!" << std::endl;
		else
			Log.info() << "no match!" << std::endl;
#endif

		return and_result;
	}

	GAFFCESParser::CESPredicate::~CESPredicate()
	{
		for (Size i=0; i<children.size(); i++)
		{
			delete children[i];
		}
	}
		

	//initialize stringToWildcard map
	void GAFFCESParser::CESPredicate::initStringToWildcard()
	{
		stringToWildcard_["XA"] = XA;
		stringToWildcard_["XB"] = XB;
		stringToWildcard_["XD"] = XD;
		stringToWildcard_["XC"] = XC;
		stringToWildcard_["XX"] = XX;
	}



	std::map<String, GAFFCESParser::CESPredicate::CESwildcards> GAFFCESParser::CESPredicate::getStringToWildcard()
	{
		initStringToWildcard();
		return stringToWildcard_;
	}

	//to expand aps_term in aps_matcher object
	void GAFFCESParser::CESPredicate::addNewAND()
	{
		aps_matcher.aps_terms.resize(aps_matcher.aps_terms.size()+1);	
	}

	//to expand aps_term in aps_matcher object
	void GAFFCESParser::CESPredicate::addNewOR(GAFFCESParser::APSMatcher::APSType aps, int feature_number)
	{
		aps_matcher.aps_terms[aps_matcher.aps_terms.size()-1].push_back(GAFFCESParser::APSMatcher::APSTerm(aps, feature_number));	
	}


	//add an CESelementConnectionPredicate to "predicate tree"
	void GAFFCESParser::CESPredicate::addCESelementConnectionPredicate(Size numberOfPartners, String element_name)
	{
		CESelementConnectionPredicate* elementConnectionPredicate = new CESelementConnectionPredicate(parser_);
		elementConnectionPredicate->parent = parser_->current_root_predicate;
		elementConnectionPredicate->setNumberOfPartners(numberOfPartners);
		elementConnectionPredicate->setElementName(element_name);
		parser_->current_predicate = elementConnectionPredicate;
		parser_->current_root_predicate->children.push_back(elementConnectionPredicate);
	}

	//add an CESelementPredicate to "predicate tree"
	void  GAFFCESParser::CESPredicate::addCESelementPredicate(String element_name)
	{
		CESelementPredicate* elementPredicate = new CESelementPredicate(parser_);
		elementPredicate->parent = parser_->current_root_predicate;
		elementPredicate->setElementName(element_name);
		parser_->current_predicate = elementPredicate;
		parser_->current_root_predicate->children.push_back(elementPredicate);
	}

	//add a CESwilddcardsAtomicPropertyPredicate to "predicate tree"
	void GAFFCESParser::CESPredicate::addCESwildcardsPredicate(String wildcard)
	{
		CESwildcardsPredicate* wildcardsPredicate = new CESwildcardsPredicate(parser_);
		wildcardsPredicate->parent = parser_->current_root_predicate;
		wildcardsPredicate->setWildcards(wildcard);
		parser_->current_predicate = wildcardsPredicate;
		parser_->current_root_predicate->children.push_back(wildcardsPredicate);	
	}

	//add a CESwildcardsConnectionPredicate to "predicate tree"
	void GAFFCESParser::CESPredicate::addCESwildcardsConnectionPredicate(String wildcard, Size numberOfPartners)
	{
		CESwildcardsConnectionPredicate* wildcardsConnectionPredicate = new CESwildcardsConnectionPredicate(parser_);
		wildcardsConnectionPredicate->parent = parser_->current_root_predicate;
		wildcardsConnectionPredicate->setNumberOfPartners(numberOfPartners);
		wildcardsConnectionPredicate->setWildcards(wildcard);
		parser_->current_predicate = wildcardsConnectionPredicate;
		parser_->current_root_predicate->children.push_back(wildcardsConnectionPredicate);	
	}

	// add a TruePredicate to "predicate tree"; since this is equivalent to not testing
	// a predicate at all, we just do nothing
	void GAFFCESParser::CESPredicate::addTruePredicate() const
	{
	}

	//check if atom matches "predicates in predicate-tree"
	bool GAFFCESParser::match(Atom& atom) const
	{
		return (*root_predicate)(atom);
	}

	
	bool GAFFCESParser::CESPredicate::operator()(Atom& atom)	
	{
		atom_to_test = &atom;

		// first, we check for the APS of this atom
		Atom* second_atom;
		if (parser_->root_predicate == this)
			second_atom = atom_to_test;
		else
			second_atom = parent->atom_to_test;

		if (!aps_matcher.aps_terms.empty())
		{
			if (!aps_matcher(*atom_to_test, *second_atom))
				return false;
		}

		bool result = false;
		//present predicate has child-predicates
		// match the current predicate before testing its children
		if(match(atom))
		{
			if (children.empty())
				return true;

			//stores if a children-predicate is already matched or not
			std::vector<bool>match_events((children.size()),false);

			Atom::BondIterator bond_it = atom.beginBond();
			for(;+bond_it;++bond_it)
			{
				Atom& partnerAtom = *(bond_it->getPartner(atom));
				if (parent->alreadySeenThisAtom(&partnerAtom))
					continue;

				for (Size i=0; (i<children.size()); i++)
				{
					//if predicate wasn't matched before
					if(!(match_events[i]))
					{
						//if one match of atom and predicate is found
						if((*children[i])(partnerAtom))
						{
							//mark predicate as matched and start with next partnerAtom
							match_events[i] = true;
							break;
						}
					}
				}
			}
			//number of partnerAtoms and number of 
			//corresponding predicate-children
			//can differ, but all occurring predicate-children 
			//have to be matched by a partnerAtom
			for(std::vector<bool>::iterator match_it = match_events.begin(); 
																			match_it != match_events.end();
																			++match_it)
			{
				//if any match_event is false, no complete match could be found
				if(!(*(match_it)))
					return false;	
			}			
			result = true;
		}

		return result;
	}
		

	GAFFCESParser::GAFFCESParser() 
		: root(this)
	{
		root_predicate = &root;
		current_root_predicate = root_predicate;
		current_predicate = root_predicate;
		current_predicate->parent = root_predicate; 
 		initElementSymbols();
	}	
	
	GAFFCESParser::GAFFCESParser(const String& cesstring) 
		: root(this)
	{
		root_predicate = &root;
		current_root_predicate = root_predicate;
		current_predicate = root_predicate;
		current_predicate->parent = root_predicate; 
 		initElementSymbols();
		parse(cesstring); 
		root_predicate = &root;
	}	

	GAFFCESParser::~GAFFCESParser()
	{
		root.clear();
		GAFFCESParser_destroy();
	}


	//delete predicates and their childPredicates
	void GAFFCESParser::CESPredicate::clear()
	{
		//if the present predicate still has children
		if (!children.empty())
		{
			std::vector<CESPredicate*>::iterator child_it = children.begin();
			for(; child_it != children.end(); ++child_it)
			{
				(*child_it)->clear();
				delete *child_it;
			}
			children.clear();	
		}
	}


	GAFFCESParser::CESelementConnectionPredicate::~CESelementConnectionPredicate()
	{
	}

	void GAFFCESParser::CESelementConnectionPredicate::setNumberOfPartners(Size number)
	{
		numberOfPartners_ = number;
	}

	Size GAFFCESParser::CESelementConnectionPredicate::getNumberOfPartners()
	{
		return numberOfPartners_;
	}

	void GAFFCESParser::CESelementConnectionPredicate::setElementName(String name)
	{
		elementName_ = name;
	}

	String GAFFCESParser::CESelementConnectionPredicate::getElementName()
	{
		return elementName_;
	}

	// check whether this atom is contained on a path to the root
	bool GAFFCESParser::CESPredicate::alreadySeenThisAtom(Atom* test_atom)
	{
		if (test_atom == atom_to_test)
		{
			return true;
		}
		if (this != parser_->root_predicate)
		{
			return parent->alreadySeenThisAtom(test_atom);
		}
		else
		{
			return false;
		}
	}

	//check if atom matches ElementConnectionPredicate
	bool GAFFCESParser::CESelementConnectionPredicate::match(Atom& atom)
	{
		bool correct_match = false;
		//match element-name?
		if((atom.getElement().getSymbol()) == elementName_)
		{
			//match number of bounded partners?
			if(((atom.countBonds()) == numberOfPartners_) || (numberOfPartners_ == 0))
			{
				correct_match = true;
			}
		}
		return correct_match;
	}



	GAFFCESParser::CESelementPredicate::~CESelementPredicate()
	{
	}

	void GAFFCESParser::CESelementPredicate::setElementName(String name)
	{
		elementName_ = name;
	}

	String GAFFCESParser::CESelementPredicate::getElementName()
	{
		return elementName_;
	}


	//check if atom matches ElementPredicate
	bool GAFFCESParser::CESelementPredicate::match(Atom& atom)
	{
		bool correct_match = false;
		//match element-name?
		if((atom.getElement().getSymbol())== elementName_)
		{
			correct_match = true;	
		}
		return correct_match;
	}			
	
	

	GAFFCESParser::CESwildcardsPredicate::~CESwildcardsPredicate()
	{
	}


	void GAFFCESParser::CESwildcardsPredicate::setWildcards(String new_wildcard)
	{
		wildcards_ = stringToWildcard_[new_wildcard];
	}

	GAFFCESParser::CESPredicate::CESwildcards GAFFCESParser::CESwildcardsPredicate::getWildcards()
	{
		return wildcards_;
	}

	//checks if atom matches the given wildcard-element (XA, XB, XD, XC, XX) 
	bool GAFFCESParser::CESwildcardsPredicate::matchWildcards(Atom& atom)
	{		
		bool matched = false;
		switch(wildcards_)
		{
			case XA:
				//XA: Sulfur/Sulpur and Oxygen
				if(		(atom.getElement()== PTE[Element::S]) 
						||(atom.getElement()== PTE[Element::O]))
				{
					matched = true;
				}
				break;
			case XB:
				//XB: Phosphorus/Phosphor and Nitrogen
				if(		(atom.getElement()== PTE[Element::P]) 
						||(atom.getElement()== PTE[Element::N]))
				{
					matched = true;
				}
				break;
			case XC:
				//XC:  Fluorine, Chloride, Bromide and Iodine 
				if (	(atom.getElement()== PTE[Element::F]) 
						||(atom.getElement()== PTE[Element::Cl])
						||(atom.getElement()== PTE[Element::Br])
						||(atom.getElement()== PTE[Element::I]))
				{
					matched = true;
				}
				break;
			case XD:
				//XD:  Phosphorus/Phosphor and Sulfur/Sulpur
				if (	(atom.getElement()== PTE[Element::P]) 
						||(atom.getElement()== PTE[Element::S]))
				{
					matched = true;
				}
				break;
			case XX:
				//XX:  Phosphorus/Phosphor, Sulfur/Sulpur, Nitrogen, Carbon, Oxygen
				if (	(atom.getElement()== PTE[Element::P]) 
						||(atom.getElement()== PTE[Element::N]) 
						||(atom.getElement()== PTE[Element::S]) 
						||(atom.getElement()== PTE[Element::C]) 
						||(atom.getElement()== PTE[Element::O]))
				{
					matched = true;
				}
				break;
		}

		return matched;
	}


	//check if atom matches WildcardsAtomicPropertyPredicatePredicate
	bool GAFFCESParser::CESwildcardsPredicate::match(Atom& atom)
	{
		bool correct_match = false;
		// match wildcard-element?
		if(matchWildcards(atom))
		{
			correct_match = true;
		}
		return correct_match;
	}			


	
	GAFFCESParser::CESwildcardsConnectionPredicate::~CESwildcardsConnectionPredicate()
	{
	}

	void GAFFCESParser::CESwildcardsConnectionPredicate::setWildcards(String new_wildcard)
	{
		wildcards_ = stringToWildcard_[new_wildcard];
	}

	GAFFCESParser::CESPredicate::CESwildcards GAFFCESParser::CESwildcardsConnectionPredicate::getWildcards()
	{
		return wildcards_;
	}

	void GAFFCESParser::CESwildcardsConnectionPredicate::setNumberOfPartners(Size number)
	{
		numberOfPartners_ = number;
	}

	Size GAFFCESParser::CESwildcardsConnectionPredicate::getNumberOfPartners()
	{
		return numberOfPartners_;
	}
	
	//checks if atom matches the given wildcard-element (XA, XB, XD, XC, XX) 
	bool GAFFCESParser::CESwildcardsConnectionPredicate::matchWildcards(Atom& atom)
	{
		bool matched = false;
		switch(wildcards_)
		{
			case XA:
				//XA: Sulfur/Sulpur and Oxygen
				if(		(atom.getElement()== PTE[Element::S]) 
						||(atom.getElement()== PTE[Element::O]))
				{
					matched = true;
				}
				break;
			case XB:
				//XB: Phosphorus/Phosphor and Nitrogen
				if(		(atom.getElement()== PTE[Element::P]) 
						||(atom.getElement()== PTE[Element::N]))
				{
					matched = true;
				}
				break;
			case XC:
				//XC:  Fluorine, Chloride, Bromide and Iodine 
				if (	(atom.getElement()== PTE[Element::F]) 
						||(atom.getElement()== PTE[Element::Cl])
						||(atom.getElement()== PTE[Element::Br])
						||(atom.getElement()== PTE[Element::I]))
				{
					matched = true;
				}
				break;
			case XD:
				//XD:  Phosphorus/Phosphor and Sulfur/Sulpur
				if (	(atom.getElement()== PTE[Element::P]) 
						||(atom.getElement()== PTE[Element::S]))
				{
					matched = true;
				}
				break;
			case XX:
				//XX:  Phosphorus/Phosphor, Sulfur/Sulpur, Nitrogen, Carbon, Oxygen
				if (	(atom.getElement()== PTE[Element::P]) 
						||(atom.getElement()== PTE[Element::N]) 
						||(atom.getElement()== PTE[Element::S]) 
						||(atom.getElement()== PTE[Element::C]) 
						||(atom.getElement()== PTE[Element::O]))
				{
					matched = true;
				}
				break;
		}

		return matched;
	}	

	//check if atom matches WildcardsConnectionPredicatePredicate
	bool GAFFCESParser::CESwildcardsConnectionPredicate::match(Atom& atom)
	{
#ifdef DEBUG
		Log.info() << "GAFFCESParser::CESwildcardsConnectionPredicate: trying to match atom " << atom.getFullName() << " with " 
							 << atom.countBonds() << " bonds against " << numberOfPartners_ << ":" << wildcards_;
#endif
		bool correct_match = false;
		//match wildcard-element?
		if(matchWildcards(atom))
		{
			//and number of bounded partners?
			if(		((atom.countBonds()) == numberOfPartners_) 
					||(numberOfPartners_ == 0))
			{
				correct_match = true;
			}
		}
#ifdef DEBUG
		if (correct_match)
			Log.info() << " (found a match!)" << std::endl;
		else
			Log.info() << " (no match!)" << std::endl;
#endif
		return correct_match;
	}


	Size GAFFCESParser::read(char* buf, Size max_size)
	{
		if (read_start_ >= cesstring_.size())
		{
			return 0;
		}
		Size up_to = std::min(cesstring_.size()-read_start_, (size_t)max_size);
		// note: this is extremely inefficient, but the strings are short
		for (int i=0; read_start_<up_to; read_start_++, i++)
		{ 
			buf[i] = cesstring_[read_start_];
		}
		return up_to;
	}


	//parse chemical environment string
	bool GAFFCESParser::parse(const String& cesstring)
	{
		state.current_parser = this;
		cesstring_ = cesstring;
		read_start_ = 0;

		if(		(current_root_predicate == root_predicate) 
				&&(current_predicate      == root_predicate))
		{
			if(!GAFFCESParserparse())
			{
				current_predicate = root_predicate;
				return true;
			}
			else
			{
				Log.error() << "Parsing of CESstring failed" << std::endl;
			}
		
		}
		else
		{
			Log.error() << "Clearing of predicate tree failed" << std::endl;
		}
		return false;
	}


	//check if any atom matches parsed ces_string
	bool GAFFCESParser::GAFFCESatomMatcher(Atom& atom, const String& cesstring)
	{
		//CESstring successfully parsed and "predicate-tree" constructed
		if(parse(cesstring))
		{
			//does atom match "predicate-tree"?
			if ((*root_predicate)(atom))
			{
				return true;
			}
		}
		return false;
	}


	//start filling the children vector for a current predicate
	void GAFFCESParser::startChildPredicates()
	{
		//current_root_predicate set to current_predicate for addPredicates
		current_root_predicate = current_predicate;		
	}
		

	//end up filling the children vector for a current predicate 
	void GAFFCESParser::endChildPredicates()
	{
		//if highest root level reached
		if(current_root_predicate == root_predicate)
		{	
			current_predicate = root_predicate;
		}
		else
		{
			current_predicate = current_predicate->parent;	
			//current_root set to superior layer
			current_root_predicate = current_predicate->parent;
		}
	}


	//initialize Set of Elementsymbols
	void GAFFCESParser::initElementSymbols()
	{
		element_symbols_.insert("C");	element_symbols_.insert("H");
		element_symbols_.insert("N");	element_symbols_.insert("O");
		element_symbols_.insert("S");	element_symbols_.insert("P");
		element_symbols_.insert("F");	element_symbols_.insert("Cl");
		element_symbols_.insert("Br");	element_symbols_.insert("I");
	}


	const std::set<String>& GAFFCESParser::getElementSymbols()
	{
		return element_symbols_;
	}


	struct GAFFCESParser::State GAFFCESParser::state;
}

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

//defined in the lexer (GAFFCESParserLexer.l)
extern void GAFFCESParser_initBuffer(const char* buf);
extern void GAFFCESParser_delBuffer();
extern int  GAFFCESParserparse();

namespace BALL
{		

	GAFFCESParser::APSMatcher::APSMatcher(const String& aps)
		: aps_string(aps)
	{
	}

	GAFFCESParser::APSMatcher::~APSMatcher()
	{
	}	

	//checks if the current atom is in a ring with size n
	bool GAFFCESParser::APSMatcher::isNRingAtom(Size size, Atom& atom)
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

		return atom.getProperty(property).getBool();
	}

	//checks if current atom is in a ring
	bool GAFFCESParser::APSMatcher::isRingAtom(Atom& atom)
	{
		// yes, this can probably be done in a better way :-)
		// we don't merely check for "InRing" because rings need to be smaller than 10 for GAFF
		for (Size i=3; i<10; i++)
		{
			if (isNRingAtom(i, atom))
				return true;
		}

		return false;
	}

	//check if the current atom is not a ringatom or
	//in a ten-membered or larger ring
	bool GAFFCESParser::APSMatcher::isNonRingAtom(Atom& atom)
	{
		/*if(!(atom.getProperty("InRing").getBool()))
		{
			return true;
		}
		else if(atom.getProperty("RingSize").getInt() >= 10)
		{
 			atom.setProperty("InRing",(bool) false);	
			return true;
		}
		*/
		return !isRingAtom(atom);
	}	

	//checks if current atom is in an aromatic ringsystem
	bool GAFFCESParser::APSMatcher::isPureAromaticRingAtom(Atom& atom)
	{
		return (atom.getProperty("IsAromatic").getBool());
	}

	//checks if current atom is in an aliphatic ringsystem,
	// which is made of sp3 Carbon
	bool GAFFCESParser::APSMatcher::isPureAliphaticRingAtom(Atom& atom)	
	{
		return atom.getProperty("IsPureAliphatic").getBool();
	}

	//check if current atom could be in a planar ring
	//s.t. atom is a C(X3),N(X2),N(X3),O(X2),S(X2),P(X2),P(X3)
	bool GAFFCESParser::APSMatcher::CouldBePlanarRingAtom(Atom& atom)
	{
		return (atom.getProperty("CouldBePlanar").getBool());
	}

	//check if current ringsystem is planar
	//s.t. consist of C(X3),N(X2),N(X3),O(X2),S(X2),P(X2),P(X3)
	bool GAFFCESParser::APSMatcher::isPlanarRingAtom(Atom& atom)
	{
		return atom.getProperty("PlanarRing").getBool();
	}

	//check various properties for atom
	bool GAFFCESParser::APSMatcher::checkProperty(Atom& atom, String property_term)
	{
		if (property_term == "NR")
		{
			return isNonRingAtom(atom);
		}
		else if (isRingAtom(atom))
		{
			// NOTE: our definitions of AR2 and AR3 are not entirely correct (and identical)
	
			//Ring (from 3-membered to nine-membered)
			if (property_term == "RG")
			{
				return isRingAtom(atom);
			}
			//3-membered ring
			if (property_term == "RG3")
			{
				return isNRingAtom(3, atom);
			}
			//4-membered ring
			if (property_term == "RG4")
			{
				return isNRingAtom(4, atom);
			}
			//5-membered ring
			if (property_term == "RG5")
			{
				return isNRingAtom(5, atom);
			}
			//6-membered ring
			if (property_term == "RG6")
			{
				return isNRingAtom(6, atom);
			}
			//7-membered ring
			if (property_term == "RG7")
			{
				return isNRingAtom(7, atom);
			}
			//8-membered ring
			if (property_term == "RG8")
			{
				return isNRingAtom(8, atom);
			}
			//9-membered ring
			if (property_term == "RG9")
			{
				return isNRingAtom(9, atom);
			}
			//pure aromatic atom (such as benzene and pyridine)
			if (property_term == "AR1")
			{
				return isPureAromaticRingAtom(atom);
			}
			//TODO better definition
			//atom in a planar ring
			if (property_term == "AR2")
			{
				return CouldBePlanarRingAtom(atom);
			}
			//TODO better definition
			//atom in a planar ring which hast one or 
			//several double bonds formed between NR and RG atoms
			if (property_term == "AR3")
			{
				return CouldBePlanarRingAtom(atom);
			}
			//atom other than AR1, AR2, AR3, AR5
			if (property_term == "AR4")
			{
				if(   isPureAromaticRingAtom(atom)
					|| CouldBePlanarRingAtom(atom)
					||  isPlanarRingAtom(atom) || 
							isPureAliphaticRingAtom(atom))
				{
					return false;
				}
				else return true;
			}
			//pure aliphatic atom in a ring which is made of sp3 carbon
			if (property_term == "AR5")
			{
				return isPureAliphaticRingAtom(atom);
			}
		}
		//if property_term is no ringtype
		//it has to be of bondtype SB, DB, TB, AB, DL, sb, db, tb
		else
		{ 
			Atom::BondConstIterator constBond_it = atom.beginBond();
			for(;+constBond_it;++constBond_it)
			{
				String gaff_bond_type = constBond_it->getProperty("GAFFBondType").getString();
				if(property_term == gaff_bond_type)
				{
					return true;
				}
			}
		}
		return false;
	}

	void GAFFCESParser::APSMatcher::addNewAND(GAFFCESParser::APSMatcher::APSType aps)
	{
		
	}

	void GAFFCESParser::APSMatcher::addNewOR(GAFFCESParser::APSMatcher::APSType aps)
	{
			
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
	void GAFFCESParser::CESPredicate::addCESwildcardsAtomicPropertyPredicate(String atomic_property, String wildcard)
	{
		CESwildcardsAtomicPropertyPredicate* wildcardsAtomicPropertyPredicate = new CESwildcardsAtomicPropertyPredicate(parser_);
		wildcardsAtomicPropertyPredicate->parent = parser_->current_root_predicate;
		wildcardsAtomicPropertyPredicate->setAtomicProperty(atomic_property);
		wildcardsAtomicPropertyPredicate->setWildcards(wildcard);
		parser_->current_predicate = wildcardsAtomicPropertyPredicate;
		parser_->current_root_predicate->children.push_back(wildcardsAtomicPropertyPredicate);	
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

	//add a TruePredicate to "predicate tree"
	void GAFFCESParser::CESPredicate::addTruePredicate()
	{
		TruePredicate* truePredicate = new TruePredicate(parser_);
		truePredicate->parent = parser_->current_root_predicate;
		parser_->current_predicate = truePredicate;
		parser_->current_root_predicate->children.push_back(truePredicate);
	}

	bool GAFFCESParser::match(Atom& atom)
	{
		return (*root_predicate)(atom);
	}

	//check if atom and its environment match predicates
	bool GAFFCESParser::CESPredicate::operator()(Atom& atom)	
	{
		std::cout << "in match "<< std::endl;
		atom_to_test = &atom;

		//present predicate has child-predicates
		if(!(children.empty()))
		{
			//present predicate isn't root and matches atom
			if((parser_->root_predicate != this) && (match(atom)))
			{
				std::cout << "matched atom and children " << std::endl;
				//stores if a children-predicate is already matched or not
				std::vector<bool>match_events((children.size()),false);
	
				Atom::BondIterator bond_it = atom.beginBond();
				for(;+bond_it;++bond_it)
				{
					Atom& partnerAtom = *(bond_it->getPartner(atom));
					std::cout << "trying " << partnerAtom.getName() << " " << atom.getName() << " " << parent->alreadySeenThisAtom(&partnerAtom) << std::endl;	
					if (parent->alreadySeenThisAtom(&partnerAtom))
						continue;

					for (Size i=0; (i<children.size()); i++)
					{
						std::cout << "matching " << i << std::endl;
						std::cout << "partnerAtom:" << partnerAtom.getName() <<  std::endl;
						//if predicate was'nt matched before
						if(!(match_events[i]))
						{
							std::cout << "not yet matched" << std::endl;
							//if one match of atom and predicate is found
							if((*children[i])(partnerAtom))
							{
								std::cout << "gematcht" << std::endl;
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
				for(std::vector<bool>::iterator match_it = match_events.begin() ; match_it != match_events.end();++match_it)
				{
					//if any match_event is false, no complete match could be found
					if(!(*(match_it)))
					{
						std::cout << "false" << endl;
						return false;	
					}
				}			
				return true;
			}
			//root_predicate cannot be matched with any atom in CES
	 		//therefore: only matching of all its children
			else if(parser_->root_predicate == this)
			{
				std::cout << "root" << std::endl;
				//stores if a children-predicate is already matched or not
				std::vector<bool>match_events((children.size()),false);
	
				Atom::BondIterator bond_it = atom.beginBond();
				for(;+bond_it;++bond_it)
				{
					Atom& partnerAtom = *(bond_it->getPartner(atom));
	
					for (Size i=0; (i<children.size()); i++)
					{
						std::cout << "matching " << i << std::endl;
						std::cout << "partnerAtom:" << partnerAtom.getElement().getName() <<  std::endl;
						//if predicate was'nt matched before
						if(!(match_events[i]))
						{
							std::cout << "not yet matched" << std::endl;
							//if one match of atom and predicate is found
							if((*children[i])(partnerAtom))
							{
								std::cout << "gematcht" << std::endl;
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
				for(Size i=0; i<match_events.size(); i++)
				{
					//if any match_event is false, no complete match could be found
					if(!match_events[i])
					{
						std::cout << "false" << endl;
						return false;	
					}
				}			
				return true;
			}
		}
		else
		{
			std::cout << "no children" << std::endl;
			return (match(atom));
		}	
		std::cout << "no match possible " << std::endl;
		return false;
	}
		

	GAFFCESParser::GAFFCESParser() 
		: root(this),
			root_predicate(&root),
			current_root_predicate(root_predicate),
			current_predicate(root_predicate)
	{
printf("GAFFCESParser()\n");
		current_predicate->parent = root_predicate; 
 		initElementSymbols();
	}	
	
	GAFFCESParser::GAFFCESParser(const String& cesstring) 
		: root(this),
			root_predicate(&root),
			current_root_predicate(root_predicate),
			current_predicate(root_predicate)
	{
printf("GAFFCESParser(String %s)\n", cesstring.c_str());
		current_predicate->parent = root_predicate; 
 		initElementSymbols();
		parse(cesstring); 
	}	

	GAFFCESParser::~GAFFCESParser()
	{
		root.clear();
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
		std::cout << "matchCESelementConnectionPredicate" << std::endl;
		bool correct_match = false;
		std::cout << "AtomName:" << atom.getElement().getSymbol() << "" << elementName_ << std::endl;
		//match element-name?
		if((atom.getElement().getSymbol()) == elementName_)
		{
			std::cout << "NumberOfPartners:" << atom.countBonds() << "" << numberOfPartners_ << std::endl;
			//match number of bounded partners?
			if(((atom.countBonds()) == numberOfPartners_) || (numberOfPartners_ == 0))
			{
				std::cout << "correct_match" << std::endl;
				correct_match = true;
			}
		}
		std::cout << "correct_match:" << correct_match << std::endl;
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
		std::cout << "trying to match " << atom.getName() << " against " << elementName_ << std::endl;
		bool correct_match = false;
		std::cout << "AtomName:" << atom.getElement().getSymbol() << "" << elementName_ << std::endl;
		//match element-name?
		if((atom.getElement().getSymbol())== elementName_)
		{
			std::cout << "correct_match" << std::endl;
			correct_match = true;	
		}
		std::cout << "correct_match:" << correct_match << std::endl;
		return correct_match;
	}			
	
	

	GAFFCESParser::CESwildcardsAtomicPropertyPredicate::~CESwildcardsAtomicPropertyPredicate()
	{
	}

	void GAFFCESParser::CESwildcardsAtomicPropertyPredicate::setAtomicProperty(String new_atomic_property)
	{
		atomic_property_ = new_atomic_property;
	}

	String GAFFCESParser::CESwildcardsAtomicPropertyPredicate::getAtomicProperty()
	{
		return atomic_property_;
	}

	void GAFFCESParser::CESwildcardsAtomicPropertyPredicate::setWildcards(String new_wildcard)
	{
		wildcards_ = stringToWildcard_[new_wildcard];
	}

	GAFFCESParser::CESPredicate::CESwildcards GAFFCESParser::CESwildcardsAtomicPropertyPredicate::getWildcards()
	{
		return wildcards_;
	}

	//checks if atom matches the given wildcard-element (XA, XB, XD, XC, XX) 
	bool GAFFCESParser::CESwildcardsAtomicPropertyPredicate::matchWildcards(Atom& atom)
	{		
		std::cout << "matchCESwildcardsAtomicPropertyPredicate" << std::endl;
		bool matched = false;
		switch(wildcards_)
			{
			//XA: Sulfur/Sulpur and Oxygen
			case 0:
				if(		(atom.getElement()== PTE[Element::S]) 
						||(atom.getElement()== PTE[Element::O]))
				{
					matched = true;
				}
				break;
			//XB: Phosphorus/Phosphor and Nitrogen
			case 1:
				if(		(atom.getElement()== PTE[Element::P]) 
						||(atom.getElement()== PTE[Element::N]))
				{
					matched = true;
				}
				break;
			//XD:  Phosphorus/Phosphor and Sulfur/Sulpur
			case 2:
				if (	(atom.getElement()== PTE[Element::P]) 
						||(atom.getElement()== PTE[Element::S]))
				{
					matched = true;
				}
				break;
			//XC:  Fluorine, Chloride, Bromide and Iodine 
			case 3:
				if (	(atom.getElement()== PTE[Element::F]) 
						||(atom.getElement()== PTE[Element::Cl])
						||(atom.getElement()== PTE[Element::Br])
						||(atom.getElement()== PTE[Element::I]))
				{
					matched = true;
				}
				break;
			//XX:  Phosphorus/Phosphor, Sulfur/Sulpur, Nitrogen, Carbon, Oxygen
			case 4:
				std::cout << "XX" << endl;
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
	bool GAFFCESParser::CESwildcardsAtomicPropertyPredicate::match(Atom& atom)
	{
		std::cout << "matchCESwildcardsAtomicPropertyPredicate" << std::endl;
	
		bool correct_match = false;
		// match wildcard-element?
		if(matchWildcards(atom))
		{
			std::cout << "wildcards gemachted" << endl;
			//and ,if given, the atomic property string?
			if(atomic_property_ != "")
			{
				//does atom match given atomic property string?
				/* TODO!!!! This does no longer exist!
				if(atomic_environments->AtomicPropertyChecker(atom, atomic_property_))
				{
					std::cout << "AtomPropertyChecker:" << correct_match << endl;
					correct_match = true;
					std::cout << "AfterAtomPropertyChecker:" << correct_match << endl;
				}
				*/
			}
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
			//XA: Sulfur/Sulpur and Oxygen
			case 0:
				if(		(atom.getElement()== PTE[Element::S]) 
						||(atom.getElement()== PTE[Element::O]))
				{
					matched = true;
				}
				break;
			//XB: Phosphorus/Phosphor and Nitrogen
			case 1:
				if(		(atom.getElement()== PTE[Element::P]) 
						||(atom.getElement()== PTE[Element::N]))
				{
					matched = true;
				}
				break;
			//XD:  Phosphorus/Phosphor and Sulfur/Sulpur
			case 2:
				if (	(atom.getElement()== PTE[Element::P]) 
						||(atom.getElement()== PTE[Element::S]))
				{
					matched = true;
				}
				break;
			//XC:  Fluorine, Chloride, Bromide and Iodine 
			case 3:
				if (	(atom.getElement()== PTE[Element::F]) 
						||(atom.getElement()== PTE[Element::Cl])
						||(atom.getElement()== PTE[Element::Br])
						||(atom.getElement()== PTE[Element::I]))
				{
					matched = true;
				}
				break;
			//XX:  Phosphorus/Phosphor, Sulfur/Sulpur, Nitrogen, Carbon, Oxygen
			case 4:
				std::cout << "XX" << endl;
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
		printf("ceswildcard...\n");
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
		return correct_match;
	}


	Size GAFFCESParser::read(char* buf, Size max_size)
	{
		if (read_start_ >= cesstring_.size())
		{
			return 0;
		}
		Size up_to = std::min(cesstring_.size()-read_start_, max_size);
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
		printf("cesstring %s\n", cesstring_.c_str());
		if(		(current_root_predicate == root_predicate) 
				&&(current_predicate == root_predicate))
		{
			std::cout << "settings ok" << endl;			

			if(!GAFFCESParserparse())
			{
				std::cout << "Parserparse succeeded" << endl;
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
				std::cout << "op:" << ((*root_predicate)(atom)) << endl;
				std::cout << "atomMatcher succeeded" << endl;
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


	const set<String>& GAFFCESParser::getElementSymbols()
	{
		return element_symbols_;
	}


	struct GAFFCESParser::State GAFFCESParser::state;
}

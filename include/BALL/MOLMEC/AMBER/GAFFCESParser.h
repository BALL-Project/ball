// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:

#ifndef BALL_MOLMEC_AMBER_GAFFCESPARSER_H
#define BALL_MOLMEC_AMBER_GAFFCESPARSER_H

#ifndef BALL_KERNEL_ATOM_H
#include <BALL/KERNEL/atom.h>
#endif

#ifndef BALL_DATATYPE_STRING_H
#include <BALL/DATATYPE/string.h>
#endif

#include <set>
#include <map>
#include <vector>

namespace BALL
{
	
	class GAFFCESParser
	{
		public:

			struct State
			{
				//not "thread-safe"
				GAFFCESParser* current_parser;
				
				int current_aps_type;
				int current_bond_to_parent;
			};
	
			//atomic property string
			class APSMatcher
			{
				public:

						String aps_string;
						//encode Ringatomtypes
						enum APSType
						{
							IS_RING_ATOM,
							IS_NON_RING_ATOM,
							IS_PLANAR,
							IS_PLANAR_WITH_DB_TO_NR,
							IS_PURELY_AROMATIC,
							IS_ALIPHATIC,
							IS_OTHER_RING,
							IS_3_RING_ATOM,
							IS_4_RING_ATOM,
							IS_5_RING_ATOM,
							IS_6_RING_ATOM,
							IS_7_RING_ATOM,
							IS_8_RING_ATOM,
							IS_9_RING_ATOM,
							ONLY_SINGLE_BOND,
							SINGLE_BOND,
							ONLY_DOUBLE_BOND,
							DOUBLE_BOND,
							TRIPLE_BOND,
							DELOCALIZED_BOND,
							AROMATIC_BOND
						};

 						//check if current atom is in a ring
 						bool isRingAtom(Atom& atom);
 						//check if the current atom is in a ring with size n
 						bool isNRingAtom(Size size, Atom& atom);
 						//check if the current atom is not a ringatom or
 						//in a ten-membered or larger ring
 						bool isNonRingAtom(Atom& atom);	
 						//check if current atom is in an aromatic ringsystem
 						bool isPureAromaticRingAtom(Atom& atom);
 						//check if current atom is in an aliphatic ringsystem
 						bool isPureAliphaticRingAtom(Atom& atom);
 						//check if current atom could be in a planar ring
 						//s.t. atom is a C(X3),N(X2),N(X3),O(X2),S(X2),P(X2),P(X3)
 						bool CouldBePlanarRingAtom(Atom& atom);
 						//check if current ringsystem is planar
 						//s.t. consist of C(X3),N(X2),N(X3),O(X2),S(X2),P(X2),P(X3)
 						bool isPlanarRingAtom(Atom& atom);

						bool checkProperty(Atom& atom, String property_term);

						APSMatcher(const String& aps);
						~APSMatcher();

						void addNewAND();
						void addNewOR(APSType aps);

						bool operator() (Atom& atom);

						//store atomic property string
						//external vector: all AND (",") types
						//internal vector: all OR (".")types
						std::vector < std::vector< APSType> > aps_terms;
			};

			//chemical environment string
			class CESPredicate
			{
				public:
					//encode wildcard elements
					enum CESwildcards
					{
						XA, XB, XC, XD, XX		
					};

					CESPredicate(GAFFCESParser* parser) 
						: parent(0),
							parser_(parser)
				  {
						initStringToWildcard();
					};

					virtual ~CESPredicate();

					//initialize stringToWildcard map
					void initStringToWildcard();
					std::map<String, CESwildcards> getStringToWildcard();

					//add a CESwildcardsConnectionPredicate to "predicate tree"
					void addCESwildcardsConnectionPredicate(String wildcard, Size partners);
					//add a CESwilddcardsAtomicPropertyPredicate to "predicate tree"
					void addCESwildcardsAtomicPropertyPredicate(String atomic_property, String wildcard);
					//add an CESelementPredicate to "predicate tree"
					void addCESelementPredicate(String name);
					//add an CESelementConnectionPredicate to "predicate tree"
					void addCESelementConnectionPredicate(Size partners, String name);
					// add a TruePredicate to "predicate tree"
					void addTruePredicate();

					//check if atom and its environment match predicates
					virtual bool operator () (Atom& atom);
					//check if atom matches predicates
					virtual bool match(Atom& /*atom*/){return false;};
					//delete children 
					void clear();

					// check whether this atom is contained on a path to the root
					bool alreadySeenThisAtom(Atom* atom);
					//TODO wo setzen???
					//store existing atomic property string
					APSMatcher aps_matcher(const String& aps_string);	
				
					//all CESPredicates for current_predicate ->children of current_predicate
					std::vector<CESPredicate*> children;
					//CESPredicate, that has current_predicate in his children vector
					CESPredicate* parent;

					//The atom we are trying to match to this predicate
					Atom* atom_to_test;
					protected:
						//map to convert String into wildcard-element
						std::map<String, CESwildcards > stringToWildcard_;		
						GAFFCESParser* parser_;
			};
	
			//element-name of the partnerAtom and number of its connected atoms
			class CESelementConnectionPredicate : public CESPredicate
			{
				public:
					CESelementConnectionPredicate(GAFFCESParser* parser) 
						: CESPredicate(parser),
							numberOfPartners_(0), 
							elementName_("NoName") 
					{};

					~CESelementConnectionPredicate();

					void setNumberOfPartners(Size number);
					void setElementName(String name);
					Size getNumberOfPartners();
					String getElementName();
					//check if atom matches predicate
					bool match(Atom& atom);
				protected:
					Size numberOfPartners_;
					String elementName_;
			};
	
			//element-name of the partnerAtom
			class CESelementPredicate : public CESPredicate
			{
				public:
					CESelementPredicate(GAFFCESParser* parser) 
						: CESPredicate(parser),
							elementName_("NoName") 
					{
					};

					~CESelementPredicate();			

					void setElementName(String name);
					String getElementName();
					//check if atom matches predicate
					bool match(Atom& atom);
				protected:
					String elementName_;
			};

			//XA,XB,XC,XD,XX and atomic property string found
			class CESwildcardsAtomicPropertyPredicate : public CESPredicate
			{
				public:
					CESwildcardsAtomicPropertyPredicate(GAFFCESParser* parser)
						: CESPredicate(parser),
							atomic_property_("") 
					{};

					~CESwildcardsAtomicPropertyPredicate();

					void setAtomicProperty(String atomic_property);
					void setWildcards(String new_wildcard);
					String getAtomicProperty();
					CESwildcards getWildcards();
					//checks if atom matches the given wildcard-element (XA, XB, XC, XD, XX) 
					bool matchWildcards(Atom& atom);
					//check if atom matches predicate
					bool match(Atom& atom);
				protected:
					//store atomic property string if given in CES
					String atomic_property_;
					CESwildcards wildcards_;
			};

			//XA,XB,XC,XD,XX and number of partnerAtoms
			class CESwildcardsConnectionPredicate : public CESPredicate
			{
				public:
					CESwildcardsConnectionPredicate(GAFFCESParser* parser) 
						: CESPredicate(parser),
							numberOfPartners_(0) 
					{};
					~CESwildcardsConnectionPredicate();

					void setNumberOfPartners(Size number);
					void setWildcards(String new_wildcard);
					Size getNumberOfPartners();
					CESwildcards getWildcards();
					//checks if atom matches the given wildcard-element (XA, XB, XC, XD, XX) 
					bool matchWildcards(Atom& atom);
					//check if atom matches predicate
					bool match(Atom& atom);
				protected:
					CESwildcards wildcards_;
					Size numberOfPartners_;
			};

			//string is "*" which means always true
			class TruePredicate : public CESPredicate
			{
				public:
					TruePredicate(GAFFCESParser* parser)
						: CESPredicate(parser)
					{};
					~TruePredicate() {};
					//check if atom matches predicate (always true!)
					bool match(Atom& /*atom*/) { return true; }
			};

			//Parser-match-Function checking if atom's environment matches the "predicate tree"
			bool match(Atom& atom);

			//initialize Set of Elementsymbols
			void initElementSymbols();
			const set<String>& getElementSymbols();

			GAFFCESParser();	
			GAFFCESParser(const String& cesstring);
			GAFFCESParser(const GAFFCESParser& parser);
			~GAFFCESParser();

			//for lexer/parser
			Size read(char* buf, Size max_size);

			static State state;
			CESPredicate root;
			//fixed root of the "predicate tree"
			CESPredicate* root_predicate;
			//parent-predicate of current_predicate
			CESPredicate* current_root_predicate;
			//"predicate-node" in the "predicate tree" we actually considering
			CESPredicate* current_predicate;
		
			
			//parse chemical environment string
			bool parse(const String& cesstring);
			//check if any atom matches parsed ces_string
			bool GAFFCESatomMatcher(Atom& atom, const String& cesstring);
			//start filling the children vector for a current predicate
			void startChildPredicates();
			//end up filling the children vector for a current predicate 
			void endChildPredicates();

		protected:
			//current chemical environment string
			String cesstring_;
			//set with all valid element symbols
			set<String> element_symbols_;
			//for Parser/Lexer function YYINPUT
			Position read_start_;
	};
	
}

#endif

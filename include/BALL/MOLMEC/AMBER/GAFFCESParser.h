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

			//atomic property string
			class APSMatcher
			{
				public:
						//encode Ringatomtypes
						enum APSType
						{
							IS_RING_ATOM,
							IS_NON_RING_ATOM,
							IS_PLANAR,
							IS_PLANAR_WITH_DB_TO_NR,
							IS_PURELY_AROMATIC,
							IS_PURELY_ALIPHATIC,
							IS_OTHER_RING,
							IS_3_RING_ATOM,
							IS_4_RING_ATOM,
							IS_5_RING_ATOM,
							IS_6_RING_ATOM,
							IS_7_RING_ATOM,
							IS_8_RING_ATOM,
							IS_9_RING_ATOM,
 							PURE_SINGLE_BOND,
 							PURE_SINGLE_BOND_TO_PARENT,
 							NO_PURE_SINGLE_BOND_TO_PARENT,
							SINGLE_BOND,
							SINGLE_BOND_TO_PARENT,
							NO_SINGLE_BOND_TO_PARENT,
							PURE_DOUBLE_BOND,
							PURE_DOUBLE_BOND_TO_PARENT,
							NO_PURE_DOUBLE_BOND_TO_PARENT,
							DOUBLE_BOND,
							DOUBLE_BOND_TO_PARENT,
							NO_DOUBLE_BOND_TO_PARENT,
							TRIPLE_BOND,
							TRIPLE_BOND_TO_PARENT,
							NO_TRIPLE_BOND_TO_PARENT,
							DELOCALIZED_BOND,
							DELOCALIZED_BOND_TO_PARENT,
							NO_DELOCALIZED_BOND_TO_PARENT,
							AROMATIC_BOND,
							AROMATIC_BOND_TO_PARENT,
							NO_AROMATIC_BOND_TO_PARENT,
							APS_TRUE
						};

						class APSTerm
						{
							public:
								APSTerm(APSType new_type, int new_feature_number)
									: type(new_type),
										feature_number(new_feature_number)
								{}

								APSType type;
								//contain the number of occurence of a given feature
								int feature_number;
						};

 						//check if current atom is in a ring
 						bool isRingAtom(Atom& atom);
 						//check if the current atom is in a ring with size n
						//and return the number of occurence
 						int isNRingAtom(Size size, Atom& atom);
 						//check if the current atom is not a ringatom or
 						//in a ten-membered or larger ring
 						bool isNonRingAtom(Atom& atom);	

						bool checkGAFFProperties(Atom& atom, Atom& predecessor, APSTerm aps);


						APSMatcher();
						~APSMatcher();

						String printAPS();

						// check if atom matches atomic property string
						bool operator() (Atom& atom, Atom& predecessor);

						//store atomic property string
						//external vector: all AND (",") types
						//internal vector: all OR (".")types
						std::vector < std::vector< APSTerm> > aps_terms;

					protected:
						bool hasBond_(Atom* atom, Atom* predecessor, int bond_type, int feature_number);
			};

			struct State
			{
				//not "thread-safe"
				GAFFCESParser* current_parser;
				
				APSMatcher::APSType current_aps_type;
				int feature_number;
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
							atom_to_test(0),
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
					//add a CESwilddcardsPredicate to "predicate tree"
					void addCESwildcardsPredicate(String wildcard);
					//add an CESelementPredicate to "predicate tree"
					void addCESelementPredicate(String name);
					//add an CESelementConnectionPredicate to "predicate tree"
					void addCESelementConnectionPredicate(Size partners, String name);
					// add a TruePredicate to "predicate tree"
					void addTruePredicate() const;

					//check if atom and its environment match predicates
					virtual bool operator () (Atom& atom);
					//check if atom matches "predicates in predicate-tree"
					virtual bool match(Atom&){return false;};
					//delete children 
					void clear();

					// check whether this atom is contained on a path to the root
					bool alreadySeenThisAtom(Atom* atom);

					//to expand aps_term in aps_matcher object
					void addNewAND();
					void addNewOR(APSMatcher::APSType aps, int feature_number);

					//store existing atomic property string
					APSMatcher aps_matcher;	
				
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

			//XA,XB,XC,XD,XX 
			class CESwildcardsPredicate : public CESPredicate
			{
				public:
					CESwildcardsPredicate(GAFFCESParser* parser)
						: CESPredicate(parser)
					{};

					~CESwildcardsPredicate();

					void setWildcards(String new_wildcard);
					CESwildcards getWildcards();
					//checks if atom matches the given wildcard-element (XA, XB, XC, XD, XX) 
					bool matchWildcards(Atom& atom);
					//check if atom matches predicate
					bool match(Atom& atom);
				protected:
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
					bool match(Atom&) { return true; }
			};

			//Parser-match-Function checking if atom's environment matches the "predicate tree"
			bool match(Atom& atom) const;

			//initialize Set of Elementsymbols
			void initElementSymbols();
			const std::set<String>& getElementSymbols();

			GAFFCESParser();	
			GAFFCESParser(const String& cesstring);
			~GAFFCESParser();

			//for lexer/parser
			Size read(char* buf, Size max_size);

			static State state;

			/// The root atom has no CESPredicate to test, so it's considered always true
			TruePredicate root;
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
			std::set<String> element_symbols_;
			//for Parser/Lexer function YYINPUT
			Position read_start_;
	};
	
}

#endif

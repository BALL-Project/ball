// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: smartsMatcher.h,v 1.2 2005/09/26 00:21:53 bertsch Exp $
//

#ifndef BALL_STRUCTURE_SMARTSMATCHER_H
#define BALL_STRUCTURE_SMARTSMATCHER_H

#ifndef BALL_DATATYPE_HASHSET_H
# include <BALL/DATATYPE/hashSet.h>
#endif

#ifndef BALL_STRUCTURE_SMARTSPARSER_H
# include <BALL/STRUCTURE/smartsParser.h>
#endif

#include <vector>

namespace BALL 
{

	/** @name	\brief SMARTS Parser.
	*/
	class SmartsMatcher
	{
		public: 
			
			typedef SmartsParser::SPNode SPNode;
			typedef SmartsParser::SPEdge SPEdge;
			typedef SmartsParser::SPAtom SPAtom;
			typedef SmartsParser::SPBond SPBond;
		
			/**	@name Constructors and Destructors
			*/
			//@{
	
			/// default constructor
			SmartsMatcher();
				
			/// copy constructor
			SmartsMatcher(const SmartsMatcher& matcher);

			/// method to match a SMARTS pattern given as a string to given molecule
			std::vector<HashSet<const Atom*> > match(const Molecule& mol, const String& smarts)
				throw(Exception::ParseError);

			/// destructor
			virtual ~SmartsMatcher();
			//@}

			/// assignment operator
			SmartsMatcher& operator = (const SmartsMatcher& matcher);
			
		protected:

			/// little helper struct for the recursive matching algorithm
			struct RecStruct_
			{
				public:
					
					/// default constructor
					RecStruct_();
					
					/// copy constructor
					RecStruct_(const RecStruct_& rec_struct);
	
					/// destructor
					virtual ~RecStruct_();
					
					/// holds the matched atoms molecule
					std::vector<HashSet<const Atom*> > matched_atoms;
	
					/// holds the mapping from SPNodes of the Smarts tree to the atoms of the molecule
					std::vector<HashMap<const SPNode*, const Atom*> > mapped_atoms;
	
					/// holds the visited atoms of the molecule
					std::vector<HashSet<const Atom*> > visited_atoms;
	
					/// holds the visited bonds of the molecule
					std::vector<HashSet<const Bond*> > visited_bonds;
	
					/// holds the visited edges of the smarts tree
					std::vector<HashSet<const SPEdge*> > visited_edges;
	
					/// adds the content of the given struct
					void add(const RecStruct_& rec_struct);
	
					/// adds the the ith part of the content of the given struct
					void add(const RecStruct_& rec_struct, Size i);

					/// deletes all contents
					void clear();
	
					/// assignment operator
					RecStruct_& operator = (const RecStruct_& rec_struct);
			};
			
			/// mehtod for evaluation of ring edges, after the the smarts tree is matched to molcule
			bool evaluateRingEdges_(const HashSet<const Atom*>& matching, const HashMap<const SPNode*, const Atom*>& mapping);
			
			/// method for the evaluation of a pseudo-tree
			void evaluate_(	RecStruct_& rs, SPNode* start_node, const Atom* start_atom);
	
			/// method for evaluating a node of a pseudo-tree
			bool evaluate_node_(RecStruct_& rs, SPNode* start_node, const Atom* start_atom);
	
			/// method for evaluating a edge of a pseudo-tree 
			bool evaluate_edge_(RecStruct_& rs, SPEdge* start_node, const Atom* start_atom, const Bond* start_bond);	
	};
  
} // namespace BALL

#endif // BALL_STRUCTURE_SMARTSMATCHER_H

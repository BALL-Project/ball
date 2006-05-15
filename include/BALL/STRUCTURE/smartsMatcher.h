// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: smartsMatcher.h,v 1.3.2.2 2006/05/15 12:20:11 amoll Exp $
//
// Author:
//   Andreas Bertsch
//
// Author:
//   Andreas Bertsch
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

	/** @name	\brief SMARTS Matcher
	*/
	class BALL_EXPORT SmartsMatcher
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
			void match(std::vector<HashSet<const Atom*> >& matches, Molecule& mol, const String& smarts)
				throw(Exception::ParseError);

			/// method to match a SMARTS pattern given as a string to given molcule. The atoms which will be used for starting matching are given in atoms
			void match(std::vector<HashSet<const Atom*> >& matches, Molecule& mol, const String& smarts, const HashSet<const Atom*>& atoms)
				throw(Exception::ParseError);

			/// destructor
			~SmartsMatcher();
			//@}

			/// assignment operator
			SmartsMatcher& operator = (const SmartsMatcher& matcher);

			/// sets an SSSR which is used instead of doing an ring perception
			void setSSSR(const std::vector<std::vector<Atom*> >& sssr);

			/// this function is used to cause the matcher to do an ring perception if needed (do not use the set SSSR any more)
			void unsetSSSR();

			/// test if the given SMARTS was valid (call after .match)
			bool isValid() const;
		
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
	
					/// holds the first mapped node/atom
					std::vector<std::pair<const SPNode*, const Atom*> > first_matches;
	
					/// adds the content of the given struct
					void add(const RecStruct_& rec_struct);
	
					/// adds the the ith part of the content of the given struct
					void add(const RecStruct_& rec_struct, Size i);

					/// deletes all contents
					void clear();
	
					/// assignment operator
					RecStruct_& operator = (const RecStruct_& rec_struct);

					// dump (for debugging)
					void dump(const String& name, Size depth_ = 0);
			};
			
			/// method for evaluation of ring edges, after the the smarts tree is matched to molcule
			bool evaluateRingEdges_(const HashSet<const Atom*>& matching, const HashMap<const SPNode*, const Atom*>& mapping, const String& smarts);
			
			/// method for the evaluation of a pseudo-tree
			void evaluate_(	RecStruct_& rs, SPNode* start_node, const Atom* start_atom);
	
			/// method for evaluating a node of a pseudo-tree
			bool evaluate_node_(RecStruct_& rs, SPNode* start_node, const Atom* start_atom);
	
			/// method for evaluating a edge of a pseudo-tree 
			bool evaluate_edge_(RecStruct_& rs, SPEdge* start_node, const Atom* start_atom, const Bond* start_bond);

			/// matches from the recurive part
			HashMap<SPNode*, std::vector<HashSet<const Atom*> > > rec_matches_;

			/// user SSSR set?
			bool has_user_sssr_;

			/// user sssr
			std::vector<std::vector<Atom*> > sssr_;

			// debug output depth
			Size depth_;
	};
  
} // namespace BALL

#endif // BALL_STRUCTURE_SMARTSMATCHER_H

// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_STRUCTURE_SMARTSMATCHER_H
#define BALL_STRUCTURE_SMARTSMATCHER_H

#ifndef BALL_STRUCTURE_SMARTSPARSER_H
# include <BALL/STRUCTURE/smartsParser.h>
#endif

#include <vector>
#include <set>
#include <map>

#include <boost/shared_ptr.hpp>

namespace BALL 
{
	// forward declaration
	class Molecule;

	/** @name	\brief SMARTS Matcher
	 
			This class implements a SMARTS matcher. Given a molecule and
			a SMARTS-string the matcher matches the string to the molecule
			and reports a vector of atom sets as a result. 

			To speedup the matching some other modes are possible. The search 
			of the matching procedure can be reduced to a set of start atoms, 
			which are used to start the matchings.

			Also if available and if needed by the SMARTS pattern, the SSSR
			can be set. The SSSR is the smallest set of smallest rings and
			are needed if the SMARTS pattern contains rings specific parts.
			The calculation of the SSSR is time-consuming and would be done 
			automatically by the smarts matcher. However, if the SSSR of the 
			molecule is already available, it can be set using the SSSR method.
			Warning, the SSSR should always be up-to-date, i.e. the SSSR of the 
			current molecule, otherwise the matcher might report wrong results.

			\ingroup StructureMatching
	*/
	class BALL_EXPORT SmartsMatcher
	{
		public: 

			/** @name Typedefs
			*/
			//@{
			typedef std::vector<std::set<const Atom*> > Match;
			//@}


			/**	@name Constructors and Destructors
			*/
			//@{
			/// default constructor
			SmartsMatcher();

			/// destructor
			virtual ~SmartsMatcher();
			//@}


			/** @name Accessors
			*/
			//@{
			/// method to match a Smarts pattern given as a string to given molecule
			void match(Match& matches, Molecule& mol, const String& smarts);

			/// method to match a Smarts pattern given as a string to given molecule. The atoms which will be used for starting matching are given in atoms
			void match(Match& matches, Molecule& mol, const String& smarts, const std::set<const Atom*>& start_atoms);

			/// method to match several Smarts patterns given as a vector of strings
			void match(std::vector<Match>& matches, Molecule& mol, const std::vector<String>& smarts);

			/// method to match several Smarts patterns given as a vector of string. The atoms used for start matchings are given in start_atoms
			void match(std::vector<Match>& matches, Molecule& mol, const std::vector<String>& smarts, const std::set<const Atom*>& start_atoms);

			/// sets an SSSR which is used instead of doing an ring perception
			void setSSSR(const std::vector<std::vector<Atom*> >& sssr);

			/// this function is used to cause the matcher to do an ring perception if needed (do not use the set SSSR any more)
			void unsetSSSR();
			//@}


		private:
			
			/// copy constructor
			SmartsMatcher(const SmartsMatcher& matcher);

			/** @name Operators
			*/
			//@{
			/// assignment operator
			SmartsMatcher& operator = (const SmartsMatcher& matcher);
			//@}


		protected:

			/** @name Typedefs
			*/
			//@{
			typedef SmartsParser::SPNode SPNode;
			typedef SmartsParser::SPEdge SPEdge;
			typedef SmartsParser::SPAtom SPAtom;
			typedef SmartsParser::SPBond SPBond;
			//@}

			/// core structure of the recursive matching algorithm for the object pool
			class RecStructCore_
			{
				public:
					
					/// default constructor
					RecStructCore_();

					/// copy constructor
					RecStructCore_(const RecStructCore_&);

					/// destructor
					virtual ~RecStructCore_();

					/// assignment operator 
					RecStructCore_& operator = (const RecStructCore_&);

					/// method that deletes all content from the containers
					void clear();
			
					/// container which contains a vector of matched atoms from different matches
					std::vector<std::set<const Atom*> > matched_atoms;

					/// container which contains a vector of mappings from different matches 
					std::vector<std::map<const SPNode*, const Atom*> > mapped_atoms;

					/// contains vector of visited atoms of different matches
					std::vector<std::set<const Atom*> > visited_atoms;

					/// contains vector of visited bonds of different matches
					std::vector<std::set<const Bond*> > visited_bonds;

					/// contains vector of visited edges of the Smarts tree of different matches
					std::vector<std::set<const SPEdge*> > visited_edges;

					/// contains the first matched atoms of different matches (needed for recursive Smarts)
					std::vector<std::pair<const SPNode*, const Atom*> > first_matches;
			};

			/// class which does the pool operations of the RecStructCore_ pool
			class RecStructPool_
			{
				public:
					
					/// default constructors
					RecStructPool_();

					/// destructor
					virtual ~RecStructPool_();

					/// returns the a free RecStructCore_ of the pool (creates new ones if needed)
					RecStructCore_* getNextFree();

					/// returns the position of the last RecStructCore_* from getNextFree()
					Position getLastPosition();

					/// frees the structure at position pos
					void destroy(Position pos);

				private:

					/// does the resize operation of the pool (creates new ones, but never release them!)
					void resize_();

					/// copy constructor (declared private as it does not make sense to use it)
					RecStructPool_(const RecStructPool_&);

					/// assignment operator (declared private as it does not make sense to use it)
					RecStructPool_& operator = (const RecStructPool_&);
	
					/// the pool of the RecStructCore_ structures
					std::vector<RecStructCore_*> rec_struct_pool_;

					/// the list of the free to use structures, represented as positions in the rec_struct_pool_ vector
					std::vector<Position> free_list_;

					/// the last position of returned structure from getNextFree()
					Position last_position_;
			};


			/// a wrapper class which is used as an interface in the matching code to the pool
			class RecStruct_
			{
				private:
					/// the underlaying core structure which contains the Containers used in this class
					RecStructCore_* rec_struct_core_;
			
				public:
					
					/// default constructor
					RecStruct_();

					/// copy constructor
					RecStruct_(const RecStruct_& rec_struct);

					/// destructor
					virtual ~RecStruct_();

					/// assignment operator 
					RecStruct_& operator = (const RecStruct_&); 

					/// reference to the matched atoms in the core structure
					std::vector<std::set<const Atom*> >& matched_atoms;

					/// reference to the mapped SPNodes to Atoms in the core structure
					std::vector<std::map<const SPNode*, const Atom*> >& mapped_atoms;

					/// reference to the visited atoms in the core structure
					std::vector<std::set<const Atom*> >& visited_atoms;

					/// reference to the visited bonds in the core structure
					std::vector<std::set<const Bond*> >& visited_bonds;

					/// reference to the visited edges in the core structure
					std::vector<std::set<const SPEdge*> >& visited_edges;

					/// reference to the first matches in the core structure
					std::vector<std::pair<const SPNode*, const Atom*> >& first_matches;

					/// adds the content of the given struct
					void add(const RecStruct_& rec_struct);

					/// adds the the ith part of the content of the given struct
					void add(const RecStruct_& rec_struct, Size i);

					/// deletes all contents
					void clear();

					/// dumps the contents (for debugging)
					void dump(const String& name, Size depth_ = 0);

				private:
				
					/// position of the RecStructCore_ in the Pool, used for destroy() method 
					Position pos_;
			};

			/// the pool of rec struct objects
			static boost::shared_ptr<RecStructPool_> pool_;

			/// method for evaluation of ring edges, after the the smarts tree is matched to molcule
			bool evaluateRingEdges_(const std::set<const Atom*>& matching, const std::map<const SPNode*, const Atom*>& mapping, const String& smarts);
			
			/// method for the evaluation of a pseudo-tree
			void evaluate_(	RecStruct_& rs, SPNode* start_node, const Atom* start_atom);
	
			/// method for evaluating a node of a pseudo-tree
			bool evaluate_node_(RecStruct_& rs, SPNode* start_node, const Atom* start_atom);
	
			/// method for evaluating a edge of a pseudo-tree 
			bool evaluate_edge_(RecStruct_& rs, SPEdge* start_node, const Atom* start_atom, const Bond* start_bond);

			/// matches from the recurive part
			std::map<SPNode*, std::vector<std::set<const Atom*> > > rec_matches_;

			/// user SSSR set?
			bool has_user_sssr_;

			/// user sssr
			std::vector<std::vector<Atom*> > sssr_;

			// debug output depth
			Size depth_;
	};
  
} // namespace BALL

#endif // BALL_STRUCTURE_SMARTSMATCHER_H


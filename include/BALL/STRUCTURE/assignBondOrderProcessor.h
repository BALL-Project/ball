#ifndef BALL_STRUCTURE_ASSIGNBONDORDERPROCESSOR_H
#define BALL_STRUCTURE_ASSIGNBONDORDERPROCESSOR_H

#ifndef BALL_CONCEPT_PROCESSOR_H
	#include <BALL/CONCEPT/processor.h>
#endif

#ifndef BALL_KERNEL_ATOMCONTAINER_H
	#include <BALL/KERNEL/atomContainer.h>
#endif

#ifndef BALL_DATATYPE_HASHMAP_H
	#include <BALL/DATATYPE/hashMap.h>
#endif

#ifndef BALL_DATATYPE_HASHSET_H
	#include <BALL/DATATYPE/hashSet.h>
#endif


#ifndef BALL_KERNEL_BOND_H
	#include <BALL/KERNEL/bond.h>
#endif

#ifndef BALL_DATATYPE_OPTIONS_H
	#include <BALL/DATATYPE/options.h>
#endif

#include <queue>

namespace BALL 
{

	/**	Bond creation processor
			\ingroup StructureMiscellaneous
	*/
	class BALL_EXPORT AssignBondOrderProcessor 
		: public UnaryProcessor<AtomContainer> 
	{
		protected:
			class Solution_;
			friend class Solution;
			class PQ_Entry_;
			friend class PQ_Entry_;

		public:

			/** @name Constant Definitions
			*/
			//@{
			/// Option names
			struct BALL_EXPORT Option
			{	
				/**	compute bond orders for all bonds of type single bond order
				*/
		 		static const char* OVERWRITE_SINGLE_BOND_ORDERS;	

				/**	compute bond orders for all bonds of type double bond order
				*/
				static const char* OVERWRITE_DOUBLE_BOND_ORDERS;	
				
				/**	compute bond orders for all bonds of type triple bond order
				*/
				static const char* OVERWRITE_TRIPLE_BOND_ORDERS;	
				
				/**	compute bond orders for all bonds of type quadruple bond order
				*/
				static const char* OVERWRITE_QUADRUPLE_BOND_ORDERS;	
				
				/**	compute bond orders for all bonds of type aromatic bond order
				*/
				static const char* OVERWRITE_AROMATIC_BOND_ORDERS; 	
				
				/**	overwrite all charges
				*/
				static const char* OVERWRITE_CHARGES;
				
				/**	assign all charges
				*/
				static const char* ASSIGN_CHARGES;
				
				/**	kekulize rings
				*/
				static const char* KEKULIZE_RINGS;

				/**	enforce octett rule
				*/
				static const char* ENFORCE_OCTETT_RULE;
				
				/**	technique to compute all solutions
				*/
				//static const char* COMPUTE_ALL_SOLUTIONS;
				static const char* ALGORITHM; 

				/** the penalty parameter file
				 */
				static const char* INIFile;	
				
				/** the maximal possible bond order
				 */
				static const char* MAX_BOND_ORDER;

				/** the weighting of bond length penalties wrt valence penalties
				 */
				static const char* BOND_LENGTH_WEIGHTING;

			};

			/// Default values for options
			struct BALL_EXPORT Default
			{
				static const bool OVERWRITE_SINGLE_BOND_ORDERS;
				static const bool OVERWRITE_DOUBLE_BOND_ORDERS;
				static const bool OVERWRITE_TRIPLE_BOND_ORDERS;
				static const bool OVERWRITE_QUADRUPLE_BOND_ORDERS;
				static const bool OVERWRITE_AROMATIC_BOND_ORDERS; 
				static const bool OVERWRITE_CHARGES;
				static const bool ASSIGN_CHARGES;
				static const bool KEKULIZE_RINGS;
				static const bool ENFORCE_OCTETT_RULE;	
				//static const String COMPUTE_ALL_SOLUTIONS;
				static const String ALGORITHM;
				static const String INIFile;
				static const int MAX_BOND_ORDER;
				static const float BOND_LENGTH_WEIGHTING;
			};

			struct BALL_EXPORT Algorithm//Algorithm::ComputeAllSolutions
			{
				//static const String DISABLED;
				//static const String ONE_BOND_HEURISTIC;
				//static const String ENUMERATION_TREE;
				static const String A_STAR;
				static const String ILP; 
			};
			//@}
		

			/** @name	Constructors and Destructors
			*/
			//@{

			BALL_CREATE(AssignBondOrderProcessor);
			
			///	default constructor
			AssignBondOrderProcessor();
		
			/// copy construcor
			AssignBondOrderProcessor(const AssignBondOrderProcessor& abop);
		
			/// constructor with parameter filename //TODO
			//AssignBondOrderProcessor(const String& file_name) throw(Exception::FileNotFound);
			
			/// destructor
			virtual ~AssignBondOrderProcessor();
			//@}

			/**	@name	Processor-related methods 
			*/
			//@{

			/// processor method which is called before the operator () call
			virtual bool start();

			/// operator () for the processor
			virtual Processor::Result operator () (AtomContainer& ac);

			/// processor method which is called after the operator () call
			virtual bool finish();

			//@}
		
			/**	@name	Accessors
			*/
			//@{
			/// Return the number of bonds built during the last application.
			Size getNumberOfBondOrdersSet();
		
			/// Returns the number of already computed solutions
			Size getNumberOfComputedSolutions() {return solutions_.size();};

			/// Returns the total penalty of the (already computed!) i-th solution //TODO atom vs bond penalty
			float getTotalPenalty(Position i) {return solutions_[i].atom_type_penalty;}
			
			/** Set the AtomContainer ac_'s bond orders to the ones found 
			 * in the (already computed!) i-th solution.
			 * Returns true if the i-th solution is valid 
			 * NOTE: start counting in 0
			 */
			bool apply(Position i);

			/* Computes and applies another solution
			*  Returns false if no further solution can be found.
			*/
			bool computeNextSolution();
			//@}
			
			/** @name Assignment
			*/
			//@{
			/// assignment operator
			AssignBondOrderProcessor& operator = (const AssignBondOrderProcessor& abop);
			//@}

			/** @name Public Attributes
			*/
			//@{
			/// options
			Options options;

			/** reset the options to default values
			*/
			void setDefaultOptions();
			//@}
		
			// for testing
			float evaluatePenalty(AtomContainer* ac);

		protected:
			
			/// Nested class storing the parameters of a solution to our ILP
			class Solution_
			{	
				friend class AssignBondOrderProcessor;

				public:
					/// Default constructor
					Solution_();
				
					// Detailed constructor for A-STAR
					Solution_(AssignBondOrderProcessor* ap, const PQ_Entry_& entry);

					/// Detailed constructor for ILP solutions
					// we assume that the AtomContainer in the constructor and the apply-method are equal
					// otherwise we would have to solve a graph matching problem
					// Note: The last parameter decides which order spectrum should be considered.
					// Since the lpsolve does not offer a unequality constraint we have to split into LE and GE
					Solution_(AssignBondOrderProcessor* ap, AtomContainer& ac, Bond* bond = NULL, int order = 0, bool lessEqualConstraint = false);
					
					/// Destructor
					virtual ~Solution_();
					
					/// 
					void clear();
				
					/// equality operator // TODO
					bool operator == (Solution_ b);

					/// denotes whether the problem could be solved or not  
					bool valid;
					
					/// the result : the complete set of bond orders for _ALL_ bonds
					HashMap<Bond*, int> bond_orders;

					/// the values of the objective function
					//float penalty;	
					float atom_type_penalty;
					float bond_length_penalty;
			};
			
			/// Nested class storing a priority queue entry for the A-STAR-Option
			class PQ_Entry_
			{	
				friend class AssignBondOrderProcessor;

				public:
				
					/// Default constructor
					PQ_Entry_(float alpha = 0.);
								
					/// Copy constructor
					PQ_Entry_(const PQ_Entry_& entry);

					/// Destructor
					virtual ~PQ_Entry_();
					
					/// 
					void clear();
					
					/** the less operator
					 *  note: we want a reverse sort, hence we actually return a "greater"
					 */
					bool operator < (const PQ_Entry_& b) const;  
					
					float coarsePenalty() const {
					//	cout << "al:" << alpha_ << " ap:" <<  estimated_atom_type_penalty << " bp:" << estimated_bond_length_penalty << " tot:" << (1.-alpha_) * estimated_atom_type_penalty + (alpha_* estimated_bond_length_penalty) << endl;
						
						
						return ((1.-alpha_) * estimated_atom_type_penalty + (alpha_* estimated_bond_length_penalty));}
					float finePenalty() const {return estimated_bond_length_penalty;}

					/// the estimated atom type penalty
					float estimated_atom_type_penalty;   //estimated_f
					/// the estimated bond length penalty
					float estimated_bond_length_penalty;

					/// the bond orders 
					/// the i-th entry denotes the bondorder of the i-th bond
					/// unset bonds get the order 0
					vector<int> bond_orders;
					
					/// the last considered bond
					Position last_bond;

					protected:
						float alpha_;
				};

			/// computes for every atom its possible atomic valences and the corresponding possible atomic penalty scores
		  /// and stores them per \b{atom in atomic_penalty_scores_}
			void calculateAtomPenalties_(AtomContainer& ac);

			/** Reads the penalty-INIFile (for example BondOrder.ini),
			 *  and stores the penalties blockwise. 
			 *	Returns false if the INIFile could not be read correctly.
			 */
			bool readAtomPenalties_()	throw(Exception::FileNotFound());
			
			/** Assigns to every atom of the AtomContainer to which the
			 *  processor is applied to a block of possible valences 
			 *  and the corresponding penalties.	
			 *  Returns false if the AtomContainer to which the processor 
			 *  is applied to has an atom with no matching penalty block. 
			 */
			bool preassignPenaltyClasses_();
	
			/** Precomputes for every bond of the AtomContainer to which the 
			 *	processor is applied to the possible bond length penalties
			 *	resulting from deviation of the actual bond length to 
			 *	a standart length for bonds with same atom types and the 
			 *	chosen bond order. bond_lengths_penalties_
			 *	If there is no information for certain atom pair, penalty 0. 
			 *	is assumed. For incomplete information, we assign the
			 *	the compare to the longest known bond for this atom pair. 
			 */
			bool precomputeBondLengthPenalties_();
			
			/// Processor is in a useable valid state. //TODO
			bool valid_;

			//TODO: change to something better than the atom index :-) 
			/// the penalties per atom 
			vector<vector< pair <int,int> > > atomic_penalty_scores_;
		
			// Map for storing whether a bond is free or fixed
			map<Bond*, bool> bond_free_;
 			
			// Map for storing the bonds fixed orders
			// if a bond is free, the map returns 0
			// TODO: remove bond_free_
			map<Bond*, int> bond_fixed_;

			/// Map for storing the bonds associated index (all bonds)
			HashMap<Bond*, Index> bond_to_index_;
			
			// TODO: constructor... Ersetzung
			// Vector for mapping from variable indices onto bonds (all bonds)
			std::vector<Bond*> index_to_bond_;

			//TODO
			/// the number of bonds given (free + fixed!)
			Position total_num_of_bonds_; 

			// TODO
			/// num of free bonds 
			int num_of_free_bonds_;

			// stores the number of bonds
			//Position num_of_bonds_; // Position total_no_bonds = ac.countBonds();

			// not necessary because of getIndex()
			//HashMap<Atom*, int> atom_to_index_;

			/// store for all atom-indices the atoms fixed valences 
			std::vector<Position> fixed_val_;

			// storing the solutions
			vector<Solution_> solutions_;
			
			/// the optimal penalty // TODO: Konsturktor, getMehtod... filled correctly in all applications?
			int optimal_penalty_; // TODO atomtype vs bond length penalty
		
		
			/// TODO: Konstruktor.... apply-methods
			// denotes the index of the last applied solution
			// -1 if there was no valid solution applied
			Position last_applied_solution_;
			
			//TODO: ac-->ac_
			/// the AtomContainer, the processor is operating on
			AtomContainer* ac_;

			////////// for Algorithm::ComputeAllSolutions::ENUMERATION_TREE ///////
			
			void recursive_solve_(AtomContainer& ac, int depth);
			void setChecked_(String orders);

			HashSet<String> checked_; //TODO: constr ...
			String current_orders_; // TODO: constr ...

			////////// for Algorithm::A_START   ComputeAllSolutions::A_STAR ///////
			/// 
			bool	performAStarStep_();

			/// the priority queue //TODO constr...
			priority_queue<PQ_Entry_> queue_;
			
			/// Method to estimate the f = g* +h*
			/// returns true, if the entry is still valid
			bool estimatePenalty_(PQ_Entry_& entry);

			/// Method to combine the atom type and bond length penalty
			/// the balance parameter is alpha_
			//float combinedAtomAndBondPenalty_(PQ_Entry_& entry);

			// filled by readAtomPenalties_
			// organized in imaginarey blocks of length  
			// block_to_length_[i], starting from 
			// block_to_start_idx_[i] associating 
			// block_to_start_valence_[i] to the start_idx
			vector<int> penalties_;
			vector<Position> block_to_start_idx_;
			vector<Size> block_to_length_;
			vector<int> block_to_start_valence_;
			
			// TODO
			// stores the defining element and the SMART-string of each block
			vector<pair<String, String> > block_definition_;

			// stores which atom belongs to which block
			vector<int> atom_to_block_;
					
			///stores the possible bond lengths penalties per order // TODO: constructor etc
			HashMap<Bond*, vector<float> > bond_lengths_penalties_;
	
			/// max bond order to consider
			int max_bond_order_;

			/// balance parameter between atom type and bond length penalty
			float alpha_; 
	};

} // namespace BALL 


#endif // BALL_STRUCTURE_ASSIGNBONDORDERPROCESSOR_H

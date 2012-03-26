// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

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
# include <BALL/DATATYPE/options.h>
#endif

#ifndef BALL_COMMON_EXCEPTION_H
# include <BALL/COMMON/exception.h>
#endif

#ifndef BALL_SYSTEM_TIMER_H
# include <BALL/SYSTEM/timer.h>
#endif

#ifndef BALL_STRUCTURE_BONDORDERS_BONDORDERASSIGNMENTSTRATEGY_H
# include <BALL/STRUCTURE/BONDORDERS/bondOrderAssignmentStrategy.h>
#endif

#ifndef BALL_STRUCTURE_BONDORDERS_BONDORDERASSIGNMENT_H
# include <BALL/STRUCTURE/BONDORDERS/bondOrderAssignment.h>
#endif

#ifndef BALL_STRUCTURE_BONDORDERS_PARTIALBONDORDERASSIGNMENT_H
# include <BALL/STRUCTURE/BONDORDERS/partialBondOrderAssignment.h>
#endif

#include <map>
#include <vector>

namespace BALL
{
	/** \brief Assignment of bond orders from topology information.
	 *  
	 *  Called with default options the processor computes up to 
	 *  \link Default::MAX_NUMBER_OF_SOLUTIONS Default::MAX_NUMBER_OF_SOLUTIONS\endlink 
	 *  many possible bond orders with optimal value and applies the first solution found 
	 *  to the given AtomContainer.
	 *  \par
	 *
	 *  All further optimal solutions can be applied by calling
	 *  the method \link apply()  apply() \endlink. 
	 *  Additional solutions can be computed by calling the method 
	 *  \link computeNextSolution()  computeNextSolution()\endlink (except when 
	 *  using the FPT strategy which currently does not support this behaviour).
	 *  \par
	 *  <br>
	 *  Example code: <br> 
	 *  \code
 	 *  	AssignBondOrderProcessor bop;	
	 *  	bop.options.setBool(AssignBondOrderProcessor::Option::COMPUTE_ALSO_NON_OPTIMAL_SOLUTIONS, true);
	 *  	...
	 *   	sys.apply(bop);
	 *   	i = bop.getNumberOfComputedSolutions();
	 *  	bop.apply(i-1);
	 *  	... 
	 *  	while (bop.computeNextSolution())
	 *   	{
	 *    	  i++;
	 *      	bop.apply(i);
	 *  	}
	 *  \endcode
	 */
	class BALL_EXPORT AssignBondOrderProcessor
		: public UnaryProcessor<AtomContainer>
	{
		protected:
			friend class PartialBondOrderAssignment;
			friend class BondOrderAssignment;

			class PQ_Entry_;
			friend class PQ_Entry_;

			friend class BondOrderAssignmentStrategy;

			friend class AStarBondOrderStrategy;
			friend class BranchAndBoundBondOrderStrategy;
			friend class FPTBondOrderStrategy;
			friend class ILPBondOrderStrategy;
			friend class KGreedyBondOrderStrategy;

		public:

			/** @name Constant Definitions
			*/
			//@{
			/// Option names
			struct BALL_EXPORT Option
			{
				/**	compute bond orders for all bonds of type single bond order.
				 *  @see OVERWRITE_SELECTED_BONDS 
				*/
			  static const char* OVERWRITE_SINGLE_BOND_ORDERS;

				/**	compute bond orders for all bonds of type double bond order.
				 *
				 *  @see OVERWRITE_SELECTED_BONDS 
				*/
				static const char* OVERWRITE_DOUBLE_BOND_ORDERS;

				/**	compute bond orders for all bonds of type triple bond order.
				 *
				 *  @see OVERWRITE_SELECTED_BONDS 
				*/
				static const char* OVERWRITE_TRIPLE_BOND_ORDERS;

				/**	compute bond orders for all selected bonds
				 
				   <b>NOTE:</b> This option has higher priority than 
				   the OVERWRITE_BOND_ORDER-Options.

				   @see Option::OVERWRITE_SINGLE_BOND_ORDERS
				   @see Option::OVERWRITE_DOUBLE_BOND_ORDERS
				   @see Option::OVERWRITE_TRIPLE_BOND_ORDERS
				*/
				static const char* OVERWRITE_SELECTED_BONDS;

				/**	add hydrogens based on free valences

				   <b>NOTE:</b> This option is still experimental!
				*/
				static const char* ADD_HYDROGENS;             //TODO

				/**  compute also the connectivity of the molecule 
				 
				    <b>NOTE:</b> This option is still experimental!
				 */
				static const char* COMPUTE_ALSO_CONNECTIVITY; //TODO

				/**  the connectivity cut off
				 *
				 *   <b>NOTE:</b> This option is still experimental!
				 */
				static const char* CONNECTIVITY_CUTOFF;       //TODO 

				/**	additionally use structural information
				*/
				static const char* USE_FINE_PENALTY;

				/**	kekulize rings
				*/
				static const char* KEKULIZE_RINGS;

				/**	the strategy used
				*/
				static const char* ALGORITHM;

				/** the penalty parameter file
				 */
				static const char* INIFile;

				/** the maximal possible bond order
				 */
				static const char* MAX_BOND_ORDER;

				/** the maximal number of solutions to compute
				 *
				 *  If set to zero all optimal or all up to \link MAX_PENALTY MAX_PENALTY \endlink 
				 *  solutions will be computed.
				 *
				 *  @see Option::COMPUTE_ALSO_NON_OPTIMAL_SOLUTIONS 
				 *  @see Option::MAX_PENALTY
				 */
				static const char* MAX_NUMBER_OF_SOLUTIONS;

				/** the maximal penalty score allowed
				 *  
				 *  This option respects option \link MAX_NUMBER_OF_SOLUTIONS MAX_NUMBER_OF_SOLUTIONS \endlink
				 *  if specified.
				 *
				 *  If set to -1 this option will be ignored. 
				 *
				 *  @see Option::COMPUTE_ALSO_NON_OPTIMAL_SOLUTIONS
				 *  @see Option::MAX_NUMBER_OF_SOLUTIONS
				 */
				static const char* MAX_PENALTY;

				/** compute also sub-optimal solutions but not more than 
				 *  \link MAX_NUMBER_OF_SOLUTIONS MAX_NUMBER_OF_SOLUTIONS \endlink solutions.
				 *  
				 *  Alternatively \link Option::MAX_PENALTY  Option::MAX_PENALTY  \endlink
				 *  allows to specify a maximal penalty. 
				 *
				 *  @see Option::MAX_NUMBER_OF_SOLUTIONS 
				 *  @see Option::MAX_PENALTY
				 */
				static const char* COMPUTE_ALSO_NON_OPTIMAL_SOLUTIONS;

				/** weighting of bond length penalties wrt valence penalties.
				 *  If set to zero, the valence penalty will not be normalized. 
				 *  
				 *   <b>NOTE:</b> This option is still experimental.
				 */
				static const char* BOND_LENGTH_WEIGHTING;

				/** apply the first solution directly.
				 *
				 *  Default is false.
				 */
				static const char* APPLY_FIRST_SOLUTION;

			};

			/// Default values for options
			struct BALL_EXPORT Default
			{
				static const bool OVERWRITE_SINGLE_BOND_ORDERS;
				static const bool OVERWRITE_DOUBLE_BOND_ORDERS;
				static const bool OVERWRITE_TRIPLE_BOND_ORDERS;
				static const bool OVERWRITE_SELECTED_BONDS;
				static const bool ADD_HYDROGENS;
				static const bool COMPUTE_ALSO_CONNECTIVITY;
				static const float CONNECTIVITY_CUTOFF;
				static const bool USE_FINE_PENALTY;
				static const bool KEKULIZE_RINGS;
				static const String ALGORITHM;
				static const String INIFile;
				static const int MAX_BOND_ORDER;
				static const int MAX_NUMBER_OF_SOLUTIONS;
				static const int MAX_PENALTY;
				static const bool COMPUTE_ALSO_NON_OPTIMAL_SOLUTIONS;
				static const float BOND_LENGTH_WEIGHTING;
				static const bool APPLY_FIRST_SOLUTION;
			};

			struct BALL_EXPORT Algorithm
			{
				/** Solves the problem using an A* formulation.
				 * 
				 */
				static const String A_STAR;

				/** Solves the problem using an ILP.
				 *
				 *  \par
				 *   <b>NOTE:</b> This option requires an ILP solver and currently 
				 *   cannot be combined with the options 
				 *   	\link Option::USE_FINE_PENALTY Option::USE_FINE_PENALTY \endlink, 
				 *   	\link Option::BOND_LENGTH_WEIGHTING Option::BOND_LENGTH_WEIGHTING\endlink, 
				 *   	\link Option::ADD_HYDROGENS Option::ADD_HYDROGENS\endlink, 
				 *   	or \link Option::COMPUTE_ALSO_CONNECTIVITY  Option::COMPUTE_ALSO_CONNECTIVITY\endlink .
				 */
				static const String ILP;

				/** Solves the problem using an FPT algorithm.
				 *
				 *  \par 
				 *  <b>NOTE:</b> 
				 *   This algorithm does not support the method computeNextSolution() by design.
				 *	 Instead, you can use the options 
				 *	  \link Option:MAX_NUMBER_OF_SOLUTIONS Option:MAX_NUMBER_OF_SOLUTIONS \endlink and
				 *	 	\link Option::COMPUTE_ALSO_NON_OPTIMAL_SOLUTIONS \endlink 
				 *	 to create an ensemble of solutions.
				 *
				 *   Furthermore, this option currently cannot be combined with the options:
				 *   	\link Option::USE_FINE_PENALTY Option::USE_FINE_PENALTY  \endlink,
				 *   	\link Option::BOND_LENGTH_WEIGHTING Option::BOND_LENGTH_WEIGHTING \endlink, 
				 *   	\link Option::ADD_HYDROGENS Option::ADD_HYDROGENS  \endlink, 
				 *   	\link Option::COMPUTE_ALSO_CONNECTIVITY Option::COMPUTE_ALSO_CONNECTIVITY\endlink,  
				 *   	\link Option::OVERWRITE_SELECTED_BONDS Option::OVERWRITE_SELECTED_BONDS\endlink,  and
				 *   	the special hack Option::MAX_NUMBER_OF_SOLUTIONS==0 to enumerate all optimal solutions. 
				 *
				 *   	@see Option::USE_FINE_PENALTY, 
				 *   	@see Option::BOND_LENGTH_WEIGHTING, 
				 *   	@see Option::ADD_HYDROGENS, 
				 *   	@see Option::COMPUTE_ALSO_CONNECTIVITY, 
				 *   	@see Option::OVERWRITE_SELECTED_BONDS
				 *   	@see Option::MAX_NUMBER_OF_SOLUTIONS
				 */
				static const String FPT;

				static const String K_GREEDY;
				static const String BRANCH_AND_BOUND;
			};

			//@}

			/** @name	Constructors and Destructors
			*/
			//@{

			///	Default Constructor
			AssignBondOrderProcessor();

			// constructor with parameter filename //TODO
			//AssignBondOrderProcessor(const String& file_name) throw(Exception::FileNotFound);

			/// Destructor
			virtual ~AssignBondOrderProcessor();
			//@}

			/**	@name	Processor-related methods 
			*/
			//@{

			/// Processor method which is called before the operator()-call.
			virtual bool start();

			/** Clears the data structures.
			 *
			 * 	<b>NOTE:</b> The options remain!
			 * 	Use \link setDefaultOptions() setDefaultOptions()\endlink to clear the options.
			 */
			void clear();

			/** Operator () for the processor 
			 *
			 * Called with %Default-options the processor computes all 
			 * possible bond order assignments with optimal atomic penalty value and 
			 * applies the first solution to the given AtomContainer.
			 * \par
			 *
			 * 	<b>NOTE:</b> Having used the \link Algorithm::A_STAR Algorithm::A_STAR\endlink-option (default)
			 * 	the method \link getNumberOfComputedSolutions() getNumberOfComputedSolutions()\endlink
			 * 	will return the  number of optimal solutions+1!
			 *
			 * 	@param  ac  the AtomContainer to which the processor is applied.
			 */
			virtual Processor::Result operator ()(AtomContainer& ac);

			/// Processor method which is called after the operator()-call.
			virtual bool finish();

			//@}

			/**	@name	Accessors
			*/
			//@{

			/** Returns the number of added hydrogens in solution i.
			 *
			 *	<b>NOTE:</b> Hydrogens can only be added using the
			 *	\link  Option::ADD_HYDROGENS  Option::ADD_HYDROGENS\endlink-option.
			 *
			 *  @return Size - number of hydrogens added in assignment solution i.
			 *  @see Option::ADD_HYDROGENS
			 */
			Size getNumberOfAddedHydrogens(Position i)
			{
				if (i >= solutions_.size())
				{
					Log.error() << "AssignBondOrderProcessor: No solution with index " << i << std::endl;
					return 0;
				}
				int num_hydrogens = 0;

				HashMap<Atom*, int>::Iterator it = solutions_[i].number_of_virtual_hydrogens.begin();
				for (; it != solutions_[i].number_of_virtual_hydrogens.end(); it++)
					num_hydrogens += it->second;
				return num_hydrogens;
			}

			/** Returns the number of already computed solutions.
			 *
			 *  <b>NOTE:</b> Having applied the operator with option \link Option::Algorithm::A_STAR Option Algorithm::A_STAR \endlink
			 * 			  this method returns the number of optimal solutions+1!
			 * 	
			 * 	@return Size - number of already computed solutions. 
			 *  @see Option::COMPUTE_ALSO_NON_OPTIMAL_SOLUTIONS
			 *  @see Option::MAX_NUMBER_OF_SOLUTIONS
			 */
			Size getNumberOfComputedSolutions() {return solutions_.size();}


			/** Returns a pointer to the original Molecule as AtomContainer.
			 */
			AtomContainer* getAtomContainer() {return ac_;}

			/** Returns a nonmutable pointer to the original Molecule as AtomContainer.
			 */
			AtomContainer const* getAtomContainer() const {return ac_;}

			/** Returns a reference to the original system to which solution i was applied.
			 *   
			 *   <b>NOTE:</b> This method has the same effect as calling %apply(i)!
			 *
			 *  @param  i index of the solution, whose bond order assignment should be applied. 
			 * 	@return const System& - the original system with bond order assignment of solution i. 
			 * 					If i is invalid, an Exception is thrown.
			 */
			const System& getSolution(Position i) throw(Exception::IndexOverflow);

			/** Returns the total charge of solution i. 
			 
			 		<b>NOTE:</b> This method is still experimental.

			 	 	@param   i index of the solution, whose charge should be computed. 
				 	@return  float - total charge of solution i.  
			 */
			float getTotalCharge(Position i)
			{
				if (i >= solutions_.size())
				{
					Log.error() << "AssignBondOrderProcessor: No solution with index " << i << std::endl;

					return std::numeric_limits<float>::max();
				}
				else
				{
					return getTotalCharge_(solutions_[i]);
				}
			}

			/** Returns the total penalty of solution i.
			 *
			 * 	@param   i  	 index of the solution, whose penalty should be returned. 
			 * 	@return  float -  total penalty of solution i.  
			 */
			float getTotalPenalty(Position i=0)
			{
				if (i >= solutions_.size())
				{
					Log.error() << "AssignBondOrderProcessor: No solution with index " << i << std::endl;

					return std::numeric_limits<float>::max();
				}
				else
					return getTotalPenalty_(solutions_[i]);
			}

			/* Returns the number of node expansions before solution i was found.
			 *
			 * param    i  index of the solution, whose number of node expansions should be returned.
			 * return  int -   number of node expansions before solution i was found.   
			 */
			int getNumberOfNodeExpansions(Position i)
			{
				if (i >= solutions_.size())
				{
					Log.error() << "AssignBondOrderProcessor: No solution with index " << i << std::endl;

					return -1;
				}
				else
					return getNumberOfNodeExpansions_(solutions_[i]);
			}

			/* Returns the number of node expansions before solution i was found.
			 *
			 * param    i  index of the solution, whose  queue size should be returned. 
			 * return  int -  queue size when solution i was found.  
			 */
			int getQueueSize(Position i)
			{
				if (i >= solutions_.size())
				{
					Log.error() << "AssignBondOrderProcessor: No solution with index " << i << std::endl;

					return -1;
				}
				else
					return getQueueSize_(solutions_[i]);
			}

			/** Applies the i-th precomputed bond order assignment.
			 *
			 * Sets the AtomContainer's bond orders to the ones found 
			 * in the (already computed!) i-th solution, start counting at 0!
			 * \par
			 * <b>NOTE:</b> All virtual hydrogens added to the processed AtomContainer
			 * 			 by a previous call of apply will be deleted by the current
			 * 			 call!
			 *
			 *  @param    i  index of the solution whose bond orders should be assigned. 
			 *	@return bool - true if the i-th solution is valid, false otherwise.
			 */
			bool apply(Position i);

			/** Resets all bond orders and assigned hydrogens.
			 *  
			 *  Assigns the original bond order assignments to the AtomContainer 
			 *  we are operating on.
			 */
			void resetBondOrders();

			/** Computes and applies one of the next best solutions.
			 *
			 *  Ignores the options  \link MAX_NUMBER_OF_SOLUTIONS MAX_NUMBER_OF_SOLUTIONS \endlink and
			 *											 \link COMPUTE_ALSO_NON_OPTIMAL_SOLUTIONS COMPUTE_ALSO_NON_OPTIMAL_SOLUTIONS\endlink .
			 *
			 *  @return bool - false if no further solution can be found.
			 */
			bool computeNextSolution(bool apply_solution = true);

			/** Resets the options to default values.
			*/
			void setDefaultOptions();

			/** Checks the options.
			*/
			bool hasValidOptions(){return readOptions_();}

			/** Evaluates the AtomContainer's bond orders as specified in 
			 *  the Options and returns the computed penalty.
			 *
			 *  @param 	 ac	AtomContainer, whose bond orders should be evaluated.
			 *  @return  float - computed penalty, -1 if current assignment is not valid or includes aromatic bonds.
			 */
			float evaluatePenalty(AtomContainer* ac);
			//@}

			/** @name Public Attributes
			*/
			//@{
			/// the processor's options
			Options options;

			//@}

		protected:

			/** Reads, checks and stores the options. 	
			 *
			 * @return bool - false if one of the options got an invalid value.
			 * @return bool - true otherwise
			 */
			bool readOptions_();


			/** Reads and stores the penalty-INIFile (for example BondOrder.ini).
			 *
			 *	@return bool - false if the INIFile could not be read correctly.
			 */
			bool readAtomPenalties_() throw(Exception::FileNotFound());

			/** Assigns every atom of the AtomContainer to which the
			 *  processor is applied to a block of possible valences 
			 *  and the corresponding penalties.	
			 *  
			 *  @retval bool - false if the AtomContainer to which the processor 
			 *  is applied to has an atom with no matching penalty block. 
			 *	@retval bool - true otherwise
			 */
			bool preassignPenaltyClasses_();

			/**
			 * Finds the first matching SMARTS-expression in the penalty-vector
			 * and returns its index.
			 *
			 * @retval int  -1 if there is no matching expression
			 */
			int getPenaltyClass_(Atom* atom);


			/** Precomputes for every bond of the AtomContainer, to which the 
			 *	processor is applied to, the possible bond length penalties
			 *	resulting from deviation of the actual bond length to 
			 *	a standard length for bonds with same atom types and the 
			 *	chosen bond order. 
			 *	\par
			 *	If there is no information for certain atom pairs, penalty 0
			 *	is assumed.
			 *	In case of incomplete information, we assume the missing bond 
			 *	orders to be really unlikely and we set a penalty 
			 *	to 2* max_deviation_found (for this bond).
			 *
			 *	@retval bool - false if the AtomContainer is invalid or the processor is in an invalid state
			 *	@retval bool - true otherwise
			 */
			bool precomputeBondLengthPenalties_();

			/** Adds missing hydrogens as virtual hydrogens to the 
			 *  given atom, determines the possible penalty blocks, and 
			 *  returns the maximal possible atom type penalty.  
			 *
			 *  "virtual" means that NO  
			 *  atoms and bonds are added to the original AtomContainer. 
			 *	
			 *	@param  atom the atom, to which the virtual hydrogens should be added.
			 *  @return float - the max possible penalty the atom can get, if hydrogen(s) are added. 
			 *  @see Option::ADD_HYDROGENS
			 */
			float computeVirtualHydrogens_(Atom* atom);

			/** Applies the given solution.
			 */
			//TODO: move to solution!
			bool apply_(BondOrderAssignment& solution);

			/** Stores the original configuration of the AtomContainer.
			 */
			void storeOriginalConfiguration_();

			/* Returns the queue's size at the moment the given solution was found.
			 *
			 * param   sol  solution, whose queue size should be returned. 
			 * return  int -  queue size when the given solution was found.  
			 */
			int getQueueSize_(const BondOrderAssignment& sol){return sol.getQueueSize();}

			/** Returns the total charge of a solution.
			 * 
			 * @param  sol solution, whose charge should be computed. 
			 * @return float -  total charge of the given solution.  
			*/

			float getTotalCharge_(const BondOrderAssignment& sol)
			{
				if (sol.valid)
				{
					return sol.total_charge;
				}
				else
				{
					return 0;
				}
			}

			/** Returns the total penalty of the given solution.
			 *
			 * @param   sol  solution, whose penalty should be returned. 
			 * @return  float -  total penalty of solution i.  
			 * @see  Option::BOND_LENGTH_WEIGHTING;
			 */
			float getTotalPenalty_(const BondOrderAssignment& sol)
			{
				return sol.coarsePenalty();
			}

			/* Returns the number of node expansions before the given solution was found.
			 *
			 * param   sol  solution, whose number of node expansions should be returned. 
			 * return  int -  number of node expansions before solution i was found.  
			 */
			int getNumberOfNodeExpansions_(const BondOrderAssignment& sol){return sol.getNumberOfNodeExpansions();}

			/// Processor is in a useable valid state. 
			bool valid_;

			/// Processor is in an evaluation mode. Default is false
			bool evaluation_mode_;

			// Map for storing the bonds fixed orders
			// if a bond is free, the map returns 0
			std::map<Bond*, short> bond_fixed_;

			// all free bonds in the atom container
			std::vector<Bond*> free_bonds_;

			// Map for storing the bonds associated index (all bonds)
			HashMap<Bond*, Index> bond_to_index_;

			// Vector for mapping from variable indices onto bonds (all bonds)
			std::vector<Bond*> index_to_bond_;



			// ***************** datastructures for virtual hydrogen bonds ****************** 
			//
			// 	NOTE: a single virtual bond represents ALL possible hydrogen 
			// 				bonds for a given atom
			//
			// the atoms with upto n possible additional hydrogens
			HashMap<Atom*, int> number_of_virtual_hydrogens_;
			//
			// the max number of virtual hydrogens per virtual bond index
			std::vector<int> virtual_bond_index_to_number_of_virtual_hydrogens_;
			//	
			// the number of virtual bonds
			Size num_of_virtual_bonds_;
			//
			// the virtual bond index assigned to this atom!
			vector<Atom*> virtual_bond_index_to_atom_;
			HashMap<Atom*, int> atom_to_virtual_bond_index_;
			//
			//
			// a virtual dummy bond
			Bond* virtual_bond_;

			// ******************* general datastructures *********************

			// the number of bonds given (free + fixed!)
			Position total_num_of_bonds_;

			// num of free bonds without virtual bonds!
			int num_of_free_bonds_;

			// store for all atom-indices the atoms fixed valences 
			std::vector<Position> fixed_val_;

			// storing the solutions
			vector<BondOrderAssignment> solutions_;

			// the original conformation before we computed anything
			// this is a vector because we can have multiple molecules...
			vector<BondOrderAssignment> starting_configuration_;

			// the inverse of the atom type penalty normalization factor
			float atom_type_normalization_factor_;

			// the inverse of the bond length penalty normalization factor
			float bond_length_normalization_factor_;

			// denotes the index of the last applied solution
			// -1 if there was no valid solution applied
			int last_applied_solution_;

			// the AtomContainer, the processor is operating on
			AtomContainer* ac_;

			// max bond order to consider
			int max_bond_order_;

			// balance parameter between atom type and bond length penalty
			float alpha_;

			// the max number of solutions to compute 
			int max_number_of_solutions_;

			// the max penalty score 
			int max_penalty_;

			// flag to indicate, whether also non-optimal solutions should be computed 
			bool compute_also_non_optimal_solutions_;

			// flag for adding missing hydrogens
			bool add_missing_hydrogens_;

			// flag for computing also the bond connectivity
			bool compute_also_connectivity_;

			// flag for using fine penalties derived from 3d information
			bool use_fine_penalty_;

			// ////////              general stuff                      /////////


			// The penalty administration datastructures.
			//  filled by readAtomPenalties_
			//  organized in imaginarey blocks of length  
			//  block_to_length_[i], starting from 
			//  block_to_start_idx_[i] associating 
			//  block_to_start_valence_[i] to the start_idx
			vector<int> penalties_;
			vector<Position> block_to_start_idx_;
			vector<Size> block_to_length_;
			vector<int> block_to_start_valence_;
			// stores the defining element and the SMART-string of each block
			vector<std::pair<String, String> > block_definition_;


			// Stores which atom belongs to which penalty block.
			// The first vector element of each atom block denotes the penalty block 
			// assigned to the atom without any additional VIRTUAL Hydrogens,
			// the second element with one additional Hydrogen and so on. 
			vector< vector<int> > atom_to_block_;

			// Stores the possible bond lengths penalties per order.
			HashMap<Bond*, vector<float> > bond_lengths_penalties_;

			Timer timer_;

			AssignBondOrderProcessor(const AssignBondOrderProcessor& abop);
			AssignBondOrderProcessor& operator = (const AssignBondOrderProcessor& abop);

			// The strategies this class can use
			StringHashMap<boost::shared_ptr<BondOrderAssignmentStrategy> > strategies_;
		};

} // namespace BALL 


#endif // BALL_STRUCTURE_ASSIGNBONDORDERPROCESSOR_H

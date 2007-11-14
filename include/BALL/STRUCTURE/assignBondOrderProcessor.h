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

#ifndef BALL_KERNEL_BOND_H
	#include <BALL/KERNEL/bond.h>
#endif

#ifndef BALL_DATATYPE_OPTIONS_H
	#include <BALL/DATATYPE/options.h>
#endif

namespace BALL 
{

	/**	Bond creation processor
			\ingroup StructureMiscellaneous
	*/
	class BALL_EXPORT AssignBondOrderProcessor 
		: public UnaryProcessor<AtomContainer> 
	{

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
		
			/// constructor with parameter filename
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
			
		protected:
	
			/// computes for every atom its possible atomic valences and the corresponding possible atomic penalty scores
		  /// and stores them per \b{atom in atomic_penalty_scores_}
			void calculateAtomPenalties_(AtomContainer& ac);

			/// number of bonds, which are created during the processor call
			Size num_bonds_;
			
			//TODO: change to something better than the atom index :-) 
			/// the penalties per atom 
			vector<vector< pair <int,int> > > atomic_penalty_scores_;
			
			// hashmap storing the considered bonds

		};

} // namespace BALL 


#endif // BALL_STRUCTURE_ASSIGNBONDORDERPROCESSOR_H

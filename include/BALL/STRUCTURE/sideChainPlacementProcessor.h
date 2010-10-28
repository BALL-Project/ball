#ifndef BALL_STRUCTURE_SIDECHAINPLACEMENTPROCESSOR_H
#define BALL_STRUCTURE_SIDECHAINPLACEMENTPROCESSOR_H

#ifndef BALL_CONCEPT_PROCESSOR_H
	#include <BALL/CONCEPT/processor.h>
#endif

#ifndef BALL_KERNEL_ATOMCONTAINER_H
	#include <BALL/KERNEL/atomContainer.h>
#endif

#ifndef BALL_DATATYPE_OPTIONS_H
# include <BALL/DATATYPE/options.h>
#endif

namespace BALL 
{
	/**	Side Chain Placement Processor
			\ingroup StructureMiscellaneous
	*/
	
	/** Placement of side chains.
	 * 
	 *  Given the path to the SCWRL binary in the option Option::SCWRL_BINARY_PATH;
	 *  the processor computes side chain conformations for the given side chains. 
	 *  If no selection is given all side chains are considered.
	 *  The option Option::MUTATE_SELECTED_SIDE_CHAINS can be used to mutate selected
	 *  amino acids as specified in the member mutated_sequence_.
	 *
	 *  <br> 
	 *  Example code: <br> 	
	 *  \code
	 *
	    	SideChainPlacementProcessor scpp;
				scpp.options.set(SideChainPlacementProcessor::Option::MUTATE_SELECTED_SIDE_CHAINS, true);

				System sys;	
				PDBFile mol("myfile.pdb"); 
				mol >> sys;
				String mutated_seq = "arCdcCeg";
				scpp.setMutations(mutated_seq);
				sys.apply(scpp); 

 	    \endcode
	 */ 
	class BALL_EXPORT SideChainPlacementProcessor 
		: public UnaryProcessor<AtomContainer> 
	{
		public:

			/** @name Constant Definitions
			*/
			//@{
			/// Option names
			struct BALL_EXPORT Option
			{	
				/**	technique to compute the side chain placement
				*/
				static const char* METHOD; 
				
				/** the path to the scwrl binary 
				 */
				static const char* SCWRL_BINARY_PATH;	

				/** An optional input file name for Scwrl.
				 *
				 *  If this is set to "" (default), we will use a
				 *  temporary file that is cleaned after the processor has
				 *  finished.
				 */
				static const char* SCWRL_INPUT_FILE;

				/** An optional input sequence file name for Scwrl.
				 *
				 *  If this is set to "" (default), we will use a
				 *  temporary file that is cleaned after the processor has
				 *  finished.
				 */
				static const char* SCWRL_SEQUENCE_FILE;

				/** An optional file name for the Scwrl output.
				 *
				 *  If this is set to "" (default), we will use a
				 *  temporary file that is cleaned after the processor has
				 *  finished and that is created by Scwrl itself.
				 *
				 *  If this is set to something else, we skip the Scwrl
				 *  execution and read in this file directly.
				 */
				static const char* SCWRL_OUTPUT_FILE;

				/** mutate and compute side chain positions for selected amino acids
				 */
				static const char* MUTATE_SELECTED_SIDE_CHAINS;
				
			};

			/// Default values for options
			struct BALL_EXPORT Default
			{
				static const String METHOD;
				static const String SCWRL_BINARY_PATH;
				static const bool   MUTATE_SELECTED_SIDE_CHAINS;
				static const String SCWRL_INPUT_FILE;
				static const String SCWRL_SEQUENCE_FILE;
				static const String SCWRL_OUTPUT_FILE;
			};

			struct BALL_EXPORT Method
			{
				/**
				 * SCWRL4 is a program for predicting side-chain conformations for a given protein backbone.
				 */
				static const String SCWRL_4_0;
				//static const String SCWRL_SERVER; 
				//static const String ILP;
			};
			
			//@}
		
			BALL_CREATE(SideChainPlacementProcessor);

			/** @name	Constructors and Destructors
			*/
			//@{
					
			///	Default Constructor
			SideChainPlacementProcessor();
		
			/// Copy Construcor
			SideChainPlacementProcessor(const SideChainPlacementProcessor& scpp);
		
			/// Destructor
			virtual ~SideChainPlacementProcessor();
			//@}

			/**	@name	Processor-related methods 
			*/
			//@{

			/// Processor method which is called before the operator()-call.
			virtual bool start();
	
			/** Clears the datastructures.
			* 	<b>NOTE:</b> The options remain!
			* 	Use setDefaultOptions() to clear the options.
			*/
			void clear();

			/** Operator () for the processor 
			 *
			 * Called with \link Default Default\endlink-options the processor 
			 * assigns side chain torsion angles to residues. 
			 * 
			 * The processor can be applied to a system, a protein, or a chain.
			 * The selection mechanism can be used to restrict the processor to
			 * certain residues. If no selection is given all side chains are considered.
	 		 * The option Option::MUTATE_SELECTED_SIDE_CHAINS can be used to mutate selected
			 * amino acids as specified in the member mutated_sequence_ 
			 * (see \link setMutations(String mutationed_seq) setMutations(String mutationed_seq) \endlink.
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
			
			/** Resets the options to default values.
			*/
			void setDefaultOptions();
			
			/** Set the mutation indicating sequence.
			*/
			void setMutations(String mutated_seq) {mutated_sequence_ = mutated_seq;} 
			
			/** Get the mutated sequence.
			 */
			const String& getMutations() const {return mutated_sequence_;}
			
			/** Get the mutated sequence.
			 */
			String getMutations() {return mutated_sequence_;}

			//@}
			/** @name Assignment
			*/
			//@{

			/// assignment operator
			SideChainPlacementProcessor& operator = (const SideChainPlacementProcessor& scpp);
			
			
			//@}
			/** @name Public Attributes
			*/
			//@{
			/// options
			Options options;

			//@}
		
		protected:
			/** Reads, checks and stores the options. 	
			 *
			 * @return bool - false if one of the options got an invalid value.
			 * @return bool - true otherwise
			 */
			bool readOptions_(); 
		
			/// Sequence in OneLetterCode with mutated residues.
			String mutated_sequence_;	

			// The processor state. 
			bool valid_;

	};

} // namespace BALL 


#endif // BALL_STRUCTURE_SIDECHAINPLACEMENTPROCESSOR_H

// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_STRUCTURE_HBONDPROCESSOR_H
#define BALL_STRUCTURE_HBONDPROCESSOR_H

#ifndef BALL_CONCEPT_PROCESSOR_H
#include <BALL/CONCEPT/processor.h>
#endif

#ifndef BALL_KERNEL_RESIDUE_H
#include <BALL/KERNEL/residue.h>
#endif

#ifndef BALL_MATHS_VECTOR3_H
#include <BALL/MATHS/vector3.h> 
#endif

#ifndef BALL_DATATYPE_OPTIONS_H
	#include <BALL/DATATYPE/options.h>
#endif

#include <vector>
#include <map>

namespace BALL
{

  /** This processor computes hydrogen bonds, stores the backbone hydrogenbonds as artificial index pairs and
	 *  introduces for all hydrogen bonds found a BALL::Bond of type BALL::Bond::TYPE__HYDROGEN.
	 * 	
	 * 	The artifical indices are created by iterating with the ResidueIterator and assigning ascending numbers starting with zero.
	 * 	NOTE: After finishing the processor, the ResidueIterator may change the arrangement of the sequence!! 
	 * 
	 *  The HBondProcessor offers two prediction methods: 
	 *  BALL::HBondProcessor::PredictionMethod::KABSCH_SANDER and
	 *  BALL::HBondProcessor::PredictionMethod::WISHART_ET_AL
	 *  Default prediction method is BALL::HBondProcessor::PredictionMethod::KABSCH_SANDER.
	 *
	 *  The BALL::HBondProcessor::PredictionMethod::KABSCH_SANDER computes all 
	 *  __backbone__ hydrogen bonds occuring between amino acids in
	 *  the composite it is applied to. The computation of the bonds follows the
	 *  criterion given in "Kabsch W & Sander C (1983). Dictionary of protein secondary 
	 *  structure: pattern recognition of hydrogen-bonded and geometrical features. 
	 *  Biopolymers, 22, 2577-2637".
	 *
	 *
	 *  The  BALL::HBondProcessor::PredictionMethod::WISHART_ET_AL computes all
	 *  hydrogen bonds occuring between amid and alpha hydrogens (H/HA) and 
	 *  carbonyl oxygens on the backbone (O) or sidechain oxygens (OD, OE, OG, OH)
	 *	in the composite it is applied to. The computation of the bonds follows the
	 *  criterion given in " Neal, S., Nip, A. M., Zhang, H., and Wishart, D. S. (2003). 
	 *  Rapid and accurate calculation of protein 1H, 13C and 15N chemical shifts. 
	 *  J Biomol NMR, 26(3):215-240.".
   */
  class BALL_EXPORT HBondProcessor 
		:	public UnaryProcessor<Composite>
  {
 
		public:
			/** @name Constant Definitions
			*/
			//@{
			/// Option names
			struct BALL_EXPORT Option
			{
				/** option for the predictions method for the hydrogen bonds
				 */
				static const String PREDICTION_METHOD;
			};	
			
			/// Default values for options
			struct BALL_EXPORT Default
			{
				/// default method for hydrogen bond prediction
				static const String PREDICTION_METHOD;
			};
			
			/// Default values for options
			struct BALL_EXPORT PredictionMethod
			{
				static const String KABSCH_SANDER;
				static const String WISHART_ET_AL;	
			};
				
			// constants for Kabsch Sander
			// 5.2 Angstrom is the maximum distance between N and O in a hydrogen bond.
			// 4.2 Angstrom is an upper bound for the distance between N and O in the same
			// residue. Therefore, if two residues are farther apart than MAX_LENGTH, N and O
			// are farther apart than 5.2 Angstroms and there will be no hydrogen bond.
			const float MAX_LENGTH;
			const float BOND_LENGTH_N_H;
			const float BOND_LENGTH_C_O;

			// constants for Wishard et al
			const float AMIDE_PROTON_OXYGEN_SEPARATION_DISTANCE; // = 3.5;
			const float ALPHA_PROTON_OXYGEN_SEPARATION_DISTANCE; // = 2.77208;

			
   	 	///
			struct BALL_EXPORT ResidueData
			{
				Vector3 pos_C;
				Vector3 pos_N;
				Vector3 pos_H;
				Vector3 pos_O;
				Size		number;
				Residue*	res; 
				/// sometimes there are defect residues around...
				bool is_complete; 
			};
  
		
		/**	Constructors and Descructor */
		//@{
		
    /// Default constructor. 
    HBondProcessor();

		/// Detailed constructor.
		HBondProcessor(Options& new_options);
		
    ///
		virtual ~HBondProcessor();
		
    /// not implemented yet
		//HBondProcessor(const HBondProcessor& hbp);
		
		//@}
		/**	Processor-related methods */
		//@{
		
		/** Initialization method.
			*/
    virtual void init() throw();

    /** Processor start method.
     */
    virtual bool start() throw();

    /// in case of Kabsch Sander: determines per residue the N, O, C , 
		//														computes the theoretical position of H
		//														and stores the residue information in residue_data_
		//														
		/// in case of Wishart et Al: collects the donors (H, HA) and acceptors all kinds of O 
    virtual Processor::Result operator() (Composite &composite);
		
		/// Finish computes all hbonds of the composite according 
		// to the chosen method <tt> PredictionMethod</tt>.
    virtual bool finish();

		//@}

		///
    const std::vector< std::vector<Position> >& getBackboneHBondPairs() const;

		/// 
    const std::vector<ResidueData>& getResidueData() const;
		
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

    void preComputeBonds_(ResidueIterator& data);
		bool finishKabschSander_();
		bool finishWishartEtAl_();
		
		/// for the Kabsch Sander method: 
		
		//_ lower point of the grid
    Vector3 upper_;
		//_ upper point of the grid
    Vector3 lower_;
    
    /// the atom positions and an accending number per residue 
    vector<ResidueData> residue_data_;


		/// for both methods:

    /// vector to store backbone HBond partners
    std::vector<std::vector<Position> > backbone_h_bond_pairs_;


		/// for the Wishart et al method: 
		
    /*_ list of HBond donors collected by <tt>operator ()</tt>
     */
    std::vector<Atom*> 						donors_;

    /*_ list of HBond acceptors collected by <tt>operator ()</tt>
     */
    std::vector<Atom*> 						acceptors_;
		
		/*_ map from residue ptr to ascending numbers for the secondary structure processor
		 */
		std::map< Residue*, Position>						residue_ptr_to_position_;		
  }; //class HBondProcessor
} //namesspace BALL


#endif // BALL_STRUCTURE_HBONDPROCESSOR_H

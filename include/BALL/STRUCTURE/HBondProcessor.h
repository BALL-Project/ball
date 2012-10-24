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
	/** Hydrogen Bond Processor
	    \ingroup StructureMiscellaneous
	 */

  /** \brief Computation of hydrogen bonds from topology information.
	
	    This processor computes hydrogen bonds and introduces for all hydrogen bonds 
			found a BALL::Bond of type BALL::Bond::TYPE__HYDROGEN, if the property ADD_BONDS is set.
			In all cases, the detected HBonds can be returned by calling getHBonds().

			Please note that the properties of the bond are <em>not</em> recomputed automatically
			if the System changes! Thus, bonds and their properties (angle, length) might
			become obsolete!
	    
	    <br>
	    Example code: <br> 
	    \code
			   Protein* protein = ..; 
   		   HBondProcessor hbp;
         hbp.options.set(HBondProcessor::Option::PREDICTION_METHOD, 
                         HBondProcessor::PredictionMethod::KABSCH_SANDER);
   		   protein->apply(hbp);  
        
   		   std::vector<std::vector<Position> > h_bond_indices = hbp.getBackboneHBondPattern();
				 std::vector<HBondProcessor::HBond>  h_bonds        = hbp.getHBonds(); 
				 for (Size i=0; i<h_bonds.size(); ++i)
   			 {
     				cout << h_bonds[i].getDonor()->getFullName()    << " bond to " 
				         << h_bonds[i].getAcceptor()->getFullName() << ":" 
            cout << h_bonds[i].getLength() << endl;
         }
      \endcode

	 * 	The artifical indices of method getBackboneHBondPattern() are 
	 * 	created by iterating with the ResidueIterator and
	 * 	assigning ascending numbers starting with zero.
	 * 	NOTE: After finishing the processor, the ResidueIterator may 
	 * 	change the arrangement of the sequence thus the return value 
	 * 	gets disrupted!! 
	 * 
	 *  The HBondProcessor offers two prediction methods: 
	 *  BALL::HBondProcessor::PredictionMethod::KABSCH_SANDER and
	 *  BALL::HBondProcessor::PredictionMethod::WISHART_ET_AL
	 *  Default prediction method is BALL::HBondProcessor::PredictionMethod::KABSCH_SANDER.
	 *
	 *  The BALL::HBondProcessor::PredictionMethod::KABSCH_SANDER computes all 
	 *  __backbone__ hydrogen bonds occuring between amino acids in
	 *  the composite it is applied to. Here, a hydrogen bond is created between
	 *  donor and acceptor, e.g., between N and O.
	 *
	 *  The computation of the bonds follows the criterion given in 
	 *  "Kabsch W & Sander C (1983). Dictionary of protein secondary 
	 *  structure: pattern recognition of hydrogen-bonded and geometrical features. 
	 *  Biopolymers, 22, 2577-2637".
	 *
	 *  Please note that in the Kabsch Sander approach water is excluded from the computations.
	 *
	 *  The  BALL::HBondProcessor::PredictionMethod::WISHART_ET_AL computes all
	 *  hydrogen bonds occuring between amid and alpha hydrogens (H/HA) and 
	 *  carbonyl oxygens on the backbone (O) or sidechain oxygens (OD, OE, OG, OH)
	 *	in the composite it is applied to. Here, a hydrogen bond is created between
	 *	the hydrogen atom of the donor and the acceptor, e.g., between HA and O.
	 *
	 *	The computation of the bonds follows the criterion given in 
	 *	" Neal, S., Nip, A. M., Zhang, H., and Wishart, D. S. (2003). 
	 *    Rapid and accurate calculation of protein 1H, 13C and 15N chemical shifts. 
	 *    J Biomol NMR, 26(3):215-240.".	
	 *
   */
  class BALL_EXPORT HBondProcessor
		: public UnaryProcessor<Composite>
  {

		public:

			/** @name Nested classes
			 */
			//@{

			/** Class to represent a hydrogen bond.
			 */
			class BALL_EXPORT HBond
			{
				public:
					HBond();
					HBond(Atom const* acceptor, Atom const* donor, bool donor_is_hydrogen=false)
					      {acceptor_ = acceptor; donor_ = donor; donor_is_hydrogen_ = donor_is_hydrogen;}
					Atom const* getAcceptor() { return acceptor_; }
					Atom const* getDonor()    { return donor_; }

					/// Return flag indicating what HBond definition was used
					bool donorIsHydrogen()    { return donor_is_hydrogen_; }

					/// Return distance between donor and acceptor
					float getLength() { return acceptor_->getDistance(*donor_); }

					// TODO: getAngle()? This is tricky as the DSSP works with implicit hydrogen positions

				protected:
					Atom const* acceptor_;
					Atom const* donor_;
					bool        donor_is_hydrogen_;
			};
			//@}

			/** @name Constant Definitions
			*/
			//@{
			/// Option names
			struct BALL_EXPORT Option
			{
				/** the hydrogen bond prediction criterion used
				 */
				static const String PREDICTION_METHOD;

				/** add bonds to the system
				 */
				static const String ADD_HBONDS;

				/** the energy cutoff for the Kabsch Sander algorithm
				 */
				static const String KABSCH_SANDER_ENERGY_CUTOFF;
			};

			/// Default values for options
			struct BALL_EXPORT Default
			{
				static const String PREDICTION_METHOD;
				static const bool ADD_HBONDS;
				static const float KABSCH_SANDER_ENERGY_CUTOFF;
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
			static float MAX_LENGTH;
			static float BOND_LENGTH_N_H;
			static float BOND_LENGTH_C_O;

			// constants for Wishard et al
			static float AMIDE_PROTON_OXYGEN_SEPARATION_DISTANCE; // = 3.5;
			static float ALPHA_PROTON_OXYGEN_SEPARATION_DISTANCE; // = 2.77208;

			///
			struct BALL_EXPORT ResidueData
			{
				Vector3  pos_C;
				Vector3  pos_N;
				Vector3  pos_H;
				Vector3  pos_O;
				Size     number;
				Residue* res;
				/// sometimes there are defect residues around...
				bool is_complete;
			};

			BALL_CREATE(HBondProcessor);

			/**	Constructors and Descructor */
			//@{

			/// Default constructor. 
			HBondProcessor();

			/// Detailed constructor.
			HBondProcessor(Options& new_options);

			///
			virtual ~HBondProcessor();

			//@}


			/** @name	Processor-related methods */
			//@{

			/** Initialization method.
			 */
			virtual void init();

			/** Processor start method.
			 */
			virtual bool start();

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

			/**	@name	Access methods
			 */
			//@{

			///	
			const std::vector< HBond>& getHBonds() const {return h_bonds_;}

			///
			std::vector< HBond> getHBonds() {return h_bonds_;}

			///	
			BALL_DEPRECATED const std::vector< std::vector<Position> >& getBackboneHBondPairs() const;

			/// computes the HBond pattern as needed, e.g. by the SecondaryStructureProcessor
			const std::vector< std::vector<Position> >& getBackboneHBondPattern() const;

			/// 
			const std::vector<ResidueData>& getResidueData() const;

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

			void preComputeBonds_(ResidueIterator& data);
			bool finishKabschSander_();
			bool finishWishartEtAl_();


			/** @name Kabsch Sander related objects
			 */
			//@{

			//_ lower point of the grid
			Vector3         upper_;
			//_ upper point of the grid
			Vector3         lower_;

			//_ the atom positions and an accending number per residue 
			std::vector<ResidueData>            residue_data_;

			//_ the backbone hydrogen bonds for the secondary structure processor
			std::vector<std::vector<Position> > backbone_h_bond_pairs_;
			//@}



			/** @name  Wishart et al related objects
			 */
			//@{

			/*_ list of __ShiftX__ HBond donors collected by <tt>operator ()</tt>
			 */
			std::vector<Atom*>             donors_;

			/*_ list of HBond acceptors collected by <tt>operator ()</tt>
			 */
			std::vector<Atom*>             acceptors_;

			std::map< Residue*, Position>  residue_ptr_to_position_;
			//@}

			/** @name  objects for both prediciton methods
			 */
			//@{

			/// store HBond 
			std::vector<HBond>        h_bonds_;
			//@}

  }; //class HBondProcessor
} //namesspace BALL


#endif // BALL_STRUCTURE_HBONDPROCESSOR_H

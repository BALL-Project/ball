// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: HBondShiftProcessor.h,v 1.14.10.5 2007-04-12 13:53:53 anne Exp $
//

#ifndef BALL_NMR_HBONDSHIFTPROCESSOR_H
#define BALL_NMR_HBONDSHIFTPROCESSOR_H

#ifndef BALL_KERNEL_PTE_H
# include <BALL/KERNEL/PTE.h>
#endif

#ifndef BALL_KERNEL_BOND_H
# include <BALL/KERNEL/bond.h>
#endif

#ifndef BALL_KERNEL_ATOM_H
# include <BALL/KERNEL/atom.h>
#endif

#ifndef BALL_NMR_SHIFT_MODULE_H
# include <BALL/NMR/shiftModule.h>
#endif

#ifndef BALL_FORMAT_PARAMETERSECTION_H
# include <BALL/FORMAT/parameterSection.h>
#endif

#include <map>

namespace BALL
{
  /** Shift assignment processor implementing the effect of hydrogen bonds. 
	\ingroup ShiftModulesNMR	
  */
  class BALL_EXPORT HBondShiftProcessor
	: public ShiftModule
  {
    public:
    BALL_CREATE(HBondShiftProcessor)

    /** @name Enums and Constants
    */
    //@{

    static const char* PROPERTY__HBOND_SHIFT;

    //@}
    /** @name Constructors and Destructors
    */
    //@{

    /** Default constructor.
    */
    HBondShiftProcessor()
      ;

    /** Copy constructor.
    */
    HBondShiftProcessor(const HBondShiftProcessor& processor)
      ;

    /** Destructor.
    */
    virtual ~HBondShiftProcessor()
      ;

    //@}
    /** @name Accessors
    */
    //@{

    /** Initialization method.
	      This method reads the parameter section "HBondEffect" and parses its content.
		    This section contains the parameters used to calculate the contribution of a hydrogen bond
		    to the chemical shift of alpha protons and amide protons. 
				
				In addition, this section contains the definition of the Hbond effector types, which are stored in the vector 
				{ /tt acceptor_types_}.

				
				The formular for amid protons is given as
				
									  amide_proton_factor_
			    shift =  ----------------------  - amide_proton_subtrahend_,
			   								r^3
			
				the formular for alpha protons is given as
				

			    				0.147521    (1.65458E-05)		 0.000134668				0.0598561			 15.6855
				  shift = -------  - -------------  - -------------   +  -----------  + --------  - 0.673905 .
			 		  		   	r							r^1.5					  r*r								r^2.5					r^3


				The section contains the following flags:
				-  amide_protons_are_targets denotes, whether amid protons are targets of the HydrogenBond 
					 shift effect. Default is false.
				-  $var amide_proton_factor$  denotes the factor of the amid term, default is zero.
				-  $var amide_proton_subtrahend$ denotes the subtrahend of the amid term, default is  zero. 
				-  $var amide_proton_oxygen_hydrogen_separation_distance $ denotes the maximal possible 
					 HN--O distance, default is zero.
			  -  $var alpha_proton_oxygen_hydrogen_separation_distance $ denotes the denotes the maximal possible 
					 H_alpha--O distance, default is zero.
				-  $var exclude_selfinteraction$ denotes if effectors (O) influence hydrogens of the same residue, default is true.
				-  $var ShiftXwise_hydrogen_bonds_computation $ denotes if the hydrogenbonds are computed as in the ShiftX approach, 
				 	 default is false.
			
				
				Right now, it is assumed that the hydrogen bonds were already set via for example the <tt>HBondProcessor</tt>.
		*/
    virtual void init();

    //@}
    /** @name Processor specific funtions.
     */
    //@{

    /** Processor start method.
		 *  Clears the donor and acceptor lists.
     */
    virtual bool start();

    /** operator ().
	      This method adds all acceptors () specified in the vector <tt>acceptor_types_</tt> 
				to  <tt>acceptors_</tt> and all donors to <tt>donors_</tt> vector.
    */
    virtual Processor::Result operator () (Composite& composite);

    
    /** Finish method.
	      This method performs the chemical shift calculation as specified in the SHIFTX.ini - file read in the
				init() method.
				According to $var ShiftXwise_hydrogen_bonds_computation_ $ we temporarily compute hydrogenbonds in an 
				ShiftX like manner or we assume the hydrogen bonds already be set. 
				
		 The ShiftX definition of hydrogen bonds is: 
		 * - Donors are: H and HA
		 * - Acceptors are: O, OD_n, OE_n, OG_n, OH_n or water in the solvent!
		 * 	 By now we do not consider water (as ShiftX does not consider too)!
		 * - Donors and Acceptors have to be on different residues.
		 * - HA even does not form hydrogen bonds with neighbours.
		 * - If the acceptor is a solvent oxygen, the donor must not be a HA (not yet implemented!)
		 * - The oxygen--hydrogen separation must be less than 3.5 A for H(N) 
		 * 	 					and 2.77 A for HA.
		 * - For all hydrogens must hold: 
		 * 	 	 ~the angle between the N_H donor bond vector and the C=O acceptor bond vector must be 
		 * 	 						90 degrees or more, and 
		 * 	 	 ~the hydrogen-oxygen  distance must be less than 2.5 + cos of the angle, and 
		 * 	 	 ~hydrogen-oxygen distance must be less than the nitrogen - oxygen distance
		 
		 
		 		Having applied these rules to each donor--acceptor pair, we  
		 		sort the list of possible bonds by the O_H separation distance, shortest to 
		 		longest. The list is then processed so that only the single strongest hydrogen 
		 		bond is identified for each donor--acceptor pair. Up to that point
		 		any bond involving the same donor or acceptor is preculded! 
				
				The chemical shift is added to the \emph{acceptor} atom using the named property 
				 \link ShiftModule::PROPERTY__SHIFT ShiftModule::PROPERTY__SHIFT \endlink 
				and stored in the named property  \link PROPERTY__HBOND_SHIFT PROPERTY__HBOND_SHIFT \endlink .
				@return bool, <b>false</b> if <tt>invalid</tt>
    */
    virtual bool finish();

    //@}

    protected:
		
		/*_ Vector of hydrogen bond acceptor types collected from the <tt>ShiftX.ini-file</tt> by <tt>init ()</tt>.
     */
		std::vector<String> 				acceptor_types_;
		
    /*_ Vector of hydrogen bond donors collected by <tt>operator ()</tt>.
     */
    std::vector<Atom*> 						donors_;

    /*_ Vector of hydrogen bond acceptors collected by <tt>operator ()</tt> as defined in <tt>acceptor_types_</tt>. 
     */
    std::vector<Atom*> 						acceptors_;

		/*_  A flag indicating whether the HBond effect affects amide protons too.
     */
		bool 												amide_protons_are_targets_;
		
		/*_  The default factor for computing the amide protons shift.
     */
		float												amide_proton_factor_ ; 
		
		/*_  The default subtrahend for computing the amide protons shift.
     */
		float 											amide_proton_subtrahend_;

		/*_  The default distance between oxygen and hydrogen for amide hydrogens.
     */
		float 											amide_proton_oxygen_hydrogen_separation_distance_;
		
		/*_  The default distance between oxygen and hydrogen for alpha hydrogens.
     */
		float 											alpha_proton_oxygen_hydrogen_separation_distance_;
		
		/*_  A flag indicating whether the HBond-donar and acceptor must be on different residues.
		 *   The default value is true. 
     */
		bool 												exclude_selfinteraction_;

		/*_  A flag indicating whether the HBonds should be computed ShiftX-wise. 
		 * 	 NOTE: This will just compute __temporarily__  hydrogen bonds 
		 * 	 				found with the ShiftX - algorithm, whose definition of a 
		 * 	 				hydrogen bond differ substantially from a BALL definition.
		 * 	 			 The ShiftX definition of hydrogen bonds is: 
		 * 	 				Donors are: H and HA
		 * 	 				Acceptors are: O, OD_n, OE_n, OG_n, OH_n or water in the solvent! ...
		 * 					By now, we do not consider water!  
		 * 	 The default value is false! 
     */
		bool 												ShiftXwise_hydrogen_bonds_computation_;

		private:
		void 			printParameters_();
		void  		printEffectors_();
		void 			printTargets_();
		Atom* 		getDonor_(Atom* a);
		void			postprocessing_();


		/* Distance, donor, acceptor for the ShiftXwise hydrogenbond determination.*/
		std::multimap<float, std::pair<Atom*, Atom*> >  hbonds_;
		std::map<Atom*, bool> donor_occupied_;
		std::map<Atom*, bool> acceptor_occupied_;
};
} // namespace BALL

#endif

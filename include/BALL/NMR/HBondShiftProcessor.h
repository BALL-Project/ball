// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: HBondShiftProcessor.h,v 1.14.10.1 2006/08/31 17:56:29 anne Exp $
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
	      This method reads the parameter section "HBondEffect" and parses its contents.
		    This section contains the parameters used to calculate the contribution of a hydrogen bond
		    to the chemical shift of alpha protons and amide protons. 
				Right now, it is assumed that the hydrogen bonds were already set via for example the <tt>HBondProcessor</tt>.
		*/
    virtual void init();

    //@}
    /** @name Processor specific funtions.
     */
    //@{

    /** Processor start method.
     */
    virtual bool start();

    /** operator ().
	      This method adds all acceptors () specified in the vector <tt>acceptor_types_</tt> 
				to  <tt>acceptors_</tt> and all donors to <tt>donors_</tt>.
    */
    virtual Processor::Result operator () (Composite& composite);

    
    /** Finish method.
	      This method performs the chemical shift calculation.
				For each donor in  \link donor_list_ donor_list_ \endlink , it looks for acceptors in the  \link acceptor_list_ acceptor_list_ \endlink  that
				lie inside a spherical shell of inner radius  \link minimum_bond_length_ minimum_bond_length_ \endlink  and outer radius 
				 \link maximum_bond_length_ maximum_bond_length_ \endlink 
				with the donor in its center. Then, it calculates the bond length $x$ and
				calculates the chemical shift as $ delta_{HBond} = a*x + b $
				The chemical shift is stored in the \emph{acceptor} atom using the named property 
				 \link ShiftModule::PROPERTY__SHIFT ShiftModule::PROPERTY__SHIFT \endlink 
				and in the named property  \link PROPERTY__HBOND_SHIFT PROPERTY__HBOND_SHIFT \endlink .
				@return bool, <b>false</b> if <tt>parameters_ == 0</tt>
    */
    virtual bool finish();

    //@}

    protected:
		
		/*_ vector of HBond acceptor types collected from the <tt>ShiftX.ini-file</tt> by <tt>init ()</tt>.
     */
		std::vector<String> 				acceptor_types_;
		
    /*_ list of HBond donors collected by <tt>operator ()</tt>
     */
    std::vector<Atom*> 						donors_;

    /*_ list of HBond acceptors collected by <tt>operator ()</tt>
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
     */
		bool 												exclude_selfinteraction_;

		private:
		void 			printParameters_();
		void  		printEffectors_();
		void 			printTargets_();
		Atom* 		getDonor_(Atom* a);
};
} // namespace BALL

#endif

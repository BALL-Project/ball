// $Id: HBondShiftProcessor.h,v 1.3 2001/03/02 22:19:33 amoll Exp $

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

#ifndef BALL_NMR_SHIFT_MODULE
# include <BALL/NMR/shiftModule.h>
#endif

#ifndef BALL_FORMAT_PARAMETERSECTION_H
# include <BALL/FORMAT/parameterSection.h>
#endif

namespace BALL
{

  /** Shift assignment processor implementing the effect of hydrogen bonds. \\
			{\bf Definition}\URL{BALL/NMR/HBondShiftProcessor.h}
  */
  class HBondShiftProcessor
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
      throw();

    /** Copy constructor.
    */
    HBondShiftProcessor(const HBondShiftProcessor& processor)
      throw();

    /** Destructor.
    */
    virtual ~HBondShiftProcessor()
      throw();

    //@}
    /** @name Accessors
    */
    //@{

    /** Initialization method.
	      This method reads the parameter section "HBondEffect" and parses its contents.
		    This section contains the parameters used to calculate the contribution of a hydrogen bond
		    to the chemical shift. Right now, it is assumed that the effect is linear in the bond length
		    and the slope {\tt a} and the shift {\tt b} are universal, that is the same for all 
				kinds of hydrogen bonds.
    */
    virtual void init()
      throw();

    //@}
    /** @name Processor specific funtions.
     */
    //@{

    /** Processor start method.
     */
    virtual bool start()
      throw();

    /** operator ().
	      This method adds all acceptors to the {\tt acceptor_list_} and all donators to the
		    {\tt donator_list_}.
    */
    virtual Processor::Result operator () (Composite& composite)
      throw();

    
    /** Finish method.
	      This method performs the chemical shift calculation.
				For each donator in \Ref{donator_list_}, it looks for acceptors in the \Ref{acceptor_list_} that
				lie inside a spherical shell of inner radius \Ref{minimum_bond_length_} and outer radius 
				\Ref{maximum_bond_length_}
				with the donator in its center. Then, it calculates the bond length $x$ and
				calculates the chemical shift as $ delta_{HBond} = a*x + b $
				The chemical shift is stored in the \emph{acceptor} atom using the named property 
				\Ref{ShiftModule::PROPERTY__SHIFT}
				and in the named property \Ref{PROPERTY__HBOND_SHIFT}.
				@return bool, {\bf false} if {\tt parameters_ == 0}
    */
    virtual bool finish()
      throw();

    //@}

    protected:

    /** list of HBond donators collected by {\tt operator ()}
     */
    std::list<Atom*> donator_list_;

    /** list of HBond acceptors collected by {\tt operator ()}
     */
    std::list<Atom*> acceptor_list_;

    /** slope of the linear relation for the chemical shift.
     */
    float a_;

    /** zero value of the linear relation for the chemical shift.
     */
    float b_;

    /** minimum distance for an HBond.
     */
    float minimum_bond_length_;

    /** maximum distance for an HBond.
     */
    float maximum_bond_length_;
};

} // namespace BALL

#endif

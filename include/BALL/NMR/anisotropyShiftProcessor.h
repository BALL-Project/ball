// $Id: anisotropyShiftProcessor.h,v 1.7 2001/02/03 13:33:25 amoll Exp $

#ifndef BALL_NMR_ANISOTROPYSHIFTPROCESSOR_H
#define BALL_NMR_ANISOTROPYSHIFTPROCESSOR_H

#ifndef BALL_KERNEL_BOND_H
# include <BALL/KERNEL/bond.h>
#endif

#ifndef BALL_KERNEL_ATOM_H
#include<BALL/KERNEL/atom.h>
#endif

#ifndef BALL_NMR_SHIFT_MODULE
#	include<BALL/NMR/shiftModule.h>
#endif

#include <list>

namespace BALL 
{
		
	/**	Shift assignment processor implementing Anisotropy
	*/
	class AnisotropyShiftProcessor
		:	public ShiftModule
	{
		public:

    BALL_CREATE(AnisotropyShiftProcessor)

    /** @name Enums and Constants
    */
    //@{

    /** A symbolic name for the electric field contribution to the chemical shift
        @see ShiftModule::PROPERTY__SHIFT
    */
    static const char* PROPERTY__ANISOTROPY_SHIFT;
    //@}
 
		/**@name	Constructors and Destructors
		*/
		//@{

		/**	Default constructor.
		*/
		AnisotropyShiftProcessor()
			throw();
		
		/**	Destructor
		*/
		virtual ~AnisotropyShiftProcessor() 
			throw();
		
		//@}
		/**@name	Processor specific functions.
		*/
		//@{

		/**	Finish method.
				Here chemical shift calculation is done.
				An iteration over every Hydrogen in {\tt proton\_list\_} is started.
				For each of these Hydrogens an iteration is started over every effector bounds
				in {\tt eff\_list\_}.
				{\tt c\_atom} is assigned the first atom of effector bond and {\tt o\_atom} is the second.
				If the actual proton is in another residue then {\tt c\_atom} calculation is started.
				The next bounded carbonate atom of {\tt c\_atom} is stored in {\tt x\_atom}.
				Now we have three atoms with their positions stored in 
				{\tt c\_pos} , {\tt o\_pos} and {\tt x\_pos}.

				We then build a cartesian coordinate system with these vectors.
				{\tt vz} = {\tt o\_pos} - {\tt c\_pos} and is normalized.
				{\tt vy} is the vectorproduct of vz and the difference vector of {\tt x\_pos} and {\tt c\_pos}
				and is normalized.
				{\tt vx} is the vectorproduct of vz and vy and is normalized.
				Then the center {\tt cen} of the effector bound is set to {\tt c\_pos} + 1.1 * {\tt vz}.
				Next three vector are calculated :
				{\tt v1} is the difference vector of the actual hydrogen and {\tt cen}.
				{\tt v2} is the vectorproduct of {\tt v1} and {\tt vy}.
				{\tt v3} is the vectorproduct of {\tt v2} and {\tt vx}.
				{\tt abstand} is assigned the length of vector {\tt v1}.
				Then {\tt stheta} is assigned the sinus of the angle between {\tt v1} and {\tt vy}.
				The sinus of the angle between {\tt v2} and {\tt vx} is assigned to {\tt sgamma}.
				Now preparations have finished and we can start the real shift calculation.
				
				{\tt calc1} = {\tt dX1} * ( ( 3.0 * {\tt stheta} * {\tt stheta} ) - 2.0 ).
				{\tt calc2} = {\tt dX2} * ( 1.0 - ( 3.0 * {\tt stheta} * {\tt stheta} * {\tt sgamma} * {\tt sgamma} ) )
				{\tt ts} = ( {\tt calc1} + {\tt calc2} ) / ( 3.0 * {\tt abstand} * {\tt abstand} * {\tt abstand} )
				{\tt ts} is added to {\tt gs}.

				{\tt dX1} and {\tt dX2} are some constant floats. 
				If the actual Hydrogen´s name is "H" the constant floats {\tt dXN1} and {\tt dXN2} are used instead.

				If {\tt eff\_list\_} has finished C=O anisotropy for actual Hydrogen is done and iteration
				over all C=N anisotropy effector bounds of {\tt eff\_list\_2\_} is started.
				This calculation is very similar with just some differences :
				We got three position vectors called:
				{\tt c\_pos}
				{\tt o\_pos} position of 
				{\tt n\_pos} position of the Nitrogen bonded to Carbonate

				the cartesion coordinate system is built on 
				{\tt vz} = {\tt n\_pos} - {\tt c\_pos}
				{\tt vy} is the vectorproduct of {\tt vz} and the difference vector of {\tt o\_pos} and {\tt c\_pos}
				{\tt vx} is the vectorproduct of {\tt vz} and {\tt vy}

				the center of the effector bond is set to :
				{\tt cen} = {\tt c_pos} + ( {\tt vz} * ( 0.85 * length of {\tt vz} ) )	

				the final calculation is just the same as above, execpt the use of different constants.
				Use {\tt ndX1} and {\tt ndX2} instead of {\tt dX1} and {\tt dX2}
				and {\tt ndXN1} and {\tt ndXN2} intead of {\tt dXN1} and {\tt dXN2}.

				Finally C=N anisotropy has finished and {\tt gs} is added to the actual hydrogen´s shift.
				Then iteration goes on with next hydrogen.
		*/
		virtual bool finish() throw();

		/**	Application method
				Atoms are stored in three different lists , named
				{\tt proton\_list\_} that contains all Hydrogens,
				{\tt eff\_list\_} that contains all effectors of C=O anisotropy
				and {\tt eff\_list\_2\_} that contains all effectors of C=N anisotropy.
				Effectors of C=O anisotropy are all bounds between atoms named "C" and "O",
				as well as bounds between atoms named "CG" and "OD1" in residues called
				"ASP" or "ASN, and finally bounds between atoms named "CD" and "OE1" in 
				residues called "GLU" and "GLN".
				Effectors of C=N anisotropy are all bounds between atoms named "C" and "N".
		*/
		virtual Processor::Result operator() (Composite& composite) throw();
		//@}

		/**	@name	Accessors
		*/
		//@{
		/**	Module initialization
		*/
		virtual void init()
			throw();
		//@}
		
		protected:
		std::list<const Atom*> proton_list_;	
		std::list<const Bond*> eff_list_;
		std::list<const Bond*> eff_list_2_;
		bool ignore_other_chain_;
	};

} // namespace BALL

#endif // BALL_NMR_ANISOTROPYSHIFTPROCESSOR_H

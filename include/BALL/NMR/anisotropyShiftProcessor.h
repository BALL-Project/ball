// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: anisotropyShiftProcessor.h,v 1.13 2003/02/21 16:05:36 anhi Exp $

#ifndef BALL_NMR_ANISOTROPYSHIFTPROCESSOR_H
#define BALL_NMR_ANISOTROPYSHIFTPROCESSOR_H

#ifndef BALL_KERNEL_BOND_H
# include <BALL/KERNEL/bond.h>
#endif

#ifndef BALL_KERNEL_ATOM_H
# include<BALL/KERNEL/atom.h>
#endif

#ifndef BALL_NMR_SHIFT_MODULE_H
#	include<BALL/NMR/shiftModule.h>
#endif

#include <list>

namespace BALL 
{
		
	/**	Shift assignment processor implementing Anisotropy.
			<b>Definition</b>BALL/NMR/anisotropyShiftProcessor.h
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
 		/** @name	Constructors and Destructors
		*/
		//@{

		/**	Default constructor.
		*/
		AnisotropyShiftProcessor()
			throw();

		/**	Copy constructor
		*/
		AnisotropyShiftProcessor(const AnisotropyShiftProcessor& processor)
			throw();
		
		/**	Destructor
		*/
		virtual ~AnisotropyShiftProcessor() 
			throw();
		
		//@}
		/** @name	Processor specific functions.
		*/
		//@{

		/**	Finish method.
				Here chemical shift calculation is done.
				An iteration over every Hydrogen in <tt>proton_list_</tt> is started.
				For each of these Hydrogens an iteration is started over every effector bounds
				in <tt>eff_list_</tt>.
				<tt>c_atom</tt> is assigned the first atom of effector bond and <tt>o_atom</tt> is the second.
				If the actual proton is in another residue then <tt>c_atom</tt> calculation is started.
				The next bounded carbonate atom of <tt>c_atom</tt> is stored in <tt>x_atom</tt>.
				Now we have three atoms with their positions stored in 
				<tt>c_pos</tt> , <tt>o_pos</tt> and {\tt x_pos}.  \par
				We then build a cartesian coordinate system with these vectors.  \par
				<tt>vz</tt> = <tt>o_pos</tt> - {\tt c_pos} and vz is normalized.
				<tt>vy</tt> is the vectorproduct of vz and the difference vector of <tt>x_pos</tt> and {\tt c_pos}
				and is normalized.
				<tt>vx</tt> is the vectorproduct of vz and vy and is normalized.
				Then the center <tt>cen</tt> of the effector bound is set to  \par
 
				<tt>c_pos</tt> + 1.1 * <tt>vz</tt>.  \par
  \par
				Next three vectors are calculated :
				<tt>v1</tt> is the difference vector of the actual hydrogen and <tt>cen</tt>.
				<tt>v2</tt> is the vectorproduct of <tt>v1</tt> and {\tt vy}.
				<tt>v3</tt> is the vectorproduct of <tt>v2</tt> and {\tt vx}.
				<tt>abstand</tt> is assigned the length of vector <tt>v1</tt>.
				Then <tt>stheta</tt> is assigned the sinus of the angle between <tt>v1</tt> and {\tt vy}.
				The sinus of the angle between <tt>v2</tt> and <tt>vx</tt> is assigned to {\tt sgamma}.
				Now preparations have finished and we can start the real shift calculation.  \par
  \par
				<tt>calc1</tt> = <tt>dX1</tt> * ( ( 3.0 * {\tt stheta} * {\tt stheta} ) - 2.0 ).  \par
				<tt>calc2</tt> = <tt>dX2</tt> * ( 1.0 - ( 3.0 * {\tt stheta} * {\tt stheta} * {\tt sgamma} * {\tt sgamma} ) )  \par
				<tt>ts</tt> = ( <tt>calc1</tt> + {\tt calc2} ) / ( 3.0 * {\tt abstand} * {\tt abstand} * {\tt abstand} )  \par
				<tt>ts</tt> is added to <tt>gs</tt>.  \par
  \par
				<tt>dX1</tt> and <tt>dX2</tt> are some constant floats. 
				If the actual Hydrogen´s name is "H" the constant floats <tt>dXN1</tt> and <tt>dXN2</tt> are used instead.  \par
				If <tt>eff_list_</tt> has finished C=O anisotropy for actual Hydrogen is done and 
				iteration	over all C=N anisotropy effector bounds of <tt>eff_list_2_</tt> is started.  \par
				This calculation is very similar with just some differences :
				We got three position vectors called:
				<tt>c_pos</tt>
				<tt>o_pos</tt> position of 
				<tt>n_pos</tt> position of the Nitrogen bonded to Carbonate.  \par
				The cartesion coordinate system is built on 
				<tt>vz</tt> = <tt>n_pos</tt> - {\tt c_pos}  \par
				<tt>vy</tt> is the vectorproduct of <tt>vz</tt> and the difference vector of {\tt o_pos} and {\tt c_pos}  \par
				<tt>vx</tt> is the vectorproduct of <tt>vz</tt> and {\tt vy}.  \par
				The center of the effector bond is set to:  \par
				<tt>cen</tt> = <tt>c_pos</tt> + ( {\tt vz} * ( 0.85 * length of {\tt vz} ) )	 \par
  \par
				The final calculation is just the same as above, except the use of different constants.
				Use <tt>ndX1</tt> and <tt>ndX2</tt> instead of {\tt dX1} and {\tt dX2}
				and <tt>ndXN1</tt> and <tt>ndXN2</tt> intead of {\tt dXN1} and {\tt dXN2}.
				Finally C=N anisotropy has finished and <tt>gs</tt> is added to the actual hydrogen´s shift.
				Then iteration goes on with the next hydrogen.
		*/
		virtual bool finish() throw();

		/**	Application method.
				Atoms are stored in three different lists , named  \par
				<tt>proton_list_</tt> that contains all Hydrogens,  \par
				<tt>eff_list_</tt> that contains all effectors of C=O anisotropy	and  \par
				<tt>eff_list_2_</tt> that contains all effectors of C=N anisotropy.
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

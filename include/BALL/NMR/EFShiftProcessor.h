// $Id: EFShiftProcessor.h,v 1.9 2001/03/02 22:19:33 amoll Exp $

#ifndef BALL_NMR_EFSHIFTPROCESSOR_H
#define BALL_NMR_EFSHIFTPROCESSOR_H

#ifndef BALL_NMR_SHIFT_MODULE
#	include<BALL/NMR/shiftModule.h>
#endif

#ifndef BALL_KERNEL_EXPRESSION_H
#	include<BALL/KERNEL/expression.h>
#endif

namespace BALL 
{

	class Atom;
		
	/**	Shift assignment processor implementing the electric field effect. \\
			{\bf Definition}\URL{BALL/NMR/EFShiftProcessor.h}
	*/
	class EFShiftProcessor
		:	public ShiftModule
	{
		public:

		BALL_CREATE(EFShiftProcessor)

		/**	@name Enums and Constants
		*/
		//@{

		/**	A symbolic name for the electric field contribution to the chemical shift.
				@see ShiftModule::PROPERTY__SHIFT
		*/
		static const char* PROPERTY__EF_SHIFT;
		//@}
		
		/** @name	Constructors and Destructors.
		*/
		//@{

		/**	Default constructor.
		*/
		EFShiftProcessor()
			throw();
	
		/**	Copy constructor.
		*/
		EFShiftProcessor(const EFShiftProcessor& processor)
			throw();
			
		/**	Destructor.
		*/
		virtual ~EFShiftProcessor()
			throw();	

		//@}
		/**	@name	Accessors.
		*/
		//@{

		/**	Initialization method.
				This method reads the parameter section {\bf ElectricFieldEffect} and
				parses its contents.
				This section contains the definition of two expressions that define
				a bond (the first expression matches the atom whose shift is to be
				calculated, the second describes its bond partner).
				For each of these bonds, two parameters are given, 
				$\varepsilon_1$ and	$\varepsilon_2$. \\
				Then, this method extracts the contents of the {\bf Charges}
				section and thus constructs a hash map containing residue and atom names 
				the corresponding charges.
				This processor is applied to all atoms in \Ref{operator ()}, 
				so expect the atom charges to change!
				@see operator ()
		*/
		virtual void init()
			throw();

		//@}
		/**	@name	Processor specific functions.
		*/
		//@{
		
		/**	Processor start method.
				This method clears the bond and effector list.
				It fails if no parameters were assigned.
				@return bool, {\bf false} if {\tt parameters_ == 0}
		*/
		virtual bool start()
			throw();


		/**	operator ().
				This method sets the charge for all atoms it encounters
				(using \Ref{assign_charge_processor_}). 
				Charged atoms	are stored in the atom list \Ref{effector_list_}.
				All bonds are stored in \Ref{bond_list_}.
				@return Processor::CONTINUE
				@param composite an arbitrary composite. All non-atom objects are ignored.
		*/
		virtual Processor::Result operator () (Composite& composite)
			throw();

		/**	Finish method.
				This method performs the chemical shift calculation.
				It iterates over all bonds stored in \Ref{bond_list_}.
				If the two bond atoms match a pair of expressions from
				\Ref{first_atom_expressions_} and \Ref{second_atom_expressions_},
				the electric field vector is calculated at the bond position using
				Coulomb's law and the charges and positions of the atoms in the 
				\Ref{effector_list_}.
				The chemical shift induced by the electric field effect is calculated as
					$ delta_{EF} = varepsilon_1 * E_z + varepsilon_2 * E^2 $
				where constants $ varepsilon_1 $ and $ varpesilon_2 $ are read
				from the parameter file (section {\bf ElectricFieldEffect}).
				The chemical shift is stored in the \emph{first} atom
				using the named property \Ref{ShiftModule::PROPERTY__SHIFT} 
				and in the named property \Ref{PROPERTY__EF_SHIFT}.
				@return bool, {\bf false} if {\tt parameters_ == 0}
		*/
		virtual bool finish()
			throw();
		//@}

		protected:
	
		/**	The list of bonds collected by {\tt operator ()}.
		*/
		std::list<Bond*>				bond_list_;

		/**	The list of charged atoms (effectors).
		*/
		std::list<Atom*>				effector_list_;

		/**	The expressions describing the first atom of a bond.
		*/
		std::vector<Expression>	first_atom_expressions_;

		/**	The expressions describing the first atom of a bond.
		*/
		std::vector<Expression>	second_atom_expressions_;

		/**	The parameter $\varepsilon_1$.
		*/
		std::vector<float>			epsilon1_;

		/**	The parameter $\varepsilon_2$.
		*/
		std::vector<float>			epsilon2_;

		/**	The charge assignment map.
		*/
		StringHashMap<float>		charge_map_;

		/**	A flag indicating whether effectors in the same residues are to be considered.
				Set this flag by specifying the option {\tt exclude_residue_field = true} in 
				the ElectricFieldShift section of the parameter file.
				Default is false.
		*/
		bool										exclude_residue_field_;

		/**	A cut off value for the electric field effect.
				Any effector that is further away than this cut off is ignored.
				The distance is read from the option {\tt cut_off} in the 
				section {\tt ElectricFieldEffect} from the parameter file.
				This member contains the squared value(!) of the distance.
		*/
		float										cut_off2_;
 	};

} // namespace BALL

#endif // BALL_NMR_EFSHIFTPROCESSOR_H

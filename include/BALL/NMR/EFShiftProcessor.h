// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: EFShiftProcessor.h,v 1.23.10.5 2007-04-12 13:53:52 anne Exp $
//

#ifndef BALL_NMR_EFSHIFTPROCESSOR_H
#define BALL_NMR_EFSHIFTPROCESSOR_H

#ifndef BALL_NMR_SHIFT_MODULE_H
#	include<BALL/NMR/shiftModule.h>
#endif

#ifndef BALL_KERNEL_EXPRESSION_H
#	include<BALL/KERNEL/expression.h>
#endif

namespace BALL 
{
	class Atom;
		
	/**	Shift assignment processor implementing the electric field effect. 
	\ingroup ShiftModulesNMR		
	*/
	class BALL_EXPORT EFShiftProcessor
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
			;
	
		/**	Copy constructor.
		*/
		EFShiftProcessor(const EFShiftProcessor& processor)
			;
			
		/**	Destructor.
		*/
		virtual ~EFShiftProcessor()
			;	

		//@}
		/**	@name	Accessors.
		*/
		//@{

		/**	Initialization method.
				This method reads the parameter section <b>ElectricFieldEffect</b> and
				parses its contents.
				This section contains four flags: 
				- $\var exclude_residue_field$ / $varexclude_adjacent_residue_field$ denotes, whether 
				effector atoms of the electric 
				field effect will be excluded, if the target bond is in the same residue, an adjacent 
				residue respectively. The defaults are false.
				- $var carbonyl_influences_amide_field$ denotes, whether a carbonyl is regarded to 
					effect the shift of an amid hydrogen. The default is false.
				- $var exclude_solvent_field$ denotes, whether water molecules are excluded to be
					effectors. The default is false.
				This section contains the definition of two expressions that define
				a bond (the first expression matches the atom whose shift is to be
				calculated, the second describes its bond partner).
				For each of these bonds, two parameters are given, 
				$\varepsilon_1$ and	$\varepsilon_2$.  \par
				Then, this method extracts the contents of the <b>Charges</b>
				section and thus constructs a hash map containing residue and atom names 
				and the corresponding charges.
				The \link operator () operator () \endlink is applied to all atoms,   
				so expect the atom charges to change!
				@see operator ()
		*/
		virtual void init()
			;

		//@}
		/**	@name	Processor specific functions.
		*/
		//@{
		
		/**	Processor start method.
				This method clears the bond and effector list.
				It fails if no parameters were assigned.
				@return bool, <b>false</b> if <tt>parameters_ == 0</tt>
		*/
		virtual bool start()
			;


		/**	Processor operator ().
				This method sets the charges for all atoms it encounters.
				The charges are given are given in parameter file (section <b>Charges</b>).
				Charged atoms	are stored in the atom list  \link effector_list_ effector_list_ \endlink .
				All tentative target bonds, specified in the parameter file (section <b>ElectricFieldEffect</b>), 
				are stored in  \link bond_list_ bond_list_ \endlink .
				@return Processor::CONTINUE
		*/
		virtual Processor::Result operator () (Composite& composite)
			;

		/**	Processor finish method.
				This method performs the chemical shift calculation.
				It iterates over all bonds stored in  \link bond_list_ bond_list_ \endlink .
				If the two bond atoms match a pair of expressions from
				 \link first_atom_expressions_ first_atom_expressions_ \endlink  and  
				 \link second_atom_expressions_ second_atom_expressions_ \endlink ,
				the electric field vector is calculated at the bond position using
				Coulomb's law and the charges and positions of the atoms in the 
				 \link effector_list_ effector_list_ \endlink .
				The chemical shift induced by the electric field effect is calculated as
					$\delta_{EF} = \varepsilon_1 * E_z + \varepsilon_2 * E^2 $
				where constants $\varepsilon_1$ and $\varepsilon_2$ are read
				from the parameter file (section <b>ElectricFieldEffect</b>).
				The chemical shift is stored in the \emph{first} atom
				using the named property  \link ShiftModule::PROPERTY__SHIFT ShiftModule::PROPERTY__SHIFT \endlink  
				and in the named property  \link PROPERTY__EF_SHIFT PROPERTY__EF_SHIFT \endlink .
				@return bool, <b>false</b> if <tt>parameters_ == 0</tt>
		*/
		virtual bool finish()
			;
			
		//@}

		protected:
	
		/*_	The list of all target bonds collected by {\tt operator ()}.
		* 	The first element of the pair is the origin of the bond, the second the destination.
		* 	The shift _always_ applies to the first element of the pair.
		*/
		std::vector<std::pair<Atom*, Atom*> >				bond_list_;

		/*_  The index of the expression that matched to result in the corresponding element in bond_list_
		 */
		std::vector<Index> 			expression_number_;

		/*_	The list of charged atoms (effectors).
		*/
		std::list<Atom*>				effector_list_;

		/*_	The expressions describing the first atom of a bond.
		*/
		std::vector<Expression>	first_atom_expressions_;

		/*_	The expressions describing the first atom of a bond.
		*/
		std::vector<Expression>	second_atom_expressions_;

		/*_	The parameter $\varepsilon_1$.
		*/
		std::vector<float>			epsilon1_;

		/*_	The parameter $\varepsilon_2$.
		*/
		std::vector<float>			epsilon2_;

		/*_	The charge assignment map.
		*/
		StringHashMap<float>		charge_map_;

		/*_	A flag indicating whether effectors in the same residues are to be considered.
				Set this flag by specifying the option {\tt exclude_residue_field = true} in 
				the ElectricFieldShift section of the parameter file.
				Default is false.
		*/
		bool										exclude_residue_field_;
		
		/*_	A flag indicating whether effectors in adjacent residues are to be considered.
				Set this flag by specifying the option {\tt exclude_adjacent_residue_field = true} in 
				the ElectricFieldShift section of the parameter file.
				Default is false.
		*/
		bool 										exclude_adjacent_residue_field_;
		
		/*_	A flag indicating whether carbonyl effectors are to be considered for amid targets.
				Set this flag by specifying the option {\tt carbonyl_influences_amide_field = false} in 
				the ElectricFieldShift section of the parameter file.
				Default is false.
		*/
		bool										carbonyl_influences_amide_field_;
		
		
		/*_	A flag indicating whether solvent atoms do act as effectors.
				Set this flag by specifying the option {\tt exclude exclude_solvent_field = true} in 
				the ElectricFieldShift section of the parameter file.
				Default is false.
		*/
		bool										exclude_solvent_field_;


		
		/*_	A cut off value for the electric field effect.
				Any effector that is further away than this cut off is ignored.
				The distance is read from the option {\tt cut_off} in the 
				section <TT>  ElectricFieldEffect </TT> from the parameter file.
				This member contains the squared value(!) of the distance.
		*/
		float										cut_off2_;


		/*_	A factor for switching the charge unit between esu and elementary charges. 
		 		The unit is read from the option {\tt unit} of the section 
				<TT>  Charges </TT> from the parameter file.
				For numeric aspects, in the init() function the esu unit is divided by 
				the charge_factor_, such that the molecules charges (which are given 
				by PDB.org in elementary units) can easily be multiplied with. 
				When computing the shift, the charge_factor is again multiplied with.  
				Default is 1.0
		*/

		float  									charge_factor_;
		
		private:
	
		/*_ Some debugging functions printing parameter/effector/target information
		 *  to the Log-stream.
		 */
		void 			printParameters_();
		void  		printEffectors_();
		void 			printTargets_();
		
		/*_ A function to perform some ShiftX-y postprocessing: 
		    add for all CA-atoms 0.2 times the EF-shift-value of the bound HA-atoms 
	  */
		void 			postprocessing_();

 	};
  
} // namespace BALL

#endif // BALL_NMR_EFSHIFTPROCESSOR_H

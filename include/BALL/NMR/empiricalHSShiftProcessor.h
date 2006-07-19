#ifndef BALL_NMR_EMPIRICALHSSHIFTPROCESSOR_H
#define BALL_NMR_EMPIRICALHSSHIFTPROCESSOR_H

#ifndef BALL_NMR_SHIFT_MODULE_H
#	include<BALL/NMR/shiftModule.h>
#endif

#ifndef BALL_KERNEL_EXPRESSION_H
#	include<BALL/KERNEL/expression.h>
#endif

# include <set>
#include <map>

namespace BALL 
{
	class Atom;
		
	/**	Shift assignment processor implementing the electric field effect. 
	\ingroup ShiftModulesNMR		
	*/
	class BALL_EXPORT EmpiricalHSShiftProcessor
		:	public ShiftModule
	{
		public:

		BALL_CREATE(EmpiricalHSShiftProcessor)

		/**	@name Enums and Constants
		*/
		//@{

		/**	A symbolic name for the electric field contribution to the chemical shift.
				@see ShiftModule::PROPERTY__SHIFT
		*/
		static const char* PROPERTY__EHS_SHIFT;
		
		//@}
		/** @name	Constructors and Destructors.
		*/
		//@{

		/**	Default constructor.
		*/
		EmpiricalHSShiftProcessor()
			throw();

			
		/**	Copy constructor.
		*/
			EmpiricalHSShiftProcessor(const 	EmpiricalHSShiftProcessor& processor)
			throw();
			
		/**	Destructor.
		*/
		virtual ~EmpiricalHSShiftProcessor()
			throw();	

		//@}
		/**	@name	Accessors.
		*/
		//@{
	
		/**	Initialization method. TODO!!!!
				This method reads the parameter section <b>ElectricFieldEffect</b> and
				parses its contents.
				This section contains the definition of two expressions that define
				a bond (the first expression matches the atom whose shift is to be
				calculated, the second describes its bond partner).
				For each of these bonds, two parameters are given, 
				$\varepsilon_1$ and	$\varepsilon_2$.  \par
				Then, this method extracts the contents of the <b>Charges</b>
				section and thus constructs a hash map containing residue and atom names 
				the corresponding charges.
				This processor is applied to all atoms in  \link operator () operator () \endlink , 
				so expect the atom charges to change!
				@see operator ()
		*/
		virtual void init()
			throw();

		//@}
		/**	@name	Processor specific functions.
		*/
		//@{
		
		/**	Processor start method. TODO!!!!
				This method clears the bond and effector list.
				It fails if no parameters were assigned.
				@return bool, <b>false</b> if <tt>parameters_ == 0</tt>
		*/
		virtual bool start()
			throw();


		/**	operator (). TODO!!!!
				This method sets the charge for all atoms it encounters
				(using  \link assign_charge_processor_ assign_charge_processor_ \endlink ). 
				Charged atoms	are stored in the atom list  \link effector_list_ effector_list_ \endlink .
				All bonds are stored in  \link bond_list_ bond_list_ \endlink .
				@return Processor::CONTINUE
				@param composite an arbitrary composite. All non-atom objects are ignored.
		*/
		virtual Processor::Result operator () (Composite& composite)
			throw();

		/**	Finish method. TODO!!!!
				This method performs the chemical shift calculation.
				It iterates over all bonds stored in  \link bond_list_ bond_list_ \endlink .
				If the two bond atoms match a pair of expressions from
				 \link first_atom_expressions_ first_atom_expressions_ \endlink  and  \link second_atom_expressions_ second_atom_expressions_ \endlink ,
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
			throw();
			
		//@}

		protected:
			
				class BALL_EXPORT PropertiesForShift_
				{
					public:
						PropertiesForShift_() throw();

						/** current target atom
						 */
						Atom*				atom;
						/**	is the current atom in the first residue of a chain? 
						 */
						bool 				is_first_residue;
						/** is the atom before in the first residue of a chain? 
						 */
						bool				prev_is_first_residue;
						/**	is the atom behind in the first residue of a chain? 
						 */
						bool				next_is_first_residue;
						
						/** amino acid type of the (previous/following) residue of the atom in degree
						 */
						char	amino_acid_type;
						char	prev_amino_acid_type;
						char	next_amino_acid_type;
						
						/** secondary structure of the (previous/following) residue of the atom in degree
						 */
						char 	secondary_structure;
						char  prev_secondary_structure;
						char 	next_secondary_structure;
						
						/** psi angle of the (previous/following) residue of the atom in degree
						 */
						float				psi_angle;
						float 			prev_psi_angle;
						float 			next_psi_angle;

						/** phi angle of the (previous/following) residue of the atom in degree
						 */
						float				phi_angle;
						float				prev_phi_angle;
						float				next_phi_angle;

						/** chi angle of the (previous/following) residue of the atom in degree
						 */
						float				chi_angle;
						float				prev_chi_angle;
						float				next_chi_angle;
						
						/** chi2 angle of the (previous/following) residue of the atom in degree
						 */
						float				chi2_angle;
						float				prev_chi2_angle;
						float				next_chi2_angle;

						/**	length of the HA-HBond of the (previous/following) residue
						 */
						float 			HA_bond_len;
						float 			prev_HA_bond_len;
						float 			next_HA_bond_len;

						/**	length of the HA2-HBond of the (previous/following) residue
						 */
						float 			HA2_bond_len;
						float 			prev_HA2_bond_len;
						float 			next_HA2_bond_len;

						/**	length of the HN-HBond of the (previous/following) residue
						 */
						float 			HN_bond_len;
						float 			prev_HN_bond_len;
						float 			next_HN_bond_len;
						
						/**	length of the O-HBond of the (previous/following) residue
						 */
						float 			OH_bond_len;
						float 			prev_OH_bond_len;
						float 			next_OH_bond_len;

						/**	has the (previous/following) residue a disulfid bond?
						 */
						bool 				has_disulfid_bond;
						bool 				prev_has_disulfid_bond;
						bool 				next_has_disulfid_bond;
						
						/**	has the (previous/following) residue a HA-HBond?
						 */
						bool 				has_HA_Hbond;
						bool 				prev_has_HA_Hbond;
						bool 				next_has_HA_Hbond;

						/**	has the (previous/following) residue a HA2-HBond?
						 */
						bool 				has_HA2_Hbond;
						bool 				prev_has_HA2_Hbond;
						bool 				next_has_HA2_Hbond;
						
						/**	has the (previous/following) residue a HN-HBond?
						 */
						bool 				has_HN_Hbond;
						bool 				prev_has_HN_Hbond;
						bool 				next_has_HN_Hbond;

						/**	has the (previous/following) residue a O-HBond?
						 */
						bool 				has_O_Hbond;

						/** hydrogen bond status
						 */
						bool				hydrogen_bond_status;

				}; // end of nested class
					
				/*_	The targeted atoms collected by {\tt operator ()}.
				*/
				std::vector<PropertiesForShift_> 				targets_;
				
				/*_ The target types stored as a vector of strings collected from the ini-file
				 *  by {\tt init ()}.
				 */
				std::vector<String>											target_names_;	

				/*_ The target properties used for determine the 
				 * empirical hypersurface shifts. The properties are collected 
				 * from the ini-file by {\tt init ()} and are stored per target atom
				 * in the same order as the target_names.
				 */
				std::vector< std::set<String> >			target_property_names_;
				
				/*_ The files storing the property-data-splines for computing the
				 * shift contributions are stored in a map, whose key is the pair of
				 * properties, to which the splines belong.
				 * The maps are stored per target atom in the same order as the target_names.
				 * The files are specified in the section {\tt EmpiricalShiftHyperSurfaces} 
				 * of the file {\tt ShiftX.ini} and are collected by {\tt init ()}.
				 */
				std::vector< std::map< std::pair<String, String>, String >	>		property_files_;
					
			private:
					void 			printParameters_() throw();
					void 			printTargets_() throw();
					float			getChiAngle_(Residue* residue) throw();
					float			getChi2Angle_(Residue* residue) throw();
					void 			setAminoAcid_(Residue* residue, char& property) throw();
					void 			setSecondaryStructure_(Residue* residue, char& property) throw();
					float 		getHA_HBondLen_(Residue* residue) throw();
					float 		getHA2_HBondLen_(Residue* residue) throw();
					float 		getHN_HBondLen_(Residue* residue) throw();


	};//End of class
} // end of namespace
#endif // BALL_NMR_EMPIRICALHSSHIFTPROCESSOR_H

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
	
		enum PROPERTIES  {
			FR,
			AA,
			SS,
			PHI,
			PSI, 
			CHI,
			CHI2,
			HA1L, 
			HA1,
			HA2L, 
			HA2,
			HNL,
			HN,
			OH,
			OHL,
			DISULFIDE,
			FR_N,
			AA_N,
			SS_N,
			PHI_N,
			PSI_N, 
			CHI_N,
			CHI2_N,
			HA1L_N, 
			HA1_N,
			HA2L_N, 
			HA2_N,
			HNL_N,
			HN_N,
			OH_N,
			OHL_N,
			DISULFIDE_N,
			FR_P,
			AA_P,
			SS_P,
			PHI_P,
			PSI_P, 
			CHI_P,
			CHI2_P,
			HA1L_P, 
			HA1_P,
			HA2L_P, 
			HA2_P,
			HNL_P,
			HN_P,
			OH_P,
			OHL_P,
			DISULFIDE_P
		};
			
		
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
		
	/** nested classe for computing Bicubic splines 
 	*/
		class CubicSpline1D_
		{
			public:
				CubicSpline1D_() {};

				void createSpline(const std::vector<float>& sample_positions, 
						const std::vector<float>& sample_values); // spline
				float operator () (float x); // splint

				std::vector<float>& getCurvature();
				void setCurvature(std::vector<float> curvature);
				void setValues(std::vector<float> values);
				void setPositions(std::vector<float> positions);
			private :
				std::vector<float> sample_positions_;
				std::vector<float> sample_values_;
				std::vector<float> curvature_;
		};


		class CubicSpline2D_
		{
			public:
				CubicSpline2D_() {};

				//precompute the second derivatives	
				void createBiCubicSpline(const std::vector<std::vector<float> >& sample_positions_x,
						const std::vector<float>& sample_positions_y,
						const std::vector<std::vector<float> >& sample_values); // splie2
				float operator () (float x, float y); // splin2

			private :
				std::vector< std::vector<float> > sample_positions_x_;
				std::vector<float> sample_positions_y_;
				std::vector<CubicSpline1D_> splines_;
		};



		/*_ neested class for storing the properties of an atom
		 */	
		class BALL_EXPORT PropertiesForShift_
		{
			public:
			
								
				PropertiesForShift_() throw();

				/** current target atom
				 */
				Atom*				atom;

				/**	 compute for the given atom all properites specified in the property set
				 **/
				bool computeProperties_(Atom* atom, std::set<String> properties) 	throw();  

				/** obtain a property. we return a std::pair<float, String> with the property that:
				 *    the first element is std::numeric_limits<float>::min() if it is a discrete property
				 *    the second element is the string "invalid" if it is a continuous property
				 */
				std::pair<float, String> operator [] (const String& property_name) throw();


			protected:
				std::map<String, float>  properties_real_;  
				std::map<String, String> properties_string_;

			protected:
				
				float			getChiAngle_(Residue* residue) throw();
				float			getChi2Angle_(Residue* residue) throw();
				char 			getAminoAcid_(Residue* residue) throw();
				char 			getSecondaryStructure_(Residue* residue) throw();
				float 		getHA_HBondLen_(Residue* residue) throw();
				float 		getHA2_HBondLen_(Residue* residue) throw();
				float 		getHN_HBondLen_(Residue* residue) throw();
				float 		getO_HBondLen_(Residue* residue) throw();
				bool 			hasDisulfidBond_(Residue* residue) throw();
				bool 			hasHA_HBond_(Residue* residue) throw();
				bool 			hasHA2_HBond_(Residue* residue) throw();
				bool 			hasHN_HBond_(Residue* residue) throw();
				bool 			hasO_HBond_(Residue* residue) throw();

				
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
				float 			HA_HBond_len;
				float 			prev_HA_HBond_len;
				float 			next_HA_HBond_len;

				/**	length of the HA2-HBond of the (previous/following) residue
				 */
				float 			HA2_HBond_len;
				float 			prev_HA2_HBond_len;
				float 			next_HA2_HBond_len;

				/**	length of the HN-HBond of the (previous/following) residue
				 */
				float 			HN_HBond_len;
				float 			prev_HN_HBond_len;
				float 			next_HN_HBond_len;

				/**	length of the O-HBond of the (previous/following) residue
				 */
				float 			O_HBond_len;
				float 			prev_O_HBond_len;
				float 			next_O_HBond_len;

				/**	has the (previous/following) residue a disulfid bond?
				 */
				bool 				has_disulfid_bond;
				bool 				prev_has_disulfid_bond;
				bool 				next_has_disulfid_bond;

				/**	has the (previous/following) residue a HA-HBond?
				 */
				bool 				has_HA_HBond;
				bool 				prev_has_HA_HBond;
				bool 				next_has_HA_HBond;

				/**	has the (previous/following) residue a HA2-HBond?
				 */
				bool 				has_HA2_HBond;
				bool 				prev_has_HA2_HBond;
				bool 				next_has_HA2_HBond;

				/**	has the (previous/following) residue a HN-HBond?
				 */
				bool 				has_HN_HBond;
				bool 				prev_has_HN_HBond;
				bool 				next_has_HN_HBond;

				/**	has the (previous/following) residue a O-HBond?
				 */
				bool 				has_O_HBond;
				bool 				prev_has_O_HBond;
				bool 				next_has_O_HBond;

				/** hydrogen bond status
				 */
				bool				hydrogen_bond_status;

		}; // end of nested class
		

		/*_ neested class for storing the hypersurfaces of a 
		 * pair of properties. 
		 **/
		
		class ShiftHyperSurface_
		{
			public:
				
				/**	@name Enums and Constants
				*/
				//@{	
				
				enum HYPERSURFACE__TYPE{
				REAL__REAL,
				REAL__DISCRETE,
				DISCRETE__REAL,
				DISCRETE__DISCRETE,
				CHI__REAL,
				REAL__CHI,
				CHI__DISCRETE,
				DISCRETE__CHI,
				CHI__CHI
				};

				/*_  Constructor
				 */
				ShiftHyperSurface_() throw();
				
				ShiftHyperSurface_(String filename, String atomtype, String firstproperty, String secondproperty) throw();  
				
				/** computes the shift of an atom. 
				 */
				float operator () (PropertiesForShift_& properties) throw();	

						
			private:
				int type_;
			 	String  first_property_;
				String  second_property_;

				// temporary variables for the property value detection
				float float_property_;
				String string_property_;

				CubicSpline2D_ 															s2d_;
				std::map<String, CubicSpline1D_ > 					s1d_; 
				std::map <String, std::map<String, float> > table_;
					
		};



		
		/*_	The targeted atoms collected by {\tt operator ()}.
				*/
				std::vector<PropertiesForShift_> 				targets_;
				
				/*_ The target types stored as a vector of strings collected from the ini-file
				 *  by {\tt init ()}.
				 */
				vector<String>											target_names_;	

				/*_ The target properties used for determine the 
				 * empirical hypersurface shifts. The properties are collected 
				 * from the ini-file by {\tt init ()} and are stored per target atom
				 * in the same order as the target_names.
				 */
				vector< std::set<String> >			target_property_names_;
				
				/*_ The files storing the property-data-splines for computing the
				 * shift contributions are stored in a map, whose key is the pair of
				 * properties, to which the splines belong.
				 * The maps are stored per target atom in the same order as the target_names.
				 * The files are specified in the section {\tt EmpiricalShiftHyperSurfaces} 
				 * of the file {\tt ShiftX.ini} and are collected by {\tt init ()}.
				 */
				vector< std::map< std::pair<String, String>, String >	>		property_files_; 
			
				vector< vector< std::pair<String, String> > >  property_pairs_;
				
				/*_ The hypersurfaces are precomputed by {\tt init()}
				 *  and then are stored in this map with key atom_type
				 *  Each entry itself is a map to with a pair of properties as key 
				 */
			
				std::map<String, std::map <std::pair<String, String>,EmpiricalHSShiftProcessor::ShiftHyperSurface_> > hypersurfaces_;
				
				
			private:
					void 			printParameters_() throw();
					void 			printTargets_() throw();
				


	};//End of class
} // end of namespace
#endif // BALL_NMR_EMPIRICALHSSHIFTPROCESSOR_H

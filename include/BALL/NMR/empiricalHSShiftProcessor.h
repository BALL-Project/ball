#ifndef BALL_NMR_EMPIRICALHSSHIFTPROCESSOR_H
#define BALL_NMR_EMPIRICALHSSHIFTPROCESSOR_H

#ifndef BALL_FORMAT_GENERICMOLFILE_H
# include <BALL/FORMAT/genericMolFile.h>
#endif

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
		
	/** nested classe for computing Bicubic splines 
 	*/
		class CubicSpline1D_
		{
			public:
				CubicSpline1D_() {};

				void createSpline(const std::vector<float>& sample_positions, 
						const std::vector<float>& sample_values,bool return_average = false); // spline
				float operator () (float x); // splint

				std::vector<float>& getCurvature();
				void setCurvature(std::vector<float> curvature);
				void setValues(std::vector<float> values);
				void setPositions(std::vector<float> positions);
				float getAverage() {return average_;}
			private :
				std::vector<float> sample_positions_;
				std::vector<float> sample_values_;
				std::vector<float> curvature_;
				bool return_average_;
			  float average_;
		};

//		public: // tO do : rÜCKGÄNGIG
		class CubicSpline2D_
		{
			public:
				CubicSpline2D_() {};

				// Simple version of spline creation. Assumes that all rows have the same x-positions
				void createBiCubicSpline(const std::vector<float>& sample_positions_x,
																 const std::vector<float>& sample_positions_y,
																 const std::vector<std::vector<float> >& sample_values); // splie2
				//precompute the second derivatives	
				void createBiCubicSpline(const std::vector<std::vector<float> >& sample_positions_x,
						const std::vector<float>& sample_positions_y,
						const std::vector<std::vector<float> >& sample_values); // splie2
				float operator () (float x, float y); // splin2
				float getAverage() { return average_; };
			private :
				std::vector< std::vector<float> > sample_positions_x_;
				std::vector<float> sample_positions_y_;
				std::vector<CubicSpline1D_> splines_;

				float average_;
		};

	//	protected:	//todO: RÜCKGÄNGIG


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

				static bool isDiscrete(String property) throw();
				static bool isMixed(String property) throw();

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
				
		}; // end of nested class
		

		/*_ neested class for storing the hypersurfaces of a 
		 * pair of properties. 
		 **/
		
		class ShiftHyperSurface_
		{
			public:
				typedef std::map<String, std::map<String, float> > tabletype;
				
				/**	@name Enums and Constants
				*/
				//@{	
				
				enum HYPERSURFACE__TYPE{
				REAL__REAL,
				REAL__DISCRETE, 	  
				DISCRETE__REAL,		  // not used	
				DISCRETE__DISCRETE,
				CHI__REAL,          // not used
				REAL__CHI,
				CHI__DISCRETE,      
				DISCRETE__CHI,      
				CHI__CHI,           // not used
				SINGLE__REAL,  
				SINGLE__DISCRETE, 
				SINGLE__CHI					// not used
				};

				/*_  Constructor
				 */
				ShiftHyperSurface_() throw();
			
				/*_ Creates a ShiftHyperSurface according to its type (see {\tt HYPERSURFACE__TYPE()} )
				 *  In the special case that the two properties have the same type, we create just one tableentry in 
				 *  {\tt table_} or just one spline  { \tt in s1d_ }. 
				 *  
				 *  Note: 
				 *  The spline or value can be accessed 
				 *  by the {\tt operator () } given __not__ the property-value, but the property name. 
				 *  
				 */
				ShiftHyperSurface_(String filename, String atomtype, String firstproperty, String secondproperty)
					throw();  
				
				/**	Destructor.
				*/
				virtual ~ShiftHyperSurface_() throw();

				/** computes the shift of an atom.  
				 */
				float operator () (PropertiesForShift_& properties) throw();	
				
				void setType_(String firstproperty, String secondproperty)
					throw();

				void readSingleReal_(BALL::File& file, String filename) throw();	
				void readSingleDiscrete_(BALL::File& file, String filename) throw();
				
				bool isvalid() throw(){return !invalid_;}
						
			private:
				float getTableAverage_() throw();
				float getTableRowAverage_(const std::map<String, float>& row) throw();
				float getTableColumnAverage_(const String& name) throw();

				bool tableHasColumn_(const String& name) throw();

				int type_; // Can be REAL__REAL... 
			 	String  first_property_;
				String  second_property_;

				CubicSpline2D_ 															s2d_;
				std::map <String, CubicSpline1D_ > 					s1d_;
				// access to the table first key x, second key y: 
				std::map <String, std::map<String, float> > table_;
				// this flag stores whether the spline we created is valid
				bool invalid_;

				/* These vectors store information about how often a certain key/value pair occurred during sampling.
				 * The values are relevant for the averaging process.
				 */
				vector<vector<int> >                        tcount_values_2d_;
				vector<int>                                 tcount_values_1d_;

				/// the average over the complete hypersurface; tcount-corrected
				float average_;

				vector<float> row_averages_;
				vector<float> col_averages_;
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
				 * The maps are stored per target atom type in the same order as the target_names.
				 * The files are specified in the section {\tt EmpiricalShiftHyperSurfaces} 
				 * of the file {\tt ShiftX.ini} and are collected by {\tt init ()}.
				 */
				vector< std::map< std::pair<String, String>, String >	>		property_files_; 
			
				/*_ The property pairs for each atom type are stored in this map.
				 *  The map is created in {\tt init ()}.
				 **/
				std::map< String, vector< std::pair<String, String> > >  property_pairs_;
				
				/*_ The hypersurfaces are precomputed by {\tt init()}
				 *  and then are stored in this map with key atom_type
				 *  Each entry itself is a map to with a pair of properties as key 
				 */
			
				std::map<String, std::map <std::pair<String, String>,EmpiricalHSShiftProcessor::ShiftHyperSurface_> > hypersurfaces_;
				
				
			private:
					void 			printParameters_() throw();
					void 			printTargets_() throw();
					void			postprocessing_() throw();

	};//End of class
} // end of namespace
#endif // BALL_NMR_EMPIRICALHSSHIFTPROCESSOR_H

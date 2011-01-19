#ifndef BALL_NMR_EMPIRICALHSSHIFTPROCESSOR_H
#define BALL_NMR_EMPIRICALHSSHIFTPROCESSOR_H

#ifndef BALL_MATHS_CUBICSPLINE2D_H
#	include <BALL/MATHS/cubicSpline2D.h>
#endif

#ifndef BALL_MATHS_CUBICSPLINE1D_H
#	include <BALL/MATHS/cubicSpline1D.h>
#endif


#ifndef BALL_FORMAT_GENERICMOLFILE_H
# include <BALL/FORMAT/genericMolFile.h>
#endif

#ifndef BALL_NMR_SHIFT_MODULE_H
#	include<BALL/NMR/shiftModule.h>
#endif

#ifndef BALL_KERNEL_EXPRESSION_H
#	include<BALL/KERNEL/expression.h>
#endif

#ifndef BALL_DATATYPE_OPTIONS_H
# include <BALL/DATATYPE/options.h>
#endif


#include <set>
#include <map>

namespace BALL 
{
	class Atom;
		
	/**	Shift assignment processor implementing the empirical hypersurface contribution. 
	\ingroup ShiftModulesNMR		
	*/
	class BALL_EXPORT EmpiricalHSShiftProcessor
		:	public ShiftModule
	{
		public:

		BALL_CREATE(EmpiricalHSShiftProcessor)
		
		static const int VERBOSITY_LEVEL_CRITICAL;
		static const int VERBOSITY_LEVEL_DEBUG;

		/** @name Constant Definitions
		*/
		//@{
		/// Option names
		struct BALL_EXPORT Option
		{	
			/** verbosity of the processor
			 */
			static const char* VERBOSITY;
			
		};
		
		/// Default values for options
		struct BALL_EXPORT Default
		{
			static const int VERBOSITY;
		};
	
		//@}


		/**	@name Enums and Constants.
		*/
		//@{

		/**	A symbolic name for the empirical hypersurfaces contribution to the chemical shift.
				@see ShiftModule::PROPERTY__SHIFT
		*/
		static const char* PROPERTY__EHS_SHIFT;
	
		//@}
		/** @name	Constructors and Destructors.
		*/
		//@{

		/**	Default constructor.
		*/
		EmpiricalHSShiftProcessor();

		/**	Destructor.
		*/
		virtual ~EmpiricalHSShiftProcessor();

		//@}
		/**	@name	Accessors.
		*/
		//@{
	
		/**	Initialization method.
				This method reads the parameter section <b>SSBondCorrection</b>, parses, and stores its content
				in the vector  \link ssbond_correction_ ssbond_correction_  \endlink. 
				The parameter section <b>SSBondCorrection</b> defines atom types of residues, which have a disulfid bond, and
				the corresponding shift contribution.
				In addition, this method reads the parameter section <b>EmpiricalShiftHyperSurfaces</b> and
				parses its content.
				The  parameter section <b>EmpiricalShiftHyperSurfaces</b> 
				defines target atom types, associated hypersurface property pairs and the corresponding datafiles.
				All target atom types are stored in a vector \link target_names_ target_names_  \endlink.
				All properties relevant for an atom type are stored in a set. The sets are stored per target atom
				in the vector \link target_property_names_ target_property_names_ \endlink
				in the same order as in the vector \link target_names_ target_names\endlink.  
				
				The property pairs for each atom type are stored as pairs in vectors. These vectors are themselves
				stored per atom type in a map \link property_pairs_ property_pairs_ \endlink.
				
				The file names of the hypersurface data files of an atom type are stored in a map,   
				whose key is the pair of properties, to which the hypersurface data belong.
				The maps themselfes are stored per target atom type in the vector 
				\link property_files_  property_files_\endlink the same order as the atom types in 
				the vector \link target_names_ target_names\endlink.  
				
				For each atom type's property combinations a hypersurface is created and stored in 
				the map \link hypersurfaces_ hypersurfaces_ \endlink key {\tt atom type} of maps with 
				key property pair. The hypersurfaces will be evaluated for each target atom 
				in \link finish () finish () \endlink. 

				The flag {\tt exclude_prolins}, denoting the consideration of prolins in the shift computations, 
				is read from this section too. 
				@see finish ()
		*/
		virtual void init();

		//@}
		/**	@name	Processor specific functions.
		*/
		//@{
		
		/**	Processor start method. 
				This method clears the \link targets_ \endlink list. 
				It fails if no parameters were assigned.
				@return bool, <b>false</b> if <tt>parameters_ == 0</tt>
		*/
		virtual bool start();


		/**	operator (). 
		 *  This method collects all target atoms, whose atom type was specified in the section 
			  {\tt EmpiricalShiftHyperSurfaces} of the {\tt ShiftX.ini} - file and stored  in
				\link target_names_ target_names_ \endlink,
				and computes all target atoms properties, which were likewise specified
				for each target atom type in the {\tt ShiftX.ini} - file and stored in 
				\link target_property_names_ target_property_names_ \endlink.
				The  atoms and their properties are stored in the list
				\link targets_ targets_ \endlink .
				@return Processor::CONTINUE
				@param composite an arbitrary composite. All non-atom objects are ignored.
		*/
		virtual Processor::Result operator () (Composite& composite);

		/**	Finish method. 
				This method performs the chemical shift calculation for all
				previously collected target atoms  (\link targets_ targets_ \endlink)
				iteratively by evaluating all 
				atom type relevant hypersurfaces (\link hypersurfaces_ hypersurfaces_ \endlink) 
				iteratively using the precomputed atoms properties (\link targets_ targets_\endlink).
				For disulfid bonds, a special term as specified in section 
				{\tt SSBondCorrection} of the {\tt ShiftX.ini} - file is added. 
				According to the flag {\tt exclude_prolins_} (default false) (pecification in the {\tt ShiftX.ini} - file)
				prolins my by excluded from shift computations.
				If the processor found no target atoms or had no hypersurfaces, 
				the processor returns immediately.
				The  empirical hypersurface related chemical shift is stored for each target atom
				using the named property  \link PROPERTY__EF_SHIFT PROPERTY__EF_SHIFT \endlink and 
				added to the named property  \link ShiftModule::PROPERTY__SHIFT ShiftModule::PROPERTY__SHIFT \endlink .
				@return bool, <b>false</b> if <tt>invalid_ </tt>
		*/
		virtual bool finish();

		//@}
		/** @name Public Attributes
			*/
			//@{
			/// options
			Options options;

			/** reset the options to default values
			*/
			void setDefaultOptions();
			//@}

		protected:
		
		
		/*_ Neested class providing atom properties for the shift computations 
		 * */	
		class BALL_EXPORT PropertiesForShift_
		{
			public:
							
				/** Default constructor.
				 */
				PropertiesForShift_(int verbosity_ = EmpiricalHSShiftProcessor::VERBOSITY_LEVEL_CRITICAL);

				/** A pointer to the atom, whose properties are computed and stored. 
				 */
				Atom*				current_atom;

				/**	 Method to set the atom and to compute all atom's properites as specified in 
				 *   the given set {\tt properties}.
				 *   The property values are stored either in the map {\tt properties_real_} with 
				 *   property type as key or in the map {\tt properties_string_} with property type 
				 *   as key. The values can be accessed via the \link operator [] operator [] \endlink
				 *   given the property type. 
				 *   The angles are given in the range (-180, 180).
				 *   Note: In case of property type CHI, numerical _and_ alphanumeric values are possible:
				 *   			 x e(-180,180) or x e {"ALA", "GLY", "Unknown"} 
				 * 	 If a certain property value is not available, the property gets
				 * 	 the predefined values FLOAT_VALUE_NA or STRING_VALUE_NA. 
				 * 	 This is needed, to access the default values of the hypersurfaces.
				 * 	 
				 *   The method fails if the atom does not belong to a residue.
				 *	 @return bool, <b>false</b> if <tt> !a->getResidue()</tt>
				 **/
				bool computeProperties_(Atom* atom, std::set<String> properties);  

				/**   Method to obtain a property's value. 
				 *    A std::pair<float, String> is returned, such that 
				 *    the first element is std::numeric_limits<float>::min() 
				 *    	if it is a alphanumeric/discrete property value
				 *    the second element is the string "invalid" 
				 *    	if it is a numeric property value.
				 *    NOTE: make sure, that all properties (except from CHI)  are stored either 
				 *     	in properties_real_ or in properties_string_ 
				 *     	otherwise the operator is not able to return the correct value!
				 *    @return pair<float, String> 
				 */
				std::pair<float, String> operator [] (const String& property_name);

				/**  Method to check the type of a property.
				 *   @return bool, <b>false</b> if <tt> property is of type
				 *   	"PSI"  || "PHI" || "HA2L" || "HA1L" || "HNL" || "OHL"|| "CHI" </tt>
				 */
				static bool isDiscrete(String property);
				
				/**  Method to check the type of a property.
				 *   @return bool, <b>false</b> if <tt> property != "CHI" || "CHI2" </tt>
				 */
				static bool isMixed(String property);

			protected:

				// Storing the atoms properties 
				// ...as numerical value
				std::map<String, float>  properties_real_;
				// ... as alphanumerical/discrete value
				std::map<String, String> properties_string_;

			protected:
				
				float			getChiAngle_(Residue* residue);
				float			getChi2Angle_(Residue* residue);
				char 			getAminoAcid_(Residue* residue);
				char 			getSecondaryStructure_(Residue* residue);
				float 		getHA_HBondLen_(Residue* residue);
				float 		getHA2_HBondLen_(Residue* residue);
				float 		getHN_HBondLen_(Residue* residue);
				float 		getO_HBondLen_(Residue* residue);
				bool 			hasDisulfidBond_(Residue* residue);
				bool 			hasHA_HBond_(Residue* residue);
				bool 			hasHA2_HBond_(Residue* residue);
				bool 			hasHN_HBond_(Residue* residue);
				bool 			hasO_HBond_(Residue* residue);
				
			private:
				// verbosity of the classes methods
				int     verbosity_;
		}; // end of nested class
		

		/*_ Neested class handling the empirical hypersurfaces. 
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
					CHI__CHI,           
					SINGLE__REAL,  
					SINGLE__DISCRETE, 
					SINGLE__CHI					
				};

				/*_  Constructors and Destructors.
				*/
				ShiftHyperSurface_(int verbosity = EmpiricalHSShiftProcessor::VERBOSITY_LEVEL_CRITICAL);

				/*_ Detailed constructor. 
				 *  Creates a ShiftHyperSurface given the <b>filename<\b> of the data file, 
				 *  the atom type, and the two property types according to its deduced 
				 *  hypersurface types (see {\tt HYPERSURFACE__TYPE()}).
				 *  
				 *  If the data file cannot be found, no hypersurface is created.
				 *
				 *  In the special case that the two properties have the same type, we create just one table entry in 
				 *  {\tt table_} or just one spline  { \tt in s1d_ }. 
				 *  
				 *  In case of hypersurfaces of type {\tt REAL__DISCRETE} the splines default values are set to the  
				 *  row-averages if given specified in the datafile. 
				 *  
				 *  In case of hypersurfaces of type {\tt  REAL__CHI, REAL__REAL } the splines default values are 
				 *  set to the splines averages. 
				 *
				 * 	In all cases but look-up tables the bounds for the splines are set as specified in 
				 * 	the datafile if specified.
				 * 
				 *  Note: 
				 *  The spline or value can be accessed 
				 *  by the {\tt operator () } given __not__ the property-value, but the property name. 
				 *  
				 */
				ShiftHyperSurface_(String filename, String atomtype, 
													 String firstproperty, String secondproperty, 
													 int verbosity = EmpiricalHSShiftProcessor::VERBOSITY_LEVEL_CRITICAL)
					throw(Exception::FileNotFound);  

				/**	Destructor.
				*/
				virtual ~ShiftHyperSurface_();

				/** Method to evaluate the empirical hypersurface given the properties of an atom.
				 *  In case of a SINGLE__?? property type, the property type string works as 
				 *  first accessor. 
				 *  If only one factor is out of bounds  {/tt FLOAT_VALUE_NA} or {/tt STRING_VALUE_NA},
				 *  we return the all-values average. (We tried to simulate the SHIFTX behaviour)  
				 *
				 *  If one of the property values is set to { /tt FLOAT_VALUE_IGNORE}, 
				 *  zero is returned.
				 */
				float operator () (PropertiesForShift_& properties);	

				/** Method to check if the hypersurface is valid.
				 *   @return bool, <b>false</b> if <tt> hypersurface is invalid.
				 */
				bool isvalid(){return !invalid_;}

				// Returns the total average given in the input file.
				float getTotalAverage() {return average_;}

			private:

				// Computes the average of the look-up table values.
				float getTableAverage_();

				// Computes the row average of the look-up table given a row name.
				float getTableRowAverage_(const std::map<String, float>& row);

				// Computes the column average of the look-up table given a column name.
				float getTableColumnAverage_(const String& name);

				/** Method to check if the hypersurface's look--up table
				 *  has a column named "name".
				 *  @return bool, <b>false</b> if <tt> the look-up table has no column "name".
				 */
				bool tableHasColumn_(const String& name) ;

				/** Method to read the datafile of a shift hypersurface. 
				 *  
				 *  We assume the datafile to have the following structure:

				 atomtype factorx factory
				 total_average
				 (row_average_1;row_average_2;...;row_average_n|N/A)
				 (col_averages_1;col_averages_2;...;col_averages_n|N/A)
				 (row_spacing|N/A)
				 (col_spacing|N/A)
				 (y_axis_1;...;y_axis_n|N/A)
				 x_axis_11;...;x_axis_1m
				 value_11;...;value_1m
				 ...
				 x_axis_n1;...;x_axis_nm
				 value_n1;...;value_nm

				 where  ( | ) denotes alternatives and "N/A" an empty entry. 
				 */ 
				void parseDataFile_(BALL::File& file, String filename) ;

				// Method to convert a vector of strings into a vector of floats {/tt output}.
				void convertToReal_(const vector<String>& input, vector<float>& output);

				/** Method to set the type of the hypersurface according to two input property types 
				 * (see {\tt HYPERSURFACE__TYPE()} )
				 */
				void setType_(String firstproperty, String secondproperty);

				// The type of the hypersurface.
				int type_;

				// The properties of the hypersurface. 
				String  first_property_; 
				String  second_property_;

				// The hypersurface can be a 2D spline, 1D spline or a look-up table.
				CubicSpline2D 							      					s2d_;
				std::map <String, CubicSpline1D > 					s1d_;
				// Note: access of the look--up table: first the discrete key x, second the numerical key y. 
				std::map <String, std::map<String, float> > table_;

				// The averages of the hypersurface as read from the file.
				// //TODO rename to row_defaults
				std::map <String, float> 										row_averages_;
				std::map <String, float>										col_averages_;

				// The spacing in x and y direction as read from the file.
				float row_spacing_;
				float col_spacing_;

				// Flag to store whether the spline we created is valid.
				bool invalid_;

				// The average over the complete hypersurface as read from the file.
				// Note: this average maybe weighted in non-obvious ways! 
				float average_;

				// verbosity of the method
				int verbosity_;

				// The underlying data of the hypersurface as read from the file.
				vector<String> 					 y_axis_values_;
				vector<vector<String> >  x_axis_values_;
				vector<vector<float> >   sample_values_;
		};




		/*_	The target atoms collected by {\tt operator ()}.
		*/
		std::vector<PropertiesForShift_> 				targets_;

		/*_ The target atom types stored as a vector of strings collected from the ini-file
		 *  by {\tt init ()}.
		 */
		vector<String>											target_names_;	

		/*_ The target properties used for determine the 
		 * empirical hypersurface shifts. The properties are collected 
		 * from the ini-file by {\tt init ()} and are stored per target atom
		 * in the same order as the target_names.
		 */
		vector< std::set<String> >			target_property_names_;

		/*_ The file names of the files storing the property-data-splines for computing the
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
		 *  and then are stored per atom type in a  map with key atom_type of 
		 *  maps with the property pair as key.
		 */

		std::map<String, std::map <std::pair<String, String>,EmpiricalHSShiftProcessor::ShiftHyperSurface_> > hypersurfaces_;

		/*_ Option to exclude prolins from the shift computations. 
			Default is false.
			*/
		bool exclude_prolins_;

		// Map containing possible correction terms for ssbonds.
		std::map<String, float> ssbond_correction_;

	private:

		/*_ Some debugging functions printing parameter/effector/target information
		 *   to the Log stream.
		 */
		void 			printParameters_();
		void 			printTargets_();	

		/*_ A function to perform some ShiftX-y postprocessing: 
			add for all CA-atoms 0.2 times the EF-shift-value of the bound HA-atoms.
			Due to some reason ShiftX does not perform this postprocessing 
			for the empirical Hypersurface contributions.
			*/
		void			postprocessing_();

		/// The verbosity of this class as taken from the options
		int 			verbosity_;

	};// end of class
} // end of namespace
#endif // BALL_NMR_EMPIRICALHSSHIFTPROCESSOR_H

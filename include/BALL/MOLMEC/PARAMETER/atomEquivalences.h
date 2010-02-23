#ifndef BALL_MOLMEC_PARAMETER_ATOMEQUIVALENCES_H
#define BALL_MOLMEC_PARAMETER_ATOMEQUIVALENCES_H

#ifndef BALL_FORMAT_PARAMETERSECTION_H
#	include <BALL/FORMAT/parameterSection.h>
#endif

#ifndef BALL_KERNEL_ATOM_H
#	include <BALL/KERNEL/atom.h>
#endif

#ifndef BALL_SYSTEM_FILE_H
# include <BALL/SYSTEM/file.h>
#endif

#ifndef BALL_MOLMEC_PARAMETER_ATOMTYPES_H
# include <BALL/MOLMEC/PARAMETER/atomTypes.h>
#endif

namespace BALL 
{
	class ForceFieldEquivalences;

	/**	Force Field Atom Type Class. 
		
			Molecular Mechanics Equivalences: class describing the equivalent atom types section of a parameter file.
    	\ingroup  MolmecParameters
	*/
	class BALL_EXPORT AtomEquivalences
		:	public ParameterSection
	{
			public:

		/**	@name	Constructors and Destructors
		*/
		//@{
		
		/**	Default constructor.
		*/
		AtomEquivalences(ForceField* force_field=0) ;

		/**	Copy constructor.
		*/
		AtomEquivalences(const AtomEquivalences& atom_type_equivalences) ;

		/**	Destructor.
		*/
		virtual ~AtomEquivalences() ;

		//@}
		/**	Parameter extraction
		*/
		//@{

		/**	Reads a parameter section from an INI file.
				This method reads the section given in section_name from ini_file,
				interprets (if given) a format line, reads the data from this section according to 
				the format, and builds some datastructures for fast and easy acces this data.
		*/
		virtual bool extractSection(Parameters& parameters, 
				const String& section_name = "AtomEquivalences") ;

		/**	Returns the numeric reference type for a given string.
		*/
		Atom::Type getReferenceType(const String& name) const ;

		/**	Returns the name of the reference type of a given index 
		*/
		String getReferenceTypeName(Atom::Type type) const ;

		/**	Queries whether two given atom types are equivalent
		*/
		bool isEquivalent(const String& name_a, const String& name_b) const ;

		/**	Queries whether two given atom types are equivalent
		*/
		bool isEquivalent(const Atom::Type type_a, const Atom::Type type_b) const ;

		/** TODO DO we need this	
		*/
		//Size getNumberOfEquivalentTypes() const ;
		//Size getNumberOfEquivalentTypes((const String& name_a) const ;
		//Size getNumberOfEquivalentTypes((const Atom::Type type_a) const ;
	
		/** Return a pointer to the forcefield
		 */
		ForceField* getForceField();


		/** Write the force field parameter in Parm-file format
		 */
		virtual bool exportParmFile(File& outfile) const;

		//@}
		/**	@name	Assignment
		*/
		//@{

		/** Clear method 
		*/
		virtual void clear() ;

		/**	Assignment operator.
				@param	atom_equivalences the atom types object to be copied
		*/
		AtomEquivalences& operator = (const AtomEquivalences& atom_equivalences) ;

		//@}
		/** @name Predicates 
		*/
		//@{

		/** Equality operator 
		*/
		bool operator == (const AtomEquivalences& atom_equivalences) const ;

		//@}

		protected:
		
		/*_	Contains the numeric types for each atom type string.
		*/
		StringHashMap<Atom::Type>				reference_type_map_;
		
		/*_ Contains the names of each equivalent atom type per reference atom type name
		 */	
		StringHashMap<vector<String> > 	equivalent_types_;  
		
		/*_ A Pointer to the forcefield 
		 */
		ForceField* 										force_field_;

	};
} // namespace BALL

#endif // BALL_MOLMEC_PARAMETER_ATOMEQUIVALENCES_H

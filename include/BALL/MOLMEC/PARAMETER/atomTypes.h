// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_MOLMEC_PARAMETER_ATOMTYPES_H
#define BALL_MOLMEC_PARAMETER_ATOMTYPES_H

#ifndef BALL_FORMAT_PARAMETERSECTION_H
#	include <BALL/FORMAT/parameterSection.h>
#endif

#ifndef BALL_KERNEL_ATOM_H
#	include <BALL/KERNEL/atom.h>
#endif

#define	BALL_ATOM_WILDCARD_NAME "*"
#define	BALL_ATOM_UNKNOWN_NAME "?"

namespace BALL 
{
	class ForceFieldParameters;

	/**	Force Field Atom Type Class. 
			
			Molecular Mechanics Parameter: class describing the atom type section of a parameter file.
    	\ingroup  MolmecParameters
	*/
	class BALL_EXPORT AtomTypes
		:	public ParameterSection
	{
		public:

		/**	@name	Constructors and Destructors
		*/
		//@{
		
		/**	Default constructor.
		*/
		AtomTypes() ;

		/**	Copy constructor.
		*/
		AtomTypes(const AtomTypes& atom_types) ;

		/**	Destructor.
		*/
		virtual ~AtomTypes() ;

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
				const String& section_name = "AtomTypes") ;

		/**	Returns the numeric type for a given string.
		*/
		Atom::Type getType(const String& name) const ;

		/**	Queries whether a given atom type is defined
		*/
		bool hasType(const String& name) const ;

		/**	Returns the name for a given index 
		*/
		String getTypeName(Atom::Type type) const ;

		/**	
		*/
		Size getNumberOfTypes() const ;

		//@}
		/**	@name	Assignment
		*/
		//@{

		/** Clear method 
		*/
		virtual void clear() ;

		/**	Assignment operator.
				@param	atom_types the atom types object to be copied
		*/
		AtomTypes& operator = (const AtomTypes& atom_types) ;

		//@}
		/** @name Predicates 
		*/
		//@{

		/** Equality operator 
		*/
		bool operator == (const AtomTypes& atom_types) const ;

		//@}

		protected:
		
		/*_	Contains the numeric types for each atom type string.
		*/
		StringHashMap<Atom::Type>	type_map_;

		/*_ Contains the symbolic names of the atom types.
		*/
		vector<String>						names_;
	};
} // namespace BALL

#endif // BALL_MOLMEC_PARAMETER_ATOMTYPES_H

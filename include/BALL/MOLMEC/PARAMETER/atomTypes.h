// $Id: atomTypes.h,v 1.2 2000/02/14 09:37:57 oliver Exp $
// Molecular Mechanics Parameter: class describing the atom type section of a parameter file

#ifndef BALL_MOLMEC_PARAMETER_ATOMTYPES_H
#define BALL_MOLMEC_PARAMETER_ATOMTYPES_H

#ifndef BALL_FORMAT_PARAMETERSECTION_H
#	include <BALL/FORMAT/parameterSection.h>
#endif

#define	BALL_ATOM_WILDCARD_NAME "*"
#define	BALL_ATOM_UNKNOWN_NAME "?"

namespace BALL 
{
	
	/**	Force Field Atom Type Class.	
			{\bf Definition:} \URL{BALL/MOLMEC/PARAMETER/atomTypes.h}
			\\
	*/
	class AtomTypesSection
		:	public ParameterSection
	{
		public:

		/**	@name	Constructors and Destructors
		*/
		//@{
		
		/**	Default constructor.
		*/
		AtomTypesSection();

		/**	Copy constructor.
		*/
		AtomTypesSection(const AtomTypesSection& atom_types, bool deep = true);

		/**	Destructor.
		*/
		virtual ~AtomTypesSection();

		/**	Destroy method.
		*/
		virtual void destroy();

		//@}
		
		/**	Parameter extraction
		*/
		//@{

		/**	Reads a parameter section from an INI file.
				This method reads the section given in section\_name from ini\_file,
				interprets (if given) a format line, reads the data from this section according to 
				the format, and builds some datastructures for fast and easy acces this data.
		*/
		virtual bool extractSection(ForceFieldParameters& parameters, const String& section_name);

		/**	Returns the numeric type for a given string.
		*/
		Atom::Type getType(const String& name) const;

		/**	Queries whether a given atom type is defined
		*/
		bool hasType(const String& name) const;

		/**	Returns the name for a given index 
		*/
		String getTypeName(Atom::Type type) const;

		/**	
		*/
		Size getNumberOfTypes() const;
		//@}

		/**	@name	Assignment
		*/
		//@{

		/**	Assignment operator.
				copy the contents of {\tt atom\_types} into {\tt this}
				atom types object.
				@param	atom_types the atom types object to be copied
		*/
		AtomTypesSection& operator = (const AtomTypesSection& atom_types);
		//@}
		protected:
		
		/*_	Contains the numeric types for each atom type string
		*/
		StringHashMap<Atom::Type>	type_map_;

		vector<String>						names_;
	};

} // namespace BALL

#endif // BALL_MOLMEC_PARAMETER_ATOMTYPES_H

// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

// Molecular Mechanics Parameter: class describing the atom type section of a parameter file

#ifndef BALL_MOLMEC_PARAMETER_TEMPLATES_H
#define BALL_MOLMEC_PARAMETER_TEMPLATES_H

#ifndef BALL_FORMAT_PARAMETERSECTION_H
#	include <BALL/FORMAT/parameterSection.h>
#endif

#ifndef BALL_KERNEL_SYSTEM_H
#	include <BALL/KERNEL/system.h>
#endif

#ifndef BALL_DATATYPE_HASHSET_H
# include <BALL/DATATYPE/hashset.h>
#endif

#include <BALL/KERNEL/atom.h>

namespace BALL 
{
	class Atom;

	/**	Force Field Residue Template Class.	
    	\ingroup  MolmecParameters
	*/
	class BALL_EXPORT Templates 
		:	public ParameterSection
	{
		public:

		/**	@name	Constructors and Destructors
		*/
		//@{
		
		/**	Default constructor.
		*/
		Templates();

		/**	Copy constructor.
		*/
		Templates(const Templates& templates, bool deep = true);

		/**	Destructor.
		*/
		virtual ~Templates() ;

		/**	Clear method. 
		*/
		virtual void clear() ;

		//@}
		/**	Parameter extraction
		*/
		//@{

		/**	Reads a parameter section from an INI file.
				This method reads the section given in section_name from ini_file,
				interprets (if given) a format line, reads the data from this section according to 
				the format, and builds some datastructures for fast and easy acces this data.
				The section is typically entitled <b>ChargesAndTypeNames</b>.
		*/
		virtual bool extractSection(Parameters& parameters, const String& section_name);

		/**	Returns the numeric type for a given string.
		*/
		float getCharge(const String& name) const;

		/**	Returns the name for a given index 
		*/
		String getTypeName(const String& name) const;

		/**	Queries whether a given template atom is defined
		*/
		bool has(const String& name) const;

		/**	Assign charges and type names
		*/
		void assign(System& system, bool overwrite_existing_typenames = true, 
								bool overwrite_non_zero_charges = true) const;

		/**	Assign charges and type names
		*/
		void assignCharges(System& system, bool overwrite_non_zero_charges = true) const;

		/**	Assign type names
		*/
		void assignTypeNames(System& system, bool overwrite_existing_typenames = true) const;

		//@}
		/**	@name	Assignment
		*/
		//@{

		/**	Assignment operator.
				Copy the contents of <tt>templates</tt> into <tt>this</tt> residue template object.
				@param	templates the residue template object to be copied
		*/
		Templates& operator = (const Templates& templates);

		/** Set the number of atoms, for which the assignment can
		    fail, until the assign() methods aborts and return false.
				By default, there is no limit set.
		*/
		void setMaximumUnassignedAtoms(Size nr);

		/** Get the number of atoms, for which the assignment can
		    fail, until the assign() methods aborts and return false.
		*/
		Size getMaximumUnassignedAtoms() const;

		/// Get the number of atoms, for which the assignment failed.
		Size getNumberOfUnassignedAtoms() const;

		/// Get the atoms, for which the assignment failed.
		HashSet<const Atom*>& getUnassignedAtoms();

		//@}

		protected:
		
		/*_	Contains the charge for each residue/atom combination
		*/
		StringHashMap<float>	charges_;

		/*_	Contains the atom type names for each residue/atom combination
		*/
		StringHashMap<String>	type_names_;

		//_ Atoms, for which the assignment fails
		HashSet<const Atom*> unassigned_atoms_;

		//_ max number of unassigned atoms
		Size max_number_unassigned_atoms_;
	};
} // namespace BALL

#endif // BALL_MOLMEC_PARAMETER_TEMPLATES_H

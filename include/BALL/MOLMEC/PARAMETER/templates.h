// $Id: templates.h,v 1.4 2000/10/05 17:34:19 anker Exp $
// Molecular Mechanics Parameter: class describing the atom type section of a parameter file

#ifndef BALL_MOLMEC_PARAMETER_TEMPLATES_H
#define BALL_MOLMEC_PARAMETER_TEMPLATES_H

#ifndef BALL_FORMAT_PARAMETERSECTION_H
#	include <BALL/FORMAT/parameterSection.h>
#endif

#ifndef BALL_KERNEL_SYSTEM_H
#	include <BALL/KERNEL/system.h>
#endif

namespace BALL 
{

	/**	Force Field Residue Template Class.	
			{\bf Definition:} \URL{BALL/MOLMEC/PARAMETER/templates.h}
			\\
	*/
	class Templates 
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
		virtual ~Templates();

		//@}
		
		/**	Parameter extraction
		*/
		//@{

		/**	Reads a parameter section from an INI file.
				This method reads the section given in section\_name from ini\_file,
				interprets (if given) a format line, reads the data from this section according to 
				the format, and builds some datastructures for fast and easy acces this data.
				The section is typically entitled {\bf ChargesAndTypeNames}.
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
		void assign(System& system, bool overwrite_existing_typenames = true, bool overwrite_non_zero_charges = true) const;

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

		/**	Clear method. */
		virtual void clear();

		/**	Assignment operator.
				copy the contents of {\tt templates} into {\tt this}
				residue template object.
				@param	templates the residue template object to be copied
		*/
		Templates& operator = (const Templates& templates);

		//@}


		protected:
		
		/*_	Contains the charge for each residue/atom combination
		*/
		StringHashMap<float>	charges_;

		/*_	Contains the atom type names for each residue/atom combination
		*/
		StringHashMap<String>	type_names_;
	};

} // namespace BALL

#endif // BALL_MOLMEC_PARAMETER_TEMPLATES_H

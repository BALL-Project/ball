// $Id: forceFieldParameters.h,v 1.1 1999/08/26 07:53:21 oliver Exp $
// Molecular Mechanics: general force field parameter class

#ifndef BALL_MOLMEC_FORCEFIELDPARAMETERS_H
#define BALL_MOLMEC_FORCEFIELDPARAMETERS_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_FORMAT_INIFILE_H
# include <BALL/FORMAT/INIFile.h>
#endif

#ifndef BALL_KERNEL_SYSTEM_H
# include <BALL/KERNEL/system.h>
#endif

#ifndef BALL_DATATYPE_OPTIONS_H
# include <BALL/DATATYPE/options.h>
#endif

#ifndef BALL_MOLMEC_PARAMETER_ATOMTYPES_H
# include <BALL/MOLMEC/PARAMETER/atomTypes.h>
#endif

namespace BALL 
{

	/**	Force field parameter class.
	*/
	class ForceFieldParameters
	{
		public:

		friend class ForceField;

		/**	Error codes of the ForceFieldParameters class
		*/
		enum ErrorCode 
		{
			ERROR__NONE	= -1,
			ERROR__UNKNOWN,
			ERROR__NOT_IMPLEMENTED,

			NUMBER_OF_ERRORS
		};

		/**@name	Constructors and destructor	*/
		//@{

		/**	Default constructor.
		*/
		ForceFieldParameters(void);

		/**	Constructor.
		*/
		ForceFieldParameters(const String& filename);

		/**	Copy constructor
		*/
		ForceFieldParameters(const ForceFieldParameters& force_field_parameter, bool deep = true);

		/**	Destructor.
		*/
		virtual ~ForceFieldParameters(void);

		//@}

		/**@name	Accessors 	*/
		//@{

		/**	Sets the filename for the INI file.
		*/
		void setFilename(const String& filename);

		/**	Return the current INI file's name.
		*/
		const String& getFilename(void) const;

		/**	Return a reference to the INI file.
		*/
		INIFile& getParameterFile();
			
		/**	Return a reference to the atom type parameter section
		*/
		FFPSAtomTypes&	getAtomTypes();

		/**	Read the contents of the INI file and interpret them.
		*/
		bool init();
		//@}

		/**	@name	Predicates
		*/
		//@{
			
		/**	Valididty predicate.
				Return {\bf true} if the force field parameters were correctly
				initialized, the internal INI file is valid and the internal atom types		
				object is valid.
				@return {\tt valid\_ \&\& parameter\_file\_.isValid() \&\& atom\_types\_.isValid()}
		*/
		bool isValid() const;
		//@}

		protected:

		/*_@name	Protected Members */
		//_@{ 

		/*_
		*/
		bool		valid_;

		/*_	the INIFile
		*/
		INIFile	INI_file_;

		/*_	the atom types section
		*/
		FFPSAtomTypes	atom_types_;
		//_@} 
	};

} // namespace BALL

#endif // BALL_MOLMEC_FORCEFIELDPARAMETERS_H

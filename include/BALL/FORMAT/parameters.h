// $Id: parameters.h,v 1.3 2000/09/18 12:15:28 oliver Exp $
// Molecular Mechanics: general force field parameter class

#ifndef BALL_FORMAT_PARAMETERS_H
#define BALL_FORMAT_PARAMETERS_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_FORMAT_INIFILE_H
# include <BALL/FORMAT/INIFile.h>
#endif

namespace BALL 
{

	/**	General Parameter class.
			This class is intended to simplify parameter management 
			for force fields or other stronlgy parameterized methods.
			It is based on \Ref{INIFile}.
			\\
			{\bf Definition:}\URL{BALL/include/FORMAT/parameters.h}
			\\
	*/
	class Parameters
	{
		public:

		/**@name	Constructors and destructor	*/
		//@{

		/**	Default constructor.
		*/
		Parameters();

		/**	Constructor.
		*/
		Parameters(const String& filename);

		/**	Copy constructor
		*/
		Parameters(const Parameters& force_field_parameter, bool deep = true);

		/**	Destructor.
		*/
		virtual ~Parameters();

		/**	Clear method.
		*/
		virtual void clear();

		//@}

		/**@name	Accessors 	
		*/
		//@{

		/**	Sets the filename for the INI file.
		*/
		void setFilename(const String& filename);

		/**	Return the current INI file's name.
		*/
		const String& getFilename() const;

		/**	Return a reference to the INI file.
		*/
		INIFile& getParameterFile();
			
		/**	Read the contents of the INI file and interpret them.
		*/
		bool init();
		//@}

		/**	@name	Predicates
		*/
		//@{
			
		/**	Validity predicate.
				Return {\bf true} if the parameters were correctly
				initialized, the internal INI file is valid.		
				@return bool - {\tt valid_ && parameter_file_.isValid()}
		*/
		virtual bool isValid() const;
		//@}

		protected:

		/*_	@name	Protected Members */
		//_@{ 

		/*_
		*/
		bool		valid_;

		/*_	the INIFile
		*/
		INIFile	INI_file_;
		//_@} 
	};

} // namespace BALL

#endif // BALL_FORMAT_PARAMETERS_H

// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: parameters.h,v 1.11 2003/02/25 15:53:08 sturm Exp $
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
    /** \ingroup General
     *  @{
     */
	/**	General Parameter class.
			This class is intended to simplify parameter management 
			for force fields or other stronlgy parameterized methods.
			It is based on  \link INIFile INIFile \endlink .	 \par
			<b>Definition:</b>BALL/FORMAT/parameters.h
	*/
	class Parameters
	{
		public:

		BALL_CREATE(Parameters)

		/** @name	Constructors and destructor	
		*/
		//@{

		/**	Default constructor.
		*/
		Parameters();

		/**	Constructor.
		*/
		Parameters(const String& filename);

		/**	Copy constructor
		*/
		Parameters(const Parameters& force_field_parameter);

		/**	Destructor.
		*/
		virtual ~Parameters();

		/**	Clear method.
		*/
		virtual void clear()
			throw();

		//@}
		/**	@name	Assignment 
		*/
		//@{

		/**	Assignment operator 
		*/
		const Parameters& operator = (const Parameters& parameters);

		//@}
		/** @name	Accessors 	
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
				Return <b>true</b> if the parameters were correctly
				initialized, the internal INI file is valid.		
				@return bool - <tt>valid_ && parameter_file_.isValid()</tt>
		*/
		virtual bool isValid() const;

		/** Equality operator 
		*/
		bool operator == (const Parameters& parameters) const;

		//@}

		protected:

		/*_	@name	Protected Members 
		*/
		//_@{ 

		/*_
		*/
		bool		valid_;

		/*_	the INIFile
		*/
		INIFile	INI_file_;
		//_@} 
	};
  /** @} */
} // namespace BALL

#endif // BALL_FORMAT_PARAMETERS_H

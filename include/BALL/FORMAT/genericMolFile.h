// $Id: genericMolFile.h,v 1.2 2000/10/23 23:31:01 amoll Exp $

#ifndef BALL_FORMAT_GENERICMOLFILE_H
#define BALL_FORMAT_GENERICMOLFILE_H

#ifndef BALL_COMMON_H
# include <BALL/common.h>
#endif

#ifndef BALL_SYSTEM_FILE_H
#	include <BALL/SYSTEM/file.h>
#endif

namespace BALL 
{
	class Atom;
	class System;

	/**	Base class for all molecule file format classes.
			\\
			{\bf Definition:} \URL{BALL/FORMAT/genericMolFile.h}
			\\
	*/
	class GenericMolFile
		: public File
	{
		public:

		/**	@name	Constructors and Destructors
		*/
		//@{

		/**	Default constructor
		*/
		GenericMolFile();

		/** Detailed constructor.
		*/
		GenericMolFile(const String& filename, File::OpenMode open_mode = std::ios::in);

		/**	Copy constructor
		*/
		GenericMolFile(const GenericMolFile& file);

		/// Destructor
		virtual ~GenericMolFile();
		
		//@}

		/**	@name Reading and Writing of Kernel Datastructures
		*/
		//@{
		
		/**	Write a system to the HIN file
		*/
		virtual void write(const System&	system);
		
		/**	Read a system from the HIN file
		*/
		virtual void read(System&	system);

		/**	Read a system from a HIN file.
		*/
		virtual GenericMolFile& operator >> (System& system);
		
		/**	Write a system to a HIN file.
		*/
		virtual GenericMolFile& operator << (const System& system);
		
		//@}

		/**	@name	Accessors
		*/
		//@{
				
		/**	Check for a periodic boundary in the file.
		*/
		bool hasPeriodicBoundary() const;

		/**	Return the periodic boundary of the file.
				an emptry box is returned if no periodic boundary is defined.
				@return	the boundary box
		*/
		Box3	getPeriodicBoundary() const;

		/**	Return the temperature stored in the file.
				HIN files may contain a {\tt sys} entry containing
				the temperature of the last simulation step. If it is set,
				it is returned. Otwerwise 0 is returned.
				@return	the final simulation temperature
		*/
		float	getTemperature() const;
		//@}

		protected:
		
		Box3		box_;
		float		temperature_;
	};

} // namespace BALL

#endif // BALL_FORMAT_GENERICMOLFILE_H

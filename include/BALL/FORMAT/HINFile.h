// $Id: HINFile.h,v 1.10 2001/01/24 11:51:20 anker Exp $

#ifndef BALL_FORMAT_HINFILE_H
#define BALL_FORMAT_HINFILE_H

#ifndef BALL_COMMON_H
# include <BALL/common.h>
#endif

#ifndef BALL_SYSTEM_FILE_H
#	include <BALL/SYSTEM/file.h>
#endif

#ifndef BALL_DATATYPE_HASHMAP_H
# include <BALL/DATATYPE/hashMap.h>
#endif

#ifndef BALL_MATHS_BOX_H
# include <BALL/MATHS/box3.h>
#endif

namespace BALL 
{
	class System;
	class Atom;

	/**	HyperChem file class.
			This class enables BALL to read and write HyperChem HIN files.
			\\
			{\bf Definition:} \URL{BALL/FORMAT/HINFile.h}
			\\
	*/
	class HINFile
		: public File
	{
		public:

		BALL_CREATE(HINFile)

		/**	@name	Constructors and Destructors
		*/
		//@{

		/**	Default constructor
		*/
		HINFile();

		/** Detailled constructor.
		*/
		HINFile(const String& filename, File::OpenMode open_mode = File::IN);

		/**	Copy constructor
		*/
		HINFile(const HINFile& file);

		/// Destructor
		virtual ~HINFile();
		
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
		virtual HINFile& operator >> (System& system);
		
		/**	Write a system to a HIN file.
		*/
		virtual HINFile& operator << (const System& system);
		
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
	
		void writeAtom_(const Atom& atom, Size number, Size atom_offset);
	};

} // namespace BALL

#endif // BALL_FORMAT_HINFILE_H

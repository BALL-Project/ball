// $Id: XYZFile.h,v 1.3 2000/07/12 19:30:56 oliver Exp $

#ifndef BALL_FORMAT_XYZFILE_H
#define BALL_FORMAT_XYZFILE_H

#ifndef BALL_COMMON_H
# include <BALL/common.h>
#endif

#ifndef BALL_SYSTEM_FILE_H
#	include <BALL/SYSTEM/file.h>
#endif

#ifndef BALL_KERNEL_SYSTEM_H
# include <BALL/KERNEL/system.h>
#endif

namespace BALL 
{

	/**	XYZ file class.
			This class enables BALL to read and write XMol XYZ files.
			The XYZ format is a very simple molecular file format. It contains
			only the atom type (i.e., the element) and the cartesian coordinates
			of the structure. Bonds, atom names, and structural information is
			not part of this file format.\\	
			The first line of oeach XYZ file contains a single integer number: the number
			of atoms in the file. The second line is just a comment line. When reading a 
			XYZ file, BALL stores this comment as the name attribute of the system read.
			Similarly, on writing the systems's name is written to this comment line.
			All remaining lines contain the element symbol and the three coordinates
			in free format.
			\\
			{\bf Definition:} \URL{BALL/FORMAT/XYZFile.h}
			\\
	*/
	class XYZFile
		: public File
	{
		public:

		/**	@name	Constructors and Destructors
		*/
		//@{

		/**	Default constructor
		*/
		XYZFile();

		/** Detailed constructor.
				Create a XYZ file and open it with mode {\tt open_mode} (reading is default)
				@param filename the filename
				@param open_mode the openmode - default is \Ref{File::IN}
		*/
		XYZFile(const String& filename, File::OpenMode open_mode = File::IN);

		/**	Copy constructor
		*/
		XYZFile(const XYZFile& file);

		/// Destructor
		virtual ~XYZFile();
		
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

		/**
		*/
		virtual XYZFile& operator >> (System& system)
		{
			read(system);
			
			return *this;
		}
		
		/**
		*/
		virtual XYZFile& operator << (const System& system)
		{
			write(system);
			
			return *this;
		}
		
		//@}
	};

} // namespace BALL

#endif // BALL_FORMAT_XYZFILE_H

// $Id: bruker1DFile.h,v 1.13 2001/12/30 13:28:37 sturm Exp $

#ifndef BALL_FORMAT_BRUKER1DFILE_H
#define BALL_FORMAT_BRUKER1DFILE_H

#include <vector>

#ifndef BALL_SYSTEM_FILESYSTEM_H
#       include <BALL/SYSTEM/fileSystem.h>
#endif

#ifndef BALL_SYSTEM_FILE_H
#	include <BALL/SYSTEM/file.h>
#endif

#ifndef BALL_DATATYPE_STRING_H
#	include <BALL/DATATYPE/string.h>
#endif

#ifndef BALL_FORMAT_JCAMPFILE_H
#       include <BALL/FORMAT/JCAMPFile.h>
#endif

#ifndef BALL_DATATYPE_REGULARDATA1D_H
#       include <BALL/DATATYPE/regularData1D.h>
#endif

namespace BALL
{
	/**	Bruker 1D spectrum format.
			This class....	\\
			{\bf Definition:}\URL{BALL/FORMAT/bruker1DFile.h}
	*/
	// ?????
	class Bruker1D 
		: public File
	{
		public:

		/**	@name	Constructors and Destructors
		*/
		//@{

		/**	Constructor
		*/
    Bruker1D();

		/**	Constructor.
				@param name important: name of the Bruker-*directory*
		*/
		Bruker1D(const String& name, OpenMode open_mode = File::IN|File::BINARY)
			throw(Exception::FileNotFound);

		/// Copy constructor
		Bruker1D(const Bruker1D& file)
			throw(Exception::FileNotFound);

		/// Destructor
		virtual ~Bruker1D()
			throw();

		//@}


		/**	@name Accessors
		*/
		//@{

		/**	Read a spectrum from {\tt name}. 
				It will be stored in spectrum_
		*/
		void read(const String &name);

		void read();

	  /** Return a pointer to the spectrum.
		 */
		RegularData1D* getData();

		/**
		*/
		JCAMPFile* getParameters();

		//@}

		protected:

		/**
		 * This class gives access to the parameters used in
		 * acquiring this spectrum.
		 */
		JCAMPFile* pars_;
		
		Size min_;
		Size max_;

		RegularData1D spectrum_;
	};
}

#endif // BALL_FORMAT_BRUKER1DFILE_H

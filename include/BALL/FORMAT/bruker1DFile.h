// $Id: bruker1DFile.h,v 1.5 2000/11/10 17:15:04 anhi Exp $

#ifndef BALL_FORMAT_BRUKER1DFILE_H
#define BALL_FORMAT_BRUKER1DFILE_H

#include <vector>
#include <glib.h>

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

namespace BALL
{
	/**	Bruker 1D spectrum format.
			This class....
			\\
			{\bf Definition:}\URL{BALL/FORMAT/bruker1DFile.h}
			\\
	*/
	class Bruker1D 
		: public File
	{
		public:
		/**	@name	Constructors and Destructors
		*/
		//@{

		/**	Constructor.
				@param name important: name of the Bruker-*directory*
		*/
		Bruker1D(const String& name, OpenMode open_mode = File::IN|File::BINARY);

		/// Copy constructor
		Bruker1D(const Bruker1D& file);

		/// Destructor
		virtual ~Bruker1D();
		//@}

		/**	@name Accessors
		*/
		//@{

		/**	Read a 1D-spectrum to dat.
		*/
		void read(std::vector<double>& dat);

		/**
		*/
		JCAMPFile* getParameters();
		//@}

		protected:

<<<<<<< bruker1DFile.h
		/**
		 * This class gives access to the parameters used in
		 * acquiring this spectrum.
		 */
		JCAMPFile* pars_;
=======
		/**	This class gives access to the parameters 
				used in acquiring this spectrum.
		*/
		BrukerParameter* pars_;
>>>>>>> 1.3
		
		Size min_;
		Size max_;
	};
}

#endif // BALL_FORMAT_BRUKER1DFILE_H

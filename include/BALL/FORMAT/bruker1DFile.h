// $Id: bruker1DFile.h,v 1.2 2000/10/05 22:26:58 oliver Exp $

#include <vector>
#include <glib.h>

#ifndef BALL_FORMAT_BRUKER1DFILE_H
#define BALL_FORMAT_BRUKER1DFILE_H

#ifndef BALL_SYSTEM_FILE_H
#	include <BALL/SYSTEM/file.h>
#endif

#ifndef BALL_DATATYPE_STRING_H
#	include <BALL/DATATYPE/string.h>
#endif

namespace BALL
{

	class BrukerParameter;

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
		/**
		 * Konstruktor. Wichtig: name ist hier der Name des Bruker-*Verzeichnisses*
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
		/**
		 * Einlesen eines 1D-Spektrums nach dat.
		 */
		void read(std::vector<double>& dat );

		/**
		*/
		BrukerParameter* getParameters();
		//@}

		protected:

		/**
		 * This class gives access to the parameters used in
		 * acquiring this spectrum.
		 */
		BrukerParameter* pars_;
		
		Size min_;

		Size max_;
	};
}

#endif // BALL_FORMAT_BRUKER1DFILE_H

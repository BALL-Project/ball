// $Id: JCAMPFile.h,v 1.1 2000/09/14 12:23:23 oliver Exp $

#ifndef BALL_FORMAT_JCAMPFILE_H
#define BALL_FORMAT_JCAMPFILE_H

#ifndef BALL_SYSTEM_FILE_H
#	include <BALL/SYSTEM/file.h>
#endif

#ifndef BALL_DATATYPE_STRING_H
#	include <BALL/DATATYPE/string.h>
#endif

#ifndef BALL_DATATYPE_STRINGHASHMAP_H
#	include <BALL/DATATYPE/stringHashMap.h>
#endif

namespace BALL
{

	class BrukerParameter 
		: public File
	{
		public:

		/**	@name	Constructors and Destructors
		*/
		//@{
		/** Detailed constructor.
		*/
		BrukerParameter(const String& name, OpenMode open_mode = IN);

		/** Copy constructor.
		*/
		BrukerParameter(const BrukerParameter& file);

		/**	Destructor.
		*/
		~BrukerParameter();
		//@}

		/**
		 * Einlesen des Files.
		*/
		void read();

		/**
		 * Rückgabe des Titels.
		 */
		String title();

		/**
		 * Rückgabe des Parameters name
		 */
		double parameter( const String& name );

		/**
		 * Existiert Parameter "name"?
		 */
		bool has( const String& name );

		protected:
		bool nextLine_();

		String line_;
		String title_;
		StringHashMap<double> parameters_;

		static const Size		MAX_LENGTH_;
		char*								buffer_;
	};
}

#endif // BALL_FORMAT_JCAMPFILE_H

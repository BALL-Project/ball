// $Id: JCAMPFile.h,v 1.5 2000/11/28 17:25:04 anhi Exp $

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

	class JCAMPFile 
		: public File
	{
		public:

		/**	@name	Constructors and Destructors
		*/
		//@{
		/** Detailed constructor.
		*/
		JCAMPFile(const String& name, OpenMode open_mode = IN);

		/** Copy constructor.
		*/
		JCAMPFile(const JCAMPFile& file);

		/**	Destructor.
		*/
		~JCAMPFile();
		//@}

		/**
		 * Einlesen des Files.
		*/
		void read();

		/**
		 * Rückgabe des Titels.
		 */
		String title();

		/**	Return parameter name
		*/
		double parameter(const String& name) const;

		/**	Exists Parameter "name"?
		*/
		bool has(const String& name) const;

		protected:
		bool nextLine_();

		String								line_;
		String								title_;
		StringHashMap<double> parameters_;
		char*									buffer_;

		static const Size			MAX_LENGTH_;
	};
}

#endif // BALL_FORMAT_JCAMPFILE_H

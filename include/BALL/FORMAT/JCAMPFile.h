// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: JCAMPFile.h,v 1.8 2002/02/27 12:18:38 sturm Exp $

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

	/**	JCAMP file.
			This class....	\\
			{\bf Definition:}\URL{BALL/FORMAT/JCAMPFile.h}	\\
	*/
	class JCAMPFile 
		: public File
	{
		public:

		/**	@name	Constructors and Destructors
		*/
		//@{

		/** Detailed constructor.
		*/
		JCAMPFile(const String& name, OpenMode open_mode = IN)
			throw(Exception::FileNotFound);

		/** Copy constructor.
		*/
		JCAMPFile(const JCAMPFile& file)
			throw(Exception::FileNotFound);

		/**	Destructor.
		*/
		~JCAMPFile()
			throw();

		//@}

		/** Read the file.
		*/
		void read();

		/** Return file title.
		*/
		String title();

		/**	Return parameter name
		*/
		double parameter(const String& name) const;

		/**	Exists the parameter {\tt name}?
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

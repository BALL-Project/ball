// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id$
//


	class KCFFile
		: GenericMolFile
	{
%TypeHeaderCode
	#include <BALL/FORMAT/KCFFile.h>
	using namespace BALL;
	using namespace BALL::Exception;
%End
		public:

		static const char* ENTRY_TAG;
		static const char* NODE_TAG;
		static const char* EDGE_TAG;
		static const char* DELIMITER_TAG;
		static const char* CONTINUED_LINE;


		KCFFile() throw();
		KCFFile(const String& /filename/, OpenMode /open_mode/ = File::MODE_IN)
			throw(FileNotFound);
		KCFFile(const KCFFile& /file/) throw(FileNotFound);
		virtual ~KCFFile() throw();

		virtual bool write(const Molecule& /molecule/) throw(File::CannotWrite);
		virtual bool write(const System& /system/) throw(File::CannotWrite);
		virtual bool read(System& /system/) throw(ParseError);
		virtual Molecule* read() throw(ParseError);

		// const KCFFile& operator = (const KCFFile& file)throw();
	};
	

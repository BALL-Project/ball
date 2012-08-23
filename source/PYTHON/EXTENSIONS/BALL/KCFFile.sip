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
		KCFFile(const String&, OpenMode open_mode = File::MODE_IN)
			throw(FileNotFound);
		virtual ~KCFFile() throw();

		virtual bool write(const Molecule&) throw(File::CannotWrite);
		virtual bool write(const System&) throw(File::CannotWrite);
		virtual bool read(System&) throw(ParseError);
		virtual Molecule* read() throw(ParseError);

		private:
			KCFFile(const KCFFile&);
	};
	

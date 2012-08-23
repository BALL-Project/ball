// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id$
//

class LineBasedFile
	:  File
{
%TypeHeaderCode
	#include <BALL/FORMAT/lineBasedFile.h>	
	using namespace BALL;
	using namespace BALL::Exception;
%End
				public:

				LineBasedFile()
								throw();
				LineBasedFile(const String&, OpenMode open_mode = std::ios::in)
								throw(FileNotFound);
				void clear()
								throw();

				bool operator == (const LineBasedFile&) throw();
				bool operator != (const LineBasedFile&) throw();

				//const LineBasedFile& operator = (const LineBasedFile&)
				//				throw();
				Position getLineNumber()
								const throw();

				//const String& getLine() const throw();
				String& getLine()	throw();

				bool readLine()
								throw(ParseError);
				bool skipLines(Size number = 1)
								throw(ParseError);

				bool search(const String&, bool return_to_point = false)
								throw(ParseError);

				bool search(const String&, const String&, 
										bool return_to_point = false)
								throw(ParseError);
				bool gotoLine(Position)
								throw(ParseError);
				void rewind()
								throw(ParseError);

				void test(const char*, int, bool, const String&)
								const throw(ParseError);

				//String getField(Index pos = 0, const String& quotes = "",
				//								const String& delimiters = String::CHARACTER_CLASS__WHITESPACE)
				//				const throw(IndexUnderflow);

				bool startsWith(const String&)
								const throw();

				bool has(const String&)
								const throw();

				//Index switchString(const std::vector<String>&)
				//				const throw();

				// bool parseColumnFormat(const char*, Position, Size, void*);

			private:
				LineBasedFile(const LineBasedFile&);
};

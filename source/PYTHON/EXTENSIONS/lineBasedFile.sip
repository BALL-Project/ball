// $Id: lineBasedFile.sip,v 1.1 2001/12/20 01:13:03 oliver Exp $


class LineBasedFile
	:  File
{
%HeaderCode
	#include <BALL/FORMAT/lineBasedFile.h>	
	using namespace BALL;
	typedef Exception::FileNotFound 	FileNotFound;
	typedef Exception::IndexUndeflow 	IndexUnderflow;
	typedef Exception::ParseError 		ParseError;
%End
				public:

				LineBasedFile()
								throw();
				LineBasedFile(const String& /filename/, File::OpenMode /open_mode/ = File::IN)
								throw(FileNotFound);
				LineBasedFile(const LineBasedFile& /f/)
								throw(FileNotFound);
				void clear()
								throw();

				bool operator == (const LineBasedFile& /f/) throw();
				//bool operator != (const LineBasedFile& /f/) throw();

				//const LineBasedFile& operator = (const LineBasedFile& /file/)
				//				throw();
				Position getLineNumber()
								const throw();
				const String& getLine()
								const throw();
				String& getLine()
								throw();
				bool readLine()
								throw(ParseError);
				bool skipLines(Size /number/ = 1)
								throw(ParseError);

				bool search(const String& /text/, bool /return_to_point/ = false)
								throw(ParseError);

				bool search(const String& /text/, const String& /stop/, 
										bool /return_to_point/ = false)
								throw(ParseError);
				bool goToLine(Position /line_number/)
								throw(ParseError);
				void rewind()
								throw(ParseError);

				void test(const char* /file/, int /line/, bool /condition/, const String& /msg/)
								const throw(ParseError);

				String getField(Index /pos/ = 0, const String& /quotes/ = "",
												const String& /delimiters/ = String::CHARACTER_CLASS__WHITESPACE)
								const throw(IndexUnderflow);

				bool startsWith(const String& /text/)
								const throw();

				bool has(const String& /text/)
								const throw();

				//Index switchString(const std::vector<String>& /data/)
				//				const throw();

				// bool parseColumnFormat(const char* /format/, Position /index/, Size /length/, void* /arg/);
};

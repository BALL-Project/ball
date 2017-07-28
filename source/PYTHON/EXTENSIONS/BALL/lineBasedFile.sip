class LineBasedFile
	: File
{
%TypeHeaderCode
	#include <BALL/FORMAT/lineBasedFile.h>
%End
	public:

	LineBasedFile() throw();
	LineBasedFile(const String&, OpenMode open_mode = std::ios::in)
		throw(FileNotFound);
	void clear() throw();

	bool operator == (const LineBasedFile&) throw();
	bool operator != (const LineBasedFile&) throw();

	Position getLineNumber() const throw();
	String& getLine() throw();
	bool readLine() throw(ParseError);
	bool skipLines(Size number = 1) throw(ParseError);
	bool search(const String&, bool return_to_point = false) throw(ParseError);
	bool search(const String&, const String&, bool return_to_point = false)
		throw(ParseError);
	bool gotoLine(Position) throw(ParseError);
	void rewind() throw(ParseError);
	void test(const char*, int, bool, const String&) const throw(ParseError);
	bool startsWith(const String&) const throw();
	bool has(const String&) const throw();

	private:
	LineBasedFile(const LineBasedFile&);
};

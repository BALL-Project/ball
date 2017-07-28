class SmilesParser
{
%TypeHeaderCode
	#include <BALL/STRUCTURE/smilesParser.h>
%End

	public:
	typedef std::list<Position> ConnectionList;

	SmilesParser();
	SmilesParser(const SmilesParser&);
	~SmilesParser();
	void parse(const String&) throw(ParseError);
	System getSystem() const;
};

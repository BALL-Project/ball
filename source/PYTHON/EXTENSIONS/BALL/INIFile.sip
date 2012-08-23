// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id$
//

%ModuleCode
	#include <BALL/FORMAT/INIFile.h>
%End

class INIFile
{
%TypeHeaderCode
  #include <BALL/FORMAT/INIFile.h>
  using namespace BALL;
%End
	public:
	enum
	{
					MAX_LINE_LENGTH = 1024
	};


	static const String UNDEFINED;
	static const String HEADER;

	INIFile();
	INIFile(const String&);
	~INIFile();
	void clear();

	bool read();

	bool write();
	const String& getFilename() const;
	void setFilename(const String&);

	bool isValid() const;

	//LineIterator getLine(Size);

	//bool setLine(LineIterator line_it, const String& line);
	//bool deleteLine(LineIterator line_it);

	//bool insertLine(LineIterator line_it, const String& line);

	//bool appendLine(const String& section_name, const String& line);
	Size getNumberOfLines() const;

	bool hasSection(const String&) const;
	//Section_iterator getSection(const String& section_name);
	//Section_iterator getSection(Position pos);
	Size getNumberOfSections() const;

	//LineIterator getSectionFirstLine(const String&);

	//LineIterator getSectionLastLine(const String&);

	Size getSectionLength(const String&) const;
	bool deleteSection(const String&);
	bool appendSection(const String&);

	bool hasEntry(const String&, const String&) const;

	String getValue(const String&, const String&) const;

	bool insertValue(const String&, const String&, const String&);
	bool setValue(const String&, const String&, const String&);
	bool operator == (const INIFile&) const;
	// bool isValid(const LineIterator& it) const;
	// bool isValid(const Section_iterator& it) const;
	// bool apply(UnaryProcessor<LineIterator>& processor);
	void setDuplicateKeyCheck(bool);
};

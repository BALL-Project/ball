// $Id: INIFile.sip,v 1.3 2001/07/25 11:32:31 oliver Exp $

//class Section
//{
//	public:
//		friend class INIFile;
//	friend class INIFile::IteratorTraits_;
//	const String& getName() const
//	{
//					return name_;
//	}
//	bool operator == (const Section& section) const
//	{
//					return (name_ == section.name_ &&
//																					lines_ == section.lines_);
//	}
//};


class INIFile
{
%HeaderCode
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
				INIFile(const String& /filename/);
				~INIFile();
				void destroy();
				void clear();

				bool read();

				bool write();
				const String& getFilename() const;
				void setFilename(const String& /filename/);

				bool isValid() const;

				//LineIterator getLine(Size /line_number/);

				//bool setLine(LineIterator line_it, const String& line);
				//bool deleteLine(LineIterator line_it);

				//bool insertLine(LineIterator line_it, const String& line);

				//bool appendLine(const String& section_name, const String& line);
				Size getNumberOfLines() const;

				bool hasSection(const String& /section_name/) const;
				//Section_iterator getSection(const String& section_name);
				//Section_iterator getSection(Position pos);
				Size getNumberOfSections() const;

				//LineIterator getSectionFirstLine(const String& /section_name/);

				//LineIterator getSectionLastLine(const String& /section_name/);

				Size getSectionLength(const String& /section_name/) const;
				bool deleteSection(const String& /section/);
				bool appendSection(const String& /section/);

				bool hasEntry(const String& /section/, const String& /key/) const;

				String getValue(const String& /section/, const String& /key/) const;

				bool setValue(const String& /section/, const String& /key/, const String& /value/);
				bool operator == (const INIFile& /inifile/) const;
				// bool isValid(const LineIterator& it) const;
				// bool isValid(const Section_iterator& it) const;
				// bool apply(UnaryProcessor<LineIterator>& processor);
				void setDuplicateKeyCheck(bool /mode/);
				bool getDuplicateKeyCheck() const;
};

// $Id: INIFile.sip,v 1.2 2001/05/07 00:58:10 oliver Exp $

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
  static const String PREFIX;  
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
  const String* getLine(Size /line_number/) const;
  bool setLine(Size /line_number/, const String& /line/);
  bool deleteLine(Position /line_number/);
  bool insertLine(const String& /section_name/, const String& /line/);
  Size getOriginalNumberOfLines() const;
  Size getNumberOfLines() const;  
  bool hasSection(const String& /section_name/) const;
   String* getSectionName(Position /pos/);
  Size getNumberOfSections() const;
  Size getSectionFirstLine(const String& /section_name/) const;
  Size getSectionLastLine(const String& /section_name/) const; 
  Size getSectionLength(const String& /section_name/) const;
  bool hasEntry(const String& /section/, const String& /key/) const;
  String getValue(const String& /section/, const String& /key/) const;
  bool setValue(const String& /section/, const String& /key/, const String& /value/);
  bool operator == (const INIFile& /inifile/) const;
};

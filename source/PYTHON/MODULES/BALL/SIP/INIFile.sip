// $Id: INIFile.sip,v 1.1 2000/07/18 21:26:55 oliver Exp $
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
  String* getLine(Size /line_number/);
  bool setLine(Size /line_number/, const String& /line/);
  Size getNumberOfLines() const;
  bool hasSection(const String& /section_name/) const;
  Size getSectionFirstLine(const String& /section_name/) const;
  Size getSectionLastLine(const String& /section_name/) const; 
  Size getSectionLength(const String& /section_name/) const;
  bool hasEntry(const String& /section/, const String& /key/) const;
  String getValue(const String& /section/, const String& /key/) const;
  bool setValue(const String& /section/, const String& /key/, const String& /value/);
};

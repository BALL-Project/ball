// $Id: XYZFile.sip,v 1.2 2003/03/25 17:08:55 anhi Exp $

class XYZFile 
	: File
{
%HeaderCode
	#include <BALL/FORMAT/XYZFile.h>
	using namespace BALL;
%End
  public:
  XYZFile();
  XYZFile(const String&, File::OpenMode /openmode/ = std::ios::in)
		throw(FileNotFound);
  ~XYZFile()
		throw();
  virtual void write(const System&);
  virtual void read(System&);
};

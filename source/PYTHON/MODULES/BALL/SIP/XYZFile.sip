// $Id: XYZFile.sip,v 1.2 2001/08/01 01:28:38 oliver Exp $

class XYZFile 
	: File
{
%HeaderCode
	#include <BALL/FORMAT/XYZFile.h>
	using namespace BALL;
%End
  public:
  XYZFile();
  XYZFile(const String&, File::OpenMode /openmode/ = File::IN)
		throw(FileNotFound);
  ~XYZFile()
		throw();
  virtual void write(const System&);
  virtual void read(System&);
};

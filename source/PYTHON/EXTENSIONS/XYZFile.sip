// $Id: XYZFile.sip,v 1.1 2001/09/03 16:53:03 oliver Exp $

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

// $Id: XYZFile.sip,v 1.2 2000/07/12 19:35:14 oliver Exp $

class XYZFile 
	: File
{
%HeaderCode
	#include <BALL/FORMAT/XYZFile.h>
	using namespace BALL;
%End
  public:
  XYZFile();
  XYZFile(const String&, File::OpenMode /openmode/ = File::IN);
  //virtual ~XYZFile();
  virtual void write(const System&);
  virtual void read(System&);
};

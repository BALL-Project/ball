// $Id: MOL2File.sip,v 1.2 2001/08/01 01:28:38 oliver Exp $
class MOL2File
  : File
{
%HeaderCode
	#include <BALL/FORMAT/MOL2File.h>
	using namespace BALL;
%End
  public:
  MOL2File();
  MOL2File(const String&, File::OpenMode /openmode/ = File::IN)
		throw(FileNotFound);
  ~MOL2File()
		throw();
  virtual void write(const System&);
  virtual void read(System&);
};

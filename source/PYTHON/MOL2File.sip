// $Id: MOL2File.sip,v 1.2 2000/07/12 19:35:13 oliver Exp $
class MOL2File
  : File
{
%HeaderCode
	#include <BALL/FORMAT/MOL2File.h>
	using namespace BALL;
%End
  public:
  MOL2File();
  MOL2File(const String&, File::OpenMode /openmode/ = File::IN);
  // virtual ~MOL2File();
  virtual void write(const System&);
  virtual void read(System&);
};

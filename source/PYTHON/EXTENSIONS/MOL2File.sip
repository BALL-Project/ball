// $Id: MOL2File.sip,v 1.3 2003/03/25 17:08:53 anhi Exp $
class MOL2File
  : File
{
%HeaderCode
	#include <BALL/FORMAT/MOL2File.h>
	using namespace BALL;
	using namespace BALL::Exception;
%End
  public:
  MOL2File();
  MOL2File(const String&, File::OpenMode /openmode/ = std::ios::in)
		throw(FileNotFound);
  ~MOL2File()
		throw();
  virtual void write(const System& /system/);
  virtual bool read(System& /system/)
		throw(ParseError);
};

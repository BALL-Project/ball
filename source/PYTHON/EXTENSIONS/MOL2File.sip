// $Id: MOL2File.sip,v 1.4 2003/04/22 21:16:56 oliver Exp $
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
  MOL2File(const String&, File::OpenMode /openmode/ = File::IN)
		throw(FileNotFound);
  ~MOL2File()
		throw();
  virtual void write(const System& /system/);
  virtual bool read(System& /system/)
		throw(ParseError);
};

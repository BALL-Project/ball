// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: MOL2File.sip,v 1.7 2003/10/29 22:54:34 oliver Exp $
//

class MOL2File
  : File
{
%TypeHeaderCode
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

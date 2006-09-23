// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: XYZFile.sip,v 1.9.2.1 2006/09/23 17:45:29 amoll Exp $
//

class XYZFile 
	: File
{
%TypeHeaderCode
	#include <BALL/FORMAT/XYZFile.h>
  #include <BALL/COMMON/exception.h>
	using namespace BALL;
	using namespace BALL::Exception;
%End
  public:
  XYZFile();
  XYZFile(const String&, OpenMode /openmode/ = std::ios::in) throw(FileNotFound);
  ~XYZFile() throw();
  virtual bool write(const System&) throw(File::CannotWrite);
  virtual void read(System&);
};

// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: XYZFile.sip,v 1.7 2003/10/31 23:15:48 oliver Exp $
//

class XYZFile 
	: File
{
%TypeHeaderCode
	#include <BALL/FORMAT/XYZFile.h>
	using namespace BALL;
	typedef File::CannotWrite CannotWrite;
%End
  public:
  XYZFile();
  XYZFile(const String&, File::OpenMode /openmode/ = std::ios::in) throw(FileNotFound);
  ~XYZFile() throw();
  virtual bool write(const System&) throw(CannotWrite);
  virtual void read(System&);
};

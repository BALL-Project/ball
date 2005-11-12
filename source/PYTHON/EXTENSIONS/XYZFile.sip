// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: XYZFile.sip,v 1.8.6.1 2005/11/12 11:18:00 oliver Exp $
//

class XYZFile 
	: File
{
%TypeHeaderCode
	#include <BALL/FORMAT/XYZFile.h>
	using namespace BALL;
%End
  public:
  XYZFile();
  XYZFile(const String&, OpenMode /openmode/ = std::ios::in) throw(FileNotFound);
  ~XYZFile() throw();
  virtual bool write(const System&) throw(File::CannotWrite);
  virtual void read(System&);
};

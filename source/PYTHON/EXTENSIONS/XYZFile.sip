// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: XYZFile.sip,v 1.6 2003/10/29 22:54:35 oliver Exp $
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
  XYZFile(const String&, File::OpenMode /openmode/ = std::ios::in)
		throw(FileNotFound);
  ~XYZFile()
		throw();
  virtual void write(const System&);
  virtual void read(System&);
};

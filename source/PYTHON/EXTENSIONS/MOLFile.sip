// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: MOLFile.sip,v 1.6 2003/10/29 22:54:34 oliver Exp $
//

class MOLFile
	: GenericMolFile
{
%TypeHeaderCode
	#include <BALL/FORMAT/MOLFile.h>
	using namespace BALL;
%End
  public:
  MOLFile();
	// default open mode is "IN"
  MOLFile(String&, File::OpenMode /openmode/ = std::ios::in)
		throw(FileNotFound);
	~MOLFile()
		throw();

  virtual void write(const System& /system/);
  virtual bool read(System& /system/)
		throw(ParseError);
	virtual void write(const Molecule& /molecule/);
	virtual Molecule* read()
		throw(ParseError);
};

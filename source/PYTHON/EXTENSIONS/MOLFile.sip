// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: MOLFile.sip,v 1.8.8.1 2006/02/14 15:02:31 amoll Exp $
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
  MOLFile(String&, OpenMode /openmode/ = std::ios::in)
		throw(FileNotFound);
	~MOLFile() throw();

  virtual bool write(const System& /system/) throw(File::CannotWrite);
	virtual bool write(const Molecule& /molecule/) throw(File::CannotWrite);
  virtual bool read(System& /system/)	throw(ParseError);
	virtual Molecule* read() throw(ParseError);
};

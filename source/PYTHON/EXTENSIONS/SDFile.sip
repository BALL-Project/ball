// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: SDFile.sip,v 1.8 2003/10/31 23:15:48 oliver Exp $
//

class SDFile
	: GenericMolFile
{
%TypeHeaderCode
	#include <BALL/FORMAT/SDFile.h>
	using namespace BALL;
%End
  public:
  SDFile();
	// default open mode is "IN"
  SDFile(String&, File::OpenMode /openmode/ = std::ios::in)	throw(FileNotFound);
	~SDFile()	throw();

  virtual bool write(const System& /system/) throw(CannotWrite);
	virtual bool write(const Molecule& /molecule/) throw(CannotWrite);
  virtual bool read(System& /system/)	throw(ParseError);
	virtual Molecule* read() throw(ParseError);
	void disableAtoms() throw();
	void enableAtoms() throw();
};

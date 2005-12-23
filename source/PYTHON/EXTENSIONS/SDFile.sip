// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: SDFile.sip,v 1.10 2005/12/23 17:02:47 amoll Exp $
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
  SDFile(String&, OpenMode /openmode/ = std::ios::in)	throw(FileNotFound);
	~SDFile()	throw();

  virtual bool write(const System& /system/) throw(File::CannotWrite);
	virtual bool write(const Molecule& /molecule/) throw(File::CannotWrite);
  virtual bool read(System& /system/)	throw(ParseError);
	virtual Molecule* read() throw(ParseError);
	void disableAtoms() throw();
	void enableAtoms() throw();
};

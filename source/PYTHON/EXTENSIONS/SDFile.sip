// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: SDFile.sip,v 1.7 2003/10/29 22:54:35 oliver Exp $
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
  SDFile(String&, File::OpenMode /openmode/ = std::ios::in)
		throw(FileNotFound);
	~SDFile()
		throw();

  virtual void write(const System& /system/);
  virtual bool read(System& /system/)
		throw(ParseError);
	virtual void write(const Molecule& /molecule/);
	virtual Molecule* read()
		throw(ParseError);
	void disableAtoms() throw();
	void enableAtoms() throw();
};

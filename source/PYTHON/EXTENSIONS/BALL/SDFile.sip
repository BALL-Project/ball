// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id$
//

class SDFile
	: GenericMolFile
{
%TypeHeaderCode
	#include <BALL/FORMAT/SDFile.h>
  #include <BALL/COMMON/exception.h>
	using namespace BALL;
	using namespace BALL::Exception;
%End
  public:
  SDFile();
	// default open mode is "IN"
  SDFile(String&, OpenMode openmode = std::ios::in)	throw(FileNotFound);
	~SDFile()	throw();

  virtual bool write(const System&) throw(File::CannotWrite);
	virtual bool write(const Molecule&) throw(File::CannotWrite);
  virtual bool read(System&)	throw(ParseError);
	virtual Molecule* read() throw(ParseError);
	void disableAtoms() throw();
	void enableAtoms() throw();
};

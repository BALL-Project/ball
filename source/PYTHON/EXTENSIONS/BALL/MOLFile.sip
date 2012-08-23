// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id$
//

class MOLFile
	: GenericMolFile
{
%TypeHeaderCode
	#include <BALL/FORMAT/MOLFile.h>
	using namespace BALL;
	using namespace BALL::Exception;
%End
  public:
  MOLFile();
	// default open mode is "IN"
  MOLFile(String&, OpenMode openmode = std::ios::in)
		throw(FileNotFound);
	~MOLFile() throw();

  virtual bool write(const System&) throw(File::CannotWrite);
	virtual bool write(const Molecule&) throw(File::CannotWrite);
  virtual bool read(System&)	throw(ParseError);
	virtual Molecule* read() throw(ParseError);
};

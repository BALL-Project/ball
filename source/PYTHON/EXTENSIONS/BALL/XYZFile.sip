// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id$
//

class XYZFile
	: GenericMolFile
{
%TypeHeaderCode
	#include <BALL/FORMAT/XYZFile.h>
  #include <BALL/COMMON/exception.h>
	using namespace BALL;
	using namespace BALL::Exception;
%End
  public:
  XYZFile();
  XYZFile(const String&, OpenMode /openmode/ = std::ios::in) throw(FileNotFound);
  ~XYZFile() throw();
  virtual bool write(const System&) throw(File::CannotWrite);
	virtual bool write(const Molecule& /molecule/) throw(File::CannotWrite);
  virtual bool read(System&) throw(ParseError);
	virtual Molecule* read() throw(ParseError);
	const String& getComment();
	private:
		XYZFile(const XYZFile&);
};

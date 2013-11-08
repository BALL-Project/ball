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
  XYZFile(const String&, OpenMode openmode = std::ios::in);
  ~XYZFile();
  virtual bool write(const System&);
	virtual bool write(const Molecule&);
  virtual bool read(System&);
	virtual Molecule* read();
	const String& getComment();
	private:
		XYZFile(const XYZFile&);
};

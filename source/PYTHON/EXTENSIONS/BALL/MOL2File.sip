// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id$
//

class MOL2File
  : GenericMolFile
{
%TypeHeaderCode
	#include <BALL/FORMAT/MOL2File.h>
	using namespace BALL;
	using namespace BALL::Exception;
%End
  public:
  MOL2File();
  MOL2File(const String&, OpenMode openmode = std::ios::in);
  ~MOL2File();
  virtual bool write(const System&);
  virtual bool write(const Molecule&);
  virtual bool read(System&);
  virtual Molecule* read();
};

// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: HINFile.sip,v 1.11 2003/10/31 23:15:48 oliver Exp $
//

class HINFile
	: GenericMolFile
{
%TypeHeaderCode
	#include <BALL/FORMAT/HINFile.h>
	using namespace BALL;
%End
  public:
  HINFile();
	// default open mode is "IN"
  HINFile(String&, File::OpenMode /openmode/ = std::ios::in)
		throw(FileNotFound);
	~HINFile()
		throw();

  virtual bool write(const System& /system/) throw(CannotWrite);
	virtual bool write(const Molecule& /molecule/) throw(CannotWrite);
  virtual bool read(System& /system/)	throw(ParseError);
	virtual Molecule* read() throw(ParseError);

  bool hasPeriodicBoundary() const;
  Box3 getPeriodicBoundary() const;
  float getTemperature() const;
};

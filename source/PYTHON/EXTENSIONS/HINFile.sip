// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: HINFile.sip,v 1.10 2003/10/29 22:54:34 oliver Exp $
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

  virtual void write(const System& /system/);
  virtual bool read(System& /system/)
		throw(ParseError);
	virtual void write(const Molecule& /molecule/);
	virtual Molecule* read()
		throw(ParseError);

  bool hasPeriodicBoundary() const;
  Box3 getPeriodicBoundary() const;
  float getTemperature() const;
};

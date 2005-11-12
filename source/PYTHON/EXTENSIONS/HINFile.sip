// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: HINFile.sip,v 1.13.6.1 2005/11/12 11:17:58 oliver Exp $
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
  HINFile(String&, OpenMode /openmode/ = std::ios::in) throw(FileNotFound);
	~HINFile() throw();

  virtual bool write(const System& /system/) throw(File::CannotWrite);
	virtual bool write(const Molecule& /molecule/) throw(File::CannotWrite);
  virtual bool read(System& /system/)	throw(ParseError);
	virtual Molecule* read() throw(ParseError);

  bool hasPeriodicBoundary() const;
  SimpleBox3 getPeriodicBoundary() const;
  float getTemperature() const;
};

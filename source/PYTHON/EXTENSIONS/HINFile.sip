// $Id: HINFile.sip,v 1.6 2003/03/25 17:08:53 anhi Exp $

class HINFile
	: GenericMolFile
{
%HeaderCode
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

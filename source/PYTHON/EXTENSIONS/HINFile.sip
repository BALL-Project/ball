// $Id: HINFile.sip,v 1.4 2001/12/21 11:47:04 oliver Exp $

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
  HINFile(String&, File::OpenMode /openmode/ = File::IN)
		throw(FileNotFound);
	~HINFile()
		throw();

  virtual void write(System&);
  virtual bool read(System&)
		throw(ParseError);
	virtual void write(const Molecule& /molecule);
	virtual Molecule* read()
		throw(ParseError);

  bool hasPeriodicBoundary() const;
  Box3 getPeriodicBoundary() const;
  float getTemperature() const;
};

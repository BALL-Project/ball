// $Id: HINFile.sip,v 1.3 2001/12/20 01:20:41 oliver Exp $

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
  virtual HINFile& operator >> (System& /system/)
		throw(ParseError);
  virtual HINFile& operator << (const System& /system/);
  bool hasPeriodicBoundary() const;
  Box3 getPeriodicBoundary() const;
  float getTemperature() const;
};

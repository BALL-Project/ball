// $Id: HINFile.sip,v 1.2 2001/12/20 01:13:02 oliver Exp $

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
  virtual bool read(System&);
  virtual HINFile& operator >> (System& /system/);
  virtual HINFile& operator << (const System& /system/);
  bool hasPeriodicBoundary() const;
  Box3 getPeriodicBoundary() const;
  float getTemperature() const;
};

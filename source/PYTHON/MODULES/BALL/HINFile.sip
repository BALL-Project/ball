// $Id: HINFile.sip,v 1.1 2000/07/18 17:01:58 oliver Exp $

class HINFile
	: File
{
%HeaderCode
	#include <BALL/FORMAT/HINFile.h>
	using namespace BALL;
%End
  public:
  HINFile();
	// default open mode is "IN"
  HINFile(String&, File::OpenMode /openmode/ = File::IN);

  virtual void write(System&);
  virtual void read(System&);
  // virtual HINFile& operator >> (System& system);
  // virtual HINFile& operator << (const System& system);
  bool hasPeriodicBoundary() const;
  Box3 getPeriodicBoundary() const;
  float getTemperature() const;
};

// $Id: HINFile.sip,v 1.1 2001/09/03 16:53:02 oliver Exp $

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
  HINFile(String&, File::OpenMode /openmode/ = File::IN)
		throw(FileNotFound);
	~HINFile()
		throw();

  virtual void write(System&);
  virtual void read(System&);
  virtual HINFile& operator >> (System& /system/);
  virtual HINFile& operator << (System& /system/);
  bool hasPeriodicBoundary() const;
  Box3 getPeriodicBoundary() const;
  float getTemperature() const;
};

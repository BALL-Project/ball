class HINFile
	: File
{
%HeaderCode
#include <BALL/FORMAT/HINFile.h>
using namespace BALL;
%End
  public:
  HINFile();
	// default open mode is "READ"
  HINFile(String&, int = 1);

  virtual void write(System&);
  virtual void read(System&);
  // virtual HINFile& operator >> (System& system);
  // virtual HINFile& operator << (const System& system);
  bool hasPeriodicBoundary() const;
  Box3 getPeriodicBoundary() const;
  float getTemperature() const;
};

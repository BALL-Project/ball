class HINFile
{
%HeaderCode
#include <BALL/FORMAT/HINFile.h>
using namespace BALL;
%End
  public:
  HINFile();
  HINFile(String&, int);

  virtual void write(System&);
  virtual void read(System&);
  // virtual HINFile& operator >> (System& system);
  // virtual HINFile& operator << (const System& system);
  bool hasPeriodicBoundary() const;
  Box3 getPeriodicBoundary() const;
  Box3 getTemperature() const;
};

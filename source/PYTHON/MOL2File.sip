class MOL2File
  : File
{
%HeaderCode
#include <BALL/FORMAT/MOL2File.h>
using namespace BALL;
%End
  public:
  MOL2File();
  MOL2File(const String&, int = 1);
  // virtual ~MOL2File();
  virtual void write(const System&);
  virtual void read(System&);
};

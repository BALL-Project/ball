class XYZFile 
	: File
{
%HeaderCode
#include <BALL/FORMAT/XYZFile.h>
using namespace BALL;
%End
  public:
  XYZFile();
  XYZFile(const String&, int = 1);
  //virtual ~XYZFile();
  virtual void write(const System&);
  virtual void read(System&);
};

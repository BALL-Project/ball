
 class PDBFile
 {
%HeaderCode
#include <BALL/FORMAT/PDBFile.h>
using namespace BALL;
%End
  public:
  PDBFile();
  PDBFile(const String&, int);
	//  virtual ~PDBFile();
  virtual void read(Protein&);
  virtual void read(System&);
  virtual void write(const Protein&);
  virtual void write(const System&);
 };

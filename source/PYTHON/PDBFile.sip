class PDBFile
 {
%HeaderCode
#include <BALL/FORMAT/PDBFile.h>
using namespace BALL;
%End
  public:
  PDBFile();
	// default open mode: READ
  PDBFile(const String&, int = 1);
	//  virtual ~PDBFile();
  virtual void read(Protein&);
  virtual void read(System&);
  virtual void write(const Protein&);
  virtual void write(const System&);
 };
